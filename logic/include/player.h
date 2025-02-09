#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "map.h"

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

typedef enum {
    EMPTY = 'E',
    KEY = 'K',
    HEALTH = 'H',
    ATTACK = 'A',
    DEFENSE = 'D'
} item_t;

// Inventory
void logic$player$experience$add( const size_t _amount );
void logic$player$inventory$add( const item_t _item );
bool logic$player$inventory$use( const item_t _item );

// Stats
bool logic$player$fight( const actor_t _who );
bool logic$player$lose$get( void );
void logic$player$lose$set( void );

// Render
void logic$player$render$stats( void );
void logic$player$render$debug( void );
