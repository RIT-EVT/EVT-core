#[[
CMake include file that creates a custom target to apply standardized formatting to all C++ files in
the project.
Requires: clang-format (v. 12 or higher) is downloaded
]]#

include_guard(GLOBAL)

# Get all project files for for the current project only
file(GLOB_RECURSE
        ALL_CXX_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.[ch]pp      ${CMAKE_SOURCE_DIR}/src/*.[CH]
        ${CMAKE_SOURCE_DIR}/include/*.[ch]pp  ${CMAKE_SOURCE_DIR}/include/*.[CH]
        ${CMAKE_SOURCE_DIR}/targets/*.[ch]pp  ${CMAKE_SOURCE_DIR}/targets/*.[CH]
        ${CMAKE_SOURCE_DIR}/samples/*.[ch]pp  ${CMAKE_SOURCE_DIR}/samples/*.[CH]
        )

# Get the path to clang-format
find_program(CLANG_FORMAT "clang-format")
# Create the formatting target if clang-format is found
if(CLANG_FORMAT)
    message(STATUS "Found clang-format: ${CLANG_FORMAT}")

    add_custom_target(clang-format)
    # Loop through each file and create a separate command because Windows has a 128-character limit
    # on its commands
    foreach(src_file ${ALL_CXX_SOURCE_FILES})
        add_custom_command(TARGET clang-format
                COMMAND ${CLANG_FORMAT} -i -style=file ${src_file}
                )
    endforeach()
else()
    message(WARNING "clang-format not found")
endif()


####################################################################################################
# How to set up clang-format
####################################################################################################

#[[
Linux:
$ sudo apt-get install clang-format
$ sudo apt-get install clang-format-12
$ sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-12 100

Windows:
1. Download the installer from https://llvm.org/builds/.
2. Run the installer, making sure you choose the option to add to the PATH variable.
]]#