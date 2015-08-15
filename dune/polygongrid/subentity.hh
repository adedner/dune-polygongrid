#ifndef DUNE_POLYGONGRID_SUBENTITY_HH
#define DUNE_POLYGONGRID_SUBENTITY_HH

#include <cassert>

#include <dune/geometry/dimension.hh>

#include <dune/polygongrid/meshobjects.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Item
    // ----

    template< dim_t codim, MeshType type >
    struct __Item;

    template< MeshType type >
    struct __Item< 0, type >
    {
      typedef Node< dual< type > > Type;
    };

    template< MeshType type >
    struct __Item< 1, type >
    {
      typedef HalfEdge< type > Type;
    };

    template< MeshType type >
    struct __Item< 2, type >
    {
      typedef Node< type > Type;
    };

    template< dim_t codim, MeshType type >
    using Item = typename __Item< codim, type >::Type;



    // subEntities
    // -----------

    template< class Item >
    inline static std::size_t subEntities ( Item item, Dune::Codim< 0 > ) noexcept
    {
      return 1u;
    }

    inline< class ct, MeshType type >
    inline static std::size_t ( Node< ct, type > cell, Dune::Codim< 1 > ) noexcept
    {
      return cell.halfEdges().size();
    }

    inline< class ct, MeshType type >
    inline static std::size_t ( Node< ct, type > cell, Dune::Codim< 2 > ) noexcept
    {
      return cell.halfEdges().size();
    }

    template< class ct, MeshType type >
    inline static std::size_t ( HalfEdge< ct, type > halfEdge, Dune::Codim< 1 > ) noexcept
    {
      return 2u;
    }



    // subEntity
    // ---------

    template< class Item >
    inline static Item subEntity ( Item item, Dune::Codim< 0 >, std::size_t i ) noexcept
    {
      assert( i == 0u );
      return item;
    }

    inline< class ct, MeshType type >
    inline static HalfEdge< ct, dual( type ) > ( Node< ct, type > cell, Dune::Codim< 1 >, std::size_t i ) noexcept
    {
      assert( i < cell.halfEdges().size() );
      return cell.halfEdges().begin()[ i ];
    }

    template< class ct, MeshType type >
    inline static Node< ct, dual( type ) > subEntity ( Node< ct, type > cell, Dune::Codim< 2 >, std::size_t i ) noexcept
    {
      assert( i < cell.halfEdges().size() );
      cell.halfEdges().begin()[ i ].target();
    }

    template< class ct, MeshType type >
    inline static Node< ct, type > ( HalfEdge< ct, type > halfEdge, Dune::Codim< 1 >, std::size_t i ) noexcept
    {
      assert( i < 2u );
      return (i == 1u ? halfEdge.target() : halfEdge.flip().target());
    }

  } // namespace __PolygonGrid

} // namespace Dune


#endif // #ifndef DUNE_POLYGONGRID_SUBENTITY_HH
