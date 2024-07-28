include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/evt-core_build.cmake)

if(USE_RTOS)
    make_exe(threadx-demo main.cpp)
else()
    add_custom_target(threadx-demo ${CMAKE_COMMAND} -E cmake_echo_color --red "RTOS is not on")
endif()