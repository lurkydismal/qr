#include "runtime.hpp"
#include "stdfunc.hpp"

static_assert( ( sizeof( void* ) == 4 ), "This is for i386 (32-bit) only" );

EXPORT void _start() {
    runtime::result_t l_callbackResult = runtime::init();

    do {
        if ( l_callbackResult != runtime::result_t::remain ) [[unlikely]] {
            break;
        }

        for ( ;; ) {
            // Event handling
            {
                // Wait-block
                runtime::move_t l_move = runtime::waitMove();

                l_callbackResult = runtime::event( l_move );

                if ( l_callbackResult != runtime::result_t::remain )
                    [[unlikely]] {
                    break;
                }
            }

            // Iteration
            {
                l_callbackResult = runtime::iterate();

                if ( l_callbackResult != runtime::result_t::remain )
                    [[unlikely]] {
                    break;
                }
            }
        }
    } while ( false );

    quit( l_callbackResult );
}
