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

    // Internal Forward Declarations
    // -----------------------------

    template< class, dim_t codim >
    class EntityIterator;



    // EntityIterator for Node
    // -----------------------

    template< class ct, dim_t codim >
    class EntityIterator< Node< ct >, codim >
    {
      typedef EntityIterator< Node< ct >, codim > This;

      typedef IndexIterator< Node< ct > > Iterator;
      typedef __PolygonGrid::Entity< Node< ct >, codim > EntityImpl;

    public:
      static const dim_t codimension = codim;
      static const dim_t dimension = 2;
      static const dim_t mydimension = dimension - codimension;

      typedef Dune::Entity< codim, EntityImpl > Entity;

      EntityIterator () = default;

      explicit EntityIterator ( Iterator iterator ) : iterator_( iterator ) {}

      Entity dereference () const { return EntityImpl( *iterator_ ); }

      bool equals ( const This &other ) const noexcept { return (iterator_ == other.iterator_); }

      void increment () noexcept { ++iterator_; }

    protected:
      Iterator iterator_;
    };



    // EntityIterator for HalfEdge
    // ---------------------------

    template< class ct, dim_t codim >
    class EntityIterator< HalfEdge< ct >, codim >
    {
      typedef EntityIterator< HalfEdge< ct >, codim > This;

      typedef IndexIterator< HalfEdge< ct > > Iterator;
      typedef __PolygonGrid::Entity< HalfEdge< ct >, codim > EntityImpl;

    public:
      static const dim_t codimension = codim;
      static const dim_t dimension = 2;
      static const dim_t mydimension = dimension - codimension;

      typedef Dune::Entity< codim, EntityImpl > Entity;

      EntityIterator () = default;

      explicit EntityIterator ( Iterator iterator ) : iterator_( iterator ) {}

      Entity dereference () const { return EntityImpl( *iterator_ ); }

      bool equals ( const This &other ) const noexcept { return (iterator_ == other.iterator_); }

      // todo: only stop once on each half edge
      void increment () noexcept { ++iterator_; }

    protected:
      Iterator iterator_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITYITERATOR_HH
