#ifndef DUNE_POLYTOPEGEOMETRY_HH
#define DUNE_POLYTOPEGEOMETRY_HH

#include <dune/common/exceptions.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>

#include <dune/geometry/type.hh>

namespace Dune
{

  // PolytopeGeometry
  // ----------------

  template< class Polytope >
  class PolytopeGeometry
  {
    typedef PolytopeGeometry< Polytope > This;

  public:
    typedef typename Polytope::ctype ctype;

    static const int mydimension = Polytope::dimension;
    static const int coorddimension = Polytope::dimensionworld;

    typedef FieldVector< ctype, mydimension > LocalCoordinate;
    typedef FieldVector< ctype, coorddimension > GlobalCoordinate;

    typedef FieldMatrix< ctype, mydimension, coorddimension > JacobianTransposed;
    typedef FieldMatrix< ctype, coorddimension, mydimension > JacobianInverseTransposed;

    explicit PolytopeGeometry ( const Polytope &polytope )
    : polytope_( polytope )
    {}

    // interface for global polytope

    int corners () const { return polytope_.corners(); }

    GlobalCoordinate corner ( int i ) const { return polytope_.corner( i ); }

    GlobalCoordinate center () const { return polytope_.center(); }

    ctype volume () const { return polytope_.volume(); }

    // interface for reference mapping

    GeometryType type () const { return GeometryType( GeometryType::None(), dimension ); }

    bool affine () const
    {
      DUNE_THROW( InvalidStateException, "Geometry::affine requires a reference element." );
    }

    GlobalCoordinate global ( const LocalCoordinate &local ) const
    {
      DUNE_THROW( InvalidStateException, "Geometry::global requires a reference element." );
    }

    LocalCoordinate local ( const GlobalCoordinate &global ) const
    {
      DUNE_THROW( InvalidStateException, "Geometry::local requires a reference element." );
    }

    ctype integrationElement ( const LocalCoordinate &local ) const
    {
      DUNE_THROW( InvalidStateException, "Geometry::integrationElement requires a reference element." );
    }

    const JacobianTransposed &jacobianTransposed ( const LocalCoordinate &local ) const
    {
      DUNE_THROW( InvalidStateException, "Geometry::jacobianTransposed requires a reference element." );
    }

    const JacobianInverseTransposed &jacobianInverseTransposed ( const LocalCoordinate &local ) const
    {
      DUNE_THROW( InvalidStateException, "Geometry::jacobianInverseTransposed requires a reference element." );
    }

  private:
    Polytope polytope_;
  };

} // namespace Dune

#endif // #ifndef DUNE_POLYTOPEGEOMETRY_HH
