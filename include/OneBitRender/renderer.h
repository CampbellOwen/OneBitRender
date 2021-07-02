#pragma once

#include <vector>

namespace OneBit
{
constexpr const uint32_t RENDER_HEIGHT{240};
constexpr const uint32_t RENDER_WIDTH{400};

class Renderer final
{

  public:
    Renderer()
    {
        m_backbuffer.assign(RENDER_HEIGHT * RENDER_WIDTH, 0);
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