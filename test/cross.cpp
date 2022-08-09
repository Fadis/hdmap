#define BOOST_TEST_MODULE cross
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <hdmap/cross.hpp>
#include <hdmap/json.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Cross ) {
  using map_t = ::hdmap::hdmap< std::uint32_t, std::string, 2u >;
  map_t a;
  a.update( a.rect( a.enc( 0u, 0u ), a.enc( 100u, 100u ) ), "hello" );
  map_t b;
  b.update( b.rect( b.enc( 50u, 30u ), b.enc( 120u, 33u ) ), "world" );
  auto c = hdmap::cross( a, b );
  std::vector< std::remove_cvref_t< decltype( c ) >::value_type > rects;
  c.find( c.rect( c.enc( 0u, 0u ), c.enc( 200u, 200u ) ), rects );
  BOOST_CHECK_EQUAL( rects.size(), 1u );
  BOOST_CHECK_EQUAL( rects[ 0 ].first.left_top, c.enc( 50u, 30u ) );
  BOOST_CHECK_EQUAL( rects[ 0 ].first.right_bottom, c.enc( 100u, 33u ) );
  BOOST_CHECK_EQUAL( rects[ 0 ].second.first, "hello" );
  BOOST_CHECK_EQUAL( rects[ 0 ].second.second, "world" );
}

BOOST_AUTO_TEST_CASE( CrossInRectRegion ) {
  using map_t = ::hdmap::hdmap< std::uint32_t, std::string, 2u >;
  map_t a;
  a.update( a.rect( a.enc( 0u, 0u ), a.enc( 100u, 100u ) ), "hello" );
  map_t b;
  b.update( b.rect( b.enc( 50u, 30u ), b.enc( 120u, 33u ) ), "world" );
  auto c = hdmap::cross(
    a,
    b,
    a.rect( a.enc( 70u, 20u ), a.enc( 110u, 40u ) ),
    b.rect( b.enc( 80u, 25u ), b.enc( 120u, 45u ) )
  );
  std::vector< std::remove_cvref_t< decltype( c ) >::value_type > rects;
  c.find( c.rect( c.enc( 0u, 0u ), c.enc( 200u, 200u ) ), rects );
  BOOST_CHECK_EQUAL( rects.size(), 1u );
  BOOST_CHECK_EQUAL( rects[ 0 ].first.left_top, c.enc( 0u, 5u ) );
  BOOST_CHECK_EQUAL( rects[ 0 ].first.right_bottom, c.enc( 30u, 8u ) );
  BOOST_CHECK_EQUAL( rects[ 0 ].second.first, "hello" );
  BOOST_CHECK_EQUAL( rects[ 0 ].second.second, "world" );
}

