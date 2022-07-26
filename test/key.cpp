#define BOOST_TEST_MODULE key
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( GetComponent8bit1d ) {
  using T = std::uint8_t;
  constexpr unsigned int dims = 1u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent8bit2d ) {
  using T = std::uint8_t;
  constexpr unsigned int dims = 2u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 3u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 2u );
}

BOOST_AUTO_TEST_CASE( GetComponent8bit3d ) {
  using T = std::uint8_t;
  constexpr unsigned int dims = 3u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  hdmap::detail::set_component< T, dims >( 2u, value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 3u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 2u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 2u, value ) ), 1u );
}

BOOST_AUTO_TEST_CASE( GetComponent16bit1d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 1u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent16bit2d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 2u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 3u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 2u );
}

BOOST_AUTO_TEST_CASE( GetComponent16bit3d ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 3u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  hdmap::detail::set_component< T, dims >( 2u, value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 3u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 2u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 2u, value ) ), 1u );
}

BOOST_AUTO_TEST_CASE( GetComponent32bit1d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 1u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent32bit2d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 2u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 3u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 2u );
}

BOOST_AUTO_TEST_CASE( GetComponent32bit3d ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 3u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  hdmap::detail::set_component< T, dims >( 2u, value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 3u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 2u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 2u, value ) ), 1u );
}

BOOST_AUTO_TEST_CASE( GetComponent64bit1d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 1u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent64bit2d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 2u;
  T value;
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 3u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 2u );
}

BOOST_AUTO_TEST_CASE( GetComponent64bit3d ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 3u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 0u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  hdmap::detail::set_component< T, dims >( 2u, value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 3u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 2u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 2u, value ) ), 1u );
}

BOOST_AUTO_TEST_CASE( GetComponent8bit1dNode ) {
  using T = std::uint8_t;
  constexpr unsigned int dims = 1u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent8bit2dNode ) {
  using T = std::uint8_t;
  constexpr unsigned int dims = 2u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 13u );
  hdmap::detail::set_component< T, dims >( 1u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent8bit3dNode ) {
  using T = std::uint8_t;
  constexpr unsigned int dims = 3u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  hdmap::detail::set_component< T, dims >( 2u, value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 2u, value ) ), 0u );
}

BOOST_AUTO_TEST_CASE( GetComponent16bit1dNode ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 1u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent16bit2dNode ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 2u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 13u );
  hdmap::detail::set_component< T, dims >( 1u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent16bit3dNode ) {
  using T = std::uint16_t;
  constexpr unsigned int dims = 3u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  hdmap::detail::set_component< T, dims >( 2u, value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 2u, value ) ), 0u );
}

BOOST_AUTO_TEST_CASE( GetComponent32bit1dNode ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 1u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent32bit2dNode ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 2u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 13u );
  hdmap::detail::set_component< T, dims >( 1u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent32bit3dNode ) {
  using T = std::uint32_t;
  constexpr unsigned int dims = 3u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  hdmap::detail::set_component< T, dims >( 2u, value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 2u, value ) ), 0u );
}

BOOST_AUTO_TEST_CASE( GetComponent64bit1dNode ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 1u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent64bit2dNode ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 2u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 13u );
  hdmap::detail::set_component< T, dims >( 1u, value, 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 12u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 12u );
}

BOOST_AUTO_TEST_CASE( GetComponent64bit3dNode ) {
  using T = std::uint64_t;
  constexpr unsigned int dims = 3u;
  T value;
  hdmap::detail::set_depth< T, dims >( value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_depth< T, dims >( value ) ), 1u );
  hdmap::detail::set_component< T, dims >( 0u, value, 3u );
  hdmap::detail::set_component< T, dims >( 1u, value, 2u );
  hdmap::detail::set_component< T, dims >( 2u, value, 1u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 0u, value ) ), 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 1u, value ) ), 0u );
  BOOST_CHECK_EQUAL( ( hdmap::detail::get_component< T, dims >( 2u, value ) ), 0u );
}

