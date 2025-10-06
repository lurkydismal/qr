#pragma once

#include <cstddef>
#include <cstdint>

#include "stdfunc.hpp"

namespace random {

// Seconds from midnight
constexpr size_t g_compilationTimeAsSeed =
    ( ( ( ( ( __TIME__[ 0 ] - '0' ) * 10 ) + ( __TIME__[ 1 ] - '0' ) ) *
        3600 ) +
      ( ( ( ( __TIME__[ 3 ] - '0' ) * 10 ) + ( __TIME__[ 4 ] - '0' ) ) * 60 ) +
      ( ( ( __TIME__[ 6 ] - '0' ) * 10 ) + ( __TIME__[ 7 ] - '0' ) ) );

// FNV-1A
[[nodiscard]] consteval auto generateHash(
    const std::span< const uint8_t >& _data ) -> uint32_t {
    // Offset basis
    uint32_t l_hash = 0x811C9DC5;

    for ( const uint8_t _item : _data ) {
        l_hash ^= _item;

        // Prime
        l_hash *= 0x1000193;
    }

    return ( l_hash );
}

[[nodiscard]] consteval auto number( uint32_t _seed ) -> uint32_t {
    const auto l_data = std::bit_cast< std::array< uint8_t, 4 > >( _seed );

    _seed = generateHash( l_data );

    _seed ^= ( _seed << 13 );
    _seed ^= ( _seed >> 17 );
    _seed ^= ( _seed << 5 );

    return ( _seed );
}

[[nodiscard]] auto number() -> uint32_t {
    static uint32_t l_seed = g_compilationTimeAsSeed;

    l_seed ^= ( l_seed << 13 );
    l_seed ^= ( l_seed >> 17 );
    l_seed ^= ( l_seed << 5 );

    return ( l_seed );
}

template < typename T, size_t N >
[[nodiscard]] FORCE_INLINE consteval auto value(
    const std::array< T, N >& _container,
    uint32_t _seed ) -> auto& {
    return ( _container[ number( _seed ) % _container.size() ] );
}

template < typename T, size_t N >
[[nodiscard]] FORCE_INLINE auto value( const std::array< T, N >& _container )
    -> auto& {
    return ( _container[ number() % _container.size() ] );
}

} // namespace random
