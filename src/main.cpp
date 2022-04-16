#include "main.hpp"
#include "stdfunc.hpp"

int main( void ) {
    initMap();

    #ifdef EMPTY_PLAYER_INVENTORY
    initInventory( (item_t)EMPTY );
    #else
    initInventory( (ITEMS)NULL );
    #endif

    playerInventory[ 2 ] = HEALTH;
    playerInventoryItemCount++;

    // First render
    UpdateScreen();

    // Game loop
    for ( ;; ) {
        // Input handling
        if ( GetAsyncKeyState( VK_NUMPAD5 ) ) {
            if ( !DoOpponentMove() ) {
                break;
            }

            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD1 ) ) {
            if ( !DoPlayerMove( 79 ) || !DoOpponentMove() ) {
                break;
            }

            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD2 ) ) {
            if ( !DoPlayerMove( 80 ) || !DoOpponentMove() ) {
                break;
            }

            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD3 ) ) {
            if ( !DoPlayerMove( 81 ) || !DoOpponentMove() ) {
                break;
            }

            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD4 ) ) {
            if ( !DoPlayerMove( -1 ) || !DoOpponentMove() ) {
                break;
            }

            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD6 ) ) {
            if ( !DoPlayerMove( 1 ) || !DoOpponentMove() ) {
                break;
            }

            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD7 ) ) {
            if ( !DoPlayerMove( -81 ) || !DoOpponentMove() ) {
                break;
            }

            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD8 ) ) {
            if ( !DoPlayerMove( -80 ) || !DoOpponentMove() ) {
                break;
            }

            UpdateScreen();

        } else if ( GetAsyncKeyState( VK_NUMPAD9 ) ) {
            if ( !DoPlayerMove( -79 ) || !DoOpponentMove() ) {
                break;
            }
            
            UpdateScreen();
        }

        #ifdef _WIN32
        Sleep( 200 );
        #endif
    }

    // Level passed
    checkLose();

    return (0);
}