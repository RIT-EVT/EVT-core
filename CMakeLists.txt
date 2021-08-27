###############################################################################
# Top level CMakeList for building the EVT common source code
###############################################################################
cmake_minimum_required(VERSION 3.15)

include(CMakeDependentOption)
set(EVT_CORE_DIR     ${CMAKE_SOURCE_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/cmake/evt-core_compiler.cmake)

###############################################################################
# Project Setup
###############################################################################
project(EVT
        VERSION 0.0.1
        LANGUAGES CXX C
)

add_library(${PROJECT_NAME} STATIC)

# Add sources
target_sources(${PROJECT_NAME} PRIVATE
    src/platform/f3xx/stm32f302x8.cpp

    src/io/ADC.cpp
    src/io/CAN.cpp
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
    src/utils/platform/f3xx/time.cpp
)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # does not produce the json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

include(GNUInstallDirs)

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

# Later this will be handled dynamically, either the HALf3 or the HALf4 will
# be linked to
target_link_libraries(${PROJECT_NAME}
    PUBLIC HALf3
)

###############################################################################
# Install and expose library
###############################################################################
# Expose headers
target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE -D_EXPORT)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-config
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    EXPORT ${PROJECT_NAME}-config
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

###############################################################################
# Handle adding samples
###############################################################################
if(EVT_BUILD_SAMPLES)
    # Add the sample code sub directory
    add_subdirectory(samples)
endif()
