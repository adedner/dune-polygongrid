#ifndef DUNE_POLYGONGRID_MULTIVECTOR_HH
#define DUNE_POLYGONGRID_MULTIVECTOR_HH

namespace Dune
{

  namespace __PolygonGrid
  {

    // MultiVector
    // -----------

    template< class T >
    struct MultiVector
    {
      typedef T value_type;

      explicit MultiVector ( const std::vector< std::size_t > &counts )
      {
        computeOffsets( counts );
        values_.resize( offsets_.back() );
      }

    private:
      void computeOffsets ( const std::vector< std::size_t > &counts )
      {
        const std::size_t size = counts.size();
        offsets_.resize( size+1 );
        offsets_[ 0 ] = 0u;
        for( std::size_t k = 0u; k < size; ++k )
          offsets_[ k+1 ] = offsets_[ k ] + 2*counts[ k ];
      }

      std::vector< std::size_t > offsets_;
      std::vector< value_type > values_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MULTIVECTOR_HH
