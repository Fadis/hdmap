#define BOOST_TEST_MODULE erase
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Erase ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 3u > uc;
  hdmap::detail::insert( uc, "hoge", hdmap::detail::to_key< std::uint32_t, 3u >( 1u, 4u, 180u, 32u ), std::equal_to< std::string >{} );
  hdmap::detail::insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 3u >( 0u, 94u, 38, 21u ), std::equal_to< std::string >{} );
  hdmap::detail::insert( uc, "foo", hdmap::detail::to_key< std::uint32_t, 3u >( 2u, 121u, 47u, 24u ), std::equal_to< std::string >{} );
  hdmap::detail::erase( uc, hdmap::detail::to_key< std::uint32_t, 3u >( 1u, 4u, 180u, 32u ) );
  {
    unsigned int count = 0u;
    for( const auto &[key,value]: uc ) {
      if( value.is_leaf() ) ++count;
    }
    BOOST_CHECK_EQUAL( count, 2u );
  }
  hdmap::detail::erase( uc, hdmap::detail::to_key< std::uint32_t, 3u >( 0u, 94u, 38, 21u ) );
  {
    unsigned int count = 0u;
    for( const auto &[key,value]: uc ) {
      if( value.is_leaf() ) ++count;
    }
    BOOST_CHECK_EQUAL( count, 1u );
  }
  hdmap::detail::erase( uc, hdmap::detail::to_key< std::uint32_t, 3u >( 2u, 121u, 47u, 24u ) );
  {
    unsigned int count = 0u;
    for( const auto &[key,value]: uc ) {
      if( value.is_leaf() ) ++count;
    }
    BOOST_CHECK_EQUAL( count, 0u );
  }
}

BOOST_AUTO_TEST_CASE( EraseSplit ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  hdmap::detail::insert( uc, "hoge", hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 16u, 16u ), std::equal_to< std::string >{} );
  hdmap::detail::erase( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 17u, 17u ) );
  {
    unsigned int count = 0u;
    for( const auto &[key,value]: uc ) {
      if( value.is_leaf() ) ++count;
    }
    BOOST_CHECK_EQUAL( count, 15u );
  }
}

