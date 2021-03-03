# Add example code
set(SAMPLE_NAME blink)

project(${SAMPLE_NAME} C CXX ASM)

add_definitions(-DUSE_HAL_LIBRARY)

add_executable(${SAMPLE_NAME} 
    main.cpp  
    /home/cbolles/devel/evt/EVT-core/libs/HALf3/src/startup_stm32f302x8.s
    /home/cbolles/devel/evt/EVT-core/samples/blink/STM32F302C8Tx_FLASH.ld
)

set_target_properties(${SAMPLE_NAME} PROPERTIES 
    OUTPUT_NAME "${SAMPLE_NAME}"
    SUFFIX ".elf"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
# Generate map
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
                            -T /home/cbolles/devel/evt/EVT-core/samples/blink/STM32F302C8Tx_FLASH.ld \
                            -Wl,-Map=${SAMPLE_NAME}.map")
set_target_properties(${SAMPLE_NAME} PROPERTIES LINK_DEPENDS "/home/cbolles/devel/evt/EVT-core/samples/blink/STM32F302C8Tx_FLASH.ld")

set(HEX_FILE "${SAMPLE_NAME}.hex")
set(BIN_FILE "${SAMPLE_NAME}.bin")
add_custom_command(TARGET ${SAMPLE_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${SAMPLE_NAME}> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${SAMPLE_NAME}> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")

# Link the EVT library
target_link_libraries(${SAMPLE_NAME} EVT)
