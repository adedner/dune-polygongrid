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
    : public Dune::Grid< 2, 2, ct, __PolygonGrid::GridFamily< ct > >
  {
    typedef PolygonGrid< ct > This;
    typedef Dune::Grid< 2, 2, ct, __PolygonGrid::GridFamily< ct > > Base;

  public:
    typedef __PolygonGrid::GridFamily< ct > GridFamily;

    typedef typename GridFamily::Traits Traits;

    using Base::dimension;

    typedef __PolygonGrid::Mesh< ctype > Mesh;
    typedef __PolygonGrid::MeshType MeshType;

    PolygonGrid ( std::shared_ptr< Mesh > mesh, __PolygonGrid::MeshType type )
      : mesh_( mesh ), type_( type )
    {}

    int maxLevel () const { return 0; }

    std::size_t numBoundarySegments () const { return mesh().numBoundaries(); }

    MacroGridView macroGridView () const
    {
      typedef typename MacroGridView::Implementation GridViewImpl;
      return GridViewImpl( *this );
    }

    LevelGridView levelGridView ( int level ) const { assert( level == 0 ); return macroGridView; }
    LeafGridView leafGridView () const { return macroGridView(); }

    const GlobalIdSet &globalIdSet () const { return idSet_; }
    const LocalIdSet &localIdSet () const { return idSet_; }

    bool globalRefine ( int refCount ) {}

    bool mark ( int refCount, const typename Traits::template Codim< 0 >::Entity &entity ) const { return false; }
    int getMark ( const typename Traits::template Codim< 0 >::Entity &entity ) const { return 0; }

    bool preAdapt () { return false; }
    bool adapt () { return false; }
    void postAdapt () {}

    const CollectiveCommunication &comm () const { return comm_; }

    bool loadBalance () { return false; }

    template< class DataHandle >
    bool loadBalance ( DataHandle &data )
    {
      return false;
    }

    template< dim_t codim >
    typename Traits::template Codim< Seed::codimension >::Entity entity ( const Seed &seed ) const noexcept
    {
      typedef typename Traits::template Codim< Seed::codimension >::Entity::Implementation EntityImpl;
      typedef std::conditional( Seed::codimension == 1, __PolygonGrid::HalfEdge< ctype > : __PolygonGrid::Node< ct > )::type Item;
      return EntityImpl( Item( mesh(), seed.index() ) );
    }

    template< class Entity >
    int level ( const Entity &entity ) const
    {
      return 0;
    }

    const Mesh &mesh () const { return *mesh_; }
    MeshType meshType () const { return meshType_; }

  private:
    std::shared_ptr< Mesh > mesh_;
    __PolygonGrid::MeshType type_:
    CollectiveCommunication comm_;
    IdSet idSet_;
  };

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRID_HH
