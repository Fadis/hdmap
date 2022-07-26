#define BOOST_TEST_MODULE rectangle_view
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( RectangleView ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  hdmap::detail::insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 2u >( 6u, 0x0u, 0x0u ), std::equal_to< std::string >{} );
  hdmap::detail::update( uc, "hoge", hdmap::rectangle< std::uint64_t, 2u >{ hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 256u ), hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 257u ) }, std::equal_to< std::string >{} );
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > copied;
  hdmap::detail::copy( uc, hdmap::rectangle< std::uint64_t, 2u >{ hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 254u ), hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 258u ) }, copied, std::equal_to< std::string >{}, false );
  auto expected_values = std::views::all( copied )|hdmap::views::leaf;
  auto filtered_values = std::views::all( uc )|hdmap::views::leaf|hdmap::views::rectangle( hdmap::rectangle< std::uint64_t, 2u >{ hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 254u ), hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 258u ) } );

  auto expected_values_iter = expected_values.begin();
  const auto expected_values_end = expected_values.end();
  auto filtered_values_iter = filtered_values.begin();
  const auto filtered_values_end = filtered_values.end();
  for( ; expected_values_iter != expected_values_end && filtered_values_iter != filtered_values_end; ++expected_values_iter, ++filtered_values_iter ) {
    BOOST_CHECK_EQUAL( expected_values_iter->second.get_range(), filtered_values_iter->second.get_range() );
    BOOST_CHECK_EQUAL( expected_values_iter->second.get_data(), filtered_values_iter->second.get_data() );
  }
}

