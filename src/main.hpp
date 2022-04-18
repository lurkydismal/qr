#pragma once

#include <stdint.h>

#include "stdfunc.hpp"

// #define CHEST_ITEM_COUNT 3
#define MAX_EXPERIENCE_FROM_TREASURE 20
#define EXPERIENCE_FOR_DMG           10
#define EXPERIENCE_FOR_SUPER_DMG     20
#define PLAYER_MAX_HEALTH      20
#define PLAYER_MAX_ITEM_COUNT  5
#define MAX_GUARDIANS_ON_MAP   1
#define MAX_MONSTERS_ON_MAP    5
#define GUARDIAN_MAX_HEALTH    8
#define MONSTER_MAX_HEALTH     20
#define KEY_MONSTER_MAX_HEALTH 20

#ifndef _WINUSER_
#define VK_NUMPAD0 0x60
#define VK_NUMPAD1 0x61
#define VK_NUMPAD2 0x62
#define VK_NUMPAD3 0x63
#define VK_NUMPAD4 0x64
#define VK_NUMPAD5 0x65
#define VK_NUMPAD6 0x66
#define VK_NUMPAD7 0x67
#define VK_NUMPAD8 0x68
#define VK_NUMPAD9 0x69
#endif

enum item_t {
    EMPTY   = 'E',
    KEY     = 'K',
    HEALTH  = 'H',
    ATTACK  = 'A',
    DEFENCE = 'D',
};

enum cell_t {
    FLOOR        = '.',
    BRIDGE       = '#',
    DOOR_LEFT    = '}',
    DOOR_RIGHT   = '{',
    DOOR_MIDDLE  = '/',
    LADDER_LEFT  = '>',
    LADDER_RIGHT = '<',
};

enum usable_t {
    CHEST    = 'C',
    TREASURE = 'T',
};

enum actor_t {
    PLAYER             = '@',
    GUARDIAN           = 'G',
    FOLLOW_MONSTER     = 'F',
    RANDOM_MONSTER     = 'R',
    KEY_MONSTER        = 'A',
    MONSTER            = 'M',
    MONSTER_WITH_A_KEY = 'W',
};

enum direction_t {
    STAY       = 0,
    UP         = -80,
    UP_RIGHT   = -79,
    RIGHT      = 1,
    DOWN_RIGHT = 81,
    DOWN       = 80,
    DOWN_LEFT  = 79,
    LEFT       = -1,
    UP_LEFT    = -81,
};

extern item_t      g_playerInventory[ PLAYER_MAX_ITEM_COUNT ];
extern uint32_t    g_playerInventoryItemCount;
extern int32_t     g_playerHealth;
extern uint32_t    g_playerExperience;
extern uint32_t    g_playerPosition;

extern uint32_t    g_guardiansPositions[ MAX_GUARDIANS_ON_MAP ];
extern uint32_t    g_followMonstersPositions[ MAX_MONSTERS_ON_MAP ];
extern uint32_t    g_randomMonstersPositions[ MAX_MONSTERS_ON_MAP ];
extern uint32_t    g_guardianHealth;
extern uint32_t    g_monsterHealth;
extern uint32_t    g_keyMonsterHealth;
extern const char  g_monsters[];
extern const char  g_keyMonsters[];
extern uint32_t    g_guardiansLeft;
extern uint32_t    g_followMonstersLeft;
extern uint32_t    g_randomMonstersLeft;

extern const char* g_emptyMap;
extern char        g_map[];
extern char        g_vision[];

void     initMap( void );
void     initInventory( item_t item );
void     getOverview( const uint32_t position );
int32_t  getPlayerInventoryPlaceOf( const item_t item );
bool     inventoryAdd( const item_t item, const int32_t itemIndex = INT8_MIN );
bool     usePlayerItem( const item_t item );
uint32_t move( const char who, unsigned int currentPosition, const int offset );
bool     doPlayerMove( const uint32_t offset );
bool     doOpponentMove( void );
uint32_t fight( const char who, uint32_t currentPosition, const int32_t offset );
void     updateScreen( void );

#ifndef _WIN32
bool GetAsyncKeyState( uint32_t key );
#endif