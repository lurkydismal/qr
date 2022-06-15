#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#else

#error Only Windows is supported

#endif

#include <main.h>
#include <stdfunc.h>

int main( int _argumentCount, char* _arguments[] ) {
    bool l_gameRunning = true;

#if defined( CLIENT ) || defined( SERVER )

    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup( MAKEWORD( 2,2 ), &wsaData );

    print( "1\n", 3 );

    struct addrinfo hints;

    ZeroMemory( &hints, sizeof( hints ) );

#if defined( CLIENT )

    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

#elif defined( SERVER )

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags    = AI_PASSIVE;

#endif // CLIENT || SERVER

    // Resolve the server address and port
    struct addrinfo* result;

#if defined( CLIENT )

    print( "2\n", 3 );

    getaddrinfo( _arguments[ 1 ], _arguments[ 2 ], &hints, &result );

#elif defined( SERVER )

    print( "3\n", 3 );

    getaddrinfo( NULL, _arguments[ 1 ], &hints, &result );

#endif // CLIENT || SERVER

    SOCKET ConnectSocket;

#if defined( CLIENT )

    // Attempt to connect to an address until one succeeds
    for (
        struct addrinfo* ptr = result;
        ptr != NULL;
        ptr = ptr->ai_next
    ) {
        print( "4\n", 3 );

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(
            ptr->ai_family,
            ptr->ai_socktype,
            ptr->ai_protocol
        );

        if ( ConnectSocket == INVALID_SOCKET ) {
            WSACleanup();
            return 1;
        }

        // Connect to server.
        if (
            connect(
                ConnectSocket,
                ptr->ai_addr,
                (int)ptr->ai_addrlen
            ) == SOCKET_ERROR
        ) {
            closesocket( ConnectSocket );

            ConnectSocket = INVALID_SOCKET;

            continue;
        }

        break;
    }

#elif defined( SERVER )

    print( "5\n", 3 );

    ConnectSocket = socket(
        result->ai_family,
        result->ai_socktype,
        result->ai_protocol
    );

    bind(
        ConnectSocket,
        result->ai_addr,
        (int)result->ai_addrlen
    );

#endif // CLIENT || SERVER

    print( "6\n", 3 );

    freeaddrinfo( result );

#if defined( SERVER )

    listen(
        ConnectSocket,
        SOMAXCONN
    );

    SOCKET CSocket = accept(
        ConnectSocket,
        NULL,
        NULL
    );

    print( "7\n", 3 );

    closesocket( ConnectSocket );

    ConnectSocket = CSocket;

#endif // SERVER

    char* buffer;

#endif // CLIENT || SERVER

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

            #if defined( CLIENT ) || defined( SERVER )

                buffer = (char*)Malloc( sizeof( l_offset ) );

                *buffer = l_offset;

                send( ConnectSocket, buffer, sizeof( l_offset ), 0 );

                l_gameRunning = doPlayerMove( l_offset, false );

                if ( recv( ConnectSocket, buffer, sizeof( l_offset ), 0 ) ) {
                    l_gameRunning = doPlayerMove( *buffer, true );
                }

                Free( buffer );

                l_gameRunning = doOpponentMove();

            #else

                l_gameRunning = (
                    doPlayerMove( l_offset, false ) &&
                    doOpponentMove()
                );

            #endif

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

#if defined( CLIENT ) || defined( SERVER )

    // Cleanup
    shutdown( 
        ConnectSocket,
        SD_SEND
    );
    closesocket( ConnectSocket );
    WSACleanup();

#endif

    return ( 0 );
}
