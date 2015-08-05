#ifndef DUNE_POLYGONGRID_ENTITYSEED_HH
#define DUNE_POLYGONGRID_ENTITYSEED_HH

#include <cstddef>

#include <limits>
#include <type_traits>

#include <dune/common/inline.hh>

#include <dune/geometry/dimension.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // EntitySeed
    // ----------

    template< dim_t codim >
    class EntitySeed
    {
      typedef EntitySeed< codim > This;

    public:
      static const dim_t codimension = codim;

      explicit EntitySeed ( std::size_t index = std::numeric_limits< std::size_t >::max() ) : index_( index ) {}

      DUNE_INLINE bool isValid () const noexcept { return (index() < std::numeric_limits< std::size_t >::max()); }

      DUNE_INLINE bool operator== ( const This &other ) const noexcept { return (index() == other.index()); }
      DUNE_INLINE bool operator!= ( const This &other ) const noexcept { return (index() != other.index()); }

      DUNE_INLINE std::size_t index () const noexcept { return index_; }

    private:
      std::size_t index_;
    };

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITYSEED_HH
