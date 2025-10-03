#pragma once

#include <sys/types.h>

#include <cstddef>
#include <cstdint>
#include <ranges>

// Function attributes
#define FORCE_INLINE [[gnu::always_inline]] inline
#define NOINLINE [[gnu::noinline, gnu::used]]
#define NO_RETURN [[gnu::noreturn]]
#define EXPORT extern "C"

namespace stdfunc {

[[nodiscard]] FORCE_INLINE constexpr auto lengthOfNumber( size_t _number )
    -> size_t {
    return ( ( _number < 10 ) ? ( 1 ) : ( ( _number < 100 ) ? ( 2 ) : ( 3 ) ) );
}

[[nodiscard]] FORCE_INLINE constexpr auto power( size_t _base,
                                                 uint8_t _exponent ) -> size_t {
    if ( _exponent == 0 ) {
        return ( 1 );
    }

    return ( _base * power( _base, _exponent - 1 ) );
}

// TODO: Improve
constexpr void convertNumberToString( char* _buffer,
                                      size_t _number,
                                      ssize_t _lengthOfNumber ) {
    for ( const auto _characterIndex :
          std::views::iota( ssize_t{}, _lengthOfNumber ) |
              std::views::reverse ) {
        _buffer[ _characterIndex ] = static_cast< char >(
            ( ( ( _number /
                  power( 10, ( _lengthOfNumber - _characterIndex - 1 ) ) ) %
                10 ) +
              '0' ) );
    }
}

} // namespace stdfunc
