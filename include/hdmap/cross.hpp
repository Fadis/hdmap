#ifndef HDMAP_CROSS_HPP
#define HDMAP_CROSS_HPP

#include <hdmap/hdmap.hpp>

namespace hdmap {


template< typename ...Eq >
struct tuple_eq {
  tuple_eq( Eq ...eq_ ) : eq( eq_... ) {}
  template< typename L, typename R >
  auto operator()( const L &l, const R &r ) const -> std::enable_if_t<
    std::tuple_size_v< L > == sizeof...( Eq ) &&
    std::tuple_size_v< R > == sizeof...( Eq ),
    bool
  > {
    return comp< 0u >( l, r );
  }
private:
  template< unsigned int i, typename L, typename R >
  auto comp( const L&, const R& ) const -> std::enable_if_t< i == sizeof...( Eq ), bool > {
    return true;
  }
  template< unsigned int i, typename L, typename R >
  auto comp( const L &l, const R &r ) const -> std::enable_if_t< i != sizeof...( Eq ), bool > {
    if( std::get< i >( eq )( std::get< i >( l ), std::get< i >( r ) ) )
      return comp< i + 1u >( l, r );
    return false;
  }
  std::tuple< std::remove_cvref_t< Eq >... > eq;
};

template< typename ...Eq >
auto make_tuple_eq( Eq ...eq ) {
  return tuple_eq< Eq... >( eq... );
}

template<
  HDMap T,
  HDMap U
>
struct cross_result;
template<
  std::unsigned_integral LT,
  typename LU,
  std::regular_invocable< const LU&, const LU& > LEq,
  HDMapUnderlyingContainer LC,
  std::unsigned_integral RT,
  typename RU,
  std::regular_invocable< const RU&, const RU& > REq,
  HDMapUnderlyingContainer RC,
  unsigned int dims
>
struct cross_result<
  hdmap< LT, LU, dims, LEq, LC >,
  hdmap< RT, RU, dims, REq, RC >
> {
  using type = hdmap<
    std::common_type_t< LT, RT >,
    std::pair< LU, RU >,
    dims,
    tuple_eq< LEq, REq >,
    standard_underlying_container_t< std::common_type_t< LT, RT >, std::pair< LU, RU >, dims >
  >;
};

template< HDMap T, HDMap U >
using cross_result_t = typename cross_result< T, U >::type;

template< HDMap T, HDMap U >
auto cross( const T &l, const U &r ) {
  cross_result_t< T, U > out( make_tuple_eq( l.value_eq(), r.value_eq() ) );
  std::vector< typename U::value_type > temp;
  for( const auto &[lkey,lvalue]: l.leaves() ) {
    temp.clear();
    r.find( lkey, temp );
    for( const auto &[rkey,rvalue]: temp ) {
      out.update( rkey, std::make_pair( lvalue, rvalue ) );
    }
  }
  return out;
}
template< HDMap M >
auto cross(
  const M &l,
  const M &r,
  const typename M::rect_type &lrect,
  const typename M::rect_type &rrect
) {
  using T = detail::extract_key_type_t< typename M::rect_type >;
  constexpr static auto dims = detail::extract_dims_v< typename M::rect_type >;
  cross_result_t< M, M > out( make_tuple_eq( l.value_eq(), r.value_eq() ) );
  std::vector< typename M::value_type > l_temp;
  std::vector< typename M::value_type > r_temp;
  l.find( lrect, l_temp );
  auto diff = detail::sub_key< T, dims >( rrect.left_top, lrect.left_top );
  for( const auto &[lkey,lvalue]: l_temp ) {
    const typename M::rect_type shifted_lrect{
      detail::add_key< T, dims >( lkey.left_top, diff ),
      detail::add_key< T, dims >( lkey.right_bottom, diff )
    };
    r_temp.clear();
    r.find( shifted_lrect, r_temp );
    for( const auto &[rkey,rvalue]: r_temp ) {
      const typename M::rect_type shifted_rrect{
        detail::sub_key< T, dims >( rkey.left_top, rrect.left_top ),
        detail::sub_key< T, dims >( rkey.right_bottom, rrect.left_top )
      };
      out.update( shifted_rrect, std::make_pair( lvalue, rvalue ) );
    }
  }
  return out;
}

}
#endif

