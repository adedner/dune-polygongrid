set( HEADERS
  declaration.hh
  entity.hh
  entityiterator.hh
  entityseed.hh
  geometry.hh
  grid.hh
  gridfamily.hh
  indexset.hh
  intersection.hh
  mesh.hh
  meshobjects.hh
  multivector.hh
  subentity.hh
)

install( FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/polygongrid )

dune_add_library( dunepolygongrid
  mesh.cc
  ADD_LIBS ${DUNE_LIBS}
)
