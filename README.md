# EVT-core

## Introduction

### What is EVT?
EVT stands for the Electric Vehicle Team at RIT. We build and race electric
vehicles. Currently we have two track motorcycles we develop for and are
desiging an electric dirt bike.

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

## Features Supported

Below is the state of supported features on each platform.

| Platform    | I2C | SPI | PWM | CAN | ADC | Flash | RTC | UART | GPIO |
|-------------|-----|-----|-----|-----|-----|-------|-----|------|------|
| STM32F302R8 | :x: | :x: | :x: | :x: | :x: | :x:   | :x: | :x:  | :x:  |
| STM32F446RE | :x: | :x: | :x: | :x: | :x: | :x:   | :x: | :x:  | :x:  |

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

## TODO

* Reduce program size by removing need for the linker flag `-specs=nosys.specs`

## Release Notes
