#[[
CMake include file that applies standardized formatting to all C++ files in the
project.
Only include this file when new formatting is desired.
Requires: clang-format (v. 12 or higher) is downloaded
]]#

# Get all project files
file(GLOB_RECURSE
        ALL_CXX_SOURCE_FILES
        *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
        )

# Get the path to clang-format
find_program(CLANG_FORMAT "clang-format")
# Run the formatting if clang-format is found
if (CLANG_FORMAT)
    message("-- Found clang-format: ${CLANG_FORMAT}")
    execute_process(
            COMMAND ${CLANG_FORMAT}
            -i
            -style=file # Sets the style to operate off the .clangformat file
            ${ALL_CXX_SOURCE_FILES}
    )
    message("-- Formatting done")
else()
    message("-- Error: clang-format not found")
endif ()


################################################################################
# How to set up clang-format
################################################################################

#[[
Linux:
$ sudo apt-get install clang-format
$ sudo apt-get install clang-format-12
$ sudo update-alternatives --install /usr/bin/clang-format clang-format
           /usr/bin/clang-format-12 100

Windows:
1. Download the installer from https://llvm.org/builds/.
2. Run the installer, making sure you choose the option to add to the PATH
       variable.
]]#