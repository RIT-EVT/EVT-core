#[[
Cmake include file that contains all compiler variables and flags for EVT-core
and related projects.
Requires: `${EVT_CORE_DIR}` cmake variable to be set.
]]#
# Verify that the ARM tools path is set
if(NOT DEFINED ENV{GCC_ARM_TOOLS_PATH})
    message(FATAL_ERROR
            "Set your environment variables you frickin' hecker."
            "   --Shane Snover"
            )
endif()

# Flags to skip compiler check
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Setup for cross compilation
set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  arm)
set(CMAKE_AR                "$ENV{GCC_ARM_TOOLS_PATH}/arm-none-eabi-ar")
set(CMAKE_ASM_COMPILER      "$ENV{GCC_ARM_TOOLS_PATH}/arm-none-eabi-gcc")
set(CMAKE_C_COMPILER        "$ENV{GCC_ARM_TOOLS_PATH}/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER      "$ENV{GCC_ARM_TOOLS_PATH}/arm-none-eabi-g++")
set(CMAKE_LINKER            "$ENV{GCC_ARM_TOOLS_PATH}/arm-none-eabi-ld")
set(CMAKE_OBJCOPY           "$ENV{GCC_ARM_TOOLS_PATH}/arm-none-eabi-objcopy")
set(CMAKE_RANLIB            "$ENV{GCC_ARM_TOOLS_PATH}/arm-none-eabi-ranlib")
set(CMAKE_SIZE              "$ENV{GCC_ARM_TOOLS_PATH}/arm-none-eabi-size")
set(CMAKE_STRIP             "$ENV{GCC_ARM_TOOLS_PATH}/arm-none-eabi-strip")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set (CMAKE_CXX_STANDARD 17)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # does not produce the json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

include(GNUInstallDirs)

# EVT Linking option, linting revolves around cpplint which is a tool that
# conforms to Google's C++ style guide. cpplint needs to be installed.
option(EVT_LINT
        "Lint source code, need to have cpplint installed"
        OFF
        )

if(EVT_LINT)
    # TODO: In the future these filter settings sound be included in cfg
    # files.
    set(CMAKE_CXX_CPPLINT "cpplint;--filter=-legal/copyright, \
                          -readability/todo,-build/include_order")
endif()

###############################################################################
# Set compiler and linker flags
###############################################################################
# TODO: Decouple hardware specific flags
set(EVT_COMMON_FLAGS        "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 \
                            -mfloat-abi=hard -Og -Wall -fdata-sections \
                            -ffunction-sections -MMD -MP -MF -ggdb -g3")

set(CMAKE_C_FLAGS           "${EVT_COMMON_FLAGS} \
                            -std=gnu99")

set(CMAKE_CXX_FLAGS         "${EVT_COMMON_FLAGS} \
                            -fno-rtti -Wvla")

# TODO: Check for platform to decide which linker script to use
set(CMAKE_EXE_LINKER_FLAGS  "-mfloat-abi=hard -specs=nano.specs -specs=nosys.specs \
                            -T ${EVT_CORE_DIR}/libs/HALf3/STM32F302C8Tx_FLASH.ld \
                            -lc -lm -lnosys -u _printf_float")