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

### Where can I get more Information?

Check out EVT-core's documentation hosted [here](https://evt-core.readthedocs.io/en/latest/)
for more information.

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
within the cloned EVT-core repository from the terminal. Below are the steps
easiest followed for Mac and Linux with CMake installed.

```bash
mkdir build/
cd build/
cmake ../
make -j
```

On Windows there are a couple of options worth looking into. CMake has a GUI
which allows you to select projects and how to build them, if you use the GUI,
you can directly select "MinGW" as the generator to ensure that the build
takes place properly. Or you can make use of CLion which will abstract the
build process away. Another option (which I recommend for long term use), is
to learn about WSL (Windows Subsystem for Linux). Linux is the OS of choice
for software developers and includes a lot of great tools for development. WSL
is a happy medium between running Windows and installing a virtual machine.

Below are incomplete and experimental instructions for building on Windows. Using
CLion is the preferred approach on Windows. But some success has come from using
the terminal, VS code, etc.

1. Install mingwin from the instructions [here](https://www.ics.uci.edu/~pattis/common/handouts/mingweclipse/mingw.html). Make sure to also follow the instructions for updating the PATH
variable! You will probably have to restart your system after this.

2. Launch the CMake GUI from the start menu.

3. For "Where is source" navigate to the top level of the EVT-core repo.

4. For "Where to build the binaries" select "<EVT-core path>/build"

5. From Tool > Configure change the generator to "Codeblock - MinGW Makefiles" and select
"use default native compilers"

6. Press Configure then Generate.

7. At this point things get choppy. There have been some success building from
terminal manually, some with VS code, but mostly with CLion.


After running those commands, you should have a fully build EVT-core including
samples.

For a more detailed build process using CLion and on Windows,
visit the [EVT wiki](https://wiki.rit.edu/display/EVT/Setup+for+EVT-core+Development).

#### Step 4. Flashing the Board

After building has taken place, you should be left with a folder that contains
all of the built samples. The samples will be located in `build/samples` and
each sample with have its own folder.

For example, in `build/samples/blink/` there will be an ELF, bin, hex, and
map files among others. To flash the blink sample to the nucleo, copy the
bin file from your directory to the nucleo that is connected to their computer.
The nucleo should show up as an external storage device. After dragging the
bin file to the nucleo, the sample should be flashed and an LED on the board
should start blinking at a set rate.

For interacting with samples that make use of a UART monitor, a serial console
will be needed. PuTTY on Windows is a good tool. Linux and MacOS will have some
built in tools for serial console and gtkterm is worth looking into.
But when in doubt, the Arduino serial console will also work!

## Supported Platform Features

Below is the supported features associated with each board

| Feature  | STM32F302R8        | STM32F334R8        | STM32F446RE        |
|----------|--------------------|--------------------|--------------------|
| ADC      | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| CAN      | :heavy_check_mark: | :heavy_check_mark: | :x:                |
| CANopen  | :heavy_check_mark: | :heavy_check_mark: | :x:                |
| GPIO     | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| I2C      | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| PWM      | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| RTC      | :heavy_check_mark: | :heavy_check_mark: | :x:                |
| SPI      | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| Timer    | :heavy_check_mark: | :heavy_check_mark: | :x:                |
| UART     | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| Watchdog | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |

## Future Features

EVT-Core is heavily a work in progress, below is a running list of the features
or concepts that EVT-Core should support.

* Test Platform
* System Initialization Function
    * Calling HAL init
    * Clock settings
* Ability for users of code to interject their own pin mappings

## Links and Resources

* [Installing CMake](https://cmake.org/install/)
* [Running CMake on Windows](https://stackoverflow.com/questions/4101456/running-cmake-on-windows)
* [Setting an Environment Variable on Mac](https://phoenixnap.fr/kb/set-environment-variable-mac)
* [GNU Arms Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* [Setting an Enviornment Variable on Windows](https://kb.wisc.edu/cae/page.php?id=24500)
