#define BOOST_TEST_MODULE camap
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( KeyFormat1d ) {
  constexpr unsigned int dims = 1u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    4u
  );
}

BOOST_AUTO_TEST_CASE( KeyFormat2d ) {
  constexpr unsigned int dims = 2u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    16u
  );
}

BOOST_AUTO_TEST_CASE( KeyFormat3d ) {
  constexpr unsigned int dims = 3u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    64u
  );
}

BOOST_AUTO_TEST_CASE( KeyFormat4d ) {
  constexpr unsigned int dims = 4u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    16u
  );
}

BOOST_AUTO_TEST_CASE( KeyFormat5d ) {
  constexpr unsigned int dims = 5u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    20u
  );
}

BOOST_AUTO_TEST_CASE( KeyFormat6d ) {
  constexpr unsigned int dims = 6u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    24u
  );
}

BOOST_AUTO_TEST_CASE( KeyFormat7d ) {
  constexpr unsigned int dims = 7u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    28u
  );
}

BOOST_AUTO_TEST_CASE( KeyFormat8d ) {
  constexpr unsigned int dims = 8u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    32u
  );
}

BOOST_AUTO_TEST_CASE( KeyFormat9d ) {
  constexpr unsigned int dims = 9u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    36u
  );
}


BOOST_AUTO_TEST_CASE( KeyFormat10d ) {
  constexpr unsigned int dims = 10u;
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::camap_traits< dims >::map_bits ),
    40u
  );
}

