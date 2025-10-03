#pragma once

#include <cstddef>
#include <cstdint>

#include "stdfunc.hpp"

namespace render {

template < std::input_iterator Iterator >
FORCE_INLINE constexpr void points( Iterator& _cursorPosition,
                                    uint8_t _points,
                                    size_t _textLength ) {
    const size_t l_lengthOfPoints = stdfunc::lengthOfNumber( _points );

    std::advance( _cursorPosition, ( _textLength - l_lengthOfPoints - 1 ) );

    stdfunc::convertNumberToString( _cursorPosition, _points,
                                    l_lengthOfPoints );

    std::advance( _cursorPosition, ( l_lengthOfPoints - 1 ) );
}

} // namespace render
