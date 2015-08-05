#ifndef DUNE_POLYGONGRID_ENTITYITERATOR_HH
#define DUNE_POLYGONGRID_ENTITYITERATOR_HH

#include <cassert>
#include <cstddef>

#include <type_traits>

#include <dune/geometry/dimension.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // EntityIterator
    // --------------

    template< dim_t codim, class Grid >
    class EntityIterator
    {
      typedef EntityIterator< codim, Grid > This;

      typedef typename std::remove_const< Grid >::type::Traits Traits;

    public:
      static const dim_t codimension = codim;
      static const dim_t dimension = Traits::dimension;
      static const dim_t mydimension = dimension - codimension;

      typedef typename Traits::template Codim< codimension >::Entity Entity;

    protected:
      typedef typename Entity::Implementation EntityImpl;

    public:
      EntityIterator () = default;

      EntityIterator ( const Grid &grid, std::size_t index )
        : grid_( &grid ), index_( index )
      {}

      Entity dereference () const { return EntityImpl( grid(), index_ ); }

      bool equals ( const This &other ) const noexcept { return (index_ == other.index_); }

      void increment () noexcept { ++index_; }

      const Grid &grid () const noexcept { assert( grid_ ); return *grid_; }

    protected:
      const Grid *grid_ = nullptr;
      std::size_t index_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITYITERATOR_HH
