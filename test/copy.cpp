#define BOOST_TEST_MODULE copy
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Copy ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  hdmap::detail::insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 2u >( 5u, 0x123u, 0x156u ), std::equal_to< std::string >{} );
  hdmap::detail::erase( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 3u, 0x123u, 0x156u ) );
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc2;
  hdmap::detail::copy( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 3u, 256u, 256u ), uc2, std::equal_to< std::string >{} );
  {
    unsigned int count = 0u;
    for( const auto &[key,value]: uc2 ) {
      if( value.is_leaf() ) ++count;
    }
    BOOST_CHECK_EQUAL( count, 1u );
    auto found = find( uc2, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 257u, 258u ) );
    BOOST_CHECK( found != uc2.end() );
    BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
    BOOST_CHECK_EQUAL( found->second.get_data(), "fuga" );
  }
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc3;
  hdmap::detail::copy( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 4u, 0x123u, 0x156u ), uc3, std::equal_to< std::string >{} );
  {
    unsigned int count = 0u;
    for( const auto &[key,value]: uc3 ) {
      if( value.is_leaf() ) ++count;
    }
    BOOST_CHECK_EQUAL( count, 15u );
  }
}

BOOST_AUTO_TEST_CASE( Move ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  hdmap::detail::insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 2u >( 5u, 0x123u, 0x156u ), std::equal_to< std::string >{} );
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc4;
  hdmap::detail::move( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 3u, 256u, 256u ), uc4, std::equal_to< std::string >{} );
  {
    unsigned int count = 0u;
    for( const auto &[key,value]: uc4 ) {
      if( value.is_leaf() ) ++count;
    }
    BOOST_CHECK_EQUAL( count, 1u );
    auto found = find( uc4, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 257u, 258u ) );
    BOOST_CHECK( found != uc4.end() );
    BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
    BOOST_CHECK_EQUAL( found->second.get_data(), "fuga" );
  }
}

