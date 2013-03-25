AC_DEFUN([DUNE_POLYGONGRID_CHECKS],[
  DUNE_DEFINE_GRIDTYPE([POLYGONGRID],[(GRIDDIM == 2) && (WORLDDIM == 2)],[Dune::PolygonGrid< double >],[dune/grid/polygongrid.hh],[dune/grid/polygongrid/dgfparser.hh])
])

AC_DEFUN([DUNE_POLYGONGRID_CHECK_MODULE],[
  AC_MSG_NOTICE([Searching for dune-polygongrid...])
  DUNE_CHECK_MODULES([dune-polygongrid],[grid/polygongrid.hh])
])
