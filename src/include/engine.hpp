#ifndef ENGINE_H
#define ENGINE_H

namespace Engine
{
    bool init();
    void close();
    int getTicks();
    bool handleEvents();
}

#endif // ENGINE_H