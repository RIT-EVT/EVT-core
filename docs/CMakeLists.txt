###############################################################################
# Find requirements
###############################################################################
find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

###############################################################################
# Doxygen Setup
###############################################################################
# Get all headers
get_target_property(EVT_PUBLIC_HEADER_DIR EVT INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE EVT_PUBLIC_HEADERS ${EVT_PUBLIC_HEADER_DIR}/*.hpp)

# Configure Doxygen
set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/include)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Add command for doxygen
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   DEPENDS ${EVT_PUBLIC_HEADERS}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                   COMMENT "Generating doxygen docs")
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

###############################################################################
# Sphinx Setup
###############################################################################
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/docs/sphinx)

add_custom_target(Sphinx ALL
                  COMMAND
                  ${SPHINX_EXECUTABLE} -b html
                  -Dbreathe_projects.EVT=${DOXYGEN_OUTPUT_DIR}/xml/
                  ${SPHINX_SOURCE} ${SPHINX_BUILD}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Generating documentation with Sphinx")
