#ifndef RECT_H
#define RECT_H

struct Rect
{
    float x;
    float y;
    float w;
    float h;

    constexpr float right()
    {
        return x + w;
    };

    constexpr float bottom()
    {
        return y + h;
    }

    constexpr float centerX()
    {
        return x + ( w / 2.0 );
    }

    constexpr float centerY()
    {
        return y + ( h / 2.0 );
    }
};

#endif // RECT_H