#pragma once

#include <cstdint>

namespace tmxbin {

enum class Orientation : std::uint8_t
{
    Orthogonal = 0,
    Isometric
};

enum class RenderOrder : std::uint8_t
{
    RightUp = 0,
    RightDown,
    LeftUp,
    LeftDown
};

}
