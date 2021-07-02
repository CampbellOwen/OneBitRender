#include <algorithm>
#include <cstdint>

#include "OneBitRender/renderer.h"

namespace OneBit
{
void Renderer::Clear(uint8_t colour) noexcept
{
    std::fill(m_backbuffer.begin(), m_backbuffer.end(), colour);
}

void Renderer::Render(const std::vector<Vertex>& vertices) noexcept
{
    int i = 0;
    while (i < vertices.size())
    {
        m_rasterizer.Rasterize(vertices[i], vertices[i + 1], vertices[i + 2]);
        i += 3;
    }
}

} // namespace OneBit