#pragma once

#include <array>
#include <cstdint>

#include "io.hpp"
#include "logic.hpp"
#include "stdfunc.hpp"
#include "system.hpp"

// #define RENDER_DEBUG_INFORMATION

namespace runtime {

using result_t = enum class result : int8_t {
    remain = -1,
    success,
    failure,
};

using move_t = enum class move : uint8_t {
    stay = 's',
    up = 'w',
    upRight = 'e',
    right = 'd',
    downRight = 'c',
    down = 'x',
    downLeft = 'z',
    left = 'a',
    upLeft = 'q',
};

[[nodiscard]] FORCE_INLINE auto waitMove() -> move_t {
    std::array< uint8_t, 1 > l_event;

    io::read( l_event );

    return ( static_cast< move_t >( l_event[ 0 ] ) );
}

static io::terminal::termios g_terminalAttributesBeforeLaunch{};

// Called once per application run
[[nodiscard]] FORCE_INLINE auto init() -> result_t {
    g_terminalAttributesBeforeLaunch = io::terminal::disableCanonicalMode();

    io::terminal::hideCursor();

    logic::map::init();

    return ( result_t::remain );
}

NO_RETURN FORCE_INLINE void quit( result_t _exitCode ) {
    tcsetattr( g_terminalAttributesBeforeLaunch );

    io::terminal::showCursor();

    system::exit( ( int )_exitCode );
}

// Called each frame
[[nodiscard]] FORCE_INLINE constexpr auto event( move_t _event ) -> result_t {
    result_t l_returnValue = result_t::remain;

    logic::map::direction_t l_direction;

    switch ( _event ) {
        case ( move_t::up ): {
            l_direction = logic::map::direction_t::up;

            break;
        }

        case ( move_t::upRight ): {
            l_direction = logic::map::direction_t::upRight;

            break;
        }

        case ( move_t::right ): {
            l_direction = logic::map::direction_t::right;

            break;
        }

        case ( move_t::downRight ): {
            l_direction = logic::map::direction_t::downRight;

            break;
        }

        case ( move_t::down ): {
            l_direction = logic::map::direction_t::down;

            break;
        }

        case ( move_t::downLeft ): {
            l_direction = logic::map::direction_t::downLeft;

            break;
        }

        case ( move_t::left ): {
            l_direction = logic::map::direction_t::left;

            break;
        }

        case ( move_t::upLeft ): {
            l_direction = logic::map::direction_t::upLeft;

            break;
        }

        default: {
            l_direction = logic::map::direction_t::stay;
        }
    }

    logic::player::move( l_direction );

    if ( logic::player::lose::get() ) [[unlikely]] {
        l_returnValue = result_t::failure;
    }

    return ( l_returnValue );
}

[[nodiscard]] FORCE_INLINE constexpr auto iterate() -> result_t {
    result_t l_returnValue = result_t::remain;

    // Logic
    {
        logic::map::ai();

        if ( logic::player::lose::get() ) {
            l_returnValue = result_t::failure;
        }
    }

    // Render
    if !consteval {
        io::terminal::clearScreen();

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
