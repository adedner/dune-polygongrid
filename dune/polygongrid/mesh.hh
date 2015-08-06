#ifndef DUNE_POLYGONGRID_MESH_HH
#define DUNE_POLYGONGRID_MESH_HH

#include <cstddef>

#include <utility>
#include <vector>

#include <dune/common/inline.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // MeshType
    // --------

    enum MeshType : std::size_t { Primal = 0u, Dual = 1u };

    inline static constexpr MeshType dual ( MeshType type ) { return static_Cast< MeshType >( type^1u ); }



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
      template< MeshType type >
      DUNE_INLINE std::size_t target ( HalfEdgeIndex< type > index ) const
      {
        return pair( index ).first;
      }

      template< MeshType type >
      DUNE_INLINE HalfEdgeIndex< __PolygonGrid::dual( type ) > dual ( HalfEdgeIndex< type > index ) const
      {
        return halfEdgeIndex< __PolygonGrid::dual( type ) >( pair( index ) );
      }

      template< MeshType type >
      DUNE_INLINE HalfEdgeIndex< type > flip ( HalfEdgeIndex< type > index ) const
      {
        return dual( dual( index ) );
      }

    private:
      template< MeshType type >
      DUNE_INLINE HalfEdgeIndex< type > halfEdgeIndex ( const Pair &pair ) const
      {
        return HalfEdgeIndex< type >( offset_[ type ][ pair.first ] + pair.second );
      }

      template< std::size_t i >
      DUNE_INLINE const Pair &pair ( HalfEdgeIndex< i > index ) const
      {
        assert( static_cast< std::size_t >( index ) < pair_[ i ].size() );
        return pair_[ i ][ static_cast< std::size_t >( index ) ];
      }

      std::array< std::vector< std::size_t >, 2 > offset_;
      std::array< std::vector< Pair >, 2 > pair_;
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

      DUNE_INLINE bool boundary () const { return flip().polygon().boundary(); }

      DUNE_INLINE This flip () const { return This( mesh(), mesh().flip( index ) );

      DUNE_ININE Vertex target () const { return Vertex( mesh(), mesh().target( index_ ) ); }

      DUNE_ININE Polygon polygon () const { return Polygon( mesh(), mesh().target( mesh().dual( index_ ) ) ); }

      DUNE_INLINE const Mesh &mesh () const { return *mesh_; }

    private:
      const Mesh *mesh_;
      HalfEdgeIndex< type > index_;
    };



    // HalfEdgeIterator
    // ----------------

    template< class Grid >
    class HalfEdgeIterator
      : public VirtualIterator< std::forward_iterator_tag, HalfEdge< Grid > >
    {
      typedef HalfEdgeIterator< Grid > This;
      typedef VirtualIterator< std::forward_iterator_tag, HalfEdge< Grid > > Base;

    public:
      typedef typename Base::pointer pointer;
      typedef typename Base::reference reference;

      reference operator* () const { return reference( halfEdge_ ); }
      pointer operator-> () const { return pointer( halfEdge_ ); }


    private:
      HalfEdge< Grid > halfEdge_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MESH_HH
