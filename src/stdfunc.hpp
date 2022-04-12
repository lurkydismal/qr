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
#include <stddef.h>

///////////////
/// @brief Pass from Python, same as NULL.
///////////////
#define Pass (void*)0

///////////////
/// @brief Inline function that converts \c bool to string.
/// @param[in] _boolean Boolean value to convert to string.
/// @return Constant char pointer to text. <b>( OK || Failed )</b>
///////////////
inline const char* const boolToString( bool _boolean ) noexcept {
    //! <b>[return]</b>
    /// End of function.
    /// @code{.cpp}
    return ( _boolean ? "OK" : "Failed" );
    /// @endcode
    //! <b>[return]</b>
}

///////////////
/// @brief Inline function that find length of an integer number.
/// @details Can get length of negative and positive number.
/// @param[in] _number Number to get length from.
/// @return Length of number.
///////////////
inline uint_fast32_t lengthOfInt( long _number ) noexcept {
    //! <b>[declare]</b>
    /// Length of _number.
    /// @code{.cpp}
    uint_fast32_t l_length = 0;
    /// @endcode
    //! <b>[declare]</b>

    //! <b>[count_length]</b>
    /// Divide by 10 while value not equal to 0.
    /// @code{.cpp}
    do {
        l_length++;
        _number /= 10;
    } while ( _number != 0 );
    /// @endcode
    //! <b>[count_length]</b>

    //! <b>[return]</b>
    /// End of function.
    /// @code{.cpp}
    return ( l_length );
    /// @endcode
    //! <b>[return]</b>
}

///////////////
/// @brief Template function that print text to console.
/// @details Uses \c WriteConsoleA from \c Windows.h
/// @param[in] _text \c Text.
///////////////
template< const unsigned int _lengthOfText >
void print( const char (&_text)[ _lengthOfText ] ) noexcept {
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
    for ( unsigned int _symbolIndex = 0; _symbolIndex < _lengthOfText; _symbolIndex++ ) {
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
template < class _typeOfArray, const unsigned int _lengthOfArray >
void pop( _typeOfArray (&_array)[ _lengthOfArray ], const _typeOfArray _elementToPop ) {
    //! <b>[for_each]</b>
    /// For each element of _array as index.
    /// @code{.cpp}
    for ( unsigned int _elementOfArray = 0; _elementOfArray < _lengthOfArray; _elementOfArray++ ) {
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

void print( const char* _text, const unsigned int _lengthOfText ) noexcept;

int Pow( int _number, unsigned int _exp );

unsigned int Rand( void );

void SRand( unsigned long _seed ) noexcept;

void clearConsole( void ) noexcept;

#ifdef _WIN32
bool LMC( HWND _windowHandle, const unsigned int _coordX, const unsigned int _coordY ) noexcept;
#endif

void* Malloc( unsigned int _numberOfBytes );

void Free( void* _firstbyte );

void Memcpy( void* _destination, void* _source, size_t _numberOfBytes ) noexcept;

char* Ltoa( long _number, char* _cString );

char* Ltoa( long _number, char* _cString, int _base );

// int findDuplicate( std::vector< int > _numbers_v );