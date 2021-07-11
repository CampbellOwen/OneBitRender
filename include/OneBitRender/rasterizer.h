#pragma once

#include "OneBitRender/shading.h"
#include "OneBitRender/vertex.h"

#include <vector>

namespace OneBit
{

struct Point2D
{
    int32_t x, y;
    void Add(const Point2D& other) noexcept
    {
        x = x + other.x;
        y = y + other.y;
    }
};

struct Edge
{
    Edge(const Point2D& a, const Point2D& b) : a(a), b(b)
    {
    }

    Point2D a, b;
};

class Rasterizer
{
  public:
    Rasterizer(std::vector<uint8_t>& backbuffer) : m_backbuffer(backbuffer)
    {
    }

    void DrawLine(const Edge& edge) noexcept;
    void Rasterize(Vertex v0, Vertex v1, Vertex p2) noexcept;

    void SetShadingType(ShadingType type) noexcept
    {
        m_shadingType = type;
    }

    void SetLineColour(bool blackLines) noexcept
    {
        m_blackLines = blackLines;
    }

  private:
    std::vector<uint8_t>& m_backbuffer;
    ShadingType m_shadingType{ShadingType::FLAT};
    bool m_blackLines{true};
    bool m_drawRightEdge{true};
};

} // namespace OneBit