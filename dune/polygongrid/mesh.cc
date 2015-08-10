#include <config.h>

#include <utility>

#include <dune/polygongrid/mesh.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // halfEdges
    // ---------

    MultiVector< std::size_t > halfEdges ( std::size_t numVertices, const MultiVector< std::size_t > &polygons )
    {
      const std::size_t numPolygons = polygons.size();

      std::vector< std::size_t > count( numVertices, 0u );
      for( std::size_t vtx : polygons )
        count[ vtx ] += 2u;

      // compute all neighboring vertices
      MultiVector< std::size_t > halfEdges( count, std::numeric_limits< std::size_t >::max() );
      std::fill( count.begin(), count.end(), 0u );
      for( std::size_t i = 0u; i < numPolygons; ++i )
      {
        const std::size_t n = offset[ i+1 ] - offset[ i ];
        for( std::size_t j = 0u; j < n; ++j )
        {
          const std::size_t vtx = polygons[ offset[ i ] + j ];
          halfEdges[ Pair( vtx, count[ vtx ]++ ) ] = polygons[ Pair( i, (j+1)%n ) ];
          halfEdges[ Pair( vtx, count[ vtx ]++ ) ] = polygons[ Pair( i, (j+n-1)%n ) ];
        }
      }

      // make neighboring vertices unique
      halfEdges.sortEach();
      halfEdges.uniqueEach();

      return std::move( halfEdges );
    }

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MESH_HH
