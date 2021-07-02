#pragma once

#include <cstdint>

namespace OneBit
{
struct Vertex final
{
    float x, y, z;
    float u, v;
    uint8_t brightness;
};
} // namespace OneBit