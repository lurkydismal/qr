#include "main.hpp"
#include "stdfunc.hpp"

item_t   g_playerInventory[ MAX_PLAYER_ITEM_COUNT ];
uint32_t g_playerInventoryItemCount = 0;
uint32_t g_playerHealth             = 2;
uint32_t g_playerExperience         = 0;
uint32_t g_playerPosition;

uint32_t      g_guardiansPositions[ MAX_GUARDIANS_ON_MAP ];
uint32_t      g_followMonstersPositions[ MAX_MONSTERS_ON_MAP ];
uint32_t      g_randomMonstersPositions[ MAX_MONSTERS_ON_MAP ];
uint32_t      g_guardianHealth     = GUARDIAN_MAX_HEALTH;
uint32_t      g_monsterHealth      = MONSTER_MAX_HEALTH;
uint32_t      g_keyMonsterHealth   = KEY_MONSTER_MAX_HEALTH;
const char    g_monsters[]         = { FOLLOW_MONSTER, RANDOM_MONSTER };
const char    g_keyMonsters[]      = { KEY_MONSTER };
uint32_t      g_guardiansLeft      = 0;
uint32_t      g_followMonstersLeft = 0;
uint32_t      g_randomMonstersLeft = 0;

const char* g_emptyMap = (
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

char g_vision[ OVERLOOK_RADIUS ];

void initMap( void ) {
    for ( uint32_t l_mapCell = 0; l_mapCell < MAP_SIZE; l_mapCell++ ) {
        switch ( g_map[ l_mapCell ] ) {
            case PLAYER:
            {
                g_playerPosition = l_mapCell;

                break;
            }

            // Spawn opponents
            case GUARDIAN:
            {
                g_guardiansPositions[ g_guardiansLeft ] = l_mapCell;
                g_guardiansLeft++;

                break;
            }

            case MONSTER:
            {
                g_map[ l_mapCell ] = g_monsters[ Rand() % 2 ];

                if ( g_map[ l_mapCell ] == 'R' ) {
                    g_randomMonstersPositions[ g_randomMonstersLeft ] = l_mapCell;
                    g_randomMonstersLeft++;

                } else {
                    g_followMonstersPositions[ g_followMonstersLeft ] = l_mapCell;
                    g_followMonstersLeft++;
                }

                break;
            }

            case MONSTER_WITH_A_KEY:
            {
                g_map[ l_mapCell ] = g_keyMonsters[ Rand() % 1 ];

                break;
            }
        };
    }
}

void initInventory( item_t _item ) {
    g_playerInventoryItemCount = ( _item == EMPTY )
        ? 0
        : MAX_PLAYER_ITEM_COUNT;
    uint32_t l_inventoryCell = MAX_PLAYER_ITEM_COUNT;

    while ( l_inventoryCell-- ) {
        g_playerInventory[ l_inventoryCell ] = _item;
    }
}

void getOverview( const uint32_t _currentPosition ) {
    uint32_t l_startPosition = ( _currentPosition - ( 80 * 2 ) - 2 );
    uint32_t l_counter         = 0;

    for ( uint32_t i = 1; i < 6; i++ ) {
        if ( ( l_startPosition % 80 ) > ( ( _currentPosition % 80 ) + 2 ) ) {
            l_startPosition++;

        } else if ( ( l_startPosition % 80 ) < ( ( _currentPosition % 80 ) - 2 ) ) {
            l_startPosition--;
        }

        for ( uint32_t _mapCell = l_startPosition; _mapCell < ( l_startPosition + 4 + 1 ); _mapCell++ ) {
            g_vision[ l_counter ] = g_map[ _mapCell ];
            l_counter++;
        }

        g_vision[ l_counter ] = '\n';
        l_counter++;

        l_startPosition = ( _currentPosition - ( 80 * ( 2 - i ) ) - 2 );
    }

    g_vision[ l_counter ] = '\0';
}

int32_t getPlayerInventoryPlaceOf( const item_t _item ) {
    for ( uint32_t _inventoryCell = 0; _inventoryCell < MAX_PLAYER_ITEM_COUNT; _inventoryCell++ ) {
        if ( g_playerInventory[ _inventoryCell ] == _item ) {
            return ( (int32_t)_inventoryCell );
        }
    }

    return ( INT8_MIN );
}

bool inventoryAdd( const item_t _item, const int32_t _itemIndex ) {
    if (
        ( g_playerInventoryItemCount >= MAX_PLAYER_ITEM_COUNT ) ||
        (
            ( _itemIndex != INT8_MIN ) &&
            ( g_playerInventory[ _itemIndex ] != EMPTY )
        )
    ) {
        return (false);
    }

    const int32_t l_emptyIndex = ( _itemIndex != INT8_MIN )
        ? _itemIndex
        : getPlayerInventoryPlaceOf( EMPTY );

    g_playerInventory[ l_emptyIndex ] = _item;
    g_playerInventoryItemCount++;

    return (true);
}

bool usePlayerItem( const item_t _item ) {
    int l_itemPlace = getPlayerInventoryPlaceOf( _item );

    if ( ( !g_playerInventoryItemCount ) || ( l_itemPlace < 0 ) ) {
        return (false);
    }

    g_playerInventory[ l_itemPlace ] = EMPTY;
    g_playerInventoryItemCount--;

    return (true);
}

uint32_t move( const char _who, uint32_t _currentPosition, const int32_t _offset ) {
    g_map[ _currentPosition ] = g_emptyMap[ _currentPosition ];
    _currentPosition += _offset;
    g_map[ _currentPosition ] = _who;

    return ( _currentPosition );
}

bool doPlayerMove( const uint32_t _offset ) {
    const char l_cellToMove = g_map[ g_playerPosition + _offset ];

    for ( uint32_t i = 0; i < MONSTER_TYPES; i++ ) {
        if ( l_cellToMove == g_monsters[ i ] ) {
            g_playerPosition = fightMonster( PLAYER, g_playerPosition, _offset );

            return ( g_playerPosition );
        }
    }

    for ( uint32_t i = 0; i < KEY_MONSTER_TYPES; i++ ) {
        if ( l_cellToMove == g_keyMonsters[ i ] ) {
            g_playerPosition = fightKeyMonster( PLAYER, g_playerPosition, _offset );

            return ( g_playerPosition );
        }
    }

    switch ( g_map[ g_playerPosition + _offset ] ) {
        case '.':
        case '#':
        {
            g_playerPosition = move( PLAYER, g_playerPosition, _offset );

            break;
        }

        case '}':
        case '{':
        case '/':
        {
            if ( usePlayerItem( KEY ) ){
                g_playerPosition = move( PLAYER, g_playerPosition, _offset );
            }

            break;
        }

        case '>':
        case '<':
        {
            if ( !g_guardiansLeft ){
                return ( false );
            }

            break;
        }

        case GUARDIAN:
        {
            g_playerPosition = fightGuardian( PLAYER, g_playerPosition, _offset );

            return ( g_playerPosition );
        }

        case KEY:
        {
            if ( g_playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                g_playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = KEY;
                g_playerInventoryItemCount++;

                g_playerPosition = move( PLAYER, g_playerPosition, _offset );
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
            }

            break;
        }

        case HEALTH:
        {
            if ( g_playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                g_playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = HEALTH;
                g_playerInventoryItemCount++;

                g_playerPosition = move( PLAYER, g_playerPosition, _offset );
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
            }

            break;
        }

        case ATTACK:
        {
            if ( g_playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                g_playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = ATTACK;
                g_playerInventoryItemCount++;

                g_playerPosition = move( PLAYER, g_playerPosition, _offset );
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
            }

            break;
        }

        case DEFENCE:
        {
            if ( g_playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                g_playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = DEFENCE;
                g_playerInventoryItemCount++;

                g_playerPosition = move( PLAYER, g_playerPosition, _offset );
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
            }

            break;
        }

        case CHEST:
        {
            if ( g_playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                const item_t l_chestItems[] = { HEALTH, ATTACK, DEFENCE };

                g_playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = l_chestItems[ ( Rand() % CHEST_ITEM_COUNT ) ];
                g_playerInventoryItemCount++;

                g_playerPosition = move( PLAYER, g_playerPosition, _offset );
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
            }

            break;
        }

        case TREASURE:
        {
            if ( g_playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                g_playerExperience += ( ( Rand() % MAX_EXPERIENCE_FROM_TREASURE ) + 1 );

                g_playerPosition = move( PLAYER, g_playerPosition, _offset );
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
            }

            break;
        }
    };

    return (true);
}

static void _randomMove( const char _who, uint32_t& _currentPosition ) {
    direction_t l_offset;

    switch ( Rand() % 4 ) {
        case 1:
        {
            l_offset = DOWN;

            break;
        }

        case 2:
        {
            l_offset = UP;

            break;
        }

        case 3:
        {
            l_offset = LEFT;

            break;
        }

        case 0:
        {
            l_offset = RIGHT;

            break;
        }

        default:
        {
            l_offset = STAY;
        }
    };

    if (
        ( g_map[ _currentPosition + l_offset ] == '.' ) ||
        ( g_map[ _currentPosition + l_offset ] == '#' )
    ) {
        _currentPosition = move( _who, _currentPosition, l_offset );

    } else if ( g_map[ _currentPosition + l_offset ] == PLAYER ) {
        _currentPosition = fightMonster( _who, _currentPosition, l_offset );
    }
}

static void _followMove( const char _who, uint32_t& _currentPosition ) {
    direction_t l_offset;

    if ( ( _currentPosition % 80 ) < ( g_playerPosition % 80 ) ) {
        l_offset = RIGHT;

    } else if ( ( _currentPosition % 80 ) > ( g_playerPosition % 80 ) ) {
        l_offset = LEFT;

    } else if ( _currentPosition < g_playerPosition ) {
        l_offset = DOWN;

    } else {
        l_offset = UP;
    }

    if (
        ( g_map[ _currentPosition + l_offset ] == '.' ) ||
        ( g_map[ _currentPosition + l_offset ] == '#' )
    ) {
        _currentPosition = move( _who, _currentPosition, l_offset );

    } else if ( g_map[ _currentPosition + l_offset ] == PLAYER ) {
        if ( _who == GUARDIAN ) {
            _currentPosition = fightGuardian( _who, _currentPosition, l_offset );

        } else {
            _currentPosition = fightMonster( _who, _currentPosition, l_offset );
        }
    }
}

bool doOpponentMove( void ) {
    bool l_isPlayerInVision = false;

    for ( uint_fast64_t _visionCellIndex = 0; _visionCellIndex < stringLength( g_vision ); _visionCellIndex++ ) {
        if ( g_vision[ _visionCellIndex ] == PLAYER ) {
            l_isPlayerInVision = true;
        }
    }

    for ( uint32_t i = 0; i < g_randomMonstersLeft; i++ ) {
        _randomMove( RANDOM_MONSTER, g_randomMonstersPositions[ i ] );
    }

    for ( uint32_t i = 0; i < g_followMonstersLeft; i++ ) {
        if ( l_isPlayerInVision ) {
            _followMove( FOLLOW_MONSTER, g_followMonstersPositions[ i ] );

        } else {
            _randomMove( FOLLOW_MONSTER, g_followMonstersPositions[ i ] );
        }
    }

    for ( uint32_t i = 0; i < g_guardiansLeft; i++ )
        if ( l_isPlayerInVision ) {
            _followMove( GUARDIAN, g_guardiansPositions[ i ] );

        } else {
            _randomMove( GUARDIAN, g_guardiansPositions[ i ] );
        }

    return (true);
}

uint32_t fightMonster( const char _who, uint32_t _currentPosition, const int32_t _offset ) {
    g_playerHealth -= 1;

    if ( !g_playerHealth ) {
        if ( usePlayerItem( HEALTH ) ) {
            g_playerHealth += 20;

        } else {
            return ( (uint32_t)0 );
        }
    }

    g_monsterHealth -= ( usePlayerItem( ATTACK ) )
        ? ( ( 5 + (uint32_t)( g_playerExperience / EXPERIENCE_FOR_DMG ) ) * 2 )
        : ( 5 + (uint32_t)( g_playerExperience / EXPERIENCE_FOR_DMG ) );

    if ( !g_monsterHealth ) {
        g_monsterHealth = MONSTER_MAX_HEALTH;

        if ( _who == PLAYER ) {
            if ( g_map[ _currentPosition + _offset ] == RANDOM_MONSTER  ) {
                pop( g_randomMonstersPositions, _currentPosition + _offset );
                g_randomMonstersLeft--;

            } else {
                pop( g_followMonstersPositions, _currentPosition + _offset );
                g_followMonstersLeft--;
            }

            return ( move( _who, _currentPosition, _offset ) );

        } else {
            if ( _who == RANDOM_MONSTER ) {
                pop( g_randomMonstersPositions, _currentPosition );
                g_randomMonstersLeft--;

            } else {
                pop( g_followMonstersPositions, _currentPosition );
                g_followMonstersLeft--;
            }

            g_map[ _currentPosition ] = g_emptyMap[ _currentPosition ];

            return ( (uint32_t)1 );
        }
    }

    return ( _currentPosition );
}

uint32_t fightKeyMonster( const char _who, uint32_t _currentPosition, const int32_t _offset ) {
    g_playerHealth -= 1;

    if ( !g_playerHealth ) {
        if ( usePlayerItem( HEALTH ) ) {
            g_playerHealth += 20;

        } else {
            return ( (uint32_t)0 );
        }
    }

    g_keyMonsterHealth -= ( usePlayerItem( ATTACK ) )
        ? ( ( 5 + (uint32_t)( g_playerExperience / EXPERIENCE_FOR_DMG ) ) * 2 )
        : ( 5 + (uint32_t)( g_playerExperience / EXPERIENCE_FOR_DMG ) );

    if ( !g_keyMonsterHealth ) {
        if ( g_playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) { // monster are invulnerable, while player has no empty item slot
            g_playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = KEY;
            g_playerInventoryItemCount++;
            g_keyMonsterHealth = KEY_MONSTER_MAX_HEALTH;

            return ( move( _who, _currentPosition, _offset ) );
        }
    }

    return ( _currentPosition );
}

uint32_t fightGuardian( const char _who, uint32_t _currentPosition, const int32_t _offset ) {
    g_playerHealth -= ( usePlayerItem( DEFENCE ) ) ? 1 : 2;

    if ( !g_playerHealth ) {
        if ( usePlayerItem( HEALTH ) ) {
            g_playerHealth += 20;

        } else {
            return ( (uint32_t)0 );
        }
    }

    g_guardianHealth -= ( usePlayerItem( ATTACK ) )
        ? ( ( 1 + (uint32_t)( g_playerExperience / EXPERIENCE_FOR_SUPER_DMG ) ) + 1 )
        : ( 1 + (uint32_t)( g_playerExperience / EXPERIENCE_FOR_SUPER_DMG ) );

    if ( !g_guardianHealth ) {
        g_guardianHealth = GUARDIAN_MAX_HEALTH;
        g_guardiansLeft--;

        if ( _who == PLAYER ) {
            pop( g_guardiansPositions, _currentPosition + _offset );
            return ( move( _who, _currentPosition, _offset ) );

        } else {
            pop( g_guardiansPositions, _currentPosition );
        }

        g_map[ _currentPosition ] = g_emptyMap[ _currentPosition ];
    }

    return ( _currentPosition );
}

void updateScreen( void ) {
    clearConsole();
    getOverview( g_playerPosition );
    print( g_vision, stringLength( g_vision ) );

    print( "\nHP:", 5 );
    char* l_buffer = (char*)Malloc( lengthOfInt( g_playerHealth ) );
    Ltoa( g_playerHealth, l_buffer );
    print( l_buffer, lengthOfInt( g_playerHealth ) );
    Free( l_buffer );
    print( "\nEXP:", 6 );
    l_buffer = (char*)Malloc( lengthOfInt( g_playerExperience ) );
    Ltoa( g_playerExperience, l_buffer );
    print( l_buffer, lengthOfInt( g_playerExperience ) );
    Free( l_buffer );

    print( "\nITEMS:", 8 );

    l_buffer = (char*)Malloc( MAX_PLAYER_ITEM_COUNT * 2 );

    for ( uint32_t l_item = 0; l_item < MAX_PLAYER_ITEM_COUNT; l_item++ ) {
        if ( g_playerInventory[ l_item ] ) {
            l_buffer[ 0 ] = g_playerInventory[ l_item ];
            l_buffer[ 1 ] = ' ';
            l_buffer[ 2 ] = '\0';
            print( l_buffer, 3 );
        }
    }

    Free( l_buffer );
}