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

    template< class Index, dim_t codim >
    class EntitySeed
    {
      typedef EntitySeed< Item, codim > This;

    public:
      static const dim_t codimension = codim;

      EntitySeed () = default;
      explicit EntitySeed ( const Index &index ) : index_( index ) {}

      DUNE_INLINE bool isValid () const noexcept { return static_cast< bool >( index() ); }

      DUNE_INLINE bool operator== ( const This &other ) const noexcept { return (index() == other.index()); }
      DUNE_INLINE bool operator!= ( const This &other ) const noexcept { return (index() != other.index()); }

      DUNE_INLINE std::size_t index () const noexcept { return index_; }

    private:
      Index index_;
    };

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITYSEED_HH
