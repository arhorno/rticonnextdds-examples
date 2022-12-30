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

#include <iostream>

/* Adapter plugin configuration property names */
const std::string FOLDER_PATH_PROPERTY_NAME = "example.adapter.folder_path";
const std::string DISCOVERY_SLEEP_PROPERTY_NAME =
        "example.adapter.discovery_sleep_period";
const std::string DIRECTION_PROPERTY_NAME = "example.adapter.direction";
const std::string DIRECTION_INPUT = "Input";
const std::string DIRECTION_OUTPUT = "Output";

const std::string READ_PERIOD_PROPERTY_NAME = "example.adapter.read_period";
const std::string SAMPLES_PER_READ_PROPERTY_NAME =
        "example.adapter.samples_per_read";

const std::string WRITE_MODE_PROPERTY_NAME = "example.adapter.write_mode";
const std::string WRITE_MODE_OVERWRITE = "overwrite";
const std::string WRITE_MODE_APPEND = "append";
const std::string WRITE_MODE_KEEP = "keep";
const std::string FLUSH_PROPERTY_NAME = "example.adapter.flush";