#define BOOST_TEST_MODULE find
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Find ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 3u > uc;
  insert( uc, "hoge", hdmap::detail::to_key< std::uint32_t, 3u >( 1u, 4u, 180u, 32u ), std::equal_to< std::string >{} );
  insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 3u >( 0u, 94u, 38, 21u ), std::equal_to< std::string >{} );
  insert( uc, "foo", hdmap::detail::to_key< std::uint32_t, 3u >( 2u, 121u, 47u, 24u ), std::equal_to< std::string >{} );
  insert( uc, "a", hdmap::detail::to_key< std::uint32_t, 3u >( 0u, 114u, 168u, 26u ), std::equal_to< std::string >{} );
  insert( uc, "mo", hdmap::detail::to_key< std::uint32_t, 3u >( 0u, 74u, 159u, 111u ), std::equal_to< std::string >{} );
  {
    auto found = hdmap::detail::find( uc, hdmap::detail::to_key< std::uint32_t, 3u >( 1u, 4u, 180u, 32u ) );
    BOOST_CHECK( found != uc.end() );
    BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 3u >( 1u, 4u, 180u, 32u ) ) );
    BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
    BOOST_CHECK_EQUAL( found->second.get_data(), "hoge" );
  }
  {
    auto found = hdmap::detail::find( uc, hdmap::detail::to_key< std::uint32_t, 3u >( 0u, 5u, 181u, 33u ) );
    BOOST_CHECK( found != uc.end() );
    BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 3u >( 1u, 4u, 180u, 32u ) ) );
    BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
    BOOST_CHECK_EQUAL( found->second.get_data(), "hoge" );
  }
}

