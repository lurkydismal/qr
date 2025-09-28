#include "callbacks.hpp"
#include "logic.hpp"
#include "stdfunc.hpp"

static_assert( ( sizeof( void* ) == 4 ), "This is for i386 (32-bit) only" );

#define EXPORT extern "C"

EXPORT void _start() {
    auto l_callbackResult = callbackResult_t::success;

    l_callbackResult = init();

    if ( !( l_callbackResult == callbackResult_t::remain ) ) [[unlikely]] {
        goto EXIT;
    }

    for ( ;; ) {
        uint8_t l_event;

        // Event handling
        {
            // Wait-block
            l_event = fetchEvent();

            l_callbackResult = event( ( event_t )l_event );

            if ( !( l_callbackResult == callbackResult_t::remain ) )
                [[unlikely]] {
                break;
            }
        }

        // Iteration
        {
            l_callbackResult = iterate();

            if ( !( l_callbackResult == callbackResult_t::remain ) )
                [[unlikely]] {
                break;
            }
        }
    }

EXIT:
    quit( l_callbackResult );
}
