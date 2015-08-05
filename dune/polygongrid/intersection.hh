#ifndef DUNE_POLYGONGRID_INTERSECTION_HH
#define DUNE_POLYGONGRID_INTERSECTION_HH

#include <cstddef>

#include <type_traits>

#include <dune/common/exceptions.hh>

#include <dune/geometry/type.hh>

#include <dune/grid/common/normals.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // External forward declaration
    // ----------------------------

    template< class Grid >
    class IntersectionIterator;



    // Intersection
    // ------------

    template< class Grid >
    class Intersection
    {
      typedef Intersection< Grid > This;

      typedef typename std::remove_const< Grid >::type::Traits Traits;

    public:
      typedef typename Traits::template Codim< 0 >::Entity Entity;

      typedef typename Traits::template Codim< 1 >::Geometry Geometry;
      typedef typename Traits::template Codim< 1 >::LocalGeometry LocalGeometry;

      typedef typename Geometry::GlobalCoordinate GlobalCoordinate;
      typedef typename Geometry::LocalCoordinate LocalCoordinate;

    private:
      typedef ConstantLocalFunction< LocalCoordinate, GlobalCoordinate > OuterNormals;

    public:
      Intersection ( HalfEdge< Grid > halfEdge ) : halfEdge_( halfEdge ) {}

      bool conforming () const { return true; }

      bool boundary () const { return halfEdge_.boundary(); }

      bool neighbor () const { return !boundary(); }

      int boundaryId () const { return 1; }

      std::size_t boundarySegmentIndex () const { assert( halfEdge_.boundary() ); return halfEdge_.index(); }

      Entity inside () const { return EntityImpl( grid(), halfEdge_.polygon() ); }
      Entity outside () const { return EntityImpl( grid(), halfEdge_.flip().polygon() ); }

      int indexInInside () const
      {
        DUNE_THROW( InvalidStateException, "Intersection::indexInInside does not make for arbitrary polytopes." );
      }

      int indexInOutside () const
      {
        DUNE_THROW( InvalidStateException, "Intersection::indexInOutside does not make for arbitrary polytopes." );
      }

      DUNE_INLINE constexpr GeometryType type () const noexcept { return GeometryType( GeometryType::None(), mydimension ); }

      Geometry geometry () const
      {
        return inside().subEntity( indexInInside(), Codim< 1 >() ).geometry();
      }

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

      const Grid &grid () const { return halfEdge_.grid(); }

    private:
      GlobalCoordinate outerNormal () const
      {
        const GlobalCoordinate tangent = (halfEdge_.target() - halfEdge_.flip().target());
        return GlobalCoordinate( -tangent[ 1 ], tangent[ 0 ] );
      }

      HalfEdge< Grid > halfEdge_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_INTERSECTION_HH
