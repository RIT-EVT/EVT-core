# Hardware Abstraction Layer (HAL)

## Introduction

This project is a CMake wrapper for the HAL provided by STM. The code itself
is not created by EVT, but the CMake wrapper that allows the code to be used
as a library is.

The goal of this is to allow easier inclusion of the HAL into the EVT core
library while maintaining a seperation between EVT code and STM provided code.
