#pragma once

#include <cstddef>
#include <cstdint>
#include <ranges>

#include "io.hpp"
#include "random.hpp"
#include "render.hpp"
#include "stdfunc.hpp"

namespace logic {

namespace map {

using tile_t = enum class tile : uint8_t {
    // Walkable
    floor = '.',
    bridge = '#',

    // Not walkable
    wallHorizontal = '-',
    wallVertical = '|',
    wallCross = '+',
};

using actionable_t = enum class actionable : uint8_t {
    chest = 'C',
    treasure = 'T',
    doorLeft = '}',
    doorRight = '{',
    doorMiddle = '/',
    ladderLeft = '>',
    ladderRight = '<',
};

using actor_t = enum class actor : uint8_t {
    player = '@',
    guardian = 'G',
    followMonster = 'F',
    randomMonster = 'R',
    keyMonster = 'O',

    // Used during map generation
    monster = 'M',
    monsterWithKey = 'W',
};

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

// g_current represents the current state of the game map, with each character
// in the array corresponding to a specific tile in the game world. Each tile
// can be a wall, floor, decoration, player, monster, chest, or other items.
// This map is essential for game mechanics such as movement, actions, and
// encounters.
//
// The map is initialized as a constant string with a detailed arrangement of
// characters representing different elements in the game environment. This
// global map is used for navigating and interacting with the game world during
// gameplay.
std::array g_current = std::to_array( MAP );

// g_empty is a copy of g_current that is used to represent the empty state
// of the map, where opponents and actionable tiles are replaced with walkable
// ones. This map is used during certain game logic operations to track the
// initial state of the map.
std::array< char, g_current.size() > g_empty{};

constexpr ssize_t g_width = ( std::string_view{ MAP }.find( '\n' ) + 1 );

using direction_t = enum class direction : int8_t {
    stay = 0,
    up = -( g_width ),
    upRight = -( g_width - 1 ),
    right = 1,
    downRight = ( g_width + 1 ),
    down = g_width,
    downLeft = ( g_width - 1 ),
    left = -1,
    upLeft = -( g_width + 1 ),
};

[[nodiscard]] FORCE_INLINE constexpr auto isTileNotDecoration( char _tile )
    -> bool;

[[nodiscard]] constexpr auto isTileWalkable( char _tile ) -> bool;

[[nodiscard]] FORCE_INLINE constexpr auto tryActionTile( char _tile ) -> bool;

} // namespace map

#define HEALTH_TEXT "\nHP:"
#define EXPERIENCE_TEXT "\nEXP:"
#define ITEMS_TEXT "\nITEMS:"

#define HEALTH_POINTS_TEXT_PLACEHOLDER "000"
#define EXPERIENCE_POINTS_TEXT_PLACEHOLDER "000"
#define ITEMS_TEXT_PLACEHOLDER "E E E E E"

namespace monster {

constexpr size_t g_maxHealth = 5;
constexpr size_t g_startHealth = g_maxHealth;

namespace stats::health {

uint8_t g_amount = g_startHealth;

}

[[nodiscard]] FORCE_INLINE constexpr auto fight() -> bool;

} // namespace monster

namespace guardian {

constexpr size_t g_maxHealth = 20;
constexpr size_t g_startHealth = g_maxHealth;

namespace stats::health {

uint8_t g_amount = g_startHealth;

}

[[nodiscard]] FORCE_INLINE constexpr auto fight() -> bool;

} // namespace guardian

namespace player {

namespace lose {

bool g_didLose = false;

FORCE_INLINE constexpr void set() {
    g_didLose = true;
}

[[nodiscard]] FORCE_INLINE constexpr auto get() -> bool {
    return ( g_didLose );
}

} // namespace lose

namespace inventory {

constexpr size_t g_maxCapacity = 5;

using item_t = enum class item : uint8_t {
    empty = 'E',
    key = 'K',
    health = 'H',
    attack = 'A',
    defense = 'D',
};

std::array g_items{
    item_t::empty, item_t::empty, item_t::empty, item_t::empty, item_t::empty,
};

constexpr void add( item_t _item ) {
    for ( auto& _slot : g_items ) {
        if ( _slot == item_t::empty ) {
            _slot = _item;

            return;
        }
    }

    g_items[ 0 ] = _item;
}

[[nodiscard]] constexpr auto use( item_t _item ) -> bool {
    for ( auto& _slot : g_items ) {
        if ( _slot == _item ) {
            _slot = item_t::empty;

            return ( true );
        }
    }

    return ( false );
}

} // namespace inventory

namespace stats {

namespace health {

constexpr size_t g_startHealthPoints = 20;
constexpr size_t g_maxHealthPoints = 60;

uint8_t g_amount = g_startHealthPoints;

} // namespace health

namespace experience {

constexpr size_t g_startExperiencePoints = 0;
constexpr size_t g_maxExperiencePoints = 100;

uint8_t g_amount = g_startExperiencePoints;

constexpr size_t g_fromMonster = 5;
constexpr size_t g_fromGuardian = 10;
constexpr size_t g_fromTreasure = 20;

constexpr size_t g_forDamage = 10;
constexpr size_t g_forDamangeGuardian = 20;

[[nodiscard]] constexpr auto get() -> uint8_t {
    return ( g_amount );
}

NOINLINE constexpr void add( size_t _amount ) {
    g_amount += _amount;

    if ( g_amount > g_maxExperiencePoints ) {
        g_amount = g_maxExperiencePoints;
    }
}

} // namespace experience

} // namespace stats

size_t g_position = ( std::string_view{ MAP }.find(
    static_cast< char >( map::actor_t::player ) ) );

[[nodiscard]] constexpr auto fight( map::actor_t _who ) -> bool {
    bool l_returnValue = false;

    stats::health::g_amount -= 1;

    if ( _who == map::actor_t::guardian ) [[unlikely]] {
        l_returnValue = guardian::fight();

    } else {
        l_returnValue = monster::fight();

        if ( ( l_returnValue ) && ( _who == map::actor_t::keyMonster ) )
            [[unlikely]] {
            inventory::add( inventory::item_t::key );
        }
    }

    // Has player died
    if ( ( !stats::health::g_amount ) ||
         ( stats::health::g_amount > stats::health::g_maxHealthPoints ) )
        [[unlikely]] {
        if ( inventory::use( inventory::item_t::health ) ) [[unlikely]] {
            // Full health points restoration
            stats::health::g_amount = stats::health::g_maxHealthPoints;

            l_returnValue = true;

        } else {
            lose::set();

            l_returnValue = false;
        }
    }

    return ( l_returnValue );
}

[[nodiscard]] FORCE_INLINE constexpr auto tryFightTile( char _tile ) -> bool {
    // Define the list of possible opponents represented by specific actor
    // types
    static constexpr std::array l_opponents{
        map::actor_t::followMonster,
        map::actor_t::randomMonster,
        map::actor_t::keyMonster,
        map::actor_t::guardian,
    };

    for ( const auto _opponent : l_opponents ) {
        if ( static_cast< char >( _opponent ) == _tile ) {
            return ( player::fight( ( map::actor_t )_tile ) );
        }
    }

    // If no opponent is found, return false (no fight occurs)
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
FORCE_INLINE constexpr void move( map::direction_t _direction ) {
    const size_t l_oldPosition = player::g_position;
    const size_t l_newPosition =
        ( player::g_position + static_cast< size_t >( _direction ) );

    char& l_tile = map::g_current[ l_newPosition ];

    if ( map::isTileWalkable( l_tile ) || tryFightTile( l_tile ) ||
         map::tryActionTile( l_tile ) ) [[likely]] {
        map::g_current[ l_oldPosition ] = map::g_empty[ l_oldPosition ];

        l_tile = static_cast< char >( map::actor_t::player );

        player::g_position = l_newPosition;
    }
}

namespace render {

// Stats
// HP:100
// EXP:100
// ITEMS:E E E E E
FORCE_INLINE void stats() {
#define FULL_HEALTH_TEXT HEALTH_TEXT HEALTH_POINTS_TEXT_PLACEHOLDER
#define FULL_EXPERIENCE_TEXT EXPERIENCE_TEXT EXPERIENCE_POINTS_TEXT_PLACEHOLDER
#define FULL_ITEMS_TEXT ITEMS_TEXT ITEMS_TEXT_PLACEHOLDER

    // FIX: Can save 8 bytes here
    std::array l_buffer =
        std::to_array( FULL_HEALTH_TEXT FULL_EXPERIENCE_TEXT FULL_ITEMS_TEXT );

    auto l_cursorPosition = l_buffer.begin();

    // Render points into buffer
    {
        // Health
        ::render::points( l_cursorPosition, stats::health::g_amount,
                          sizeof( FULL_HEALTH_TEXT ) );

        // FIX: Convertion to int
        std::advance( l_cursorPosition, 1 );

        // Experience
        ::render::points( l_cursorPosition, stats::experience::get(),
                          sizeof( FULL_EXPERIENCE_TEXT ) );

        std::advance( l_cursorPosition, -1 );
    }

    // Items
    {
        std::advance( l_cursorPosition, ( sizeof( FULL_ITEMS_TEXT ) -
                                          sizeof( ITEMS_TEXT_PLACEHOLDER ) ) );

        for ( auto& _item : inventory::g_items ) {
            // Move to the next character
            // Skip delimiter
            std::advance( l_cursorPosition, 2 );

            *l_cursorPosition = static_cast< char >( _item );
        }
    }

    io::print( l_buffer );

#undef FULL_ITEMS_TEXT
#undef FULL_EXPERIENCE_TEXT
#undef FULL_HEALTH_TEXT
}

} // namespace render

} // namespace player

[[nodiscard]] constexpr auto monster::fight() -> bool {
    const size_t l_damageAmount =
        ( 1 + ( player::stats::experience::get() /
                player::stats::experience::g_forDamage ) );
    stats::health::g_amount -= l_damageAmount;

    if ( ( !stats::health::g_amount ) ||
         ( stats::health::g_amount > g_maxHealth ) ) [[unlikely]] {
        stats::health::g_amount = g_maxHealth;

        player::stats::experience::add(
            player::stats::experience::g_fromMonster );

        return ( true );
    }

    return ( false );
}

[[nodiscard]] constexpr auto guardian::fight() -> bool {
    size_t l_damageAmount =
        ( 1 + ( player::stats::experience::get() /
                player::stats::experience::g_forDamangeGuardian ) );

    if ( player::inventory::use( player::inventory::item_t::attack ) ) {
        l_damageAmount += 5;
    }

    if ( !player::inventory::use( player::inventory::item_t::defense ) ) {
        player::stats::health::g_amount -= 4;
    }

    stats::health::g_amount -= l_damageAmount;

    if ( ( !stats::health::g_amount ) ||
         ( stats::health::g_amount > g_maxHealth ) ) [[unlikely]] {
        stats::health::g_amount = g_maxHealth;

        player::stats::experience::add(
            player::stats::experience::g_fromGuardian );

        return ( true );
    }

    return ( false );
}

namespace map {

[[nodiscard]] FORCE_INLINE constexpr auto isTileNotDecoration( char _tile )
    -> bool {
    return ( ( _tile != static_cast< char >( tile_t::wallHorizontal ) ) &&
             ( _tile != static_cast< char >( tile_t::wallVertical ) ) &&
             ( _tile != static_cast< char >( tile_t::wallCross ) ) &&
             ( _tile != static_cast< char >( actionable_t::ladderLeft ) ) &&
             ( _tile != static_cast< char >( actionable_t::ladderRight ) ) &&
             ( _tile != ' ' ) && ( _tile != '\n' ) && ( _tile != '0' ) );
}

[[nodiscard]] constexpr auto isTileWalkable( char _tile ) -> bool {
    const auto l_tile = static_cast< tile_t >( _tile );

    return ( ( l_tile == tile_t::floor ) || ( l_tile == tile_t::bridge ) );
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
 * 1. Copies the current map (`g_current`) to a new empty map (`g_empty`).
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
 *       - `g_current`: The original map layout.
 *       - `g_empty`: The map that has been processed for walkability and
 * content.
 *       - `player::osition`: The player's starting position on the map.
 */
FORCE_INLINE void init() {
    g_empty = g_current;

    // Define directions for potential tile replacements
    // First 4 will empty the whole map
    constexpr std::array l_allDirections{
        direction_t::down,
        direction_t::left,
        direction_t::right,
        direction_t::upRight,

        // Extra, clockwise
        direction_t::up,
        direction_t::downRight,
        direction_t::downLeft,
        direction_t::upLeft,
    };

    // Empty map is a copy of map
    for ( char* _element = g_empty.data();
          _element < ( g_empty.data() + g_empty.size() );
          std::advance( _element, 1 ) ) {
        char* l_tile = _element;

        // Generate empty map
        // Replace non-walkable, non-decorative tiles with walkable ones
        if ( isTileNotDecoration( *l_tile ) && !isTileWalkable( *l_tile ) )
            [[unlikely]] {
            // Try to find a walkable replacement tile by checking the
            // adjacent tiles
            for ( const direction_t _direction : l_allDirections ) {
                const auto l_replacement =
                    ( *( l_tile + static_cast< char >( _direction ) ) );

                if ( isTileWalkable( l_replacement ) ) [[likely]] {
                    *l_tile = static_cast< char >( l_replacement );

                    break;
                }
            }
        }
    }

    // Generate level
    for ( char& _tile : g_current ) {
        auto& l_tile = reinterpret_cast< actor_t& >( _tile );

        // If the tile represents a monster with a key, replace it with
        // random key monster
        if ( l_tile == actor_t::monsterWithKey ) [[unlikely]] {
            constexpr std::array l_keyMonsters{
                actor_t::keyMonster,
            };

            l_tile = random::value( l_keyMonsters );
        }

        // If the tile represents a monster, replace it with random monster
        if ( l_tile == actor_t::monster ) [[unlikely]] {
            static constexpr std::array l_monsters{
                actor_t::followMonster,
                actor_t::randomMonster,
            };

            l_tile = random::value( l_monsters );
        }
    }
}

namespace action {

FORCE_INLINE constexpr void chest() {
    // Define a list of possible items that can be found in the chest
    constexpr std::array l_items{
        player::inventory::item_t::health,
        player::inventory::item_t::attack,
        player::inventory::item_t::defense,
    };

    // Add a randomly selected item from the list to the player's inventory
    player::inventory::add( random::value( l_items ) );
}

FORCE_INLINE constexpr void treasure() {
    player::stats::experience::add( player::stats::experience::g_fromTreasure );
}

[[nodiscard]] FORCE_INLINE constexpr auto door() -> bool {
    // Attempt to use a key from the player's inventory to open the door
    return ( player::inventory::use( player::inventory::item_t::key ) );
}

} // namespace action

[[nodiscard]] FORCE_INLINE constexpr auto tryActionTile( char _tile ) -> bool {
    // Use tile
    switch ( static_cast< actionable_t >( _tile ) ) {
        case ( actionable_t::chest ): {
            action::chest();

            return ( true );
        }

        case ( actionable_t::treasure ): {
            action::treasure();

            return ( true );
        }

        case ( actionable_t::doorLeft ):
        case ( actionable_t::doorRight ):
        case ( actionable_t::doorMiddle ): {
            return ( action::door() );
        }

        case ( actionable_t::ladderLeft ):
        case ( actionable_t::ladderRight ): {
            player::lose::set();

            return ( false );
        }
    }

    // Pick-up item
    {
        static constexpr std::array l_items{
            player::inventory::item_t::key,
            player::inventory::item_t::health,
            player::inventory::item_t::attack,
            player::inventory::item_t::defense,
        };

        for ( const auto _item : l_items ) {
            if ( static_cast< char >( _item ) == _tile ) {
                player::inventory::add( ( player::inventory::item_t )_tile );

                return ( true );
            }
        }
    }

    // Return false if no action occurred
    return ( false );
}

constexpr void move( actor_t _who,
                     size_t _currentPosition,
                     direction_t _direction ) {
    const size_t l_oldPosition = _currentPosition;
    const size_t l_newPosition =
        ( _currentPosition + static_cast< size_t >( _direction ) );

    char& l_tile = g_current[ l_newPosition ];

    if ( ( map::isTileWalkable( l_tile ) ) ||
         ( ( l_tile == static_cast< char >( actor_t::player ) ) &&
           ( player::tryFightTile( static_cast< char >( _who ) ) ) ) )
        [[likely]] {
        map::g_current[ l_oldPosition ] = map::g_empty[ l_oldPosition ];

        l_tile = static_cast< char >( _who );
    }
}

FORCE_INLINE constexpr void move$random( actor_t _who,
                                         size_t _currentPosition ) {
    // Define all possible movement directions, including staying in place
    static std::array l_allDirections{
        direction_t::down,     direction_t::left,   direction_t::right,
        direction_t::upRight,  direction_t::up,     direction_t::downRight,
        direction_t::downLeft, direction_t::upLeft, direction_t::stay,
    };

    move( _who, _currentPosition, random::value( l_allDirections ) );
}

// TODO: Implement movement to follow the player if close
FORCE_INLINE constexpr void move$follow( actor_t _who,
                                         size_t _currentPosition ) {
    move$random( _who, _currentPosition );

#if 0
    const size_t l_playerPositionX = ( player::g_position % g_width );
    const size_t l_playerPositionY = ( player::g_position / g_width );
    const size_t l_whoPositionX = ( _currentPosition % g_width );
    const size_t l_whoPositionY = ( _currentPosition / g_width );

    if ( l_playerPositionX > l_whoPositionX ) {
        move( _who, _currentPosition, direction_t::right );

    } else if ( l_playerPositionX < l_whoPositionX ) {
        move( _who, _currentPosition, direction_t::left );
    }

    if ( l_playerPositionY > l_whoPositionY ) {
        move( _who, _currentPosition, direction_t::down );

    } else if ( l_playerPositionY < l_whoPositionY ) {
        move( _who, _currentPosition, direction_t::up );
    }
#endif
}

/**
 * @brief Moves all AI-controlled monsters on the map.
 *
 * This function iterates through each tile on the map, and for each tile
 * that contains a monster (RANDOM_MONSTER, FOLLOW_MONSTER, KEY_MONSTER, or
 * GUARDIAN), it moves the corresponding monster to a corresponding adjacent
 * position by calling the `move$...` function. The movement is based on the
 * monster's type, ensuring that each specific type of monster is moved
 * according to predefined behavior.
 */
FORCE_INLINE constexpr void ai() {
    const auto l_range = std::views::iota( size_t{}, g_current.size() / 2 );

    for ( const auto [ _index, _indexReverse ] :
          std::views::zip( l_range, l_range | std::views::reverse ) ) {
        auto l_tile = static_cast< actor_t >( g_current[ _index ] );

        switch ( l_tile ) {
            case ( actor_t::randomMonster ): {
                move$random( actor_t::randomMonster, _index );

                break;
            }

            case ( actor_t::followMonster ): {
                move$follow( actor_t::followMonster, _index );

                break;
            }

            case ( actor_t::keyMonster ): {
                move$random( actor_t::keyMonster, _index );

                break;
            }

            case ( actor_t::guardian ): {
                move$follow( actor_t::guardian, _index );

                break;
            }

            default: {
            }
        }
    }
}

namespace render {

FORCE_INLINE void debug() {
#define MONSTER_TEXT "\nM:"
#define GUARDIAN_TEXT "\nB:"

#define FULL_MONSTER_TEXT MONSTER_TEXT HEALTH_POINTS_TEXT_PLACEHOLDER
#define FULL_GUARDIAN_TEXT GUARDIAN_TEXT HEALTH_POINTS_TEXT_PLACEHOLDER

    // FIX: Can save 8 bytes here
    std::array l_buffer = std::to_array( FULL_MONSTER_TEXT FULL_GUARDIAN_TEXT );

    // Render points into buffer
    {
        auto l_cursorPosition = l_buffer.begin();

        // Monster HP
        ::render::points( l_cursorPosition, monster::stats::health::g_amount,
                          sizeof( FULL_MONSTER_TEXT ) );

        // FIX: Convertion to int
        std::advance( l_cursorPosition, 1 );

        // Guardian HP
        ::render::points( l_cursorPosition, guardian::stats::health::g_amount,
                          sizeof( FULL_GUARDIAN_TEXT ) );

        std::advance( l_cursorPosition, -1 );
    }

    io::print( l_buffer );

#undef GUARDIAN_TEXT
#undef MONSTER_TEXT
}

/**
 * @brief Renders the current state of the map to the output.
 *
 * This function iterates through each tile of the global `g_current` array
 * and prints the character representing the tile. It essentially outputs
 * the map as it is in the game's internal state, tile by tile.
 *
 * - Each tile in `g_current` is represented by a single character that
 * could be a floor, wall, player, monster, chest, treasure, etc.
 *
 * This function assumes that the map data (`g_current`) is structured in a
 * way where each element corresponds to a character that needs to be
 * printed to the screen.
 *
 * @note The rendering is done one character at a time. Depending on how the
 * `print` function works, it could render a single line or the entire map
 * one by one. Make sure the print function handles newlines and coordinates
 * correctly.
 *
 * @see `print()` for more information on how tiles are printed to the
 * screen.
 */
FORCE_INLINE void current() {
    // Iterate through each tile on map
    for ( const char _tile : g_current ) {
        io::print( { _tile } );
    }

#if defined( RENDER_DEBUG_INFORMATION )

    // Debug information
    {
        debug();
    }

#endif
}

} // namespace render

} // namespace map

} // namespace logic
