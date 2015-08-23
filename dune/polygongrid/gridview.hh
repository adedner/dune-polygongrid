#ifndef DUNE_POLYGONGRID_GRIDVIEW_HH
#define DUNE_POLYGONGRID_GRIDVIEW_HH

#include <functional>
#include <type_traits>

#include <dune/common/parallel/collectivecommunication.hh>

#include <dune/geometry/type.hh>

#include <dune/grid/common/datahandleif.hh>
#include <dune/grid/common/gridenums.hh>

#include <dune/polygongrid/declaration.hh>
#include <dune/polygongrid/entity.hh>
#include <dune/polygongrid/entityiterator.hh>
#include <dune/polygongrid/indexset.hh>
#include <dune/polygongrid/intersection.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // GridView
    // --------

    template< class ct >
    class GridView
    {
      typedef GridView< ct > This;

    public:
      typedef PolygonGrid< ct > Grid;

      typedef __PolygonGrid::IndexSet< const Grid > IndexSet;

      template< dim_t codim >
      struct Codim
      {
        typedef typename std::conditional< codim == 1 ? HalfEdge< ctype > : Node< ctype > >::type Item;

        typedef Dune::Entity< codim, __PolygonGrid::Entity< Item, codim > > Entity;

        typedef typename Entity::EntitySeed EntitySeed;
        typedef typename Entity::Geometry Geometry;

        // local geometry does not make sense; add a phony typedef
        typedef Geometry LocalGeometry;

        template< PartitionIteratorType pitype >
        struct Partition
        {
          typedef Dune::EntityIterator< __PolygonGrid::EntityIterator< Item, codim > > Iterator;
        };
      };

      typedef Dune::IntersectionIterator< __PolygonGrid::IntersectionIterator< ct > > IntersectionIterator;

      typedef Dune::CollectiveCommunication< NoComm > CollectiveCommunication;

      GridView ( const Grid &grid ) : grid_( grid ) {}

      const IndexSet &indexSet () const { return indexSet_; }

      template< class Entity >
      bool contains ( const Entity &entity ) const
      {
        return indexSet().contains( entity );
      }

      int size ( int codim ) const { return indexSet().size( codim ); }

      int size ( GeometryType type ) const { return indexSet().size( type ); }

      template< int codim, PartitionIteratorType pitype >
      typename Codim< codim >::template Partition< pitype >::Iterator begin () const
      {
        typedef typename Codim< codim >::template Partition< pitype >::Iterator::Implementation IteratorImpl;
        return IteratorImpl( hierarchy(), level(), 0 );
      }

      template< int codim, PartitionIteratorType pitype >
      typename Codim< codim >::template Partition< pitype >::Iterator end () const
      {
        typedef typename Codim< codim >::template Partition< pitype >::Iterator::Implementation IteratorImpl;
        const int size = (pitype == Ghost_Partition) ? 0 : this->size( codim );
        return IteratorImpl( hierarchy(), level(), size );
      }

      IntersectionIterator ibegin ( const typename Codim< 0 >::Entity &entity ) const
      {
        typedef typename IntersectionIterator::Implementation IntersectionIteratorImpl;
        return IntersectionIteratorImpl( entity.impl().item().halfEdges().begin() );
      }

      IntersectionIterator iend ( const typename Codim< 0 >::Entity &entity ) const
      {
        typedef typename IntersectionIterator::Implementation IntersectionIteratorImpl;
        return IntersectionIteratorImpl( entity.impl().item().halfEdges().end() );
      }

      template< class DataHandle, class DataType >
      void communicate ( CommDataHandleIF< DataHandle, DataType > &, InterfaceType, CommunicationDirection ) const
      {}

      const CollectiveCommunication &comm () const { return grid().comm(); }

      const Grid &grid () const { return grid_; }

    private:
      std::reference_wrapper< const Grid > grid_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_GRIDVIEW_HH
