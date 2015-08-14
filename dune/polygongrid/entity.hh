#ifndef DUNE_POLYGONGRID_ENTITY_HH
#define DUNE_POLYGONGRID_ENTITY_HH

#include <dune/grid/common/entity.hh>

#include <dune/geometry/dimension.hh>

#include <dune/polygongrid/entityseed.hh>
#include <dune/polygongrid/geometry.hh>


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

    template< class MeshObject, dim_t codim >
    class BasicEntity
    {
      typedef BasicEntity< MeshObject, codim > This;

      typedef __PolygonGrid::Geometry< MeshObject, codim > GeometryImpl;

    public:
      static const dim_t codimension = codim;
      static const dim_t dimension = 2;
      static const dim_t mydimension = dimension - codimension;

      // typedef typename Grid::template Codim< codimension >::EntitySeed EntitySeed;
      typedef Dune::Geometry< GeometryImpl > Geometry;

      explicit Entity ( const MeshObject &meshObject ) : meshObject_( meshObject ) {}

      GeometryType type () const { return GeometryType( GeometryType::None(), mydimension ); }

      PartitionType partitionType () const { return InteriorEntity; }

      Geometry geometry () const { return GeometryImpl( meshObject_ ); }

      // EntitySeed seed () const { return typename EntitySeed::Implementation( index() ); }

#if 0
      unsigned int subEntities ( dim_t codim ) const
      {
        // use a helper, here
      }

      template< dim_t codim >
      typename Codim< codim >::Entity subEntity ( int i ) const
      {
        typedef typename Traits::template Codim< codim >::EntityImpl EntityImpl;
        assert( static_cast< unsigned int >( i ) < subEntities( codim ) );
        return EntityImpl( grid(), subIndex( i, codim ) );
      }

      Index index () const { return index_; }

      Index subIndex ( int i, dim_t codim ) const
      {
        // use a helper, here
      }
#endif

    private:
      MeshObject meshObject_;
    };



    // Entity
    // ------

    template< dim_t codim, class Grid >
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

      typedef typename Traits::HierarchicIterator HierarchicIterator;

      Entity ( const Grid &grid, Index index ) : Base( grid, index ) {}

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
