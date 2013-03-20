#ifndef DUNE_POLYGONGRID_GRID_HH
#define DUNE_POLYGONGRID_GRID_HH

#include <dune/common/parallel/mpicollectivecommunication.hh>

#include <dune/grid/common/grid.hh>

#include <dune/grid/polygongrid/declaration.hh>

namespace Dune
{

  // Internal Forward Declarations
  // -----------------------------

#if HAVE_MPI
  template< class ct, int dim, int dimworld = dim, class Comm = MPI_Comm >
  class PolygonGrid;
#else
  template< class ct, int dim, int dimworld = dim, class Comm = No_Comm >
  class PolygonGrid;
#endif // #if !HAVE_MPI



  // PolygonGridFamily
  // -----------------

  template< class ct, int dim, int dimworld, class Comm >
  struct PolygonGridFamily
  {
    struct Traits
    {
      typedef PolygonGrid< ct, dim, dimworld, Comm > Grid;
    };
  };



  // PolygonGrid
  // -----------

  template< class ct, int dim, int dimworld, class Comm >
  class SPGrid
  : public GridDefaultImplementation< dim, dimworld, ct, PolygonGridFamily< ct, dim, dimworld, Comm > >
  {
    typedef PolygonGrid< ct, dim, dimworld, Comm > This;
    typedef GridDefaultImplementation< dim, dimworld, ct, PolygonGridFamily< ct, dim, dimworld, Comm > > Base;

  public:
    typedef PolygonGridFamily< ct, dim, dimworld, Comm > GridFamily;

    typedef typename GridFamily::Traits Traits;

    static const int dimension = dim;
    static const int dimensionworld = dimworld;
  };

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRID_HH
