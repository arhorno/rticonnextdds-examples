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
#include <rti/core/Exception.hpp>
#include <rti/topic/to_string.hpp>

using namespace rti::routing;
using namespace rti::routing::adapter;
using namespace rti::community::examples;

const std::string FileStreamWriter::WRITE_MODE_PROPERTY_NAME =
        "example.adapter.write_mode";
const std::string FileStreamWriter::FLUSH_PROPERTY_NAME =
        "example.adapter.flush";

FileStreamWriter::FileStreamWriter(
        const PropertySet& properties,
        const StreamInfo& info,
        std::string folder_path)
{
    std::string file_path = folder_path + '/' + info.stream_name();
    output_file_.open(file_path);
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
        output_file_.flush();
    }
    return 0;
}

FileStreamWriter::~FileStreamWriter()
{
    output_file_.close();
}
