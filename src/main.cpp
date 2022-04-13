#include "main.hpp"
#include "stdfunc.hpp" // Rand clear_screen
#include "printChar.h" // printChar

int main() {
    // Map parse
    initMap();

    initInventory();

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
    checkLose();

    return (0);
}