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
  template< class ct, class Comm = MPI_Comm >
  class PolygonGrid;
#else
  template< class ct, class Comm = No_Comm >
  class PolygonGrid;
#endif // #if !HAVE_MPI



  // PolygonGridFamily
  // -----------------

  template< class ct, class Comm >
  struct PolygonGridFamily
  {
    struct Traits
    {
      typedef PolygonGrid< ct, Comm > Grid;
    };
  };



  // PolygonGrid
  // -----------

  template< class ct, class Comm >
  class SPGrid
  : public GridDefaultImplementation< 2, 2, ct, PolygonGridFamily< ct, Comm > >
  {
    typedef PolygonGrid< ct, Comm > This;
    typedef GridDefaultImplementation< 2, 2, ct, PolygonGridFamily< ct, Comm > > Base;

  public:
    typedef PolygonGridFamily< ct, Comm > GridFamily;

    typedef typename GridFamily::Traits Traits;

    static const int dimension = 2;
    static const int dimensionworld = 2;
  };



  // Implementation of PolygonGrid
  // -----------------------------

  template< class ct, class Comm >
  const int SPGrid::dimension;

  template< class ct, class Comm >
  const int SPGrid::dimensionworld;

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRID_HH
