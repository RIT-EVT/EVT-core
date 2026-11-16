include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/evt-core_build.cmake)

make_exe(static_dac static_dac.cpp)
make_exe(buffered_dac buffered_dac.cpp)
