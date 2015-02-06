#ifndef DUNE_POLYGONGRID_ENTITY_HH
#define DUNE_POLYGONGRID_ENTITY_HH

#include <dune/common/typetraits.hh>

#include <dune/grid/common/entity.hh>

#include <dune/grid/polygongrid/entityseed.hh>
#include <dune/grid/polygongrid/geometry.hh>


namespace Dune
{

  // Internal Forward Declarations
  // -----------------------------

  template< int codim, int dim, class Grid >
  class PolygonGridEntity;



  // PolygonGridBasicEntity
  // ----------------------

  template< int codim, class Grid >
  class PolygonGridBasicEntity
  {
    typedef typename remove_const< Grid >::type::Traits Traits;

  public:
    static const int codimension = codim;
    static const int dimension = 2;
    static const int mydimension = dimension - codimension;
    static const int dimensionworld = 2;

    typedef typename Traits::ctype ctype;
    typedef typename Traits::Index Index;

    typedef typename Grid::template Codim< codimension >::EntitySeed EntitySeed;
    typedef typename Traits::template Codim< codimension >::Geometry Geometry;

    PolygonGridEntity ( const Grid &grid, Index index )
    : grid_( &grid ).
      index_( index )
    {}

    operator bool () const { return index_ < std::numeric_limits< Index >::max(); }

    GeometryType type () const { return GeometryType( GeometryType::None(), dimension ); }

    PartitionType partitionType () const { return InteriorEntity; }

    Geometry geometry () const { return Geometry( grid(), index() ); }

    EntitySeed seed () const { return typename EntitySeed::Implementation( index() ); }

    template< int cd >
    int count () const
    {
      // use a helper, here
    }

    template< int cd >
    typename Grid::template Codim< cd >::EntityPointer
    subEntity ( int i ) const
    {
      typedef typename Traits::template Codim< cd >::EntityPointerImpl EntityPointerImpl;
      assert( (i >= 0) && (i <= count< cd >()) );
      return EntityPointerImpl( grid(), subIndex( i, cd ) );
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



  // PolygonGridEntity
  // -----------------

  template< int codim, class Grid >
  class PolygonGridEntity< codim, 2, Grid >
  : public PolygonGridBasicEntity< codim, Grid >
  {
    typedef PolygonGridBasicEntity< codim, Grid > Base;

  public:
    PolygonGridEntity ( const Grid &grid, Index index )
    : Base( grid, index )
    {}
  };



  // PolygonGridEntity for codimension 0
  // -----------------------------------

  template< class Grid >
  class PolygonGridEntity< 0, 2, Grid >
  : public PolygonGridBasicEntity< 0, Grid >
  {
    typedef PolygonGridBasicEntity< 0, Grid > Base;

  public:
    typedef typename Traits::template Codim< 0 >::LocalGeometry LocalGeometry;
    typedef typename Traits::template Codim< 0 >::EntityPointer EntityPointer;

    typedef typename Traits::HierarchicIterator HierarchicIterator;

    PolygonGridEntity ( const Grid &grid, Index index )
    : Base( grid, index )
    {}

    bool hasBoundaryIntersections () const
    {
      return hostEntity().hasBoundaryIntersections();
    }

    bool isLeaf () const { return true; }
 
    EntityPointer father () const
    {
      typedef typename Traits::template Codim< 0 >::EntityPointerImpl EntityPointerImpl;
      return EntityPointerImpl( hostEntity().father() );
    }

    bool hasFather () const { return false; }

    LocalGeometry geometryInFather () const
    {
      return LocalGeometry( hostEntity().geometryInFather() );
    }
 
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

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_ENTITY_HH
