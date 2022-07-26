#define BOOST_TEST_MODULE hdmap
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <hdmap/json.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Update ) {
  using map_t = ::hdmap::hdmap< std::uint32_t, std::string, 2u >;
  map_t map;
  map.update( map.rect( map.enc( 0u, 0u ), map.enc( 500u, 500u ) ), "hello" );
  std::vector< map_t::value_type > affected;
  map.update( map.rect( map.enc( 50u, 30u ), map.enc( 52u, 33u ) ), "world" );
  map.find( map.rect( map.enc( 50u, 30u ), map.enc( 53u, 34u ) ), affected );
  BOOST_CHECK_EQUAL( affected.size(), 3u );
  BOOST_CHECK_EQUAL( affected[ 0 ].first.left_top, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 50u, 33u ) ) );
  BOOST_CHECK_EQUAL( affected[ 0 ].first.right_bottom, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 53u, 34u ) ) );
  BOOST_CHECK_EQUAL( affected[ 1 ].first.left_top, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 52u, 30u ) ) );
  BOOST_CHECK_EQUAL( affected[ 1 ].first.right_bottom, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 53u, 33u ) ) );
  BOOST_CHECK_EQUAL( affected[ 2 ].first.left_top, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 50u, 30u ) ) );
  BOOST_CHECK_EQUAL( affected[ 2 ].first.right_bottom, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 52u, 33u ) ) );
  BOOST_CHECK_EQUAL( map.size(), 250000u );
}

