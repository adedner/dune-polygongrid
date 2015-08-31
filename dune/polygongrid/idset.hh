#ifndef DUNE_POLYGONGRID_IDSET_HH
#define DUNE_POLYGONGRID_IDSET_HH

#include <type_traits>

#include <dune/geometry/dimension.hh>

#include <dune/grid/common/indexidset.hh>

#include <dune/polygongrid/declaration.hh>
#include <dune/polygongrid/entity.hh>
#include <dune/polygongrid/meshobjects.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // IdSet
    // -----

    template< class ct >
    class IdSet
      : public Dune::IdSet< const PolygonGrid< ct >, IdSet< ct >, std::size_t >
    {
      typedef IdSet< ct > This;
      typedef Dune::IdSet< const PolygonGrid< ct >, This, std::size_t > Base;

    public:
      typedef typename Base::Id Id;

      static const dim_t dimension = 2;

      template< dim_t codim >
      struct Codim
      {
        typedef typename std::conditional< codim == 1, HalfEdge< ct >, Node< ct > >::type Item;

        typedef Dune::Entity< codim, __PolygonGrid::Entity< Item, codim > > Entity;
      };

    public:
      template< class Entity >
      Id id ( const Entity &entity ) const
      {
        return id< Entity::codimension >( entity );
      }

      template< dim_t codim >
      Id id ( const typename Codim< codim >::Entity &entity ) const
      {
        return id( entity.impl().index(), codim );
      }

      template< class Entity >
      Id subId ( const Entity &entity, int i, dim_t codim ) const
      {
        return subId< Entity::codimension >( entity, i, codim );
      }

      template< dim_t cd >
      Id subId ( const typename Codim< cd >::Entity &entity, int i, dim_t codim ) const
      {
        return id( entity.impl().subIndex( i, codim ), codim );
      }

    private:
      static constexpr Id id ( std::size_t index, std::size_t codim ) noexcept { return (index << 2) | codim; }
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_IDSET_HH
