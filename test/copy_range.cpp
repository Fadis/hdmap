#define BOOST_TEST_MODULE copy_range
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Trimmed ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  hdmap::detail::insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 2u >( 6u, 0x0u, 0x0u ), std::equal_to< std::string >{} );
  hdmap::detail::update( uc, "hoge", hdmap::rectangle< std::uint64_t, 2u >{ hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 256u ), hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 257u ) }, std::equal_to< std::string >{} );
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > copied;
  hdmap::detail::copy( uc, hdmap::rectangle< std::uint64_t, 2u >{ hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 254u ), hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 258u ) }, copied, std::equal_to< std::string >{}, true );
  unsigned int count = 0u;
  std::vector< std::uint64_t > range;
  std::vector< std::string > data;
  for( auto &[r,value]: std::views::all( copied )|hdmap::views::leaf ) {
    ++count;
    data.push_back( value.get_data() );
    range.push_back( value.get_range() );
  }
  BOOST_CHECK_EQUAL( count, 4u );
  BOOST_CHECK_EQUAL( data[ 0 ], "fuga" );
  BOOST_CHECK_EQUAL( data[ 1 ], "hoge" );
  BOOST_CHECK_EQUAL( data[ 2 ], "fuga" );
  BOOST_CHECK_EQUAL( data[ 3 ], "fuga" );
  BOOST_CHECK_EQUAL( range[ 0 ], ( hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 281u, 257u ) ) );
  BOOST_CHECK_EQUAL( range[ 1 ], ( hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 281u, 256u ) ) );
  BOOST_CHECK_EQUAL( range[ 2 ], ( hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 281u, 255u ) ) );
  BOOST_CHECK_EQUAL( range[ 3 ], ( hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 281u, 254u ) ) );
}

BOOST_AUTO_TEST_CASE( NotTrimmed ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  hdmap::detail::insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 2u >( 6u, 0x0u, 0x0u ), std::equal_to< std::string >{} );
  hdmap::detail::update( uc, "hoge", hdmap::rectangle< std::uint64_t, 2u >{ hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 256u ), hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 257u ) }, std::equal_to< std::string >{} );
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > copied;
  hdmap::detail::copy( uc, hdmap::rectangle< std::uint64_t, 2u >{ hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 281u, 254u ), hdmap::detail::to_key< std::uint64_t, 2u >( 0u, 282u, 258u ) }, copied, std::equal_to< std::string >{}, false );
  unsigned int count = 0u;
  std::vector< std::uint64_t > range;
  std::vector< std::string > data;
  for( auto &[r,value]: std::views::all( copied )|hdmap::views::leaf ) {
    ++count;
    data.push_back( value.get_data() );
    range.push_back( value.get_range() );
  }
  BOOST_CHECK_EQUAL( count, 3u );
  BOOST_CHECK_EQUAL( data[ 0 ], "fuga" );
  BOOST_CHECK_EQUAL( data[ 1 ], "hoge" );
  BOOST_CHECK_EQUAL( data[ 2 ], "fuga" );
  BOOST_CHECK_EQUAL( range[ 0 ], ( hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 281u, 257u ) ) );
  BOOST_CHECK_EQUAL( range[ 1 ], ( hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 281u, 256u ) ) );
  BOOST_CHECK_EQUAL( range[ 2 ], ( hdmap::detail::to_key< std::uint32_t, 2u >( 4u, 281u, 255u ) ) );
}

