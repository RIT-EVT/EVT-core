include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/evt-core_build.cmake)


set( SAMPLE_SOURCES main.cpp TestCanNode.cpp)
make_exe(canopen "${SAMPLE_SOURCES}")
