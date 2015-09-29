#ifndef DUNE_POLYGONGRID_GEOMETRY_HH
#define DUNE_POLYGONGRID_GEOMETRY_HH

#include <type_traits>

#include <dune/common/exceptions.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>
#include <dune/common/math.hh>

#include <dune/geometry/dimension.hh>
#include <dune/geometry/type.hh>

#include <dune/grid/common/geometry.hh>

#include <dune/polygongrid/subentity.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Internal Forward Declarations
    // -----------------------------

    template< int mydim, int cdim, class Grid >
    class Geometry;



    // BasicGeometry
    // -------------

    template< class ct, int codim >
    class BasicGeometry
    {
      typedef BasicGeometry< ct, codim > This;

    public:
      typedef ct ctype;

      static const int mydimension = 2 - codim;
      static const int coorddimension = 2;

      typedef FieldVector< ctype, mydimension > LocalCoordinate;
      typedef FieldVector< ctype, coorddimension > GlobalCoordinate;

      typedef FieldMatrix< ctype, mydimension, coorddimension > JacobianTransposed;
      typedef FieldMatrix< ctype, coorddimension, mydimension > JacobianInverseTransposed;

      GeometryType type () const noexcept { return GeometryType( GeometryType::none, mydimension ); }

      bool affine () const
      {
        DUNE_THROW( InvalidStateException, "Geometry::affine does not make for arbitrary polytopes." );
      }

      GlobalCoordinate global ( const LocalCoordinate &local ) const
      {
        DUNE_THROW( InvalidStateException, "Geometry::global does not make for arbitrary polytopes." );
      }

      LocalCoordinate local ( const GlobalCoordinate &global ) const
      {
        DUNE_THROW( InvalidStateException, "Geometry::local does not make for arbitrary polytopes." );
      }

      ctype integrationElement ( const LocalCoordinate &local ) const
      {
        DUNE_THROW( InvalidStateException, "Geometry::integrationElement does not make for arbitrary polytopes." );
      }

      JacobianTransposed jacobianTransposed ( const LocalCoordinate &local ) const
      {
        DUNE_THROW( InvalidStateException, "Geometry::jacobianTransposed does not make for arbitrary polytopes." );
      }

      JacobianInverseTransposed jacobianInverseTransposed ( const LocalCoordinate &local ) const
      {
        DUNE_THROW( InvalidStateException, "Geometry::jacobianInverseTransposed does not make for arbitrary polytopes." );
      }
    };



    // Geometry for Codimension 0
    // --------------------------

    template< int cdim, class Grid >
    class Geometry< 2, cdim, Grid >
      : public BasicGeometry< typename std::remove_const< Grid >::type::ctype, 0 >
    {
      typedef Geometry< 2, cdim, Grid > This;
      typedef BasicGeometry< typename std::remove_const< Grid >::type::ctype, 0 > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      typedef __PolygonGrid::Node< ctype > Cell;

      Geometry () = default;

      explicit Geometry ( const Cell &cell ) : cell_( cell ) {}

      int corners () const noexcept { return numSubEntities( cell_, Dune::Codim< 2 >() ); }

      const GlobalCoordinate &corner ( int i ) const noexcept
      {
        return subEntity( cell_, Dune::Codim< 2 >(), i ).position();
      }

      GlobalCoordinate center () const noexcept
      {
        GlobalCoordinate center( 0 );
        ctype volume = 0;
        for( int i = 0; i < corners(); ++i )
        {
          const GlobalCoordinate &x = corner( i );
          const GlobalCoordinate &y = corner( (i+1) % corners() );
          const ctype weight = x[ 0 ]*y[ 1 ] - x[ 1 ]*y[ 0 ];
          center.axpy( weight, x+y );
          volume += weight;
        }
        return center *= ctype( 1 ) / (ctype( 3 )*volume);
      }

      ctype volume () const noexcept
      {
        ctype volume = 0;
        for( int i = 0; i < corners(); ++i )
        {
          const GlobalCoordinate &x = corner( i );
          const GlobalCoordinate &y = corner( (i+1) % corners() );
          volume += x[ 0 ]*y[ 1 ] - x[ 1 ]*y[ 0 ];
        }
        return volume / ctype( 2 );
      }

    private:
      Cell cell_;
    };



    // Geometry for Codimension 1
    // --------------------------

    template< int cdim, class Grid >
    class Geometry< 1, cdim, Grid >
      : public BasicGeometry< typename std::remove_const< Grid >::type::ctype, 1 >
    {
      typedef Geometry< 1, cdim, Grid > This;
      typedef BasicGeometry< typename std::remove_const< Grid >::type::ctype, 1 > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      typedef __PolygonGrid::HalfEdge< ctype > HalfEdge;

      Geometry () = default;

      Geometry ( const HalfEdge &halfEdge ) : halfEdge_( halfEdge ) {}

      int corners () const { return numSubEntities( halfEdge_, Dune::Codim< 1 >() ); }

      const GlobalCoordinate &corner ( int i ) const
      {
        return subEntity( halfEdge_, Dune::Codim< 1 >(), i ).position();
      }

      GlobalCoordinate center () const
      {
        GlobalCoordinate center( corner( 0 ) + corner( 1 ) );
        return center /= ctype( 2 );
      }

      ctype volume () const noexcept { return (corner( 1 ) - corner( 0 )).two_norm(); }

    private:
      HalfEdge halfEdge_;
    };



    // Geometry for Codimension 2
    // --------------------------

    template< int cdim, class Grid >
    class Geometry< 0, cdim, Grid >
      : public BasicGeometry< typename std::remove_const< Grid >::type::ctype, 2 >
    {
      typedef Geometry< 0, cdim, Grid > This;
      typedef BasicGeometry< typename std::remove_const< Grid >::type::ctype, 2 > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      typedef __PolygonGrid::Node< ctype > Node;

      Geometry () = default;

      explicit Geometry ( const Node &node ) : node_( node ) {}

      int corners () const { return 1; }

      const GlobalCoordinate &corner ( int i ) const { return center(); }
      const GlobalCoordinate &center () const { return node_.position(); }

      ctype volume () const noexcept { return ctype( 1 ); }

    private:
      Node node_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GEOMETRY_HH
