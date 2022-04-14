#pragma once

#include <stdint.h>

#define MAP_SIZE 80 * 10

// view
#define OVERLOOK_RADIUS 29 + 2

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
    DOWN = 80,
    UP = -80,
    LEFT = -1,
    RIGHT = 1,
};

// extern ITEMS items;
// extern MOVEMENT_DIRECTIONS movement;

extern uint32_t playerHealth;
extern ITEMS playerInventory[ MAX_PLAYER_ITEM_COUNT ];
extern uint32_t playerInventoryItemCount;
extern uint32_t playerExperience;
extern uint32_t playerPosition;

extern const char* monsters;
extern uint32_t monsterHealth;
extern uint32_t g_randomMonstersPositions[ MAX_MONSTERS_ON_MAP ];
extern uint32_t randomMonstersLeft;
extern uint32_t g_followMonstersPositions[ MAX_MONSTERS_ON_MAP ];
extern uint32_t followMonstersLeft;

extern const char* key_monsters;
extern uint32_t keyMonsterHealth;

extern uint32_t guardianHealth;
extern uint32_t g_guardiansPositions[ MAX_GUARDIANS_ON_MAP ];
extern uint32_t guardiansLeft;

extern const char* emptyMap;

extern char g_map[ MAP_SIZE ];

extern char vision[ OVERLOOK_RADIUS ];

void initMap( void );
void initInventory( ITEMS item );
bool checkLose( void );
void getOverview( const unsigned int );
int getPlayerInventoryPlaceOf( const ITEMS item );
bool inventoryAdd( const ITEMS item, const int itemIndex = INT8_MIN );
bool usePlayerItem( const ITEMS item );
uint32_t move( const char who, unsigned int currentPosition, const int offset );
bool DoPlayerMove( int offset ) noexcept;
bool DoOpponentMove( void ) noexcept;
uint32_t fightMonster( const char who, unsigned int currentPosition, const int offset );
uint32_t fightKeyMonster( const char who, unsigned int currentPosition, const int offset );
uint32_t fightGuardian( const char who, unsigned int currentPosition, const int offset );
void UpdateScreen( void ) noexcept;

#ifndef _WIN32
bool GetAsyncKeyState( unsigned int );
#endif