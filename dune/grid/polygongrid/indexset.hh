#ifndef DUNE_POLYGONGRID_INDEXSET_HH
#define DUNE_POLYGONGRID_INDEXSET_HH

#include <vector>

#include <dune/common/nullptr.hh>
#include <dune/common/typetraits.hh>

#include <dune/grid/common/gridenums.hh>
#include <dune/grid/common/indexidset.hh>

#include <dune/grid/idgrid/declaration.hh>

namespace Dune
{

  // PolygonGridIndexSet
  // --------------

  template< class Grid >
  class PolygonGridIndexSet
  : public IndexSet< Grid, PolygonGridIndexSet< Grid >, typename remove_const< Grid >::type::Traits::Index >
  {
    typedef PolygonGridIndexSet< Grid, HostIndexSet > This;
    typedef IndexSet< Grid, PolygonGridIndexSet< Grid >, typename remove_const< Grid >::type::Traits::Index > Base;

    typedef typename remove_const< Grid >::type::Traits Traits;

  public:
    static const int dimension = Traits::dimension;

    typedef typename Base::IndexType IndexType;

    PolygonGridIndexSet ( const Grid &grid )
    : grid_( &grid )
    {}

    PolygonGridIndexSet ( const This &other )
    : grid_( other.grid_ )
    {}

    const This &operator= ( const This &other )
    {
      grid_ = other.grid_;
      return *this;
    }

    template< class Entity >
    IndexType index ( const Entity &entity ) const
    {
      return index< Entity::codimension >( entity );
    }

    template< int cd >
    IndexType index ( const typename Traits::template Codim< cd >::Entity &entity ) const
    {
      return entity.impl().index();
    }

    template< class Entity >
    IndexType subIndex ( const Entity &entity, int i, unsigned int codim ) const
    {
      return subIndex< Entity::codimension >( entity, i, codim );
    }

    template< int cd >
    IndexType subIndex ( const typename Traits::template Codim< cd >::Entity &entity, int i, unsigned int codim ) const
    {
      return entity.impl().subEntity( i, codim )->impl().index();
    }

    IndexType size ( GeometryType type ) const { return grid().size( type ); }
    IndexType size ( int codim ) const { return grid().size( codim ); }

    template< class Entity >
    bool contains ( const Entity &entity ) const { return true; }

    const std::vector< GeometryType > &geomTypes ( int codim ) const
    {
      return grid().geomTypes_( codim );
    }

    const Grid &grid () const { return *grid_; }

  private:
    const Grid *grid_;
  };

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_INDEXSET_HH
