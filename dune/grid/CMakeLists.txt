install(FILES polygongrid.hh
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/grid)

add_subdirectory(polygongrid)
add_subdirectory(test)
