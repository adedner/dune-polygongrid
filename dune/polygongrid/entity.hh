#ifndef DUNE_POLYGONGRID_ENTITY_HH
#define DUNE_POLYGONGRID_ENTITY_HH

#include <dune/grid/common/entity.hh>

#include <dune/geometry/dimension.hh>

#include <dune/polygongrid/entityseed.hh>
#include <dune/polygongrid/geometry.hh>
#include <dune/polygongrid/subentitiy.hh>

namespace Dune
{

  namespace __PolygonGrid
  {

    // Internal Forward Declarations
    // -----------------------------

    template< class, dim_t codim >
    class Entity;



    // BasicEntity
    // -----------

    template< class Item, dim_t codim >
    class BasicEntity
    {
      typedef BasicEntity< Item, codim > This;

      typedef __PolygonGrid::Geometry< Item, codim > GeometryImpl;

    public:
      static const dim_t codimension = codim;
      static const dim_t dimension = 2;
      static const dim_t mydimension = dimension - codimension;

      template< dim_t cd >
      struct Codim
      {
        typedef decltype( __PolygonGrid::subEntity( std::declval< Item >(), Dune::Codim< cd - codimension >(), std::size_t() ) ) SubItem;
        typedef Dune::Entity< __PolygonGrid::Entity< SubItem, cd > > Entity;
      };

      // typedef typename Grid::template Codim< codimension >::EntitySeed EntitySeed;
      typedef Dune::Geometry< GeometryImpl > Geometry;

      explicit Entity ( const Item &item ) : item_( item ) {}

      GeometryType type () const { return GeometryType( GeometryType::None(), mydimension ); }

      PartitionType partitionType () const { return InteriorEntity; }

      Geometry geometry () const { return GeometryImpl( item_ ); }

      // EntitySeed seed () const { return typename EntitySeed::Implementation( index() ); }

      template< dim_t cd >
      typename Codim< cd >::Entity subEntity ( int i ) const
      {
        typedef typename Codim< cd >::Entity::Implementation EntityImpl;
        return EntityImpl( __PolygonGrid::subEntity( item_, Dune::Codim< cd - codimension >(), i ) );
      }

      Index index () const { return item_.index(); }

    private:
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
      explicit Entity ( const Item &item ) : Base( item ) {}

      unsigned int subEntities ( dim_t codim ) const noexcept { return (codim == 2 ? 1u : 0u); }
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
      explicit Entity ( const Item &item ) : Base( item ) {}

      unsigned int subEntities ( dim_t codim ) const noexcept { return (codim == 2 ? 2u : (codim == 1 ? 1u : 0u)); }
    };



    // Entity for codimension 0
    // ------------------------

    template< class Item >
    class Entity< Item, 0 >
      : public BasicEntity< Item, 0 >
    {
      typedef Entity< Item, 0 > This;
      typedef BasicEntity< Item, 0 > Base;

    public:
      typedef typename Traits::template Codim< 0 >::LocalGeometry LocalGeometry;

      typedef typename Traits::HierarchicIterator HierarchicIterator;

      explicit Entity ( const Item &item ) : Base( item ) {}

      unsigned int subEntities ( dim_t codim ) const noexcept
      {
        return ((codim == 1) || (codim == 2) ? item_.halfEdges().size() : (codim == 0u ? 1u : 0u));
      }

      bool isLeaf () const { return true; }

      Entity father () const { DUNE_THROW( GridError, "Father does not exist." ); }

      bool hasFather () const { return false; }

      LocalGeometry geometryInFather () const { DUNE_THROW( GridError, "Father does not exist." ); }

      HierarchicIterator hbegin ( int maxLevel ) const
      {
        // ...
      }

      HierarchicIterator hend ( int maxLevel ) const
      {
        // ...
      }

      bool isRegular () const { return true; }
      bool isNew () const { return false; }
      bool mightVanish () const { return false; }
    };

 } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITY_HH
