===
API
===

The EVT-core library is broken up into several semantic sections.
The sections are meant as a way to divide functionality semantically. Each
section may or may not have additional divisions between platform specific
and platform agnostic features. For more information on each section, and
for specifics on using the EVT-core API refer to the links below.

DEV
===
Devices, representation of hardware that can be interfaced with. In
general, devices are communicated with via some sort of IO interface, but that
is not strictly a rule. An LED is a simplistic example of a device.

IWDG
---

.. doxygenclass:: core::dev::IWDG
   :members:

LED
---

.. doxygenclass:: core::dev::LED
   :members:

RTC
---

.. doxygenclass:: core::dev::RTC
   :members:

Timer
-----

.. doxygenclass:: core::dev::Timer
   :members:

EEPROM
------

.. doxygenclass:: core::dev::EEPROM
   :members:

IO
==

IO represents different IO interfaces. The common ones are I2C, GPIO,
PWM, and CAN. IO generally includes and means for interfacing beyond the
microcontroller running the code.

Each platform will implement the IO functionality (if supported by the
microcontroller). Since the IO interfaces represent common functionality
shared across many devices, platform specific functionality may be
supported by the platform's implementation of the IO interface.

ADC
---

.. doxygenclass:: core::io::ADC
   :members:

CAN
---

.. doxygenclass:: core::io::CAN
   :members:

GPIO
----

.. doxygenclass:: core::io::GPIO
   :members:

I2C
---

.. doxygenclass:: core::io::I2C
   :members:

PWM
---

.. doxygenclass:: core::io::PWM
   :members:

SPI
---

.. doxygenclass:: core::io::SPI
   :members:

UART
----

.. doxygenclass:: core::io::UART
   :members:

Types
=====

CANMessage
----------

.. doxygenclass:: core::io::CANMessage
   :members:

Platform Implementations
========================

.. toctree::
   :maxdepth: 2
   :caption: Supported Platforms:

   stm32f3xx/stm32f302x8_dev.rst
   stm32f3xx/stm32f3xx_io.rst


Platform
--------

Platform represents the microcontroller that the code is running
on. Platform specific code such as system setup is included here. For example,
placing the microcontroller into a "low power mode" is specific to the
microcontroller itself.

STM32f3xx
^^^^^^^^^

Interrupt priorities are configured with priorities defined by
`stm32f3xx.hpp`.  A lower number corresponds to a higher priority.
Interrupts of higher priority may interrupt those of lower priority.

.. doxygenfile:: EVT/platform/f3xx/stm32f3xx.hpp

Utils
=====

Utilities refer to software utilities that make development easier.
These can be used to reduce duplicated code, abstract common tasks, or handle
functionality that is very common across platforms such as having the system
wait for a number of seconds.

Time
====

.. doxygenfile:: EVT/utils/time.hpp
