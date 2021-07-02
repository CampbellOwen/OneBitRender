#include "OneBitRender/rasterizer.h"

#include "OneBitRender/constants.h"

namespace OneBit
{

static struct Point2D
{
    uint32_t x, y;
};

static int32_t orient2d(const Point2D& a, const Point2D& b, const Point2D& c)
{
    // Computes 2D determinant
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static uint32_t min3(uint32_t a, uint32_t b, uint32_t c)
{
    uint32_t min = a;
    if (b < min)
    {
        min = b;
    }
    if (c < min)
    {
        min = c;
    }
    return min;
}

static uint32_t max3(uint32_t a, uint32_t b, uint32_t c)
{
    uint32_t max = a;
    if (b > max)
    {
        max = b;
    }
    if (c > max)
    {
        max = c;
    }
    return max;
}

void Rasterizer::Rasterize(Vertex v0, Vertex v1, Vertex v2)
{
    // Snap to integer grid
    // TODO: Support subpixel precision by making this fixed point.
    Point2D p0{v0.x * RENDER_WIDTH, v0.y * RENDER_HEIGHT};
    Point2D p1{v1.x * RENDER_WIDTH, v1.y * RENDER_HEIGHT};
    Point2D p2{v2.x * RENDER_WIDTH, v2.y * RENDER_HEIGHT};

    // Bounding box of triangle
    Point2D pMin{min3(p0.x, p1.x, p2.x), min3(p0.y, p1.y, p2.y)};
    Point2D pMax{max3(p0.x, p1.x, p2.x), max3(p0.y, p1.y, p2.y)};

    // Clip bounding box on screen
    if (pMax.x > RENDER_WIDTH - 1)
    {
        pMax.x = RENDER_WIDTH - 1;
    }

    if (pMax.y > RENDER_HEIGHT - 1)
    {
        pMax.y = RENDER_HEIGHT - 1;
    }

    if (pMin.x < 0)
    {
        pMin.x = 0;
    }

    if (pMin.y < 0)
    {
        pMin.y = 0;
    }

    Point2D p{0, 0};
    for (p.y = pMin.y; p.y <= pMax.y; p.y++)
    {
        for (p.x = pMin.x; p.x <= pMax.x; p.x++)
        {
            int32_t w0 = orient2d(p0, p1, p);
            int32_t w1 = orient2d(p1, p2, p);
            int32_t w2 = orient2d(p2, p0, p);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                m_backbuffer[p.y * RENDER_WIDTH + p.x] = 255;
            }
        }
    }
}

} // namespace OneBit