#pragma once

#include <cstdint>

#include "io.hpp"
#include "logic.hpp"
#include "stdfunc.hpp"
#include "system.hpp"

// #define RENDER_DEBUG_INFORMATION

namespace runtime {

FORCE_INLINE auto fetchEvent() -> uint8_t {
    uint8_t l_event;

    io::read( ( char* )( &l_event ), 1 );

    return ( l_event );
}

using callbackResult_t = enum class callbackResult : int8_t {
    remain = -1,
    success,
    failure
};

using event_t = enum {
    stay = 's',
    up = 'w',
    upRight = 'e',
    right = 'd',
    downRight = 'c',
    down = 'x',
    downLeft = 'z',
    left = 'a',
    upLeft = 'q'
};

// Called once per application run
auto init() -> callbackResult_t;
auto quit( callbackResult_t _exitCode ) -> callbackResult_t;

// Called each frame
auto event( event_t _event ) -> callbackResult_t;
auto iterate() -> callbackResult_t;

// C

static io::terminal::termios g_terminalAttributesBeforeLaunch;

FORCE_INLINE auto init() -> callbackResult_t {
    g_terminalAttributesBeforeLaunch = io::terminal::disableCanonicalMode();

    io::terminal::hideCursor();

    logic$map$init();

    return ( callbackResult_t::remain );
}

FORCE_INLINE auto quit( callbackResult_t _exitCode ) -> callbackResult_t {
    tcsetattr( io::descriptor_t::input, io::terminal::mode_t::allNow,
               &( g_terminalAttributesBeforeLaunch ) );

    io::terminal::showCursor();

    system::exit( ( int )_exitCode );
}

FORCE_INLINE auto event( event_t _event ) -> callbackResult_t {
    callbackResult_t l_returnValue = callbackResult_t::remain;

    direction_t l_direction;

    switch ( _event ) {
        case ( ( event_t )up ): {
            l_direction = ( direction_t )UP;

            break;
        }

        case ( ( event_t )upRight ): {
            l_direction = ( direction_t )UP_RIGHT;

            break;
        }

        case ( ( event_t )right ): {
            l_direction = ( direction_t )RIGHT;

            break;
        }

        case ( ( event_t )downRight ): {
            l_direction = ( direction_t )DOWN_RIGHT;

            break;
        }

        case ( ( event_t )down ): {
            l_direction = ( direction_t )DOWN;

            break;
        }

        case ( ( event_t )downLeft ): {
            l_direction = ( direction_t )DOWN_LEFT;

            break;
        }

        case ( ( event_t )left ): {
            l_direction = ( direction_t )LEFT;

            break;
        }

        case ( ( event_t )upLeft ): {
            l_direction = ( direction_t )UP_LEFT;

            break;
        }

        default: {
            l_direction = ( direction_t )STAY;
        }
    }

    logic$map$move$player( l_direction );

    if ( logic$player$lose$get() ) {
        l_returnValue = callbackResult_t::failure;
    }

    return ( l_returnValue );
}

FORCE_INLINE auto iterate() -> callbackResult_t {
    callbackResult_t l_returnValue = callbackResult_t::remain;

    // Logic
    {
        logic$map$move$ai();
    }

    if ( logic$player$lose$get() ) {
        l_returnValue = callbackResult_t::failure;
    }

    // Render
    {
        io::clearScreen();

        logic$map$render();

        // HUD
        {
            logic$player$render$stats();

#if defined( RENDER_DEBUG_INFORMATION )
            logic$player$render$debug();
#endif
        }
    }

    return ( l_returnValue );
}

} // namespace runtime
