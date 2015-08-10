#ifndef DUNE_POLYGONGRID_MULTIVECTOR_HH
#define DUNE_POLYGONGRID_MULTIVECTOR_HH

namespace Dune
{

  namespace __PolygonGrid
  {

    // MultiVector
    // -----------

    /**
     * \brief efficient vector of small vectors
     *
     * The MultiVector< T > essentially behaves like a
     * std::vector< std::vector< T > >.
     */
    template< class T >
    struct MultiVector
    {
      typedef T value_type;

      typedef std::pair< std::size_t, std::size_t > index_type;

      explicit MultiVector ( const std::vector< std::size_t > &counts )
      {
        computeOffsets( counts );
        values_.resize( offsets_.back() );
      }

      MultiVector ( const std::vector< std::size_t > &counts, const T &value )
      {
        computeOffsets( counts );
        values_.resize( offsets_.back(), value );
      }

      const value_type &operator[] ( index_type i ) const noexcept { return values_[ offsets_[ i.first ] + i.second ]; }
      value_type operator[] ( index_type i ) noexcept { return values_[ offsets_[ i.first ] + i.second ]; }

      const value_type &at ( index_type i ) const { return values_.at( offsets_.at( i.first ) + i.second ); }
      value_type &at ( index_type i ) { return values_.at( offsets_.at( i.first ) + i.second ); }

      bool empty () const noexcept { return (offsets_.size() == 1u); }
      std::size_t size () const noexcept { return (offsets_.size()-1); }

      void clear ()
      {
        offsets_.resize( 1 );
        values_.clear();
      }

      void push_back ( std::vector< T > &vector )
      {
        std::size_t size = vector.size();
        offsets_.push_back( offsets_.back() + size );
        values_.reserve( offsets_.back() );
        for( const T &value : vector )
          values_.push_back( value );
      }

      void pop_pack ()
      {
        offsets_.pop_back();
        values_.resize( offsets_.back() );
      }

      const std::vector< T > &values () const noexcept { return values_; }
      std::vector< T > &values () noexcept { return values_; }

      void sortEach ()
      {
        for( std::size_t k = 0u; k < size(); ++k )
          std::sort( values_.begin() + offsets_[ k ], values_.begin() + offets_[ k+1 ] );
      }

      void uniqueEach ()
      {
        auto pos = values_.begin();
        std::size_t offset = 0u;
        for( std::size_t k = 0u; k < size(); ++k )
        {
          auto end = std::unique_copy( values_.begin() + offset, values_.end() + offsets_[ k+1 ], pos );
          offset_[ k+1 ] = std::distance( pos, end );
          pos = end;
        }
        values_.resize( offsets_.back() );
      }

    private:
      void computeOffsets ( const std::vector< std::size_t > &counts )
      {
        const std::size_t size = counts.size();
        offsets_.resize( size+1 );
        offsets_[ 0 ] = 0u;
        for( std::size_t k = 0u; k < size; ++k )
          offsets_[ k+1 ] = offsets_[ k ] + counts[ k ];
      }

      std::vector< std::size_t > offsets_;
      std::vector< T > values_;
    };

  } // namespace __PolygonGrid

} // namespace Dune

#endif // #ifndef DUNE_POLYGONGRID_MULTIVECTOR_HH
