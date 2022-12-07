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

#include "FileInputDiscoveryStreamReader.hpp"

using namespace rti::routing;
using namespace rti::routing::adapter;
using namespace rti::community::examples;


const std::string FileInputDiscoveryStreamReader::INPUT_DIR_PROPERTY_NAME =
        "example.adapter.folder_path";
const std::string FileInputDiscoveryStreamReader::INPUT_DIR_PROPERTY_NAME =
        "example.adapter.discovery_sleep_period";

bool FileInputDiscoveryStreamReader::fexists(const std::string filename)
{
    std::ifstream input_file;
    input_file.open(filename);
    return input_file.is_open();
}

FileInputDiscoveryStreamReader::FileInputDiscoveryStreamReader(
        const PropertySet& properties,
        StreamReaderListener *input_stream_discovery_listener,
        dds::core::xtypes::DynamicType stream_type)
        : stream_type(stream_type),
          is_running_enabled_(true),
          discovery_sleep_period_(5)
{
    input_stream_discovery_listener_ = input_stream_discovery_listener;

    std::string input_dir_name;
    for (auto& property : properties) {
        if (property.first == INPUT_DIR_PROPERTY_NAME) {
            input_dir_name = property.second;
        } else if (property.first == DISCOVERY_SLEEP_PROPERTY_NAME) {
            discovery_sleep_period_ =
                    std::chrono::seconds(stoi(property.second));
        }
    }

    /**
     * Start the thread that is going to be executing periodic scans on the
     * directory
     */
    disc_thread_ = std::thread(
            &FileInputDiscoveryStreamReader::discovery_thread,
            this,
            input_dir_name,
            input_stream_discovery_listener);
}

FileInputDiscoveryStreamReader::~FileInputDiscoveryStreamReader()
{
    is_running_enabled_ = false;
    disc_thread_.join();
}

void FileInputDiscoveryStreamReader::dispose(
        const rti::routing::StreamInfo& stream_info)
{
    /**
     * This guard is essential since the take() and return_loan() operations
     * triggered by calling on_data_available() execute on an internal Routing
     * Service thread. The custom dispose() operation doesn't run on that
     * thread. Since the take() and return_loan() operations also need to access
     * the data_samples_ list this protection is required.
     */
    std::lock_guard<std::mutex> guard(data_samples_mutex_);

    std::unique_ptr<rti::routing::StreamInfo> stream_info_disposed(
            new StreamInfo(
                    stream_info.stream_name(),
                    stream_info.type_info().type_name()));
    stream_info_disposed.get()->disposed(true);

    this->data_samples_.push_back(std::move(stream_info_disposed));
    input_stream_discovery_listener_->on_data_available(this);
}

void FileInputDiscoveryStreamReader::take(
        std::vector<rti::routing::StreamInfo *>& stream)
{
    /**
     * This guard is essential since the take() and return_loan() operations
     * triggered by calling on_data_available() execute on an internal Routing
     * Service thread. The custom dispose() operation doesn't run on that
     * thread. Since the take() and return_loan() operations also need to access
     * the data_samples_ list this protection is required.
     */
    std::lock_guard<std::mutex> guard(data_samples_mutex_);
    std::transform(
            data_samples_.begin(),
            data_samples_.end(),
            std::back_inserter(stream),
            [](const std::unique_ptr<rti::routing::StreamInfo>& element) {
                return element.get();
            });
}

void FileInputDiscoveryStreamReader::return_loan(
        std::vector<rti::routing::StreamInfo *>& stream)
{
    /**
     * This guard is essential since the take() and return_loan() operations
     * triggered by calling on_data_available() execute on an internal Routing
     * Service thread. The custom dispose() operation doesn't run on that
     * thread. Since the take() and return_loan() operations also need to access
     * the data_samples_ list this protection is required.
     */
    std::lock_guard<std::mutex> guard(data_samples_mutex_);

    /**
     * For discovery streams there will never be any outstanding return_loan().
     * Thus we can be sure that each take() will be followed by a call to
     * return_loan(), before the next take() executes.
     */
    this->data_samples_.erase(
            data_samples_.begin(),
            data_samples_.begin() + stream.size());
    stream.clear();
}

void FileInputDiscoveryStreamReader::discovery_thread(
        std::string input_directory,
        rti::routing::adapter::StreamReaderListener *listener)
{
    /* NOTE: In C++17 a std::filesystem library is available */

    /* Open directory */
    DIR *directory = opendir(input_directory.c_str());

    if (directory == NULL) {
        std::cerr << "Error opening input directory\n";
        return;
    }

    /* We just need the name of the discovered files*/
    std::set<std::string> discovered_files;

    while (is_running_enabled_) {
        struct dirent *file;
        while ((file = readdir(directory)) != NULL) {
            if (file->d_type == DT_REG && file->d_name[0] != '.'
                && discovered_files.emplace(std::string(file->d_name)).second) {
                /*
                 * New file discovered, we add it to the data_samples_ vector
                 * and call on_data_available()
                 */

                rti::routing::StreamInfo *infoSample(
                        new StreamInfo("TextLineStream", "TextLine"));

                /* If we don't define the type here, we have to define it in
                 * xml and register it in the <connection tag>*/
                infoSample->type_info().dynamic_type(&stream_type);

                this->data_samples_.push_back(
                        std::unique_ptr<rti::routing::StreamInfo>(infoSample));

                listener->on_data_available(this);
                std::cout << "discovered new file: " << file->d_name
                          << std::endl;
            }
        }

        std::this_thread::sleep_for(discovery_sleep_period_);
    }
}