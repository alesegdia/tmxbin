#pragma once

#include <cstdint>


namespace tmxbin {


enum class RenderOrder : std::uint8_t
{
    RightUp = 0,
    RightDown,
    LeftUp,
    LeftDown
};


}
