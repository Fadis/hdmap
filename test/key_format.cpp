#define BOOST_TEST_MODULE key_format
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

#define HDMAP_KEY_REQUIREMENTS \
  BOOST_CHECK_LE( \
    ( \
      hdmap::detail::get_depth_bits< T, dims >() + \
      hdmap::detail::get_component_bits< T, dims >() * dims \
    ), \
    sizeof( T ) * 8u \
  ); \
  BOOST_CHECK_LE( \
    sizeof( T ) * 8u - \
    ( \
      hdmap::detail::get_depth_bits< T, dims >() + \
      hdmap::detail::get_component_bits< T, dims >() * dims \
    ), \
    dims \
  ); \
  BOOST_CHECK_LE( \
    ( \
      hdmap::detail::get_depth_bits< T, dims >() + \
      hdmap::detail::get_component_bits< T, dims >() * dims \
    ), \
    sizeof( T ) * 8u \
  ); \
  BOOST_CHECK_GE( \
    ( hdmap::detail::get_component_bits< T, dims >() ), \
    2u \
  ); \
  BOOST_CHECK_GT( \
    T( 1u ) << ( hdmap::detail::get_depth_bits< T, dims >() * 2u ), \
    ( hdmap::detail::get_component_bits< T, dims >() ) \
  );

BOOST_AUTO_TEST_CASE( KeyFormat8bit1d ) {
  using T = std::uint8_t;
  constexpr unsigned int dims = 1u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat8bit2d ) {
  using T = std::uint8_t;
  constexpr unsigned int dims = 2u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat8bit3d ) {
  using T = std::uint8_t;
  constexpr unsigned int dims = 3u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat16bit1d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 1u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat16bit2d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 2u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat16bit3d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 3u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat16bit4d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 4u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat16bit5d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 5u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat16bit6d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 6u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat16bit7d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 7u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit1d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 1u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit2d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 2u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit3d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 3u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit4d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 4u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit5d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 5u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit6d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 6u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit7d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 7u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit8d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 8u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit9d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 9u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat32bit10d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 10u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit1d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 1u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit2d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 2u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit3d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 3u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit4d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 4u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit5d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 5u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit6d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 6u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit7d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 7u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit8d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 8u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit9d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 9u;
  HDMAP_KEY_REQUIREMENTS
}

BOOST_AUTO_TEST_CASE( KeyFormat64bit10d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 10u;
  HDMAP_KEY_REQUIREMENTS
}

