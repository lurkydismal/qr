#pragma once

#include "stdfunc.hpp"

// Player
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>

#define START_HEALTH_POINTS 100
#define START_EXPERIENCE_POINTS 0

#define MAX_HEALTH_POINTS 200
#define MAX_EXPERIENCE_POINTS 100
#define MAX_ITEM_COUNT 5

#define EXPERIENCE_FROM_MONSTER 5
#define EXPERIENCE_FROM_GUARDIAN 10
#define EXPERIENCE_FROM_TREASURE 20

#define EXPERIENCE_FOR_DAMAGE 10
#define EXPERIENCE_FOR_DAMAGE_GUARDIAN 20

#define MAX_MONSTER_HEALTH_POINTS 5
#define MAX_GUARDIAN_HEALTH_POINTS 20

#define START_MONSTER_HEALTH_POINTS MAX_MONSTER_HEALTH_POINTS
#define START_GUARDIAN_HEALTH_POINTS MAX_GUARDIAN_HEALTH_POINTS

#define HEALTH_TEXT "\nHP:"
#define EXPERIENCE_TEXT "\nEXP:"
#define ITEMS_TEXT "\nITEMS:"

#define HEALTH_POINTS_TEXT_PLACEHOLDER "000"
#define EXPERIENCE_POINTS_TEXT_PLACEHOLDER "000"
#define ITEMS_TEXT_PLACEHOLDER "E E E E E"

using item_t = enum {
    EMPTY = 'E',
    KEY = 'K',
    HEALTH = 'H',
    ATTACK = 'A',
    DEFENSE = 'D'
};

using actor_t = enum {
    PLAYER = '@',
    GUARDIAN = 'G',
    FOLLOW_MONSTER = 'F',
    RANDOM_MONSTER = 'R',
    KEY_MONSTER = 'O',
    MONSTER = 'M',
    MONSTER_WITH_KEY = 'W'
};

// Inventory
void logic$player$experience$add( const size_t _amount );
void logic$player$inventory$add( const item_t _item );
auto logic$player$inventory$use( const item_t _item ) -> bool;

// Stats
auto logic$player$fight( const actor_t _who ) -> bool;
auto logic$player$lose$get() -> bool;
void logic$player$lose$set();

// Render
void logic$player$render$stats();
void logic$player$render$debug();

// C
uint8_t g_playerHealthPoints = START_HEALTH_POINTS;
uint8_t g_playerExperiencePoints = START_EXPERIENCE_POINTS;
item_t g_playerItems[ MAX_ITEM_COUNT ] = { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };

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
        if ( *_element == EMPTY ) [[unlikely]] {
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

    if ( ( !g_monsterHealthPoints ) ||
         ( g_monsterHealthPoints > MAX_MONSTER_HEALTH_POINTS ) ) [[unlikely]] {
        g_monsterHealthPoints = MAX_MONSTER_HEALTH_POINTS;

        logic$player$experience$add( EXPERIENCE_FROM_MONSTER );

        return ( true );
    }

    return ( false );
}

FORCE_INLINE bool logic$player$fight$guardian( void ) {
    if ( !( logic$player$inventory$use( DEFENSE ) ) ) [[likely]] {
        g_playerHealthPoints -= 4;
    }

    g_guardianHealthPoints -=
        ( 1 + ( g_playerExperiencePoints / EXPERIENCE_FOR_DAMAGE_GUARDIAN ) +
          ( ( uint8_t )( logic$player$inventory$use( ATTACK ) ) * 5 ) );

    if ( ( !g_guardianHealthPoints ) ||
         ( g_guardianHealthPoints > MAX_GUARDIAN_HEALTH_POINTS ) )
        [[unlikely]] {
        g_guardianHealthPoints = MAX_GUARDIAN_HEALTH_POINTS;

        logic$player$experience$add( EXPERIENCE_FROM_MONSTER );

        return ( true );
    }

    return ( false );
}

FORCE_INLINE bool logic$player$fight( const actor_t _who ) {
    bool l_returnValue = false;

    g_playerHealthPoints -= 1;

    if ( _who == GUARDIAN ) [[unlikely]] {
        l_returnValue = logic$player$fight$guardian();

    } else {
        l_returnValue = logic$player$fight$monster();

        if ( ( l_returnValue ) && ( _who == KEY_MONSTER ) ) [[unlikely]] {
            logic$player$inventory$add( KEY );
        }
    }

    // Has player died
    if ( ( !g_playerHealthPoints ) ||
         ( g_playerHealthPoints > MAX_HEALTH_POINTS ) ) [[unlikely]] {
        if ( logic$player$inventory$use( HEALTH ) ) [[unlikely]] {
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

// MAP
#define MAP_WIDTH 61

using tile_t = enum {
    FLOOR = '.',
    BRIDGE = '#',
    WALL_HORIZONTAL = '-',
    WALL_VERTICAL = '|',
    WALL_CROSS = '+'
};

using direction_t = enum {
    STAY = 0,
    UP = -( MAP_WIDTH ),
    UP_RIGHT = -( MAP_WIDTH - 1 ),
    RIGHT = 1,
    DOWN_RIGHT = ( MAP_WIDTH + 1 ),
    DOWN = MAP_WIDTH,
    DOWN_LEFT = ( MAP_WIDTH - 1 ),
    LEFT = -1,
    UP_LEFT = -( MAP_WIDTH + 1 )
};

using actionable_t = enum {
    CHEST = 'C',
    TREASURE = 'T',
    DOOR_LEFT = '}',
    DOOR_RIGHT = '{',
    DOOR_MIDDLE = '/',
    LADDER_LEFT = '>',
    LADDER_RIGHT = '<'
};

// Positions
/**
 * @brief Initializes the game map.
 *
 * This function initializes the game map by copying the original map to an
 * empty map, ensuring that all opponents and actionable tiles are replaced with
 * adjacent walkable tiles, and the player's starting position is identified. It
 * also places monsters in the game world.
 *
 * @details
 * The function performs the following steps:
 * 1. Copies the current map (`g_map`) to a new empty map (`g_mapEmpty`).
 * 2. Identifies the player's starting position by searching for the tile marked
 * as `PLAYER`.
 * 3. Ensures that all opponents and actionable tiles (e.g., door tiles) are
 *    replaced with walkable tiles from adjacent directions (e.g., floor tiles).
 * 4. Randomly generates monsters placed on the map:
 *    - If the tile contains a `MONSTER_WITH_KEY`, it is replaced with a
 * randomly chosen key monster.
 *    - If the tile contains a `MONSTER`, it is replaced with a randomly chosen
 * monster.
 *
 * The function ensures that the map is prepared and ready for gameplay by
 * setting the player’s position and ensuring that tiles are walkable.
 *
 * @note This function modifies the following global variables:
 *       - `g_map`: The original map layout.
 *       - `g_mapEmpty`: The map that has been processed for walkability and
 * content.
 *       - `g_playerPosition`: The player's starting position on the map.
 */
void logic$map$init();

/**
 * @brief Moves the player on the map in a specified direction.
 *
 * This function handles the logic for moving the player on the game map. It
 * checks if the destination tile is walkable, if there's a fight with an
 * opponent, or if an action can be performed on the destination tile. If
 * movement is allowed, it updates the map by moving the player to the new
 * position and adjusting the old position to reflect the movement.
 *
 * The function follows these steps:
 * 1. It calculates the player's new position based on the given direction.
 * 2. It checks if the destination tile is walkable.
 * 3. If the destination is not walkable, it checks if the player can fight the
 *    opponent present on the destination tile.
 * 4. If an action is possible (such as interacting with a chest or door), the
 *    function tries to trigger that action.
 * 5. If movement, fight, or action is successful, the player's position is
 *    updated accordingly.
 *
 * @param _direction The direction in which the player wants to move (one of the
 *                   defined movement directions, such as UP, DOWN, LEFT, etc.).
 *
 * @note The player's position is tracked globally by `g_playerPosition`. This
 *       function ensures that the player's position on the map is updated based
 *       on their movement or interactions.
 */
void logic$map$move$player( const direction_t _direction );

/**
 * @brief Moves all AI-controlled monsters randomly on the map.
 *
 * This function iterates through each tile on the map, and for each tile that
 * contains a monster (RANDOM_MONSTER, FOLLOW_MONSTER, KEY_MONSTER, or
 * GUARDIAN), it moves the corresponding monster to a random adjacent position
 * by calling the `logic$map$move$random` function. The movement is based on the
 * monster's type, ensuring that each specific type of monster is moved
 * according to predefined behavior.
 *
 * @note This function uses the FOR macro to iterate over the entire map and
 * process each tile.
 */
void logic$map$move$ai();

// Render
/**
 * @brief Renders the current state of the map to the output.
 *
 * This function iterates through each tile of the global `g_map` array and
 * prints the character representing the tile. It essentially outputs the map as
 * it is in the game's internal state, tile by tile.
 *
 * - Each tile in `g_map` is represented by a single character that could be
 *   a floor, wall, player, monster, chest, treasure, etc.
 *
 * This function assumes that the map data (`g_map`) is structured in a way
 * where each element corresponds to a character that needs to be printed to the
 * screen.
 *
 * @note The rendering is done one character at a time. Depending on how the
 * `print` function works, it could render a single line or the entire map one
 * by one. Make sure the print function handles newlines and coordinates
 * correctly.
 *
 * @see `print()` for more information on how tiles are printed to the screen.
 */
void logic$map$render();

// C

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

#if 0
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
#endif

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
    // std::ranges::copy( g_map, g_mapEmpty );
    memcpy( g_mapEmpty, g_map, sizeof( g_map ) );

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
        if ( isTileNotDecoration( *l_tile ) && !isTileWalkable( *l_tile ) )
            [[unlikely]] {
            // Try to find a walkable replacement tile by checking the adjacent
            // tiles
            FOR( const direction_t*, l_allDirections ) {
                const char l_replacement = *( l_tile + *_element );

                if ( isTileWalkable( l_replacement ) ) [[likely]] {
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
            return ( logic$player$fight( ( actor_t )_tile ) );
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
                logic$player$inventory$add( ( item_t )_tile );

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

    if ( isTileWalkable( l_tile ) ) [[likely]] {
        l_needMove = true;

    } else if ( tryFightTile( l_tile ) ) [[unlikely]] {
        l_needMove = true;

    } else if ( tryActionTile( l_tile ) ) [[unlikely]] {
        l_needMove = true;
    }

    if ( l_needMove ) [[likely]] {
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

    if ( isTileWalkable( l_tile ) ) [[likely]] {
        g_map[ l_oldPosition ] = g_mapEmpty[ l_oldPosition ];

        g_map[ l_newPosition ] = _who;

    } else if ( l_tile == PLAYER ) [[unlikely]] {
        if ( tryFightTile( _who ) ) [[unlikely]] {
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

// LOGIC C
#if 0
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
#endif
