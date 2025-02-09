#include "logic.h"

#include "stdfunc.h"

char g_map[] =
    "0\
     +------+     +-------+   M#                  K####T\n\
     |....K.|  ###}.......|    #             +----/----+        C\n\
     |.@.....###  |.......|    #             |.........|        #\n\
     |......|  #  |........###### ###########}.........|     W##M\n\
     |......|     |.M.....|      K           |.........|     #\n\
     +------+     |...C...|            +-----+C........|     #\n\
     |.G....|     +-------+            |.....C.M.......{#####\n\
     |......+-----+                    |..........M....|\n\
     >............}K###################}....W..........|\n\
     +------------+                    +---------------+\n";

static uint8_t g_playerHealthPoints = START_HEALTH_POINTS;
static uint8_t g_playerExperiencePoints = START_EXPERIENCE_POINTS;
static uint8_t g_playerItems[ MAX_ITEM_COUNT ] = { 'E', 'E', 'E', 'E', 'E' };

FORCE_INLINE void renderPoints( char** _cursorPosition,
                                uint8_t _points,
                                const size_t _textLength ) {
    const size_t l_lengthOfPoints = lengthOfNumber( _points );

    *_cursorPosition += ( _textLength - l_lengthOfPoints );

    convertNumberToString( *_cursorPosition, _points, l_lengthOfPoints );

    *_cursorPosition += ( l_lengthOfPoints - 1 );
}

FORCE_INLINE void logic$render$map( void ) {
    FOR( const char*, g_map ) {
        char x[ 1 ] = { *_element };

        print( x, 1 );
    }
}

// HP:100
// EXP:100
// ITEMS:E E E E E
FORCE_INLINE void logic$render$playerStats( void ) {
#define FULL_HEALTH_TEXT HEALTH_TEXT HEALTH_POINTS_TEXT_PLACEHOLDER
#define FULL_EXPERIENCE_TEXT EXPERIENCE_TEXT EXPERIENCE_POINTS_TEXT_PLACEHOLDER
#define FULL_ITEMS_TEXT ITEMS_TEXT ITEMS_TEXT_PLACEHOLDER

    char l_buffer[] = ( FULL_HEALTH_TEXT FULL_EXPERIENCE_TEXT FULL_ITEMS_TEXT );
    char* l_cursorPosition = ( l_buffer - 1 );

    // Health
    {
        renderPoints( &l_cursorPosition, g_playerHealthPoints,
                      sizeof( FULL_HEALTH_TEXT ) );
    }

    // Experience
    {
        renderPoints( &l_cursorPosition, g_playerExperiencePoints,
                      sizeof( FULL_EXPERIENCE_TEXT ) );
    }

    // Items
    {
        l_cursorPosition += ( sizeof( FULL_ITEMS_TEXT ) -
                              ( sizeof( ITEMS_TEXT_PLACEHOLDER ) - 1 ) );

        FOR( const uint8_t*, g_playerItems ) {
            *l_cursorPosition = ( char )( *_element );

            // Move to the next character
            l_cursorPosition++;

            // Skip delimiter
            l_cursorPosition++;
        }
    }

    print( l_buffer, ( sizeof( l_buffer ) - 1 ) );

#undef FULL_ITEMS_TEXT
#undef FULL_EXPERIENCE_TEXT
#undef FULL_HEALTH_TEXT
}
