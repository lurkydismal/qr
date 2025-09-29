#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>

#include "io.hpp"
#include "stdfunc.hpp"

namespace logic {

constexpr size_t g_startHealthPoints = 100;
constexpr size_t g_startExperiencePoints = 0;

constexpr size_t g_maxHealthPoints = 200;
constexpr size_t g_maxExperiencePoints = 100;
constexpr size_t g_maxItemCount = 5;

constexpr size_t g_maxMonsterHealthPoints = 5;
constexpr size_t g_maxGuardianHealthPoints = 20;

constexpr size_t g_startMonsterHealthPoints = g_maxMonsterHealthPoints;
constexpr size_t g_startGuardianHealthPoints = g_maxGuardianHealthPoints;

#define HEALTH_TEXT "\nHP:"
#define EXPERIENCE_TEXT "\nEXP:"
#define ITEMS_TEXT "\nITEMS:"

#define HEALTH_POINTS_TEXT_PLACEHOLDER "000"
#define EXPERIENCE_POINTS_TEXT_PLACEHOLDER "000"
#define ITEMS_TEXT_PLACEHOLDER "E E E E E"

using item_t = enum class item : uint8_t {
    empty = 'E',
    key = 'K',
    health = 'H',
    attack = 'A',
    defense = 'D'
};

using actor_t = enum class actor : uint8_t {
    player = '@',
    guardian = 'G',
    followMonster = 'F',
    randomMonster = 'R',
    keyMonster = 'O',
    monster = 'M',
    monsterWithKey = 'W'
};

uint8_t g_monsterHealthPoints = g_startMonsterHealthPoints;
uint8_t g_guardianHealthPoints = g_startGuardianHealthPoints;

namespace monster {

FORCE_INLINE constexpr auto fight() -> bool;

} // namespace monster

namespace guardian {

FORCE_INLINE constexpr auto fight() -> bool;

}

namespace player {

// player::position tracks the current position of the player within the map. It
// is stored as a size_t representing the index of the player's location within
// the g_map array. This variable is critical for updating and rendering the
// player's movement on the map.
size_t g_position;

uint8_t g_playerHealthPoints = g_startHealthPoints;
uint8_t g_playerExperiencePoints = g_startExperiencePoints;
auto g_playerItems = std::array{ item_t::empty, item_t::empty, item_t::empty,
                                 item_t::empty, item_t::empty };

namespace lose {

bool g_didLose = false;

FORCE_INLINE constexpr void set() {
    g_didLose = true;
}

[[nodiscard]] FORCE_INLINE constexpr auto get() -> bool {
    return ( g_didLose );
}

} // namespace lose

namespace experience {

constexpr size_t g_fromMonster = 5;
constexpr size_t g_fromGuardian = 10;
constexpr size_t g_fromTreasure = 20;

constexpr size_t g_forDamage = 10;
constexpr size_t g_forDamangeGuardian = 20;

NOINLINE constexpr void add( size_t _amount ) {
    g_playerExperiencePoints += _amount;

    if ( g_playerExperiencePoints > g_maxExperiencePoints ) {
        g_playerExperiencePoints = g_maxExperiencePoints;
    }
}

} // namespace experience

namespace inventory {

constexpr void add( item_t _item ) {
    for ( item_t& _slot : g_playerItems ) {
        if ( _slot == item_t::empty ) {
            _slot = _item;

            return;
        }
    }

    g_playerItems[ 0 ] = _item;
}

[[nodiscard]] constexpr auto use( item_t _item ) -> bool {
    for ( item_t& _slot : g_playerItems ) {
        if ( _slot == _item ) {
            _slot = item_t::empty;

            return ( true );
        }
    }

    return ( false );
}

} // namespace inventory

[[nodiscard]] constexpr auto fight( actor_t _who ) -> bool {
    bool l_returnValue = false;

    g_playerHealthPoints -= 1;

    if ( _who == actor_t::guardian ) [[unlikely]] {
        l_returnValue = guardian::fight();

    } else {
        l_returnValue = monster::fight();

        if ( ( l_returnValue ) && ( _who == actor_t::keyMonster ) )
            [[unlikely]] {
            inventory::add( item_t::key );
        }
    }

    // Has player died
    if ( ( !g_playerHealthPoints ) ||
         ( g_playerHealthPoints > g_maxHealthPoints ) ) [[unlikely]] {
        if ( inventory::use( item_t::health ) ) [[unlikely]] {
            g_playerHealthPoints = g_maxHealthPoints;

            l_returnValue = true;

        } else {
            lose::set();

            l_returnValue = false;
        }
    }

    return ( l_returnValue );
}

// Stats
FORCE_INLINE void renderPoints( char** _cursorPosition,
                                uint8_t _points,
                                size_t _textLength ) {
    const size_t l_lengthOfPoints = stdfunc::lengthOfNumber( _points );

    *_cursorPosition += ( _textLength - l_lengthOfPoints );

    stdfunc::convertNumberToString( *_cursorPosition, _points,
                                    l_lengthOfPoints );

    *_cursorPosition += ( l_lengthOfPoints - 1 );
}

// Render
// HP:100
// EXP:100
// ITEMS:E E E E E
FORCE_INLINE void render$stats() {
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

        for ( item_t& _item : g_playerItems ) {
            *l_cursorPosition = ( char )( _item );

            // Move to the next character
            l_cursorPosition++;

            // Skip delimiter
            l_cursorPosition++;
        }
    }

    io::print( l_buffer, ( sizeof( l_buffer ) - 1 ) );

#undef FULL_ITEMS_TEXT
#undef FULL_EXPERIENCE_TEXT
#undef FULL_HEALTH_TEXT
}

// TODO: Unused
void render$debug() {
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

    io::print( l_buffer, ( sizeof( l_buffer ) - 1 ) );

#undef GUARDIAN_TEXT
#undef MONSTER_TEXT
}

} // namespace player

[[nodiscard]] constexpr auto monster::fight() -> bool {
    g_monsterHealthPoints -= ( 1 + ( player::g_playerExperiencePoints /
                                     player::experience::g_forDamage ) );

    if ( ( !g_monsterHealthPoints ) ||
         ( g_monsterHealthPoints > g_maxMonsterHealthPoints ) ) [[unlikely]] {
        g_monsterHealthPoints = g_maxMonsterHealthPoints;

        player::experience::add( player::experience::g_fromMonster );

        return ( true );
    }

    return ( false );
}

[[nodiscard]] constexpr auto guardian::fight() -> bool {
    if ( !( player::inventory::use( item_t::defense ) ) ) [[likely]] {
        player::g_playerHealthPoints -= 4;
    }

    g_guardianHealthPoints -=
        ( 1 +
          ( player::g_playerExperiencePoints /
            player::experience::g_forDamangeGuardian ) +
          ( ( uint8_t )( player::inventory::use( item_t::attack ) ) * 5 ) );

    if ( ( !g_guardianHealthPoints ) ||
         ( g_guardianHealthPoints > g_maxGuardianHealthPoints ) ) [[unlikely]] {
        g_guardianHealthPoints = g_maxGuardianHealthPoints;

        player::experience::add( player::experience::g_fromGuardian );

        return ( true );
    }

    return ( false );
}

namespace map {

#define MAP                                                          \
    "+------+     +-------+   M#                  K####T         \n" \
    "|....K.|  ###}.......|    #             +----/----+        C\n" \
    "|.@.....###  |.......|    #             |.........|        #\n" \
    "|......|  #  |........###### ###########}.........|     W##M\n" \
    "|......|     |.M.....|      K           |.........|     #   \n" \
    "+------+     |...C...|            +-----+C........|     #   \n" \
    "|.G....|     +-------+            |.....C.M.......{#####    \n" \
    "|......+-----+                    |..........M....|         \n" \
    ">............}K###################}....W..........|         \n" \
    "+------------+                    +---------------+         \n"

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
char g_map[] = MAP;

// g_mapEmpty is a copy of g_map that is used to represent the empty state of
// the map, where opponents and actionable tiles are replaced with walkable
// ones. This map is used during certain game logic operations to track the
// initial state of the map.
char g_mapEmpty[ sizeof( g_map ) ];

constexpr ssize_t g_mapWidth = ( std::string_view{ MAP }.find( '\n' ) + 1 );

using tile_t = enum class tile : uint8_t {
    floor = '.',
    bridge = '#',
    wallHorizontal = '-',
    wallVertical = '|',
    wallCross = '+'
};

using direction_t = enum class direction : int8_t {
    stay = 0,
    up = -( g_mapWidth ),
    upRight = -( g_mapWidth - 1 ),
    right = 1,
    downRight = ( g_mapWidth + 1 ),
    down = g_mapWidth,
    downLeft = ( g_mapWidth - 1 ),
    left = -1,
    upLeft = -( g_mapWidth + 1 )
};

using actionable_t = enum class actionable : uint8_t {
    chest = 'C',
    treasure = 'T',
    doorLeft = '}',
    doorRight = '{',
    doorMiddle = '/',
    ladderLeft = '>',
    ladderRight = '<'
};

[[nodiscard]] FORCE_INLINE constexpr auto isTileNotDecoration( char _tile )
    -> bool {
    return ( ( _tile != ( char )tile_t::wallHorizontal ) &&
             ( _tile != ( char )tile_t::wallVertical ) &&
             ( _tile != ( char )tile_t::wallCross ) &&
             ( _tile != ( char )actionable_t::ladderLeft ) &&
             ( _tile != ( char )actionable_t::ladderRight ) &&
             ( _tile != ' ' ) && ( _tile != '\n' ) && ( _tile != '0' ) );
}

[[nodiscard]] constexpr auto isTileWalkable( tile_t _tile ) -> bool {
    return ( ( _tile == tile_t::floor ) || ( _tile == tile_t::bridge ) );
}

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
 *       - `player::osition`: The player's starting position on the map.
 */
FORCE_INLINE void init() {
    std::ranges::copy( g_map, g_mapEmpty );

    // Define directions for potential tile replacements
    // First 4 will empty the whole map
    constexpr direction_t l_allDirections[] = {
        direction_t::down,
        direction_t::left,
        direction_t::right,
        direction_t::upRight,
        /* extra, clockwise */ direction_t::up,
        direction_t::downRight,
        direction_t::downLeft,
        direction_t::upLeft };

    // Empty map is a copy of map
    FOR( char*, g_mapEmpty ) {
        char* l_tile = _element;

        // Search for player
        if ( *l_tile == ( char )actor_t::player ) [[unlikely]] {
            player::g_position = ( _element - g_mapEmpty );
        }

        // Generate empty map
        // Replace non-walkable, non-decorative tiles with walkable ones
        if ( isTileNotDecoration( *l_tile ) &&
             !isTileWalkable( ( tile_t )*l_tile ) ) [[unlikely]] {
            // Try to find a walkable replacement tile by checking the adjacent
            // tiles
            for ( direction_t _direction : l_allDirections ) {
                const auto l_replacement =
                    ( tile_t )( *( l_tile + ( char )_direction ) );

                if ( isTileWalkable( l_replacement ) ) [[likely]] {
                    *l_tile = ( char )l_replacement;

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
        if ( *l_tile == ( char )actor_t::monsterWithKey ) [[unlikely]] {
            constexpr actor_t l_keyMonsters[] = { actor_t::keyMonster };

            *l_tile = ( char )randomValueFromContainer( l_keyMonsters );
        }

        // If the tile represents a monster, replace it with random monster
        if ( *l_tile == ( char )actor_t::monster ) [[unlikely]] {
            constexpr actor_t l_monsters[] = { actor_t::followMonster,
                                               actor_t::randomMonster };

            *l_tile = ( char )randomValueFromContainer( l_monsters );
        }
    }
}

[[nodiscard]] FORCE_INLINE constexpr auto tryFightTile( char _tile ) -> bool {
    // Define the list of possible opponents represented by specific actor types
    const actor_t l_opponents[] = { actor_t::followMonster,
                                    actor_t::randomMonster, actor_t::keyMonster,
                                    actor_t::guardian };

    FOR( const actor_t*, l_opponents ) {
        if ( *_element == ( actor_t )_tile ) {
            return ( player::fight( ( actor_t )_tile ) );
        }
    }

    // If no opponent is found, return false (no fight occurs)
    return ( false );
}

FORCE_INLINE constexpr void action$chest() {
    // Define a list of possible items that can be found in the chest
    const item_t l_items[] = { item_t::health, item_t::attack,
                               item_t::defense };

    // Add a randomly selected item from the list to the player's inventory
    player::inventory::add( randomValueFromContainer( l_items ) );
}

FORCE_INLINE constexpr void action$treasure() {
    player::experience::add( player::experience::g_fromTreasure );
}

[[nodiscard]] FORCE_INLINE constexpr auto action$door() -> bool {
    // Attempt to use a key from the player's inventory to open the door
    return ( player::inventory::use( item_t::key ) );
}

[[nodiscard]] FORCE_INLINE constexpr auto tryActionTile( char _tile ) -> bool {
    // Use tile
    {
        switch ( ( actionable_t )_tile ) {
            case ( actionable_t::chest ): {
                action$chest();

                return ( true );
            }

            case ( actionable_t::treasure ): {
                action$treasure();

                return ( true );
            }

            case ( actionable_t::doorLeft ):
            case ( actionable_t::doorRight ):
            case ( actionable_t::doorMiddle ): {
                return ( action$door() );
            }

            case ( actionable_t::ladderLeft ):
            case ( actionable_t::ladderRight ): {
                player::lose::set();

                return ( false );
            }
        }
    }

    // Pick-up item
    {
        const item_t l_items[] = { item_t::key, item_t::health, item_t::attack,
                                   item_t::defense };

        FOR( const item_t*, l_items ) {
            if ( *_element == ( item_t )_tile ) {
                player::inventory::add( ( item_t )_tile );

                return ( true );
            }
        }
    }

    // Return false if no action or item pickup occurred
    return ( false );
}

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
 * @note The player's position is tracked globally by `player::position`. This
 *       function ensures that the player's position on the map is updated based
 *       on their movement or interactions.
 */
FORCE_INLINE constexpr void move$player( direction_t _direction ) {
    const size_t l_oldPosition = player::g_position;
    const size_t l_newPosition = ( player::g_position + ( size_t )_direction );

    const char l_tile = g_map[ l_newPosition ];

    bool l_needMove = false;

    if ( isTileWalkable( ( tile_t )l_tile ) ) [[likely]] {
        l_needMove = true;

    } else if ( tryFightTile( l_tile ) ) [[unlikely]] {
        l_needMove = true;

    } else if ( tryActionTile( l_tile ) ) [[unlikely]] {
        l_needMove = true;
    }

    if ( l_needMove ) [[likely]] {
        g_map[ l_oldPosition ] = g_mapEmpty[ l_oldPosition ];

        g_map[ l_newPosition ] = ( char )actor_t::player;

        player::g_position = l_newPosition;
    }
}

constexpr void move( actor_t _who,
                     size_t _currentPosition,
                     direction_t _direction ) {
    const size_t l_oldPosition = _currentPosition;
    const size_t l_newPosition = ( _currentPosition + ( size_t )_direction );

    const char l_tile = g_map[ l_newPosition ];

    if ( isTileWalkable( ( tile_t )l_tile ) ) [[likely]] {
        g_map[ l_oldPosition ] = g_mapEmpty[ l_oldPosition ];

        g_map[ l_newPosition ] = ( char )_who;

    } else if ( l_tile == ( char )actor_t::player ) [[unlikely]] {
        if ( tryFightTile( ( char )_who ) ) [[unlikely]] {
            g_map[ l_oldPosition ] = g_mapEmpty[ l_oldPosition ];
        }
    }
}

constexpr void move$random( actor_t _who, size_t _currentPosition ) {
    // Define all possible movement directions, including staying in place
    const direction_t l_allDirections[] = {
        direction_t::down,     direction_t::left,   direction_t::right,
        direction_t::upRight,  direction_t::up,     direction_t::downRight,
        direction_t::downLeft, direction_t::upLeft, direction_t::stay };

    move( _who, _currentPosition, randomValueFromContainer( l_allDirections ) );
}

// TODO: Add movement to follow the player if close
#if 0
constexpr void move$follow( const actor_t _who,
                            const size_t _currentPosition ) {
    const size_t l_playerPositionX = ( player::position % MAP_WIDTH );
    const size_t l_playerPositionY = ( player::position / MAP_WIDTH );
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

    move( _who, _currentPosition, l_whoNewPosition );
}
#endif

/**
 * @brief Moves all AI-controlled monsters randomly on the map.
 *
 * This function iterates through each tile on the map, and for each tile that
 * contains a monster (RANDOM_MONSTER, FOLLOW_MONSTER, KEY_MONSTER, or
 * GUARDIAN), it moves the corresponding monster to a random adjacent position
 * by calling the `move$random` function. The movement is based on the
 * monster's type, ensuring that each specific type of monster is moved
 * according to predefined behavior.
 *
 * @note This function uses the FOR macro to iterate over the entire map and
 * process each tile.
 */
FORCE_INLINE constexpr void move$ai() {
    FOR( const char*, g_map ) {
        const char l_tile = *_element;
        const size_t l_position = ( _element - g_map );

        switch ( ( actor_t )l_tile ) {
            case ( actor_t::randomMonster ): {
                move$random( actor_t::randomMonster, l_position );

                break;
            }

            case ( actor_t::followMonster ): {
                // TODO: Add movement to follow the player if close
                move$random( actor_t::followMonster, l_position );

                break;
            }

            case ( actor_t::keyMonster ): {
                move$random( actor_t::keyMonster, l_position );

                break;
            }

            case ( actor_t::guardian ): {
                // TODO: Add movement to follow the player if close
                move$random( actor_t::guardian, l_position );

                break;
            }
        }
    }
}

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
FORCE_INLINE constexpr void render() {
    // Iterate through each tile in the map (g_map)
    FOR( const char*, g_map ) {
        // Create a buffer of size 1 to hold the current tile's character
        char l_buffer[ sizeof( char ) ] = { *_element };

        // Print the current tile's character
        io::print( l_buffer, 1 * sizeof( char ) );
    }
}

} // namespace map

} // namespace logic
