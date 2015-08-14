#ifndef DUNE_POLYGONGRID_MESHOBJECTS_HH
#define DUNE_POLYGONGRID_MESHOBJECTS_HH

#include <dune/common/iterator/tags.hh>

#include <dune/polygongrid/mesh.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Internal Forward Declarations
    // -----------------------------

    template< class ct, MeshType type >
    class HalfEdge;

    template< class ct, MeshType type >
    class HalfEdgeIterator;

    template< class ct, MeshType type >
    class HalfEdges;

    template< class ct, MeshType type >
    class Node;



    // Node
    // ----

    /**
     * \brief node (a.k.a. vertex)
     *
     * A node corresponds to the vertices of a mesh.
     * Consequently, it also corresponds to an element in the dual mesh.
     */
    template< class ct, MeshType type >
    class Node
    {
      typedef Node< ct, type > This;

    public:
      typedef __PolygonGrid::Mesh< ct > Mesh;
      typedef __PolygonGrid::HalfEdges< ct, dual( type ) > HalfEdges;

      typedef typename Mesh::GlobalCoordinate GlobalCoordinate;

      const GlobalCoordinate &position () const noexcept { return mesh().position( index_ ); }

      HalfEdges halfEdges () const noexcept;

      const Mesh &mesh () const noexcept { return *mesh_; }

    private:
      const Mesh *mesh_;
      NodeIndex< type > index_;
    };



    // HalfEdge
    // --------

    /**
     * \brief half edge (a.k.a. directed edge)
     *
     * A half edge corresponds to an edge in the mesh with an additinal
     * direction.
     * Thus, each edge is made up by two half edges, one for each direction.
     *
     * Moreover, an edge in the primal grid also corresponds to an edge in the
     * dual grid, connecting the two neighbors.
     * If elements are oriented, their boundaries are traversed in opposite
     * direction.
     * A half edge knows the element, whose boundary traverses it in the same
     * direction.
     */
    template< class ct, MeshType type = Primal >
    class HalfEdge
    {
      typedef HalfEdge< ct, type > This;

    public:
      typedef __PolygonGrid::Mesh< ct > Mesh;
      typedef __PolygonGrid::Node< ct, type > Vertex;
      typedef __PolygonGrid::Node< ct, dual( type ) > Polygon;

      HalfEdge ( const Mesh &mesh, HalfEdgeIndex< type > index ) : mesh_( &mesh ), index_( index ) {}

      bool boundary () const noexcept { return flip().polygon().boundary(); }

      This flip () const noexcept { return This( mesh(), mesh().flip( index ) );

      Vertex target () const noexcept { return Vertex( mesh(), mesh().target( index_ ) ); }

      Polygon polygon () const noexcept { return Polygon( mesh(), mesh().target( mesh().dual( index_ ) ) ); }

      const Mesh &mesh () const noexcept { return *mesh_; }

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
      HalfEdgeIterator ( const Mesh &mesh, HalfEdgeIndex< type > index ) : mesh_( &mesh ), index_( index ) {}

      reference operator* () const noexcept { return value_type( mesh(), index_ ); }
      pointer operator-> () const noexcept { return value_type( mesh(), index_ ); }

      reference operator[] ( std::ptrdiff_t n ) const noexcept { return value_type( mesh(), index_ + n ); }

      bool operator== ( const This &other ) const noexcept { return (index_ == other.index_); }
      bool operator!= ( const This &other ) const noexcept { return (index_ != other.index_); }

      bool operator< ( const This &other ) const noexcept { return (index_ < other.index_); }
      bool operator<= ( const This &other ) const noexcept { return (index_ <= other.index_); }
      bool operator> ( const This &other ) const noexcept { return (index_ > other.index_); }
      bool operator>= ( const This &other ) const noexcept { return (index_ >= other.index_); }

      This &operator++ () noexcept { ++index_; return *this; }
      This operator++ ( int ) noexcept { This copy( *this ); ++(*this); return copy; }

      This &operator-- () noexcept { --index_; return *this; }
      This operator-- ( int ) noexcept { This copy( *this ); --(*this); return copy; }

      This &operator+= ( std::ptrdiff_t n ) noexcept { index_ += n; return *this; }
      This &operator-= ( std::ptrdiff_t n ) noexcept { index_ -= n; return *this; }

      friend This operator+ ( const This &a, std::ptrdiff_t n ) noexcept { return This( mesh(), a.index_ + n ); }
      friend This operator+ ( std::ptrdiff_t n, const This &a ) noexcept { return This( mesh(), a.index_ + n ); }
      friend This operator- ( const This &a, std::ptrdiff_t n ) const noexcept { return This( mesh(), a.index_ - n ); }

      std::ptrdiff_t operator- ( const This &other ) const noexcept { return (index_ - other.index_); }

      const Mesh &mesh () const noexcept { assert( mesh_ ); return *mesh_; }

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
      typedef HalfEdgeIterator< ct. type > Iterator;

      HalfEdges ( const Mesh &mesh, NodeIndex< dual( type ) > index ) : mesh_( &mesh ), index_( index ) {}

      Iterator begin () const noexcept { return Iterator( mesh(), mesh().begin( index_ ) ); }
      Iterator end () const noexcept { return Iterator( mesh(), mesh().end( index_ ) ); }

      const Mesh &mesh () const noexcept { return *mesh_; }

    private:
      const Mesh *mesh_;
      NodeIndex< dual( type ) > index_;
    };



    // Implementation of Node
    // ----------------------

    template< class ct, MeshType type >
    inline typename Node< ct, type >::HalfEdges Node< ct, type >::halfEdges () const noexcept
    {
      return HalfEdges( mesh(), index_ );
    }

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MESH_HH
