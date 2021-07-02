#pragma once

#include "vertex.h"

#include <vector>

namespace OneBit
{
class Rasterizer
{
  public:
    Rasterizer(std::vector<uint8_t>& backbuffer) : m_backbuffer(backbuffer)
    {
    }

    void Rasterize(Vertex v0, Vertex v1, Vertex p2);

  private:
    std::vector<uint8_t>& m_backbuffer;
};

} // namespace OneBit