#pragma once

#include <vector>

namespace OneBit
{
constexpr const uint32_t RENDER_HEIGHT{240};
constexpr const uint32_t RENDER_WIDTH{400};

class Renderer
{

  public:
    Renderer()
    {
        m_backbuffer.reserve(RENDER_HEIGHT * RENDER_HEIGHT);
    }

    void Clear(uint8_t colour) noexcept;

    std::vector<uint8_t>& GetBackbuffer() noexcept
    {
        return m_backbuffer;
    }

  private:
    std::vector<uint8_t> m_backbuffer;
};

} // namespace OneBit