#include <config.h>

#include <dune/common/parallel/mpihelper.hh>

#include <dune/polygongrid/grid.hh>

#include <dune/grid/test/gridcheck.hh>


// main
// ----

int main ( int argc, char **argv )
try
{
  Dune::MPIHelper::instance( argc, argv );

  return 0;
}
catch( const Dune::Exception &e )
{
  std::cerr << e << std::endl;
  return 1;
}
