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
    for (int i = 0; i < vertices.size() / 3; i += 3)
    {
        m_rasterizer.Rasterize(vertices[i], vertices[i + 1], vertices[i + 2]);
    }
}

} // namespace OneBit