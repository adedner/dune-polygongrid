#ifndef DUNE_POLYGONGRID_GRIDFAMILY_HH
#define DUNE_POLYGONGRID_GRIDFAMILY_HH

#include <dune/common/parallel/mpicollectivecommunication.hh>

#include <dune/geometry/dimension.hh>

#include <dune/grid/common/entityseed.hh>
#include <dune/grid/common/grid.hh>

#include <dune/polygongrid/gridview.hh>
#include <dune/polygongrid/idset.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // GridViewTraits
    // --------------

    template< class ct >
    struct GridViewTraits
    {
      typedef __PolygonGrid::GridView< ct > GridViewImp;

      typedef ct ctype;

      typedef typename GridViewImp::Grid Grid;
      typedef typename GridViewImp::IndexSet IndexSet;

      template< dim_t codim >
      using Codim = typename GridViewImp::template Codim< codim >;

      typedef typename GridViewImp::IntersectionIterator IntersectionIterator;
      typedef typename IntersectionIterator::Intersection Intersection;

      typedef typename GridViewImp::CollectiveCommunication CollectiveCommunication;

      static const bool conforming = true;
    };



    // GridFamily
    // ----------

    template< class ct >
    struct GridFamily
    {
      struct Traits
      {
        typedef ct ctype;

        typedef Dune::PolygonGrid< ct > Grid;

        typedef Dune::GridView< GridViewTraits< ct > > MacroGridView;
        typedef MacroGridView LeafGridView;
        typedef MacroGridView LevelGridView;

        typedef __PolygonGrid::IdSet< ct > LocalIdSet;
        typedef LocalIdSet GlobalIdSet;

        typedef __PolygonGrid::IndexSet< ct > LeafIndexSet;
        typedef __PolygonGrid::IndexSet< ct > LevelIndexSet;

        typedef Dune::CollectiveCommunication< No_Comm > CollectiveCommunication;

        template< dim_t codim >
        struct Codim
        {
          typedef typename std::conditional< codim == 1, HalfEdge< ct >, Node< ct > >::type Item;

          typedef LocalNoneGeometryTypeIndexSet< 2 - codim > GeometryTypeIndexSet;

          typedef Dune::Entity< codim, __PolygonGrid::Entity< Item, codim > > Entity;

          typedef typename Entity::EntitySeed EntitySeed;
          typedef typename Entity::Geometry Geometry;

          // local geometry does not make sense; add a phony typedef
          typedef Geometry LocalGeometry;
        };
      };
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRIDFAMILY_HH
