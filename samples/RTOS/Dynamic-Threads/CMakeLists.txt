include(../../../cmake/evt-core_build.cmake)

set(SAMPLE_SOURCES main.cpp)
make_exe(Dynamic-Threads "${SAMPLE_SOURCES}")