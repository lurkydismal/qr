#pragma once

#include <algorithm>
#include <cstdint>
#include <string_view>

#include "stdfunc.hpp"
#include "system.hpp"

namespace io {

// File descriptors
using descriptor_t = enum class descriptor : uint8_t {
    input = 0,
    out = 1,
};

FORCE_INLINE void read( char* _buffer, uint8_t _bufferLength ) {
    syscall( system::call_t::read,
             static_cast< uintptr_t >( descriptor_t::input ),
             std::bit_cast< uintptr_t >( _buffer ), _bufferLength );
}

FORCE_INLINE void write( const char* _buffer, uint8_t _bufferLength ) {
    syscall( system::call_t::write,
             static_cast< uintptr_t >( descriptor_t::out ),
             std::bit_cast< uintptr_t >( _buffer ), _bufferLength );
}

FORCE_INLINE void print( const char* _text, uint8_t _lengthOfText ) {
    write( _text, _lengthOfText );
}

template < std::size_t N >
    requires( N > 0 )
FORCE_INLINE void write( const std::array< char, N >& _buffer ) {
    write( _buffer.data(), _buffer.size() );
}

template < std::size_t N >
FORCE_INLINE void print( const std::array< char, N >& _text ) {
    write( _text );
}

FORCE_INLINE void write( const std::string_view& _buffer ) {
    write( _buffer.data(), _buffer.size() );
}

FORCE_INLINE void print( const std::string_view& _text ) {
    write( _text );
}

FORCE_INLINE void clearScreen() {
#define ANSI_TO_POINT_AT_THE_BEGINNING "\033[H"
#define ANSI_TO_CLEAR_ENTIRE_SCREEN "\033[J"

    constexpr std::string_view l_x =
        ( ANSI_TO_POINT_AT_THE_BEGINNING ANSI_TO_CLEAR_ENTIRE_SCREEN );

    print( l_x );

#undef ANSI_TO_CLEAR_ENTIRE_SCREEN
#undef ANSI_TO_POINT_AT_THE_BEGINNING
}

namespace terminal {

namespace {

using code_t = enum class code : uint16_t {
    getAttributes = 0x5401,
    setAttributes = 0x5402,
};

}

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

[[nodiscard]] constexpr auto operator|( attribute_t _storage,
                                        attribute_t _value )
    -> attributeUnderlying_t {
    return ( static_cast< attributeUnderlying_t >( _storage ) |
             static_cast< attributeUnderlying_t >( _value ) );
}

[[nodiscard]] constexpr auto operator&( attribute_t _storage,
                                        attribute_t _value )
    -> attributeUnderlying_t {
    return ( static_cast< attributeUnderlying_t >( _storage ) &
             static_cast< attributeUnderlying_t >( _value ) );
}

constexpr void operator|=( attribute_t& _storage, attribute_t _value ) {
    _storage = static_cast< attribute_t >( _storage | _value );
}

[[nodiscard]] constexpr auto operator~( attribute_t _value )
    -> attributeUnderlying_t {
    return ( ~( static_cast< attributeUnderlying_t >( _value ) ) );
}

using mode_t = enum class mode : uint8_t {
    allNow = 0, // Apply all changes to terminal settings immediately
};

constexpr size_t CONTROL_CHARACTERS_COUNT = 32;
constexpr size_t CONTROL_CHARACTERS_COUNT_KERNEL = 19;

struct termios {
    uint32_t inputModeFlags; // Input mode flags (e.g., raw or canonical mode)
    uint32_t
        outputModeFlags; // Output mode flags (e.g., non-canonical processing)
    uint32_t controlModeFlags; // Control mode flags (e.g., enabling/disabling
                               // signals)
    uint32_t localModeFlags;   // Local mode flags (e.g., handling of special
                               // characters)
    uint8_t line; // Line discipline (e.g., terminal line processing)
    std::array< uint8_t, CONTROL_CHARACTERS_COUNT >
        characters;       // Control characters (e.g.,
                          // Ctrl+C, Ctrl+Z)
    uint32_t inputSpeed;  // Input speed (baud rate)
    uint32_t outputSpeed; // Output speed (baud rate)
};

NOINLINE void ioctl( code_t _operationCode, uintptr_t _memory ) {
    syscall( system::call_t::ioctl,
             static_cast< uint8_t >( descriptor_t::input ),
             ( uint32_t )_operationCode, _memory );
}

void tcgetattr( termios& _termios ) {
    termios l_termios;

    ioctl( code_t::getAttributes, ( uintptr_t )( &l_termios ) );

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

    // copy first part
    std::ranges::copy_n( l_termios.characters.data(),
                         CONTROL_CHARACTERS_COUNT_KERNEL,
                         _termios.characters.data() );

    // zero-fill the rest
    std::ranges::fill_n(
        _termios.characters.data() + CONTROL_CHARACTERS_COUNT_KERNEL,
        CONTROL_CHARACTERS_COUNT - CONTROL_CHARACTERS_COUNT_KERNEL,
        static_cast< uint8_t >( 0 ) );
}

void tcsetattr( const termios* _termios ) {
    ioctl( code_t::setAttributes, ( uintptr_t )_termios );
}

FORCE_INLINE auto disableCanonicalMode() -> terminal::termios {
    terminal::termios l_currentAttributes;

    tcgetattr( l_currentAttributes );

    // Return old attributes
    terminal::termios l_returnValue = l_currentAttributes;

    // Exclude flags from current
    l_currentAttributes.localModeFlags &=
        ~( terminal::attribute_t::enableCanonicalMode |
           terminal::attribute_t::enableEcho );

    tcsetattr( &l_currentAttributes );

    return ( l_returnValue );
}

FORCE_INLINE void hideCursor() {
    constexpr std::string_view l_ansiToHideCursor = "\033[?25l";

    print( l_ansiToHideCursor );
}

FORCE_INLINE void showCursor() {
    constexpr std::string_view l_ansiToShowCursor = "\033[?25h";

    print( l_ansiToShowCursor );
}

} // namespace terminal

} // namespace io
