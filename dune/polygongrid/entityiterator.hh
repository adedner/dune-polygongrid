#ifndef DUNE_POLYGONGRID_ENTITYITERATOR_HH
#define DUNE_POLYGONGRID_ENTITYITERATOR_HH

#include <cassert>
#include <cstddef>

#include <type_traits>

#include <dune/common/iterator/tags.hh>

#include <dune/geometry/dimension.hh>

#include <dune/polygongrid/entity.hh>
#include <dune/polygongrid/meshobjects.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Internal Forward Declarations
    // -----------------------------

    template< class, dim_t codim >
    class EntityIterator;



    // EntityIterator for Node
    // -----------------------

    template< class ct, dim_t codim >
    class EntityIterator< Node< ct >, codim >
    {
      typedef EntityIterator< Node< ct >, codim > This;

      typedef IndexIterator< Node< ct > > Iterator;
      typedef __PolygonGrid::Entity< Node< ct >, codim > EntityImpl;

    public:
      static const dim_t codimension = codim;
      static const dim_t dimension = 2;
      static const dim_t mydimension = dimension - codimension;

      typedef Dune::Entity< codim, EntityImpl > Entity;

      EntityIterator () = default;

      EntityIterator ( Tag::Begin, const Mesh< ct > &mesh, MeshType type ) : iterator_( mesh, mesh.begin( codim == 0 ? dual( type ) : type ) ) {}
      EntityIterator ( Tag::End, const Mesh< ct > &mesh, MeshType type ) : iterator_( mesh, mesh.end( codim == 0 ? dual( type ) : type ) ) {}

      Entity dereference () const { return EntityImpl( *iterator_ ); }

      bool equals ( const This &other ) const noexcept { return (iterator_ == other.iterator_); }

      void increment () noexcept { ++iterator_; }

    protected:
      Iterator iterator_;
    };



    // EntityIterator for HalfEdge
    // ---------------------------

    template< class ct, dim_t codim >
    class EntityIterator< HalfEdge< ct >, codim >
    {
      typedef EntityIterator< HalfEdge< ct >, codim > This;

      typedef IndexIterator< HalfEdge< ct > > Iterator;
      typedef __PolygonGrid::Entity< HalfEdge< ct >, codim > EntityImpl;

    public:
      static const dim_t codimension = codim;
      static const dim_t dimension = 2;
      static const dim_t mydimension = dimension - codimension;

      typedef Dune::Entity< codimension, EntityImpl > Entity;

      EntityIterator () = default;

      EntityIterator ( Tag::Begin, const Mesh< ct > &mesh, MeshType type ) : iterator_( begin( mesh, type ) ) { advance(); }
      EntityIterator ( Tag::End, const Mesh< ct > &mesh, MeshType type ) : iterator_( end( mesh, type ) ) {}

      Entity dereference () const { return EntityImpl( *iterator_ ); }

      bool equals ( const This &other ) const noexcept { return (iterator_ == other.iterator_); }

      void increment () noexcept { ++iterator_; advance(); }

    protected:
      static Iterator begin ( const Mesh< ct > &mesh, MeshType type ) { return Iterator( mesh, mesh.begin( mesh.begin( dual( type ) ) ) ); }
      static Iterator end ( const Mesh< ct > &mesh, MeshType type ) { return Iterator( mesh, mesh.end( --mesh.end( dual( type ) ) ) ); }

      Iterator begin () const { return begin( iterator_->mesh(), iterator_->type() ); }
      Iterator end () const { return end( iterator_->mesh(), iterator_->type() ); }

      void advance ()
      {
        for( ; (iterator_ != end()) && (iterator_->cell().index() > iterator_->neighbor().index()); ++iterator_ )
          continue;
      }

      Iterator iterator_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITYITERATOR_HH
