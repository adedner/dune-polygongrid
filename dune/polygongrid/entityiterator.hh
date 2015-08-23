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

    template< class Item, dim_t codim >
    class EntityIterator
    {
      typedef EntityIterator< Item, codim > This;

      typedef __PolygonGrid::Entity< Item, codim > EntityImpl;

    public:
      static const dim_t codimension = codim;
      static const dim_t dimension = 2;
      static const dim_t mydimension = dimension - codimension;

      typedef Dune::Entity< codim, EntityImpl > Entity;

      EntityIterator () = default;

      explicit EntityIterator ( IndexIterator< Item > iterator ) : iterator_( iterator ) {}

      Entity dereference () const { return EntityImpl( *iterator_ ); }

      bool equals ( const This &other ) const noexcept { return (iterator_ == other.iterator_); }

      void increment () noexcept { ++iterator_; }

    protected:
      IndexIterator< Item > iterator_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITYITERATOR_HH
