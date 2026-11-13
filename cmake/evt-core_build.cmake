#[[
Creates an executable with the provided name. Generates bin, hex, elf, lst and map
files for the given name. This will compile a dedicated project from the passed in source files.
]]#
macro(make_exe proj_name sources)
    project(${proj_name} C CXX ASM)
    add_definitions(-DUSE_HAL_LIBRARY)

    # Tells gcc to make a project with the passed in source files
    add_executable(${proj_name} ${sources})

    # Make the main executable have an ".elf" suffix
    set_target_properties(${proj_name} PROPERTIES
        OUTPUT_NAME "${proj_name}"
        SUFFIX ".elf"
    )

    # Generate a map file
    target_link_options(${proj_name} PRIVATE "-Wl,-Map=${proj_name}.map")

    # Create hex, binary, and listing files after compilation is complete
    set(HEX_FILE "${proj_name}.hex")
    set(BIN_FILE "${proj_name}.bin")
    set(LST_FILE "${CMAKE_BINARY_DIR}/${proj_name}.lst")
    add_custom_command(TARGET ${proj_name} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${proj_name}> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${proj_name}> ${BIN_FILE}
    COMMAND ${CMAKE_OBJDUMP} -d -S -t -w --show-raw-insn $<TARGET_FILE:${proj_name}> > ${LST_FILE}
    COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE} \nBuilding ${LST_FILE}")

    target_link_libraries(${proj_name} PUBLIC EVT)

    if(USE_RTOS)
        target_link_libraries(${proj_name} PUBLIC threadx)
    endif()
endmacro()
