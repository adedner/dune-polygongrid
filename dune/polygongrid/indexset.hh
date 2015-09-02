#ifndef DUNE_POLYGONGRID_INDEXSET_HH
#define DUNE_POLYGONGRID_INDEXSET_HH

#include <cassert>

#include <array>
#include <type_traits>

#include <dune/geometry/dimension.hh>
#include <dune/geometry/type.hh>

#include <dune/grid/common/gridenums.hh>
#include <dune/grid/common/indexidset.hh>

#include <dune/polygongrid/declaration.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // IndexSet
    // --------

    template< class ct >
    class IndexSet
      : public Dune::IndexSet< const PolygonGrid< ct >, IndexSet< ct >, std::size_t, std::array< GeometryType,1 > >
    {
      typedef IndexSet< ct > This;
      typedef Dune::IndexSet< const PolygonGrid< ct >, This, std::size_t, std::array< GeometryType,1 > > Base;

    public:
      static const dim_t dimension = 2;

      typedef typename Base::Index Index;
      typedef typename Base::Types Types;

      template< dim_t codim >
      struct Codim
      {
        typedef typename std::conditional< codim == 1, HalfEdge< ct >, Node< ct > >::type Item;

        typedef Dune::Entity< codim, __PolygonGrid::Entity< Item, codim > > Entity;
      };

      IndexSet () : size_{{ 0u, 0u, 0u }} {}

      template< class Entity >
      Index index ( const Entity &entity ) const
      {
        return index< Entity::codimension >( entity );
      }

      template< dim_t cd >
      Index index ( const typename Codim< cd >::Entity &entity ) const
      {
        return entity.impl().index();
      }

      template< class Entity >
      Index subIndex ( const Entity &entity, int i, dim_t codim ) const
      {
        return subIndex< Entity::codimension >( entity, i, codim );
      }

      template< dim_t cd >
      Index subIndex ( const typename Codim< cd >::Entity &entity, int i, dim_t codim ) const
      {
        return entity.impl().subIndex( codim, i );
      }

      Index size ( GeometryType type ) const { return (type.isNone() ? size( dimension - type.dimension() ) : 0u); }
      Index size ( dim_t codim ) const { assert( (codim >= 0) && (codim <= dimension) ); return size_[ codim ]; }

      template< class Entity >
      bool contains ( const Entity &entity ) const
      {
        return true;
      }

      Types types ( int codim ) const noexcept { return {{ GeometryType( GeometryType::None(), dimension - codim ) }}; }

      void update ( Index numPolygons, Index numEdges, Index numVertices ) { size_ = {{ numPolygons, numEdges, numVertices }}; }

    private:
      std::array< Index, dimension+1 > size_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_INDEXSET_HH
