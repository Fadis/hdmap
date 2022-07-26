#define BOOST_TEST_MODULE update_range
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <hdmap/json.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Update ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  hdmap::detail::insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 2u >( 6u, 0x0u, 0x0u ), std::equal_to< std::string >{} );
  {
    auto affected = hdmap::detail::update( uc, "hoge", hdmap::rectangle< std::uint64_t, 2u >{ hdmap::detail::to_key< std::uint64_t, 2u >( 1u, 281u, 256u ), hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 284u, 259u ) }, std::equal_to< std::string >{} );
    {
      unsigned int count = 0u;
      for( const auto &[key,value]: uc ) {
        if( value.is_leaf() ) ++count;
      }
      BOOST_CHECK_EQUAL( count, 91u );
    }
    BOOST_CHECK_EQUAL( affected.size(), 1u );
    BOOST_CHECK_EQUAL( affected.begin()->first.left_top, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 280u, 256u )  ) );
    BOOST_CHECK_EQUAL( affected.begin()->first.right_bottom, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 284u, 259u )  ) );
  }
  {
    auto affected = hdmap::detail::update( uc, "piyo", hdmap::rectangle< std::uint64_t, 2u >{ hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 256u ), hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 261u ) }, std::equal_to< std::string >{} );
    BOOST_CHECK_EQUAL( affected.size(), 3u );
    for( const auto &v: affected ) {
      std::cout << hdmap::detail::key_to_string< std::uint64_t, 2u >( v.first.left_top ) << " " << hdmap::detail::key_to_string< std::uint64_t, 2u >( v.first.right_bottom ) << std::endl;
    }
    BOOST_CHECK_EQUAL( affected[ 0 ].first.left_top, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 260u ) ) );
    BOOST_CHECK_EQUAL( affected[ 0 ].first.right_bottom, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 261u ) ) );
    BOOST_CHECK_EQUAL( affected[ 1 ].first.left_top, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 259u ) ) );
    BOOST_CHECK_EQUAL( affected[ 1 ].first.right_bottom, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 260u ) ) );
    BOOST_CHECK_EQUAL( affected[ 2 ].first.left_top, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 256u ) ) );
    BOOST_CHECK_EQUAL( affected[ 2 ].first.right_bottom, ( hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 259u ) ) );
  }
}

