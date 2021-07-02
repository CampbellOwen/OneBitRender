#pragma once

#include "OneBitRender/constants.h"
#include "OneBitRender/rasterizer.h"

#include <vector>

namespace OneBit
{

class Renderer final
{

  public:
    Renderer() : m_rasterizer(m_backbuffer)
    {
        m_backbuffer.assign(RENDER_HEIGHT * RENDER_WIDTH, 0);
    }

    void Clear(uint8_t colour) noexcept;

    void Render(const std::vector<Vertex>& vertices) noexcept;

    std::vector<uint8_t>& GetBackbuffer() noexcept
    {
        return m_backbuffer;
    }

  private:
    std::vector<uint8_t> m_backbuffer;
    Rasterizer m_rasterizer;
};

} // namespace OneBit