#include "main.h"
#include "stdfunc.h"

int main( int argumentCount, char** argumentArray ) {
    bool l_gameRunning = true;

    initMap();

    initInventory( (enum item_t)EMPTY );

    g_playerInventory[ 2 ] = HEALTH;
    g_playerInventoryItemCount++;

    const uint8_t l_keyScancodes[ 9 ] = {
        VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9
    };

    // First render
    updateScreen();

    // Game loop
    while ( l_gameRunning ) {
        // Input handling
        for ( uint8_t _keyScancodeIndex = 0; _keyScancodeIndex < sizeof( l_keyScancodes ); _keyScancodeIndex++ ) {
            if ( GetAsyncKeyState( l_keyScancodes[ _keyScancodeIndex ] ) ) {
                enum direction_t l_offset;

                switch ( l_keyScancodes[ _keyScancodeIndex ] ) {
                    case VK_NUMPAD8:
                    {
                        l_offset = UP;

                        break;
                    }

                    case VK_NUMPAD9:
                    {
                        l_offset = UP_RIGHT;

                        break;
                    }

                    case VK_NUMPAD6:
                    {
                        l_offset = RIGHT;

                        break;
                    }

                    case VK_NUMPAD3:
                    {
                        l_offset = DOWN_RIGHT;

                        break;
                    }

                    case VK_NUMPAD2:
                    {
                        l_offset = DOWN;

                        break;
                    }

                    case VK_NUMPAD1:
                    {
                        l_offset = DOWN_LEFT;

                        break;
                    }

                    case VK_NUMPAD4:
                    {
                        l_offset = LEFT;

                        break;
                    }

                    case VK_NUMPAD7:
                    {
                        l_offset = UP_LEFT;

                        break;
                    }

                    default:
                    {
                        l_offset = STAY;

                        break;
                    }
                }

                if ( !doPlayerMove( l_offset ) || !doOpponentMove() ) {
                    l_gameRunning = false;
                }

                updateScreen();
            }
        }

        #ifdef _WIN32
        Sleep( 200 );
        #else
        sleep( 0.2 );
        #endif
    }

    // Level passed
    if ( g_playerHealth ) {
        #ifdef _WIN32
        MessageBoxA( 0, "You Win!", "Victory!", 0 );
        #else
        print( "You Win!", 9 );
        #endif

    } else {
        #ifdef _WIN32
        MessageBoxA( 0, "You Lose!", "Defeat!", 0 );
        #else
        print( "You Lose!", 10 );
        #endif
    }

    return (0);
}