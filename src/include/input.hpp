#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <vector>

namespace Input
{
    enum class Key
    {
        CONFIRM,
        MENU,
        JUMP,
        UP,
        RIGHT,
        DOWN,
        LEFT
    };
    static constexpr int NUMBER_OF_KEYS = 7;
    void init();
    void initKeys( std::unordered_map<Key, std::vector<int>> new_keys );
    void press( int raw_key );
    void release( int raw_key );
    void update();
    bool heldJump();
    bool heldUp();
    bool heldRight();
    bool heldDown();
    bool heldLeft();
    bool pressedConfirm();
    bool pressedMenu();
    bool pressedUp();
    bool pressedRight();
    bool pressedLeft();
};

#endif // INPUT_H