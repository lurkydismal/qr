#include "main.hpp"
#include "stdfunc.hpp"

item_t   playerInventory[ MAX_PLAYER_ITEM_COUNT ];
uint32_t playerHealth             = 2;
uint32_t playerInventoryItemCount = 0;
uint32_t playerExperience         = 0;
uint32_t playerPosition;

uint32_t    g_randomMonstersPositions[ MAX_MONSTERS_ON_MAP ];
uint32_t    g_followMonstersPositions[ MAX_MONSTERS_ON_MAP ];
uint32_t    g_guardiansPositions[ MAX_GUARDIANS_ON_MAP ];
const char* monsters           = "FR";
uint32_t    monsterHealth      = MONSTER_MAX_HEALTH;
uint32_t    randomMonstersLeft = 0;
uint32_t    followMonstersLeft = 0;
const char* key_monsters       = "A"; // they drop keys
uint32_t    keyMonsterHealth   = KEY_MONSTER_MAX_HEALTH;
uint32_t    guardianHealth     = GUARDIAN_MAX_HEALTH;
uint32_t    guardiansLeft      = 0;

const char* emptyMap = (
"\
+------+     +-------+   ##                  ######                             \
|......|  ###........|    #             +----.----+        #                    \
|.......###  |.......|    #             |.........|        #                    \
|......|  #  |........###### ###########..........|     ####                    \
|......|     |.......|      #           |.........|     #                       \
+------+     |.......|            +-----+.........|     #                       \
|......|     +-------+            |................#####                        \
|......+-----+                    |...............|                             \
>.............####################................|                             \
+------------+                    +---------------+                           \n\
");

char g_map[ MAP_SIZE ] = (
"\
+------+     +-------+   M#                  K####T                             \
|....K.|  ###}.......|    #             +----/----+        C                    \
|.@.....###  |.......|    #             |.........|        #                    \
|......|  #  |........###### ###########}.........|     W##M                    \
|......|     |.M.....|      K           |.........|     #                       \
+------+     |...C...|            +-----+C........|     #                       \
|.G....|     +-------+            |.....C.M.......{#####                        \
|......+-----+                    |..........M....|                             \
>............}K###################}....W..........|                             \
+------------+                    +---------------+                           \n\
");

char vision[ OVERLOOK_RADIUS ];

void initMap( void ) {
    for ( uint32_t l_mapCell = 0; l_mapCell < MAP_SIZE; l_mapCell++ ) {
        switch ( g_map[ l_mapCell ] ) {
            case '@': // Player
            {
                playerPosition = l_mapCell;
                break;
            }

            case 'G': // Guardian
            {
                g_guardiansPositions[ guardiansLeft ] = l_mapCell;
                guardiansLeft++;
                break;
            }

            // Spawn monsters
            case 'M': // Random Monster
            {
                g_map[ l_mapCell ] = monsters[ Rand() % 2 ];

                if ( g_map[ l_mapCell ] == 'R' ) {
                    g_randomMonstersPositions[ randomMonstersLeft ] = l_mapCell;
                    randomMonstersLeft++;
                } else {
                    g_followMonstersPositions[ followMonstersLeft ] = l_mapCell;
                    followMonstersLeft++;
                }
                break;
            }

            case 'W': // Random Monster with key
            {
                g_map[ l_mapCell ] = key_monsters[ Rand() % 1 ];
                break;
            }
        };
    }
}

void initInventory( item_t _item ) {
    playerInventoryItemCount = ( _item == EMPTY )
        ? 0
        : MAX_PLAYER_ITEM_COUNT;
    uint32_t l_inventoryCell = MAX_PLAYER_ITEM_COUNT;

    while ( l_inventoryCell-- ) {
        playerInventory[ l_inventoryCell ] = _item;
    }
}

bool checkLose( void ) {
    if ( playerHealth ) {
        #ifdef _WIN32
        MessageBoxA( 0, "You Win!", "Victory!", 0 );
        #endif

        return (true);

    } else {
        #ifdef _WIN32
        MessageBoxA( 0, "You Lose!", "Defeat!", 0 );
        #endif

        return (false);
    }
}

void getOverview( const uint32_t currentPosition ) {
    uint32_t l_startPosition = ( currentPosition - ( 80 * 2 ) - 2 );
    uint32_t counter         = 0;

    for ( uint32_t i = 1; i < 6; i++ ) {
        if ( ( l_startPosition % 80 ) > ( ( currentPosition % 80 ) + 2 ) ) {
            l_startPosition++;

        } else if ( ( l_startPosition % 80 ) < ( ( currentPosition % 80 ) - 2 ) ) {
            l_startPosition--;
        }

        for ( uint32_t l_mapCell = l_startPosition; l_mapCell < ( l_startPosition + 4 + 1 ); l_mapCell++ ) {
            vision[ counter ] = g_map[ l_mapCell ];
            counter++;
        }

        vision[ counter ] = '\n';
        counter++;

        l_startPosition = ( currentPosition - ( 80 * ( 2 - i ) ) - 2 );
    }

    vision[ counter ] = '\0';
}

int32_t getPlayerInventoryPlaceOf( const item_t _item ) {
    for ( uint32_t l_inventoryCell = 0; l_inventoryCell < MAX_PLAYER_ITEM_COUNT; l_inventoryCell++ ) {
        if ( playerInventory[ l_inventoryCell ] == _item ) {
            return ( (int32_t)l_inventoryCell );
        }
    }

    return ( INT8_MIN );
}

bool inventoryAdd( const item_t _item, const int32_t _itemIndex ) {
    if (
        ( playerInventoryItemCount >= MAX_PLAYER_ITEM_COUNT ) ||
        (
            ( _itemIndex != INT8_MIN ) &&
            ( playerInventory[ _itemIndex ] != EMPTY )
        )
    ) {
        return (false);
    }

    const int32_t l_emptyIndex = ( _itemIndex != INT8_MIN )
        ? _itemIndex
        : getPlayerInventoryPlaceOf( EMPTY );

    playerInventory[ l_emptyIndex ] = _item;
    playerInventoryItemCount++;

    return (true);
}

bool usePlayerItem( const item_t _item ) {
    int l_itemPlace = getPlayerInventoryPlaceOf( _item );

    if ( ( !playerInventoryItemCount ) || ( l_itemPlace < 0 ) ) {
        return (false);
    }

    playerInventory[ l_itemPlace ] = EMPTY;
    playerInventoryItemCount--;

    return (true);
}

uint32_t move( const char who, uint32_t currentPosition, const int32_t l_offset ) {
    g_map[ currentPosition ] = emptyMap[ currentPosition ];
    currentPosition += l_offset;
    g_map[ currentPosition ] = who;

    return ( currentPosition );
}

bool DoPlayerMove( const int32_t l_offset )  {
    const char cellToMove = g_map[ playerPosition + l_offset ];

    for ( uint32_t i = 0; i < MONSTER_TYPES; i++ ) {
        if ( cellToMove == monsters[ i ] ) {
            playerPosition = fightMonster( '@', playerPosition, l_offset );

            return ( playerPosition );
        }
    }

    for ( uint32_t i = 0; i < KEY_MONSTER_TYPES; i++ ) {
        if ( cellToMove == key_monsters[ i ] ) {
            playerPosition = fightKeyMonster( '@', playerPosition, l_offset );

            return ( playerPosition );
        }
    }

    switch ( g_map[ playerPosition + l_offset ] ) {
        case '.':
        case '#':
            playerPosition = move( '@', playerPosition, l_offset );
            break;

        case '}':
        case '{':
        case '/':
            if ( usePlayerItem( KEY ) )
                playerPosition = move( '@', playerPosition, l_offset );
            break;

        case '>':
        case '<':
            if ( !guardiansLeft )
                return false;
            break;

        case 'G':
            playerPosition = fightGuardian( '@', playerPosition, l_offset );

            return ( playerPosition );
            break;

        case KEY:
            if ( playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
                playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = KEY;
                playerInventoryItemCount++;
                playerPosition = move( '@', playerPosition, l_offset );
            }
            break;

        case HEALTH:
            if ( playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
                playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = HEALTH;
                playerInventoryItemCount++;
                playerPosition = move( '@', playerPosition, l_offset );
            }
            break;
        
        case ATTACK:
            if ( playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
                playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = ATTACK;
                playerInventoryItemCount++;
                playerPosition = move( '@', playerPosition, l_offset );
            }
            break;
        
        case DEFENCE:
            if ( playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
                playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = DEFENCE;
                playerInventoryItemCount++;
                playerPosition = move( '@', playerPosition, l_offset );
            }
            break;

        case 'C':
            if ( playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
                const item_t l_chestItems[ CHEST_ITEM_COUNT ] = { HEALTH, ATTACK, DEFENCE };
                playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = l_chestItems[ ( Rand() % CHEST_ITEM_COUNT ) ];
                playerInventoryItemCount++;
                playerPosition = move( '@', playerPosition, l_offset );
            }
            break;

        case 'T':
            if ( playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
                playerExperience += ( ( Rand() % MAX_EXPERIENCE_FROM_TREASURE ) + 1 );
                playerPosition = move( '@', playerPosition, l_offset );
            }
            break;
    };

    return (true);
}

bool l_watchForPlayer( void ) {
    for ( uint_fast64_t _visionCellIndex = 0; _visionCellIndex < stringLength( vision ); _visionCellIndex++ ) {
        if ( vision[ _visionCellIndex ] == '@' ) {
            return (true);
        }
    }

    return (false);
}

void l_randomMove( const char who, uint32_t& currentPosition ) {
    direction_t l_offset;

    switch ( (direction_t)( Rand() % 4 ) ) {
        case DOWN:
        {
            l_offset = DOWN;
            break;
        }

        case UP:
        {
            l_offset = UP;
            break;
        }

        case LEFT:
        {
            l_offset = LEFT;
            break;
        }

        case RIGHT:
        {
            l_offset = RIGHT;
            break;
        }
    };

    if (
        ( g_map[ currentPosition + l_offset ] == '.' ) ||
        ( g_map[ currentPosition + l_offset ] == '#' )
    ) {
        currentPosition = move( who, currentPosition, l_offset );

    } else if ( g_map[ currentPosition + l_offset ] == '@' ) {
        currentPosition = fightMonster( who, currentPosition, l_offset );
    }
}

void l_followMove( const char who, uint32_t& currentPosition ) {
    direction_t l_offset;

    if ( ( currentPosition % 80 ) < ( playerPosition % 80 ) ) {
        l_offset = RIGHT;

    } else if ( ( currentPosition % 80 ) > ( playerPosition % 80 ) ) {
        l_offset = LEFT;

    } else if ( currentPosition < playerPosition ) {
        l_offset = DOWN;

    } else {
        l_offset = UP;
    }

    if ( ( g_map[ currentPosition + l_offset ] == '.' ) || ( g_map[ currentPosition + l_offset ] == '#' ) ) {
        currentPosition = move( who, currentPosition, l_offset );

    } else if ( g_map[ currentPosition + l_offset ] == '@' ) {
        if ( who == 'G' ) {
            currentPosition = fightGuardian( who, currentPosition, l_offset );

        } else {
            currentPosition = fightMonster( who, currentPosition, l_offset );
        }
    }
}

bool DoOpponentMove( void )  {
    for ( uint32_t i = 0; i < randomMonstersLeft; i++ ) {
        l_randomMove( 'R', g_randomMonstersPositions[ i ] );
    }

    for ( uint32_t i = 0; i < followMonstersLeft; i++ ) {
        if ( l_watchForPlayer() ) {
            l_followMove( 'F', g_followMonstersPositions[ i ] );

        } else {
            l_randomMove( 'F', g_followMonstersPositions[ i ] );
        }
    }

    for ( uint32_t i = 0; i < guardiansLeft; i++ )
        if ( l_watchForPlayer() ) {
            l_followMove( 'G', g_guardiansPositions[ i ] );

        } else {
            l_randomMove( 'G', g_guardiansPositions[ i ] );
        }

    return (true);
}

uint32_t fightMonster( const char who, uint32_t currentPosition, const int32_t l_offset ) {
    playerHealth -= 1;

    if ( !playerHealth ) {
        if ( usePlayerItem( HEALTH ) ) {
            playerHealth += 20;

        } else {
            return ( (uint32_t)0 );
        }
    }

    monsterHealth -= ( usePlayerItem( ATTACK ) )
        ? ( ( 5 + (uint32_t)( playerExperience / EXPERIENCE_FOR_DMG ) ) * 2 )
        : ( 5 + (uint32_t)( playerExperience / EXPERIENCE_FOR_DMG ) );

    if ( !monsterHealth ) {
        monsterHealth = MONSTER_MAX_HEALTH;

        if ( who == '@' ) {
            if ( g_map[ currentPosition + l_offset ] == 'R'  ) {
                pop( g_randomMonstersPositions, currentPosition + l_offset );
                randomMonstersLeft--;

            } else {
                pop( g_followMonstersPositions, currentPosition + l_offset );
                followMonstersLeft--;
            }

            return ( move( who, currentPosition, l_offset ) );

        } else {
            if ( who == 'R' ) {
                pop( g_randomMonstersPositions, currentPosition );
                randomMonstersLeft--;

            } else {
                pop( g_followMonstersPositions, currentPosition );
                followMonstersLeft--;
            }

            g_map[ currentPosition ] = emptyMap[ currentPosition ];

            return ( (uint32_t)1 );
        }
    }

    return ( currentPosition );
}

uint32_t fightKeyMonster( const char who, uint32_t currentPosition, const int32_t l_offset ) {
    playerHealth -= 1;

    if ( !playerHealth ) {
        if ( usePlayerItem( HEALTH ) ) {
            playerHealth += 20;

        } else {
            return ( (uint32_t)0 );
        }
    }

    keyMonsterHealth -= ( usePlayerItem( ATTACK ) )
        ? ( ( 5 + (uint32_t)( playerExperience / EXPERIENCE_FOR_DMG ) ) * 2 )
        : ( 5 + (uint32_t)( playerExperience / EXPERIENCE_FOR_DMG ) );

    if ( !keyMonsterHealth ) {
        if ( playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) { // monster are invulnerable, while player has no empty item slot
            playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = KEY;
            playerInventoryItemCount++;
            keyMonsterHealth = KEY_MONSTER_MAX_HEALTH;

            return ( move( who, currentPosition, l_offset ) );
        }
    }

    return ( currentPosition );
}

uint32_t fightGuardian( const char who, uint32_t currentPosition, const int32_t l_offset ) {
    playerHealth -= ( usePlayerItem( DEFENCE ) ) ? 1 : 2;

    if ( !playerHealth ) {
        if ( usePlayerItem( HEALTH ) ) {
            playerHealth += 20;

        } else {
            return ( (uint32_t)0 );
        }
    }

    guardianHealth -= ( usePlayerItem( ATTACK ) )
        ? ( ( 1 + (uint32_t)( playerExperience / EXPERIENCE_FOR_SUPER_DMG ) ) + 1 )
        : ( 1 + (uint32_t)( playerExperience / EXPERIENCE_FOR_SUPER_DMG ) );

    if ( !guardianHealth ) {
        guardianHealth = GUARDIAN_MAX_HEALTH;
        guardiansLeft--;
        if ( who == '@' ) {
            pop( g_guardiansPositions, currentPosition + l_offset );
            return ( move( who, currentPosition, l_offset ) );

        } else {
            pop( g_guardiansPositions, currentPosition );
        }

        g_map[ currentPosition ] = emptyMap[ currentPosition ];
    }

    return ( currentPosition );
}

void UpdateScreen( void )  {
    clearConsole();
    getOverview( playerPosition );
    print( vision, stringLength( vision ) );

    print( "\nHP:", 5 );
    char* l_buffer = (char*)Malloc( lengthOfInt( playerHealth ) );
    Ltoa( playerHealth, l_buffer );
    print( l_buffer, lengthOfInt( playerHealth ) );
    Free( l_buffer );
    print( "\nEXP:", 6 );
    l_buffer = (char*)Malloc( lengthOfInt( playerExperience ) );
    Ltoa( playerExperience, l_buffer );
    print( l_buffer, lengthOfInt( playerExperience ) );
    Free( l_buffer );

    print( "\nITEMS:", 8 );

    l_buffer = (char*)Malloc( MAX_PLAYER_ITEM_COUNT * 2 );

    for ( uint32_t l_item = 0; l_item < MAX_PLAYER_ITEM_COUNT; l_item++ ) {
        if ( playerInventory[ l_item ] ) {
            l_buffer[ 0 ] = playerInventory[ l_item ];
            l_buffer[ 1 ] = ' ';
            l_buffer[ 2 ] = '\0';
            print( l_buffer, 3 );
        }
    }

    Free( l_buffer );
}