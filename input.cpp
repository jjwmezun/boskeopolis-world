#include "input.hpp"
#include "nasr_input.h"

namespace BSW
{
    void Input::init()
    {
        NasrInput inputs[] =
        {
            { static_cast<unsigned int>( Keys::UP ), NASR_KEY_UP },
            { static_cast<unsigned int>( Keys::RIGHT ), NASR_KEY_RIGHT },
            { static_cast<unsigned int>( Keys::DOWN ), NASR_KEY_DOWN },
            { static_cast<unsigned int>( Keys::LEFT ), NASR_KEY_LEFT },
            { static_cast<unsigned int>( Keys::JUMP ), NASR_KEY_Z },
            { static_cast<unsigned int>( Keys::RUN ), NASR_KEY_X },
            { static_cast<unsigned int>( Keys::MENU ), NASR_KEY_C }
        };
        NasrRegisterInputs( inputs, 7 );
    };

    bool Input::heldUp()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::UP ) );
    };

    bool Input::heldRight()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::RIGHT ) );
    };

    bool Input::heldDown()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::DOWN ) );
    };

    bool Input::heldLeft()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::LEFT ) );
    };

    bool Input::heldJump()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::JUMP ) );
    };

    bool Input::heldRun()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::RUN ) );
    };

    bool Input::heldMenu()
    {
        return NasrHeld( static_cast<unsigned int>( Keys::MENU ) );
    };

    bool Input::pressedUp()
    {
        return NasrPressed( static_cast<unsigned int>( Keys::UP ) );
    };

    bool Input::pressedRight()
    {
        return NasrPressed( static_cast<unsigned int>( Keys::RIGHT ) );
    };

    bool Input::pressedDown()
    {
        return NasrPressed( static_cast<unsigned int>( Keys::DOWN ) );
    };

    bool Input::pressedLeft()
    {
        return NasrPressed( static_cast<unsigned int>( Keys::LEFT ) );
    };

    bool Input::pressedJump()
    {
        return NasrPressed( static_cast<unsigned int>( Keys::JUMP ) );
    };

    bool Input::pressedRun()
    {
        return NasrPressed( static_cast<unsigned int>( Keys::RUN ) );
    };

    bool Input::pressedMenu()
    {
        return NasrPressed( static_cast<unsigned int>( Keys::MENU ) );
    };
}