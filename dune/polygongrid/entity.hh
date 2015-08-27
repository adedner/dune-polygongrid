#ifndef DUNE_POLYGONGRID_ENTITY_HH
#define DUNE_POLYGONGRID_ENTITY_HH

#include <dune/grid/common/entity.hh>

#include <dune/geometry/dimension.hh>

#include <dune/grid/common/entity.hh>
#include <dune/grid/common/entityiterator.hh>
#include <dune/grid/common/exceptions.hh>

#include <dune/polygongrid/entityseed.hh>
#include <dune/polygongrid/geometry.hh>
#include <dune/polygongrid/subentity.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Internal Forward Declarations
    // -----------------------------

    template< class, dim_t codim >
    class Entity;



    // External Forward Declarations
    // -----------------------------

    template< class, dim_t codim >
    class EntityIterator;



    // BasicEntity
    // -----------

    template< class Item, dim_t codim >
    class BasicEntity
    {
      typedef BasicEntity< Item, codim > This;

      typedef __PolygonGrid::EntitySeed< typename Item::Index, codim > EntitySeedImpl;
      typedef __PolygonGrid::Geometry< Item, codim > GeometryImpl;

    public:
      static const dim_t codimension = codim;
      static const dim_t dimension = 2;
      static const dim_t mydimension = dimension - codimension;

      template< dim_t cd >
      struct Codim
      {
        typedef decltype( __PolygonGrid::subEntity( std::declval< Item >(), Dune::Codim< cd - codimension >(), std::size_t() ) ) SubItem;
        typedef Dune::Entity< cd, __PolygonGrid::Entity< SubItem, cd > > Entity;
      };

      typedef Dune::EntitySeed< EntitySeedImpl > EntitySeed;
      typedef Dune::Geometry< GeometryImpl > Geometry;

      explicit BasicEntity ( const Item &item ) : item_( item ) {}

      GeometryType type () const { return GeometryType( GeometryType::None(), mydimension ); }

      PartitionType partitionType () const { return InteriorEntity; }

      Geometry geometry () const { return GeometryImpl( item() ); }

      EntitySeed seed () const { return EntitySeedImpl( item().index() ); }

      template< dim_t cd >
      typename Codim< cd >::Entity subEntity ( int i ) const
      {
        typedef typename Codim< cd >::Entity::Implementation EntityImpl;
        return EntityImpl( __PolygonGrid::subEntity( item(), Dune::Codim< cd - codimension >(), i ) );
      }

      std::size_t index () const { return item().uniqueIndex(); }

      const Item &item () const { return item_; }

    protected:
      Item item_;
    };



    // Entity for Codimension 2
    // ------------------------

    template< class Item >
    class Entity< Item, 2 >
      : public BasicEntity< Item, 2 >
    {
      typedef Entity< Item, 2 > This;
      typedef BasicEntity< Item, 2 > Base;

    public:
      using Base::item;

      explicit Entity ( const Item &item ) : Base( item ) {}

      std::size_t subEntities ( dim_t codim ) const noexcept { return (codim == 2 ? 1u : 0u); }

      std::size_t subIndex ( dim_t codim, std::size_t i ) const noexcept
      {
        assert( i < subEntities( codim ) );
        return __PolygonGrid::subEntity( item(), Dune::Codim< 0 >(), i ).uniqueIndex();
      }
    };



    // Entity for Codimension 1
    // ------------------------

    template< class Item >
    class Entity< Item, 1 >
      : public BasicEntity< Item, 1 >
    {
      typedef Entity< Item, 1 > This;
      typedef BasicEntity< Item, 1 > Base;

    public:
      using Base::item;

      explicit Entity ( const Item &item ) : Base( item ) {}

      std::size_t subEntities ( dim_t codim ) const noexcept { return (codim == 2 ? 2u : (codim == 1 ? 1u : 0u)); }

      std::size_t subIndex ( dim_t codim, std::size_t i ) const noexcept
      {
        assert( i < subEntities( codim ) );
        if( codim == 2 )
          return __PolygonGrid::subEntity( item(), Dune::Codim< 0 >(), i ).uniqueIndex();
        else
          return __PolygonGrid::subEntity( item(), Dune::Codim< 1 >(), i ).uniqueIndex();
      }
    };



    // Entity for codimension 0
    // ------------------------

    template< class Item >
    class Entity< Item, 0 >
      : public BasicEntity< Item, 0 >
    {
      typedef Entity< Item, 0 > This;
      typedef BasicEntity< Item, 0 > Base;

      typedef __PolygonGrid::EntityIterator< Item, 0 > HierarchicIteratorImpl;

    public:
      typedef typename Base::Geometry LocalGeometry;

      typedef Dune::EntityIterator< HierarchicIteratorImpl > HierarchicIterator;

      using Base::item;

      explicit Entity ( const Item &item ) : Base( item ) {}

      unsigned int subEntities ( dim_t codim ) const noexcept
      {
        return ((codim == 1) || (codim == 2) ? item().halfEdges().size() : (codim == 0u ? 1u : 0u));
      }

      std::size_t subIndex ( dim_t codim, std::size_t i ) const noexcept
      {
        assert( i < subEntities( codim ) );
        switch( codim )
        {
        case 0:
          return __PolygonGrid::subEntity( item(), Dune::Codim< 0 >(), i ).uniqueIndex();

        case 1:
          return __PolygonGrid::subEntity( item(), Dune::Codim< 1 >(), i ).uniqueIndex();

        case 2:
          return __PolygonGrid::subEntity( item(), Dune::Codim< 2 >(), i ).uniqueIndex();
        }
      }

      bool isLeaf () const { return true; }

      Entity father () const { DUNE_THROW( GridError, "Father does not exist." ); }

      bool hasFather () const { return false; }

      LocalGeometry geometryInFather () const { DUNE_THROW( GridError, "Father does not exist." ); }

      HierarchicIterator hbegin ( int maxLevel ) const { return HierarchicIteratorImpl(); }
      HierarchicIterator hend ( int maxLevel ) const { return HierarchicIteratorImpl(); }

      bool isRegular () const { return true; }
      bool isNew () const { return false; }
      bool mightVanish () const { return false; }
    };

 } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITY_HH
