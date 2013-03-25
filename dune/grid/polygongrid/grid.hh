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
  template< class ct, int dim, class Comm = MPI_Comm >
  class PolygonGrid;
#else
  template< class ct, int dim, class Comm = No_Comm >
  class PolygonGrid;
#endif // #if !HAVE_MPI



  // PolygonGridFamily
  // -----------------

  template< class ct, int dim, class Comm >
  struct PolygonGridFamily
  {
    struct Traits
    {
      typedef PolygonGrid< ct, dim, Comm > Grid;
    };
  };



  // PolygonGrid
  // -----------

  template< class ct, int dim, class Comm >
  class SPGrid
  : public GridDefaultImplementation< dim, ct, PolygonGridFamily< ct, dim, Comm > >
  {
    typedef PolygonGrid< ct, dim, Comm > This;
    typedef GridDefaultImplementation< dim, ct, PolygonGridFamily< ct, dim, Comm > > Base;

  public:
    typedef PolygonGridFamily< ct, dim, Comm > GridFamily;

    typedef typename GridFamily::Traits Traits;

    static const int dimension = dim;
    static const int dimensionworld = dim;
  };



  // Implementation of PolygonGrid
  // -----------------------------

  template< class ct, int dim, class Comm >
  const int SPGrid::dimension;

  template< class ct, int dim, class Comm >
  const int SPGrid::dimensionworld;

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRID_HH
