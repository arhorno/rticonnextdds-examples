# Example Code: Routing Service File Adapter

## Example Description
This example shows how to create a simple adapter for the ShapeType using the RTI Routing Service Adapter SDK in C. The adapter creates a stream reader/writer to use it as an input/output for routing service. The file to read/write and other options can be configured as properties in the xml configuration file.
The format used to read or write in files is CSV, where each line contains the fields of a single sample.
## Building C example

In order to build this example, you need to provide the following variables to
`CMake`:

- `CONNEXTDDS_DIR`
- `CONNEXTDDS_ARCH` (needed only if you have multiple architectures installed)

```bash
cmake -DCONNEXTDDS_DIR=<Connext DDS Directory> \
    -DCONNEXTDDS_ARCH=<Connext DDS Architecture> \
    -DBUILD_SHARED_LIBS=ON|OFF \
    -DCMAKE_BUILD_TYPE=Debug|Release .
cmake --build build
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

**Note:** The following instructions assume you are using `bash` shell on a
UNIX-like system. For other shells like `tcsh` adapting shouldn't be very
difficult.

Before running the example, take a look at file_bridge.xml. It defines the
different settings to load and configure the adapter.

To run the example, you just need to run the following command from the `build`
folder (where the adapter plugin library has been created).

This example has been written to allow easy experimentation with the Shapes Demo
shipped with *RTI Connext DDS* installer bundle. We use the environment variable
```SHAPE_TOPIC``` in the ```RsFileAdapter.xml``` configuration file.
```SHAPE_TOPIC``` should be the type of shape you are publishing or subscribing to.

Possible values for ```SHAPE_TOPIC``` are:

- Square
- Circle
- Triangle

You have 3 different configurations (cfgName) to choose from:

-   **file_to_file** - This reads data from a file and writes data to
another file both using the FileAdapter plug-in. As before you should set the
appropriate value of ```SHAPE_TOPIC``` before starting Routing Service.
-   **file_to_dds** - This reads data from a file using the FileAdapter and
outputs it to DDS. You can visualize the ouptut by subscribing to the chosen
```SHAPE_TOPIC``` in Shapes Demo.
-   **dds_to_file** - This writes data to a file using the FileAdapter by
reading the input from DDS. You can publish data in Shapes Demo for a shape and
set that value to ```SHAPE_TOPIC``` before starting Routing Service.

To run Routing Service, you will need first to set up your environment as follows.

Before running the RTI Routing Service, you need to specify where the
`fileadapter` library is located as shown below:

```bash
export RTI_LD_LIBRARY_PATH=<Connext DDS Directory>/lib/<Connext DDS Architecture>:<Path to fileadapter library>
```

```bash
# From the build/ directory
$<Connext DDS Directory>/bin/rtiroutingservice
-cfgFile RsFileAdapter.xml
-cfgName <cfgName>
```
The general behavior of this example can be controlled using properties and the
```SHAPE_TOPIC``` environment variable. You can modify properties in the Routing
Service configuration file. This is the list of available properties:

| Property                                 | Tag        | Description                                                                                   |
| ---------------------------------------- | ---------- | ----------------------------------------------------------------------------------------------|
| `example.adapter.input_file`             | `<input>`  | Path to a CSV file that contains the sample data. File must exist and contain valid CSV data. |
| `example.adapter.sample_period_millisec` | `<input>`  | Periodic rate of reading samples from the file                                                |
| `example.adapter.sample_period_loop`     | `<input>`  | Whether to restart reading at the beginning of the file when it reach the                     |
| `example.adapter.output_file`            | `<output>` | Path to the file where to store the received samples                                          |
| `example.adapter.write_mode`             | `<output>` | The write mode can be:<ul><li>overwrite (write in the file deleting previous content)</li><li>append (write at then end of the file)</li><li>keep (fail if the file already exists and don't modify it)</li></ul>|
| `example.adapter.flush`                  | `<output>` | Whether to flush the file after every sample is written.                                      |
