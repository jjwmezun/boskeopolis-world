#ifndef ENGINE_H
#define ENGINE_H

namespace Engine
{
    int init();
    void close();
    int getTicks();
    int handleEvents();
}

#endif // ENGINE_H