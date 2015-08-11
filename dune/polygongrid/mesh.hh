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



    // Type Definitions
    // ----------------

    typedef std::pair< std::size_t, std::size_t > IndexPair;

    typedef std::array< MultiVector< IndexPair >, 2 > MeshStructure;



    // External Forward Declarations
    // -----------------------------

    MultiVector< std::size_t > boundaries ( std::size_t numVertices, const MultiVector< std::size_t > &polygons );

    MeshStructure meshStructure ( std::size_t numVertices, const MultiVector< std::size_t > &polygons, const MultiVector< std::size_t > &boundaries );



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

      Mesh ( const std::vector< GlobalCoordinate > &vertices, const MultiVector< std::size_t > &polygons );

      template< MeshType type >
      NodeIndex< type > target ( HalfEdgeIndex< type > index ) const noexcept
      {
        return NodeIndex< type >( indexPair( index ).first );
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

      MeshStructure structure_;
      std::array< std::vector< GlobalCoordinate >, 2 > position_;
    };



    // Implementation of Mesh
    // ----------------------

    template< class ct >
    inline Mesh< ct >::Mesh ( const std::vector< GlobalCoordinate > &position, const MultiVector< std::size_t > &polygons )
    {
      const std::size_t numVertices = position.size();

      MultiVector< std::size_t > boundaries = __polygonGrid::boundaries( numVertices, polygons );
      structure_ = __polygonGrid::meshStructure( numVertices, polygons, boundaries );

    }

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MESH_HH
