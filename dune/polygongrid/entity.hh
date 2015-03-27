#ifndef DUNE_POLYGONGRID_ENTITY_HH
#define DUNE_POLYGONGRID_ENTITY_HH

#include <dune/common/typetraits.hh>

#include <dune/grid/common/entity.hh>

#include <dune/polygongrid/entityseed.hh>


namespace Dune
{

  namespace __PolygonGrid
  {

    // Internal Forward Declarations
    // -----------------------------

    template< int codim, class Grid >
    class Entity;



    // BasicEntity
    // -----------

    template< int codim, class Grid >
    class BasicEntity
    {
      typedef typename remove_const< Grid >::type::Traits Traits;

    public:
      static const int codimension = codim;
      static const int dimension = 2;
      static const int mydimension = dimension - codimension;

      typedef typename Traits::ctype ctype;
      typedef typename Traits::Index Index;

      typedef typename Grid::template Codim< codimension >::EntitySeed EntitySeed;
      typedef typename Traits::template Codim< codimension >::Geometry Geometry;

      Entity ( const Grid &grid, Index index ) : grid_( &grid ), index_( index ) {}

      operator bool () const { return index_ < std::numeric_limits< Index >::max(); }

      GeometryType type () const { return GeometryType( GeometryType::None(), dimension ); }

      PartitionType partitionType () const { return InteriorEntity; }

      Geometry geometry () const { return Geometry( grid(), index() ); }

      EntitySeed seed () const { return typename EntitySeed::Implementation( index() ); }

      unsigned int subEntities ( unsigned int codim ) const
      {
        // use a helper, here
      }

      template< int codim >
      typename Codim< codim >::Entity subEntity ( int i ) const
      {
        typedef typename Traits::template Codim< codim >::EntityImpl EntityImpl;
        assert( static_cast< unsigned int >( i ) < subEntities( codim ) );
        return EntityImpl( grid(), subIndex( i, codim ) );
      }

      const Grid &grid () const { return *grid_; }

      Index index () const { return index_; }

      Index subIndex ( int i, unsigned int codim ) const
      {
        // use a helper, here
      }

    private:
      const Grid *grid_;
      Index index_;
    };



    // Entity
    // ------

    template< int codim, class Grid >
    class Entity< codim, Grid >
      : public BasicEntity< codim, Grid >
    {
      typedef BasicEntity< codim, Grid > Base;

    public:
      Entity ( const Grid &grid, Index index ) : Base( grid, index ) {}
    };



    // Entity for codimension 0
    // ------------------------

    template< class Grid >
    class Entity< 0, Grid >
      : public BasicEntity< 0, Grid >
    {
      typedef Entity< 0, Grid > This;
      typedef BasicEntity< 0, Grid > Base;

    public:
      typedef typename Traits::template Codim< 0 >::LocalGeometry LocalGeometry;
      typedef typename Traits::template Codim< 0 >::EntityPointer EntityPointer;

      typedef typename Traits::HierarchicIterator HierarchicIterator;

      Entity ( const Grid &grid, Index index ) : Base( grid, index ) {}

      bool isLeaf () const { return true; }

      Entity father () const { DUNE_THROW( GridError, "Father does not exist." ); }

      bool hasFather () const { return false; }

      LocalGeometry geometryInFather () const { DUNE_THROW( GridError, "Father does not exist." ); }

      HierarchicIterator hbegin ( int maxLevel ) const
      {
        typedef IdGridIterator< Grid, typename HostEntity::HierarchicIterator > IteratorImpl;
        return IteratorImpl( hostEntity().hbegin( maxLevel ) );
      }

      HierarchicIterator hend ( int maxLevel ) const
      {
        typedef IdGridIterator< Grid, typename HostEntity::HierarchicIterator > IteratorImpl;
        return IteratorImpl( hostEntity().hend( maxLevel ) );
      }

      bool isRegular () const { return true; }
      bool isNew () const { return false; }
      bool mightVanish () const { return false; }
    };

 } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITY_HH
