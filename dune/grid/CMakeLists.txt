add_subdirectory( polygongrid )

set( HEADERS
  polygongrid.hh
)

install( FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/grid )
