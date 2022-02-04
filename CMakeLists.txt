###############################################################################
# Top level CMakeList for building the EVT common source code
###############################################################################
cmake_minimum_required(VERSION 3.15)

set(EVT_CORE_DIR     ${CMAKE_CURRENT_SOURCE_DIR})

include(CMakeDependentOption)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/evt-core_compiler.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/evt-core_install.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/evt-core_clang-format.cmake)

###############################################################################
# Project Setup
###############################################################################
file(STRINGS version.txt EVT_VERSION)

project(EVT
    VERSION ${EVT_VERSION}
    LANGUAGES CXX C
)

add_library(${PROJECT_NAME} STATIC)

# Add sources
target_sources(${PROJECT_NAME} PRIVATE
    src/platform/f3xx/stm32f302x8.cpp

    src/io/ADC.cpp
    src/io/CAN.cpp
    src/io/CANopen.cpp
    src/io/GPIO.cpp
    src/io/I2C.cpp
    src/io/PWM.cpp
    src/io/UART.cpp
    src/io/platform/f3xx/f302x8/ADCf302x8.cpp
    src/io/platform/f3xx/f302x8/CANf302x8.cpp
    src/io/platform/f3xx/f302x8/GPIOf302x8.cpp
    src/io/platform/f3xx/f302x8/I2Cf302x8.cpp
    src/io/platform/f3xx/f302x8/PWMf302x8.cpp
    src/io/platform/f3xx/f302x8/UARTf302x8.cpp
    src/io/types/CANMessage.cpp

    src/dev/LED.cpp
    src/dev/RTCTimer.cpp
    src/dev/platform/f3xx/f302x8/RTCf302x8.cpp
    src/dev/platform/f3xx/f302x8/IWDGf302x8.cpp
    src/dev/platform/f3xx/f302x8/Timerf302x8.cpp
    src/dev/storage/M24C32.cpp
    src/dev/Thermistor.cpp

    src/utils/platform/f3xx/time.cpp
    src/utils/log.cpp
)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
###############################################################################
# Provide build options
###############################################################################
# Check to see if this project is being built on its own, or as a dependency
# of another project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(EVT_IS_MAIN_PROJECT TRUE)
else()
    set(EVT_IS_MAIN_PROJECT FALSE)
endif()

# Determine if the sample code should be built. Defaults to false if this
# project is not the main project.
cmake_dependent_option(EVT_BUILD_SAMPLES
        "Build the EVT core samples" ON
        "EVT_IS_MAIN_PROJECT" OFF
)

###############################################################################
# Handle dependencies
###############################################################################

# TODO: This should be set by the user of this library
add_compile_definitions(STM32F302x8)
add_compile_definitions(USE_HAL_DRIVER)

add_subdirectory(libs/HALf3/)
add_subdirectory(libs/canopen/canopen/)

install(TARGETS Canopen EXPORT Canopen-config)
install(EXPORT Canopen-config DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Canopen)

# Later this will be handled dynamically, either the HALf3 or the HALf4 will
# be linked to
target_link_libraries(${PROJECT_NAME}
    PUBLIC HALf3
    PUBLIC Canopen
)

###############################################################################
# Install and expose library
###############################################################################
install_and_expose(${PROJECT_NAME})

###############################################################################
# Handle adding samples
###############################################################################
if(EVT_BUILD_SAMPLES)
    # Add the sample code sub directory
    add_subdirectory(samples)
endif()
