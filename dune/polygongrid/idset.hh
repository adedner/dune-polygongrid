#ifndef DUNE_POLYGONGRID_IDSET_HH
#define DUNE_POLYGONGRID_IDSET_HH

#include <type_traits>

#include <dune/geometry/dimension.hh>

#include <dune/grid/common/indexidset.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // IdSet
    // -----

    template< class Grid >
    class IdSet
      : public Dune::IdSet< Grid, IdSet< Grid >, std::size_t >
    {
      typedef IdSet< Grid > This;
      typedef Dune::IdSet< Grid, This, std::size_t > Base;

      typedef typename std::remove_const< Grid >::type::Traits Traits;

    public:
      typedef typename Base::Id Id;

      static const dim_t dimension = 2;

      template< dim_t codim >
      struct Codim
      {
        typedef typename Traits::template Codim< codim >::Entity Entity;
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
      static constexpr Id id ( std::size_t index, std::size_t codim ) noexcept { return (id << 2) | codim; }
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_IDSET_HH
