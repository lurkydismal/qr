#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#else // _WIN32

#error Only Windows is supported

#endif // _WIN32

#include <main.h>
#include <stdfunc.h>

int main( int _argumentCount, char* _arguments[] ) {
    bool l_gameRunning = true;

#if defined( CLIENT ) || defined( SERVER )

    WSADATA          l_wsaData;
    struct addrinfo  l_hints;
    struct addrinfo* l_result;
    SOCKET           l_connectSocket;
    char*            l_secondPlayerOffset;

#endif // CLIENT || SERVER

#if defined( CLIENT ) || defined( SERVER )

    WSAStartup( MAKEWORD( 2,2 ), &l_wsaData );

    ZeroMemory( &l_hints, sizeof( l_hints ) );

#if defined( CLIENT )

    l_hints.ai_family   = AF_UNSPEC;
    l_hints.ai_socktype = SOCK_STREAM;
    l_hints.ai_protocol = IPPROTO_TCP;

#elif defined( SERVER ) // CLIENT

    l_hints.ai_family   = AF_INET;
    l_hints.ai_socktype = SOCK_STREAM;
    l_hints.ai_protocol = IPPROTO_TCP;
    l_hints.ai_flags    = AI_PASSIVE;

#endif // CLIENT

#if defined( CLIENT )

    getaddrinfo(
        _arguments[ 1 ],
        _arguments[ 2 ],
        &l_hints,
        &l_result
    );

#elif defined( SERVER ) // CLIENT

    getaddrinfo(
        NULL,
        _arguments[ 1 ],
        &l_hints,
        &l_result
    );

#endif // CLIENT

#if defined( CLIENT )

    // Attempt to connect to an address until one succeeds
    for (
        struct addrinfo* _currentAddressInformation = l_result;
        _currentAddressInformation != NULL;
        _currentAddressInformation = _currentAddressInformation->ai_next
    ) {
        l_connectSocket = socket(
            _currentAddressInformation->ai_family,
            _currentAddressInformation->ai_socktype,
            _currentAddressInformation->ai_protocol
        );

        if ( l_connectSocket == INVALID_SOCKET ) {
            WSACleanup();

            return ( 1 );
        }

        // Connect to server
        if (
            connect(
                l_connectSocket,
                _currentAddressInformation->ai_addr,
                (int)_currentAddressInformation->ai_addrlen
            ) == SOCKET_ERROR
        ) {
            closesocket( l_connectSocket );

            l_connectSocket = INVALID_SOCKET;

            continue;
        }

        break;
    }

#elif defined( SERVER ) // CLIENT

    l_connectSocket = socket(
        l_result->ai_family,
        l_result->ai_socktype,
        l_result->ai_protocol
    );

    bind(
        l_connectSocket,
        l_result->ai_addr,
        (int)l_result->ai_addrlen
    );

#endif // CLIENT

    freeaddrinfo( l_result );

#if defined( SERVER )

    listen(
        l_connectSocket,
        SOMAXCONN
    );

    SOCKET l_buffer = accept(
        l_connectSocket,
        NULL,
        NULL
    );

    closesocket( l_connectSocket );

    l_connectSocket = l_buffer;

#endif // SERVER

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

                l_secondPlayerOffset = (char*)Malloc( sizeof( l_offset ) );

                *l_secondPlayerOffset = l_offset;

                send( l_connectSocket, l_secondPlayerOffset, sizeof( l_offset ), 0 );

                recv( l_connectSocket, l_secondPlayerOffset, sizeof( l_offset ), 0 );

                l_gameRunning = (
                    doPlayerMove( l_offset, false ) &&
                    doPlayerMove( *l_secondPlayerOffset, true ) &&
                    doOpponentMove()
                );

                Free( l_secondPlayerOffset );

            #else // CLIENT || SERVER

                l_gameRunning = (
                    doPlayerMove( l_offset, false ) &&
                    doOpponentMove()
                );

            #endif // CLIENT || SERVER

                updateScreen();
            }
        }

        Sleep( 100 );
    }

    // Level passed
    if ( g_playerHealth ) {
        MessageBoxA( 0, "You Win!", "Victory!", 0 );

    } else {
        MessageBoxA( 0, "You Lose!", "Defeat!", 0 );
    }

#if defined( CLIENT ) || defined( SERVER )

    // Cleanup
    shutdown( 
        l_connectSocket,
        SD_SEND
    );
    closesocket( l_connectSocket );
    WSACleanup();

#endif // CLIENT || SERVER

    return ( 0 );
}
