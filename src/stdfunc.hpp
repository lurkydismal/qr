///////////////
/// @file stdfunc.hpp
/// @brief Declarations of many useful functions and lambdas related to replace standart if needed.
///////////////
#pragma once

#ifdef _WIN32
#include <windows.h>                // PVOID GetCurrentThread uint_fast32_t WriteConsoleA GetStdHandle HWND
// #pragma comment( lib, "user32" )
#else
#include <unistd.h>
#include <stdio.h>
#endif
#include <stdint.h>

#ifndef INT8_MIN
#define INT8_MIN -128
#endif

///////////////
/// @brief Pass from Python, same as NULL.
///////////////
#define Pass (void*)0

///////////////
/// @brief Inline function that converts \c bool to string.
/// @param[in] _boolean Boolean value to convert to string.
/// @return Constant char pointer to text. <b>( OK || Failed )</b>
///////////////
inline const char* const boolToString( bool _boolean ) {
    //! <b>[return]</b>
    /// End of function.
    /// @code{.cpp}
    return ( _boolean ? "OK" : "Failed" );
    /// @endcode
    //! <b>[return]</b>
}

///////////////
/// @brief Template function that print text to console.
/// @details Uses \c WriteConsoleA from \c Windows.h
/// @param[in] _text \c Text.
///////////////
template< const uint32_t _lengthOfText >
void print( const char (&_text)[ _lengthOfText ] ) {
    //! <b>[print]</b>
    /// Write text to console
    /// @code{.cpp}
    #ifdef _WIN32
    WriteConsoleA(
        GetStdHandle( STD_OUTPUT_HANDLE ), // Console handle
        _text,                             // Pointer to text
        _lengthOfText,                     // Length of text
        NULL,                              // Buffer to write value of characters written
        NULL                               // Reserved. Must be NULL
    );
    #else
    for ( uint32_t _symbolIndex = 0; _symbolIndex < _lengthOfText; _symbolIndex++ ) {
        putc( _text[ _symbolIndex ], stdout );
    }
    #endif
    /// @endcode
    //! <b>[print]</b>
}

///////////////
/// @brief Function that pop out element of array.
/// @details Can pop out element of any type, but array should be of the same type. Pop out first element with needed value.
/// @param[in] _array Elements array to pop from.
/// @param[in] _elementToPop Element to pop.
///////////////
template < class _typeOfArray, const uint32_t _lengthOfArray >
void pop( _typeOfArray (&_array)[ _lengthOfArray ], const _typeOfArray _elementToPop ) {
    //! <b>[for_each]</b>
    /// For each element of _array as index.
    /// @code{.cpp}
    for ( uint32_t _elementOfArray = 0; _elementOfArray < _lengthOfArray; _elementOfArray++ ) {
    /// @endcode
        //! <b>[compare]</b>
        /// Comparison each element of _array to needed value.
        /// @code{.cpp}
        if ( _array[ _elementOfArray ] == _elementToPop ) {
        /// @endcode
            //! <b>[pop]</b>
            /// Moving values from next index to current.
            /// @code{.cpp}
            while ( _elementOfArray < ( _lengthOfArray - 1 ) ) {
                _array[ _elementOfArray ] = _array[ _elementOfArray + 1 ];

                _elementOfArray++;
            }
            /// @endcode

            /// Replace the last element of _array with NULL value.
            /// @code{.cpp}
            _array[ _elementOfArray ] = (_typeOfArray)NULL;
            /// @endcode
            //! <b>[pop]</b>

            //! <b>[explicit]</b>
            /// Explicit end of loop.
            /// @code{.cpp}
            break;
            /// @endcode
            //! <b>[explicit]</b>
        }
        //! <b>[compare]</b>
    }
    //! <b>[for_each]</b>
}

uint_fast32_t lengthOfInt( long _number );

uint_fast32_t stringLength( char* _string );

void print( const char* _text, const uint32_t _lengthOfText );

int Pow( int32_t _number, uint32_t _exp );

uint32_t Rand( void );

void SRand( unsigned long _seed );

void clearConsole( void );

#ifdef _WIN32
bool LMC( HWND _windowHandle, const uint32_t _coordX, const uint32_t _coordY );
#endif

void* Malloc( uint32_t _numberOfBytes );

void Free( void* _firstbyte );

void Memcpy( void* _destination, void* _source, size_t _numberOfBytes );

char* Ltoa( long _number, char* _cString );

char* Ltoa( long _number, char* _cString, int32_t _base );

///////////////
/// @brief Function that find duplicate number in integer array.
/// @details Robert W. Floyd's tortoise and hare algorithm moves two pointers at different speeds through the sequence of values until they both point to equal values.
/// @param[in] _numbers Array of integer values.
/// @return Duplicate number or first number in array, if there is no duplicate.
///////////////
template< const uint32_t _lengthOfArray >
int32_t findDuplicate( int32_t (&_numbers)[ _lengthOfArray ] ) {
    //! <b>[declare]</b>
    /// Two variables that contains first element of array
    /// @code{.cpp}
    int32_t l_tortoise = _numbers[ 0 ];
    int32_t l_hare     = _numbers[ 0 ];
    /// @endcode
    //! <b>[declare]</b>

    //! <b>[intersection]</b>
    /// Find the intersection point of the two runners.
    /// @code{.cpp}
    do {
        l_tortoise = _numbers[ l_tortoise ];
        l_hare     = _numbers[ _numbers[ l_hare ] ];
    } while ( l_tortoise != l_hare );
    /// @endcode
    //! <b>[intersection]</b>

    //! <b>[entrance]</b>
    /// Find the "entrance" to the cycle.
    /// @code{.cpp}
    l_tortoise = _numbers[ 0 ];

    while ( l_tortoise != l_hare ) {
        l_tortoise = _numbers[ l_tortoise ];
        l_hare     = _numbers[ l_hare ];
    }
    /// @endcode
    //! <b>[entrance]</b>

    //! <b>[return]</b>
    /// Parentheses with whitespaces means what we could change the return value in the asked place without pitfalls.
    /// @code{.cpp}
    return ( l_hare );
    /// @endcode
    //! <b>[return]</b>
}