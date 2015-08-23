#ifndef DUNE_POLYGONGRID_GRIDFAMILY_HH
#define DUNE_POLYGONGRID_GRIDFAMILY_HH

#include <dune/common/parallel/mpicollectivecommunication.hh>

#include <dune/geometry/dimension.hh>

#include <dune/grid/common/entityseed.hh>
#include <dune/grid/common/grid.hh>

#include <dune/polygongrid/gridview.hh>

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
      using Codim = GridViewImp::Codim;

      typedef typename GridViewImp::IntersetionIterator IntersectionIterator;
      typedef typename IntersectionIterator::Intersection Intersection;

      typedef typename GridView::CollectiveCommunication CollectiveCommunication;

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

        typedef Dune::GridView< GridViewTraits< ct > > GridView;

        template< dim_t codim >
        struct Codim
        {
          typedef typename GridView::template Codim< codim >::Entity Entity;
          typedef typename GridView::template Codim< codim >::EntitySeed EntitySeed;

          typedef typename GridView::template Codim< codim >::Geometry Geometry;
          typedef typename GridView::template Codim< codim >::LocalGeometry LocalGeometry;
        };
      };
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRIDFAMILY_HH
