#[[
Creates an executable with the provided name. Generates bin, hex, elf, and map
files for the given name. This will make a dedicated project.
]]#
macro(make_exe proj_name sources)
    project(${proj_name} C CXX ASM)
    add_definitions(-DUSE_HAL_LIBRARY)
    add_executable(${proj_name} ${sources})

    # Make the main executable have an ".elf" suffix
    set_target_properties(${proj_name} PROPERTIES
        OUTPUT_NAME "${proj_name}"
        SUFFIX ".elf"
    )

    # Generate a map file
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
                                -Wl,-Map=${proj_name}.map")
    set(HEX_FILE "${proj_name}.hex")
    set(BIN_FILE "${proj_name}.bin")
    add_custom_command(TARGET ${proj_name} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${proj_name}> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${proj_name}> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")

    # Link the EVT-core library
    target_link_libraries(${proj_name} PUBLIC EVT)
endmacro()
