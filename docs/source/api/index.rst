API
===

The EVT-core library is broken up into several semantic sections.
The sections are meant as a way to divide functionality semantically. Each
section may or may not have additional divisions between platform specific
and platform agnostic features. For more information on each section, and
for specifics on using the EVT-core API refer to the links below.

DEV
^^^
Devices, representation of hardware that can be interfaced with. In
general, devices are communicated with via some sort of IO interface, but that
is not strictly a rule. An LED is a simplistic example of a device.

.. doxygennamespace:: EVT::core::DEV
   :members:

IO
^^
IO represents different IO interfaces. The common ones are I2C, GPIO,
PWM, and CAN. IO generally includes and means for interfacing beyond the
microcontroller running the code.

.. doxygennamespace:: EVT::core::IO
    :members:

Platform
^^^^^^^^
Platform represents the microcontroller that the code is running
on. Platform specific code such as system setup is included here. For example,
placing the microcontroller into a "low power mode" is specific to the
microcontroller itself.


Utils
^^^^^
Utilities refer to software utilities that make development easier.
These can be used to reduce duplicated code, abstract common tasks, or handle
functionality that is very common across platforms such as having the system
wait for a number of seconds.

