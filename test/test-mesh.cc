#include <config.h>

#include <dune/common/parallel/mpihelper.hh>

#include <dune/polygongrid/mesh.hh>
#include <dune/polygongrid/multivector.hh>

using Dune::__PolygonGrid::Primal;
using Dune::__PolygonGrid::Dual;

using Dune::__PolygonGrid::MultiVector;
using Dune::__PolygonGrid::Mesh;
using Dune::__PolygonGrid::MeshStructure;

using Dune::__PolygonGrid::boundaries;
using Dune::__PolygonGrid::checkStructure;
using Dune::__PolygonGrid::meshStructure;


// main
// ----

int main ( int argc, char **argv )
try
{
  Dune::MPIHelper::instance( argc, argv );

  const std::size_t numVertices = 13;
  MultiVector< std::size_t > polys
    = { { 0, 1, 4, 3 }, { 1, 2, 6, 5, 4 }, { 3, 4, 5, 8, 11, 7 }, { 5, 6, 9, 8 }, { 7, 11, 10 }, { 8, 9, 12, 11 } };

  MultiVector< std::size_t > bnds = boundaries( numVertices, polys );
  MeshStructure mesh = meshStructure( numVertices, polys, bnds );

  std::cout << std::endl << std::endl;
  std::cout << "Primal Structure:" << std::endl;
  printStructure( mesh[ Primal ] );
  std::cout << std::endl;

  std::cout << std::endl << std::endl;
  std::cout << "Dual Structure:" << std::endl;
  printStructure( mesh[ Dual ] );
  std::cout << std::endl;

  if( !checkStructure( mesh ) )
    std::cerr << "Error: Mesh structure not valid." << std::endl;

  return 0;
}
catch( const Dune::Exception &e )
{
  std::cerr << e << std::endl;
  return 1;
}
