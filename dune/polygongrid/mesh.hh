#ifndef DUNE_POLYGONGRID_MESH_HH
#define DUNE_POLYGONGRID_MESH_HH

#include <cstddef>

#include <utility>
#include <vector>

#include <dune/common/fvector.hh>
#include <dune/common/inline.hh>
#include <dune/common/iterator/tags.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // MeshType
    // --------

    enum MeshType : std::size_t { Primal = 0u, Dual = 1u };

    inline static constexpr MeshType dual ( MeshType type ) { return static_Cast< MeshType >( type^1u ); }



    // NodeIndex
    // ---------

    template< MeshType type >
    struct NodeIndex
    {
      explicit NodeIndex ( std::size_t index = std::numeric_limits< std::size_t >::max() ) : index_( index ) {}

      explicit operator std::size_t () const { return index_; }

      explicit operator bool () const { return (index_ < std::numeric_limits< std::size_t >::max()); }

    private:
      std::size_t index_;
    };



    // HalfEdgeIndex
    // -------------

    template< MeshType type >
    struct HalfEdgeIndex
    {
      explicit HalfEdgeIndex ( std::size_t index = std::numeric_limits< std::size_t >::max() ) : index_( index ) {}

      explicit operator std::size_t () const { return index_; }

      explicit operator bool () const { return (index_ < std::numeric_limits< std::size_t >::max()); }

    private:
      std::size_t index_;
    };



    // Mesh
    // ----

    template< class ct >
    class Mesh
    {
      typedef Mesh< ct > This;

      typedef std::pair< std::size_t, std::size_t > Pair;

    public:
      typedef FieldVector< ct, 2 > GlobalCoordinate;

      template< MeshType type >
      NodeIndex< type > target ( HalfEdgeIndex< type > index ) const noexcept
      {
        return NodeIndex< type >( pair( index ).first );
      }

      template< MeshType type >
      const GlobalCoordinate &position ( NodeIndex< type > index ) const noexcept
      {
        assert( static_cast< std::size_t >( index ) < position_[ type ].size() );
        return position_[ type ][ static_cast< std::size_t >( index ) ];
      }

      template< MeshType type >
      HalfEdgeIndex< __PolygonGrid::dual( type ) > dual ( HalfEdgeIndex< type > index ) const noexcept
      {
        return halfEdgeIndex< __PolygonGrid::dual( type ) >( pair( index ) );
      }

      template< MeshType type >
      HalfEdgeIndex< type > flip ( HalfEdgeIndex< type > index ) const noexcept
      {
        return dual( dual( index ) );
      }

      template< MeshType type >
      std::size_t size ( NodeIndex< type > index ) const noexcept
      {
        return (offset_[ type ][ static_cast< std::size_t >( index )+1 ] - offset_[ type ][ static_cast< std::size_t >( index ) ]);
      }

      template< MeshType type >
      HalfEdgeIndex< __PolygonGrid::dual( type ) > begin ( NodeIndex< type > index ) const noexcept
      {
        return halfEdgeIndex< __PolygonGrid::dual( type ) >( Pair( static_cast< std::size_t >( index ), 0 ) );
      }

      template< MeshType type >
      HalfEdgeIndex< __PolygonGrid::dual( type ) > end ( NodeIndex< type > index ) const noexcept
      {
        return halfEdgeIndex< __PolygonGrid::dual( type ) >( Pair( static_cast< std::size_t >( index ), size( index ) ) );
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
        assert( static_cast< std::size_t >( index ) < pair_[ type ].size() );
        return pair_[ type ][ static_cast< std::size_t >( index ) ];
      }

      std::array< std::vector< std::size_t >, 2 > offset_;
      std::array< std::vector< Pair >, 2 > pair_;
      std::array< std::vector< GlobalCoordinate >, 2 > position_;
    };



    // Node
    // ----

    template< class ct, MeshType type >
    class Node
    {
      typedef Node< ct, type > This;

    public:
      typedef __PolygonGrid::Mesh< ct > Mesh;

      typedef typename Mesh::GlobalCoordinate GlobalCoordinate;

      DUNE_INLINE const GlobalCoordinate &position () const noexcept { return mesh().position( index_ ); }

      DUNE_INLINE const Mesh &mesh () const { return *mesh_; }

    private:
      const Mesh *mesh_;
      NodeIndex< type > index_;
    };



    // HalfEdge
    // --------

    template< class ct, MeshType type = Primal >
    class HalfEdge
    {
      typedef HalfEdge< ct, type > This;

    public:
      typedef __PolygonGrid::Mesh< ct > Mesh;
      typedef __PolygonGrid::Node< ct, type > Vertex;
      typedef __PolygonGrid::Node< ct, dual( type ) > Polygon;

      HalfEdge ( const Mesh &mesh, HalfEdgeIndex< type > index ) : mesh_( &mesh ), index_( index ) {}

      bool boundary () const { return flip().polygon().boundary(); }

      This flip () const { return This( mesh(), mesh().flip( index ) );

      Vertex target () const { return Vertex( mesh(), mesh().target( index_ ) ); }

      Polygon polygon () const { return Polygon( mesh(), mesh().target( mesh().dual( index_ ) ) ); }

      const Mesh &mesh () const { return *mesh_; }

    private:
      const Mesh *mesh_;
      HalfEdgeIndex< type > index_;
    };



    // HalfEdgeIterator
    // ----------------

    template< class ct, MeshType type >
    class HalfEdgeIterator
      : public VirtualIterator< std::random_access_iterator_tag, HalfEdge< ct, type > >
    {
      typedef HalfEdgeIterator< ct, type > This;
      typedef VirtualIterator< std::random_access_iterator_tag, HalfEdge< ct, type > > Base;

    public:
      typedef typename Base::value_type value_type;
      typedef typename Base::pointer pointer;
      typedef typename Base::reference reference;

      HalfEdgeIterator () = default;

      reference operator* () const { return value_type( mesh(), index_ ); }
      pointer operator-> () const { return value_type( mesh(), index_ ); }

      bool operator== ( const This &other ) const noexcept { return (index_ == other.index_); }
      bool operator!= ( const This &other ) const noexcept { return (index_ != other.index_); }

      This &operator++ () { ++index_; return *this; }
      This operator++ ( int ) { This copy( *this ); ++(*this); return copy; }

      const Mesh &mesh () const { return *mesh_; }

    private:
      const Mesh *mesh_ = nullptr;
      HalfEdgeIndex< type > index_;
    };


    // HalfEdges
    // ---------

    template< class ct, MeshType type >
    class HalfEdges
    {
      typedef HalfEdges< ct, type > This;

    public:

    private:
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MESH_HH
