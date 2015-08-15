#ifndef DUNE_POLYGONGRID_GRID_HH
#define DUNE_POLYGONGRID_GRID_HH

#include <dune/geometry/dimension.hh>

#include <dune/grid/common/grid.hh>

#include <dune/grid/polygongrid/gridfamily.hh>

namespace Dune
{

  // Internal Forward Declarations
  // -----------------------------

  template< class ct, __PolygonGrid::MeshType type >
  class PolygonGrid;

  template< class ct, class Comm = MPI_Comm >
  using PrimalPolygonGrid = PolygonGrid< ct, __PolygonGrid::Primal >;

  template< class ct, class Comm = MPI_Comm >
  using DualPolygonGrid = PolygonGrid< ct, __PolygonGrid::Dual >;



  // PolygonGrid
  // -----------

  template< class ct, __PolygonGrid::MeshType type >
  class PolygonGrid
    : public GridDefaultImplementation< 2, 2, ct, __PolygonGrid::GridFamily< ct, type > >
  {
    typedef PolygonGrid< ct, type > This;
    typedef GridDefaultImplementation< 2, 2, ct, __PolygonGrid::GridFamily< ct, type > > Base;

  public:
    typedef __PolygonGrid::GridFamily< ct, type > GridFamily;

    typedef typename GridFamily::Traits Traits;

    static const dim_t dimension = 2;
    static const dim_t dimensionworld = 2;

    template< dim_t codim >
    typename Traits::template Codim< Seed::codimension >::Entity entity ( const Seed &seed ) const noexcept
    {
      typedef typename Traits::template Codim< Seed::codimension >::Entity::Implementation EntityImpl;
      typedef __PolygonGrid::Item< Seed::codimension, type > Item;
      return EntityImpl( Item( mesh(), seed.index() ) );
    }
  };



  // Implementation of PolygonGrid
  // -----------------------------

  template< class ct, __PolygonGrid::MeshType type >
  const dim_t PolygonGrid< ct, type >::dimension;

  template< class ct, __PolygonGrid::MeshType type >
  const dim_t PolygonGrid< ct, type >::dimensionworld;

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRID_HH
