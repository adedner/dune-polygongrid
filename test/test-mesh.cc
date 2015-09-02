#include <config.h>

#include <dune/common/parallel/mpihelper.hh>

#include <dune/polygongrid/mesh.hh>
#include <dune/polygongrid/meshobjects.hh>
#include <dune/polygongrid/multivector.hh>

using Dune::__PolygonGrid::Primal;
using Dune::__PolygonGrid::Dual;
using Dune::__PolygonGrid::primalMesh;
using Dune::__PolygonGrid::dualMesh;

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

  {
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
  }

  std::vector< Dune::FieldVector< double, 2 > > vertices
    = { { 0.0, 0.0 }, { 0.5, 0.0 }, { 1.0, 0.0 },
        { 0.0, 0.4 }, { 0.5, 0.2 }, { 0.7, 0.4 }, { 1.0, 0.4 },
        { 0.0, 0.7 }, { 0.7, 0.6 }, { 1.0, 0.6 },
        { 0.0, 1.0 }, { 0.3, 1.0 }, { 1.0, 1.0 } };

  Mesh< double > mesh( vertices, polys );

  std::cout << std::endl << std::endl;
  std::cout << "Primal Vertices:" << std::endl << std::endl;
  for( auto node : nodes( mesh, primalMesh ) )
    std::cout << node.position() << std::endl;
  std::cout << std::endl;

  std::cout << std::endl << std::endl;
  std::cout << "Dual Vertices:" << std::endl << std::endl;
  for( auto node : nodes( mesh, dualMesh ) )
    std::cout << node.position() << std::endl;
  std::cout << std::endl;

  for( auto node : nodes( mesh, dualMesh ) )
  {
    for( auto halfEdge : node.halfEdges() )
    {
      if( halfEdge.cell().index() != node.index() )
      {
        std::cerr << "Error: halfEdge.flip().target() does not return original node." << std::endl;
        std::cerr << "       (halfEdge.index() = " << static_cast< std::size_t >( halfEdge.index() )
                  << ", halfEdge.cell().index() = " << static_cast< std::size_t >( halfEdge.cell().index() )
                  << ", node.index() = " << static_cast< std::size_t >( node.index() ) << ")" << std::endl;
        std::abort();
      }
    }
  }

  return 0;
}
catch( const Dune::Exception &e )
{
  std::cerr << e << std::endl;
  return 1;
}
