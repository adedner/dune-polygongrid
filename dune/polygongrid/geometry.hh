#ifndef DUNE_POLYGONGRID_GEOMETRY_HH
#define DUNE_POLYGONGRID_GEOMETRY_HH

#include <dune/common/exceptions.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>
#include <dune/common/math.hh>

#include <dune/polygongrid/mesh.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Internal Forward Declarations
    // -----------------------------

    template< class ct, MeshType meshtype, int codim >
    class Geometry;



    // BasicGeometry
    // -------------

    template< class ct, int codim >
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

    template< class ct, MeshType meshtype >
    class Geometry< ct, meshtype, 0 >
      : public BasicGeometry< ct, 0 >
    {
      typedef Geometry< ct, meshtype, 0 > This;
      typedef BasicGeometry< ct, 0 > Base;

    public:
      typedef typename Base::ctype ctype;
      typedef typename Base::GlobalCoordinate GlobalCoordinate;

      typedef __PolygonGrid::Mesh< ct > Mesh;
      typedef __PolygonGrid::NodeIndex< type > NodeIndex;

      Geometry () = default;

      Geometry ( const Mesh &mesh, NodeIndex index ) : mesh_( &mesh ), index_( index ) {}

      DUNE_INLINE int corners () const noexcept { return mesh().size( index_ ); }

      DUNE_INLINE const GlobalCoordinate &corner ( int i ) const noexcept
      {
        return mesh().position( mesh().begin( index_ ) + static_cast< std::size_t >( i ) );
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

      const Mesh &mesh () const noexcept { assert( mesh_ ); return *mesh_; }

    private:
      const Mesh *mesh_ = nullptr;
      NodeIndex index_;
    };



    // Geometry for Codimension 1
    // --------------------------

    template< class ct, MeshType meshtype >
    class Geometry< ct, meshtype, 1 >
      : public BasicGeometry< ct, 1 >
    {
      typedef Geometry< ct, meshtype, 1 > This;
      typedef BasicGeometry< ct, 1 > Base;

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

    template< class ct, MeshType meshtype >
    class Geometry< ct, meshtype, 2 >
      : public BasicGeometry< ct, 2 >
    {
      typedef Geometry< ct, meshtype, 2 > This;
      typedef BasicGeometry< ct, 2 > Base;

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
