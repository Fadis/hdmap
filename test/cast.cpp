#define BOOST_TEST_MODULE cast
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Cast ) {
  const auto base = hdmap::rectangle< std::uint32_t, 2u >{ hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 281u, 254u ), hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 282u, 258u ) };
  const hdmap::rectangle< std::uint64_t, 2u > converted = hdmap::rectangle_cast< hdmap::rectangle< std::uint64_t, 2u > >( base );
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::get_component< std::uint32_t, 2u >( 0, base.left_top ) ),
    ( hdmap::detail::get_component< std::uint64_t, 2u >( 0, converted.left_top ) )
  );
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::get_component< std::uint32_t, 2u >( 1, base.left_top ) ),
    ( hdmap::detail::get_component< std::uint64_t, 2u >( 1, converted.left_top ) )
  );
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::get_depth< std::uint32_t, 2u >( base.left_top ) ),
    ( hdmap::detail::get_depth< std::uint64_t, 2u >( converted.left_top ) )
  );
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::get_component< std::uint32_t, 2u >( 0, base.right_bottom ) ),
    ( hdmap::detail::get_component< std::uint64_t, 2u >( 0, converted.right_bottom ) )
  );
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::get_component< std::uint32_t, 2u >( 1, base.right_bottom ) ),
    ( hdmap::detail::get_component< std::uint64_t, 2u >( 1, converted.right_bottom ) )
  );
  BOOST_CHECK_EQUAL(
    ( hdmap::detail::get_depth< std::uint32_t, 2u >( base.right_bottom ) ),
    ( hdmap::detail::get_depth< std::uint64_t, 2u >( converted.right_bottom ) )
  );
}

