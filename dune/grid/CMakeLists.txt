add_subdirectory( polygongrid )
add_subdirectory( test )

set( HEADERS
  polygongrid.hh
)

install( FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/grid )
