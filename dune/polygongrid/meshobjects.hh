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
      typedef ct ctype;

      typedef __PolygonGrid::Mesh< ctype > Mesh;
      typedef __PolygonGrid::HalfEdges< ctype, dual( type ) > HalfEdges;

      typedef typename Mesh::GlobalCoordinate GlobalCoordinate;

      typedef NodeIndex< type > Index;

      Node ( const Mesh &mesh, Index index ) : mesh_( &mesh ), index_( index ) {}

      const GlobalCoordinate &position () const noexcept { return mesh().position( index_ ); }

      HalfEdges halfEdges () const noexcept;

      const Mesh &mesh () const noexcept { return *mesh_; }

    private:
      const Mesh *mesh_;
      Index index_;
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
    template< class ct, MeshType type >
    class HalfEdge
    {
      typedef HalfEdge< ct, type > This;

    public:
      typedef ct ctype;

      typedef __PolygonGrid::Mesh< ctype > Mesh;
      typedef __PolygonGrid::Node< ctype, type > Node;
      typedef __PolygonGrid::Node< ctypet, dual( type ) > Cell;

      typedef HalfEdgeIndex< type > Index;

      HalfEdge ( const Mesh &mesh, Index index ) : mesh_( &mesh ), index_( index ) {}

      /** \brief flip the direction of the half edge */
      This flip () const noexcept { return This( mesh(), mesh().flip( index ) ); }

      /** \brief obtain node, the half edge points to */
      Node target () const noexcept { return Node( mesh(), mesh().target( index_ ) ); }

      /** \brief obtain neighboring cell (whose boundary contains the flipped half edge) */
      Cell neighbor () const noexcept { return Cell( mesh(), mesh().target( mesh().dual( index_ ) ) ); }

      /** \brief obtain cell whose boundary contains this half edge */
      Cell cell () const noexcept { return flip().neighbor(); }

      const Mesh &mesh () const noexcept { return *mesh_; }

    private:
      const Mesh *mesh_;
      Index index_;
    };



    // IndexIterator
    // -------------

    template< class V >
    class IndexIterator
      : public VirtualIterator< std::random_access_iterator_tag, V >
    {
      typedef IndexIterator< V > This;
      typedef VirtualIterator< std::random_access_iterator_tag, V > Base;

    public:
      typedef typename Base::value_type value_type;
      typedef typename Base::pointer pointer;
      typedef typename Base::reference reference;

      typedef typename V::Mesh Mesh;
      typedef typename V::Index Index;

      IndexIterator () = default;
      IndexIterator ( const Mesh &mesh, Index index ) : mesh_( &mesh ), index_( index ) {}

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

      friend This operator+ ( const This &a, std::ptrdiff_t n ) noexcept { return This( a.mesh(), a.index_ + n ); }
      friend This operator+ ( std::ptrdiff_t n, const This &a ) noexcept { return This( a.mesh(), a.index_ + n ); }
      friend This operator- ( const This &a, std::ptrdiff_t n ) noexcept { return This( a.mesh(), a.index_ - n ); }

      std::ptrdiff_t operator- ( const This &other ) const noexcept { return (index_ - other.index_); }

      const Mesh &mesh () const noexcept { assert( mesh_ ); return *mesh_; }

    private:
      const Mesh *mesh_ = nullptr;
      Index index_;
    };



    // Nodes
    // -----

    template< class ct, MeshType type >
    class Nodes
    {
      typedef Nodes< ct, type > This;

    public:
      typedef __PolygonGrid::Mesh< ct > Mesh;
      typedef IndexIterator< Node< ct, type > > Iterator;

      explicit Nodes ( const Mesh &mesh ) noexcept : mesh_( &mesh ) {}

      Iterator begin () const noexcept { return Iterator( mesh(), mesh().template begin< type >() ); }
      Iterator end () const noexcept { return Iterator( mesh(), mesh().template end< type >() ); }

      const Mesh &mesh () const noexcept { return *mesh_; }

    private:
      const Mesh *mesh_;
    };

    template< MeshType type, class ct >
    inline static Nodes< ct, type >
    nodes ( const Mesh< ct > &mesh, std::integral_constant< MeshType, type > = std::integral_constant< MeshType, type >() ) noexcept
    {
      return Nodes< ct, type >( mesh );
    }



    // HalfEdges
    // ---------

    template< class ct, MeshType type >
    class HalfEdges
    {
      typedef HalfEdges< ct, type > This;

    public:
      typedef __PolygonGrid::Mesh< ct > Mesh;
      typedef IndexIterator< HalfEdge< ct, type > > Iterator;

      HalfEdges ( const Mesh &mesh, NodeIndex< dual( type ) > index ) : mesh_( &mesh ), index_( index ) {}

      Iterator begin () const noexcept { return Iterator( mesh(), mesh().begin( index_ ) ); }
      Iterator end () const noexcept { return Iterator( mesh(), mesh().end( index_ ) ); }

      std::size_t size () const { return mesh().size( index_ ); }

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
