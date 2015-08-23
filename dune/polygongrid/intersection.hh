#ifndef DUNE_POLYGONGRID_INTERSECTION_HH
#define DUNE_POLYGONGRID_INTERSECTION_HH

#include <cstddef>

#include <type_traits>

#include <dune/common/exceptions.hh>

#include <dune/geometry/type.hh>

#include <dune/grid/common/intersection.hh>
#include <dune/grid/common/intersectioniterator.hh>
#include <dune/grid/common/normals.hh>

#include <dune/polygongrid/entity.hh>
#include <dune/polygongrid/geometry.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Intersection
    // ------------

    template< class ct >
    class Intersection
    {
      typedef Intersection< ct > This;

      typedef HalfEdge< ct > Item;

      typedef __PolygonGrid::Entiy< Node< ct >, 0 > EntityImpl;
      typedef __PolygonGrid::Geometry< Item, codim > GeometryImpl;

    public:
      typedef Dune::Entity< 0, EntityImpl > Entity;
      typedef Dune::Geometry< GeometryImpl > Geometry;
      typedef Dune::Geometry< LocalGeometryImpl > LocalGeometry;

      typedef typename Geometry::GlobalCoordinate GlobalCoordinate;
      typedef typename Geometry::LocalCoordinate LocalCoordinate;

    private:
      typedef ConstantLocalFunction< LocalCoordinate, GlobalCoordinate > OuterNormals;

    public:
      Intersection ( Item item ) : item_( item ) {}

      bool conforming () const { return true; }

      bool boundary () const { return item_.boundary(); }

      bool neighbor () const { return !boundary(); }

      int boundaryId () const { return 1; }

      std::size_t boundarySegmentIndex () const { assert( item_.boundary() ); return item_.index(); }

      Entity inside () const { return EntityImpl( grid(), item_.flip().cell() ); }
      Entity outside () const { return EntityImpl( grid(), item_.cell() ); }

      int indexInInside () const
      {
        DUNE_THROW( InvalidStateException, "Intersection::indexInInside does not make for arbitrary polytopes." );
      }

      int indexInOutside () const
      {
        DUNE_THROW( InvalidStateException, "Intersection::indexInOutside does not make for arbitrary polytopes." );
      }

      DUNE_INLINE constexpr GeometryType type () const noexcept { return GeometryType( GeometryType::None(), mydimension ); }

      Geometry geometry () const { return GeometryImpl( item_ ); } }

      LocalGeometry geometryInInside () const
      {
        DUNE_THROW( InvalidStateException, "Intersection::geometryInOutside does not make for arbitrary polytopes." );
      }

      LocalGeometry geometryInOutside () const
      {
        DUNE_THROW( InvalidStateException, "Intersection::geometryInOutside does not make for arbitrary polytopes." );
      }

      OuterNormals integrationOuterNormals () const { return OuterNormals( type(), outerNormal() ); }

      OuterNormals outerNormals () const { return OuterNormals( type(), outerNormal() ); }

      OuterNormals unitOuterNormals () const
      {
        GlobalCoordinate normal = outerNormal< false >();
        return OuterNormals( type(), normal *= Math::one / normal.two_norm() );
      }

    private:
      GlobalCoordinate outerNormal () const
      {
        const GlobalCoordinate tangent = (item_.target().position() - item_.flip().target().position());
        return GlobalCoordinate( -tangent[ 1 ], tangent[ 0 ] );
      }

      Item item_;
    };



    // IntersectionIterator
    // --------------------

    template< class ct >
    class IntersectionIterator
    {
      typedef IntersectionIterator< ct > This;

      typedef __PolygonGrid::Intersection< ct > IntersectionImpl;

    public:
      typedef Dune::Intersection< IntersectionImpl > Intersection:

      IntersectionIterator () = default;

      IntersectionIterator ( const HalfEdgeIterator< ct > halfEdgeIterator ) : halfEdgeIterator_( halfEdgeIterator ) {}

      DUNE_INLINE void increment () { ++halfEdgeIterator_; }

      DUNE_INLINE Intersection dereference () const { return IntersectionImpl( *halfEdgeIterator_ ); }

      DUNE_INLINE bool equals ( const This &other ) const { return (halfEdgeIterator_ == other.halfEdgeIterator_); }

    private:
      HalfEdgeIterator< ct > halfEdgeIterator_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_INTERSECTION_HH
