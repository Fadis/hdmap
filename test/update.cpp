#define BOOST_TEST_MODULE update
#define BOOST_TEST_DYN_LINK

#include <hdmap/hdmap.hpp>
#include <cstdint>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Update ) {
  hdmap::standard_underlying_container_t< std::uint32_t, std::string, 2u > uc;
  hdmap::detail::insert( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 2u >( 6u, 0x0u, 0x0u ), std::equal_to< std::string >{} );
  {
    unsigned int count = 0u;
    for( const auto &[key,value]: uc ) {
      if( value.is_leaf() ) ++count;
    }
    BOOST_CHECK_EQUAL( count, 1u );
  }
  {
    auto found = hdmap::detail::find( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 1u, 2u ) );
    BOOST_CHECK( found != uc.end() );
    BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 6u, 0u, 0u ) ) );
    BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
    BOOST_CHECK_EQUAL( found->second.get_data(), "fuga" );
  }
  {
    auto [update_iterator,affected] = hdmap::detail::update( uc, "hoge", hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 256u ), std::equal_to< std::string >{} );
    {
      unsigned int count = 0u;
      for( const auto &[key,value]: affected ) {
        if( value.is_leaf() ) ++count;
      }
      BOOST_CHECK_EQUAL( count, 1u );
    }
    {
      unsigned int count = 0u;
      for( const auto &[key,value]: uc ) {
        if( value.is_leaf() ) ++count;
      }
      BOOST_CHECK_EQUAL( count, 76u );
    }
    {
      auto found = hdmap::detail::find( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 1u, 2u ) );
      BOOST_CHECK( found != uc.end() );
      BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 4u, 0u, 0u ) ) );
      BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
      BOOST_CHECK_EQUAL( found->second.get_data(), "fuga" );
    }
    {
      auto found = hdmap::detail::find( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 257u ) );
      BOOST_CHECK( found != uc.end() );
      BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 256u ) ) );
      BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
      BOOST_CHECK_EQUAL( found->second.get_data(), "hoge" );
    }
    {
      auto found = hdmap::detail::find( affected, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 1u, 2u ) );
      BOOST_CHECK( found == affected.end() );
    }
    {
      auto found = hdmap::detail::find( affected, hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 257u ) );
      BOOST_CHECK( found != affected.end() );
      BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 256u ) ) );
      BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
      BOOST_CHECK_EQUAL( found->second.get_data(), "fuga" );
    }
  }
  {
    auto [update_iterator,affected] = hdmap::detail::update( uc, "piyo", hdmap::detail::to_key< std::uint32_t, 2u >( 2u, 256u, 256u ), std::equal_to< std::string >{} );
    {
      unsigned int count = 0u;
      for( const auto &[key,value]: affected ) {
        if( value.is_leaf() ) ++count;
      }
      BOOST_CHECK_EQUAL( count, 16u );
    }
    {
      unsigned int count = 0u;
      for( const auto &[key,value]: uc ) {
        if( value.is_leaf() ) ++count;
      }
      BOOST_CHECK_EQUAL( count, 61u );
    }
    {
      auto found = hdmap::detail::find( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 1u, 2u ) );
      BOOST_CHECK( found != uc.end() );
      BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 4u, 0u, 0u ) ) );
      BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
      BOOST_CHECK_EQUAL( found->second.get_data(), "fuga" );
    }
    {
      auto found = hdmap::detail::find( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 257u ) );
      BOOST_CHECK( found != uc.end() );
      BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 2u, 256u, 256u ) ) );
      BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
      BOOST_CHECK_EQUAL( found->second.get_data(), "piyo" );
    }
    {
      auto found = hdmap::detail::find( affected, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 1u, 2u ) );
      BOOST_CHECK( found == affected.end() );
    }
    {
      auto found = hdmap::detail::find( affected, hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 257u ) );
      BOOST_CHECK( found != affected.end() );
      BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 256u ) ) );
      BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
      BOOST_CHECK_EQUAL( found->second.get_data(), "hoge" );
    }
  }
  {
    auto [update_iterator,affected] = hdmap::detail::update( uc, "fuga", hdmap::detail::to_key< std::uint32_t, 2u >( 3u, 256u, 256u ), std::equal_to< std::string >{} );
    {
      unsigned int count = 0u;
      for( const auto &[key,value]: affected ) {
        if( value.is_leaf() ) ++count;
      }
      BOOST_CHECK_EQUAL( count, 16u );
    }
    {
      unsigned int count = 0u;
      for( const auto &[key,value]: uc ) {
        if( value.is_leaf() ) ++count;
      }
      BOOST_CHECK_EQUAL( count, 1u );
    }
    {
      auto found = hdmap::detail::find( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 1u, 2u ) );
      BOOST_CHECK( found != uc.end() );
      BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 6u, 0u, 0u ) ) );
      BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
      BOOST_CHECK_EQUAL( found->second.get_data(), "fuga" );
    }
    {
      auto found = hdmap::detail::find( uc, hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 257u ) );
      BOOST_CHECK( found != uc.end() );
      BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 6u, 0u, 0u ) ) );
      BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
      BOOST_CHECK_EQUAL( found->second.get_data(), "fuga" );
    }
    {
      auto found = hdmap::detail::find( affected, hdmap::detail::to_key< std::uint32_t, 2u >( 0u, 1u, 2u ) );
      BOOST_CHECK( found == affected.end() );
    }
    {
      auto found = hdmap::detail::find( affected, hdmap::detail::to_key< std::uint32_t, 2u >( 1u, 256u, 257u ) );
      BOOST_CHECK( found != affected.end() );
      BOOST_CHECK_EQUAL( found->second.get_range(), ( hdmap::detail::to_key< std::uint32_t, 2u >( 2u, 256u, 256u ) ) );
      BOOST_CHECK_EQUAL( found->second.is_leaf(), true );
      BOOST_CHECK_EQUAL( found->second.get_data(), "piyo" );
    }
  }
}

