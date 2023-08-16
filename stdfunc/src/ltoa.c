///////////////
/// @file ltoa.c
/// @brief Definitions of \c Memcpy and \c Ltoa functions.
///////////////
#include <stddef.h>
#include <stdint.h>

///////////////
/// @brief Size of string buffer.
///////////////
#define BUFSIZE ( sizeof( long ) * 8 + 1 )

///////////////
/// @brief Function that copy bytes from source variable to destination variable.
/// @param[in] _destination Pointer to destination variable.
/// @param[in] _source Pointer to source variable.
/// @param[in] _numberOfBytes Size of source.
///////////////
void Memcpy( void* _destination, const void* _source, size_t _numberOfBytes )  {
    //! <b>[typecast]</b>
    /// Typecast _source and _destination addresses to <tt>(char*)</tt>.
    /// @code{.c}
    const char* l_cSource      = (const char*)_source;
    char* l_cDestination = (char*)_destination;
    /// @endcode
    //! <b>[typecast]</b>

    //! <b>[copy]</b>
    /// Copy contents of _source array to _destination array.
    /// @code{.c}
    for ( uint32_t _byteIndex = 0; _byteIndex < _numberOfBytes; _byteIndex++ ) {
        l_cDestination[ _byteIndex ] = l_cSource[ _byteIndex ];
    }
    /// @endcode
    //! <b>[copy]</b>
}

///////////////
/// @brief Converts a long integer to a string.
/// @details Copyright 1988-90 by Robert B. Stout dba MicroFirm. Released to public domain, 1991. Can not convert negative values.
/// @param[in] _number Number to be converted.
/// @param[in] _cString Buffer in which to build the converted string.
/// @return A character pointer to the converted string if successful, a NULL pointer if the number base specified is out of range.
///////////////
char* Ltoa( unsigned long _number, char* _cString ) {
    //! <b>[declare]</b>
    /// Declare l_characterIndex to register, pointer to cString and buffer with converted string.
    /// @code{.c}
    register uint32_t l_characterIndex;
    char *l_tail, *l_head = _cString, l_buf[ BUFSIZE ];
    /// @endcode

    /// Set the last character of string to NULL terminator.
    /// @code{.c}
    l_tail    = &l_buf[ BUFSIZE - 1 ];       // last character position
    *l_tail-- = '\0';
    /// @endcode
    //! <b>[declare]</b>

    //! <b>[convert]</b>
    /// Convert integer value to string value.
    /// @code{.c}
    l_characterIndex = 1;

    do {
        ++l_characterIndex;

        *l_tail-- = (char)( ( _number % 10 ) + '0' );

        _number /= 10;
    } while ( _number != 0 );
    /// @endcode
    //! <b>[convert]</b>

    //! <b>[memory_copy]</b>
    /// Copy l_tail string to l_head string.
    /// @code{.c}
    Memcpy(
        l_head,                                // Destination
        ++l_tail,                              // Source
        l_characterIndex                       // Number of bytes
    );
    /// @endcode
    //! <b>[memory_copy]</b>

    //! <b>[return]</b>
    /// End of function.
    /// @code{.c}
    return ( _cString );
    /// @endcode
    //! <b>[return]</b>
}
