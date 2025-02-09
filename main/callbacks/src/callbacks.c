#include "callbacks.h"

#include "logic.h"
#include "stdfunc.h"

static struct termios g_terminalAttributesBeforeLaunch;

FORCE_INLINE struct termios terminal$disableCanonicalMode( void ) {
#define TERMINAL_CANONICAL_MODE_FLAG ENABLE_CANONICAL_MODE
#define TERMINALECHO_MODE_FLAG ENABLE_ECHO

    struct termios l_currentAttributes;

    tcgetattr( INPUT_FILE_DESCRIPTOR_NUMBER, &l_currentAttributes );

    // Return old attributes
    struct termios l_returnValue = l_currentAttributes;

    // Exclude flags from current
    l_currentAttributes.localModeFlags &=
        ~( TERMINAL_CANONICAL_MODE_FLAG | TERMINALECHO_MODE_FLAG );

    tcsetattr( INPUT_FILE_DESCRIPTOR_NUMBER, TERMINAL_CONTROL_SET_ALL_NOW,
               &l_currentAttributes );

    return ( l_returnValue );

#undef TERMINAL_CANONICAL_MODE_FLAG
#undef TERMINALECHO_MODE_FLAG
}

FORCE_INLINE void terminal$hideCursor( void ) {
#define ANSI_TO_HIDE_CURSOR "\033[?25l"

    print( ANSI_TO_HIDE_CURSOR, sizeof( ANSI_TO_HIDE_CURSOR ) );

#undef ANSI_TO_HIDE_CURSOR
}

FORCE_INLINE void terminal$showCursor( void ) {
#define ANSI_TO_SHOW_CURSOR "\033[?25h"

    print( ANSI_TO_SHOW_CURSOR, sizeof( ANSI_TO_SHOW_CURSOR ) );

#undef ANSI_TO_SHOW_CURSOR
}

FORCE_INLINE callbackResult_t init( void ) {
    g_terminalAttributesBeforeLaunch = terminal$disableCanonicalMode();

    terminal$hideCursor();

    logic$map$init();

    return ( ( callbackResult_t )remain );
}

FORCE_INLINE callbackResult_t quit( callbackResult_t _exitCode ) {
    tcsetattr( INPUT_FILE_DESCRIPTOR_NUMBER, TERMINAL_CONTROL_SET_ALL_NOW,
               &( g_terminalAttributesBeforeLaunch ) );

    terminal$showCursor();

    exit( _exitCode );
}

FORCE_INLINE callbackResult_t event( event_t _event ) {
    callbackResult_t l_returnValue = ( callbackResult_t )remain;

    direction_t l_direction;

    switch ( _event ) {
        case ( ( event_t )up ): {
            l_direction = ( direction_t )UP;

            break;
        }

        case ( ( event_t )up_right ): {
            l_direction = ( direction_t )UP_RIGHT;

            break;
        }

        case ( ( event_t )right ): {
            l_direction = ( direction_t )RIGHT;

            break;
        }

        case ( ( event_t )down_right ): {
            l_direction = ( direction_t )DOWN_RIGHT;

            break;
        }

        case ( ( event_t )down ): {
            l_direction = ( direction_t )DOWN;

            break;
        }

        case ( ( event_t )down_left ): {
            l_direction = ( direction_t )DOWN_LEFT;

            break;
        }

        case ( ( event_t )left ): {
            l_direction = ( direction_t )LEFT;

            break;
        }

        case ( ( event_t )up_left ): {
            l_direction = ( direction_t )UP_LEFT;

            break;
        }

        default: {
            l_direction = ( direction_t )STAY;
        }
    }

    logic$map$move$player( l_direction );

    if ( logic$player$lose$get() ) {
        l_returnValue = ( callbackResult_t )failure;
    }

    return ( l_returnValue );
}

FORCE_INLINE callbackResult_t iterate( void ) {
    callbackResult_t l_returnValue = ( callbackResult_t )remain;

    // Logic
    {
        logic$map$move$ai();
    }

    if ( logic$player$lose$get() ) {
        l_returnValue = ( callbackResult_t )failure;
    }

    // Render
    {
        clearScreen();

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
