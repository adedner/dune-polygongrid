from __future__ import absolute_import, division, print_function, unicode_literals

from dune.common.checkconfiguration import assertHave, ConfigurationError
from dune.generator import Method

from .blossoms import blossomDomain
from .voronoi import voronoiDomain

try:
    assertHave("HAVE_DUNE_POLYGONGRID")
except ConfigurationError:
    raise ImportError("DUNE module dune-polygongrid was not found.")


def polygonGrid(domain, ctype="double", dualGrid=False ):
    from ..grid.grid_generator import module, getDimgrid

    typeName = "Dune::PolygonGrid< " + ctype + " >"
    includes = ["dune/polygongrid/grid.hh", "dune/polygongrid/dgf.hh"]

    dualGrid = Method('dualGrid', '''[]( DuneType &self ) { return self.dualGrid(); }''' )
    gridModule = module(includes, typeName, dualGrid)

    grid = gridModule.LeafGrid(gridModule.reader(domain))
    if dualGrid:
        grid = grid.hierarchicalGrid.dualGrid()
        return grid.leafView
    else:
        return grid


registry = {}
registry["grid"] = {
        "Polygon": polygonGrid
    }


if __name__ == "__main__":
    import doctest
    doctest.testmod(optionflags=doctest.ELLIPSIS)
