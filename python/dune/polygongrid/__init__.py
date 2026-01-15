from __future__ import absolute_import, division, print_function, unicode_literals
import io
import types

from dune.common.checkconfiguration import assertHave, ConfigurationError
from dune.generator import Constructor, Method
from dune.grid import cartesianDomain
from dune.generator import algorithm

from .blossoms import blossomDomain
from .voronoi import voronoiDomain

def polygonGrid(domain, ctype="double", dualGrid=False ):
    """
    Parameters:
      domain:   a domain such as `cartesianDomain`, dgf reader, or a gridView.
      ctype:    type of coordinate field
      dualGrid: whether or not to construct the dual grid

    Returns:
      A constructed polygonal grid.
    """
    from ..grid.grid_generator import module, getDimgrid

    typeName = "Dune::PolygonGrid< " + ctype + " >"
    includes = ["dune/polygongrid/grid.hh", "dune/polygongrid/dgf.hh"]

    dualGridMethod = Method('dualGrid', '''[]( DuneType &self ) { return self.dualGrid(); }''' )
    cachingStorage = Method('cachingStorage', '''[]( DuneType &self ) { return false; }''' )
    #updateMethod   = Method('update', '''[]( DuneType &self, const GridView &gv, bool dual) { self.update( gv, dual ); }''' )
    gridModule = module(includes, typeName, dualGridMethod, cachingStorage)

    gridView = None
    # check if domain is grid view
    if hasattr(domain, "elements"):
        gridView = domain
        # create dummy domain
        domain = cartesianDomain([0,0], [1,1], [1,1])

    grid = gridModule.LeafGrid(gridModule.reader(domain))

    def update( self, gv, dualGrid = False, resizeMemory=True ):
        """
        Update the polygonal grid based on a given grid view.

        Parameters:
            gv:            grid view to use as basis for updating the internal polygonal grid
            dualGrid:      flag to indicate whether or not a dual grid is computed
            resizeMemory:  flag to indicate whether or not dune-fem memory attached to the polygonal grid is adjusted

        Returns: None.
        """
        try:
            upd = self._updaters[ gv.cppTypeName ]
        except KeyError:
            ## add algorithm to call update for any given grid view type
            _code = """
#if HAVE_DUNE_FEM
#include <dune/fem/space/common/dofmanager.hh>
#endif
template< class Grid, class GV>
void update(Grid& hgrid, const GV& gv, const bool dualGrid, const bool resizeMemory )
{
  hgrid.update( gv, dualGrid );
#if HAVE_DUNE_FEM
  // resize memory for all discrete functions attached to this grid
  if( resizeMemory )
  {
    typedef Dune::Fem::DofManager< Grid > DofManagerType;
    DofManagerType& dm = DofManagerType::instance( hgrid );
    dm.resizeMemory();
    dm.compress();
  }
#endif
}
"""
            self._updaters[ gv.cppTypeName ] = algorithm.load("update", io.StringIO(_code), self, gv, dualGrid, resizeMemory )
            upd = self._updaters[ gv.cppTypeName ]
        upd( self, gv, dualGrid, resizeMemory )

    if gridView:
        # create empty updater dictionary
        grid.hierarchicalGrid._updaters = dict()
        update( grid.hierarchicalGrid, gridView, dualGrid, resizeMemory=False )
    elif dualGrid:
        grid = grid.hierarchicalGrid.dualGrid()
        # create empty updater dictionary
        grid._updaters = dict()
        grid = grid.leafView

    grid.hierarchicalGrid.update = types.MethodType(update, grid.hierarchicalGrid)
    return grid


registry = {}
registry["grid"] = {
        "Polygon": polygonGrid
    }


if __name__ == "__main__":
    import doctest
    doctest.testmod(optionflags=doctest.ELLIPSIS)
