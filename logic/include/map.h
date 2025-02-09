#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAP_WIDTH 61

typedef enum {
    FLOOR = '.',
    BRIDGE = '#',
    WALL_HORIZONTAL = '-',
    WALL_VERTICAL = '|',
    WALL_CROSS = '+'
} tile_t;

typedef enum {
    STAY = 0,
    UP = -( MAP_WIDTH ),
    UP_RIGHT = -( MAP_WIDTH - 1 ),
    RIGHT = 1,
    DOWN_RIGHT = ( MAP_WIDTH + 1 ),
    DOWN = MAP_WIDTH,
    DOWN_LEFT = ( MAP_WIDTH - 1 ),
    LEFT = -1,
    UP_LEFT = -( MAP_WIDTH + 1 )
} direction_t;

typedef enum {
    CHEST = 'C',
    TREASURE = 'T',
    DOOR_LEFT = '}',
    DOOR_RIGHT = '{',
    DOOR_MIDDLE = '/',
    LADDER_LEFT = '>',
    LADDER_RIGHT = '<'
} actionable_t;

typedef enum {
    PLAYER = '@',
    GUARDIAN = 'G',
    FOLLOW_MONSTER = 'F',
    RANDOM_MONSTER = 'R',
    KEY_MONSTER = 'O',
    MONSTER = 'M',
    MONSTER_WITH_KEY = 'W'
} actor_t;

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
void logic$map$init( void );

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
void logic$map$move$ai( void );

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
void logic$map$render( void );
