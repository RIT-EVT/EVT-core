#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "HALf3::HALf3" for configuration "Debug"
set_property(TARGET HALf3::HALf3 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(HALf3::HALf3 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "ASM;C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libHALf3d.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS HALf3::HALf3 )
list(APPEND _IMPORT_CHECK_FILES_FOR_HALf3::HALf3 "${_IMPORT_PREFIX}/lib/libHALf3d.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
