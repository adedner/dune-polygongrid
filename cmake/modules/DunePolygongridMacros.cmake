dune_define_gridtype( GRID_CONFIG_H_BOTTOM
                      GRIDTYPE POLYGONGRID
                      ASSERTION "GRIDDIM == WORLDDIM"
                      DUNETYPE "Dune::PolygonGrid< double >"
                      HEADERS "dune/polygongrid/grid.hh", "dune/polygongrid/dgf.hh" )
