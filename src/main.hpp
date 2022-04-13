#pragma once

#define MAP_SIZE 80 * 10

// view
#define OVERLOOK_RADIUS 29

// loot
#define CHEST_ITEM_COUNT 3

#define MAX_EXPERIENCE_FROM_TREASURE 20

#define EXPERIENCE_FOR_DMG 10
#define EXPERIENCE_FOR_SUPER_DMG 20

// units
#define MAX_PLAYER_HEALTH 2
#define MAX_PLAYER_ITEM_COUNT 5

#define MAX_MONSTERS_ON_MAP 5
#define MONSTER_TYPES 2
#define MONSTER_MAX_HEALTH 20

#define KEY_MONSTER_TYPES 1
#define KEY_MONSTER_MAX_HEALTH 20

#define MAX_GUARDIANS_ON_MAP 1
#define GUARDIAN_MAX_HEALTH 8

#ifndef _WIN32
enum VK_NUMPAD {
    VK_NUMPAD1,
    VK_NUMPAD2,
    VK_NUMPAD3,
    VK_NUMPAD4,
    VK_NUMPAD5,
    VK_NUMPAD6,
    VK_NUMPAD7,
    VK_NUMPAD8,
    VK_NUMPAD9,
};
#endif

enum ITEMS { // Player inventory
    EMPTY   = 'E',
    KEY     = 'K',
    HEALTH  = 'H',
    ATTACK  = 'A',
    DEFENCE = 'D',
};

enum MOVEMENT_DIRECTIONS { // Directions 2 8 4 6
    DOWN = 0,
    UP,
    LEFT,
    RIGHT,
};

void initMap( void );
void initInventory( void );
void checkLose( void );
bool DoPlayerMove( int offset ) noexcept;
bool DoOpponentMove() noexcept;
bool usePlayerItem( const ITEMS item );
void UpdateScreen() noexcept;

#ifndef _WIN32
bool GetAsyncKeyState( unsigned int );
#endif