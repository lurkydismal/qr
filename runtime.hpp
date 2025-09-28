#pragma once

#include <array>
#include <cstdint>

#include "io.hpp"
#include "logic.hpp"
#include "stdfunc.hpp"
#include "system.hpp"

// #define RENDER_DEBUG_INFORMATION

namespace runtime {

using result_t = enum class result : int8_t { remain = -1, success, failure };

using move_t = enum class move : uint8_t {
    stay = 's',
    up = 'w',
    upRight = 'e',
    right = 'd',
    downRight = 'c',
    down = 'x',
    downLeft = 'z',
    left = 'a',
    upLeft = 'q'
};

FORCE_INLINE auto waitMove() -> move_t {
    std::array< uint8_t, 1 > l_event;

    io::read( l_event );

    return ( static_cast< move_t >( l_event[ 0 ] ) );
}

static io::terminal::termios g_terminalAttributesBeforeLaunch;

// Called once per application run
FORCE_INLINE auto init() -> result_t {
    g_terminalAttributesBeforeLaunch = io::terminal::disableCanonicalMode();

    io::terminal::hideCursor();

    logic::map::init();

    return ( result_t::remain );
}

FORCE_INLINE auto quit( result_t _exitCode ) -> result_t {
    tcsetattr( g_terminalAttributesBeforeLaunch );

    io::terminal::showCursor();

    system::exit( ( int )_exitCode );
}

// Called each frame
FORCE_INLINE constexpr auto event( move_t _event ) -> result_t {
    result_t l_returnValue = result_t::remain;

    logic::map::direction_t l_direction;

    switch ( _event ) {
        case ( move_t::up ): {
            l_direction = logic::map::direction_t::UP;

            break;
        }

        case ( move_t::upRight ): {
            l_direction = logic::map::direction_t::UP_RIGHT;

            break;
        }

        case ( move_t::right ): {
            l_direction = logic::map::direction_t::RIGHT;

            break;
        }

        case ( move_t::downRight ): {
            l_direction = logic::map::direction_t::DOWN_RIGHT;

            break;
        }

        case ( move_t::down ): {
            l_direction = logic::map::direction_t::DOWN;

            break;
        }

        case ( move_t::downLeft ): {
            l_direction = logic::map::direction_t::DOWN_LEFT;

            break;
        }

        case ( move_t::left ): {
            l_direction = logic::map::direction_t::LEFT;

            break;
        }

        case ( move_t::upLeft ): {
            l_direction = logic::map::direction_t::UP_LEFT;

            break;
        }

        default: {
            l_direction = logic::map::direction_t::STAY;
        }
    }

    logic::map::move$player( l_direction );

    if ( logic::player::lose::get() ) [[unlikely]] {
        l_returnValue = result_t::failure;
    }

    return ( l_returnValue );
}

FORCE_INLINE constexpr auto iterate() -> result_t {
    result_t l_returnValue = result_t::remain;

    // Logic
    {
        logic::map::move$ai();
    }

    if ( logic::player::lose::get() ) {
        l_returnValue = result_t::failure;
    }

    // Render
    if !consteval {
        io::clearScreen();

        logic::map::render();

        // HUD
        {
            logic::player::render$stats();

#if defined( RENDER_DEBUG_INFORMATION )
            player$render$debug();
#endif
        }
    }

    return ( l_returnValue );
}

} // namespace runtime
