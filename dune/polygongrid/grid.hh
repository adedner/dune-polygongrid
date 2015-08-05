#ifndef DUNE_POLYGONGRID_GRID_HH
#define DUNE_POLYGONGRID_GRID_HH

#include <dune/common/parallel/mpicollectivecommunication.hh>

#include <dune/grid/common/grid.hh>

#include <dune/grid/polygongrid/gridfamily.hh>

namespace Dune
{

  // Internal Forward Declarations
  // -----------------------------

#if HAVE_MPI
  template< class ct, class idx = unsigned int, class Comm = MPI_Comm >
  class PolygonGrid;
#else
  template< class ct, class idx = unsigned int, class Comm = No_Comm >
  class PolygonGrid;
#endif // #if !HAVE_MPI



  // PolygonGrid
  // -----------

  template< class ct, class idx, class Comm >
  class PolygonGrid
    : public GridDefaultImplementation< 2, 2, ct, __PolygonGrid::GridFamily< ct, idx, Comm > >
  {
    typedef PolygonGrid< ct, idx, Comm > This;
    typedef GridDefaultImplementation< 2, 2, ct, __PolygonGrid::GridFamily< ct, idx, Comm > > Base;

  public:
    typedef __PolygonGrid::GridFamily< ct, idx, Comm > GridFamily;

    typedef typename GridFamily::Traits Traits;

    static const int dimension = 2;
    static const int dimensionworld = 2;
  };



  // Implementation of PolygonGrid
  // -----------------------------

  template< class ct, class idx, class Comm >
  const int PolygonGrid< ct, idx, Comm >::dimension;

  template< class ct, class idx, class Comm >
  const int PolygonGrid< ct, idx, Comm >::dimensionworld;

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRID_HH