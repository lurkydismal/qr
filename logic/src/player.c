#include "player.h"

#include "stdfunc.h"

uint8_t g_playerHealthPoints = START_HEALTH_POINTS;
uint8_t g_playerExperiencePoints = START_EXPERIENCE_POINTS;
item_t g_playerItems[ MAX_ITEM_COUNT ] = { 'E', 'E', 'E', 'E', 'E' };

uint8_t g_monsterHealthPoints = START_MONSTER_HEALTH_POINTS;
uint8_t g_guardianHealthPoints = START_GUARDIAN_HEALTH_POINTS;

bool g_didLose = false;

void logic$player$experience$add( const size_t _amount ) {
    g_playerExperiencePoints += _amount;

    if ( g_playerExperiencePoints > MAX_EXPERIENCE_POINTS ) {
        g_playerExperiencePoints = MAX_EXPERIENCE_POINTS;
    }
}

FORCE_INLINE void logic$player$inventory$add( const item_t _item ) {
    FOR( item_t*, g_playerItems ) {
        if ( UNLIKELY( *_element == EMPTY ) ) {
            *_element = _item;

            return;
        }
    }

    g_playerItems[ 0 ] = _item;
}

bool logic$player$inventory$use( const item_t _item ) {
    FOR( item_t*, g_playerItems ) {
        if ( *_element == _item ) {
            *_element = EMPTY;

            return ( true );
        }
    }

    return ( false );
}

FORCE_INLINE bool logic$player$fight$monster( void ) {
    g_monsterHealthPoints -=
        ( 1 + ( g_playerExperiencePoints / EXPERIENCE_FOR_DAMAGE ) );

    if ( UNLIKELY( !g_monsterHealthPoints ) ||
         UNLIKELY( g_monsterHealthPoints > MAX_MONSTER_HEALTH_POINTS ) ) {
        g_monsterHealthPoints = MAX_MONSTER_HEALTH_POINTS;

        logic$player$experience$add( EXPERIENCE_FROM_MONSTER );

        return ( true );
    }

    return ( false );
}

FORCE_INLINE bool logic$player$fight$guardian( void ) {
    if ( LIKELY( !( logic$player$inventory$use( DEFENSE ) ) ) ) {
        g_playerHealthPoints -= 4;
    }

    g_guardianHealthPoints -=
        ( 1 + ( g_playerExperiencePoints / EXPERIENCE_FOR_DAMAGE_GUARDIAN ) +
          ( ( uint8_t )( logic$player$inventory$use( ATTACK ) ) * 5 ) );

    if ( UNLIKELY( !g_guardianHealthPoints ) ||
         UNLIKELY( g_guardianHealthPoints > MAX_GUARDIAN_HEALTH_POINTS ) ) {
        g_guardianHealthPoints = MAX_GUARDIAN_HEALTH_POINTS;

        logic$player$experience$add( EXPERIENCE_FROM_MONSTER );

        return ( true );
    }

    return ( false );
}

FORCE_INLINE bool logic$player$fight( const actor_t _who ) {
    bool l_returnValue = false;

    g_playerHealthPoints -= 1;

    if ( UNLIKELY( _who == GUARDIAN ) ) {
        l_returnValue = logic$player$fight$guardian();

    } else {
        l_returnValue = logic$player$fight$monster();

        if ( UNLIKELY( l_returnValue ) && UNLIKELY( _who == KEY_MONSTER ) ) {
            logic$player$inventory$add( KEY );
        }
    }

    // Has player died
    if ( UNLIKELY( !g_playerHealthPoints ) ||
         UNLIKELY( g_playerHealthPoints > MAX_HEALTH_POINTS ) ) {
        if ( UNLIKELY( logic$player$inventory$use( HEALTH ) ) ) {
            g_playerHealthPoints = MAX_HEALTH_POINTS;

            l_returnValue = true;

        } else {
            logic$player$lose$set();

            l_returnValue = false;
        }
    }

    return ( l_returnValue );
}

FORCE_INLINE bool logic$player$lose$get( void ) {
    return ( g_didLose );
}

FORCE_INLINE void logic$player$lose$set( void ) {
    g_didLose = true;
}

FORCE_INLINE void renderPoints( char** _cursorPosition,
                                uint8_t _points,
                                const size_t _textLength ) {
    const size_t l_lengthOfPoints = lengthOfNumber( _points );

    *_cursorPosition += ( _textLength - l_lengthOfPoints );

    convertNumberToString( *_cursorPosition, _points, l_lengthOfPoints );

    *_cursorPosition += ( l_lengthOfPoints - 1 );
}

// HP:100
// EXP:100
// ITEMS:E E E E E
FORCE_INLINE void logic$player$render$stats( void ) {
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

        FOR( const item_t*, g_playerItems ) {
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

void logic$player$render$debug( void ) {
#define MONSTER_TEXT "\nM:"
#define GUARDIAN_TEXT "\nB:"

#define FULL_MONSTER_TEXT MONSTER_TEXT HEALTH_POINTS_TEXT_PLACEHOLDER
#define FULL_GUARDIAN_TEXT GUARDIAN_TEXT HEALTH_POINTS_TEXT_PLACEHOLDER

    char l_buffer[] = FULL_MONSTER_TEXT FULL_GUARDIAN_TEXT;
    char* l_cursorPosition = ( l_buffer - 1 );

    renderPoints( &l_cursorPosition, g_monsterHealthPoints,
                  sizeof( FULL_MONSTER_TEXT ) );
    renderPoints( &l_cursorPosition, g_guardianHealthPoints,
                  sizeof( FULL_GUARDIAN_TEXT ) );

    print( l_buffer, ( sizeof( l_buffer ) - 1 ) );

#undef GUARDIAN_TEXT
#undef MONSTER_TEXT
}
