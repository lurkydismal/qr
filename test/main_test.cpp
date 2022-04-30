#include <gtest/gtest.h>

#define MAP_SIZE        80 * 10
#define OVERLOOK_RADIUS 29 + 2

#include "../src/main.h"
#include "../src/stdfunc.h"

TEST( logic, initMap ) {
    char     temp_emptyMap[ MAP_SIZE ];
    char     temp_map[ MAP_SIZE ];
    uint32_t temp_guardiansPositions[ MAX_GUARDIANS_ON_MAP ];
    uint32_t temp_randomMonstersPositions[ MAX_MONSTERS_ON_MAP ];
    uint32_t temp_followMonstersPositions[ MAX_MONSTERS_ON_MAP ];
    uint32_t temp_playerPosition     = g_playerPosition;
    uint32_t temp_guardiansLeft      = g_guardiansLeft;
    uint32_t temp_randomMonstersLeft = g_randomMonstersLeft;
    uint32_t temp_followMonstersLeft = g_followMonstersLeft;

    memset( &temp_emptyMap, 0,          sizeof( temp_emptyMap ) );
    memcpy( temp_emptyMap,  g_emptyMap, sizeof( temp_emptyMap ) );

    memset( &temp_map, 0,     sizeof( temp_map ) );
    memcpy( temp_map,  g_map, sizeof( temp_map ) );

    memset( &temp_guardiansPositions, 0,                    sizeof( g_guardiansPositions ) );
    memcpy( temp_guardiansPositions,  g_guardiansPositions, sizeof( g_guardiansPositions ) );

    memset( &temp_randomMonstersPositions, 0,                         sizeof( g_randomMonstersPositions ) );
    memcpy( temp_randomMonstersPositions,  g_randomMonstersPositions, sizeof( g_randomMonstersPositions ) );

    memset( &temp_followMonstersPositions, 0,                         sizeof( g_followMonstersPositions ) );
    memcpy( temp_followMonstersPositions,  g_followMonstersPositions, sizeof( g_followMonstersPositions ) );

    initMap();

    ASSERT_STREQ( temp_emptyMap, g_emptyMap );
    ASSERT_STRNE( temp_map,      g_map );

    for ( uint32_t _guardianPositionIndex : temp_guardiansPositions ) {
        ASSERT_NE(
            temp_guardiansPositions[ _guardianPositionIndex ],
            g_guardiansPositions[ _guardianPositionIndex ]
        );
    }

    for ( uint32_t _randomMonsterPositionIndex : temp_randomMonstersPositions ) {
        ASSERT_NE(
            temp_randomMonstersPositions[ _randomMonsterPositionIndex ],
            g_randomMonstersPositions[ _randomMonsterPositionIndex ]
        );
    }

    for ( uint32_t _followMonsterPositionIndex : temp_followMonstersPositions ) {
        ASSERT_NE(
            temp_followMonstersPositions[ _followMonsterPositionIndex ],
            g_followMonstersPositions[ _followMonsterPositionIndex ]
        );
    }

    ASSERT_NE( temp_playerPosition,     g_playerPosition );
    ASSERT_NE( temp_guardiansLeft,      g_guardiansLeft );
    ASSERT_NE( temp_randomMonstersLeft, g_randomMonstersLeft );
    ASSERT_NE( temp_followMonstersLeft, g_followMonstersLeft );
}

TEST( logic, initInventory ) {
    initInventory( (enum item_t)NULL );

    g_playerInventory[ 2 ] = HEALTH;
    g_playerInventoryItemCount++;

    ASSERT_EQ( g_playerInventory[ 2 ], HEALTH );
    ASSERT_EQ( (int32_t)g_playerInventoryItemCount, PLAYER_MAX_ITEM_COUNT + 1 );

    initInventory( EMPTY );

    inventoryAdd( HEALTH, 2 );

    ASSERT_EQ( g_playerInventory[ 0 ], EMPTY );
    ASSERT_EQ( g_playerInventory[ 1 ], EMPTY );
    ASSERT_EQ( g_playerInventory[ 2 ], HEALTH );
    ASSERT_EQ( g_playerInventory[ 3 ], EMPTY );
    ASSERT_EQ( g_playerInventory[ 4 ], EMPTY );
    ASSERT_EQ( g_playerInventoryItemCount, (uint32_t)1 );
}

TEST( logic, getOverview ) {
    char temp_vision[ OVERLOOK_RADIUS ];

    initMap();

    getOverview( g_playerPosition );

    memset( &temp_vision, 0,        stringLength( g_vision ) );
    memcpy( temp_vision,  g_vision, stringLength( g_vision ) );

    ASSERT_STREQ( temp_vision, g_vision );

    doPlayerMove( RIGHT );

    getOverview( g_playerPosition );

    ASSERT_STRNE( temp_vision, g_vision );

    memset( &temp_vision, 0,        stringLength( g_vision ) );
    memcpy( temp_vision,  g_vision, stringLength( g_vision ) );

    ASSERT_STREQ( temp_vision, g_vision );
}

TEST( logic, getPlayerInventoryPlaceOf ) {
    initInventory( EMPTY );

    inventoryAdd( DEFENCE, INT8_MIN );
    inventoryAdd( ATTACK, INT8_MIN );
    inventoryAdd( HEALTH, INT8_MIN );
    inventoryAdd( KEY, INT8_MIN );

    ASSERT_EQ( getPlayerInventoryPlaceOf( EMPTY ),   4 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( KEY ),     3 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( HEALTH ),  2 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( ATTACK ),  1 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( DEFENCE ), 0 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( (enum item_t)NULL ), INT8_MIN );
}

TEST( logic, inventoryAdd ) {
    initInventory( EMPTY );

    ASSERT_TRUE( inventoryAdd( ATTACK, INT8_MIN ) );
    ASSERT_EQ( g_playerInventory[ 0 ], ATTACK );
    ASSERT_EQ( getPlayerInventoryPlaceOf( ATTACK ), 0 );

    ASSERT_TRUE( inventoryAdd( (enum item_t)NULL, INT8_MIN ) );
    ASSERT_EQ( g_playerInventory[ 1 ], NULL );
    ASSERT_EQ( getPlayerInventoryPlaceOf( (enum item_t)NULL ), 1 );

    ASSERT_FALSE( inventoryAdd( ATTACK, 0 ) );

    initInventory( (enum item_t)NULL );

    ASSERT_FALSE( inventoryAdd( ATTACK, INT8_MIN ) );
    ASSERT_FALSE( inventoryAdd( (enum item_t)NULL, INT8_MIN ) );
}

TEST( logic, usePlayerItem ) {
    initInventory( DEFENCE );

    ASSERT_EQ( getPlayerInventoryPlaceOf( DEFENCE ), 0 );
    ASSERT_EQ( g_playerInventory[ 0 ], DEFENCE );
    ASSERT_EQ( g_playerInventory[ 1 ], DEFENCE );
    ASSERT_EQ( g_playerInventory[ 2 ], DEFENCE );
    ASSERT_EQ( g_playerInventory[ 3 ], DEFENCE );
    ASSERT_EQ( g_playerInventory[ 4 ], DEFENCE );
    ASSERT_EQ( g_playerInventoryItemCount, (uint32_t)PLAYER_MAX_ITEM_COUNT );

    ASSERT_TRUE( usePlayerItem( DEFENCE ) );

    ASSERT_EQ( g_playerInventory[ 0 ], EMPTY );
    ASSERT_EQ( getPlayerInventoryPlaceOf( EMPTY ), 0 );

    ASSERT_FALSE( usePlayerItem( ATTACK ) );
    ASSERT_FALSE( usePlayerItem( (enum item_t)NULL ) );
}

TEST( logic, move ) {
    initMap();

    EXPECT_EQ( g_map[ g_playerPosition + 1 ], '.' );
    ASSERT_EQ( move( PLAYER, g_playerPosition, 1 ), g_playerPosition + 1 );
}

TEST( logic, DoPlayerMove ) {
    initMap();

    uint32_t temp_playerPosition = g_playerPosition;

    ASSERT_TRUE( doPlayerMove( DOWN ) );
    ASSERT_NE( temp_playerPosition, g_playerPosition );

    temp_playerPosition = g_playerPosition;

    ASSERT_TRUE( doPlayerMove( 5 ) ); // + 5 from current position, '.' or '#' expected
    ASSERT_EQ( temp_playerPosition, g_playerPosition );

    initMap(); // Reset player position

    g_guardiansLeft = 0; // We can pass through door if all guardians annihilated

    ASSERT_FALSE( doPlayerMove( 396 ) ); // + 396 from current position, door expected ( '>' or '<' )
    ASSERT_EQ( temp_playerPosition, g_playerPosition );
}