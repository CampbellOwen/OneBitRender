#pragma once

#include "OneBitRender/constants.h"
#include "OneBitRender/rasterizer.h"
#include "OneBitRender/shading.h"

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
    void DrawLine(const Edge& edge) noexcept
    {
        m_rasterizer.DrawLine(edge);
    };

    std::vector<uint8_t>& GetBackbuffer() noexcept
    {
        return m_backbuffer;
    }

    void SetShadingType(ShadingType type) noexcept
    {
        m_rasterizer.SetShadingType(type);
    }

    void SetLineColour(LineColour colour) noexcept
    {
        switch (colour)
        {
        case LineColour::BLACK:
            m_rasterizer.SetLineColour(true);
            break;
        case LineColour::WHITE:
            m_rasterizer.SetLineColour(false);
            break;
        }
    }

  private:
    std::vector<uint8_t> m_backbuffer;
    Rasterizer m_rasterizer;
};

} // namespace OneBit