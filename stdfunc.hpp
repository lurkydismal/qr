#pragma once

#include <sys/types.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>
#include <type_traits>

#include "random.hpp"

// Function attributes
#define FORCE_INLINE [[gnu::always_inline]] inline
#define NOINLINE [[gnu::noinline, gnu::used]]
#define NO_RETURN [[gnu::noreturn]]
#define EXPORT extern "C"

#define randomValueFromContainer( _array ) \
    ( _array[ random::randomNumber() % arrayLength( _array ) ] )

namespace stdfunc {

#define arrayLength( _array ) ( sizeof( _array ) / sizeof( _array[ 0 ] ) )

#define FOR( _type, _array )       \
    for ( _type _element = _array; \
          _element < ( _array + arrayLength( _array ) ); _element++ )

// Utility functions ( no side-effects )
auto lengthOfNumber( size_t _number ) -> size_t;
auto power( size_t _base, uint8_t _exponent ) -> size_t;
void convertNumberToString( char* _buffer,
                            size_t _number,
                            size_t _lengthOfNumber );

// C

auto lengthOfNumber( size_t _number ) -> size_t {
    size_t l_length = 0;

    do {
        l_length++;

        _number /= 10;
    } while ( _number );

    return ( l_length );
}

FORCE_INLINE auto power( size_t _base, uint8_t _exponent ) -> size_t {
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

void convertNumberToString( char* _buffer,
                            size_t _number,
                            size_t _lengthOfNumber ) {
    char* l_buffer = _buffer;

    for ( ssize_t _characterIndex = ( _lengthOfNumber - 1 );
          _characterIndex >= 0; _characterIndex-- ) {
        l_buffer[ _characterIndex ] =
            ( char )( ( ( _number /
                          ( size_t )( power( 10, ( _lengthOfNumber -
                                                   _characterIndex - 1 ) ) ) ) %
                        10 ) +
                      '0' );
    }
}

} // namespace stdfunc
