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

The compilation process is handled via CMake following general standard
practices. The noticable addition is the need for an environment variable
to identify the location of the GCC ARM toolchain which is used for compiling
for the ARM micrcontrollers.

On your system [install CMake](https://cmake.org/install/).

#### Step 1. Install GCC ARM Tools

First install the GCC ARM tools for your platform. You can find the
installation [link here](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads).
On Mac OS and Linux downloading the tarball file is preferred.

After installing, take note of where the files have been installed. On Windows
systems the files are often installed in `C:\Program Files` or `C:\Program Files (x86)`.
On Mac OS and Linux if you downloaded a tarball you can extract the files to
a known location. From here you will need to set and environment variable to
the `bin/` folder contained in the installation.

#### Step 2. Setting Environment Variable

Next an environment variable needs to be set. On Windows follow
the directions [here](https://kb.wisc.edu/cae/page.php?id=24500). For variable
name enter `GCC_ARM_TOOLS_PATH`. For the value, select "Browse Directory.."
and locate the GCC ARM installation and select the "bin" folder contained. After
pressing "Ok" a system restart may be required for the variable to take effect.

For Mac OS and Linux, setting an environment variable can be done by
editing `.bashrc` on Linux and `.bash_profile` or `.zshrc` on Mac OS. In
the file that exists on your system, add the line below.

```
export GCC_ARM_TOOLS_PATH=/path/to/gcc_arm_tools_path/bin/
```

After adding that line, restart your terminal for the changes to take effect.

#### Step 3. Build EVT-core

Below are the commands for building EVT-core. You can execute them from
within the cloned EVT-core repository from the terminal.

```bash
mkdir build/
cd build/
cmake ../
make -j
```

After running those commands, you should have a fully build EVT-core including
samples.

## Features Supported

Below is the state of supported features on each platform.

NOTE: In these early releases testing is not fully verbose.

| Platform    | I2C                | SPI | PWM                | CAN                | ADC                | Flash | RTC | UART                | GPIO                |
|-------------|--------------------|-----|--------------------|--------------------|--------------------|-------|-----|---------------------|---------------------|
| STM32F302R8 | :heavy_check_mark: | :x: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x:   | :x: | :heavy_check_mark:  | :heavy_check_mark:  |
| STM32F446RE | :x:                | :x: | :x:                | :x:                | :x:                | :x:   | :x: | :x:                 | :x:                 |

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

## Links and Resources

* [Installing CMake](https://cmake.org/install/)
* [Running CMake on Windows](https://stackoverflow.com/questions/4101456/running-cmake-on-windows)
* [Setting an Environment Variable on Mac](https://phoenixnap.fr/kb/set-environment-variable-mac)
* [GNU Arms Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* [Setting an Enviornment Variable on Windows](https://kb.wisc.edu/cae/page.php?id=24500)
