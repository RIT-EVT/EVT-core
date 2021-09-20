.. EVT-core documentation master file, created by
   sphinx-quickstart on Sat Sep  4 10:17:15 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

========
EVT-core
========

Introduction
============

What is EVT
-----------

EVT stands for Electric Vehicle Team at RIT. We build and race electric
vehicles. Currently we have two track motorcycles and we are developing for
an electric dirt bike.

Firmware in EVT
---------------

In EVT the Firmware team has the responsibility for desigining, developing,
and maintaining the software that is associated with the team. The bulk of
the software is in the form of the firmware that runs on our custom hardware.

As an interdisiplinary team, the Firmware teams directly interfaces with the
other sub-teams at EVT. Most noteably we work directly with the Electrical
team in the design and testing of firmware.

What is EVT-core
----------------

EVT-core is the main library that the rest of the firmware is based around.
The library is designed to be an abstraction layer between the application
level firmware and the microcontroller. EVT-core is not a full RTOS, but
instead aims to abstract common IO interfaces and devices that the team
interacts with. As such we include interfaces for I2C, PWM, UART, CAN, etc.

How do I Get Started with EVT-core?
-----------------------------------

To get started visit the `EVT-core repository <https://github.com/RIT-EVT/EVT-core>`_.
The README will go over the high level process for building the code. The
project is built using CMake and requires an environment variable containing
the location of the GCC ARM toolchain. The README will go over setting the
environment variable and building EVT-core.

Several samples are included in EVT-core directly. As a libary, EVT-core can
be linked against in external projects.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   design/index.rst
   api/index.rst
