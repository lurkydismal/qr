#include "stdfunc.hpp" // Rand clear_screen
#include "printChar.h" // printChar

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

// Enums
enum ITEMS { // Player inventory
    EMPTY   = 'E',
    KEY     = 'K',
    HEALTH  = 'H',
    ATTACK  = 'A',
    DEFENCE = 'D',
} items ;

enum MOVEMENT_DIRECTIONS { // Directions 2 8 4 6
    DOWN = 0,
    UP,
    LEFT,
    RIGHT,
} movement ;

// Global
unsigned int playerHealth = MAX_PLAYER_HEALTH;
ITEMS playerInventory[ MAX_PLAYER_ITEM_COUNT ];
unsigned int playerInventoryItemCount = 0;
unsigned int playerExperience = 0;
unsigned int playerPosition;

const char* monsters = "FR";
unsigned int monsterHealth = MONSTER_MAX_HEALTH;
unsigned int g_randomMonstersPositions[ MAX_MONSTERS_ON_MAP ];
unsigned int randomMonstersLeft = 0;
unsigned int g_followMonstersPositions[ MAX_MONSTERS_ON_MAP ];
unsigned int followMonstersLeft = 0;

const char* key_monsters = "A"; // they drop keys
unsigned int keyMonsterHealth = KEY_MONSTER_MAX_HEALTH;

unsigned int guardianHealth = GUARDIAN_MAX_HEALTH;
unsigned int g_guardiansPositions[ MAX_GUARDIANS_ON_MAP ];
unsigned int guardiansLeft = 0;

// Function declarations
bool DoPlayerMove( int offset ) noexcept;
bool DoOpponentMove() noexcept;
bool usePlayerItem( const ITEMS item );
void UpdateScreen() noexcept;

#ifndef _WIN32
bool GetAsyncKeyState( unsigned int );
#endif

// Maps
const char emptyMap[ MAP_SIZE ] = (
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

// lambdas
void getOverview( const unsigned int currentPosition ) {
    unsigned int l_startPosition = ( currentPosition - ( 80 * 2 ) - 2 );
    unsigned int counter = 0;

    for ( unsigned int i = 1; i < 6; i++ ) {
        if ( ( l_startPosition % 80 ) > ( ( currentPosition % 80 ) + 2 ) )
            l_startPosition++;
        else if ( ( l_startPosition % 80 ) < ( ( currentPosition % 80 ) - 2 ) )
            l_startPosition--;

        for ( unsigned int l_mapCell = l_startPosition; l_mapCell < ( l_startPosition + 4 + 1 ); l_mapCell++ ) {
            vision[ counter ] = g_map[ l_mapCell ];
            counter++;
        }

        vision[ counter ] = '\n';
        counter++;

        l_startPosition = ( currentPosition - ( 80 * ( 2 - i ) ) - 2 );
    }

    vision[ counter ] = '\0';
}

int getPlayerInventoryPlaceOf( const ITEMS l_item ) {
    for ( unsigned int l_inventoryCell = 0; l_inventoryCell < MAX_PLAYER_ITEM_COUNT; l_inventoryCell++ )
        if ( playerInventory[ l_inventoryCell ] == l_item )
            return ( (int)l_inventoryCell );

    return ( (int)-1 );
}

int main() {
    // Map parse
    for ( unsigned int l_mapCell = 0; l_mapCell < MAP_SIZE; l_mapCell++ ) {
        switch ( g_map[ l_mapCell ] ) {
            case '@': // Player
                playerPosition = l_mapCell;
                break;

            case 'G': // Guardian
                g_guardiansPositions[ guardiansLeft ] = l_mapCell;
                guardiansLeft++;
                break;

            // Spawn monsters
            case 'M': // Random Monster
                g_map[ l_mapCell ] = monsters[ Rand() % 2 ];

                if ( g_map[ l_mapCell ] == 'R' ) {
                    g_randomMonstersPositions[ randomMonstersLeft ] = l_mapCell;
                    randomMonstersLeft++;
                } else {
                    g_followMonstersPositions[ followMonstersLeft ] = l_mapCell;
                    followMonstersLeft++;
                }
                break;

            case 'W': // Random Monster with key
                g_map[ l_mapCell ] = key_monsters[ Rand() % 1 ];
                break;
        };
    }

    #ifdef EMPTY_PLAYER_INVENTORY
        unsigned int l_inventoryCell = MAX_PLAYER_ITEM_COUNT;

        while ( l_inventoryCell-- )
            playerInventory[ l_inventoryCell ] = EMPTY;
    #endif

    playerInventory[ 2 ] = HEALTH;
    playerInventoryItemCount++;

    UpdateScreen(); // first render

    while ( true ) { // main loop
        // keyboard handling
        if ( GetAsyncKeyState( VK_NUMPAD5 ) ) {
            // logic
            if ( !DoOpponentMove() ) {
                break;
            }

            // render
            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD1 ) ) {
            // logic
            if ( !DoPlayerMove( 79 ) || !DoOpponentMove() ) {
                break;
            }

            // render
            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD2 ) ) {
            // logic
            if ( !DoPlayerMove( 80 ) || !DoOpponentMove() )
                break;

            // render
            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD3 ) ) {
            // logic
            if ( !DoPlayerMove( 81 ) || !DoOpponentMove() )
                break;

            // render
            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD4 ) ) {
            // logic
            if ( !DoPlayerMove( -1 ) || !DoOpponentMove() )
                break;

            // render
            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD6 ) ) {
            // logic
            if ( !DoPlayerMove( 1 ) || !DoOpponentMove() )
                break;

            // render
            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD7 ) ) {
            // logic
            if ( !DoPlayerMove( -81 ) || !DoOpponentMove() )
                break;

            // render
            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD8 ) ) {
            // logic
            if ( !DoPlayerMove( -80 ) || !DoOpponentMove() )
                break;

            // render
            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD9 ) ) {
            // logic
            if ( !DoPlayerMove( -79 ) || !DoOpponentMove() )
                break;
            
            // render
            UpdateScreen();
        }

        #ifdef _WIN32
        Sleep( 200 );
        #endif
    }

    // Lose check
    if ( !playerHealth ) {
        #ifdef _WIN32
        MessageBoxA( 0, "You Lose!", "Defeat!", 0 );
        #endif

    } else {
        #ifdef _WIN32
        MessageBoxA( 0, "You Win!", "Victory!", 0 );
        #endif
    }

    return (0);
}

// Lambda definitions
unsigned int move( const char who, unsigned int currentPosition, const int l_offset ) {
    g_map[ currentPosition ] = emptyMap[ currentPosition ];
    currentPosition += l_offset;
    g_map[ currentPosition ] = who;

    return ( currentPosition );
}

unsigned int fightMonster( const char who, unsigned int currentPosition, const int l_offset ) {
    playerHealth -= 1;

    if ( !playerHealth ) {
        if ( usePlayerItem( HEALTH ) ) {
            playerHealth += 20;

        } else {
            return ( (unsigned int)0 );
        }
    }

    monsterHealth -= ( usePlayerItem( ATTACK ) ) ? ( ( 5 + ( unsigned int )( playerExperience / EXPERIENCE_FOR_DMG ) ) * 2 ) : ( 5 + ( unsigned int )( playerExperience / EXPERIENCE_FOR_DMG ) );

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

            return ( (unsigned int)1 );
        }
    }

    return ( currentPosition );
}

unsigned int fightKeyMonster( const char who, unsigned int currentPosition, const int l_offset ) {
    playerHealth -= 1;

    if ( !playerHealth ) {
        if ( usePlayerItem( HEALTH ) ) {
            playerHealth += 20;

        } else {
            return ( (unsigned int)0 );
        }
    }

    keyMonsterHealth -= ( usePlayerItem( ATTACK ) ) ? ( ( 5 + ( unsigned int )( playerExperience / EXPERIENCE_FOR_DMG ) ) * 2 ) : ( 5 + ( unsigned int )( playerExperience / EXPERIENCE_FOR_DMG ) );

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

unsigned int fightGuardian( const char who, unsigned int currentPosition, const int l_offset ) {
    playerHealth -= ( usePlayerItem( DEFENCE ) ) ? 1 : 2;

    if ( !playerHealth ) {
        if ( usePlayerItem( HEALTH ) ) {
            playerHealth += 20;

        } else {
            return ( (unsigned int)0 );
        }
    }

    guardianHealth -= ( usePlayerItem( ATTACK ) ) ? ( ( 1 + ( unsigned int )( playerExperience / EXPERIENCE_FOR_SUPER_DMG ) ) + 1 ): 1 + ( unsigned int )( playerExperience / EXPERIENCE_FOR_SUPER_DMG );

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

// Function definitions
bool DoPlayerMove( const int l_offset ) noexcept {
    const char cellToMove = g_map[ playerPosition + l_offset ];

    for ( unsigned int i = 0; i < MONSTER_TYPES; i++ ) {
        if ( cellToMove == monsters[ i ] ) {
            playerPosition = fightMonster( '@', playerPosition, l_offset );

            return ( playerPosition );
        }
    }

    for ( unsigned int i = 0; i < KEY_MONSTER_TYPES; i++ ) {
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

        case 'K':
            if ( playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
                playerInventory[ getPlayerInventoryPlaceOf( EMPTY ) ] = KEY;
                playerInventoryItemCount++;
                playerPosition = move( '@', playerPosition, l_offset );
            }
            break;

        case 'C':
            if ( playerInventoryItemCount < MAX_PLAYER_ITEM_COUNT ) {
                // PlaySoundA( "SystemExit", NULL, SND_SYNC );
                const ITEMS l_chestItems[ CHEST_ITEM_COUNT ] = { HEALTH, ATTACK, DEFENCE };
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

bool DoOpponentMove() noexcept {
    // Labmdas
    auto l_watchForPlayer = [ & ]() {
        for ( unsigned int l_visionCell = 0; l_visionCell < OVERLOOK_RADIUS; l_visionCell++ )
            if ( vision[ l_visionCell ] == '@' )
                return (true);

        return (false);
    };

    auto l_randomMove = [ & ]( const char who, unsigned int& currentPosition ) {
        unsigned int l_offset;

        switch ( (MOVEMENT_DIRECTIONS)Rand() % 4 ) {
                case DOWN:
                    l_offset = 80;
                    break;

                case UP:
                    l_offset = -80;
                    break;

                case LEFT:
                    l_offset = -1;
                    break;

                case RIGHT:
                    l_offset = 1;
                    break;
            };

            if ( ( g_map[ currentPosition + l_offset ] == '.' ) || ( g_map[ currentPosition + l_offset ] == '#' ) )
                currentPosition = move( who, currentPosition, l_offset );

            else if ( g_map[ currentPosition + l_offset ] == '@' )
                currentPosition = fightMonster( who, currentPosition, l_offset );
    };

    auto l_followMove = [ & ]( const char who, unsigned int& currentPosition ) {
        int l_offset;

        if ( ( currentPosition % 80 ) < ( playerPosition % 80 ) ) {
            l_offset = 1;

        } else if ( ( currentPosition % 80 ) > ( playerPosition % 80 ) ) {
            l_offset = -1;

        } else if ( currentPosition < playerPosition ) {
            l_offset = 80;

        } else {
            l_offset = -80;
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
    };

    for ( unsigned int i = 0; i < randomMonstersLeft; i++ ) {
        l_randomMove( 'R', g_randomMonstersPositions[ i ] );
    }

    for ( unsigned int i = 0; i < followMonstersLeft; i++ ) {
        if ( l_watchForPlayer() ) {
            l_followMove( 'F', g_followMonstersPositions[ i ] );

        } else {
            l_randomMove( 'F', g_followMonstersPositions[ i ] );
        }
    }

    for ( unsigned int i = 0; i < guardiansLeft; i++ )
        if ( l_watchForPlayer() ) {
            l_followMove( 'G', g_guardiansPositions[ i ] );

        } else {
            l_randomMove( 'G', g_guardiansPositions[ i ] );
        }

    return (true);
}

bool usePlayerItem( const ITEMS l_item ) {
    int l_itemPlace = getPlayerInventoryPlaceOf( l_item );

    if ( l_itemPlace + 1 ) {
        playerInventory[ l_itemPlace ] = EMPTY;
        playerInventoryItemCount--;

        return (true);
    }

    return (false);
}

void UpdateScreen() noexcept {
    // lambdas
    auto l_lengthOfInt = []( int number ) {
        unsigned int counter = 0;

        do {
            number /= 10;
            counter++;
        } while ( number != 0 );

        return ( counter );
    };

    clearConsole();
    getOverview( playerPosition );
    print( vision );

    printChar( '\n' );
    print( "HP:" );
    char* l_buffer = (char*)Malloc( l_lengthOfInt( playerHealth ) );
    Ltoa( playerHealth, l_buffer );
    print( l_buffer, l_lengthOfInt( playerHealth ) );
    Free( l_buffer );
    printChar( '\n' );
    print( "EXP:" );
    l_buffer = (char*)Malloc( l_lengthOfInt( playerExperience ) );
    Ltoa( playerExperience, l_buffer );
    print( l_buffer, l_lengthOfInt( playerExperience ) );
    Free( l_buffer );

    printChar( '\n' );
    print( "ITEMS:" );

    l_buffer = (char*)Malloc( MAX_PLAYER_ITEM_COUNT * 2 );

    for ( unsigned int l_item = 0; l_item < MAX_PLAYER_ITEM_COUNT; l_item++ ) {
        if ( playerInventory[ l_item ] ) {
            printChar( playerInventory[ l_item ] );
            printChar( ' ' );
        }
    }

    Free( l_buffer );
}