include(../../../cmake/evt-core_build.cmake)

set( SAMPLE_SOURCES main.cpp TPDOCanNode.cpp)
make_exe(canopen_tpdo "${SAMPLE_SOURCES}")

