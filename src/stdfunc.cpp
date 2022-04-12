///////////////
/// @file stdfunc.cpp
/// @brief Definition of many useful functions related to replace standart if needed.
///////////////

#ifndef _WIN32
#include <stdio.h>
#endif
#include "stdfunc.hpp"                     // windows.h

//! <b>[declare]</b>
/// \c seed that is in use by \c Rand and \c SRand functions.
/// @code{.cpp}
static unsigned long g_seed = 1;
/// @endcode
//! <b>[declare]</b>

///////////////
/// @brief Function that print out char pointer text to console.
/// @details Uses \c WriteConsoleA from \c Windows.h
/// @param[in] _text Pointer to text.
/// @param[in] _lengthOfText Length of text.
///////////////
void print( const char* _text, const unsigned int _lengthOfText ) noexcept {
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
/// @brief Function that exponentiation integer number.
/// @details Recursive function.
/// @param[in] _number Number to power.
/// @param[in] _exp Exponentiation value.
/// @return Exponentiation of number.
///////////////
int Pow( int _number, unsigned int _exp ) {
    if ( _exp == 1 ) {
        //! <b>[return]</b>
        /// End of function.
        /// @code{.cpp}
        return (1);
        /// @endcode
        //! <b>[return]</b>

    } else {
        //! <b>[return]</b>
        /// Recursive multiply.
        /// @code{.cpp}
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
unsigned int Rand( void ) {
    //! <b>[random]</b>
    /// Generating random number from the \c seed.
    /// @code{.cpp}
    g_seed = g_seed * (int)16807 % (unsigned int)0x7fffffff;
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
void SRand( unsigned long l_seed ) noexcept {
    //! <b>[change]</b>
    /// Setting the \c seed.
    /// @code{.cpp}
    g_seed = l_seed;
    /// @endcode
    //! <b>[change]</b>
}

///////////////
/// @brief Function that clear the console out screen.
/// @details Filling console length with empty text.
///////////////
void clearConsole( void ) noexcept {
    #ifdef _WIN32
    //! <b>[declare]</b>
    /// Getting and declaring coordinates and console handle where to write.
    /// @code{.cpp}
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
    /// @code{.cpp}
    GetConsoleScreenBufferInfo( l_consoleHandle, &l_consoleScreenInfo );
    DWORD l_numberOfWrittenCharacters, l_lengthOfText = l_consoleScreenInfo.dwSize.X * l_consoleScreenInfo.dwSize.Y;
    /// @endcode
    //! <b>[length]</b>

    //! <b>[print]</b>
    /// Write empty text to console.
    /// @code{.cpp}
    FillConsoleOutputCharacter( l_consoleHandle, ' ', l_lengthOfText, l_topLeft, &l_numberOfWrittenCharacters );
    FillConsoleOutputAttribute( l_consoleHandle, l_consoleScreenInfo.wAttributes, l_lengthOfText, l_topLeft, &l_numberOfWrittenCharacters );
    /// @endcode
    //! <b>[print]</b>

    //! <b>[reset]</b>
    /// Reset cursor position in console.
    /// @code{.cpp}
    SetConsoleCursorPosition( l_consoleHandle, l_topLeft );
    /// @endcode
    //! <b>[reset]</b>
    #else
    const char l_magicText[ 11 ] = "\e[1;1H\e[2J";
    for ( unsigned int _symbolIndex = 0; _symbolIndex < 11; _symbolIndex++ ) {
        putc( l_magicText[ _symbolIndex ], stdout );
    }
    #endif
}

///////////////
/// @brief Function that implements left mouse click.
/// @details Clicks by \b X and \b Y coordinates.
/// param[in] _windowHandle Window handle.
///////////////
#ifdef _WIN32
bool LMC( HWND _windowHandle, const unsigned int _coordX, const unsigned int _coordY ) noexcept {
    POINT l_point;
    l_point.x = _coordX; l_point.y = _coordY;

    //! <b>[]</b>
    /// The ClientToScreen function converts the client-area coordinates of a specified l_point to screen coordinates.
    /// @code{.cpp}
    if ( ClientToScreen(
        _windowHandle,              // A handle to the window whose client area is used for the conversion.
        &l_point                    // A l_pointer to a \c POINT structure that contains the client coordinates to be converted. The new screen coordinates are copied into this structure if the function succeeds.
        )
    ) {
    /// @endcode
    //! <b>[]</b>
        //! <b>[set_cursor_position]</b>
        /// Setting cursor position by X and Y.
        /// @code{.cpp}
        SetCursorPos(
            l_point.x,              // X
            l_point.y               // Y
        );
        /// @endcode
        //! <b>[set_cursor_position]</b>

        //! <b>[input]</b>
        /// Creating the \c INPUT structure with parameters to down left mouse button.
        /// @code{.cpp}
        INPUT l_mouseInput = { 0 }; // Empty

        l_mouseInput.type = INPUT_MOUSE;
        l_mouseInput.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        /// @endcode
        //! <b>[input]</b>

        //! <b>[click]</b>
        /// Sendind input with \c INPUT structure to \c winuser.
        /// @code{.cpp}
        SendInput(
            1,                      // The number of structures in the \c pInputs array.
            &l_mouseInput,          // An array of \c INPUT structures. Each structure represents an event to be inserted into the keyboard or mouse input stream.
            sizeof( l_mouseInput )  // The size, in bytes, of an \c INPUT structure. If cbSize is not the size of an \c INPUT structure, the function fails.
        );
        /// @endcode
        //! <b>[click]</b>

        //! <b>[sleep]</b>
        /// Pause between mouse down and mouse up.
        /// @code{.cpp}
        Sleep( 200 );               // 200 ms
        /// @endcode
        //! <b>[sleep]</b>

        //! <b>[input]</b>
        /// Editing the \c INPUT structure with parameters to up left mouse button.
        /// @code{.cpp}
        l_mouseInput.type = INPUT_MOUSE;
        l_mouseInput.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        /// @endcode
        //! <b>[input]</b>

        //! <b>[click]</b>
        /// Sendind input with \c INPUT structure to \c winuser.
        /// @code{.cpp}
        SendInput(
            1,                      // The number of structures in the pInputs array.
            &l_mouseInput,            // An array of INPUT structures. Each structure represents an event to be inserted into the keyboard or mouse input stream.
            sizeof( l_mouseInput )    // The size, in bytes, of an INPUT structure. If cbSize is not the size of an INPUT structure, the function fails.
        );
        /// @endcode
        //! <b>[click]</b>

        //! <b>[return]</b>
        /// Good end of function.
        /// @code{.cpp}
        return (true);
        /// @endcode
        //! <b>[return]</b>
    }

    //! <b>[return]</b>
    /// Bad end of function.
    /// @code{.cpp}
    return (false);
    /// @endcode
    //! <b>[return]</b>
}
#endif