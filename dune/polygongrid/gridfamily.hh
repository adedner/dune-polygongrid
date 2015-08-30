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

      typedef typename GridViewImp::IntersetionIterator IntersectionIterator;
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

        typedef typename LeafGridView::IndexSet LeafIndexSet;
        typedef typename LevelGridView::IndexSet LevelIndexSet;

        template< dim_t codim >
        struct Codim
        {
          typedef typename MacroGridView::template Codim< codim >::Entity Entity;
          typedef typename MacroGridView::template Codim< codim >::EntitySeed EntitySeed;

          typedef typename MacroGridView::template Codim< codim >::Geometry Geometry;
          typedef typename MacroGridView::template Codim< codim >::LocalGeometry LocalGeometry;
        };
      };
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRIDFAMILY_HH
