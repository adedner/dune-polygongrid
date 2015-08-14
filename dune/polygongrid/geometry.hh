#ifndef DUNE_POLYGONGRID_GEOMETRY_HH
#define DUNE_POLYGONGRID_GEOMETRY_HH

#include <dune/common/exceptions.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>
#include <dune/common/math.hh>

#include <dune/geometry/dimension.hh>

#include <dune/polygongrid/subentity.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Internal Forward Declarations
    // -----------------------------

    template< class, dim_t codim >
    class Geometry;



    // BasicGeometry
    // -------------

    template< class ct, dim_t codim >
    class BasicGeometry
    {
      typedef BasicGeometry< ct, codim > This;

    public:
      typedef ct ctype;

      static const dim_t mydimension = 2 - codim;
      static const dim_t coorddimension = 2;

      typedef FieldVector< ctype, mydimension > LocalCoordinate;
      typedef FieldVector< ctype, coorddimension > GlobalCoordinate;

      typedef FieldMatrix< ctype, mydimension, coorddimension > JacobianTransposed;
      typedef FieldMatrix< ctype, coorddimension, mydimension > JacobianInverseTransposed;

      DUNE_INLINE constexpr GeometryType type () const noexcept { return GeometryType( GeometryType::None(), mydimension ); }

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

    template< class Cell >
    class Geometry< Cell, 0 >
      : public BasicGeometry< typename Cell::ctype, 0 >
    {
      typedef Geometry< Cell, 0 > This;
      typedef BasicGeometry< typename Cell::ctype, 0 > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      Geometry () = default;

      explicit Geometry ( const Cell &cell ) : cell_( cell ) {}

      DUNE_INLINE int corners () const noexcept { return subEntities( cell_, Dune::Codim< 2 >() ); }

      DUNE_INLINE const GlobalCoordinate &corner ( int i ) const noexcept
      {
        return subEntity( cell_, Dune::Codim< 2 >, i ).position();
      }

      GlobalCoordinate center () const noexcept
      {
        GlobalCoordinate center( Math::zero );
        ctype volume = Math::zero;
        for( int i = 0; i < corneres(); ++i )
        {
          const GlobalCoordinate &x = corner( i );
          const GlobalCoordinate &y = corner( (i+1) % corners() );
          const ctype weight = x[ 0 ]*y[ 1 ] - x[ 1 ]*y[ 0 ];
          axpy( weight, x+y, center );
          volume += weight;
        }
        return center *= Math::one / (ctype( 3 )*volume);
      }

      ctype volume () const noexcept
      {
        ctype volume = Math::zero;
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

    template< class HalfEdge >
    class Geometry< HalfEdge, 1 >
      : public BasicGeometry< typename HalfEdge::ctype, 1 >
    {
      typedef Geometry< HalfEdge, 1 > This;
      typedef BasicGeometry< typename HalfEdge::ctype, 1 > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      Geometry () = default;

      Geometry ( const HalfEdge &halfEdge ) : halfEdge_( halfEdge ) {}

      DUNE_INLINE int corners () const { return subEntities( halfEdge_, Dune::Codim< 1 >() ); }

      const GlobalCoordinate &corner ( int i ) const
      {
        return subEntity( halfEdge_, Dune::Codim< 1 >, i ).position();
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

    template< class Node >
    class Geometry< Node, 2 >
      : public BasicGeometry< typename Node::ctype, 2 >
    {
      typedef Geometry< Node, 2 > This;
      typedef BasicGeometry< typename Node::ctype, 2 > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      Geometry () = default;

      explicit Geometry ( const Node &node ) : node_( node ) {}

      DUNE_INLINE int corners () const { return 1; }

      const GlobalCoordinate &corner ( int i ) const { return center(); }
      const GlobalCoordinate &center () const { return node_.position(); }

      ctype volume () const noexcept { return Math::one; }

    private:
      Node node_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GEOMETRY_HH
