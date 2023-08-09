///////////////
/// @file stdfunc.c
/// @brief Definition of many useful functions related to replace standard if needed.
///////////////
#ifndef _WIN32

#include <stdio.h>

#endif // _WIN32

#include <stdfunc.h>

///////////////
/// @brief Inline function that converts \c bool to string.
/// @param[in] _boolean Boolean value to convert to string.
/// @return Char pointer to text. <b>( OK || Failed )</b>
///////////////
const char* const boolToString( bool _boolean ) {
    //! <b>[return]</b>
    /// End of function.
    /// @code{.c}
    return ( _boolean ? "OK" : "Failed" );
    /// @endcode
    //! <b>[return]</b>
}

///////////////
/// @brief Function that pop out element of array.
/// @details Can pop out element of any type, but array should be of the same type. Pop out first element with needed value.
/// @param[in] _array Elements array to pop from.
/// @param[in] _lengthOfArray Array length.
/// @param[in] _elementToPop Element to pop.
/// @result Array without popped element.
///////////////
uint32_t* pop( uint32_t* _array, uint32_t _lengthOfArray, const uint32_t _elementToPop ) {
    //! <b>[for_each]</b>
    /// For each element of _array as index.
    /// @code{.c}
    for ( uint32_t _elementOfArray = 0; _elementOfArray < _lengthOfArray; _elementOfArray++ ) {
    /// @endcode
        //! <b>[compare]</b>
        /// Comparison each element of _array to needed value.
        /// @code{.c}
        if ( _array[ _elementOfArray ] == _elementToPop ) {
        /// @endcode
            //! <b>[pop]</b>
            /// Moving values from next index to current.
            /// @code{.c}
            while ( _elementOfArray < ( _lengthOfArray - 1 ) ) {
                _array[ _elementOfArray ] = _array[ _elementOfArray + 1 ];

                _elementOfArray++;
            }
            /// @endcode

            /// Replace the last element of _array with 0.
            /// @code{.c}
            _array[ _elementOfArray ] = 0;
            /// @endcode
            //! <b>[pop]</b>

            //! <b>[explicit]</b>
            /// Explicit end of loop.
            /// @code{.c}
            break;
            /// @endcode
            //! <b>[explicit]</b>
        }
        //! <b>[compare]</b>
    }
    //! <b>[for_each]</b>

    //! <b>[return]</b>
    /// End of function.
    /// @code{.c}
    return ( _array );
    /// @endcode
    //! <b>[return]</b>
}

///////////////
/// @brief Function that find length of an integer number.
/// @details Can get length of negative and positive number.
/// @param[in] _number Number to get length from.
/// @return Number length.
///////////////
uint_fast32_t lengthOfInt( long _number ) {
    //! <b>[declare]</b>
    /// Length of _number.
    /// @code{.c}
    uint_fast32_t l_length = 0;
    /// @endcode
    //! <b>[declare]</b>

    //! <b>[count_length]</b>
    /// Divide by 10 while value not equal to 0.
    /// @code{.c}
    do {
        l_length++;
        _number /= 10;
    } while ( _number != 0 );
    /// @endcode
    //! <b>[count_length]</b>

    //! <b>[return]</b>
    /// End of function.
    /// @code{.c}
    return ( l_length );
    /// @endcode
    //! <b>[return]</b>
}

///////////////
/// @brief Function that find length of an string.
/// @param[in] _string String pointer to get length from.
/// @return String length.
///////////////
uint_fast32_t lengthOfString( char* _string ) {
    //! <b>[declare]</b>
    /// Last symbol from _string.
    /// @code{.c}
    char* l_temp_string = _string;
    /// @endcode
    //! <b>[declare]</b>

    //! <b>[count_length]</b>
    /// Go through _string upon null-terminator.
    /// @code{.c}
    while ( *l_temp_string != '\0' ) {
        l_temp_string++;
    }
    /// @endcode
    //! <b>[count_length]</b>

    //! <b>[return]</b>
    /// End of function.
    /// @code{.c}
    return ( l_temp_string - _string + 1 );
    /// @endcode
    //! <b>[return]</b>
}

//! <b>[declare]</b>
/// \c seed that is in use by \c Rand and \c SRand functions.
/// @code{.c}
static unsigned long g_seed = 1;
/// @endcode
//! <b>[declare]</b>

///////////////
/// @brief Function that print out char pointer text to console.
/// @details Uses \c WriteConsoleA from \c Windows.h
/// @param[in] _text Pointer to text.
/// @param[in] _lengthOfText Length of text.
///////////////
void print( const char* _text, const uint32_t _lengthOfText ) {
    //! <b>[print]</b>
    /// Write text to console
    /// @code{.c}

#ifdef _WIN32

    WriteConsoleA(
        GetStdHandle( STD_OUTPUT_HANDLE ), // Console handle
        _text,                             // Pointer to text
        _lengthOfText,                     // Length of text
        NULL,                              // Buffer to write value of characters written
        NULL                               // Reserved. Must be NULL
    );

#else // _WIN32

    for ( uint32_t _symbolIndex = 0; _symbolIndex < _lengthOfText; _symbolIndex++ ) {
        putchar( _text[ _symbolIndex ] );
    }

#endif // _WIN32

    /// @endcode
    //! <b>[print]</b>
}

///////////////
/// @brief Function that exponentiation integer number.
/// @details Recursive function.
/// @param[in] _number Number to power.
/// @param[in] _exp Exponentiation value.
/// @return Exponentiation of number.
///////////////
int32_t Pow( int32_t _number, uint32_t _exp ) {
    if ( _exp == 1 ) {
        //! <b>[return]</b>
        /// End of function.
        /// @code{.c}
        return (1);
        /// @endcode
        //! <b>[return]</b>

    } else {
        //! <b>[return]</b>
        /// Recursive multiply.
        /// @code{.c}
        return ( _number * Pow( _number, _exp-- ) );
        /// @endcode
        //! <b>[return]</b>
    }
}

///////////////
/// @brief Function that generate random number from the \c seed.
/// @details Use \c SRand() to set the seed.
/// @return Generated random number.
///////////////
uint32_t Rand( void ) {
    //! <b>[random]</b>
    /// Generating random number from the \c seed.
    /// @code{.c}
    g_seed = g_seed * (int32_t)16807 % (uint32_t)0x7fffffff;
    /// @endcode
    //! <b>[random]</b>

    //! <b>[return]</b>
    /// End of function.
    return ( g_seed );
    //! <b>[return]</b>
}

///////////////
/// @brief Function that set the \c seed.
/// @details Use \c Rand() to generate random number from the \c seed.
/// @param[in] l_seed Seed to set.
///////////////
void SRand( unsigned long l_seed ) {
    //! <b>[change]</b>
    /// Setting the \c seed.
    /// @code{.c}
    g_seed = l_seed;
    /// @endcode
    //! <b>[change]</b>
}

///////////////
/// @brief Function that clear the console out screen.
/// @details Filling console length with empty text.
///////////////
void clearConsole( void ) {

#ifdef _WIN32

    //! <b>[declare]</b>
    /// Getting and declaring coordinates and console handle where to write.
    /// @code{.c}
    COORD l_topLeft = {
        0,              // X
        0               // Y
    };
    CONSOLE_SCREEN_BUFFER_INFO l_consoleScreenInfo;
    HANDLE l_consoleHandle = GetStdHandle( STD_OUTPUT_HANDLE );
    /// @endcode
    //! <b>[declare]</b>

    //! <b>[length]</b>
    /// Getting length of console.
    /// @code{.c}
    GetConsoleScreenBufferInfo( l_consoleHandle, &l_consoleScreenInfo );
    DWORD l_numberOfWrittenCharacters, l_lengthOfText = l_consoleScreenInfo.dwSize.X * l_consoleScreenInfo.dwSize.Y;
    /// @endcode
    //! <b>[length]</b>

    //! <b>[print]</b>
    /// Write empty text to console.
    /// @code{.c}
    FillConsoleOutputCharacter( l_consoleHandle, ' ', l_lengthOfText, l_topLeft, &l_numberOfWrittenCharacters );
    FillConsoleOutputAttribute( l_consoleHandle, l_consoleScreenInfo.wAttributes, l_lengthOfText, l_topLeft, &l_numberOfWrittenCharacters );
    /// @endcode
    //! <b>[print]</b>

    //! <b>[reset]</b>
    /// Reset cursor position in console.
    /// @code{.c}
    SetConsoleCursorPosition( l_consoleHandle, l_topLeft );
    /// @endcode
    //! <b>[reset]</b>

#else // _WIN32

    const char l_magicText[] = "\033[1;1H\033[2J";

    for ( uint32_t _symbolIndex = 0; _symbolIndex < sizeof( l_magicText ); _symbolIndex++ ) {
        putchar( l_magicText[ _symbolIndex ] );
    }

#endif // _WIN32

}

///////////////
/// @brief Function that find duplicate number in integer array.
/// @details Robert W. Floyd's tortoise and hare algorithm moves two pointers at different speeds through the sequence of values until they both point to equal values.
/// @param[in] _numbers Array of integer values.
/// @return Duplicate number or first number in array, if there is no duplicate.
///////////////
int32_t findDuplicate( int32_t* _numbers ) {
    //! <b>[declare]</b>
    /// Two variables that contains first element of array
    /// @code{.c}
    int32_t l_tortoise = _numbers[ 0 ];
    int32_t l_hare     = _numbers[ 0 ];
    /// @endcode
    //! <b>[declare]</b>

    //! <b>[intersection]</b>
    /// Find the intersection point of the two runners.
    /// @code{.c}
    do {
        l_tortoise = _numbers[ l_tortoise ];
        l_hare     = _numbers[ _numbers[ l_hare ] ];
    } while ( l_tortoise != l_hare );
    /// @endcode
    //! <b>[intersection]</b>

    //! <b>[entrance]</b>
    /// Find the "entrance" to the cycle.
    /// @code{.c}
    l_tortoise = _numbers[ 0 ];

    while ( l_tortoise != l_hare ) {
        l_tortoise = _numbers[ l_tortoise ];
        l_hare     = _numbers[ l_hare ];
    }
    /// @endcode
    //! <b>[entrance]</b>

    //! <b>[return]</b>
    /// Parentheses with whitespaces means what we could change the return value in the asked place without pitfalls.
    /// @code{.c}
    return ( l_hare );
    /// @endcode
    //! <b>[return]</b>
}
