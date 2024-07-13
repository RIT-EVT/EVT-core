include(../../../cmake/evt-core_build.cmake)

set(SAMPLE_SOURCES main.cpp tx_initialize_low_level.s)
make_exe(Dynamic-Threads "${SAMPLE_SOURCES}")