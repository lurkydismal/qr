#pragma once

#include <stdint.h>

#include "stdfunc.h"

// #define RENDER_DEBUG_INFORMATION

typedef enum { remain = -1, success, failure } callbackResult_t;

typedef enum {
    stay = 's',
    up = 'w',
    up_right = 'e',
    right = 'd',
    down_right = 'c',
    down = 'x',
    down_left = 'z',
    left = 'a',
    up_left = 'q'
} event_t;

// Called once per application run
callbackResult_t init( void );
callbackResult_t quit( callbackResult_t _exitCode );

// Called each frame
callbackResult_t event( event_t _event );
callbackResult_t iterate( void );
