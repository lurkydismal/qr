#include "map.h"

#include "player.h"
#include "stdfunc.h"

// g_map represents the current state of the game map, with each character in
// the array corresponding to a specific tile in the game world. Each tile can
// be a wall, floor, decoration, player, monster, chest, or other items. This
// map is essential for game mechanics such as movement, actions, and
// encounters.
//
// The map is initialized as a constant string with a detailed arrangement of
// characters representing different elements in the game environment. This
// global map is used for navigating and interacting with the game world during
// gameplay.
char g_map[] =
    "0"
    "+------+     +-------+   M#                  K####T         \n"
    "|....K.|  ###}.......|    #             +----/----+        C\n"
    "|.@.....###  |.......|    #             |.........|        #\n"
    "|......|  #  |........###### ###########}.........|     W##M\n"
    "|......|     |.M.....|      K           |.........|     #   \n"
    "+------+     |...C...|            +-----+C........|     #   \n"
    "|.G....|     +-------+            |.....C.M.......{#####    \n"
    "|......+-----+                    |..........M....|         \n"
    ">............}K###################}....W..........|         \n"
    "+------------+                    +---------------+         \n";

// g_mapEmpty is a copy of g_map that is used to represent the empty state of
// the map, where opponents and actionable tiles are replaced with walkable
// ones. This map is used during certain game logic operations to track the
// initial state of the map.
char g_mapEmpty[ sizeof( g_map ) ];

// g_playerPosition tracks the current position of the player within the map. It
// is stored as a size_t representing the index of the player's location within
// the g_map array. This variable is critical for updating and rendering the
// player's movement on the map.
size_t g_playerPosition;

FORCE_INLINE bool isTileNotDecoration( char _tile ) {
    return ( ( _tile != WALL_HORIZONTAL ) && ( _tile != WALL_VERTICAL ) &&
             ( _tile != WALL_CROSS ) && ( _tile != LADDER_LEFT ) &&
             ( _tile != LADDER_RIGHT ) && ( _tile != ' ' ) &&
             ( _tile != '\n' ) && ( _tile != '0' ) );
}

bool isTileWalkable( char _tile ) {
    return ( ( _tile == FLOOR ) || ( _tile == BRIDGE ) );
}

FORCE_INLINE void logic$map$init( void ) {
    __builtin_memcpy( g_mapEmpty, g_map, sizeof( g_map ) );

    // Define directions for potential tile replacements
    // First 4 will empty the whole map
    const direction_t l_allDirections[] = {
        DOWN,       LEFT,      RIGHT,  UP_RIGHT, /* extra, clockwise */ UP,
        DOWN_RIGHT, DOWN_LEFT, UP_LEFT };

    // Empty map is a copy of map
    FOR( char*, g_mapEmpty ) {
        char* l_tile = _element;

        // Search for player
        if ( *l_tile == PLAYER ) {
            g_playerPosition = ( _element - g_mapEmpty );
        }

        // Generate empty map
        // Replace non-walkable, non-decorative tiles with walkable ones
        if ( UNLIKELY( isTileNotDecoration( *l_tile ) ) &&
             UNLIKELY( !( isTileWalkable( *l_tile ) ) ) ) {
            // Try to find a walkable replacement tile by checking the adjacent
            // tiles
            FOR( const direction_t*, l_allDirections ) {
                const char l_replacement = *( l_tile + *_element );

                if ( LIKELY( isTileWalkable( l_replacement ) ) ) {
                    *l_tile = l_replacement;

                    break;
                }
            }
        }
    }

    // Generate level
    FOR( char*, g_map ) {
        char* l_tile = _element;

        // If the tile represents a monster with a key, replace it with random
        // key monster
        if ( *l_tile == MONSTER_WITH_KEY ) {
            const actor_t l_keyMonsters[] = { KEY_MONSTER };

            *l_tile = randomValueFromArray( l_keyMonsters );
        }

        // If the tile represents a monster, replace it with random monster
        if ( *l_tile == MONSTER ) {
            const actor_t l_monsters[] = { FOLLOW_MONSTER, RANDOM_MONSTER };

            *l_tile = randomValueFromArray( l_monsters );
        }
    }
}

FORCE_INLINE bool tryFightTile( const char _tile ) {
    // Define the list of possible opponents represented by specific actor types
    const actor_t l_opponents[] = { FOLLOW_MONSTER, RANDOM_MONSTER, KEY_MONSTER,
                                    GUARDIAN };

    FOR( const actor_t*, l_opponents ) {
        if ( *_element == _tile ) {
            return ( logic$player$fight( _tile ) );
        }
    }

    // If no opponent is found, return false (no fight occurs)
    return ( false );
}

FORCE_INLINE void action$chest( void ) {
    // Define a list of possible items that can be found in the chest
    const item_t l_items[] = { HEALTH, ATTACK, DEFENSE };

    // Add a randomly selected item from the list to the player's inventory
    logic$player$inventory$add( randomValueFromArray( l_items ) );
}

FORCE_INLINE void action$treasure( void ) {
    logic$player$experience$add( EXPERIENCE_FROM_TREASURE );
}

FORCE_INLINE bool action$door( void ) {
    // Attempt to use a key from the player's inventory to open the door
    return ( logic$player$inventory$use( ( item_t )KEY ) );
}

FORCE_INLINE bool tryActionTile( const char _tile ) {
    // Use tile
    {
        switch ( _tile ) {
            case ( ( actionable_t )CHEST ): {
                action$chest();

                return ( true );
            }

            case ( ( actionable_t )TREASURE ): {
                action$treasure();

                return ( true );
            }

            case ( ( actionable_t )DOOR_LEFT ):
            case ( ( actionable_t )DOOR_RIGHT ):
            case ( ( actionable_t )DOOR_MIDDLE ): {
                return ( action$door() );
            }

            case ( ( actionable_t )LADDER_LEFT ):
            case ( ( actionable_t )LADDER_RIGHT ): {
                logic$player$lose$set();

                return ( false );
            }
        }
    }

    // Pick-up item
    {
        const item_t l_items[] = { KEY, HEALTH, ATTACK, DEFENSE };

        FOR( const item_t*, l_items ) {
            if ( *_element == _tile ) {
                logic$player$inventory$add( _tile );

                return ( true );
            }
        }
    }

    // Return false if no action or item pickup occurred
    return ( false );
}

FORCE_INLINE void logic$map$move$player( const direction_t _direction ) {
    const size_t l_oldPosition = g_playerPosition;
    const size_t l_newPosition = ( g_playerPosition + _direction );

    const char l_tile = g_map[ l_newPosition ];

    bool l_needMove = false;

    if ( LIKELY( isTileWalkable( l_tile ) ) ) {
        l_needMove = true;

    } else if ( UNLIKELY( tryFightTile( l_tile ) ) ) {
        l_needMove = true;

    } else if ( UNLIKELY( tryActionTile( l_tile ) ) ) {
        l_needMove = true;
    }

    if ( LIKELY( l_needMove ) ) {
        g_map[ l_oldPosition ] = g_mapEmpty[ l_oldPosition ];

        g_map[ l_newPosition ] = PLAYER;

        g_playerPosition = l_newPosition;
    }
}

void logic$map$move( const actor_t _who,
                     const size_t _currentPosition,
                     const direction_t _direction ) {
    const size_t l_oldPosition = _currentPosition;
    const size_t l_newPosition = ( _currentPosition + _direction );

    const char l_tile = g_map[ l_newPosition ];

    if ( LIKELY( isTileWalkable( l_tile ) ) ) {
        g_map[ l_oldPosition ] = g_mapEmpty[ l_oldPosition ];

        g_map[ l_newPosition ] = _who;

    } else if ( UNLIKELY( l_tile == PLAYER ) ) {
        if ( UNLIKELY( tryFightTile( _who ) ) ) {
            g_map[ l_oldPosition ] = g_mapEmpty[ l_oldPosition ];
        }
    }
}

void logic$map$move$random( const actor_t _who,
                            const size_t _currentPosition ) {
    // Define all possible movement directions, including staying in place
    const direction_t l_allDirections[] = {
        DOWN, LEFT, RIGHT, UP_RIGHT, UP, DOWN_RIGHT, DOWN_LEFT, UP_LEFT, STAY };

    logic$map$move( _who, _currentPosition,
                    randomValueFromArray( l_allDirections ) );
}

// TODO: Add movement to follow the player if close
#if 0
void logic$map$move$follow( const actor_t _who,
                            const size_t _currentPosition ) {
    const size_t l_playerPositionX = ( g_playerPosition % MAP_WIDTH );
    const size_t l_playerPositionY = ( g_playerPosition / MAP_WIDTH );
    const size_t l_whoPositionX = ( _currentPosition % MAP_WIDTH );
    const size_t l_whoPositionY = ( _currentPosition / MAP_WIDTH );
    size_t l_whoNewPosition = _currentPosition;

    if ( l_playerPositionX > l_whoPositionX ) {
        l_whoNewPosition += ( direction_t )RIGHT;

    } else if ( l_playerPositionX < l_whoPositionX ) {
        l_whoNewPosition += ( direction_t )LEFT;
    }

    if ( l_playerPositionY > l_whoPositionY ) {
        l_whoNewPosition += ( direction_t )DOWN;

    } else if ( l_playerPositionY < l_whoPositionY ) {
        l_whoNewPosition += ( direction_t )UP;
    }

    logic$map$move( _who, _currentPosition, l_whoNewPosition );
}
#endif

FORCE_INLINE void logic$map$move$ai( void ) {
    FOR( const char*, g_map ) {
        const char l_tile = *_element;
        const size_t l_position = ( _element - g_map );

        switch ( l_tile ) {
            case ( RANDOM_MONSTER ): {
                logic$map$move$random( ( actor_t )RANDOM_MONSTER, l_position );

                break;
            }

            case ( FOLLOW_MONSTER ): {
                // TODO: Add movement to follow the player if close
                logic$map$move$random( ( actor_t )FOLLOW_MONSTER, l_position );

                break;
            }

            case ( KEY_MONSTER ): {
                logic$map$move$random( ( actor_t )KEY_MONSTER, l_position );

                break;
            }

            case ( GUARDIAN ): {
                // TODO: Add movement to follow the player if close
                logic$map$move$random( ( actor_t )GUARDIAN, l_position );

                break;
            }
        }
    }
}

FORCE_INLINE void logic$map$render( void ) {
    // Iterate through each tile in the map (g_map)
    FOR( const char*, g_map ) {
        // Create a buffer of size 1 to hold the current tile's character
        char l_buffer[ sizeof( char ) ] = { *_element };

        // Print the current tile's character
        print( l_buffer, 1 * sizeof( char ) );
    }
}
