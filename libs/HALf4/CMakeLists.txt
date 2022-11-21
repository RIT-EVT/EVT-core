###############################################################################
# CMake file for exposing HAL as library.
###############################################################################
cmake_minimum_required(VERSION 3.15)

include(GNUInstallDirs)

###############################################################################
# Project Setup
###############################################################################
project(HALf4
    VERSION 0.0.1
    LANGUAGES C ASM
)

add_library(${PROJECT_NAME} STATIC)

# Glob together all C source files
file(GLOB HALf4_C_SRC_FILES src/*.c)

if(TARGET_DEV STREQUAL "STM32F446xx")
    set(STARTUP_ASSEMBLY src/startup_stm32f446xx.s)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -TSTM32F446RETX_FLASH.ld")
endif()

# Handle assembly file
set_source_files_properties(
    ${STARTUP_ASSEMBLY}
    PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp"
)

# Add C and Assembly source files
target_sources(${PROJECT_NAME} PRIVATE
    ${HALf4_C_SRC_FILES}
    ${STARTUP_ASSEMBLY}
)

# Add headers privately.
# NOTE: This is a flat inclusion of the headers as that is how the STM HAL
# code expects the headers to be
target_include_directories(${PROJECT_NAME} PRIVATE include/HALf4/)

###############################################################################
# Compiler settings
###############################################################################
# Add flags for specific version of hardware
set(HALf4_HARDWARE_FLAGS "-mthumb -mcpu=cortex-m4")

add_compile_definitions(USE_HAL_DRIVER)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${HALf4_HARDWARE_FLAGS}")

###############################################################################
# Expose library
###############################################################################
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

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)