#pragma once

#include <cstddef>
#include <cstdint>

#include "stdfunc.hpp"

namespace random {

[[nodiscard]] constexpr auto randomNumber() -> size_t {
#define COMPILATION_TIME_AS_SEED                                      \
    ( __TIME__[ 0 ] + __TIME__[ 1 ] + __TIME__[ 2 ] + __TIME__[ 3 ] + \
      __TIME__[ 4 ] + __TIME__[ 5 ] )

    static uint32_t l_seed = COMPILATION_TIME_AS_SEED;

    l_seed ^= ( l_seed << 13 );
    l_seed ^= ( l_seed >> 17 );
    l_seed ^= ( l_seed << 5 );

    return ( l_seed );

#undef COMPILATION_TIME_AS_SEED
}

#if 0
template < typename T, size_t N >
FORCE_INLINE constexpr auto randomValueFromContainer(
    const std::array< const T, N >& _container ) -> T {
    return ( _container[ randomNumber() % _container.size() ] );
}
#endif

} // namespace random
