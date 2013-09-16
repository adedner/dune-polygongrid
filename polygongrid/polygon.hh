#ifndef POLYGONGRID_POLYGON_HH
#define POLYGONGRID_POLYGON_HH

#include <vector>

namespace PolygonGrid
{

  // Internal Forward Declarations
  // -----------------------------

  template< class Object >
  struct Vertex;

  template< class Object >
  struct HalfEdge;

  template< class Object >
  struct Polygon;



  // Vertex
  // ------

  template< class Object >
  struct Vertex
  : public Object
  {
    typedef typename Object::Vector Vector;

    const Vector &position () const { return position_; }

  private:
    Vector position_;
  };



  // HalfEdge
  // --------

  template< class Object >
  struct HalfEdge
  : public Object
  {
    typedef PolygonGrid::Vertex< Object > Vertex;
    typedef PolygonGrid::HalfEdge< Object > HalfEdge;

    typedef typename Object::template Pointer< Vertex >::Type VertexPtr;
    typedef typename Object::template Pointer< HalfEdge >::Type HalfEdgePtr;

  private:
    VertexPtr target;
    HalfEdgePtr next;
  };



  // Polygon
  // -------

  template< class Object >
  struct Polygon
  : public Object
  {
    typedef PolygonGrid::HalfEdge< Object > HalfEdge;

    typedef typename Object::Size Size;
    typedef typename Object::template Pointer< HalfEdge >::Type HalfEdgePtr;

    HalfEdgePtr first;
    Index numHalfEdges;
  };



  // Mesh
  // ----

  template< class Object >
  struct Mesh
  {
    typedef PolygonGrid::Vertex< Object > Vertex;
    typedef PolygonGrid::HalfEdge< Object > HalfEdge;
    typedef PolygonGrid::Polygon< Object > Polygon;

    std::vector< Vertex > vertices;
    std::vector< HalfEdge > halfEdges;
    std::vector< Polygon > polygons;
  };

} // namespace PolygonGrid

#endif // #ifndef POLYGONGRID_POLYGON_HH
