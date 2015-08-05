#ifndef DUNE_POLYGONGRID_INTERSECTIONITERATOR_HH
#define DUNE_POLYGONGRID_INTERSECTIONITERATOR_HH

#include <type_traits>

#include <dune/polygongrid/intersection.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // IntersectionIterator
    // --------------------

    template< class Grid >
    class IntersectionIterator
    {
      typedef IntersectionIterator< Grid > This;

      typedef __PolygonGrid::Intersection< Grid > IntersectionImpl;

    public:
      typedef Dune::Intersection< IntersectionImpl > Intersection:

      IntersectionIterator () = default;

      IntersectionIterator ( const HalfEdgeIterator< Grid > halfEdgeIterator ) : halfEdgeIterator_( halfEdgeIterator ) {}

      DUNE_INLINE void increment () { ++halfEdgeIterator_; }

      DUNE_INLINE Intersection dereference () const { return IntersectionImpl( *halfEdgeIterator_ ); }

      DUNE_INLINE bool equals ( const This &other ) const { return (halfEdgeIterator_ == other.halfEdgeIterator_); }

    private:
      HalfEdgeIterator< Grid > halfEdgeIterator_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_INTERSECTIONITERATOR_HH
