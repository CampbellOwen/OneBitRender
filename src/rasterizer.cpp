#include "OneBitRender/rasterizer.h"

#include "OneBitRender/constants.h"

namespace OneBit
{

static int32_t orient2d(const Point2D& a, const Point2D& b, const Point2D& c) noexcept
{
    // Computes 2D determinant
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static int32_t orient2d(const Edge& edge, const Point2D& p) noexcept
{
    return orient2d(edge.a, edge.b, p);
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

template <class T> static void swap(T* a, T* b)
{
    T temp = *a;
    *a = *b;
    *b = temp;
}

void Rasterizer::DrawLine(const Edge& edge) noexcept
{
    // Bresenham's algorithm

    //// Always want x to increase
    // Point2D left, right;
    // if (edge.a.x < edge.b.x)
    //{
    //    left = edge.a;
    //    right = edge.b;
    //}
    // else
    //{
    //    left = edge.b;
    //    right = edge.a;
    //}

    // if (left.y == right.y)
    //{
    //    for (int x = left.x; x <= right.x; x++)
    //    {
    //        m_backbuffer[left.y * RENDER_WIDTH + x] = m_blackLines ? 0 : 255;
    //    }
    //}

    int32_t dx = edge.b.x - edge.a.x;
    int32_t dy = edge.b.y - edge.a.y;

    int32_t dLong = abs(dx);
    int32_t dShort = abs(dy);

    int32_t offsetLong = dx > 0 ? 1 : -1;
    int32_t offsetShort = -RENDER_WIDTH;
    if (dy > 0)
    {
        offsetShort = RENDER_WIDTH;
    }

    if (dLong < dShort)
    {
        swap(&dShort, &dLong);
        swap(&offsetShort, &offsetLong);
    }

    int32_t error = dLong / 2;
    int32_t index = edge.a.y * RENDER_WIDTH + edge.a.x;
    const int offset[] = {offsetLong, offsetLong + offsetShort};
    const int abs_d[] = {dShort, dShort - dLong};
    for (int i = 0; i <= dLong; ++i)
    {
        m_backbuffer[index] = 0; // or a call to your painting method
        const int errorIsTooBig = error >= dLong;
        index += offset[errorIsTooBig];
        error += abs_d[errorIsTooBig];
    }
}

void Rasterizer::Rasterize(Vertex v0, Vertex v1, Vertex v2) noexcept
{
    // Snap to integer grid
    // TODO: Support subpixel precision by making this fixed point.
    Point2D p0{v0.x * RENDER_WIDTH, v0.y * RENDER_HEIGHT};
    Point2D p1{v1.x * RENDER_WIDTH, v1.y * RENDER_HEIGHT};
    Point2D p2{v2.x * RENDER_WIDTH, v2.y * RENDER_HEIGHT};

    // Special case: just draw lines between vertices
    if (m_shadingType == ShadingType::WIREFRAME)
    {
        if (m_drawRightEdge)
        {
            DrawLine({p0, p1});
            DrawLine({p1, p2});
            DrawLine({p2, p0});
        }

        return;
    }

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
                m_backbuffer[p.y * RENDER_WIDTH + p.x] = 0;
            }
        }
    }
}

} // namespace OneBit