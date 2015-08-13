#ifndef DUNE_POLYGONGRID_MESH_HH
#define DUNE_POLYGONGRID_MESH_HH

#include <cstddef>

#include <algorithm>
#include <array>
#include <iostream>
#include <utility>
#include <vector>

#include <dune/common/fvector.hh>
#include <dune/common/math.hh>

#include <dune/polygongrid/multivector.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // MeshType
    // --------

    enum MeshType : std::size_t { Primal = 0u, Dual = 1u };

    inline static constexpr MeshType dual ( MeshType type ) noexcept { return static_cast< MeshType >( type^1u ); }

    inline std::ostream &operator<< ( std::ostream &out, MeshType type ) { return out << (type == Primal ? "primal" : "dual"); }



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

    template< MeshType type >
    struct NodeTag
    {};

    template< MeshType type >
    using NodeIndex = Index< NodeTag< type > >;



    // HalfEdgeIndex
    // -------------

    template< MeshType type >
    struct HalfEdgeTag
    {};

    template< MeshType type >
    using HalfEdgeIndex = Index< HalfEdgeTag< type > >;



    // Type Definitions
    // ----------------

    typedef std::pair< std::size_t, std::size_t > IndexPair;

    typedef std::array< MultiVector< IndexPair >, 2 > MeshStructure;



    // External Forward Declarations
    // -----------------------------

    MultiVector< std::size_t > boundaries ( std::size_t numVertices, const MultiVector< std::size_t > &polygons );

    void printStructure ( const MultiVector< IndexPair > &structure, std::ostream &out = std::cout );

    MeshStructure meshStructure ( std::size_t numVertices, const MultiVector< std::size_t > &polygons, const MultiVector< std::size_t > &boundaries );

    bool checkStructure ( const MeshStructure &structure, MeshType type, std::ostream &out = std::cout );
    bool checkStructure ( const MeshStructure &structure, std::ostream &out = std::cout );



    // vertexPositions
    // ---------------

    template< class V >
    inline std::array< std::vector< V >, 2 > positions ( const MeshStructure &structure, const std::vector< V > &vertices )
    {
      typedef typename FieldTraits< V >::field_type ctype;

      const std::size_t numVertices = vertices.size();
      const std::size_t numBoundaries = (structure[ Dual ].size() - numVertices) / 2u;
      const std::size_t numPolygons = (structure[ Primal ].size() - 2u*numBoundaries);

      std::array< std::vector< V >, 2 > positions;
      positions[ Primal ].resize( structure[ Primal ].size(), Math::zero );
      positions[ Dual ].resize( structure[ Dual ].size(), Math::zero );

      // copy given vertex positions
      std::copy( vertices.begin(), vertices.end(), positions[ Dual ].begin() );

      // for now, use the average of polygon vertices as center position
      for( std::size_t i = 0u; i < numPolygons; ++i )
      {
        for( IndexPair j : structure[ Primal ][ i ] )
          positions[ Primal ][ i ] += positions[ Dual ][ j.first ];
        positions[ Primal ][ i ] *= Math::one / ctype( structure[ Primal ][ i ].size() );
      }

      // positions for boundary edge cells
      for( std::size_t i = numPolygons; i < numPolygons + numBoundaries; ++i )
      {
        for( std::size_t j = 0u; j < 2u; ++j )
          axpy( Math::one / ctype( 2 ), positions[ Dual ][ structure[ Primal ][ i ][ j ] ], positions[ Primal ][ i ] );
      }

      // positions for boundary vertex cells
      for( std::size_t i = numPolygons + numBoundaries; i < numPolygons + 2u*numBoundaries; ++i )
        positions[ Primal ][ i ] = positions[ Dual ][ structure[ Primal ][ i ][ 0 ] ];

      // positions for dual boundaries
      for( std::size_t i = 0u; i < numBoundaries; ++i )
      {
        for( std::size_t j = 0u; j < 2u; ++j )
        {
          const std::size_t v = structure[ Primal ][ numPolygons + i ][ j ].first;
          axpy( Math::one / ctype( 2 ), positions[ Dual ][ v ], positions[ Dual ][ numVertices + 2*i+j ] );
          axpy( Math::one / ctype( 2 ), positions[ Primal ][ numPolygons + i ], positions[ Dual ][ numVertices + 2*i+j ] );
        }
      }

      return std::move( positions );
    }



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

      Mesh ( const std::vector< GlobalCoordinate > &vertices, const MultiVector< std::size_t > &polygons )
        : numRegular_{{ polygons.size(), vertices.size() }}
      {
        MultiVector< std::size_t > boundaries = __PolygonGrid::boundaries( numRegular_[ Dual ], polygons );
        structure_ = __PolygonGrid::meshStructure( numRegular_[ Dual ], polygons, boundaries );
        positions_ = __PolygonGrid::positions( structure_, vertices );
      }

      template< MeshType type >
      NodeIndex< type > target ( HalfEdgeIndex< type > index ) const noexcept
      {
        return NodeIndex< type >( indexPair( index ).first );
      }

      template< MeshType type >
      const GlobalCoordinate &position ( NodeIndex< type > index ) const noexcept
      {
        assert( index < positions_[ type ].size() );
        return positions_[ type ][ index ];
      }

      template< MeshType type >
      HalfEdgeIndex< dual( type ) > dual ( HalfEdgeIndex< type > index ) const noexcept
      {
        return halfEdgeIndex< dual( type ) >( indexPair( index ) );
      }

      template< MeshType type >
      HalfEdgeIndex< type > flip ( HalfEdgeIndex< type > index ) const noexcept
      {
        return dual( dual( index ) );
      }

      template< MeshType type >
      std::size_t size ( NodeIndex< type > index ) const noexcept
      {
        return structure_[ type ].size();
      }

      template< MeshType type >
      HalfEdgeIndex< dual( type ) > begin ( NodeIndex< type > index ) const noexcept
      {
        return HalfEdgeIndex< dual( type ) >( structure_[ dual( type ) ].begin_of( index ) );
      }

      template< MeshType type >
      HalfEdgeIndex< dual( type ) > end ( NodeIndex< type > index ) const noexcept
      {
        return HalfEdgeIndex< dual( type ) >( structure_[ dual( type ) ].end_of( index ) );
      }

    private:
      template< MeshType type >
      HalfEdgeIndex< type > halfEdgeIndex ( const IndexPair &indexPair ) const noexcept
      {
        return HalfEdgeIndex< type >( structure_[ type ].position_of( indexPair ) );
      }

      template< MeshType type >
      const IndexPair &indexPair ( HalfEdgeIndex< type > index ) const noexcept
      {
        assert( index < structure_[ type ].values().size() );
        return structure_[ type ].values()[ index ];
      }

      std::array< std::size_t, 2 > numRegular_;
      MeshStructure structure_;
      std::array< std::vector< GlobalCoordinate >, 2 > positions_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MESH_HH
