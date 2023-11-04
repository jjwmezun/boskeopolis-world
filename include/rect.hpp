#ifndef RECT_H
#define RECT_H

namespace BSW
{
    struct Rect
    {
        float x;
        float y;
        float w;
        float h;

        constexpr float right() const { return x + w; };
        constexpr float bottom() const { return y + h; };
        constexpr float centerX() const { return x + w / 2.0f; };
        constexpr float centerY() const { return y + h / 2.0f; };
        constexpr float halfWidth() const { return w / 2.0f; };
        constexpr float halfHeight() const { return h / 2.0f; };
        constexpr bool testCollision( const Rect & o ) const
        {
            return o.right() > x && o.x < right() && o.bottom() > y && o.y < bottom();
        };
    };
}

#endif // RECT_H