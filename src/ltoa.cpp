///////////////
/// @file ltoa.cpp
/// @brief Definitions of \c Memcpy and \c Ltoa functions.
///////////////
#include <stddef.h>                 // size_t

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
void Memcpy( void* _destination, void* _source, size_t _numberOfBytes ) noexcept {
    //! <b>[typecast]</b>
    /// Typecast _source and _destination addresses to <tt>(char*)</tt>.
    /// @code{.cpp}
    char* l_cSource      = (char*)_source;
    char* l_cDestination = (char*)_destination;
    /// @endcode
    //! <b>[typecast]</b>

    //! <b>[copy]</b>
    /// Copy contents of _source array to _destination array.
    /// @code{.cpp}
    for ( unsigned int _byteIndex = 0; _byteIndex < _numberOfBytes; _byteIndex++ ) {
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
char* Ltoa( long _number, char* _cString ) {
    //! <b>[declare]</b>
    /// Declare l_characterIndex to register, pointer to cString and buffer with converted string.
    /// @code{.cpp}
    register unsigned int l_characterIndex;
    char *l_tail, *l_head = _cString, l_buf[ BUFSIZE ];
    /// @endcode

    /// Set the last character of string to NULL terminator.
    /// @code{.cpp}
    l_tail    = &l_buf[ BUFSIZE - 1 ];           /* last character position      */
    *l_tail-- = '\0';
    /// @endcode
    //! <b>[declare]</b>

    //! <b>[convert]</b>
    /// Convert integer value to string value.
    /// @code{.cpp}
    for ( l_characterIndex = 1; _number; ++l_characterIndex ) {
        *l_tail-- = (char)(
            ( _number % 10 ) + ( (
                9L < ( _number % 10 )        // if decimal is not NULL
            ) ? (
                'A' - 10L
            ) : (
                '0'
            ) )
        );

        _number /= 10;
    }
    /// @endcode
    //! <b>[convert]</b>

    //! <b>[memory_copy]</b>
    /// Copy l_tail string to l_head string.
    /// @code{.cpp}
    Memcpy(
        l_head,                                // Destination
        ++l_tail,                              // Source
        l_characterIndex                       // Number of bytes
    );
    /// @endcode
    //! <b>[memory_copy]</b>

    //! <b>[return]</b>
    /// End of function.
    /// @code{.cpp}
    return ( _cString );
    /// @endcode
    //! <b>[return]</b>
}

///////////////
/// @brief Converts a long integer to a string.
/// @details Copyright 1988-90 by Robert B. Stout dba MicroFirm. Released to public domain, 1991.
/// @param[in] _number Number to be converted.
/// @param[in] _cString Buffer in which to build the converted string.
/// @param[in] _base Number base to use for conversion.
/// @return A character pointer to the converted string if successful, a NULL pointer if the number base specified is out of range.
///////////////
char* Ltoa( long _number, char* _cString, int _base ) {
    //! <b>[declare]</b>
    /// Declare l_characterIndex to register, pointer to cString and buffer with converted string.
    /// @code{.cpp}
    register int l_characterIndex = 2;
    char *l_tail, *l_head = _cString, l_buf[ BUFSIZE ];
    /// @endcode

    /// can only use 0-9, A-Z.
    /// @code{.cpp}
    if ( 36 < _base || 2 > _base ) {
        _base = 10;
    }
    /// @endcode

    /// Set the last character of string to NULL terminator.
    /// @code{.cpp}
    l_tail    = &l_buf[ BUFSIZE - 1 ];           /* last character position      */
    *l_tail-- = '\0';
    /// @endcode

    /// Convert _number to positive value.
    /// @code{.cpp}
    if ( ( 10 == _base ) && ( _number < 0L ) ) {
        *l_head++ = '-';
        _number    = -_number;

    }
    /// @endcode
    //! <b>[declare]</b>

    //! <b>[convert]</b>
    /// Convert integer value to string value.
    /// @code{.cpp}
    if ( _number ) {
        for ( l_characterIndex = 1; _number; ++l_characterIndex ) {
            *l_tail-- = (char)(
                ( _number % _base ) + ( (
                    9L < ( _number % _base ) // if decimal is not NULL
                ) ? (
                    'A' - 10L
                ) : (
                    '0'
                ) )
            );

            _number   /= _base;
        }

    } else {
        *l_tail--     = '0';
    }
    /// @endcode
    //! <b>[convert]</b>

    //! <b>[memory_copy]</b>
    /// Copy l_tail string to l_head string.
    /// @code{.cpp}
    Memcpy(
        l_head,                                // Destination
        ++l_tail,                              // Source
        l_characterIndex                       // Number of bytes
    );
    /// @endcode
    //! <b>[memory_copy]</b>

    //! <b>[return]</b>
    /// End of function.
    /// @code{.cpp}
    return ( _cString );
    /// @endcode
    //! <b>[return]</b>
}