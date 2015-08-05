#ifndef DUNE_POLYGONGRID_HALFEDGE_HH
#define DUNE_POLYGONGRID_HALFEDGE_HH

namespace Dune
{

  namespace __PolygonGrid
  {

    // HalfEdge
    // --------

    template< class Grid >
    class HalfEdge
    {
      typedef HalfEdge< Grid > This;

    public:
      HalfEdge ( const Grid &grid, std::size_t index ) : grid_( &grid ), index_( index ) {}

      bool boundary () const
      {
        // ...
      }

      This flip () const
      {
        // ...
      }

      Vertex target () const
      {
        // ...
      }

      Polygon polygon () const
      {
        // ...
      }

      const Grid &grid () const { return *grid_; }

    private:
      const Grid *grid_;
      std::size_t index_;
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

#endif // #ifndef DUNE_POLYGONGRID_HALFEDGE_HH
