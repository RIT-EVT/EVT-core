include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/evt-core_build.cmake)

make_exe(single_adc single_adc.cpp)
make_exe(multi_adc multi_adc.cpp)
make_exe(vref_demo vref_demo.cpp)
