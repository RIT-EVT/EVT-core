include(../../../cmake/evt-core_build.cmake)

set( SAMPLE_SOURCES main.cpp TestCanNode.cpp)
make_exe(canopen_sample "${SAMPLE_SOURCES}")

