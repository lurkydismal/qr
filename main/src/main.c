#include "callbacks.h"

#define LOOP_COUNT_PER_SECDND 10

void _start( void ) {
    callbackResult_t l_callbackResult = ( callbackResult_t )success;

    l_callbackResult = init();

    if ( UNLIKELY( !( l_callbackResult == ( callbackResult_t )remain ) ) ) {
        goto EXIT;
    }

    for ( ;; ) {
        uint8_t l_event;

        // Event handling
        {
            // Wait-block
            l_event = fetchEvent();

            l_callbackResult = event( l_event );

            if ( UNLIKELY(
                     !( l_callbackResult == ( callbackResult_t )remain ) ) ) {
                break;
            }
        }

        // Iteration
        {
            l_callbackResult = iterate();

            if ( UNLIKELY(
                     !( l_callbackResult == ( callbackResult_t )remain ) ) ) {
                break;
            }
        }
    }

EXIT:
    quit( l_callbackResult );
}
