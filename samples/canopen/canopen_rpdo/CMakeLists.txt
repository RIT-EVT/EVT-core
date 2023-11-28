include(../../../cmake/evt-core_build.cmake)

set( SAMPLE_SOURCES main.cpp RPDOCanNode.cpp)
make_exe(canopen_rpdo "${SAMPLE_SOURCES}")

