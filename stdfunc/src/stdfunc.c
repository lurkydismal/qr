#include "stdfunc.h"

#include <sys/types.h>

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

NO_OPTIMIZE void ioctl( uint8_t _fileDescriptor,
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

    volatile struct timeSpecifier l_timeToSleep;

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

void tcgetattr( uint8_t _fileDescriptor, struct termios* _termios ) {
#define TERMINAL_IO_CONTROL_GET_ATTRIBUTES 0x5401

    struct termios l_termios;

    ioctl( _fileDescriptor, TERMINAL_IO_CONTROL_GET_ATTRIBUTES,
           ( uintptr_t )( &l_termios ) );

    *_termios = l_termios;

    __builtin_memset(
        __builtin_mempcpy(
            &_termios->characters[ 0 ], &l_termios.characters[ 0 ],
            ( CONTROL_CHARACTERS_COUNT_KERNEL * sizeof( uint8_t ) ) ),
        0,
        ( ( CONTROL_CHARACTERS_COUNT - CONTROL_CHARACTERS_COUNT_KERNEL ) *
          sizeof( uint8_t ) ) );

#undef TERMINAL_IO_CONTROL_GET_ATTRIBUTES
}

void tcsetattr( uint8_t _fileDescriptor,
                /* unused */ uint8_t _optionalActions,
                const struct termios* _termios ) {
    // Unused
    ( void )( sizeof( _optionalActions ) );

#define TERMINAL_IO_CONTROL_SET_ATTRIBUTES 0x5402

    ioctl( _fileDescriptor, TERMINAL_IO_CONTROL_SET_ATTRIBUTES,
           ( uintptr_t )_termios );

#undef TERMINAL_IO_CONTROL_SET_ATTRIBUTES
}

NO_OPTIMIZE void write( uint8_t _fileDescriptor,
                        const char* _buffer,
                        uint8_t _bufferLength ) {
    syscall( __NR_write, _fileDescriptor, ( uintptr_t )_buffer, _bufferLength );
}

FORCE_INLINE void print( const char* _text, const uint16_t _lengthOfText ) {
    write( 1, _text, _lengthOfText );
}

FORCE_INLINE void clearScreen( void ) {
#define ANSI_TO_POINT_AT_THE_BEGINNING "\033[H"
#define ANSI_TO_CLEAR_ENTIRE_SCREEN "\033[J"

    print( ( ANSI_TO_POINT_AT_THE_BEGINNING ANSI_TO_CLEAR_ENTIRE_SCREEN ),
           ( sizeof( ANSI_TO_POINT_AT_THE_BEGINNING ) +
             sizeof( ANSI_TO_CLEAR_ENTIRE_SCREEN ) ) );

#undef ANSI_TO_CLEAR_ENTIRE_SCREEN
#undef ANSI_TO_POINT_AT_THE_BEGINNING
}

size_t lengthOfNumber( size_t _number ) {
    size_t l_length = 0;

    do {
        l_length++;

        _number /= 10;
    } while ( _number );

    return ( l_length );
}

FORCE_INLINE size_t power( size_t _base, uint8_t _exponent ) {
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
    for ( register ssize_t _characterIndex = ( _lengthOfNumber - 1 );
          _characterIndex >= 0; _characterIndex-- ) {
        l_buffer[ _characterIndex ] =
            ( char )( ( ( _number /
                          ( size_t )( power( 10, ( _lengthOfNumber -
                                                   _characterIndex - 1 ) ) ) ) %
                        10 ) +
                      '0' );
    }
}

size_t randomNumber( void ) {
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

FORCE_INLINE uint8_t fetchEvent( void ) {
    uint8_t l_event;

    read( INPUT_FILE_DESCRIPTOR_NUMBER, ( char* )( &l_event ), 1 );

    return ( l_event );
}
