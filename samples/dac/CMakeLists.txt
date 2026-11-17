include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/evt-core_build.cmake)

make_exe(dac_static dac_static.cpp)
make_exe(dac_buffered dac_buffered.cpp)
