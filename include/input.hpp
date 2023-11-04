#ifndef INPUT_H
#define INPUT_H

namespace BSW
{
    class Input
    {
        public:
            static void init();
            static bool heldUp();
            static bool heldRight();
            static bool heldDown();
            static bool heldLeft();
            static bool heldJump();
            static bool heldRun();
            static bool heldMenu();

            static bool pressedUp();
            static bool pressedRight();
            static bool pressedDown();
            static bool pressedLeft();
            static bool pressedJump();
            static bool pressedRun();
            static bool pressedMenu();

            enum class Keys
            {
                UP,
                RIGHT,
                DOWN,
                LEFT,
                JUMP,
                RUN,
                MENU
            };
    };
}

#endif // INPUT_H