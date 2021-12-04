# Additional targets to perform clang-format/clang-tidy
# Get all project files
file(GLOB_RECURSE
        ALL_CXX_SOURCE_FILES
        *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
        )

message(ATTEMPT)
if(EVT_CORE_FORMATTING)
    message(OUTSIDE)
    # Adding clang-format target if executable is found
    find_program(CLANG_FORMAT "clang-format")
    message(${CLANG_FORMAT})
    if(CLANG_FORMAT)
        message(INSIDE)
        execute_process(
                COMMAND /usr/bin/clang-format-12
                -i
                -style=file
                ${ALL_CXX_SOURCE_FILES}
        )
    endif()
endif()