#include "runtime.hpp"
#include "stdfunc.hpp"

static_assert( ( sizeof( void* ) == 4 ), "This is for i386 (32-bit) only" );

EXPORT void _start() {
    runtime::callbackResult_t l_callbackResult = runtime::init();

    do {
        if ( l_callbackResult != runtime::callbackResult_t::remain )
            [[unlikely]] {
            break;
        }

        for ( ;; ) {
            // Event handling
            {
                // Wait-block
                uint8_t l_event = runtime::fetchEvent();

                l_callbackResult = event( ( runtime::event_t )l_event );

                if ( l_callbackResult != runtime::callbackResult_t::remain )
                    [[unlikely]] {
                    break;
                }
            }

            // Iteration
            {
                l_callbackResult = runtime::iterate();

                if ( l_callbackResult != runtime::callbackResult_t::remain )
                    [[unlikely]] {
                    break;
                }
            }
        }
    } while ( false );

    quit( l_callbackResult );
}
