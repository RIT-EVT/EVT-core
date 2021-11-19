#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "EVT::EVT" for configuration "Debug"
set_property(TARGET EVT::EVT APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(EVT::EVT PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libEVTd.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS EVT::EVT )
list(APPEND _IMPORT_CHECK_FILES_FOR_EVT::EVT "${_IMPORT_PREFIX}/lib/libEVTd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
