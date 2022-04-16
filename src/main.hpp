#pragma once

#include <stdint.h>

#include "stdfunc.hpp"

#define MAP_SIZE         ( 80 * 10 )
#define OVERLOOK_RADIUS  ( 29 + 2 )
#define CHEST_ITEM_COUNT 3
#define MAX_EXPERIENCE_FROM_TREASURE 20
#define EXPERIENCE_FOR_DMG           10
#define EXPERIENCE_FOR_SUPER_DMG     20
#define MAX_PLAYER_HEALTH      20
#define MAX_PLAYER_ITEM_COUNT  5
#define MAX_MONSTERS_ON_MAP    5
#define MONSTER_TYPES          2
#define MONSTER_MAX_HEALTH     20
#define KEY_MONSTER_TYPES      1
#define KEY_MONSTER_MAX_HEALTH 20
#define MAX_GUARDIANS_ON_MAP   1
#define GUARDIAN_MAX_HEALTH    8

#ifndef _WIN32
enum VK_NUMPAD {
    VK_NUMPAD1 = 1,
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

enum item_t {
    EMPTY   = 'E',
    KEY     = 'K',
    HEALTH  = 'H',
    ATTACK  = 'A',
    DEFENCE = 'D',
};

enum direction_t {
    UP    = -80,
    RIGHT = 1,
    LEFT  = -1,
    DOWN  = 80,
};

extern uint32_t playerHealth;
extern item_t   playerInventory[ MAX_PLAYER_ITEM_COUNT ];
extern uint32_t playerInventoryItemCount;
extern uint32_t playerExperience;
extern uint32_t playerPosition;

extern const char* monsters;
extern uint32_t    monsterHealth;
extern uint32_t    g_randomMonstersPositions[ MAX_MONSTERS_ON_MAP ];
extern uint32_t    randomMonstersLeft;
extern uint32_t    g_followMonstersPositions[ MAX_MONSTERS_ON_MAP ];
extern uint32_t    followMonstersLeft;

extern const char* key_monsters;
extern uint32_t    keyMonsterHealth;

extern uint32_t guardianHealth;
extern uint32_t g_guardiansPositions[ MAX_GUARDIANS_ON_MAP ];
extern uint32_t guardiansLeft;

extern const char* emptyMap;
extern char        g_map[ MAP_SIZE ];
extern char        vision[ OVERLOOK_RADIUS ];

void initMap( void );
void initInventory( item_t item );
bool checkLose( void );
void getOverview( const uint32_t position );
int32_t getPlayerInventoryPlaceOf( const item_t item );
bool inventoryAdd( const item_t item, const int32_t itemIndex = INT8_MIN );
bool usePlayerItem( const item_t item );
uint32_t move( const char who, unsigned int currentPosition, const int offset );
bool DoPlayerMove( int32_t offset );
bool DoOpponentMove( void );
uint32_t fightMonster( const char who, uint32_t currentPosition, const int32_t offset );
uint32_t fightKeyMonster( const char who, uint32_t currentPosition, const int32_t offset );
uint32_t fightGuardian( const char who, uint32_t currentPosition, const int32_t offset );
void UpdateScreen( void );

#ifndef _WIN32
bool GetAsyncKeyState( uint32_t key );
#endif