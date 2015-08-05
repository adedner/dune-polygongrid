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

    template< class ct, class idx, class Comm >
    struct GridFamily
    {
      struct Traits
      {
        typedef ct ctype;
        typedef idx Index;

        typedef Dune::PolygonGrid< ct, idx, Comm > Grid;

        template< dim_t codim >
        struct Codim
        {
          typedef Dune::EntitySeed< __PolygonGrid::EntitySeed< codim > > EntitySeed;
        };
      };
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRIDFAMILY_HH
