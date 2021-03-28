# Add example code
set(SAMPLE_NAME uart)

project(${SAMPLE_NAME} C CXX ASM)

add_definitions(-DUSE_HAL_LIBRARY)

add_executable(${SAMPLE_NAME}
    main.cpp
)

set_target_properties(${SAMPLE_NAME} PROPERTIES
    OUTPUT_NAME "${SAMPLE_NAME}"
    SUFFIX ".elf"
)

# Generate map
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
                            -Wl,-Map=${SAMPLE_NAME}.map")

set(HEX_FILE "${SAMPLE_NAME}.hex")
set(BIN_FILE "${SAMPLE_NAME}.bin")
add_custom_command(TARGET ${SAMPLE_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${SAMPLE_NAME}> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${SAMPLE_NAME}> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")

# Link the EVT library
target_link_libraries(${SAMPLE_NAME} EVT)
