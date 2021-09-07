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
};

#endif // RECT_H