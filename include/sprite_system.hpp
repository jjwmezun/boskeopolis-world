#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include "dir.hpp"
#include "rect.hpp"
#include "renderer.hpp"

namespace BSW
{
    class Game;
    class Inventory;
    class Map;

    enum class SpriteType
    {
        AUTUMN,
        CRAB,
        BADAPPLE,
        TRUCK,
        SCALE_LIFT,
        PUFFERBEE,
        POLLO
    };

    enum class SpriteTag
    {
        PROTAG,
        ENEMY,
        BOPPABLE
    };

    enum class LivingState
    {
        ALIVE,
        DYING,
        DEAD
    };

    struct Collision
    {
        float top;
        float bottom;
        float left;
        float right;
    };

    struct Sprite
    {
        SpriteGraphic graphic_;
        Rect pos_;
        Collision collision_;
        float startx_;
        float starty_;
        float accx_;
        float vx_;
        float prevx_;
        float accy_;
        float vy_;
        float prevy_;
        float start_speed_;
        float max_speed_;
        float max_jump_;
        float animation_timer_;
        Dir::X dir_x_;
        Dir::Y dir_y_;
        bool is_moving_;
        bool is_jumping_;
        bool on_ground_;
        union
        {
            struct
            {
                float jump_padding;
                float invincibility_;
                uint_fast8_t walk_frame;
                bool jump_lock;
            } autumn;
        } misc_;

        void init( Game & game );
        void update( Game & game, Map & map, Inventory & inventory, float dt );
        void goLeft();
        void goRight();
        void updatePositionGraphics( float xoffset = 0.0f, float yoffset = 0.0f );
        bool isAutumnGoingFast() const;
        void autumnLanding();
        bool updateAnimationTimer( float dt, float time = 8.0f );
        void rotateOnDirectionChange( float dt );
    };

    struct SpriteSystem
    {
        Sprite hero_;

        void init( Game & game, Map & map, float x, float y );
        void update( Game & game, Map & map, Inventory & inventory, float dt );
    };
};

#endif // SPRITE_SYSTEM_H