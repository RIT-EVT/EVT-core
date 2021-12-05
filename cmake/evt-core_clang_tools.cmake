# Additional targets to perform clang-format/clang-tidy
# Get all project files
file(GLOB_RECURSE
        ALL_CXX_SOURCE_FILES
        *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
        )

# Adding clang-format target if executable is found
find_program(CLANG_FORMAT "clang-format")
message(${CLANG_FORMAT})
if(CLANG_FORMAT)
    message(APPLYING)
    execute_process(
            COMMAND /usr/bin/clang-format-12
            -i
            -style=file
            ${ALL_CXX_SOURCE_FILES}
    )
endif()