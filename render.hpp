#pragma once

#include <cstddef>
#include <cstdint>

#include "stdfunc.hpp"

namespace render {

// Stats
FORCE_INLINE void points( char** _cursorPosition,
                          uint8_t _points,
                          size_t _textLength ) {
    const size_t l_lengthOfPoints = stdfunc::lengthOfNumber( _points );

    *_cursorPosition += ( _textLength - l_lengthOfPoints );

    stdfunc::convertNumberToString( *_cursorPosition, _points,
                                    l_lengthOfPoints );

    *_cursorPosition += ( l_lengthOfPoints - 1 );
}

} // namespace render
