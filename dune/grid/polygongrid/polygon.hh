#ifndef DUNE_POLYGONGRID_POLYGON_HH
#define DUNE_POLYGONGRID_POLYGON_HH

namespace Dune
{

  namespace PolygonGridImpl
  {

    // Object
    // ------

    template< class Comm >
    struct Object;

#if HAVE_MPI
    struct Object< MPI_Comm >
    {
      PartitionType partition () { return partition_; }

    private:
      PartitionType partition_;
    };
#endif // #if HAVE_MPI

    struct Object< No_Comm >
    {
      PartitionType partition () { return InteriorEntity; }
    };



    // Vertex
    // ------

    template< class ct, class Comm >
    struct Vertex
    : public Object< Comm >
    {
      const FieldVector< ct, 2 > &position () const { return position_; }

    private:
      FieldVector< ct, 2 > position_;
    };


    // HalfEdge
    // --------

    template< class Index, class Comm >
    struct HalfEdge
    : public Object< Comm >
    {
      Index targetVertex;
      Index nextHalfEdge;
    };



    // Polygon
    // -------

    template< class Index, class Comm >
    struct Polygon
    : public Object< Comm >
    {
      Index firstHalfEdge;
      Index numHalfEdges;
    };

  } // namespace PolygonGridImpl

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_POLYGON_HH
