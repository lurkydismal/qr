#include <gtest/gtest.h>
// #include <iostream>

#include "../src/main.hpp"

TEST( logic, initMap ) {
    char     temp_emptyMap[ MAP_SIZE ];
    char     temp_map[ MAP_SIZE ];
    uint32_t temp_guardiansPositions[ MAX_GUARDIANS_ON_MAP ];
    uint32_t temp_randomMonstersPositions[ MAX_MONSTERS_ON_MAP ];
    uint32_t temp_followMonstersPositions[ MAX_MONSTERS_ON_MAP ];
    uint32_t temp_playerPosition     = playerPosition;
    uint32_t temp_guardiansLeft      = guardiansLeft;
    uint32_t temp_randomMonstersLeft = randomMonstersLeft;
    uint32_t temp_followMonstersLeft = followMonstersLeft;

    memset( &temp_emptyMap, 0,        sizeof( temp_emptyMap ) );
    memcpy( temp_emptyMap,  emptyMap, sizeof( temp_emptyMap ) );

    memset( &temp_map, 0,     sizeof( temp_map ) );
    memcpy( temp_map,  g_map, sizeof( temp_map ) );

    memset( &temp_guardiansPositions, 0,                    sizeof( g_guardiansPositions ) );
    memcpy( temp_guardiansPositions,  g_guardiansPositions, sizeof( g_guardiansPositions ) );

    memset( &temp_randomMonstersPositions, 0,                         sizeof( g_randomMonstersPositions ) );
    memcpy( temp_randomMonstersPositions,  g_randomMonstersPositions, sizeof( g_randomMonstersPositions ) );

    memset( &temp_followMonstersPositions, 0,                         sizeof( g_followMonstersPositions ) );
    memcpy( temp_followMonstersPositions,  g_followMonstersPositions, sizeof( g_followMonstersPositions ) );

    initMap();

    ASSERT_STREQ( temp_emptyMap, emptyMap );
    ASSERT_STRNE( temp_map, g_map );

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

    ASSERT_NE( temp_playerPosition, playerPosition );
    ASSERT_NE( temp_guardiansLeft,  guardiansLeft );
    ASSERT_NE( temp_randomMonstersLeft, randomMonstersLeft );
    ASSERT_NE( temp_followMonstersLeft, followMonstersLeft );
}

TEST( logic, initInventory ) {
    initInventory( (ITEMS)NULL );

    playerInventory[ 2 ] = HEALTH;
    playerInventoryItemCount++;

    ASSERT_EQ( playerInventory[ 2 ], HEALTH );
    ASSERT_EQ( (int)playerInventoryItemCount, MAX_PLAYER_ITEM_COUNT + 1 );

    initInventory( EMPTY );

    inventoryAdd( HEALTH, 2 );

    ASSERT_EQ( playerInventory[ 0 ], EMPTY );
    ASSERT_EQ( playerInventory[ 1 ], EMPTY );
    ASSERT_EQ( playerInventory[ 2 ], HEALTH );
    ASSERT_EQ( playerInventory[ 3 ], EMPTY );
    ASSERT_EQ( playerInventory[ 4 ], EMPTY );
    ASSERT_EQ( playerInventoryItemCount, (uint32_t)1 );
}

TEST( logic, checkLose ) {
    playerHealth = 1;
    
    ASSERT_TRUE( checkLose() );

    playerHealth = 0;

    ASSERT_FALSE( checkLose() );
}

TEST( logic, getOverview ) {
    char temp_vision[ OVERLOOK_RADIUS ];
    // temp_vision[ 29 ] = '\n';
    // temp_vision[ 30 ] = '\0';

    initMap();

    getOverview( playerPosition );

    memset( &temp_vision, 0,      sizeof( temp_vision ) );
    memcpy( temp_vision,  vision, sizeof( temp_vision ) );

    ASSERT_STREQ( temp_vision, vision );

    DoPlayerMove( RIGHT );

    getOverview( playerPosition );

    ASSERT_STRNE( temp_vision, vision );

    memset( &temp_vision, 0,      sizeof( temp_vision ) );
    memcpy( temp_vision,  vision, sizeof( temp_vision ) );

    ASSERT_STREQ( temp_vision, vision );
}

TEST( logic, getPlayerInventoryPlaceOf ) {
    initInventory( EMPTY );

    inventoryAdd( DEFENCE );
    inventoryAdd( ATTACK );
    inventoryAdd( HEALTH );
    inventoryAdd( KEY );

    ASSERT_EQ( getPlayerInventoryPlaceOf( EMPTY ),   4 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( KEY ),     3 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( HEALTH ),  2 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( ATTACK ),  1 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( DEFENCE ), 0 );
    ASSERT_EQ( getPlayerInventoryPlaceOf( (ITEMS)NULL ), INT8_MIN );
}

TEST( logic, inventoryAdd ) {
    initInventory( EMPTY );

    ASSERT_TRUE( inventoryAdd( ATTACK ) );
    ASSERT_EQ( playerInventory[ 0 ], ATTACK );
    ASSERT_EQ( getPlayerInventoryPlaceOf( ATTACK ), 0 );

    ASSERT_TRUE( inventoryAdd( (ITEMS)NULL ) );
    ASSERT_EQ( playerInventory[ 1 ], NULL );
    ASSERT_EQ( getPlayerInventoryPlaceOf( (ITEMS)NULL ), 1 );

    ASSERT_FALSE( inventoryAdd( ATTACK, 0 ) );

    initInventory( (ITEMS)NULL );

    ASSERT_FALSE( inventoryAdd( ATTACK ) );
    ASSERT_FALSE( inventoryAdd( (ITEMS)NULL ) );
}

TEST( logic, usePlayerItem ) {
    initInventory( DEFENCE );

    ASSERT_EQ( getPlayerInventoryPlaceOf( DEFENCE ), 0 );
    ASSERT_EQ( playerInventory[ 0 ], DEFENCE );
    ASSERT_EQ( playerInventory[ 1 ], DEFENCE );
    ASSERT_EQ( playerInventory[ 2 ], DEFENCE );
    ASSERT_EQ( playerInventory[ 3 ], DEFENCE );
    ASSERT_EQ( playerInventory[ 4 ], DEFENCE );
    ASSERT_EQ( playerInventoryItemCount, (uint32_t)MAX_PLAYER_ITEM_COUNT );

    ASSERT_TRUE( usePlayerItem( DEFENCE ) );

    ASSERT_EQ( playerInventory[ 0 ], EMPTY );
    ASSERT_EQ( getPlayerInventoryPlaceOf( EMPTY ), 0 );

    ASSERT_FALSE( usePlayerItem( ATTACK ) );
    ASSERT_FALSE( usePlayerItem( (ITEMS)NULL ) );
}

TEST( logic, move ) {
    initMap();

    EXPECT_EQ( g_map[ playerPosition + 1 ], '.' );
    ASSERT_EQ( move( '@', playerPosition, 1 ), playerPosition + 1 );
}

TEST( logic, DoPlayerMove ) {
    initMap();

    uint32_t temp_playerPosition = playerPosition;

    ASSERT_TRUE( DoPlayerMove( 80 ) ); // DOWN
    ASSERT_NE( temp_playerPosition, playerPosition );

    temp_playerPosition = playerPosition;

    ASSERT_TRUE( DoPlayerMove( 5 ) ); // + 5 from current position, '.' or '#' expected
    ASSERT_EQ( temp_playerPosition, playerPosition );

    initMap();  // reset player position

    guardiansLeft = 0; // we can pass through door if all guardians annihilated

    ASSERT_FALSE( DoPlayerMove( 396 ) ); // + 396 from current position, door expected ( '>' or '<' )
    ASSERT_EQ( temp_playerPosition, playerPosition );
}