#ifndef DUNE_POLYGONGRID_DGF_HH
#define DUNE_POLYGONGRID_DGF_HH

#include <cstddef>

#include <algorithm>
#include <fstream>
#include <iosfwd>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <dune/common/exceptions.hh>

#include <dune/geometry/dimension.hh>
#include <dune/geometry/type.hh>

#include <dune/grid/io/dgf/gridfactory.hh>

#include <dune/polygongrid/grid.hh>

namespace Dune
{

  // External Forward Declarations
  // -----------------------------

  template< class Grid >
  struct DGFGridInfo;



  // DGFGridInfo
  // -----------

  template< class ct >
  struct DGFGridInfo< PolygonGrid< ct > >
  {
    static constexpr int refineStepsForHalf () { return 0; }

    static constexpr double refineWeight () { return 1.0; }
  };



  // DGFGridFactory
  // --------------

  template< class ct >
  struct DGFGridFactory< PolygonGrid< ct > >
  {
    typedef PolygonGrid< ct > Grid;
    static const dim_t dimension = 2;

    typedef typename Grid::CollectiveCommunication CollectiveCommunication;
    typedef typename Grid::GlobalCoordinate GlobalCoordinate;

    explicit DGFGridFactory ( std::istream &input, const CollectiveCommunication &comm = CollectiveCommunication() )
      : parser_( comm.rank(), comm.size() )
    {
      generate( input );
    }

    explicit DGFGridFactory ( const std::string &filename, const CollectiveCommunication &comm = CollectiveCommunication() )
      : parser_( comm.rank(), comm.size() )
    {
      std::ifstream input( filename );
      generate( input );
    }

    Grid *grid () { return grid_.release(); }

    template< class Intersection >
    bool wasInserted ( const Intersection & ) const
    {
      return false;
    }

    template< class Intersection >
    int boundaryId ( const Intersection &intersection ) const
    {
#if 0
      const auto entity = intersection.inside();
      const int face = intersection.indexInInside();

      const unsigned int p0 = entity.impl().gridLevel().subIndex( entity.impl().index(), 0, (3-face)%3, 2 );
      const unsigned int p1 = entity.impl().gridLevel().subIndex( entity.impl().index(), 0, (4-face)%3, 2 );

      DuneGridFormatParser::facemap_t::key_type key( { p0, p1 }, false );
      const auto pos = parser_.facemap.find( key );
      if( pos != parser_.facemap.end() )
        return pos->second.first;
      else
#endif
        return (intersection.boundary() ? 1 : 0);
    }

    bool haveBoundaryParameters () const { return false; }

    template< class Intersection >
    const DGFBoundaryParameter::type &boundaryParameter ( const Intersection &intersection ) const
    {
#if 0
      const auto entity = intersection.inside();
      const int face = intersection.indexInInside();

      const unsigned int p0 = entity.impl().subIndex( entity.impl().index(), 0, (3-face)%3, 2 );
      const unsigned int p1 = entity.impl().subIndex( entity.impl().index(), 0, (4-face)%3, 2 );

      DuneGridFormatParser::facemap_t::key_type key( { p0, p1 }, false );
      const auto pos = parser_.facemap.find( key );
      if( pos != parser_.facemap.end() )
        return pos->second.second;
      else
#endif
        return DGFBoundaryParameter::defaultValue();
    }

    template< int codim >
    int numParameters () const
    {
      return (codim == 0 ? parser_.nofelparams : (codim == dimension ? parser_.nofvtxparams : 0));
    }

    std::vector< double > &parameter ( const typename Grid::template Codim< 0 >::Entity &element )
    {
      if( numParameters< 0 >() <= 0 )
        DUNE_THROW( InvalidStateException, "Calling DGFGridFactory::parameter is only allowed if there are parameters." );
      return parser_.elParams[ element.impl().index() ];
    }

    std::vector< double > &parameter ( const typename Grid::template Codim< dimension >::Entity &vertex )
    {
      if( numParameters< dimension >() <= 0 )
        DUNE_THROW( InvalidStateException, "Calling DGFGridFactory::parameter is only allowed if there are parameters." );
      return parser_.vtxParams[ vertex.impl().index() ];
    }

  private:
    void generate ( std::ifstream &input );

    std::unique_ptr< Grid > grid_;
    DuneGridFormatParser parser_;
  };



  // DGFGridFactory::generate
  // ------------------------

  template< class ct >
  inline void DGFGridFactory< PolygonGrid< ct > >::generate ( std::ifstream &input )
  {
    parser_.element = dgf::General;

    if( !parser_.readDuneGrid( input, dimension, GlobalCoordinate::dimension ) )
      DUNE_THROW( DGFException, "Unable to read DGF stream." );

    std::vector< GlobalCoordinate > vertices( parser_.nofvtx );
    for( int i = 0; i < parser_.nofvtx; ++i )
      std::copy( parser_.vtx[ i ].begin(), parser_.vtx[ i ].end(), vertices[ i ].begin() );

    std::vector< std::size_t > counts( parser_.nofelements );
    for( int i = 0; i < parser_.nofelements; ++i )
      counts[ i ] = parser_.elements[ i ].size();

    __PolygonGrid::MultiVector< std::size_t > polygons( counts );
    for( int i = 0; i < parser_.nofelements; ++i )
    {
      std::copy( parser_.elements[ i ].begin(), parser_.elements[ i ].end(), polygons[ i ].begin() );
      if( counts[ i ] == 4u )
        std::swap( polygons[ i ][ 2u ], polygons[ i ][ 3u ] );
    }

    grid_.reset( new Grid( std::make_shared< __PolygonGrid::Mesh >( vertices, polygons ), __PolygonGrid::Primal ) );
  }

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_DGF_HH
