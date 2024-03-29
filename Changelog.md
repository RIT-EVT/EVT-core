# Changelog

Contains notable changes there were added, fixed, or removed in each release.

## 1.0.0

### Added

* Support for the STM32f334
    * ADC
    * CAN
    * GPIO
    * I2C
    * PWM
    * SPI
    * Timer
    * UART
    * Watchdog
* Support for the STM32F302r8
    * SPI
    * Timer
* CANopen Support via canopen-stack
* Logging
* Style linting
* Device Drivers
    * EEPROM
        * M24C32
    * Watchdog
        * STM internal IWDG
    * Button
    * Thermistor
* CAN ID Message Filtering

### Fixed

* CAN hardfault handler associated with TX queue
* Linker flags to Remove Un-Used Code
* Multi-Channel ADC Support

### Changed

* Error Handling/IO Status
    * I2C status return value
    * CAN status return value
* Standardize GPIO Initialization Logic
* Fixed Point PWM
* Option for Internal GPIO Resistor Direction

## 0.2.0

### Added

* RTC Support for the STM32F302r8
* Improved Windows build support
* GPIO Interrupt Support for the STM32F302r8
* Sample for back-and-forth CAN communication
* Documentation auto-generation

### Fixed

* UART TX/RX bug due to differences in clock settings

### Changed

* Relax line length style requirements to 100 characters

## 0.1.0

### Added

* I2C Support for the STM32F302r8
* PWM Support for the STM32F302r8
* UART Support for the STM32F302r8
* CAN Support for the STM32F302r8
* GPIO Support for the STM32F302r8
* Basic System Intialization for the STM32F302r8
* Ability to build as library
