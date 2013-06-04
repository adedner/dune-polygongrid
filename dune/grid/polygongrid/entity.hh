#ifndef DUNE_POLYGONGRID_ENTITY_HH
#define DUNE_POLYGONGRID_ENTITY_HH

//- dune-common includes
#include <dune/common/typetraits.hh>

//- dune-grid includes
#include <dune/grid/common/entity.hh>

//- dune-polygongrid includes
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

    GeometryType type () const
    {
      GeometryType type;
      type.makeNone( mydimension );
      return type;
    }

    int level () const { return 0; }
    
    PartitionType partitionType () const { return InteriorEntity; }

    Geometry geometry () const { return Geometry( grid(), index() ); }

    EntitySeed seed () const { return typename EntitySeed::Implementation( index() ); }

    Index index () const { return index_; }
    const Grid &grid () const { return *grid_; }

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

    template< int codim >
    int count () const { return (codim == 0 ? 1 : numVertices()); }
    
    template< int codim >
    typename Grid::template Codim< codim >::EntityPointer
    subEntity ( int i ) const
    {
      typedef typename Traits::template Codim< codim >::EntityPointerImpl EntityPointerImpl;
      return EntityPointerImpl( hostEntity().template subEntity< codim >( i ) );
    }

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

    bool hasFather () const
    {
      return hostEntity().hasFather();
    }
      
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
