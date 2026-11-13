include(../../../cmake/evt-core_build.cmake)

set( SAMPLE_SOURCES main.cpp SDOCanNode.cpp)
make_exe(canopen_sdo "${SAMPLE_SOURCES}")