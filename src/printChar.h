///////////////
/// @file printChar.h
/// @brief Declarations some of print functions which related to be analog of standart one
///////////////
#ifndef _PRINTCHAR
#define _PRINTCHAR

#ifndef _WIN32
#include <stdio.h>
#endif

///////////////
/// @brief Function that print out ASCII value as character to console.
/// @details Uses \c WriteConsoleA from \c Windows.h
/// @param[in] _decimalCharacter ASCII value.
///////////////
#ifdef _WIN32
extern "C" void printChar( const unsigned int _decimalCharacter ) noexcept;
// extern "C" void __declspec( naked ) printCharA( const unsigned int _decimalCharacter ) noexcept {
//     __asm{
//         push _decimalCharacter
//         call _printChar
//     };
//     asm(
//         "push %0\n\t"
//         "call _printChar\n\t"
//         : "0" ( _decimalCharacter )
//     );
// }
#else
extern "C" void printChar( const unsigned int _decimalCharacter ) noexcept {
    putc( _decimalCharacter, stdout );
}
#endif

///////////////
/// @brief Function that print out array of ASCII values as characters to console.
/// @details Uses \c WriteConsoleA from \c Windows.h
/// @param[in] _lengthOfText Length of text.
/// @param[in] _decimalText Array if ASCII values.
///////////////
#ifdef _WIN32
extern "C" void printText( const unsigned int _lengthOfText, const unsigned int _decimalText[] ) noexcept;
#else
extern "C" void printText( const unsigned int _lengthOfText, const unsigned int _decimalText[] ) noexcept {
    for ( unsigned int _symbolIndex = 0; _symbolIndex < _lengthOfText; _symbolIndex++ ) {
        printChar( _decimalText[ _symbolIndex ] );
    }
}
#endif

#endif