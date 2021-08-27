 ###############################################################################
 # Install and expose library
 ###############################################################################
macro(install_and_expose proj_name)
   
    # Expose headers
    target_include_directories(${proj_name}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    target_compile_definitions(${proj_name} PRIVATE -D_EXPORT)

    set_target_properties(${proj_name} PROPERTIES DEBUG_POSTFIX "d")

    install(TARGETS ${proj_name}
    EXPORT ${proj_name}-config
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(
    EXPORT ${proj_name}-config
    NAMESPACE ${proj_name}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${proj_name}
    )

    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${proj_name}
    )
endmacro()