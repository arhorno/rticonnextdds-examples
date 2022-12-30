# Example Code: Routing Service File Adapter

## Example Description

This example shows how to create a simple adapter using the RTI Routing Service
Adapter SDK in C. The adapter scans the file system for one specific folder
defined in the XML configuration file. For every file present in that folder,
it creates a stream to communicate with the output. The data that will flow in
this stream are the lines inside the files. In summary, we have a stream for
every file in the specified folder, and every stream transports on the other
side the text lines contained inside the specific file. In the output, the
adapter will take care of creating a file for every stream received. The files
will be created on a specific path, which you can also configure in the XML
configuration file.

## Building C example

In order to build this example, you need to provide the following variables to
`CMake`:

- `CONNEXTDDS_DIR`
- `CONNEXTDDS_ARCH` (needed only if you have multiple architectures installed)

<!-- Here cd build is missing -->
```bash
$mkdir build
$cmake -DCONNEXTDDS_DIR=<Connext DDS Directory>
    -DCONNEXTDDS_ARCH=<Connext DDS Architecture>
    -DBUILD_SHARED_LIBS=ON|OFF
    -DCMAKE_BUILD_TYPE=Debug|Release ..
cmake --build .
```

**Note:** Since this example uses the `pthread` library, it only works on
UNIX-like systems. It has been successfully tested on Ubuntu and macOS.

**Note:** The `BUILD_SHARED_LIBS` allows you to control if the generated library
for this example is a static or a dynamic shared library. The following sections
assume you are building a dynamic shared library. However Routing Service also
supports static linking of adapters. To use this functionality you would need to
create an application that uses Routing Service as a library component and
statically links to this `FileAdapter` library.

## Running C Example

Before running the example, take a look at file_bridge.xml. It defines the
different settings to load and configure the adapter. Among other things it
also includes two environment variables that allow you to easily specify the
input and output directories, namely:

- `INPUT_DIRECTORY`
- `OUTPUT_DIRECTORY`

They indicate the path to the input directory that will be scanned to check for
files, as well as the path to the output directory where all the discovered
files will be copied over, respectively.

**Note:** The following instructions assume you are using `bash` shell on a
UNIX-like system. For other shells like `tcsh` adapting shouldn't be very
difficult.

Here is how you would set the environment variables:

```bash
$export INPUT_DIRECTORY=<Input Directory>
$export OUTPUT_DIRECTORY=<Output Directory>
```

We also advise setting path to your installation of RTI Connext DDS in the
variable `NDDSHOME` as shown below:

```bash
$export NDDSHOME=<RTI Connext DDS Directory>
```

Before running the RTI Routing Service, you also need to specify where the
`fileadapter` library is located as shown below:

```bash
$export RTI_LD_LIBRARY_PATH=<Path to CMake build folder>
```

Now we can run the RTI Routing Service to copy over the files from
`INPUT_DIRECTORY` to `OUTPUT_DIRECTORY`:

```bash
# From within the /build folder
$<NDDSHOME>/bin/rtiroutingservice -cfgFile file_bridge.xml -cfgName file_to_file
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
| `example.adapter.write_mode`            | `<output>`    | The write mode can be:<ul><li>```overwrite``` (write in the file deleting previous content)</li><li>```append``` (write at then end of the file)</li><li>```keep``` (fail if the file already exists and don't modify it)</li></ul>|
| `example.adapter.flush`       | `<output>` | Wether to flush the stream every time the adapter writes a sample.|
