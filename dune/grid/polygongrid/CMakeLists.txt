set( HEADERS
  declaration.hh
  entity.hh
  grid.hh
  indexset.hh
  object.hh
  polytopegeometry.hh
)

install( FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/grid/polygongrid )
