from __future__ import absolute_import, division, print_function, unicode_literals

from dune.common.checkconfiguration import assertHave, ConfigurationError

try:
    assertHave("HAVE_DUNE_POLYGONGRID")
except ConfigurationError:
    raise ImportError("DUNE module dune-polygongrid was not found.")


def polygonGrid(domain, ctype="double"):
    from ..grid.grid_generator import module, getDimgrid

    typeName = "Dune::PolygonGrid< " + ctype + " >"
    includes = ["dune/polygongrid/grid.hh", "dune/polygongrid/dgf.hh"]
    gridModule = module(includes, typeName)

    return gridModule.LeafGrid(gridModule.reader(domain))


registry = {}
registry["grid"] = {
        "Polygon": polygonGrid
    }


if __name__ == "__main__":
    import doctest
    doctest.testmod(optionflags=doctest.ELLIPSIS)
