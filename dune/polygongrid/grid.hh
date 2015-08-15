#ifndef DUNE_POLYGONGRID_GRID_HH
#define DUNE_POLYGONGRID_GRID_HH

#include <dune/geometry/dimension.hh>

#include <dune/grid/common/grid.hh>

#include <dune/grid/polygongrid/gridfamily.hh>

namespace Dune
{

  // PolygonGrid
  // -----------

  template< class ct >
  class PolygonGrid
    : public GridDefaultImplementation< 2, 2, ct, __PolygonGrid::GridFamily< ct > >
  {
    typedef PolygonGrid< ct > This;
    typedef GridDefaultImplementation< 2, 2, ct, __PolygonGrid::GridFamily< ct > > Base;

  public:
    typedef __PolygonGrid::GridFamily< ct > GridFamily;

    typedef typename GridFamily::Traits Traits;

    static const dim_t dimension = 2;
    static const dim_t dimensionworld = 2;

    template< dim_t codim >
    typename Traits::template Codim< Seed::codimension >::Entity entity ( const Seed &seed ) const noexcept
    {
      typedef typename Traits::template Codim< Seed::codimension >::Entity::Implementation EntityImpl;
      typedef std::conditional( Seed::codimension == 1, __PolygonGrid::HalfEdge< ctype > : __PolygonGrid::Node< ctype > )::type Item;
      return EntityImpl( Item( mesh(), seed.index() ) );
    }

  private:
    typedef __PolygonGrid::Mesh< ctype > Mesh;

    const Mesh &mesh () const { return *mesh_; }

    std::shared_ptr< Mesh > mesh_;
  };



  // Implementation of PolygonGrid
  // -----------------------------

  template< class ct >
  const dim_t PolygonGrid< ct >::dimension;

  template< class ct >
  const dim_t PolygonGrid< ct >::dimensionworld;

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRID_HH
