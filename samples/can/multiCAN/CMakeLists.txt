include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/evt-core_build.cmake)

if(COMPDEFS MATCHES "(.*)STM32F4xx(.*)")
    make_exe(multiCAN main.cpp)
else()
    add_custom_target(multiCAN ${CMAKE_COMMAND} -E cmake_echo_color --red "multiCAN only works on an STM32F446 due to dual CAN interfaces")
endif()


