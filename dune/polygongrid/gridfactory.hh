#ifndef DUNE_POLYGONGRID_GRIDFACTORY_HH
#define DUNE_POLYGONGRID_GRIDFACTORY_HH

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <dune/common/exceptions.hh>

#include <dune/geometry/type.hh>

#include <dune/grid/common/gridfactory.hh>

#include <dune/polygongrid/grid.hh>
#include <dune/polygongrid/mesh.hh>

namespace Dune
{

  // GridFactory for PolygonGrid
  // ---------------------------

  template< class ct >
  class GridFactory< PolygonGrid< ct > >
  {
    typedef GridFactory< PolygonGrid< ct > > This;

  public:
    static const int dimension = 2;

    typedef PolygonGrid< ct > Grid;
    typedef FieldVector< ct, 2 > GlobalCoordinate;

    GridFactory () {}

    void insertVertex ( const GlobalCoordinate &vertex ) { vertices_.push_back( vertex ); }

    void insertElement ( const GeometryType &type, const std::vector< unsigned int > &nodes )
    {
      if( type.dim() != 2 )
        DUNE_THROW( GridError, "PolygonGrid can only handle two-dimensional elements." );

      const std::size_t numNodes = nodes.size();

      std::vector< std::size_t > polygon( numNodes );
      for( std::size_t i = 0; i < numNodes; ++i )
      {
        if( nodes[ i ] >= vertices_.size() )
          DUNE_THROW( GridError, "No such vertex: " << nodes[ i ] << "." );
        polygon[ i ] = nodes[ i ];
      }

      if( type.isSimplex() && (numNodes != 3u) )
        DUNE_THROW( GridError, "Wrong number of triangle vertices: " << numNodes << "." );
      if( type.isCube() )
      {
        if( numNodes != 4u )
          DUNE_THROW( GridError, "Wrong number of quadrilateral vertices: " << numNodes << "." );
        std::swap( polygon[ 2 ], polygon[ 3 ] );
      }

      // insert polygon oriented counter-clockwise
      const GlobalCoordinate a = vertices_[ polygon[ 1 ] ] - vertices_[ polygon[ 0 ] ];
      const GlobalCoordinate b = vertices_[ polygon[ 2 ] ] - vertices_[ polygon[ 0 ] ];
      if( a[ 0 ]*b[ 1 ] - a[ 1 ]*b[ 0 ] < ct( 0 ) )
        std::reverse( polygon.begin(), polygon.end() );

      // todo: improve to take convex hull of inserted vertices

      polygons_.push_back( polygon );
    }

    void insertBoundarySegment ( const std::vector< unsigned int > & )
    {
      DUNE_THROW( NotImplemented, "Method insertBoundarySegment() not implemented yet" );
    }

    void insertBoundarySegment ( const std::vector< unsigned int > &, std::shared_ptr< BoundarySegment< dimension, 2 > > )
    {
      DUNE_THROW( NotImplemented, "Method insertBoundarySegment() not implemented yet" );
    }

    Grid *createGrid ()
    {
      return new Grid( std::make_shared< typename Grid::Mesh >( vertices_, polygons_ ), __PolygonGrid::Primal );
    }

  private:
    std::vector< GlobalCoordinate > vertices_;
    __PolygonGrid::MultiVector< std::size_t > polygons_;
  };

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRIDFACTORY_HH
