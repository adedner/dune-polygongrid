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
      halfEdges.sort_each();
      halfEdges.unique_each();

      return std::move( halfEdges );
    }



    // boundaries
    // ----------

    MultiVector< std::size_t > boundaries ( const MultiVector< std::size_t > &polygons, const MultiVector< std::size_t > &halfEdges )
    {
      std::vector< bool > isBoundary( halfEdges.values().size(), true );
      for( auto polygon : polygons )
      {
        for( std::size_t j = 0u; j < polygon.size(); ++j )
        {
          const std::size_t vtx = polygon[ j ];
          const std::size_t k = std::distance( halfEdges[ vtx ].begin(), std::find( halfEdges[ vtx ].begin(), halfEdges[ vtx ].end(), (j+1)%n ) );
          isBoundary[ halfEdges.position_of( vtx, k ) ] = false;
        }
      }

      MultiVector< std::size_t > boundaries;
      for( std::size_t vtx = 0u; vtx < halfEdges.size(); ++vtx )
      {
        std::vector< std::size_t > boundary;
        std::size_t v = vtx;
        for( std::size_t k = 0u; (k != halfEdges.size( v )) && !isBoundary[ halfEdges.position_of( v, k ) ]; ++k )
          continue;
        while( k != halfEdges[ v ].size() )
        {
          isBoundary[ halfEdges.position_of( v, k ) ] = false;
          boundary.push_back( v );

          v = halfEdges[ v ][ k ];
          for( std::size_t k = 0u; (k != halfEdges.size( v )) && !isBoundary[ halfEdges.position_of( v, k ) ]; ++k )
            continue;
        }
        if( !boundary.empty() )
          boundaries.push_back( boundary );
      }
      return std::move( boundaries );
    }


  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MESH_HH
