# EVT-core

## Introduction

### What is EVT?
EVT stands for the Electric Vehicle Team at RIT. We build and race electric
vehicles. Currently we have two track motorcycles we develop for and are
designing an electric dirt bike.

This repository is managed by the Firmware team which handles the firmware
used by the team on the various vehicles.

### What is EVT-Core?
EVT-Core is the central library used by EVT for interacting with the MCUs on
the different components on the electric vehicles. The library is designed to
be an alternative to Mbed which was the previous tool used for programming the
MCUs. EVT-Core is not a RTOS, it instead contains abstractions for bare metal
programming. Contained is abstractions for working with I2C, SPI, GPIO, PWM,
RTC, etc.

### How is EVT-Core Used?
EVT-Core is a CMake library and as such can be built as a library and
manually linked to, or can be added as a sub-directory to a larger project.
Further information is to come.

### How do I Build the Code?
The compilation process is handled via CMake and in general follows standard
practices. The noticable addition is the need to setup an environment variable
which stores the location to the gcc arm toolchain. Below is an example of
how to set the environment variable on Linux/Unix.

```bash
export GCC_ARM_TOOLS_PATH=/path/to/gcc_arm_tools_path/bin/
```

The compilation logic will make use of that enivronment variable to locate
the needed tools to generate the code.

After that, the compilation process follows standard practice. Below is
an example compilation process.

```bash
mkdir build/
cd build/
cmake ../
make -j
```

After that, in the `build` folder will be the compiled library code as well
as generated samples.

For building with linting enabled (recommended). First install cpplint, then
run the installation with the modification below.

```bash
mkdir build/
cd build/
cmake -DEVT_LINT=ON ../
make -j
```

## Features Supported

Below is the state of supported features on each platform.

NOTE: In these early releases testing is not fully verbose.

| Platform    | I2C | SPI | PWM                | CAN | ADC | Flash | RTC | UART                | GPIO                |
|-------------|-----|-----|--------------------|-----|-----|-------|-----|---------------------|---------------------|
| STM32F302R8 | :x: | :x: | :heavy_check_mark: | :heavy_check_mark: | :x: | :x:   | :x: | :heavy_check_mark:  | :heavy_check_mark:  |
| STM32F446RE | :x: | :x: | :x:                | :x: | :x: | :x:   | :x: | :x:                 | :x:                 |

## Future Features

EVT-Core is heavily a work in progress, below is a running list of the features
or concepts that EVT-Core should support.

* STM32F446RE HAL
* I2C
* SPI
* PWM
* ADC
* Flash
* RTC
* UART
* GPIO
* Test Platform
* Linting
* System Initialization Function
    * Calling HAL init
    * Clock settings
* Ability for users of code to interject their own pin mappings

## TODO

* Add ability to prune out un-used code by adding the linker argument
`-Wl,--gc-sections` without removing the handler functions. This can half by
accomplished by calling the target handler once from main code execution.
* Style checking should be part of the build process.
* Add documentation for using this library.

## Release Notes
