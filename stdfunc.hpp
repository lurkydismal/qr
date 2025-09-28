#pragma once

#include <sys/types.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>

// File descriptors
#define INPUT_FILE_DESCRIPTOR_NUMBER 0
#define OUTPUT_FILE_DESCRIPTOR_NUMBER 1

// Function attributes
#define FORCE_INLINE [[gnu::always_inline]] inline
#define NOINLINE [[gnu::noinline, gnu::used]]
#define NO_RETURN [[gnu::noreturn]]
#define EXPORT extern "C"

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
    std::array< uint8_t, CONTROL_CHARACTERS_COUNT >
        characters;       // Control characters (e.g.,
                          // Ctrl+C, Ctrl+Z)
    uint32_t inputSpeed;  // Input speed (baud rate)
    uint32_t outputSpeed; // Output speed (baud rate)
};

// System calls
NO_RETURN void exit( int _exitCode );
void ioctl( uint8_t _fileDescriptor,
            uint32_t _operationCode, // ignored
            uintptr_t _memory );

// Wrapped system calls
void usleep( uint32_t _milliseconds );
void tcgetattr( uint8_t _fileDescriptor, termios& _termios );
void tcsetattr( uint8_t _fileDescriptor,
                /* unused */ uint8_t _optionalActions,
                const termios* _termios );

// Utility functions ( no side-effects )
auto lengthOfNumber( size_t _number ) -> size_t;
auto power( size_t _base, uint8_t _exponent ) -> size_t;
void convertNumberToString( char* _buffer,
                            size_t _number,
                            size_t _lengthOfNumber );
auto randomNumber() -> size_t;

// Utility functions ( with side-effects )
void print( const char* _text, const uint16_t _lengthOfText );
void clearScreen(); // Clears the terminal screen

// Specialized functions for application logic
auto fetchEvent() -> uint8_t;

// C

#define __NR_exit 1
#define __NR_read 3
#define __NR_write 4
#define __NR_ioctl 54
#define __NR_nanosleep 162

struct timeSpecifier {
    long seconds;
    long nanoseconds;
};

void syscall( uint16_t _systemCallNumber,
              uintptr_t _argument1,
              uintptr_t _argument2,
              uintptr_t _argument3 ) {
    __asm__ volatile(
        "movl %0, %%eax\n"
        "movl %1, %%ebx\n"
        "movl %2, %%ecx\n"
        "movl %3, %%edx\n"
        "int $0x80\n"
        :
        : "g"( _systemCallNumber ), "g"( _argument1 ), "g"( _argument2 ),
          "g"( _argument3 )
        : "%eax", "%ebx", "%ecx", "%edx" );
}

NO_RETURN FORCE_INLINE void exit( int _exitCode ) {
    __asm__ volatile(
        "movl %0, %%eax\n"
        "movl %1, %%ebx\n"
        "int $0x80\n"
        :
        : "g"( __NR_exit ), "g"( _exitCode )
        : "%eax", "%ebx" );

    __builtin_unreachable();
}

NOINLINE void ioctl( uint8_t _fileDescriptor,
                     uint32_t _operationCode,
                     uintptr_t _memory ) {
    syscall( __NR_ioctl, _fileDescriptor, _operationCode, _memory );
}

FORCE_INLINE void usleep( uint32_t _milliseconds ) {
#define MILLISECONDS_IN_SECOND 1000
#define NANOSECONDS_IN_SECOND ( MILLISECONDS_IN_SECOND * 1000 )

#define MILLISECONDS_TO_SECONDS( _milliseconds ) \
    ( ( _milliseconds ) / MILLISECONDS_IN_SECOND )
#define MILLISECONDS_TO_NANOSECONDS( _milliseconds ) \
    ( ( _milliseconds ) * NANOSECONDS_IN_SECOND )

#define SECONDS_TO_MILLISECONDS( _seconds ) \
    ( ( _seconds ) * MILLISECONDS_IN_SECOND )

    volatile timeSpecifier l_timeToSleep;

    const size_t l_secondsToSleep = MILLISECONDS_TO_SECONDS( _milliseconds );
    const size_t l_remainderLessThanASecond =
        ( _milliseconds - SECONDS_TO_MILLISECONDS( l_secondsToSleep ) );
    const size_t l_nanoSecondsToSleep =
        MILLISECONDS_TO_NANOSECONDS( l_remainderLessThanASecond );

    l_timeToSleep.seconds = l_secondsToSleep;
    l_timeToSleep.nanoseconds = l_nanoSecondsToSleep;

    __asm__ volatile(
        "movl %0, %%eax\n"
        "movl %1, %%ebx\n"
        "xorl %%ecx, %%ecx\n"
        "int $0x80\n"
        :
        : "g"( __NR_nanosleep ), "g"( ( uintptr_t )( &l_timeToSleep ) )
        : "%eax", "%ebx" );

#undef SECONDS_TO_MILLISECONDS

#undef MILLISECONDS_TO_NANOSECONDS
#undef MILLISECONDS_TO_SECONDS

#undef MILLISECONDS_IN_SECOND
#undef NANOSECONDS_IN_SECOND
}

void tcgetattr( uint8_t _fileDescriptor, termios& _termios ) {
#define TERMINAL_IO_CONTROL_GET_ATTRIBUTES 0x5401

    termios l_termios;

    ioctl( _fileDescriptor, TERMINAL_IO_CONTROL_GET_ATTRIBUTES,
           ( uintptr_t )( &l_termios ) );

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

#undef TERMINAL_IO_CONTROL_GET_ATTRIBUTES
}

void tcsetattr( uint8_t _fileDescriptor,
                [[maybe_unused]] uint8_t _optionalActions,
                const termios* _termios ) {
#define TERMINAL_IO_CONTROL_SET_ATTRIBUTES 0x5402

    ioctl( _fileDescriptor, TERMINAL_IO_CONTROL_SET_ATTRIBUTES,
           ( uintptr_t )_termios );

#undef TERMINAL_IO_CONTROL_SET_ATTRIBUTES
}

NOINLINE void write( uint8_t _fileDescriptor,
                     const char* _buffer,
                     uint8_t _bufferLength ) {
    syscall( __NR_write, _fileDescriptor, ( uintptr_t )_buffer, _bufferLength );
}

FORCE_INLINE void print( const char* _text, const uint16_t _lengthOfText ) {
    write( 1, _text, _lengthOfText );
}

FORCE_INLINE void clearScreen() {
#define ANSI_TO_POINT_AT_THE_BEGINNING "\033[H"
#define ANSI_TO_CLEAR_ENTIRE_SCREEN "\033[J"

    print( ( ANSI_TO_POINT_AT_THE_BEGINNING ANSI_TO_CLEAR_ENTIRE_SCREEN ),
           ( sizeof( ANSI_TO_POINT_AT_THE_BEGINNING ) +
             sizeof( ANSI_TO_CLEAR_ENTIRE_SCREEN ) ) );

#undef ANSI_TO_CLEAR_ENTIRE_SCREEN
#undef ANSI_TO_POINT_AT_THE_BEGINNING
}

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

#pragma omp simd
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

auto randomNumber() -> size_t {
#define COMPILATION_TIME_AS_SEED                                      \
    ( __TIME__[ 0 ] + __TIME__[ 1 ] + __TIME__[ 2 ] + __TIME__[ 3 ] + \
      __TIME__[ 4 ] + __TIME__[ 5 ] )

    static uint32_t l_seed = COMPILATION_TIME_AS_SEED;

    l_seed ^= ( l_seed << 13 );
    l_seed ^= ( l_seed >> 17 );
    l_seed ^= ( l_seed << 5 );

    return ( l_seed );

#undef COMPILATION_TIME_AS_SEED
}

FORCE_INLINE void read( uint8_t _fileDescriptor,
                        char* _buffer,
                        uint8_t _bufferLength ) {
    syscall( __NR_read, _fileDescriptor, ( uintptr_t )_buffer, _bufferLength );
}

FORCE_INLINE auto fetchEvent() -> uint8_t {
    uint8_t l_event;

    read( INPUT_FILE_DESCRIPTOR_NUMBER, ( char* )( &l_event ), 1 );

    return ( l_event );
}
