# Example Code: Routing Service C++11 Adapter

## Example Description

This example shows how to create a simple adapter using the RTI Routing Service Adapter SDK in C++11. The adapter scans the file system for one specific folder defined in the XML configuration file. For every file present in that folder, it creates a stream to communicate with the output. The data that will flow in this stream are the lines inside the files. In summary, we have a stream for every file in the specified folder, and every stream transports on the other side the text lines contained inside the specific file. In the output, the adapter will take care of creating a file for every stream received. The files will be created on a specific path, which you can also configure in the XML configuration file.

The code in this directory provides the following components:

-   `FileAdapter` implements the plugin that is loaded by *RTI Routing Service*.
It is responsible to create and delete connections.
-   `FileConnection` implements a connection. This component is responsible of the
creation and deletion of `StreamReaders` and `StreamWriters`.
-   `FileInputDiscoveryStreamReader` implements the logic necessary to propagate
information about the discovered input streams (in this case files) to the
Routing Service. It scans a directory looking for files to read from. We do not have a `FileOutputDiscoveryStreamReader` since we
directly write to a file with the same name in the output directory specified. However, it can be implemented in a
very similar vein to the `FileInputDiscoveryStreamReader`.
-   `FileStreamReader` implements an `StreamReader` that reads sample information
from a file.
-   `FileStreamWriter` implements an `StreamWriter` that writes sample information
to a file.

For more details, please refer to the *RTI Routing Service SDK* documentation.

## Building C++ example

In order to build this example, you need to provide the following variables to
`CMake`:

- `CONNEXTDDS_DIR`
- `CONNEXTDDS_ARCH`

```bash
$ cmake -B build \
    -DCONNEXTDDS_DIR=<Connext DDS Directory> \
    -DCONNEXTDDS_ARCH=<Connext DDS Architecture> \
    -DBUILD_SHARED_LIBS=ON|OFF \
    -DCMAKE_BUILD_TYPE=Debug|Release .
$ cmake --build build
```

**Note**: Since we use POSIX functions for scanning. this example does not work on windows.

**Note:** If you are using a multi-configuration generator, such as Visual Studio
Solutions, you can specify the configuration mode to build as follows:

```bash
$ cmake --build build --config Release|Debug
```

Here is more information about generating
[Visual Studio Solutions for Windows using CMake](https://cmake.org/cmake/help/v3.16/generator/Visual%20Studio%2016%202019.html#platform-selection).

**Note:** The `BUILD_SHARED_LIBS` allows you to control if the generated library
for this example is a static or a dynamic shared library. The following sections
assume you are building a dynamic shared library. However Routing Service also
supports static linking of adapters. To use this functionality you would need to
create an application that uses Routing Service as a library component and
statically links to this `FileAdapter` library.

## Running C++ example

Before running the example, take a look at file_bridge.xml. It defines the different settings to load and configure the adapter. Among other things it also includes two environment variables that allow you to easily specify the input and output directories, namely:

- INPUT_DIRECTORY
- OUTPUT_DIRECTORY
They indicate the path to the input directory that will be scanned to check for files, as well as the path to the output directory where all the discovered files will be copied over, respectively.

Note: The following instructions assume you are using bash shell on a UNIX-like system. For other shells like tcsh adapting shouldn't be very difficult.


Before running the RTI Routing Service, you need to specify where the
`fileadapter` library is located as shown below:

To run the example, you just need to run the following command from the `build`
folder (where the adapter plugin library has been created).

```bash
$<NDDSHOME>/bin/rtiroutingservice -cfgFile RsFileAdapter.xml -cfgName FileAdapterToFileAdapter
```
The general behavior of this example, can be controlled using properties and the
```INPUT_DIRECTORY``` and ```OUTPUT_DIRECTORY``` environment variable. You can modify properties in the Routing
Service configuration file. This is the list of available properties:

| Property                                | Tag           | Description                                                                                   |
| --------------------------------------- | ------------- | ----------------------------------------------------------------------------------------------|
| `example.adapter.direction`             | `<connection>`| Wether to use the adapter to read from or write to a directory. Valid options are "```Input```" and "```Output```" |
| `example.adapter.folder_path`           | `<connection>`| Path to the input or output directory|
| `example.adapter.discovery_sleep_period`| `<connection>`| Number of seconds to sleep between scans in the input folder.
|`example.adapter.read_period`            | `<input>`     | Periodic rate of reading samples from the file
| `example.adapter.samples_per_read`      | `<input>`     | How many samples read in each iteration.
| `example.adapter.write_mode`            | `<output>`    | The write mode can be:<ul><li>overwrite (write in the file deleting previous content)</li><li>append (write at then end of the file)</li><li>keep (fail if the file already exists and don't modify it)</li></ul>|
| `example.adapter.flush`       | `<output>` | Wether to flush the stream every time the adapter writes a sample.|

## Requirements

To run this example you will need:

- RTI Connext Professional version 6.0.0 or higher.
- CMake version 3.10 or higher
- A target platform with support for RTI Routing Service and C++11.
