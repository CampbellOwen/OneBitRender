#include "OneBitRender/rasterizer.h"

#include "OneBitRender/constants.h"

#include <iostream>
#include <limits>

namespace OneBit
{

static bool OutOf16(int32_t x)
{
    const auto min16 = std::numeric_limits<int16_t>::min();
    const auto max16 = std::numeric_limits<int16_t>::max();

    return (x < 0 && x <= min16) || (x > 0 && x >= max16);
}

static int32_t clamp(int32_t x, int32_t min, int32_t max)
{
    if (x < min)
    {
        x = min;
    }
    if (x > max)
    {
        x = max;
    }

    return x;
}

static int32_t clamp16(int32_t x)
{
    static const auto min16 = std::numeric_limits<int16_t>::min();
    static const auto max16 = std::numeric_limits<int16_t>::max();
    return clamp(x, min16, max16);
}

static int64_t orient2d(const Point2D& a, const Point2D& b, const Point2D& c) noexcept
{
    // Computes 2D determinant

    auto x1 = (b.x - a.x);

    auto y1 = (c.y - a.y);

    auto x2 = (c.x - a.x);
    auto y2 = (b.y - a.y);

    auto mul1 = (x1 * y1);
    auto mul2 = (y2 * x2);

    return mul1 - mul2;

    // return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static int64_t orient2d(const Edge& edge, const Point2D& p) noexcept
{
    return orient2d(edge.a, edge.b, p);
}

static int32_t min3(int32_t a, int32_t b, int32_t c)
{
    int32_t min = a;
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

static int32_t max3(int32_t a, int32_t b, int32_t c)
{
    int32_t max = a;
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

// Very much inspired by/taken from Fabian Giesen's amazing series
// https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/
void Rasterizer::Rasterize(Vertex v0, Vertex v1, Vertex v2) noexcept
{
    // Snap to integer grid
    // TODO: Support subpixel precision by making this fixed point.
    // TODO: Support proper edge fill rules

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

    // Add 8 bits of subpixel precision
    static const int subPixelBits = 8;
    static const int subStep = 1 << subPixelBits;
    static const int subMask = subStep - 1;

    p0.x *= subStep;
    p0.y *= subStep;

    p1.x *= subStep;
    p1.y *= subStep;

    p2.x *= subStep;
    p2.y *= subStep;

    // Round to nearest integer multiple of subStep
    p0.x = (p0.x + subMask) & ~subMask;
    p0.y = (p0.y + subMask) & ~subMask;

    p1.x = (p1.x + subMask) & ~subMask;
    p1.y = (p1.y + subMask) & ~subMask;

    p2.x = (p2.x + subMask) & ~subMask;
    p2.y = (p2.y + subMask) & ~subMask;

    // Triangle Setup
    int64_t A01 = p0.y - p1.y, B01 = p1.x - p0.x;
    int64_t A12 = p1.y - p2.y, B12 = p2.x - p1.x;
    int64_t A20 = p2.y - p0.y, B20 = p0.x - p2.x;

    Edge e01{p0, p1};
    Edge e12{p1, p2};
    Edge e20{p2, p0};

    int64_t twoTriangleArea = orient2d(p0, p1, p2);
    int64_t twoTriangleBright0 = (twoTriangleArea * v0.brightness);

    int32_t twoTriangleArea32 = twoTriangleArea >> subPixelBits;
    int32_t twoTriangleBright032 = twoTriangleBright0 >> subPixelBits;

    int64_t brightness1 = v1.brightness - v0.brightness;
    int64_t brightness2 = v2.brightness - v0.brightness;

    // Barycentric coordinates at minX/minY
    Point2D p = pMin;
    int64_t w0_row = orient2d(e12, p);
    int64_t w1_row = orient2d(e20, p);
    int64_t w2_row = orient2d(e01, p);

    // subMask bits stay constant, safe to shift them out
    w0_row = w0_row >> subPixelBits;
    w1_row = w1_row >> subPixelBits;
    w2_row = w2_row >> subPixelBits;

    for (p.y = pMin.y; p.y <= pMax.y; p.y++)
    {

        // Barycentric coordinates at start of row
        int32_t w0 = w0_row;
        int32_t w1 = w1_row;
        int32_t w2 = w2_row;

        for (p.x = pMin.x; p.x <= pMax.x; p.x++)
        {
            if ((w0 | w1 | w2) >= 0)
            {

                auto brightness = (twoTriangleBright032) + (w1 * brightness1) + (w2 * brightness2);
                m_backbuffer[(p.y) * RENDER_WIDTH + (p.x)] = static_cast<uint8_t>((brightness / (twoTriangleArea32)));
            }

            // One step to the right
            w0 += A12;
            w1 += A20;
            w2 += A01;
        }

        // One row step
        w0_row += B12;
        w1_row += B20;
        w2_row += B01;
    }
}

} // namespace OneBit