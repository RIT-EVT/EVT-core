#[[
Cmake include file that contains all compiler variables and flags for EVT-core
and related projects.
Requires: `${EVT_CORE_DIR}` cmake variable to be set.
]]#
# Verify that the ARM tools path is set
if(NOT DEFINED ENV{GCC_ARM_TOOLS_PATH})
    message(WARNING
            "Set your environment variables you frickin' hecker."
            "   --Shane Snover"
            )
endif()

# Handle Selection of the target device
option(TARGET_DEV "Target device" "STM32F334x8")
if(NOT TARGET_DEV)
    set(TARGET_DEV "STM32F334x8")
endif()


if(TARGET_DEV STREQUAL "STM32F302x8")
    add_compile_definitions(STM32F302x8)
    add_compile_definitions(STM32F3xx)
elseif(TARGET_DEV STREQUAL "STM32F334x8")
    add_compile_definitions(STM32F334x8)
    add_compile_definitions(STM32F3xx)
elseif(TARGET_DEV STREQUAL "STM32F446xx")
    add_compile_definitions(STM32F446xx)
    add_compile_definitions(STM32F4xx)
else()
    message(FATAL_ERROR "The target device is not supported")
endif()
message(STATUS "${TARGET_DEV} targeted")


# Flags to skip compiler check
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Setup for cross compilation
file(TO_CMAKE_PATH "$ENV{GCC_ARM_TOOLS_PATH}" GCC_ARM_TOOLS_PATH)
set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  arm)

if (WIN32)
    set(CMAKE_AR                "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-ar.exe")
    set(CMAKE_ASM_COMPILER      "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-gcc.exe")
    set(CMAKE_C_COMPILER        "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-gcc.exe")
    set(CMAKE_CXX_COMPILER      "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-g++.exe")
    set(CMAKE_LINKER            "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-ld.exe")
    set(CMAKE_OBJCOPY           "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-objcopy.exe")
    set(CMAKE_RANLIB            "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-ranlib.exe")
    set(CMAKE_SIZE              "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-size.exe")
    set(CMAKE_STRIP             "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-strip.exe")
else()
    set(CMAKE_AR                "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-ar")
    set(CMAKE_ASM_COMPILER      "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-gcc")
    set(CMAKE_C_COMPILER        "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER      "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-g++")
    set(CMAKE_LINKER            "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-ld")
    set(CMAKE_OBJCOPY           "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-objcopy")
    set(CMAKE_RANLIB            "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-ranlib")
    set(CMAKE_SIZE              "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-size")
    set(CMAKE_STRIP             "${GCC_ARM_TOOLS_PATH}/arm-none-eabi-strip")
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set (CMAKE_CXX_STANDARD 17)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # does not produce the json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

include(GNUInstallDirs)

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

set(CMAKE_ASM_FLAGS         "-mthumb -mcpu=cortex-m4 \
                            -x assembler-with-cpp")

if(TARGET_DEV STREQUAL "STM32F302x8")
    set(CMAKE_EXE_LINKER_FLAGS "-T ${EVT_CORE_DIR}/libs/HALf3/STM32F302C8Tx_FLASH.ld")
elseif(TARGET_DEV STREQUAL "STM32F334x8")
    set(CMAKE_EXE_LINKER_FLAGS "-T ${EVT_CORE_DIR}/libs/HALf3/STM32F334R8TX_FLASH.ld")
elseif(TARGET_DEV STREQUAL "STM32F446xx")
    set(CMAKE_EXE_LINKER_FLAGS "-T ${EVT_CORE_DIR}/libs/HALf4/STM32F446RETX_FLASH.ld")
else()
    message(FATAL_ERROR "The target device is not supported")
endif()

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -mfloat-abi=hard \
                            -specs=nano.specs -specs=nosys.specs \
                            -lc -lm -lnosys -Wl,--gc-section,--print-memory-usage")
