/*
 * (c) 2019 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 *
 * RTI grants Licensee a license to use, modify, compile, and create derivative
 * works of the Software.  Licensee has the right to distribute object form
 * only for use with RTI products.  The Software is provided "as is", with no
 * warranty of any type, including any warranty for fitness for any purpose.
 * RTI is under no obligation to maintain or support the Software.  RTI shall
 * not be liable for any incidental or consequential damages arising out of the
 * use or inability to use the software.
 */

#include <algorithm>
#include <cctype>
#include <sstream>
#include <thread>

#include "FileStreamReader.hpp"
#include "Properties.hpp"
#include <rti/core/Exception.hpp>
#include <rti/topic/to_string.hpp>

using namespace dds::core::xtypes;
using namespace rti::routing;
using namespace rti::routing::adapter;
using namespace rti::community::examples;

bool FileStreamReader::is_digit(const std::string& value)
{
    return std::find_if(
                   value.begin(),
                   value.end(),
                   [](unsigned char c) { return !std::isdigit(c); })
            == value.end();
}

void FileStreamReader::file_reading_thread()
{
    while (!stop_thread_) {
        if (input_file_stream_.is_open()) {
            /**
             * Here we notify Routing Service, that there is data available
             * on the StreamReader, triggering a call to take().
             */
            if (!input_file_stream_.eof()) {
                reader_listener_->on_data_available(this);
            } else {
                stop_thread_ = true;
            }
        }

        std::this_thread::sleep_for(sampling_period_);
    }
    std::cout << "Reached end of stream for file: " << input_file_name_
              << std::endl;
    file_connection_->dispose_discovery_stream(stream_info_);
}

FileStreamReader::FileStreamReader(
        FileConnection *connection,
        const StreamInfo& info,
        const PropertySet& properties,
        StreamReaderListener *listener,
        std::string folder_path)
        : sampling_period_(1),
          stop_thread_(false),
          stream_info_(info),
          buffer_size_(1024)
{
    std::cout << "FileStreamReader created" << std::endl;
    file_connection_ = connection;
    reader_listener_ = listener;


    // Parse the properties provided in the xml configuration file
    for (const auto& property : properties) {
        if (property.first == READ_PERIOD_PROPERTY_NAME) {
            sampling_period_ = std::chrono::seconds(std::stoi(property.second));
        } else if (property.first == SAMPLES_PER_READ_PROPERTY_NAME) {
            samples_per_read_ = std::stoi(property.second);
        }
    }

    input_file_name_ = folder_path + '/' + info.stream_name();
    input_file_stream_.open(input_file_name_);

    if (!input_file_stream_.is_open()) {
        throw dds::core::IllegalOperationError(
                "Error opening input file: " + input_file_name_);
    } else {
        std::cout << "Input file name: " << input_file_name_ << std::endl;
    }

    filereader_thread_ =
            std::thread(&FileStreamReader::file_reading_thread, this);
}

void FileStreamReader::take(
        std::vector<dds::core::xtypes::DynamicData *>& samples,
        std::vector<dds::sub::SampleInfo *>& infos)
{
    /**
     * Note that we read samples_per_read_ lines at a time
     * from the CSV file in the
     * function file_reading_thread()
     */
    samples.resize(samples_per_read_);

    /* We do not use sample info.
     * TODO: docs say you have to leave this vector
     * untouched if not using sampleInfo*/
    infos.resize(samples_per_read_);

    for (size_t i = 0; i < samples_per_read_; i++) {
        std::unique_ptr<DynamicData> sample(
                new DynamicData(stream_info_.type_info().dynamic_type()));

        if (!input_file_stream_.eof()) {
            std::string read_str;
            std::getline(input_file_stream_, read_str);
            dds::core::ByteSeq buff(read_str.begin(), read_str.end());
            sample->set_values("value", buff);
        }
        samples[i] = sample.release();
    }


    return;
}

void FileStreamReader::take(
        std::vector<dds::core::xtypes::DynamicData *>& samples,
        std::vector<dds::sub::SampleInfo *>& infos,
        const SelectorState& selector_state)
{
    take(samples, infos);
}

void FileStreamReader::return_loan(
        std::vector<dds::core::xtypes::DynamicData *>& samples,
        std::vector<dds::sub::SampleInfo *>& infos)
{
    for (size_t i = 0; i < samples.size(); ++i) {
        delete samples[i];
        delete infos[i];
    }
    samples.clear();
    infos.clear();
}

void FileStreamReader::shutdown_file_reader_thread()
{
    stop_thread_ = true;
    filereader_thread_.join();
}

FileStreamReader::~FileStreamReader()
{
    input_file_stream_.close();
}
