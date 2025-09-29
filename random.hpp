#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include "stdfunc.hpp"

#define randomValueFromContainer( _array ) \
    ( _array[ random::random() % arrayLength( _array ) ] )

namespace random {

[[nodiscard]] constexpr auto random() -> size_t {
    constexpr size_t l_compilationTimeAsSeed =
        ( __TIME__[ 0 ] + __TIME__[ 1 ] + __TIME__[ 2 ] + __TIME__[ 3 ] +
          __TIME__[ 4 ] + __TIME__[ 5 ] );

    static uint32_t l_seed = l_compilationTimeAsSeed;

    l_seed ^= ( l_seed << 13 );
    l_seed ^= ( l_seed >> 17 );
    l_seed ^= ( l_seed << 5 );

    return ( l_seed );
}

template < typename T >
[[nodiscard]] FORCE_INLINE constexpr auto value( const T* _container,
                                                 size_t _length ) -> T {
    return ( _container[ random() % _length ] );
}

template < typename T, size_t N >
[[nodiscard]] FORCE_INLINE constexpr auto value(
    const std::array< T, N >& _container ) -> T {
    return ( value( _container.data(), _container.size() ) );
}

template < typename T, size_t N >
[[nodiscard]] FORCE_INLINE constexpr auto value(
    const std::array< const T, N >& _container ) -> T {
    return ( value( _container.data(), _container.size() ) );
}

template < typename T >
[[nodiscard]] FORCE_INLINE constexpr auto value(
    const std::span< const T >& _container ) -> T {
    return ( value( _container.data(), _container.size() ) );
}

} // namespace random
