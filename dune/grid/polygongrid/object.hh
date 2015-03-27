#ifndef DUNE_POLYGONGRID_OBJECT_HH
#define DUNE_POLYGONGRID_OBJECT_HH

namespace Dune
{

  namespace __PolygonGrid
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

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_OBJECT_HH
