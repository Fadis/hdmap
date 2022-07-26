#ifndef HDMAP_JSON_HPP
#define HDMAP_JSON_HPP

#include <string>
#include <nlohmann/json.hpp>
#include <hdmap/hdmap.hpp>

namespace hdmap {

namespace detail {

template< std::unsigned_integral T, unsigned int dims, unsigned int i >
auto key_to_json_internal( nlohmann::json&, T ) -> std::enable_if_t< i == dims > {}

template< std::unsigned_integral T, unsigned int dims, unsigned int i >
auto key_to_json_internal( nlohmann::json &dest, T v ) -> std::enable_if_t< i != dims > {
  dest.push_back( get_component< T, dims >( i, v ) );
  key_to_json_internal< T, dims, i + 1u >( dest, v );
}

template< std::unsigned_integral T, unsigned int dims >
nlohmann::json key_to_json( T left_top ) {
  nlohmann::json root = nlohmann::json::object();
  const auto depth = get_depth< T, dims >( left_top );
  root[ "depth" ] = depth;
  if( depth == 0u ) {
    nlohmann::json pos = nlohmann::json::array();
    key_to_json_internal< T, dims, 0u >( pos, left_top );
    root[ "pos" ] = pos;
  }
  else {
    root[ "range" ] = nlohmann::json::array();
    nlohmann::json begin = nlohmann::json::array();
    nlohmann::json end = nlohmann::json::array();
    const auto right_bottom = get_right_bottom< T, dims >( left_top );
    key_to_json_internal< T, dims, 0u >( begin, left_top );
    key_to_json_internal< T, dims, 0u >( end, right_bottom );
    root[ "range" ].push_back( begin );
    root[ "range" ].push_back( end );
  }
  return root;
}

template< std::unsigned_integral T, unsigned int dims >
std::string key_to_string( T v ) {
  return key_to_json< T, dims >( v ).dump();
}

template< std::unsigned_integral T, typename U, unsigned int dims >
void to_json(
  nlohmann::json &root,
  const node< T, U, dims > &node
) {
  root[ "range" ] = key_to_json< T, dims >( node.get_range() );
  if( node.is_leaf() ) {
    root[ "type" ] = "leaf";
    root[ "value" ] = node.get_data();
  }
  else {
    root[ "type" ] = "node";
    root[ "count" ] = node.get_count();
    const auto camap = node.get_child_availability_map();
    root[ "camap" ] = camap.value;
  }
}

template< std::unsigned_integral T, typename U, unsigned int dims >
void to_json(
  nlohmann::json &root,
  const std::pair< T, node< T, U, dims > > &node
) {
  root[ "key" ] = key_to_json< T, dims >( node->first );
  to_json( root, node->second );
}


template< HDMapUnderlyingContainer C >
void recursive_node_to_json(
  nlohmann::json &root,
  const std::pair< extract_key_type_t< C >, extract_node_type_t< C > > &node,
  const C &map
);

template< HDMapUnderlyingContainer C >
void recursive_node_to_json(
  nlohmann::json &root,
  const extract_node_type_t< C > &node,
  const C &map
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  root = node;
  if( !node.is_leaf() ) {
    const auto current_depth = get_depth< T, dims >( node.get_range() );
    auto child_key = get_key_in_depth< T, dims >( current_depth - 1u, node.get_range() );
    root[ "child" ] = nlohmann::json::object();
    for( unsigned int i = 0u; i != get_max_child_count< dims >(); ++i ) {
      if( node.has_child( child_key ) ) {
        const auto node = map.find( child_key );
        if( node != map.end() ) {
          root[ "child" ][ std::to_string( i ) ] = nlohmann::json::object();
          recursive_node_to_json( root[ "child" ][ std::to_string( i ) ], *node, map );
        }
      }
      child_key = next_key< T, dims >( child_key, 1u );
    }
  }
}

template< HDMapUnderlyingContainer C >
void recursive_node_to_json(
  nlohmann::json &root,
  const std::pair< extract_key_type_t< C >, extract_node_type_t< C > > &node,
  const C &map
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  root[ "key" ] = key_to_json< T, dims >( node.first );
  recursive_node_to_json( root, node.second, map );
}

template< HDMapUnderlyingContainer C >
void to_json(
  nlohmann::json &dest,
  const C &map
) {
  using T = extract_key_type_t< C >;
  constexpr auto dims = extract_dims_v< C >;
  const auto node = map.find( get_root_key< T, dims >() );
  if( node == map.end() ) dest = nlohmann::json( nullptr );
  else recursive_node_to_json( dest, *node, map );
}

template< HDMapUnderlyingContainer C >
std::string to_string(
  const extract_node_type_t< C > &node,
  const C &map
) {
  nlohmann::json dest;
  recursive_node_to_json( dest, node, map );
  return dest.dump();
}

template< HDMapUnderlyingContainer C >
std::string to_string(
  const std::pair< extract_key_type_t< C >, extract_node_type_t< C > > &node,
  const C &map
) {
  nlohmann::json dest;
  recursive_node_to_json( dest, node, map );
  return dest.dump();
}

template< HDMapUnderlyingContainer C >
std::string to_json(
  const C &map
) {
  nlohmann::json dest = map;
  return dest.dump();
}

template< HDMapUnderlyingContainer C >
std::string to_string(
  const C &map
) {
  nlohmann::json dest = map;
  return dest.dump();
}

}

template< std::unsigned_integral T, unsigned int dims >
void to_json(
  nlohmann::json &root,
  const rectangle< T, dims > &v
) {
  root = nlohmann::json::object();
  root[ "begin" ] = detail::key_to_json< T, dims >( v.left_top );
  root[ "end" ] = detail::key_to_json< T, dims >( v.right_bottom );
}

template< std::unsigned_integral T, unsigned int dims >
std::string to_string(
  const rectangle< T, dims > &v
) {
  return nlohmann::json( v ).dump();
}

}
#endif

