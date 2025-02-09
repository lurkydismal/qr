#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// File descriptors
#define INPUT_FILE_DESCRIPTOR_NUMBER 0
#define OUTPUT_FILE_DESCRIPTOR_NUMBER 1

// Function attributes
#define FORCE_INLINE __attribute__( ( always_inline ) ) inline
#define NO_OPTIMIZE __attribute__( ( optimize( "0" ) ) )
#define NO_RETURN __attribute__( ( noreturn ) )

// Branch prediction hints
#define LIKELY( _expression ) __builtin_expect( !!( _expression ), 1 )
#define UNLIKELY( _expression ) __builtin_expect( !!( _expression ), 0 )

// Control mode flags for terminal attributes
#define ENABLE_SIGNALS \
    0000001 // Enable signal processing (e.g., Ctrl+C, Ctrl+Z)
#define ENABLE_CANONICAL_MODE \
    0000002 // Enable canonical mode (input is line-by-line)
#define ENABLE_CASE_MAPPING \
    0000004 // Enable case mapping (e.g., upper/lower case conversion)
#define ENABLE_ECHO \
    0000010 // Enable echoing of input characters to the terminal
#define ECHO_ERASE_CHAR \
    0000020 // Echo erase character as BS-SP-BS (backspace, space, backspace)
#define ECHO_NEWLINE_AFTER_KILL \
    0000040 // Echo newline after kill character (e.g., Ctrl+U)
#define ECHO_NEWLINE_ALWAYS 0000100 // Echo newline even if ECHO is disabled
#define DISABLE_FLUSH_ON_INTERRUPT \
    0000200 // Disable flush after interrupt or quit (e.g., Ctrl+C)
#define STOP_BACKGROUND_JOBS_OUTPUT \
    0000400 // Stop background jobs from writing to the terminal
#define ENABLE_EXTENDED_FUNCTIONS \
    0001000 // Enable extended terminal functions (e.g., non-standard input
            // processing)
#define ECHO_CONTROL_CHARACTERS \
    0002000 // Echo control characters as ^X (e.g., Ctrl+A as ^A)
#define ECHO_ERASED_CHAR_AS_DELETE \
    0004000 // Echo erased character as delete character
#define ECHO_KILL_CHAR_AS_KILL \
    0010000 // Echo kill character as KILL (e.g., Ctrl+U)

// tcsetattr operation codes
#define TERMINAL_CONTROL_SET_ALL_NOW \
    0 // Apply all changes to terminal settings immediately

#define CONTROL_CHARACTERS_COUNT 32
#define CONTROL_CHARACTERS_COUNT_KERNEL 19

#define arrayLength( _array ) ( sizeof( _array ) / sizeof( _array[ 0 ] ) )
#define randomValueFromArray( _array ) \
    ( _array[ randomNumber() % arrayLength( _array ) ] )

#define FOR( _type, _array )       \
    for ( _type _element = _array; \
          _element < ( _array + arrayLength( _array ) ); _element++ )

struct termios {
    uint32_t inputModeFlags; // Input mode flags (e.g., raw or canonical mode)
    uint32_t
        outputModeFlags; // Output mode flags (e.g., non-canonical processing)
    uint32_t controlModeFlags; // Control mode flags (e.g., enabling/disabling
                               // signals)
    uint32_t localModeFlags;   // Local mode flags (e.g., handling of special
                               // characters)
    uint8_t line; // Line discipline (e.g., terminal line processing)
    uint8_t characters[ CONTROL_CHARACTERS_COUNT ]; // Control characters (e.g.,
                                                    // Ctrl+C, Ctrl+Z)
    uint32_t inputSpeed;                            // Input speed (baud rate)
    uint32_t outputSpeed;                           // Output speed (baud rate)
};

// System calls
NO_RETURN void exit( int _exitCode );
void ioctl( uint8_t _fileDescriptor,
            uint32_t _operationCode, // ignored
            uintptr_t _memory );

// Wrapped system calls
void usleep( uint32_t _milliseconds );
void tcgetattr( uint8_t _fileDescriptor, struct termios* _termios );
void tcsetattr( uint8_t _fileDescriptor,
                /* unused */ uint8_t _optionalActions,
                const struct termios* _termios );

// Utility functions ( no side-effects )
size_t lengthOfNumber( size_t _number );
size_t power( size_t _base, uint8_t _exponent );
void convertNumberToString( char* _buffer,
                            size_t _number,
                            size_t _lengthOfNumber );
size_t randomNumber( void );

// Utility functions ( with side-effects )
void print( const char* _text, const uint16_t _lengthOfText );
void clearScreen( void ); // Clears the terminal screen

// Specialized functions for application logic
uint8_t fetchEvent( void );
