#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <vector>

namespace Input
{
    enum class Key
    {
        CONFIRM,
        RIGHT,
        LEFT
    };
    static constexpr int NUMBER_OF_KEYS = 2;
    void init();
    void initKeys( std::unordered_map<Key, std::vector<int>> new_keys );
    void press( int raw_key );
    void release( int raw_key );
    void update();
    bool heldRight();
    bool heldLeft();
    bool pressedConfirm();
    bool pressedRight();
    bool pressedLeft();
};

#endif // INPUT_H