#include <config.h>

#include <algorithm>
#include <utility>

#include <dune/polygongrid/mesh.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // boundaries
    // ----------

    MultiVector< std::size_t > boundaries ( std::size_t numVertices, const MultiVector< std::size_t > &polygons )
    {
      const std::size_t numPolygons = polygons.size();

      // count number of polygons adjacent to a vertex
      std::vector< std::size_t > count( numVertices, 0u );
      for( std::size_t vtx : polygons.values() )
        ++count[ vtx ];

      // store succeeding vertices
      MultiVector< std::size_t > halfEdges( count, std::numeric_limits< std::size_t >::max() );
      std::fill( count.begin(), count.end(), 0u );
      for( auto polygon : polygons )
      {
        const std::size_t n = polygon.size();
        for( std::size_t j = 0u; j < n; ++j )
        {
          const std::size_t vtx = polygon[ j ];
          halfEdges[ vtx ][ count[ vtx ]++ ] = polygon[ (j+1)%n ];
        }
      }

      // if a preceeding vertex is not in the list, we have a boundary edge
      MultiVector< std::size_t > boundaries;
      std::vector< std::size_t > edge( 2 );
      for( auto polygon : polygons )
      {
        const std::size_t n = polygon.size();
        for( std::size_t j = 0u; j < n; ++j )
        {
          edge[ 0 ] = polygon[ j ];
          edge[ 1 ] = polygon[ (j+n-1)%n ];
          if( std::find( halfEdges[ edge[ 0 ] ].begin(), halfEdges[ edge[ 0 ] ].end(), edge[ 1 ] ) == halfEdges[ edge[ 0 ] ].end() )
            boundaries.push_back( edge );
        }
      }
      return std::move( boundaries );
    }

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MESH_HH
