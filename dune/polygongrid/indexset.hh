#ifndef DUNE_POLYGONGRID_INDEXSET_HH
#define DUNE_POLYGONGRID_INDEXSET_HH

#include <typetraits>
#include <vector>

#include <dune/grid/common/gridenums.hh>
#include <dune/grid/common/indexidset.hh>

#include <dune/polynomgrid/declaration.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // IndexSet
    // --------

    template< class Grid >
    class IndexSet
      : public Dune::IndexSet< Grid, IndexSet< Grid >, typename std::remove_const< Grid >::type::Traits::Index >
    {
      typedef IndexSet< Grid, HostIndexSet > This;
      typedef Dune::IndexSet< Grid, IndexSet< Grid >, typename std::remove_const< Grid >::type::Traits::Index > Base;

      typedef typename remove_const< Grid >::type::Traits Traits;

    public:
      static const int dimension = Traits::dimension;

      typedef typename Base::Index Index;

      IndexSet ( const Grid &grid ) : grid_( &grid ) {}

      template< class Entity >
      Index index ( const Entity &entity ) const
      {
        return index< Entity::codimension >( entity );
      }

      template< int cd >
      Index index ( const typename Traits::template Codim< cd >::Entity &entity ) const
      {
        return entity.impl().index();
      }

      template< class Entity >
      Index subIndex ( const Entity &entity, int i, unsigned int codim ) const
      {
        return subIndex< Entity::codimension >( entity, i, codim );
      }

      template< int cd >
      Index subIndex ( const typename Traits::template Codim< cd >::Entity &entity, int i, unsigned int codim ) const
      {
        return entity.impl().subIndex( i, codim );
      }

      Index size ( GeometryType type ) const { return grid().size( type ); }
      Index size ( int codim ) const { return grid().size( codim ); }

      template< class Entity >
      bool contains ( const Entity &entity ) const { return true; }

      const std::vector< GeometryType > &types ( int codim ) const
      {
        return grid().geomTypes_( codim );
      }

      const Grid &grid () const { return *grid_; }

    private:
      const Grid *grid_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_INDEXSET_HH
