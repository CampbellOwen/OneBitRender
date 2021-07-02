#include <algorithm>
#include <cstdint>

#include "OneBitRender/renderer.h"

namespace OneBit
{
void Renderer::Clear(uint8_t colour) noexcept
{
    std::fill(m_backbuffer.begin(), m_backbuffer.end(), colour);
}

} // namespace OneBit