// Copyright 2008, Google Inc.
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:

//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <gtest/gtest.h>

#include <main.h>
#include <stdfunc.h>

#define MAP_SIZE        80 * 10
#define OVERLOOK_RADIUS 29 + 2

#ifndef CopyMemory

#define CopyMemory memcpy

#endif

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
    CopyMemory( temp_emptyMap,  g_emptyMap, sizeof( temp_emptyMap ) );

    memset( &temp_map, 0,     sizeof( temp_map ) );
    CopyMemory( temp_map,  g_map, sizeof( temp_map ) );

    memset( &temp_guardiansPositions, 0,                    sizeof( g_guardiansPositions ) );
    CopyMemory( temp_guardiansPositions,  g_guardiansPositions, sizeof( g_guardiansPositions ) );

    memset( &temp_randomMonstersPositions, 0,                         sizeof( g_randomMonstersPositions ) );
    CopyMemory( temp_randomMonstersPositions,  g_randomMonstersPositions, sizeof( g_randomMonstersPositions ) );

    memset( &temp_followMonstersPositions, 0,                         sizeof( g_followMonstersPositions ) );
    CopyMemory( temp_followMonstersPositions,  g_followMonstersPositions, sizeof( g_followMonstersPositions ) );

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

    memset( &temp_vision, 0,        lengthOfString( g_vision ) );
    CopyMemory( temp_vision,  g_vision, lengthOfString( g_vision ) );

    ASSERT_STREQ( temp_vision, g_vision );

    doPlayerMove( RIGHT, false );

    getOverview( g_playerPosition );

    ASSERT_STRNE( temp_vision, g_vision );

    memset( &temp_vision, 0,        lengthOfString( g_vision ) );
    CopyMemory( temp_vision,  g_vision, lengthOfString( g_vision ) );

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

    ASSERT_TRUE( doPlayerMove( DOWN, false ) );
    ASSERT_NE( temp_playerPosition, g_playerPosition );

    temp_playerPosition = g_playerPosition;

    ASSERT_TRUE( doPlayerMove( 5, false ) ); // + 5 from current position, '.' or '#' expected
    ASSERT_EQ( temp_playerPosition, g_playerPosition );

    initMap(); // Reset player position

    g_guardiansLeft = 0; // We can pass through door if all guardians annihilated

    ASSERT_FALSE( doPlayerMove( 396, false ) ); // + 396 from current position, door expected ( '>' or '<' )
    ASSERT_EQ( temp_playerPosition, g_playerPosition );
}