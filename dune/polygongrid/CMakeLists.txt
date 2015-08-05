set( HEADERS
  declaration.hh
  entity.hh
  entityiterator.hh
  entityseed.hh
  geometry.hh
  grid.hh
  gridfamily.hh
  halfedge.hh
  indexset.hh
  intersection.hh
)

install( FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/polygongrid )
