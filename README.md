# SORACOM Orbit SDK for C/C++

This is the SDK for C/C++ programming language for SORACOM Orbit.

Please use it to generate WASM modules for data transformation processing in SORACOM Orbit from C/C++ language source code.

## Prerequisites

To compile the SDK itself, as well as programs using the SDK, [Emscripten](https://emscripten.org) is required. Please install it in advance.

## Usage

The `soracom` directory in this repository contains the SDK's source code and object files. You can generate `orbit.o` by executing `make` in the `soracom` directory. The object file contains all the code of the SDK, so please link this object file for use.

The function prototypes provided by the SDK are included in `orbit.h`. Please include this file in your include path.

For more specific usage, please refer to each sample in the `examples` directory.

## Examples

The `examples` directory in this repository includes the following samples:

- [`gps-multi-unit`](./examples/gps-multi-unit/)

  A sample for geofencing based on data sent from [GPS Multi Unit SORACOM Edition (Battery Built-in Type)](https://soracom.jp/store/5235/).

- [`lte-m-button`](./examples/lte-m-button/)

  A sample that adds auxiliary information to the data sent from the SORACOM LTE-M Button series ([SORACOM LTE-M Button Plus](https://soracom.jp/store/5207/) and [SORACOM LTE-M Button for Enterprise](https://soracom.jp/store/5206/)) and transmits it.

## License

This SDK is released under the MIT License. For details, please see the [LICENSE](./LICENSE) file.
