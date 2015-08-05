#ifndef DUNE_POLYGONGRID_INDEXSET_HH
#define DUNE_POLYGONGRID_INDEXSET_HH

#include <array>
#include <type_traits>

#include <dune/geometry/dimension.hh>

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
      : public Dune::IndexSet< Grid, IndexSet< Grid >, std::size_t, std::array< GeometryType > >
    {
      typedef IndexSet< Grid > This;
      typedef Dune::IndexSet< Grid, IndexSet< Grid >, std::size_t, std::array< GeometryType > > Base;

      typedef typename remove_const< Grid >::type::Traits Traits;

    public:
      static const dim_t dimension = 2;

      typedef typename Base::Index Index;

      IndexSet ( const Grid &grid ) : grid_( &grid ) {}

      template< class Entity >
      Index index ( const Entity &entity ) const
      {
        return index< Entity::codimension >( entity );
      }

      template< dim_t cd >
      Index index ( const typename Traits::template Codim< cd >::Entity &entity ) const
      {
        return entity.impl().index();
      }

      template< class Entity >
      Index subIndex ( const Entity &entity, int i, dim_t codim ) const
      {
        return subIndex< Entity::codimension >( entity, i, codim );
      }

      template< dim_t cd >
      Index subIndex ( const typename Traits::template Codim< cd >::Entity &entity, int i, dim_t codim ) const
      {
        return entity.impl().subIndex( i, codim );
      }

      Index size ( GeometryType type ) const { return grid().size( type ); }
      Index size ( dim_t codim ) const { return grid().size( codim ); }

      template< class Entity >
      bool contains ( const Entity &entity ) const { return true; }

      Types types ( int codim ) const noexcept { return {{ GeometryType( GeometryType::None(), dimension - codim ); }}; }

      const Grid &grid () const { return *grid_; }

    private:
      const Grid *grid_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_INDEXSET_HH
