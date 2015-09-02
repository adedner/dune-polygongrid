#ifndef DUNE_POLYGONGRID_INTERSECTION_HH
#define DUNE_POLYGONGRID_INTERSECTION_HH

#include <cassert>
#include <cstddef>

#include <type_traits>

#include <dune/common/exceptions.hh>

#include <dune/geometry/type.hh>

#include <dune/grid/common/intersection.hh>
#include <dune/grid/common/intersectioniterator.hh>
#include <dune/grid/common/normals.hh>

#include <dune/polygongrid/declaration.hh>
#include <dune/polygongrid/entity.hh>
#include <dune/polygongrid/geometry.hh>
#include <dune/polygongrid/meshobjects.hh>

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

      typedef __PolygonGrid::Entity< Node< ct >, 0 > EntityImpl;
      typedef __PolygonGrid::Geometry< Item, 1 > GeometryImpl;

    public:
      static const dim_t dimension = 2;
      static const dim_t codimension = 1;
      static const dim_t mydimension = dimension - codimension;

      typedef Dune::Entity< 0, EntityImpl > Entity;
      typedef Dune::Geometry< GeometryImpl > Geometry;
      typedef Geometry LocalGeometry;

      typedef typename Geometry::GlobalCoordinate GlobalCoordinate;
      typedef typename Geometry::LocalCoordinate LocalCoordinate;

    private:
      typedef ConstantLocalFunction< LocalCoordinate, GlobalCoordinate > OuterNormals;

    public:
      explicit Intersection ( Item item ) : item_( item ) {}

      bool conforming () const noexcept { return true; }

      bool boundary () const noexcept { return !neighbor(); }

      bool neighbor () const noexcept { return item().neighbor().regular(); }

      int boundaryId () const noexcept { return 1; }

      std::size_t boundarySegmentIndex () const noexcept { assert( boundary() ); return item().index(); }

      Entity inside () const { return EntityImpl( item().cell() ); }
      Entity outside () const { return EntityImpl( item().neighbor() ); }

      int indexInInside () const { return item().indexInCell(); }
      int indexInOutside () const { return item().indexInNeighbor(); }

      DUNE_INLINE constexpr GeometryType type () const noexcept { return GeometryType( GeometryType::None(), mydimension ); }

      Geometry geometry () const { return GeometryImpl( item() ); }

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
        GlobalCoordinate normal = outerNormal();
        return OuterNormals( type(), normal *= Math::one / normal.two_norm() );
      }

      const Item &item () const { return item_; }

    private:
      GlobalCoordinate outerNormal () const
      {
        const GlobalCoordinate tangent = (item().target().position() - item().flip().target().position());
        return GlobalCoordinate{ -tangent[ 1 ], tangent[ 0 ] };
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
      typedef typename HalfEdges< ct >::Iterator HalfEdgeIterator;

      typedef Dune::Intersection< const PolygonGrid< ct >, IntersectionImpl > Intersection;

      IntersectionIterator () = default;

      explicit IntersectionIterator ( const HalfEdgeIterator halfEdgeIterator ) : halfEdgeIterator_( halfEdgeIterator ) {}

      DUNE_INLINE void increment () { ++halfEdgeIterator_; }

      DUNE_INLINE Intersection dereference () const { return IntersectionImpl( *halfEdgeIterator_ ); }

      DUNE_INLINE bool equals ( const This &other ) const { return (halfEdgeIterator_ == other.halfEdgeIterator_); }

    private:
      HalfEdgeIterator halfEdgeIterator_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_INTERSECTION_HH
