#define BOOST_TEST_MODULE insert
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Insert ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 3u > uc;
  insert( uc, "hoge", hdmap::detail::to_key< std::uint32_t, 3u >( 1u, 4u, 180u, 32u ), std::equal_to< std::string >{} );
  insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 3u >( 0u, 94u, 38, 21u ), std::equal_to< std::string >{} );
  insert( uc, "foo", hdmap::detail::to_key< std::uint32_t, 3u >( 2u, 121u, 47u, 24u ), std::equal_to< std::string >{} );
  insert( uc, "a", hdmap::detail::to_key< std::uint32_t, 3u >( 0u, 114u, 168u, 26u ), std::equal_to< std::string >{} );
  insert( uc, "mo", hdmap::detail::to_key< std::uint32_t, 3u >( 0u, 74u, 159u, 111u ), std::equal_to< std::string >{} );
  unsigned int count = 0u;
  for( const auto &[key,value]: uc ) {
    if( value.is_leaf() ) ++count;
  }
  BOOST_CHECK_EQUAL( count, 5u );
}

BOOST_AUTO_TEST_CASE( InsertMerge ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  for( unsigned int x = 0u; x != 4u; ++x ) {
    for( unsigned int y = 0u; y != 4u; ++y ) {
      insert( uc, "hoge", hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 16u + x, 16u + y ), std::equal_to< std::string >{} );
    }
  }
  unsigned int count = 0u;
  for( const auto &[key,value]: uc ) {
    if( value.is_leaf() ) ++count;
  }
  BOOST_CHECK_EQUAL( count, 1u );
}

BOOST_AUTO_TEST_CASE( UnableToInsert ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  insert( uc, "hoge", hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 16u, 16u ), std::equal_to< std::string >{} );
  insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 17u, 17u ), std::equal_to< std::string >{} );
  unsigned int count = 0u;
  for( const auto &[key,value]: uc ) {
    if( value.is_leaf() ) ++count;
  }
  BOOST_CHECK_EQUAL( count, 1u );
}

