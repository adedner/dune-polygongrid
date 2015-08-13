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



    // meshStructure
    // -------------

    MeshStructure meshStructure ( std::size_t numVertices, const MultiVector< std::size_t > &polygons, const MultiVector< std::size_t > &boundaries )
    {
      const std::size_t numPolygons = polygons.size();
      const std::size_t numBoundaries = boundaries.size();

      MeshStructure structure;
      std::vector< std::size_t > count;

      // number of halfedges for primal grid:
      // - p.size() for each polygon p
      // - 3 for each boundary edge
      // - 2 for each boundary vertex
      count = polygons.sizes();
      count.insert( count.end(), numBoundaries, 3u );
      count.insert( count.end(), numBoundaries, 2u );

      // initialize primal structure (without target position)
      structure[ Primal ].resize( count );
      auto makeIndexPair = [] ( std::size_t i ) { return IndexPair( i, std::numeric_limits< std::size_t >::max() ); };
      for( std::size_t i = 0; i < numPolygons; ++i )
        std::transform( polygons[ i ].begin(), polygons[ i ].end(), structure[ Primal ][ i ].begin(), makeIndexPair );
      for( std::size_t i = 0; i < numBoundaries; ++i )
      {
        // create boundary edge cell
        auto item = structure[ Primal ][ numPolygons + i ];
        item[ 0 ] = IndexPair( boundaries[ i ][ 0 ], 1 );
        item[ 1 ] = makeIndexPair( boundaries[ i ][ 1 ] );
        item[ 2 ] = IndexPair( numVertices + 2*i+1, 0 );
      }
      for( std::size_t i = 0; i < numBoundaries; ++i )
      {
        // create boundary vertex cell
        auto item = structure[ Primal ][ numPolygons + numBoundaries + i ];
        item[ 0 ] = IndexPair( boundaries[ i ][ 0 ], 2 );
        item[ 1 ] = IndexPair( numVertices + 2*i, 0 );
      }

      // number of halfedges for dual grid:
      // - for each regular vertex:
      //   + 1 for each adjacent polygon
      //   + 1 for each adjacent boundary edge (for bisected boundary edge)
      //   + 1 for being a boundary vertex (for vertex element)
      // - 1 for each bisected boundary edge center
      count = std::vector< std::size_t >( numVertices, 0u );
      for( std::size_t vtx : polygons.values() )
        ++count[ vtx ];
      for( auto boundary : boundaries )
      {
        ++count[ boundary[ 0 ] ];
        for( std::size_t vtx : boundary )
          ++count[ vtx ];
      }
      count.insert( count.end(), 2*numBoundaries, 1u );

      // a regular vertex points to:
      // - the succeeding position in a polygon
      // - the second position in a boundary edge cell
      // - the second position in a boundary vertex cell
      structure[ Dual ].resize( count );
      std::fill( count.begin(), count.end(), 0u );
      for( std::size_t i = 0; i < numBoundaries; ++i )
      {
        // boundary vertices automatically get 3 connections
        const std::size_t v0 = boundaries[ i ][ 0 ];
        count[ v0 ] = 3;
        structure[ Dual ][ v0 ][ 0 ] = IndexPair( numPolygons + i, 1 );
        structure[ Dual ][ v0 ][ 1 ] = IndexPair( numPolygons + numBoundaries + i, 1 );
        const std::size_t v1 = boundaries[ i ][ 1 ];
        structure[ Dual ][ v1 ][ 2 ] = IndexPair( numPolygons + i, 2 );

        structure[ Dual ][ numVertices + 2*i ][ 0 ] = IndexPair( numPolygons + i, 0 );
        structure[ Dual ][ numVertices + 2*i+1 ][ 0 ] = IndexPair( numPolygons + numBoundaries + (i+1)%numBoundaries, 0 );
      }
      for( std::size_t i = 0; i < numPolygons; ++i )
      {
        const std::size_t n = polygons[ i ].size();
        for( std::size_t j = 0u; j < n; ++j )
        {
          const std::size_t vtx = polygons[ i ][ j ];
          structure[ Dual ][ vtx ][ count[ vtx ]++ ] = IndexPair( i, (j+1)%n );
        }
      }

      // sort regular dual cells (those corresponding to regular vertices)
      for( std::size_t i = 0; i < numVertices; ++i )
      {
        auto cell2 = structure[ Dual ][ i ];
        const std::size_t n2 = cell2.size();
        std::size_t k2 = (cell2[ 0 ].first >= numPolygons ? 4u : 1u);
        assert( k2 < n2 );
        while( true )
        {
          // look at preceeding half edge
          auto cell1 = structure[ Primal ][ cell2[ k2-1 ].first ];
          const std::size_t n1 = cell1.size();
          const std::size_t k1 = cell2[ k2-1 ].second;

          // the preceeding vertex points to us
          assert( cell1[ (k1+n1-1)%n1 ].first == i );
          cell1[ (k1+n1-1)%n1 ].second = k2 % n2;

          // now find the next half edge
          std::size_t nbvtx = cell1[ (k1+n1-2)%n1 ].first;
          auto pos = std::find_if( cell2.begin()+k2, cell2.end(), [ &structure, nbvtx ] ( IndexPair p ) { return (structure[ Primal ][ p ].first == nbvtx); } );
          assert( (k2 == n2) || (pos != cell2.end()) );
          if( pos == cell2.end() )
            break;
          std::swap( cell2[ k2 ], *pos );
        }
      }

      return std::move( structure );
    }

  } // namespace __PolygonGrid

} // namespace Dune
