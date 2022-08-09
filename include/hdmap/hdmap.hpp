#ifndef HDMAP_HDMAP_HPP
#define HDMAP_HDMAP_HPP

#include <cstdint>
#include <cassert>
#include <concepts>
#include <bit>
#include <vector>
#include <variant>
#include <ranges>
#include <unordered_map>
#include <type_traits>
#include <boost/integer.hpp>
#include <boost/container/static_vector.hpp>

namespace hdmap {

template< typename T, unsigned int dims >
struct rectangle {
  T left_top = 0u;
  T right_bottom = 0u;
};

template< typename T, unsigned int dims >
bool operator==( const rectangle< T, dims > &l, const rectangle< T, dims > &r ) {
  return l.left_top == r.left_top && l.left_top == r.right_bottom;
}

template< typename T, unsigned int dims >
bool operator!=( const rectangle< T, dims > &l, const rectangle< T, dims > &r ) {
  return l.left_top != r.left_top || l.left_top != r.right_bottom;
}

namespace detail {

enum class camode_t {
  FULL,
  AXIS
};

template< unsigned int dims >
struct camap_traits {
  constexpr static const auto mode = camode_t::AXIS;
  constexpr static const auto map_bits = 4u * dims;
  constexpr static const auto count_bits = 2u * dims + 1u;
  using type = boost::uint_t< map_bits + count_bits >::least;
};

template<>
struct camap_traits< 1u > {
  constexpr static const auto mode = camode_t::FULL;
  constexpr static const auto map_bits = 4u;
  constexpr static const auto count_bits = 0u;
  using type = std::uint8_t;
};

template<>
struct camap_traits< 2u > {
  constexpr static const auto mode = camode_t::FULL;
  constexpr static const auto map_bits = 16u;
  constexpr static const auto count_bits = 0u;
  using type = std::uint16_t;
};

template<>
struct camap_traits< 3u > {
  constexpr static const auto mode = camode_t::FULL;
  constexpr static const auto map_bits = 64u;
  constexpr static const auto count_bits = 0u;
  using type = std::uint64_t;
};

template< unsigned int dims >
struct struct_key {
  std::uint8_t depth = 0u;
  std::array< std::uint64_t, dims > position = { 0 };
};

template< unsigned int dims >
bool operator==( const struct_key< dims > &l, const struct_key< dims > &r ) {
  return l.depth == r.depth && l.position == r.position;
};

template< unsigned int dims >
bool operator!=( const struct_key< dims > &l, const struct_key< dims > &r ) {
  return l.depth != r.depth || l.position != r.position;
};

template< typename T >
struct is_struct_key : public std::false_type {};
template< unsigned int dims >
struct is_struct_key< struct_key< dims > > : public std::true_type {};
template< typename T >
constexpr auto is_struct_key_v = is_struct_key< T >::value;

template< typename T >
concept StructKey = detail::is_struct_key_v< std::remove_cvref_t< T > >;


template< typename T >
concept KeyType = std::unsigned_integral< T > || StructKey< T >;

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_max_depth( unsigned int component_bits ) {
  return
    ( component_bits >> 1 ) + ( component_bits & 0x1u );
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_depth_bits( unsigned int component_bits ) {
  auto max = get_max_depth< T, dims >( component_bits );
  return sizeof( max ) * 8u - std::countl_zero( max );
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_component_bits( unsigned int current_component_bits ) {
  auto required_size = get_depth_bits< T, dims >( current_component_bits ) + current_component_bits * dims;
  if( required_size > sizeof( T ) * 8u ) {
    if( current_component_bits == 2u ) {
      return 0u;
    }
    return get_component_bits< T, dims >( current_component_bits - 1u );
  }
  return current_component_bits;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_component_bits() {
  return get_component_bits< T, dims >( sizeof( T ) * 8u / dims );
}

template< StructKey T, unsigned int dims >
constexpr auto get_component_bits() {
  return sizeof( std::uint64_t ) * 8u - 1u;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_component_offset( unsigned int i ) {
  return get_component_bits< T, dims >() * i;
}

template< StructKey T, unsigned int dims >
constexpr auto get_component_offset( unsigned int ) {
  return 0u;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_max_depth() {
  auto component_bits = get_component_bits< T, dims >();
  return
    ( component_bits >> 1 ) + ( component_bits & 0x1u );
}

template< StructKey T, unsigned int dims >
constexpr auto get_max_depth() {
  return 31u;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_depth_bits() {
  auto max = get_max_depth< T, dims >();
  return sizeof( max ) * 8u - std::countl_zero( max );
}

template< StructKey T, unsigned int dims >
constexpr auto get_depth_bits() {
  return 5u;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_depth_offset() {
  return get_component_offset< T, dims >( dims );
}

template< StructKey T, unsigned int dims >
constexpr auto get_depth_offset() {
  return 0u;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_component_mask() {
  return ( T( 1u ) << ( get_component_bits< T, dims >() ) ) - 1u;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_depth_mask() {
  return ( T( 1u ) << ( get_depth_bits< T, dims >() ) ) - 1u;
}

template< unsigned int dims >
constexpr auto get_camap_offset() {
  return 0u;
}

template< unsigned int dims >
constexpr auto get_camap_mask() {
  if constexpr ( camap_traits< dims >::map_bits == ( sizeof( typename camap_traits< dims >::type ) * 8u ) ) {
    return typename camap_traits< dims >::type( -1 );
  }
  else {
    return ( typename camap_traits< dims >::type( 1u ) << ( camap_traits< dims >::map_bits ) ) - 1u;
  }
}

template< unsigned int dims >
constexpr auto get_count_offset() {
  return camap_traits< dims >::map_bits;
}

template< unsigned int dims >
constexpr auto get_count_mask() {
  return ( typename camap_traits< dims >::type( 1u ) << ( camap_traits< dims >::count_bits ) ) - 1u;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_depth( T v ) {
  return ( v >> get_depth_offset< T, dims >() ) & get_depth_mask< T, dims >();
}

template< StructKey T, unsigned int dims >
constexpr auto get_depth( T v ) {
  return v.depth;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr void set_depth( T &dest, unsigned int v ) {
  dest = ( v & get_depth_mask< T, dims >() ) << get_depth_offset< T, dims >();
}

template< StructKey T, unsigned int dims >
constexpr void set_depth( T &dest, unsigned int v ) {
  dest.depth = v;
}





template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_component( unsigned int i, T v ) {
  return ( ( v >> get_component_offset< T, dims >( i ) ) & get_component_mask< T, dims >() ) << ( get_depth< T, dims >( v ) * 2u );
}

template< StructKey T, unsigned int dims >
constexpr auto get_component( unsigned int i, const T &v ) {
  return v.position[ i ] << ( get_depth< T, dims >( v ) * 2u );
}

template< std::unsigned_integral T, unsigned int dims >
constexpr void set_component( unsigned int i, T &v, T new_value ) {
  v &= ~( get_component_mask< T, dims >() << get_component_offset< T, dims >( i ) );
  v |= ( ( new_value >> ( get_depth< T, dims >( v ) * 2u ) ) & get_component_mask< T, dims >() ) << get_component_offset< T, dims >( i );
}

template< StructKey T, unsigned int dims >
constexpr void set_component( unsigned int i, const T &v, std::uint64_t new_value ) {
  v.position[ i ] = new_value >> ( get_depth< T, dims >( v ) * 2u );
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_raw_component( unsigned int i, T v ) {
  return ( v >> get_component_offset< T, dims >( i ) ) & get_component_mask< T, dims >();
}

template< StructKey T, unsigned int dims >
constexpr auto get_raw_component( unsigned int i, T v ) {
  return v.position[ i ];
}

template< std::unsigned_integral T, unsigned int dims >
constexpr void set_raw_component( unsigned int i, T &v, T new_value ) {
  v &= ~( get_component_mask< T, dims >() << get_component_offset< T, dims >( i ) );
  v |= ( new_value & get_component_mask< T, dims >() ) << get_component_offset< T, dims >( i );
}

template< StructKey T, unsigned int dims >
constexpr void set_raw_component( unsigned int i, const T &v, std::uint64_t new_value ) {
  v.position[ i ] = new_value;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_component_max() {
  return ( T( 1u ) << get_component_bits< T, dims >() ) - 1u;
}

template< StructKey T, unsigned int dims >
constexpr auto get_component_max() {
  return ( std::uint64_t( 1u ) << 63u ) - 1u;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_width( T v ) {
  return T( 1u ) << ( get_depth< T, dims >( v ) * 2u );
}

template< StructKey T, unsigned int dims >
constexpr auto get_width( T v ) {
  return std::uint64_t( 1u ) << ( get_depth< T, dims >( v ) * 2u );
}

template< unsigned int dims >
constexpr typename camap_traits< dims >::type get_camap( typename camap_traits< dims >::type v ) {
  return ( v >> get_camap_offset< dims >() ) & get_camap_mask< dims >();
}

template< unsigned int dims >
constexpr void set_camap( typename camap_traits< dims >::type &l, typename camap_traits< dims >::type r ) {
  l &= ~( get_camap_mask< dims >() << get_camap_offset< dims >() );
  l |= r << get_camap_offset< dims >();
}


template< unsigned int dims >
constexpr auto get_count( typename camap_traits< dims >::type v ) {
  return ( v >> get_count_offset< dims >() ) & get_count_mask< dims >();
}

template< unsigned int dims >
constexpr void get_count( typename camap_traits< dims >::type &l, typename camap_traits< dims >::type r ) {
  l &= ~( get_count_mask< dims >() << get_count_offset< dims >() );
  l |= r << get_count_offset< dims >();
}

template< unsigned int dims >
constexpr void increment_count( typename camap_traits< dims >::type v ) {
  set_count< dims >( v, get_count< dims >( v ) + 1u );
}

template< unsigned int dims >
constexpr void decrement_count( typename camap_traits< dims >::type v ) {
  set_count< dims >( v, get_count< dims >( v ) - 1u );
}

struct out_of_range {};

template< KeyType T, unsigned int dims >
constexpr void to_key_internal( T&, unsigned int, unsigned int ) {}

template< std::unsigned_integral T, unsigned int dims, typename Head, typename ...Tail >
constexpr void to_key_internal( T &dest, unsigned int depth, unsigned int i, Head head, Tail... tail ) {
  if( !std::is_constant_evaluated() ) {
    if( head > get_component_max< T, dims >() ) {
      throw std::out_of_range( "to_key: The component value is higher than the key format can encode." );
    }
  }
  dest |= ( ( head >> ( depth * 2u ) ) & get_component_mask< T, dims >() ) << get_component_offset< T, dims >( i );
  to_key_internal< T, dims >( dest, depth, i + 1u, tail... );
}

template< StructKey T, unsigned int dims, typename Head, typename ...Tail >
constexpr void to_key_internal( T &dest, unsigned int depth, unsigned int i, Head head, Tail... tail ) {
  if( !std::is_constant_evaluated() ) {
    if( head > get_component_max< T, dims >() ) {
      throw std::out_of_range( "to_key: The component value is higher than the key format can encode." );
    }
  }
  dest[ i ] = head;
  to_key_internal< T, dims >( dest, depth, i + 1u, tail... );
}

template< std::unsigned_integral T, unsigned int dims, typename ...I >
constexpr auto to_key( unsigned int depth, I ... p ) -> std::enable_if_t< sizeof...( I ) <= dims, T > {
  T temp = ( depth & get_depth_mask< T, dims >() ) << get_depth_offset< T, dims >();
  to_key_internal< T, dims >( temp, depth, 0u, p... );
  return temp;
}

template< StructKey T, unsigned int dims, typename ...I >
constexpr auto to_key( unsigned int depth, I ... p ) -> std::enable_if_t< sizeof...( I ) <= dims, T > {
  T temp;
  temp.depth = depth;
  to_key_internal< T, dims >( temp, depth, 0u, p... );
  return temp;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto add_key( T left, T right ) {
  T temp;
  set_depth< T, dims >( temp, 0u );
  for( unsigned int i = 0u; i != dims; ++i ) {
    set_component< T, dims >( i, temp, get_component< T, dims >( i, left ) + get_component< T, dims >( i, right ) );
  }
  return temp;
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto sub_key( T left, T right ) {
  T temp;
  set_depth< T, dims >( temp, 0u );
  for( unsigned int i = 0u; i != dims; ++i ) {
    set_component< T, dims >( i, temp, get_component< T, dims >( i, left ) - get_component< T, dims >( i, right ) );
  }
  return temp;
}

template< unsigned int dims >
constexpr void to_axis_camap( typename camap_traits< dims >::type&, unsigned int, unsigned int ) {}

template< unsigned int dims, typename Head, typename ...Tail >
constexpr void to_axis_camap( typename camap_traits< dims >::type &dest, unsigned int child_depth, unsigned int i, Head head, Tail... tail ) {
  dest |= ( ( head >> ( child_depth * 2u ) ) & 0x03 ) << ( i * 2u );
  to_axis_camap< dims >( dest, child_depth, i + 1u, tail... );
}

template< unsigned int dims, typename ...I >
constexpr auto to_camap( unsigned int depth, I ... p ) -> std::enable_if_t< ( camap_traits< dims >::mode == camode_t::AXIS ) && ( sizeof...( I ) <= dims ), typename camap_traits< dims >::type > {
  typename camap_traits< dims >::type temp = 0u;
  to_axis_camap< dims >( temp, depth - 1u, 0u, p... );
  return temp;
}

template< unsigned int dims, typename ...I >
constexpr auto to_camap( unsigned int depth, I ... p ) -> std::enable_if_t< ( camap_traits< dims >::mode == camode_t::FULL ) && ( sizeof...( I ) <= dims ), typename camap_traits< dims >::type > {
  typename camap_traits< dims >::type temp = 0u;
  to_axis_camap< dims >( temp, depth - 1u, 0u, p... );
  return typename camap_traits< dims >::type( 1u ) << temp;
}

template< KeyType T, unsigned int dims >
constexpr auto key_to_child_index( const T &key ) {
  unsigned int index = 0u;
  for( unsigned int i = 0u; i != dims; ++i ) {
    auto component = get_raw_component< T, dims >( i, key );
    index |= ( component & 0x3u ) << ( i * 2u );
  }
  return index;
}

template< KeyType T, unsigned int dims >  
constexpr auto child_index_to_key( T key, unsigned int index ) {
  for( unsigned int i = 0u; i != dims; ++i  ) {
    auto component = get_raw_component< T, dims >( i, key );
    component &= ~0x3u;
    component |= index & 0x3u;
    set_raw_component< T, dims >( i, key, component );
    index >>= 2u;
  }
  return key;
}

template< KeyType T, unsigned int dims >
constexpr auto key_to_camap( const T &key ) -> std::enable_if_t< ( camap_traits< dims >::mode == camode_t::FULL ), typename camap_traits< dims >::type > {
  return typename camap_traits< dims >::type( 1u ) << key_to_child_index< T, dims >( key );
}

template< KeyType T, unsigned int dims >
constexpr auto key_to_camap( const T &key ) -> std::enable_if_t< ( camap_traits< dims >::mode == camode_t::AXIS ), typename camap_traits< dims >::type > {
  typename camap_traits< dims >::type temp = 0u;
  for( unsigned int i = 0u; i != dims; ++i ) {
    temp |= ( get_raw_component< T, dims >( i, key ) & 0x3u ) << ( i * 4u );
  }
  return temp;
}

template< unsigned int dims >
constexpr auto get_max_child_count() {
  return 1u << ( 2u * dims );
}

template< KeyType T, unsigned int dims >
constexpr auto next_key( const T &key, unsigned int step ) {
  const auto index = key_to_child_index< T, dims >( key );
  return child_index_to_key< T, dims >( key, std::min( std::max( index + step, 0u ), get_max_child_count< dims >() + 1u ) );
}

template< KeyType T, unsigned int dims >
constexpr auto prev_key( const T &key, unsigned int step ) {
  const auto index = key_to_child_index< T, dims >( key );
  return child_index_to_key< T, dims >( key, std::min( std::max( index - step, 0u ), get_max_child_count< dims >() - 1u ) );
}

template< unsigned int dims >
constexpr auto combine_camap_and_count( typename camap_traits< dims >::type camap, typename camap_traits< dims >::type count ) {
  return
    ( ( count & get_count_mask< dims >() ) << get_count_offset< dims >() ) |
    ( count & get_camap_mask< dims >() );
}

template< KeyType T, unsigned int dims >
constexpr auto get_key_in_depth( T new_depth, T v );

template< KeyType T, unsigned int dims >
constexpr auto contains( const T &left, const T &key ) {
  return get_depth< T, dims >( left ) >= get_depth< T, dims >( key ) && left == get_key_in_depth< T, dims >( get_depth< T, dims >( left ), key );
}

template< KeyType T, unsigned int dims, unsigned int i >
constexpr auto get_key_in_upper_depth_internal( T&, unsigned int, T ) -> std::enable_if_t< i == dims > {}

template< KeyType T, unsigned int dims, unsigned int i >
constexpr auto get_key_in_upper_depth_internal( T &dest, unsigned int depth_diff, T v ) -> std::enable_if_t< i != dims > {
  set_component< T, dims >( i, dest, get_component< T, dims >( i, v ) );
  get_key_in_upper_depth_internal< T, dims, i + 1u >( dest, depth_diff, v );
}

template< KeyType T, unsigned int dims, unsigned int i >
constexpr auto get_key_in_lower_depth_internal( T&, unsigned int, T ) -> std::enable_if_t< i == dims > {}

template< KeyType T, unsigned int dims, unsigned int i >
constexpr auto get_key_in_lower_depth_internal( T &dest, unsigned int depth_diff, T v ) -> std::enable_if_t< i != dims > {
  set_component< T, dims >( i, dest, get_component< T, dims >( i, v ) );
  get_key_in_lower_depth_internal< T, dims, i + 1u >( dest, depth_diff, v );
}

template< KeyType T, unsigned int dims >
constexpr auto get_key_in_depth( T new_depth, T v ) {
  const auto current_depth = get_depth< T, dims >( v );
  if( new_depth > current_depth ) {
    const auto depth_diff = new_depth - current_depth;
    T dest;
    set_depth< T, dims >( dest, new_depth );
    get_key_in_upper_depth_internal< T, dims, 0u >( dest, depth_diff, v );
    return dest;
  }
  if( new_depth < current_depth ) {
    const auto depth_diff = current_depth - new_depth;
    T dest;
    set_depth< T, dims >( dest, new_depth );
    get_key_in_lower_depth_internal< T, dims, 0u >( dest, depth_diff, v );
    return dest;
  }
  return v;
}

template< KeyType T, unsigned int dims >
constexpr auto upper_depth( T v ) {
  return get_key_in_depth< T, dims >( get_depth< T, dims >( v ) + 1u, v );
}

template< KeyType T, unsigned int dims >
constexpr auto get_key_containing_both( T left, T right ) {
  T prev_left = left;
  T prev_right = right;
  if( get_depth< T, dims >( left ) < get_depth< T, dims >( right ) ) {
    left = get_key_in_depth< T, dims >( get_depth< T, dims >( right ), left );
  }
  if( get_depth< T, dims >( left ) > get_depth< T, dims >( right ) ) {
    right = get_key_in_depth< T, dims >( get_depth< T, dims >( left ), right );
  }
  while( left != right ) {
    prev_left = left;
    prev_right = right;
    left = upper_depth< T, dims >( left );
    right = upper_depth< T, dims >( right );
  }
  return std::make_tuple( left, prev_left, prev_right );
}

template< KeyType T, unsigned int dims >
constexpr auto get_root_key() {
  return to_key< T, dims >( get_max_depth< T, dims >() );
}

template< KeyType T, unsigned int dims, typename Enable = void >
struct child_availability_map {};

template< KeyType T, unsigned int dims >
struct child_availability_map< T, dims, std::enable_if_t< ( camap_traits< dims >::mode == camode_t::AXIS ) > > {
  typename camap_traits< dims >::type value = 0u;
  bool has_child( unsigned int depth, T key ) const {
    if( ( depth <= get_depth< T, dims >( key ) ) ) return false;
    if( depth == 0u ) return false;
    const auto child_key = get_key_in_depth< T, dims >( depth - 1u, key );
    const auto existing = get_camap< dims >( value );
    const auto new_child = key_to_camap< T, dims >( child_key );
    return ( existing & new_child ) == new_child;
  }
  template< typename ...I >
  void set_child( unsigned int depth, T key ) {
    assert( ( depth > get_depth< T, dims >( key ) ) );
    assert( depth != 0u );
    const auto child_key = get_key_in_depth< T, dims >( depth - 1u, key );
    const auto existing = get_camap< dims >( value );
    set_camap< dims >( value, existing | key_to_camap< T, dims >( child_key ) );
    increment_count< dims >( value );
  }
  template< typename ...I >
  void remove_child( unsigned int depth, T key ) {
    assert( ( depth > get_depth< T, dims >( key ) ) );
    assert( depth != 0u );
    decrement_count< dims >( value );
  }
  typename camap_traits< dims >::type get_count() const {
    return get_count< dims >( value );
  }
};

template< KeyType T, unsigned int dims >
struct child_availability_map< T, dims, std::enable_if_t< ( camap_traits< dims >::mode == camode_t::FULL ) > > {
  typename camap_traits< dims >::type value = 0u;
  bool has_child( unsigned int depth, T key ) const {
    assert( ( depth > get_depth< T, dims >( key ) ) );
    assert( depth != 0u );
    const auto child_key = get_key_in_depth< T, dims >( depth - 1u, key );
    const auto existing = get_camap< dims >( value );
    const auto new_child = key_to_camap< T, dims >( child_key );
    return ( existing & new_child ) == new_child;
  }
  void set_child( unsigned int depth, T key ) {
    assert( ( depth > get_depth< T, dims >( key ) ) );
    assert( depth != 0u );
    auto child_key = get_key_in_depth< T, dims >( depth - 1u, key );
    auto existing = get_camap< dims >( value );
    set_camap< dims >( value, existing | key_to_camap< T, dims >( child_key ) );
  }
  void remove_child( unsigned int depth, T key ) {
    assert( ( depth > get_depth< T, dims >( key ) ) );
    assert( depth != 0u );
    const auto child_key = get_key_in_depth< T, dims >( depth - 1u, key );
    const auto existing = get_camap< dims >( value );
    set_camap< dims >( value, existing ^ key_to_camap< T, dims >( child_key ) );
  }
  typename camap_traits< dims >::type get_count() const {
    return std::popcount( get_camap< dims >( value ) );
  }
};

template< KeyType T, typename U, unsigned int dims >
struct node {
  template< typename V >
  node(
    const T &range_,
    V &&data_
  ) :
    range( range_ ),
    data( std::move( data_ ) ) {}
  bool is_leaf() const {
    return data.index() == 1u;
  }
  const U &get_data() const {
    assert( is_leaf() );
    return std::get< U >( data );
  }
  U &&move_data() {
    assert( is_leaf() );
    return std::move( std::get< U >( data ) );
  }
  const child_availability_map< T, dims > &get_child_availability_map() const {
    assert( !is_leaf() );
    return std::get< child_availability_map< T, dims > >( data );
  }
  const U &operator*() const {
    assert( is_leaf() );
    return std::get< U >( data );
  }
  bool has_child( const T &key ) const {
    assert( !is_leaf() );
    const auto &camap_and_count = std::get< child_availability_map< T, dims > >( data );
    return camap_and_count.has_child( get_depth< T, dims >( range ), key );
  }
  void set_child( const T &key ) {
    assert( !is_leaf() );
    auto &camap_and_count = std::get< child_availability_map< T, dims > >( data );
    camap_and_count.set_child( get_depth< T, dims >( range ), key );
  }
  void remove_child( const T &key ) {
    assert( !is_leaf() );
    auto &camap_and_count = std::get< child_availability_map< T, dims > >( data );
    camap_and_count.remove_child( get_depth< T, dims >( range ), key );
  }
  void set_data( U &&data_ ) {
    data = std::move( data_ );
  }
  void set_data( const U &data_ ) {
    data = data_;
  }
  void clear_child() {
    data = child_availability_map< T, dims >{};
  }
  typename camap_traits< dims >::type get_count() const {
    assert( !is_leaf() );
    auto &camap_and_count = std::get< child_availability_map< T, dims > >( data );
    return camap_and_count.get_count();
  }
  const T &get_range() const {
    return range;
  }
private:
  T range = get_root_key< T, dims >();
  std::variant< child_availability_map< T, dims >, U > data = child_availability_map< T, dims >();
};

template< typename T >
struct extract_dims {};
template< typename K, typename T, typename U, unsigned int dims >
struct extract_dims< std::pair< K, node< T, U, dims > > > {
  constexpr static auto value = dims;
};
template< typename T, typename U, unsigned int dims >
struct extract_dims< node< T, U, dims > > {
  constexpr static auto value = dims;
};
template< typename T, unsigned int dims >
struct extract_dims< child_availability_map< T, dims > > {
  constexpr static auto value = dims;
};
template< typename T, unsigned int dims >
struct extract_dims< rectangle< T, dims > > {
  constexpr static auto value = dims;
};
template< std::ranges::range R >
struct extract_dims< R > :
  public extract_dims< std::remove_cvref_t< decltype( std::ranges::begin( std::declval< R& >() )->second ) > > {};
template< typename T >
constexpr auto extract_dims_v = extract_dims< T >::value;

template< typename T >
struct extract_key_type {};
template< typename K, typename T, typename U, unsigned int dims >
struct extract_key_type< std::pair< K, node< T, U, dims > > > {
  using type = T;
};
template< typename T, typename U, unsigned int dims >
struct extract_key_type< node< T, U, dims > > {
  using type = T;
};
template< typename T, unsigned int dims >
struct extract_key_type< child_availability_map< T, dims > > {
  using type = T;
};
template< typename T, unsigned int dims >
struct extract_key_type< rectangle< T, dims > > {
  using type = T;
};
template< std::ranges::range R >
struct extract_key_type< R > :
  public extract_key_type< std::remove_cvref_t< decltype( std::ranges::begin( std::declval< R& >() )->second ) > > {};
template< typename T >
using extract_key_type_t = typename extract_key_type< T >::type;

template< typename T >
struct extract_value_type {};
template< typename K, typename T, typename U, unsigned int dims >
struct extract_value_type< std::pair< K, node< T, U, dims > > > {
  using type = U;
};
template< typename T, typename U, unsigned int dims >
struct extract_value_type< node< T, U, dims > > {
  using type = U;
};
template< std::ranges::range R >
struct extract_value_type< R > :
  public extract_value_type< std::remove_cvref_t< decltype( std::ranges::begin( std::declval< R& >() )->second ) > > {};
template< typename T >
using extract_value_type_t = typename extract_value_type< T >::type;

template< typename T > 
struct extract_node_type {
  using type = node<
    extract_key_type_t< T >,
    extract_value_type_t< T >,
    extract_dims_v< T >
  >;
};
template< typename T >
using extract_node_type_t = typename extract_node_type< T >::type;

template< typename T >
struct larger_rectangle {};
template< unsigned int dims >
struct larger_rectangle< rectangle< std::uint8_t, dims > > {
  using type = rectangle< std::uint16_t, dims >;
};
template< unsigned int dims >
struct larger_rectangle< rectangle< std::uint16_t, dims > > {
  using type = rectangle< std::uint32_t, dims >;
};
template< unsigned int dims >
struct larger_rectangle< rectangle< std::uint32_t, dims > > {
  using type = rectangle< std::uint64_t, dims >;
};
template< unsigned int dims >
struct larger_rectangle< rectangle< std::uint64_t, dims > > {
  using type = rectangle< struct_key< dims >, dims >;
};

template< typename T >
using larger_rectangle_t = typename larger_rectangle< T >::type;

template< KeyType T, unsigned int dims >
auto get_max_leaf_count(
  const rectangle< T, dims > &rect
) {
  T area = 1u;
  for( unsigned int i = 0u; i != dims; ++i )
    area *= get_component< T, dims >( i, rect.right_bottom ) - get_component< T, dims >( i, rect.left_top );
  return area;
}

template< typename T >
struct is_node : public std::false_type {};
template< typename T, typename U, unsigned int dims >
struct is_node< node< T, U, dims > > : public std::true_type {};
template< typename T >
constexpr auto is_node_v = is_node< T >::value;

template< typename T >
concept Node = detail::is_node_v< std::remove_cvref_t< T > >;

template< unsigned int dims, unsigned int i, KeyType T, std::unsigned_integral U >
void key_cast_internal( T &converted, const U &v ) {
  if constexpr ( dims != i ) {
    set_component< T, dims >( i, converted, get_component< U, dims >( i, v ) );
    key_cast_internal< dims, i + 1u >( converted, v );
  }
}

}

template< detail::KeyType T, unsigned int dims, detail::KeyType U >
T key_cast( const U &v ) {
  T converted = detail::to_key< T, dims >( detail::get_depth< U, dims >( v ) );
  detail::key_cast_internal< dims, 0u >( converted, v );
  return converted;
}


template< typename T, detail::KeyType U, unsigned int dims >
T rectangle_cast( const rectangle< U, dims > &v )
  requires std::unsigned_integral< detail::extract_key_type_t< T > > {
  static_assert( detail::extract_dims_v< T > == dims );
  T converted;
  return T{
    key_cast< detail::extract_key_type_t< T >, dims >( v.left_top ),
    key_cast< detail::extract_key_type_t< T >, dims >( v.right_bottom )
  };
}

template< typename Container >
concept HDMapUnderlyingContainer = requires(
  Container &c,
  std::remove_cvref_t< decltype( std::declval< Container& >().begin()->second ) > &&v,
  std::remove_cvref_t< decltype( std::declval< Container& >().begin()->first ) > p
) {
  { c.find( p )->second } -> std::same_as<
    std::remove_cvref_t< decltype( std::declval< Container& >().begin()->second ) >&
  >;
  { c.find( p )->second } -> detail::Node; 
  { c.insert( std::make_pair( p, v ) ).second } -> std::same_as< bool&& >;
  c.erase( p );
  c.erase( c.begin() );
  c.end();
};

namespace detail {

template< KeyType T, unsigned int dims >
using depth_log_t =
  boost::container::static_vector< unsigned int, get_max_depth< T, dims >() >;

template< HDMapUnderlyingContainer C >
auto find_nearest(
  C &map,
  extract_key_type_t< C > root,
  extract_key_type_t< C > key,
  depth_log_t< extract_key_type_t< C >, extract_dims_v< C > > &log
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  if( map.empty() ) return map.end();
  const auto existing_node = map.find( root );
  if( existing_node == map.end() ) return map.end();
  if( existing_node->second.is_leaf() ) return existing_node;
  const auto current_depth = get_depth< T, dims >( existing_node->second.get_range() );
  const auto key_depth = get_depth< T, dims >( key );
  if( current_depth <= key_depth && get_key_in_depth< T, dims >( key_depth, existing_node->second.get_range() ) == key ) return existing_node;
  assert( current_depth != 0u );
  const auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, key );
  if( !existing_node->second.has_child( child_key ) ) return existing_node;
  log.push_back( get_depth< T, dims >( root ) );
  auto lower = find_nearest( map, child_key, key, log );
  if( lower == map.end() ) return existing_node;
  return lower;
}

template< HDMapUnderlyingContainer C >
auto find_nearest(
  const C &map,
  extract_key_type_t< C > root,
  extract_key_type_t< C > key,
  depth_log_t< extract_key_type_t< C >, extract_dims_v< C > > &log
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  if( map.empty() ) return map.end();
  const auto existing_node = map.find( root );
  if( existing_node == map.end() ) return map.end();
  if( existing_node->second.is_leaf() ) return existing_node;
  const auto current_depth = get_depth< T, dims >( existing_node->second.get_range() );
  const auto key_depth = get_depth< T, dims >( key );
  if( current_depth <= key_depth && get_key_in_depth< T, dims >( key_depth, existing_node->second.get_range() ) == key ) return existing_node;
  assert( current_depth != 0u );
  const auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, key );
  if( !existing_node->second.has_child( child_key ) ) return existing_node;
  log.push_back( get_depth< T, dims >( root ) );
  auto lower = find_nearest( map, child_key, key, log );
  if( lower == map.end() ) return existing_node;
  return lower;
}

template< HDMapUnderlyingContainer C >
auto find_nearest(
  C &map,
  extract_key_type_t< C > root,
  extract_key_type_t< C > key
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  if( map.empty() ) return map.end();
  const auto existing_node = map.find( root );
  if( existing_node == map.end() ) return map.end();
  if( existing_node->second.is_leaf() ) return existing_node;
  const auto current_depth = get_depth< T, dims >( existing_node->second.get_range() );
  const auto key_depth = get_depth< T, dims >( key );
  if( current_depth <= key_depth && get_key_in_depth< T, dims >( key_depth, existing_node->second.get_range() ) == key ) return existing_node;
  assert( current_depth != 0u );
  const auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, key );
  if( !existing_node->second.has_child( child_key ) ) return existing_node;
  auto lower = find_nearest( map, child_key, key );
  if( lower == map.end() ) return existing_node;
  return lower;
}

template< HDMapUnderlyingContainer C >
auto find_nearest(
  const C &map,
  extract_key_type_t< C > root,
  extract_key_type_t< C > key
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  if( map.empty() ) return map.end();
  const auto existing_node = map.find( root );
  if( existing_node == map.end() ) return map.end();
  if( existing_node->second.is_leaf() ) return existing_node;
  const auto current_depth = get_depth< T, dims >( existing_node->second.get_range() );
  const auto key_depth = get_depth< T, dims >( key );
  if( current_depth <= key_depth && get_key_in_depth< T, dims >( key_depth, existing_node->second.get_range() ) == key ) return existing_node;
  assert( current_depth != 0u );
  const auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, key );
  if( !existing_node->second.has_child( child_key ) ) return existing_node;
  auto lower = find_nearest( map, child_key, key );
  if( lower == map.end() ) return existing_node;
  return lower;
}

template< HDMapUnderlyingContainer C >
auto get_child_key(
  const C&,
  const extract_node_type_t< C > &current_node,
  unsigned int begin = 0u
) -> std::enable_if_t< ( camap_traits< extract_dims_v< C > >::mode == camode_t::FULL ), std::pair< extract_key_type_t< C >, unsigned int > > {
  using T = extract_key_type_t< C >;
  using camap_type = typename camap_traits< extract_dims_v< C > >::type;
  constexpr auto dims = extract_dims_v< C >;
  if constexpr ( is_struct_key_v< T > ) {
    if( current_node.is_leaf() ) return std::make_pair( T{}, get_max_child_count< dims >() );
    if( begin >= get_max_child_count< dims >() ) return std::make_pair( T{}, get_max_child_count< dims >() );
  }
  else {
    if( current_node.is_leaf() ) return std::make_pair( T( 0 ), get_max_child_count< dims >() );
    if( begin >= get_max_child_count< dims >() ) return std::make_pair( T( 0 ), get_max_child_count< dims >() );
  }
  const auto &camap = current_node.get_child_availability_map();

  const unsigned int child_index = std::countr_zero( camap_type( camap.value >> begin ) ) + begin;
  if constexpr ( is_struct_key_v< T > ) {
    if( child_index >= get_max_child_count< dims >() ) return std::make_pair( T{}, get_max_child_count< dims >() );
  }
  else {
    if( child_index >= get_max_child_count< dims >() ) return std::make_pair( T( 0 ), get_max_child_count< dims >() );
  }
  assert( child_index < camap_traits< dims >::map_bits );
  const auto current_depth = get_depth< T, dims >( current_node.get_range() );
  assert( current_depth > 0u );
  const auto lower_key = get_key_in_depth< T, dims >( current_depth - 1u, current_node.get_range() );
  const auto child_key = child_index_to_key< T, dims >( lower_key, child_index );
  return std::make_pair( child_key, child_index );
}

template< HDMapUnderlyingContainer C >
auto get_child_key(
  const C&,
  const extract_node_type_t< C > &current_node,
  unsigned int begin = 0u
) -> std::enable_if_t< ( camap_traits< extract_dims_v< C > >::mode == camode_t::AXIS ), std::pair< extract_key_type_t< C >, unsigned int > > {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  if constexpr ( is_struct_key_v< T > ) {
  }
  else {
    if( current_node.is_leaf() ) return std::make_pair( T( 0 ), get_max_child_count< dims >() );
    if( begin >= get_max_child_count< dims >() ) return std::make_pair( T( 0 ), get_max_child_count< dims >() );
  }
  const auto current_depth = get_depth< T, dims >( current_node->second.get_range() );
  assert( current_depth > 0u );
  auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, current_node.get_range() );
  child_key = next_key< T, dims >( child_key, begin );
  const auto camap = current_node.get_child_availability_map();
  for( unsigned int i = begin; i != get_max_child_count< dims >(); ++i ) {
    if( current_node.has_child( child_key ) ) {
      return std::make_pair( child_key, i );
    }
    child_key = next_key< T, dims >( child_key, 1u );
  }
  if constexpr ( is_struct_key_v< T > ) {
    return std::make_pair( T{}, get_max_child_count< dims >() );
  }
  else {
    return std::make_pair( T( 0 ), get_max_child_count< dims >() );
  }
}

template< HDMapUnderlyingContainer C >
auto get_child_key(
  const C &map,
  const extract_key_type_t< C > &current_key,
  unsigned int begin = 0u
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  const auto node = map.find( current_key );
  if constexpr ( is_struct_key_v< T > ) {
    if( node == map.end() ) return std::make_pair( T{}, get_max_child_count< dims >() );
  }
  else {
    if( node == map.end() ) return std::make_pair( T( 0 ), get_max_child_count< dims >() );
  }
  return get_child_key( map, node->second, begin );
}

template< HDMapUnderlyingContainer C >
auto get_child(
  C &map,
  const extract_node_type_t< C > &current_node,
  unsigned int begin = 0u
) {
  constexpr auto dims = extract_dims_v< C >;
  const auto [child_key,child_index] = get_child_key( map, current_node, begin );
  if( child_index == get_max_child_count< dims >() ) return std::make_pair( map.end(), get_max_child_count< dims >() );
  const auto child_node = map.find( child_key );
  if( child_node == map.end() ) return std::make_pair( map.end(), get_max_child_count< dims >() );
  return std::make_pair( child_node, child_index );
}

template< HDMapUnderlyingContainer C >
auto get_child(
  const C &map,
  const extract_node_type_t< C > &current_node,
  unsigned int begin = 0u
) {
  constexpr auto dims = extract_dims_v< C >;
  const auto [child_key,child_index] = get_child_key( map, current_node, begin );
  if( child_index == get_max_child_count< dims >() ) return std::make_pair( map.end(), get_max_child_count< dims >() );
  const auto child_node = map.find( child_key );
  if( child_node == map.end() ) return std::make_pair( map.end(), get_max_child_count< dims >() );
  return std::make_pair( child_node, child_index );
}

template< HDMapUnderlyingContainer C >
auto get_child(
  C &map,
  const extract_key_type_t< C > &current_key,
  unsigned int begin = 0u
) {
  constexpr auto dims = extract_dims_v< C >;
  const auto node = map.find( current_key );
  if( node == map.end() ) return std::make_pair( map.end(), get_max_child_count< dims >() );
  return get_child( map, node->second, begin );
}

template< HDMapUnderlyingContainer C >
auto get_child(
  const C &map,
  const extract_key_type_t< C > &current_key,
  unsigned int begin = 0u
) {
  constexpr auto dims = extract_dims_v< C >;
  const auto node = map.find( current_key );
  if( node == map.end() ) return std::make_pair( map.end(), get_max_child_count< dims >() );
  return get_child( map, node->second, begin );
}

template< HDMapUnderlyingContainer C, typename F >
auto for_each_child(
  C &map,
  extract_key_type_t< C > key,
  F &&func
) {
  for(
    auto [child_node,child_index] = get_child( map, key );
    child_node != map.end();
    std::tie( child_node, child_index ) = get_child( map, key, child_index + 1u )
  ) {
    if( !func( child_node, child_index ) ) break;
  }
}

template< HDMapUnderlyingContainer C, typename F >
auto for_each_child(
  const C &map,
  extract_key_type_t< C > key,
  F &&func
) {
  for(
    auto [child_node,child_index] = get_child( map, key );
    child_node != map.end();
    std::tie( child_node, child_index ) = get_child( map, key, child_index + 1u )
  ) {
    if( !func( child_node, child_index ) ) break;
  }
}

template< HDMapUnderlyingContainer C, typename F >
auto for_each_child(
  C &map,
  const extract_node_type_t< C > &node,
  F &&func
) {
  for(
    auto [child_node,child_index] = get_child( map, node );
    child_node != map.end();
    std::tie( child_node, child_index ) = get_child( map, node, child_index + 1u )
  ) {
    if( !func( child_node, child_index ) ) break;
  }
}

template< HDMapUnderlyingContainer C, typename F >
auto for_each_child(
  const C &map,
  const extract_node_type_t< C > &node,
  F &&func
) {
  for(
    auto [child_node,child_index] = get_child( map, node );
    child_node != map.end();
    std::tie( child_node, child_index ) = get_child( map, node, child_index + 1u )
  ) {
    if( !func( child_node, child_index ) ) break;
  }
}

template< HDMapUnderlyingContainer C, typename F >
auto for_each_child_key(
  const C &map,
  const extract_node_type_t< C > &node,
  F &&func
) {
  constexpr auto dims = extract_dims_v< C >;
  for(
    auto [child_key,child_index] = get_child_key( map, node );
    child_index != get_max_child_count< dims >();
    std::tie( child_key, child_index ) = get_child_key( map, node, child_index + 1u )
  ) {
    if( !func( child_key, child_index ) ) break;
  }
}

template< HDMapUnderlyingContainer C, typename F >
auto for_each_child_key(
  const C &map,
  const extract_key_type_t< C > &key,
  F &&func
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  boost::container::static_vector< std::pair< T, T >, get_max_child_count< dims >() > child_indices;
  for(
    auto [child_key,child_index] = get_child_key( map, key );
    child_index != get_max_child_count< dims >();
    std::tie( child_key, child_index ) = get_child_key( map, key, child_index + 1u )
  ) {
    child_indices.push_back( std::make_pair( child_key, child_index ) );
  }
  for( const auto &[child_key, child_index]: child_indices ) {
    if( !func( child_key, child_index ) ) break;
  }
}



template< HDMapUnderlyingContainer C >
auto find(
  C &map,
  extract_key_type_t< C > key
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  auto nearest_node = find_nearest( map, get_root_key< T, dims >(), key );
  if( nearest_node == map.end() ) return map.end();
  if( nearest_node->second.is_leaf() ) {
    if( nearest_node->second.get_range() == key ) return nearest_node;
    if( nearest_node->second.get_range() == get_key_in_depth< T, dims >( get_depth< T, dims >( nearest_node->second.get_range() ), key ) ) return nearest_node;
    return map.end();
  }
  return map.end();
}

template< std::unsigned_integral T, unsigned int dims >
constexpr auto get_max_leaf_count(
  T key
) {
  return 1u << ( 2u * get_depth< T, dims >( key ) * dims );
}

template< HDMapUnderlyingContainer C, typename F >
constexpr auto get_leaf_count(
  const C &map,
  const extract_node_type_t< C > &node,
  F &&cond
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  unsigned int count = 0u;
  for_each_child(
    map,
    node,
    [&]( const auto &child_node, auto ) {
      if( !child_node->second.is_leaf() ) {
        count += get_leaf_count( map, child_node );
      }
      else if( cond( child_node->second.get_data() ) ) {
        count += get_max_leaf_count< T, dims >( child_node->second.get_range() );
      }
      return true;
    }
  );
  return count;
}


template< HDMapUnderlyingContainer C, typename F >
auto is_uniform_of(
  const C &map,
  const extract_node_type_t< C > &current_node,
  const extract_value_type_t< C > &value,
  const F &func
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  const unsigned int current_range = current_node.get_range();
  unsigned int count = 0u;
  for_each_child(
    map,
    current_node,
    [&]( const auto &child_node, auto ) {
      if( child_node->first != child_node->second.get_range() ) return false;
      if( !child_node->second.is_leaf() ) return false;
      if( !func( value, child_node->second.get_data() ) ) return false;
      count += get_max_leaf_count< T, dims >( child_node->second.get_range() );
      return true;
    }
  );
  return count == get_max_leaf_count< T, dims >( current_range );
}

template< HDMapUnderlyingContainer C, typename F >
auto is_uniform_of(
  const C &map,
  extract_key_type_t< C > key,
  const extract_value_type_t< C > &value,
  const F &func
) {
  if( map.empty() ) return false;
  auto current_node = map.find( key );
  assert( current_node != map.end() );
  return is_uniform_of( map, current_node->second, value, func );
}

template< KeyType T, unsigned int dims, unsigned int i >
auto get_overlap_count_internal(
  const rectangle< T, dims >&,
  const rectangle< T, dims >&
) -> std::enable_if_t< dims == i, T > {
  return 1u;
}

template< KeyType T, unsigned int dims, unsigned int i >
auto get_overlap_count_internal(
  const rectangle< T, dims > &l,
  const rectangle< T, dims > &r
) -> std::enable_if_t< dims != i, T > {
  const auto l_begin = get_component< T, dims >( i, l.left_top );
  const auto l_end = get_component< T, dims >( i, l.right_bottom );
  const auto r_begin = get_component< T, dims >( i, r.left_top );
  const auto r_end = get_component< T, dims >( i, r.right_bottom );
  if( l_begin <= r_begin ) {
    if( l_end <= r_begin ) return 0u;
    else if( r_end <= l_end ) {
      return ( r_end - r_begin ) * get_overlap_count_internal< T, dims, i + 1u >( l, r );
    }
    else {
      return ( l_end - r_begin ) * get_overlap_count_internal< T, dims, i + 1u >( l, r );
    }
  }
  else {
    if( r_end <= l_begin ) return 0u;
    else if( l_end <= r_end ) {
      return ( l_end - l_begin ) * get_overlap_count_internal< T, dims, i + 1u >( l, r );
    }
    else {
      return ( r_end - l_begin ) * get_overlap_count_internal< T, dims, i + 1u >( l, r );
    }
  }
}

template< KeyType T, unsigned int dims >
auto get_overlap_count(
  const rectangle< T, dims > &l,
  const rectangle< T, dims > &r
) {
  return get_overlap_count_internal< T, dims, 0u >( l, r );
}

template< typename T >
struct covering_rectangle {
  using type =
    larger_rectangle_t<
      rectangle<
        extract_key_type_t< T >,
        extract_dims_v< T >
      >
    >;
};
template< typename T >
using covering_rectangle_t = typename covering_rectangle< T >::type;

template< std::unsigned_integral T, unsigned int dims >
auto get_right_bottom(
  T key
);

template< std::ranges::range Range, typename F >
auto is_uniform_of(
  Range &&leaves,
  const covering_rectangle_t< Range > &range,
  const extract_value_type_t< Range > &value,
  unsigned int axis,
  const F &func
) {
  using rect_t = covering_rectangle_t< Range >;
  using T = extract_key_type_t< rect_t >;
  constexpr auto dims = extract_dims_v< rect_t >;
  const auto expected_count = get_overlap_count< T, dims >( range, range );
  T count = 0u;
  T min = 0u;
  T max = get_component_max< T, dims >();
  for( const auto &[key,leaf]: leaves ) {
    if constexpr ( is_struct_key_v< T > ) {
      if( !func( leaf.get_data(), value ) ) return std::make_tuple( false, T{}, T{} );
    }
    else {
      if( !func( leaf.get_data(), value ) ) return std::make_tuple( false, T( 0u ), T( 0u ) );
    }
    const auto extended_leaf_left_top = key_cast< T, dims >( leaf.get_range() );
    const auto extended_leaf_right_bottom = get_right_bottom< T, dims >( extended_leaf_left_top );
    const auto overlap = get_overlap_count( rect_t{ extended_leaf_left_top, extended_leaf_right_bottom }, range );
    if( overlap ) {
      count += overlap;
      min = std::max( min, get_component< T, dims >( axis, extended_leaf_left_top ) );
      max = std::min( max, get_component< T, dims >( axis, extended_leaf_right_bottom ) );
    }
  }
  if( expected_count == count ) return std::make_tuple( true, min, max );
  else {
    if constexpr ( is_struct_key_v< T > ) {
      return std::make_tuple( false, T{}, T{} );
    }
    else {
      return std::make_tuple( false, T( 0u ), T( 0u ) );
    }
  }
}


template< HDMapUnderlyingContainer C >
void erase_children(
  C &map,
  extract_key_type_t< C > key
) {
  for_each_child(
    map,
    key,
    [&]( auto &child_node, auto ) {
      map.erase( child_node );
      return true;
    }
  );
}


template< HDMapUnderlyingContainer C, typename F >
auto insert(
  C &map,
  extract_value_type_t< C > &&value,
  extract_key_type_t< C > key,
  const F &func
) {
  using T = extract_key_type_t< C >;
  using U = extract_value_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  constexpr auto root_key = get_root_key< T, dims >();
  depth_log_t< T, dims > log;
  auto nearest_node = find_nearest( map, root_key, key, log );
  if( nearest_node == map.end() ) { // empty
    return map.insert( std::make_pair( root_key, node< T, U, dims >( key, std::move( value ) ) ) );
  }
  if( !contains< T, dims >( nearest_node->second.get_range(), key ) ) { // out of range
    const auto new_value_key = key;
    const auto [key,lchild,rchild] = get_key_containing_both< T, dims >( nearest_node->second.get_range(), new_value_key );
    const auto lkey = nearest_node->first;
    auto ldata = std::move( nearest_node->second );
    map.erase( nearest_node );
    auto lc_insert_result = map.insert( std::make_pair( lchild,std::move( ldata ) ) );
    assert( lc_insert_result.second );
    auto rc_insert_result = map.insert( std::make_pair( rchild, node< T, U, dims >( new_value_key, std::move( value ) ) ) );
    assert( rc_insert_result.second );
    child_availability_map< T, dims > camap;
    node< T, U, dims > new_node( key, child_availability_map< T, dims >() );
    new_node.set_child( lchild );
    new_node.set_child( rchild );
    const auto insert_result = map.insert( std::make_pair( lkey, std::move( new_node ) ) );
    return insert_result;
  }
  if( !nearest_node->second.is_leaf() ) {
    if( nearest_node->second.get_range() != key ) { // conflict
      const auto new_value_key = key;
      const auto child_key = get_key_in_depth< T, dims >( get_depth< T, dims >( nearest_node->second.get_range() ) - 1u, key );
      nearest_node->second.set_child( child_key );
      auto insert_result = map.insert( std::make_pair( child_key, node< T, U, dims >( new_value_key, std::move( value ) ) ) );
      assert( insert_result.second );
      auto &inserted_value = insert_result.first->second.get_data();
      if( nearest_node->second.get_count() == get_max_child_count< dims >() ) {
        auto current_key = nearest_node->first;
        auto value = inserted_value;
        while( is_uniform_of( map, current_key, value, func ) ) {
          erase_children( map, current_key );
          auto node = map.find( current_key );
          assert( node != map.end() );
          node->second.set_data( value );
          if( log.empty() ) break;
          current_key = get_key_in_depth< T, dims >( log.back(), current_key );
          log.pop_back();
        }
      }
      return insert_result;
    }
  }
  return std::make_pair( nearest_node, false );
}

template< HDMapUnderlyingContainer C >
void split(
  C &map,
  extract_node_type_t< C > &current_node
) {
  using T = extract_key_type_t< C >;
  using U = extract_value_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  assert( current_node.is_leaf() );
  const auto current_depth = get_depth< T, dims >( current_node.get_range() );
  assert( current_depth > 0u );
  auto value = current_node.get_data();
  current_node.clear_child();
  {
    auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, current_node.get_range() );
    for( unsigned int i = 0u; i != get_max_child_count< dims >(); ++i ) {
      current_node.set_child( child_key );
      child_key = next_key< T, dims >( child_key, 1u );
    }
  }
  {
    auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, current_node.get_range() );
    for( unsigned int i = 0u; i != get_max_child_count< dims >(); ++i ) {
      auto insert_result = map.insert( std::make_pair( child_key, node< T, U, dims >( child_key, U( value ) ) ) );
      assert( insert_result.second );
      child_key = next_key< T, dims >( child_key, 1u );
    }
  }
}

template< HDMapUnderlyingContainer C >
auto erase(
  C &map,
  extract_key_type_t< C > current_key
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  constexpr auto root_key = get_root_key< T, dims >();
  depth_log_t< T, dims > log;
  auto nearest_node = find_nearest( map, root_key, current_key, log );
  if( nearest_node == map.end() ) { // empty
    return 0u;
  }
  if( nearest_node->second.get_range() != current_key ) {
    if( !contains< T, dims >( current_key, nearest_node->second.get_range() ) ) {
      if( contains< T, dims >( nearest_node->second.get_range(), current_key ) ) { // poking
        assert( nearest_node->second.is_leaf() );
        split( map, nearest_node->second );
        return erase( map, current_key );
      }
      else { // not found
        return 0u;
      }
    }
  }
  unsigned int erased_child_count = 0u;
  if( !nearest_node->second.is_leaf() ) { // is node
    auto nearest_key = nearest_node->first;
    for_each_child(
      map,
      nearest_key,
      [&]( auto &child_node, auto ) {
        erased_child_count += erase( map, child_node->first );
        return true;
      }
    );
    log.clear();
    nearest_node = find_nearest( map, root_key, current_key, log );
  }
  assert( nearest_node->second.is_leaf() );
  if( nearest_node->first == get_root_key< T, dims >() ) { // is root
    const auto leaves = get_max_leaf_count< T, dims >( nearest_node->second.get_range() );
    map.erase( nearest_node );
    assert( map.empty() );
    return leaves;
  }
  const auto parent_node = map.find( get_key_in_depth< T, dims >( log.back(), nearest_node->first ) );
  assert( parent_node != map.end() );
  const auto leaves = get_max_leaf_count< T, dims >( nearest_node->second.get_range() );
  parent_node->second.remove_child( nearest_node->second.get_range() );
  const auto left_children = parent_node->second.get_count();
  assert( left_children > 0u );
  if( left_children == 1u ) { // remove parent node and leaf
    const auto [last_child_node,child_index] = get_child( map, parent_node->first );
    assert( last_child_node != map.end() );
    parent_node->second = last_child_node->second;
    const auto last_child_key = last_child_node->first;
    map.erase( nearest_node );
    assert( map.erase( last_child_key ) == 1u );
    assert( !map.empty() );
    return erased_child_count + leaves;
  }
  // remove leaf only
  assert( nearest_node->second.is_leaf() );
  map.erase( nearest_node );
  assert( !map.empty() );
  return erased_child_count + leaves;
}

template< HDMapUnderlyingContainer C, typename F >
auto copy(
  const C &from,
  extract_key_type_t< C > current_key,
  C &to,
  const F &func,
  bool trim = true,
  bool is_root = true
) {
  using T = extract_key_type_t< C >;
  using U = extract_value_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  auto nearest_node = find_nearest( from, is_root ? get_root_key< T, dims >() : current_key, current_key );
  if( nearest_node == from.end() ) return;
  if( contains< T, dims >( current_key, nearest_node->second.get_range() ) ) {
    if( !nearest_node->second.is_leaf() ) {
      auto current_depth = get_depth< T, dims >( nearest_node->second.get_range() );
      auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, nearest_node->second.get_range() );
      for( unsigned int i = 0u; i != get_max_child_count< dims >(); ++i ) {
        if( nearest_node->second.has_child( child_key ) ) {
          copy( from, child_key, to, func, trim, false );
        }
        child_key = next_key< T, dims >( child_key, 1u );
      }
    }
    else {
      assert( ( insert( to, U( nearest_node->second.get_data() ), nearest_node->second.get_range(), func ).second ) );
    }
  }
  else if( contains< T, dims >( nearest_node->second.get_range(), current_key ) ) {
    if( nearest_node->second.is_leaf() ) {
      if( trim )
        assert( ( insert( to, U( nearest_node->second.get_data() ), current_key, func ).second ) );
      else
        insert( to, U( nearest_node->second.get_data() ), nearest_node->second.get_range(), func );
    }
  }
}

template< HDMapUnderlyingContainer C, typename F >
auto copy(
  const C &from,
  const covering_rectangle_t< C > &range,
  C &to,
  const F &func,
  bool trim = true
) {
  using T = extract_key_type_t< C >;
  using U = extract_value_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  depth_log_t< T, dims > log;
  auto cached_node = from.end();
  for_each_in_rectangle_region(
    range,
    [&]( auto ... p ) {
      const auto key = to_key< T, dims >( 0u, p... );
      if( cached_node != from.end() ) {
        if( contains< T, dims >( cached_node->second.get_range(), key ) ) {
          if( trim )
            insert( to, U( cached_node->second.get_data() ), key, func );
        }
        else {
          auto root_key = cached_node->first;
          unsigned int log_pos = log.size();
          while( !contains< T, dims >( root_key, key ) ) {
            --log_pos;
            root_key = get_key_in_depth< T, dims >( log[ log_pos ], root_key );
          }
          depth_log_t< T, dims > new_log;
          auto node = find_nearest( from, root_key, key, new_log );
          if( node->second.is_leaf() ) {
            if( contains< T, dims >( node->second.get_range(), key ) ) {
              if( trim )
                insert( to, U( node->second.get_data() ), key, func );
              else 
                insert( to, U( node->second.get_data() ), node->second.get_range(), func );
              log.erase( std::next( log.begin(), log_pos ), log.end() );
              log.insert( log.end(), new_log.begin(), new_log.end() );
              cached_node = node;
            }
          }
        }
      }
      else {
        depth_log_t< T, dims > new_log;
        auto node = find_nearest( from, get_root_key< T, dims >(), key, new_log );
        if( node->second.is_leaf() ) {
          if( contains< T, dims >( node->second.get_range(), key ) ) {
            if( trim )
              insert( to, U( node->second.get_data() ), key, func );
            else
              insert( to, U( node->second.get_data() ), node->second.get_range(), func );
            log = std::move( new_log );
            cached_node = node;
          }
        }
      }
      return true;
    }
  );
}

template< HDMapUnderlyingContainer C, typename F >
auto move(
  C &from,
  extract_key_type_t< C > current_key,
  C &to,
  const F &func,
  bool trim = true,
  bool is_root = true
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  depth_log_t< T, dims > log;
  auto nearest_node = find_nearest( from, is_root ? get_root_key< T, dims >() : current_key, current_key, log );
  if( nearest_node == from.end() ) return;
  if( contains< T, dims >( current_key, nearest_node->second.get_range() ) ) {
    if( !nearest_node->second.is_leaf() ) {
      auto current_depth = get_depth< T, dims >( nearest_node->second.get_range() );
      auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, nearest_node->second.get_range() );
      for( unsigned int i = 0u; i != get_max_child_count< dims >(); ++i ) {
        if( nearest_node->second.has_child( child_key ) ) {
          copy( from, child_key, to, func, trim, false );
        }
        child_key = next_key< T, dims >( child_key, 1u );
      }
    }
    else {
      assert( ( insert( to, std::move( nearest_node->second.move_data() ), nearest_node->second.get_range(), func ).second ) );
    }
  }
  else if( contains< T, dims >( nearest_node->second.get_range(), current_key ) ) {
    if( nearest_node->second.is_leaf() ) {
      if( trim )
        assert( ( insert( to, std::move( nearest_node->second.move_data() ), current_key, func ).second ) );
      else
        assert( ( insert( to, std::move( nearest_node->second.move_data() ), nearest_node->second.get_range(), func ).second ) );
    }
  }
}

template< HDMapUnderlyingContainer C, typename F >
auto update(
  C &map,
  extract_value_type_t< C > &&value,
  extract_key_type_t< C > key,
  const F &func
) {
  C affected;
  copy( map, key, affected, func );
  erase( map, key );
  auto [insert_iter,inserted] = insert( map, std::move( value ), key, func );
  assert( inserted );
  return std::make_pair( insert_iter, std::move( affected ) );
}

template< std::unsigned_integral T, unsigned int dims, unsigned int d, typename F, typename ...I >
auto for_each_in_rectangle_region_internal(
  const rectangle< T, dims > &range,
  F &&func,
  I ...p
) {
  if constexpr ( d == dims ) {
    func( p... );
    return;
  }
  else {
    const auto begin = get_component< T, dims >( d, range.left_top );
    const auto end = get_component< T, dims >( d, range.right_bottom );
    for( unsigned int i = begin; i != end; ++i ) {
      for_each_in_rectangle_region_internal< T, dims, d + 1u >( range, std::move( func ), p..., i );
    }
  }
}

template< std::unsigned_integral T, unsigned int dims, typename F, typename ...I >
auto for_each_in_rectangle_region(
  const rectangle< T, dims > &range,
  F &&func,
  I ...p
) {
  return for_each_in_rectangle_region_internal< T, dims, 0u >( range, std::forward< F >( func ), p... );
}

template< std::unsigned_integral T, unsigned int dims >
auto get_right_bottom(
  T key
) {
  for( unsigned int i = 0u; i != dims; ++i ) {
    key += 1u << ( i * get_component_bits< T, dims >() );
  }
  return key;
}


template< std::unsigned_integral T, unsigned int dims >
auto contain(
  const rectangle< T, dims > &range,
  T key_left_top
) {
  const auto key_right_bottom = get_right_bottom< T, dims >( key_left_top );
  for( unsigned int i = 0u; i != dims; ++i ) {
    if( get_component< T, dims >( i, range.left_top ) > get_component< T, dims >( i, key_left_top ) ) return false;
    if( get_component< T, dims >( i, range.right_bottom ) < get_component< T, dims >( i, key_right_bottom ) ) return false;
  }
  return true;
}

template< std::unsigned_integral T, unsigned int dims >
auto contain(
  T key_left_top,
  const rectangle< T, dims > &range
) {
  const auto key_right_bottom = get_right_bottom< T, dims >( key_left_top );
  for( unsigned int i = 0u; i != dims; ++i ) {
    if( get_component< T, dims >( i, range.left_top ) < get_component< T, dims >( i, key_left_top ) ) return false;
    if( get_component< T, dims >( i, range.right_bottom ) > get_component< T, dims >( i, key_right_bottom ) ) return false;
  }
  return true;
}

template< HDMapUnderlyingContainer C, typename F >
auto insert(
  C &map,
  const extract_value_type_t< C > &value,
  extract_key_type_t< C > current_key,
  const covering_rectangle_t< C > &range,
  const F &func
) {
  using rect_t = covering_rectangle_t< C >;
  using L = extract_key_type_t< rect_t >;
  using U = extract_value_type_t< C >;
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;

  {
    auto extended_key_left_top = key_cast< L, dims >( current_key );
    auto extended_key_right_bottom = get_right_bottom< L, dims >( extended_key_left_top );
    const auto overlap = get_overlap_count( rect_t{ extended_key_left_top, extended_key_right_bottom }, range );
    if( overlap == 0u ) return false;
    if( overlap == get_max_leaf_count< L, dims >( extended_key_left_top ) ) {
      return insert( map, U( value ), current_key, func ).second;
    }
  }

  auto child_key = get_key_in_depth< T, dims >( get_depth< T, dims >( current_key ) - 1u, current_key );
  bool inserted = false;
  for( unsigned int i = 0u; i != get_max_child_count< dims >(); ++i ) {
    inserted |= insert( map, value, child_key, range, func );
    child_key = next_key< T, dims >( child_key, 1u );
  }
  return inserted;
}

template< HDMapUnderlyingContainer C, typename F >
auto insert(
  C &map,
  const extract_value_type_t< C > &value,
  const covering_rectangle_t< C > &range,
  const F &func
) {
  return insert( map, value, get_root_key< extract_key_type_t< C >, extract_dims_v< C > >(), range, func );
}

template< HDMapUnderlyingContainer C >
auto erase(
  C &map,
  extract_key_type_t< C > current_key,
  const covering_rectangle_t< C > &range
) {
  using rect_t = covering_rectangle_t< C >;
  using L = extract_key_type_t< rect_t >;
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  {
    auto extended_key_left_top = key_cast< L, dims >( current_key );
    auto extended_key_right_bottom = get_right_bottom< L, dims >( extended_key_left_top );
    const auto overlap = get_overlap_count( rect_t{ extended_key_left_top, extended_key_right_bottom }, range );
    if( overlap == 0u ) {
      return T( 0u );
    }
    if( overlap == get_max_leaf_count< L, dims >( extended_key_left_top ) ) {
      return erase( map, current_key );
    }
  }

  auto current_node = map.find( current_key );
  if( current_node == map.end() ) {
    auto nearest_node = find_nearest( map, get_root_key< T, dims >(), current_key );
    if( nearest_node == map.end() ) return T( 0u );
    if( !contains< T, dims >( current_key, nearest_node->second.get_range() ) ) return T( 0u );
    current_node = nearest_node;
  }

  auto extended_key_left_top = key_cast< L, dims >( current_node->second.get_range() );
  auto extended_key_right_bottom = get_right_bottom< L, dims >( extended_key_left_top );
  const auto overlap = get_overlap_count( rect_t{ extended_key_left_top, extended_key_right_bottom }, range );
  if( overlap == 0u ) {
    return T( 0u );
  }
  if( overlap == get_max_leaf_count< L, dims >( extended_key_left_top ) ) {
    return erase( map, current_node->second.get_range() );
  }

  T erased_count = 0u;
  if( current_node->second.is_leaf() ) {
    split( map, current_node->second );
    current_node = find_nearest( map, get_root_key< T, dims >(), current_key );
    assert( current_node != map.end() );
    assert( !current_node->second.is_leaf() );
  }
  for_each_child_key(
    map,
    current_node->first,
    [&]( auto &child_key, auto ) {
      erased_count += erase( map, child_key, range );
      return true;
    }
  );
  return erased_count;
}

template< HDMapUnderlyingContainer C >
auto erase(
  C &map,
  const covering_rectangle_t< C > &range
) {
  return erase( map, get_root_key< extract_key_type_t< C >, extract_dims_v< C > >(), range );
}


template< HDMapUnderlyingContainer C, typename F >
auto update(
  C &map,
  const extract_value_type_t< C > &value,
  const covering_rectangle_t< C > &range,
  const F &func
) {
  C affected;
  copy( map, range, affected, func, false );
  erase( map, range );
  auto inserted = insert( map, value, range, func );
  assert( inserted );
  auto rects = convert_to_rectangle_regions( affected, range, func );
  return rects;
}

}

namespace views {
  namespace leaf_detail {
    struct leaf_tag {};
    template< std::ranges::range R >
    auto operator|( R &&r, leaf_tag ) {
      return
        std::forward< R >( r ) |
        std::views::filter( []( const auto &v ){ return v.second.is_leaf(); } );
    }
  }
  inline constexpr auto leaf = leaf_detail::leaf_tag{};
  namespace rectangle_detail {
    template< std::unsigned_integral T, unsigned int dims >
    struct rectangle_tag {
      rectangle< T, dims > rect;
    };
    template< std::ranges::range R >
    auto operator|( R &&r, const rectangle_tag< detail::extract_key_type_t< detail::covering_rectangle_t< R > >, detail::extract_dims_v< R > > &params ) {
      using rect_t = detail::covering_rectangle_t< R >;
      using L = detail::extract_key_type_t< rect_t >;
      constexpr auto dims = detail::extract_dims_v< R >;
      return
        std::forward< R >( r ) |
        std::views::filter(
          [rect=params.rect]( const auto &v ) -> decltype(auto) {
            return detail::get_overlap_count< L, dims >( rect_t{ key_cast< L, dims >( v.second.get_range() ), detail::get_right_bottom< L, dims >( key_cast< L, dims >( v.second.get_range() ) ) }, rect ) != 0u;
          }
        );
    }
  }
  template< std::unsigned_integral T, unsigned int dims >
  constexpr auto rectangle( const ::hdmap::rectangle< T, dims > &rect ) {
    return rectangle_detail::rectangle_tag< T, dims >{ rect };
  }
  namespace indirect_detail {
    struct indirect_tag {};
    template< std::ranges::range R >
    auto operator|( R &&r, indirect_tag ) {
      return
        std::forward< R >( r ) |
        std::views::transform( []( auto &v ) -> decltype(auto) { return *v; } );
    }
  }
  inline constexpr auto indirect = indirect_detail::indirect_tag{};
  namespace node_detail {
    struct node_tag {};
    template< std::ranges::range R >
    auto operator|( R &&r, node_tag ) {
      using rect_t = detail::covering_rectangle_t< R >;
      using U = detail::extract_value_type_t< R >;
      using L = detail::extract_key_type_t< rect_t >;
      constexpr auto dims = detail::extract_dims_v< R >;
      return
        std::forward< R >( r ) |
        std::views::values |
        std::views::transform( []( const auto &v ) { return std::pair< rect_t, const U& >( rect_t{ key_cast< L, dims >( v.get_range() ), detail::get_right_bottom< L, dims >( key_cast< L, dims >( v.get_range() ) ) }, v.get_data() ); } );
    }
  }
  inline constexpr auto node = node_detail::node_tag{};
  namespace indirect_node_detail {
    struct tag {};
    template< std::ranges::range R >
    auto operator|( R &&r, tag ) {
      using rect_t = detail::covering_rectangle_t< R >;
      using U = std::remove_cvref_t< decltype( *std::declval< detail::extract_value_type_t< R >& >() ) >;
      using L = detail::extract_key_type_t< rect_t >;
      constexpr auto dims = detail::extract_dims_v< R >;
      return
        std::forward< R >( r ) |
        std::views::values |
        std::views::transform( []( const auto &v ) { return std::pair< rect_t, const U& >( rect_t{ key_cast< L, dims >( v.get_range() ), detail::get_right_bottom< L, dims >( key_cast< L, dims >( v.get_range() ) ) }, *v.get_data() ); } );
    }
  }
  inline constexpr auto indirect_node = indirect_node_detail::tag{};
  namespace indirect_rectangle_region_detail {
    struct tag {};
    template< std::ranges::range R >
    auto operator|( R &&r, tag ) {
      return
        std::forward< R >( r ) |
        std::views::values |
        std::views::transform( []( const auto &v ) { return std::pair< decltype( v.first ), decltype( *v.second ) >( v.first, *v.second ); } );
    }
  }
  inline constexpr auto indirect_rectangle_region = indirect_rectangle_region_detail::tag{};
}

namespace detail {

template< HDMapUnderlyingContainer C, typename F >
auto expand_rectangle_positive(
  C &&map,
  const covering_rectangle_t< C > &range,
  unsigned int axis,
  const extract_value_type_t< C > &value,
  const F &func
) {
  using T = extract_key_type_t< C >;
  using L = extract_key_type_t< covering_rectangle_t< C > >;
  constexpr auto dims = extract_dims_v< C >;
  auto left_top = get_key_in_depth< L, dims >( 0u, range.left_top );
  auto right_bottom = get_key_in_depth< L, dims >( 0u, range.right_bottom );
  if( get_component< L, dims >( axis, right_bottom ) == L( get_component_max< T, dims >() ) + 1u ) return range;
  set_component< L, dims >( axis, left_top, get_component< L, dims >( axis, right_bottom ) );
  set_component< L, dims >( axis, right_bottom, get_component< L, dims >( axis, right_bottom ) + 1u );
  std::remove_cvref_t< C > overlap;
  while( get_component< L, dims >( axis, left_top ) <= L( get_component_max< T, dims >() ) + 1u ) {
    overlap.clear();
    copy( map, rectangle< L, dims >{ left_top, right_bottom }, overlap, func, false );
    const auto [uniform,min,max] = is_uniform_of( overlap|views::leaf, rectangle< L, dims >{ left_top, right_bottom }, value, axis, func );
    if( !uniform ) break;
    set_component< L, dims >( axis, left_top, max );
    set_component< L, dims >( axis, right_bottom, max + 1u );
  }
  set_component< L, dims >( axis, right_bottom, get_component< L, dims >( axis, left_top ) );
  return rectangle< L, dims >{ range.left_top, right_bottom };
}

template< HDMapUnderlyingContainer C, typename F >
auto expand_rectangle_negative(
  C &&map,
  const covering_rectangle_t< C > &range,
  unsigned int axis,
  const extract_value_type_t< C > &value,
  const F &func
) {
  using L = extract_key_type_t< covering_rectangle_t< C > >;
  constexpr auto dims = extract_dims_v< C >;
  auto left_top = get_key_in_depth< L, dims >( 0u, range.left_top );
  auto right_bottom = get_key_in_depth< L, dims >( 0u, range.right_bottom );
  if( get_component< L, dims >( axis, left_top ) <= 0u ) return range;
  set_component< L, dims >( axis, right_bottom, get_component< L, dims >( axis, left_top ) );
  set_component< L, dims >( axis, left_top, get_component< L, dims >( axis, left_top ) - 1u );
  std::remove_cvref_t< C > overlap;
  while( get_component< L, dims >( axis, right_bottom ) > 0u ) {
    overlap.clear();
    copy( map, rectangle< L, dims >{ left_top, right_bottom }, overlap, func, false );
    const auto [uniform,min,max] = is_uniform_of( overlap|views::leaf, rectangle< L, dims >{ left_top, right_bottom }, value, axis, func );
    if( !uniform ) break;
    set_component< L, dims >( axis, left_top, min - 1u );
    set_component< L, dims >( axis, right_bottom, min );
  }
  set_component< L, dims >( axis, left_top, get_component< L, dims >( axis, right_bottom ) );
  return rectangle< L, dims >{ left_top, range.right_bottom };
}

template< std::unsigned_integral T, unsigned int dims >
rectangle< T, dims > operator&(
  const rectangle< T, dims > &l,
  const rectangle< T, dims > &r
) {
  rectangle< T, dims > temp;
  for( unsigned int i = 0u; i != dims; ++i ) {
    set_component< T, dims >( i, temp.left_top, std::max( get_component< T, dims >( i, l.left_top ), get_component< T, dims >( i, r.left_top ) ) );
    set_component< T, dims >( i, temp.right_bottom, std::min( get_component< T, dims >( i, l.right_bottom ), get_component< T, dims >( i, r.right_bottom ) ) );
    if( get_component< T, dims >( i, temp.left_top ) >= get_component< T, dims >( i, temp.right_bottom ) )
      return rectangle< T, dims >{};
  }
  return temp;
}

template< HDMapUnderlyingContainer C, typename F >
auto expand_rectangle(
  C &&map,
  const F &func
) {
  using L = extract_key_type_t< covering_rectangle_t< C > >;
  using U = extract_value_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  if( map.empty() ) return std::make_pair( covering_rectangle_t< C >{}, U() );
  auto view = map|views::leaf;
  const auto begin = std::ranges::begin( view );
  assert( std::ranges::distance( view ) != 0 );
  const auto left_top = key_cast< L, dims >( begin->second.get_range() );
  const auto right_bottom = get_right_bottom< L, dims >( left_top );
  auto rect = covering_rectangle_t< C >{
    left_top,
    right_bottom
  };
  for( unsigned int i = 0u; i != dims; ++i ) {
    rect = expand_rectangle_positive( map, rect, i, begin->second.get_data(), func );
    rect = expand_rectangle_negative( map, rect, i, begin->second.get_data(), func );
  }
  return std::make_pair( rect, begin->second.get_data() );
}

template< HDMapUnderlyingContainer C, typename F >
auto convert_to_rectangle_regions(
  C &&map,
  const covering_rectangle_t< C > &range,
  std::vector< std::pair< covering_rectangle_t< C >, extract_value_type_t< C > > > &rects,
  const F &func
) {
  using L = extract_key_type_t< covering_rectangle_t< C > >;
  constexpr auto dims = extract_dims_v< C >;
  while( !map.empty() ) {
    auto [rect,value] = expand_rectangle( map, func );
    auto inner_rect =  rect & range;
    if( get_max_leaf_count< L, dims >( inner_rect ) )
      rects.emplace_back( rect & range, value );
    erase( map, rect );
  }
}

template< HDMapUnderlyingContainer C, typename F >
auto convert_to_rectangle_regions(
  C &&map,
  const covering_rectangle_t< C > &range,
  const F &func
) {
  using L = extract_key_type_t< covering_rectangle_t< C > >;
  using U = extract_value_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  std::vector< std::pair< rectangle< L, dims >, U > > rects;
  convert_to_rectangle_regions( std::forward< C >( map ), range, rects, func );
  return rects;
}

}

template<
  std::unsigned_integral T,
  typename U,
  unsigned int dims,
  typename Hash = std::hash< T >,
  typename Pred = std::equal_to< T >,
  typename Allocator = std::allocator< std::pair< const T, detail::node< T, U, dims > > >
>
using standard_underlying_container_t = std::unordered_map<
  T,
  detail::node< T, U, dims >,
  Hash,
  Pred,
  Allocator
>;

template<
  std::unsigned_integral T,
  typename U,
  unsigned int dims,
  std::regular_invocable< const U&, const U& > EqualTo = std::equal_to< U >,
  HDMapUnderlyingContainer C = standard_underlying_container_t< T, U, dims >
>
class hdmap {
public:
  using rect_type = detail::covering_rectangle_t< C >;
  using key_type = detail::extract_key_type_t< rect_type >;
  using mapped_type = detail::extract_value_type_t< C >;
  using value_type = std::pair< rect_type, mapped_type >;
  hdmap(
    EqualTo &&equal_to_ = EqualTo{},
    C &&map_ = C{}
  ) :
    equal_to( std::move( equal_to_ ) ),
    map( std::move( map_ ) ) {
  }
  hdmap( const hdmap& ) = default;
  hdmap( hdmap&& ) = default;
  hdmap &operator=( const hdmap& ) = default;
  hdmap &operator=( hdmap&& ) = default;
  template< std::unsigned_integral ...I >
  static key_type enc( I ...v ) {
    return detail::to_key< key_type, dims >( 0u, v... );
  }
  static std::array< T, dims > dec( key_type v ) {
    std::array< T, dims > temp;
    for( unsigned int i = 0u; i != dims; ++i )
      temp[ i ] = detail::get_component< key_type, dims >( i, v );
    return temp;
  }
  static rect_type rect( key_type begin, key_type end ) {
    return rect_type{ begin, end };
  }
  auto erase(
    const rect_type &range
  ) {
    return detail::erase( map, range );
  }
  auto find(
    const rect_type &range,
    std::vector< value_type > &dest
  ) const {
    C affected;
    detail::copy( map, range, affected, equal_to, false );
    detail::convert_to_rectangle_regions( affected, range, dest, equal_to );
  }
  auto update(
    const rect_type &range,
    U &&value
  ) {
    detail::erase( map, range );
    return detail::insert( map, std::move( value ), range, equal_to );
  }
  auto all(
    std::vector< value_type > &dest
  ) {
    C affected = map;
    detail::convert_to_rectangle_regions( affected, rect_type{ detail::get_root_key< key_type, dims >(), detail::get_right_bottom< key_type, dims >( detail::get_root_key< key_type, dims >() ) }, dest, equal_to );
  }
  void clear() {
    map.clear();
  }
  bool empty() const {
    return map.empty();
  }
  const C &nodes() const {
    return map;
  }
  auto leaves() const {
    return map|views::leaf|views::node;
  }
  auto size() const {
    auto l = leaves();
    std::size_t sum = 0u;
    for( const auto &v: l ) {
      sum += detail::get_max_leaf_count( v.first );
    }
    return sum;
  }
  auto hash_function() const {
    return map.hash_function();
  }
  auto key_eq() const {
    return map.key_eq();
  }
  EqualTo value_eq() const {
    return equal_to;
  }
private:
  EqualTo equal_to;
  mutable C map;
};

template< typename T >
struct is_hdmap : public std::false_type {};
template<
  std::unsigned_integral T,
  typename U,
  unsigned int dims,
  std::regular_invocable< const U&, const U& > EqualTo,
  HDMapUnderlyingContainer C
>
struct is_hdmap< hdmap< T, U, dims, EqualTo, C > > : public std::true_type {};
template< typename T >
constexpr bool is_hdmap_v = is_hdmap< T >::value;
template< typename T >
concept HDMap = is_hdmap_v< std::remove_cvref_t< T > >;

}
#endif

