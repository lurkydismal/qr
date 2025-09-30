#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include "stdfunc.hpp"

namespace random {

[[nodiscard]] constexpr auto number() -> uint32_t {
    constexpr size_t l_compilationTimeAsSeed =
        ( __TIME__[ 0 ] + __TIME__[ 1 ] + __TIME__[ 3 ] + __TIME__[ 4 ] +
          __TIME__[ 6 ] + __TIME__[ 7 ] );

    static uint32_t l_seed = l_compilationTimeAsSeed;

    l_seed ^= ( l_seed << 13 );
    l_seed ^= ( l_seed >> 17 );
    l_seed ^= ( l_seed << 5 );

    return ( l_seed );
}

[[nodiscard]] FORCE_INLINE constexpr auto value( auto* _container,
                                                 size_t _length ) -> auto& {
    return ( _container[ number() % _length ] );
}

template < typename T, size_t N >
[[nodiscard]] FORCE_INLINE constexpr auto value(
    const std::array< T, N >& _container ) -> auto& {
    return ( value( _container.data(), _container.size() ) );
}

template < typename T >
[[nodiscard]] FORCE_INLINE constexpr auto value(
    std::span< const T > _container ) -> auto& {
    return ( _value( _container.data(), _container.size() ) );
}

} // namespace random
