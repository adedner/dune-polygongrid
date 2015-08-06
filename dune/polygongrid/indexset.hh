#ifndef DUNE_POLYGONGRID_INDEXSET_HH
#define DUNE_POLYGONGRID_INDEXSET_HH

#include <cassert>

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

      IndexSet () : size_{{ 0u, 0u, 0u }} {}

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

      Index size ( GeometryType type ) const { return (type.isNone() ? size( dimension - type.dimension() ) : 0u); }
      Index size ( dim_t codim ) const { assert( (codim >= 0) && (codim <= dimension) ); return size_[ codim ]; }

      template< class Entity >
      bool contains ( const Entity &entity ) const
      {
        return true;
      }

      Types types ( int codim ) const noexcept { return {{ GeometryType( GeometryType::None(), dimension - codim ); }}; }

      void update ( Index numPolygons, Index numEdges, Index numVertices ) { size_ = {{ numPolygons, numEdges, numVertices }}; }

    private:
      std::array< Index, dimension+1 > size_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_INDEXSET_HH
