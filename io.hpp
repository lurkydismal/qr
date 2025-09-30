#pragma once

#include <algorithm>
#include <bit>
#include <cstdint>
#include <initializer_list>
#include <string_view>

#include "stdfunc.hpp"
#include "system.hpp"

namespace io {

// File descriptors
using descriptor_t = enum class descriptor : uint8_t {
    input = 0,
    output = 1,
};

void read( std::span< uint8_t > _buffer ) {
    syscall( system::call_t::read,
             static_cast< uint8_t >( descriptor_t::input ),
             std::bit_cast< uintptr_t >( _buffer.data() ), _buffer.size() );
}

void write( const char* _buffer, uint8_t _bufferLength ) {
    syscall( system::call_t::write,
             static_cast< uint8_t >( descriptor_t::output ),
             std::bit_cast< uintptr_t >( _buffer ), _bufferLength );
}

void print( const char* _text, uint8_t _lengthOfText ) {
    write( _text, _lengthOfText );
}

void print( std::span< const char > _text ) {
    print( _text.data(), _text.size() );
}

void print( std::initializer_list< char > _text ) {
    print( std::span( _text ) );
}

namespace terminal {

using code_t = enum class code : uint16_t {
    getAttributes = 0x5401,
    setAttributes = 0x5402,
};

constexpr size_t g_controlCharactersCount = 32;
constexpr size_t g_controlCharactersCountKernel = 19;

// Control mode flags for terminal attributes
using attribute_t = enum class attribute : uint16_t {
    enableSignals = 1,       // Enable signal processing (e.g., Ctrl+C, Ctrl+Z)
    enableCanonicalMode = 2, // Enable canonical mode (input is line-by-line)
    enableCaseMapping =
        4,           // Enable case mapping (e.g., upper/lower case conversion)
    enableEcho = 10, // Enable echoing of input characters to the terminal
    echoEraseChar =
        20, // Echo erase character as BS-SP-BS (backspace, space, backspace)
    echoNewlineAfterKill =
        40,                  // Echo newline after kill character (e.g., Ctrl+U)
    echoNewlineAlways = 100, // Echo newline even if ECHO is disabled
    disableFlushOnInterrupt =
        200, // Disable flush after interrupt or quit (e.g., Ctrl+C)
    stopBackgroundJobsOutput =
        400, // Stop background jobs from writing to the terminal
    enableExtendedFunctions = 1000, // Enable extended terminal functions (e.g.,
                                    // non-standard input processing)
    echoControlCharacter =
        2000, // Echo control characters as ^X (e.g., Ctrl+A as ^A)
    echoErasedCharAsDelete = 4000, // Echo erased character as delete character
    echoKillCharAsKill = 10'000,   // Echo kill character as KILL (e.g., Ctrl+U)
};

using attributeUnderlying_t = std::underlying_type_t< attribute_t >;

[[nodiscard]] constexpr auto operator|( attribute_t _a, attribute_t _b )
    -> attributeUnderlying_t {
    return ( static_cast< attributeUnderlying_t >( _a ) |
             static_cast< attributeUnderlying_t >( _b ) );
}

[[nodiscard]] constexpr auto operator~( attribute_t _value )
    -> attributeUnderlying_t {
    return ( ~( static_cast< attributeUnderlying_t >( _value ) ) );
}

using termios_t = struct termios {
    uint32_t inputModeFlags; // Input mode flags (e.g., raw or canonical mode)
    uint32_t
        outputModeFlags; // Output mode flags (e.g., non-canonical processing)
    uint32_t controlModeFlags; // Control mode flags (e.g., enabling/disabling
                               // signals)
    uint32_t localModeFlags;   // Local mode flags (e.g., handling of special
                               // characters)
    uint8_t line; // Line discipline (e.g., terminal line processing)
    std::array< uint8_t, g_controlCharactersCount >
        characters;       // Control characters (e.g.,
                          // Ctrl+C, Ctrl+Z)
    uint32_t inputSpeed;  // Input speed (baud rate)
    uint32_t outputSpeed; // Output speed (baud rate)
};

void ioctl( code_t _operationCode, termios_t& _memory ) {
    syscall( system::call_t::ioctl,
             static_cast< uint8_t >( descriptor_t::input ),
             static_cast< uint32_t >( _operationCode ),
             std::bit_cast< uintptr_t >( &_memory ) );
}

void ioctl( code_t _operationCode, const termios_t& _memory ) {
    syscall( system::call_t::ioctl,
             static_cast< uint8_t >( descriptor_t::input ),
             static_cast< uint32_t >( _operationCode ),
             std::bit_cast< uintptr_t >( &_memory ) );
}

FORCE_INLINE void tcgetattr( termios_t& _termios ) {
    termios_t l_termios;

    ioctl( code_t::getAttributes, l_termios );

    _termios = l_termios;

#if 0
    __builtin_memset(
        __builtin_mempcpy(
            &_termios->characters[ 0 ], &l_termios.characters[ 0 ],
            ( CONTROL_CHARACTERS_COUNT_KERNEL * sizeof( uint8_t ) ) ),
        0,
        ( ( CONTROL_CHARACTERS_COUNT - CONTROL_CHARACTERS_COUNT_KERNEL ) *
          sizeof( uint8_t ) ) );
#endif

    // Copy first part
    std::ranges::copy_n( l_termios.characters.data(),
                         g_controlCharactersCountKernel,
                         _termios.characters.data() );

#if 0
    // Zero-fill the rest
    std::ranges::fill_n(
        ( _termios.characters.data() + g_controlCharactersCountKernel ),
        ( g_controlCharactersCount - g_controlCharactersCountKernel ),
        uint8_t{} );
#endif
}

FORCE_INLINE void tcsetattr( const termios_t& _termios ) {
    ioctl( code_t::setAttributes, _termios );
}

[[nodiscard]] FORCE_INLINE auto disableCanonicalMode() -> terminal::termios_t {
    terminal::termios_t l_currentAttributes;

    tcgetattr( l_currentAttributes );

    // Return old attributes
    terminal::termios_t l_returnValue = l_currentAttributes;

    // Exclude flags from current
    l_currentAttributes.localModeFlags &=
        ~( terminal::attribute_t::enableCanonicalMode |
           terminal::attribute_t::enableEcho );

    tcsetattr( l_currentAttributes );

    return ( l_returnValue );
}

void hideCursor() {
    constexpr std::string_view l_ansiToHideCursor = "\033[?25l";

    print( l_ansiToHideCursor );
}

void showCursor() {
    constexpr std::string_view l_ansiToShowCursor = "\033[?25h";

    print( l_ansiToShowCursor );
}

void clearScreen() {
#define ANSI_TO_POINT_AT_THE_BEGINNING "\033[H"
#define ANSI_TO_CLEAR_THE_ENTIRE_SCREEN "\033[J"

    constexpr std::string_view
        l_ansiToPointAtTheBeginningAndClearTheEntireScreen =
            ( ANSI_TO_POINT_AT_THE_BEGINNING ANSI_TO_CLEAR_THE_ENTIRE_SCREEN );

    print( l_ansiToPointAtTheBeginningAndClearTheEntireScreen );

#undef ANSI_TO_CLEAR_THE_ENTIRE_SCREEN
#undef ANSI_TO_POINT_AT_THE_BEGINNING
}

} // namespace terminal

} // namespace io
