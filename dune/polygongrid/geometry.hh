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

    // Geometry
    // --------

    template< int codim, class Grid >
    class Geometry
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

      Geometry () = default;

      Geometry ( const Grid &grid, std::size_t index ) : grid_( &grid ), index_( index ) {}

      DUNE_INLINE constexpr GeometryType type () const noexcept
      {
        return GeometryType( GeometryType::None(), mydimension );
      }

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
        // ...
      }

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

    private:
      const Grid *grid_;
      std::size_t index_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GEOMETRY_HH
