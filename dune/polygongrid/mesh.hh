#ifndef DUNE_POLYGONGRID_MESH_HH
#define DUNE_POLYGONGRID_MESH_HH

#include <cstddef>

#include <utility>
#include <vector>

#include <dune/common/fvector.hh>

#include <dune/polygongrid/multivector.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // MeshType
    // --------

    enum MeshType : std::size_t { Primal = 0u, Dual = 1u };

    inline static constexpr MeshType dual ( MeshType type ) noexcept { return static_Cast< MeshType >( type^1u ); }



    // Index
    // -----

    template< class Tag >
    class Index
    {
      typedef Index< Tag > This;

    public:
      explicit Index ( std::size_t index = std::numeric_limits< std::size_t >::max() ) : index_( index ) {}

      operator std::size_t () const noexcept { return index_; }

      explicit operator bool () const noexcept { return (index_ < std::numeric_limits< std::size_t >::max()); }

      bool operator== ( const This &other ) const noexcept { return (index_ == other.index_); }
      bool operator!= ( const This &other ) const noexcept { return (index_ != other.index_); }
      bool operator< ( const This &other ) const noexcept { return (index_ < other.index_); }
      bool operator<= ( const This &other ) const noexcept { return (index_ <= other.index_); }
      bool operator> ( const This &other ) const noexcept { return (index_ > other.index_); }
      bool operator>= ( const This &other ) const noexcept { return (index_ >= other.index_); }

      This &operator++ () noexcept { ++index_; return *this; }
      This &operator-- () noexcept { --index_; return *this; }

      This &operator+= ( std::ptrdiff_t n ) noexcept { index_ += n; return *this; }
      This &operator-= ( std::ptrdiff_t n ) noexcept { index_ += n; return *this; }

      friend This operator+ ( This a, std::ptrdiff_t b ) noexcept { return a += b; }
      friend This operator+ ( std::ptrdiff_t a, This b ) noexcept { return b += a; }
      friend This operator- ( This a, std::ptrdiff_t b ) noexcept { return a -= b; }

      friend std::ptrdiff_t operator- ( This a, This b ) noexcept { return (a.index_ - b.index_); }

    private:
      std::size_t index_;
    };



    // NodeIndex
    // ---------

    template< class MeshType type >
    struct NodeTag
    {};

    template< class MeshType type >
    using NodeIndex = Index< NodeTag< type > >;



    // HalfEdgeIndex
    // -------------

    template< class MeshType type >
    struct HalfEdgeTag
    {};

    template< MeshType type >
    using HalfEdgeIndex = Index< HalfEdgeTag< type > >;



    // library functions
    // -----------------

    void halfEdges ();



    // Mesh
    // ----

    template< class ct >
    class Mesh
    {
      typedef Mesh< ct > This;

      typedef std::pair< std::size_t, std::size_t > Pair;

      static constexpr MeshType dual ( MeshType type ) noexcept { return __PolygonGrid::dual( type ); }

    public:
      typedef FieldVector< ct, 2 > GlobalCoordinate;

      Mesh ( std::vector< GlobalCoordinate > vertices, MultiVector< std::size_t > polygons );

      template< MeshType type >
      NodeIndex< type > target ( HalfEdgeIndex< type > index ) const noexcept
      {
        return NodeIndex< type >( pair( index ).first );
      }

      template< MeshType type >
      const GlobalCoordinate &position ( NodeIndex< type > index ) const noexcept
      {
        assert( index < position_[ type ].size() );
        return position_[ type ][ index ];
      }

      template< MeshType type >
      HalfEdgeIndex< dual( type ) > dual ( HalfEdgeIndex< type > index ) const noexcept
      {
        return halfEdgeIndex< dual( type ) >( pair( index ) );
      }

      template< MeshType type >
      HalfEdgeIndex< type > flip ( HalfEdgeIndex< type > index ) const noexcept
      {
        return dual( dual( index ) );
      }

      template< MeshType type >
      std::size_t size ( NodeIndex< type > index ) const noexcept
      {
        return (offset_[ type ][ index+1 ] - offset_[ type ][ index ]);
      }

      template< MeshType type >
      HalfEdgeIndex< dual( type ) > begin ( NodeIndex< type > index ) const noexcept
      {
        return HalfEdgeIndex< dual( type ) >( offset_[ dual( type ) ][ index ] );
      }

      template< MeshType type >
      HalfEdgeIndex< dual( type ) > end ( NodeIndex< type > index ) const noexcept
      {
        return HalfEdgeIndex< dual( type ) >( offset_[ dual( type ) ][ index+1 ] );
      }

    private:
      template< MeshType type >
      HalfEdgeIndex< type > halfEdgeIndex ( const Pair &pair ) const noexcept
      {
        return HalfEdgeIndex< type >( offset_[ type ][ pair.first ] + pair.second );
      }

      template< MeshType type >
      const Pair &pair ( HalfEdgeIndex< type > index ) const noexcept
      {
        assert( index < pair_[ type ].size() );
        return pair_[ type ][ index ];
      }

      std::array< std::vector< std::size_t >, 2 > offset_;
      std::array< std::vector< Pair >, 2 > pair_;
      std::array< std::vector< GlobalCoordinate >, 2 > position_;
    };



    // Implementation of Mesh
    // ----------------------

    template< class ct >
    inline Mesh< ct >::Mesh ( std::vector< GlobalCoordinate > position, MultiVector< std::size_t > polygons )
    {
      const std::size_t numVertices = position.size();
      const std::size_t numPolygons = polygons.size();

      // halfEdges();

      std::vector< std::size_t > count( numVertices, 0u );
      for( std::size_t vtx : polygons )
        count[ vtx ] += 2u;

      // compute all neighboring vertices
      MultiVector< std::size_t > halfEdges( count, std::numeric_limits< std::size_t >::max() );
      std::fill( count.begin(), count.end(), 0u );
      for( std::size_t i = 0u; i < numPolygons; ++i )
      {
        const std::size_t n = offset[ i+1 ] - offset[ i ];
        for( std::size_t j = 0u; j < n; ++j )
        {
          const std::size_t vtx = polygons[ offset[ i ] + j ];
          halfEdges[ Pair( vtx, count[ vtx ]++ ) ] = polygons[ Pair( i, (j+1)%n ) ];
          halfEdges[ Pair( vtx, count[ vtx ]++ ) ] = polygons[ Pair( i, (j+n-1)%n ) ];
        }
      }

      // make neighboring vertices unique
      halfEdges.sortEach();
      halfEdges.uniqueEach();

      // find boundary half edges
      const std::size_t numBoundarySegments = edgeOffset.back() - offset.back();

      // ...

      pair_[ Primal ].resize( offset.back() );
      for( std::size_t i = 0u; i < numPolygons; ++i )
      {
        const std::size_t n = offset[ i+1 ] - offset[ i ];
        for( std::size_t j = 0u; j < n; ++j )
        {
          const std::size_t vtx = polygons[ offset[ i ] + j ];
          const std::size_t m = edgeOffset[ vtx+1 ] - edgeOffset[ vtx ];
          const auto eit = edgeTarget.begin() + edgeOffset[ vtx ];
          const std::size_t k = std::distance( eit, std::find( eit, eit + m, polygons[ offset[ i ] + ((j+1)%n) ] ) );
          //pair_[ Primal ][ offset[ i ] + j ] = Pair( vtx, ((k+m-1)%m) );
          //pair_[ Dual ][ edgeOffset[ i ] + k ] = Pair( i, j );
        }
      }


      // ...


      position_[ Primal ] = std::move( position );
      offset_[ Primal ] = std::move( offset );


      pair_[ Primal ].resize( offset_.back() );
    }

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MESH_HH
