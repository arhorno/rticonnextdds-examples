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

#include "FileStreamWriter.hpp"
#include "Properties.hpp"
#include <rti/core/Exception.hpp>
#include <rti/topic/to_string.hpp>

using namespace rti::routing;
using namespace rti::routing::adapter;
using namespace rti::community::examples;

FileStreamWriter::FileStreamWriter(
        const PropertySet& properties,
        const StreamInfo& info,
        std::string folder_path)
{
    char write_mode = 'o';
    flush_ = true;
    for (const auto& Property : properties) {
        if (Property.first == WRITE_MODE_PROPERTY_NAME) {
            if (Property.second == WRITE_MODE_APPEND) {
                write_mode = 'a';
            } else if (Property.second == WRITE_MODE_KEEP) {
                write_mode = 'k';
            }
        } else if (Property.first == FLUSH_PROPERTY_NAME) {
            flush_ = Property.second == "true" || Property.second == "1";
        }
    }

    std::string file_path = folder_path + '/' + info.stream_name();


    if (write_mode == 'o') {
        output_file_.open(file_path, std::ofstream::trunc);
    } else if (write_mode == 'a') {
        output_file_.open(file_path, std::ofstream::app);
    } else {
        /* If the open mode is keep, we need to open in read mode, if opened,
         * the file exist*/
        std::ifstream in_stream;
        in_stream.open(file_path);
        if (in_stream.is_open()) {
            in_stream.close();
            throw dds::core::IllegalOperationError(
                    "Error opening output file: " + file_path
                    + ": The file exist and write mode is \"keep\"");
        }
    }


    if (!output_file_.is_open()) {
        throw dds::core::IllegalOperationError(
                "Error opening output file: " + file_path);
    } else {
        std::cout << "Output file name: " << file_path << std::endl;
    }
}

int FileStreamWriter::write(
        const std::vector<dds::core::xtypes::DynamicData *>& samples,
        const std::vector<dds::sub::SampleInfo *>& infos)
{
    for (auto sample : samples) {
        std::vector<uint8_t> buff =
                sample->get_values<uint8_t>(std::string("value"));

        for (auto character : buff) {
            output_file_ << character;
        }
        output_file_ << '\n';
        if (flush_) {
            output_file_.flush();
        }
    }
    return 0;
}

FileStreamWriter::~FileStreamWriter()
{
    output_file_.close();
}
