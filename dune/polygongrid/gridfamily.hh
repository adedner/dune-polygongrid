#ifndef DUNE_POLYGONGRID_GRIDFAMILY_HH
#define DUNE_POLYGONGRID_GRIDFAMILY_HH

#include <dune/common/parallel/mpicollectivecommunication.hh>

#include <dune/geometry/dimension.hh>

#include <dune/grid/common/entityseed.hh>
#include <dune/grid/common/grid.hh>

#include <dune/polygongrid/declaration.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // GridFamily
    // ----------

    template< class ct >
    struct GridFamily
    {
      struct Traits
      {
        typedef ct ctype;

        typedef Dune::PolygonGrid< ct > Grid;

        template< dim_t codim >
        struct Codim
        {
          typedef typename std::conditional< codim == 1 ? HalfEdge< ctype > : Node< ctype > >::type Item;

          typedef Dune::Entity< codim, __PolygonGrid::Entity< Item, codim > > Entity;

          typedef Dune::EntitySeed< __PolygonGrid::EntitySeed< Item::Index, codim > > EntitySeed;
        };
      };
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRIDFAMILY_HH
