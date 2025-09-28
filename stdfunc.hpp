#pragma once

#include <sys/types.h>

#include <cstddef>
#include <cstdint>

#include "random.hpp"

// Function attributes
#define FORCE_INLINE [[gnu::always_inline]] inline
#define NOINLINE [[gnu::noinline, gnu::used]]
#define NO_RETURN [[gnu::noreturn]]
#define EXPORT extern "C"

#define randomValueFromContainer( _array ) \
    ( _array[ random::randomNumber() % arrayLength( _array ) ] )

#define arrayLength( _array ) ( sizeof( _array ) / sizeof( _array[ 0 ] ) )

#define FOR( _type, _array )       \
    for ( _type _element = _array; \
          _element < ( _array + arrayLength( _array ) ); _element++ )

namespace stdfunc {

// Utility functions ( no side-effects )
FORCE_INLINE constexpr auto lengthOfNumber( size_t _number ) -> size_t {
    size_t l_length = 0;

    do {
        l_length++;

        _number /= 10;
    } while ( _number );

    return ( l_length );
}

FORCE_INLINE constexpr auto power( size_t _base, uint8_t _exponent ) -> size_t {
    size_t l_returnValue = 1;

    for ( ;; ) {
        if ( _exponent & 1 ) {
            l_returnValue *= _base;
        }

        _exponent >>= 1;

        if ( !_exponent ) {
            break;
        }

        _base *= _base;
    }

    return ( l_returnValue );
}

constexpr void convertNumberToString( char* _buffer,
                                      size_t _number,
                                      size_t _lengthOfNumber ) {
    for ( ssize_t _characterIndex = ( _lengthOfNumber - 1 );
          _characterIndex >= 0; _characterIndex-- ) {
        _buffer[ _characterIndex ] = static_cast< char >(
            ( ( ( _number /
                  static_cast< size_t >( power(
                      10, ( _lengthOfNumber - _characterIndex - 1 ) ) ) ) %
                10 ) +
              '0' ) );
    }
}

} // namespace stdfunc
