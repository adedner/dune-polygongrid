#ifndef DUNE_POLYGONGRID_GEOMETRY_HH
#define DUNE_POLYGONGRID_GEOMETRY_HH

#include <dune/common/exceptions.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>
#include <dune/common/math.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Internal Forward Declarations
    // -----------------------------

    template< int codim, class Grid >
    class Geometry;



    // BasicGeometry
    // -------------

    template< int codim, class Grid >
    class BasicGeometry
    {
      typedef Geometry< codim, Grid > This;

      typedef typename std::remove_const< Grid >::type::Traits Traits;

    public:
      typedef typename Traits::ctype ctype;

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

    template< class Grid >
    class Geometry< 0, Grid >
      : public BasicGeometry< 0, Grid >
    {
      typedef Geometry< 0, Grid > This;
      typedef BasicGeometry< 0, Grid > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      Geometry () = default;

      Geometry ( const Grid &grid, std::size_t index ) : grid_( &grid ), index_( index ) {}

      DUNE_INLINE int corners () const
      {
        // ...
      }

      GlobalCoordinate corner ( int i ) const
      {
        // ...
      }

      GlobalCoordinate center () const
      {
        // ...
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
      const Grid *grid_;
      std::size_t index_;
    };



    // Geometry for Codimension 1
    // --------------------------

    template< class Grid >
    class Geometry< 1, Grid >
      : public BasicGeometry< 1, Grid >
    {
      typedef Geometry< 1, Grid > This;
      typedef BasicGeometry< 1, Grid > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      Geometry () = default;

      Geometry ( const Grid &grid, std::size_t index ) : grid_( &grid ), index_( index ) {}

      DUNE_INLINE int corners () const { return 2; }

      GlobalCoordinate corner ( int i ) const
      {
        // ...
      }

      GlobalCoordinate center () const
      {
        GlobalCoordinate center( corner( 0 ) + corner( 1 ) );
        return center /= ctype( 2 );
      }

      ctype volume () const noexcept { return (corner( 1 ) - corner( 0 )).two_norm(); }

    private:
      const Grid *grid_;
      std::size_t index_;
    };



    // Geometry for Codimension 2
    // --------------------------

    template< class Grid >
    class Geometry< 2, Grid >
      : public BasicGeometry< 2, Grid >
    {
      typedef Geometry< 2, Grid > This;
      typedef BasicGeometry< 2, Grid > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      Geometry () = default;

      Geometry ( const Grid &grid, std::size_t index ) : grid_( &grid ), index_( index ) {}

      DUNE_INLINE int corners () const { return 1; }

      GlobalCoordinate corner ( int i ) const
      {
        // ...
      }

      GlobalCoordinate center () const { return corner( 0 ); }

      ctype volume () const noexcept { return Math::one; }

    private:
      const Grid *grid_;
      std::size_t index_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GEOMETRY_HH
