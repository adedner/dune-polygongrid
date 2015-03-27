set( HEADERS
  declaration.hh
  entity.hh
  grid.hh
  gridfamily.hh
  indexset.hh
  object.hh
)

install( FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/polygongrid )
