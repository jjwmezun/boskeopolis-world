#include "sprite_system.hpp"
#include "game.hpp"
#include "input.hpp"
#include "renderer.hpp"

namespace BSW
{
    static constexpr float START_SPEED = 0.15f;
    static constexpr float MAX_SPEED = 2.0f;
    static constexpr float FALL_SPEED = 0.5f;
    static constexpr float GRAVITY = 6.0f;
    static constexpr float JUMP_ACC = 0.2f;
    static constexpr float JUMP_INIT = 2.0f;
    static constexpr float JUMP_MAX = 4.75f;
    static constexpr float autumn_walk_frames[ 4 ] = { 0.0f, 16.0f, 0.0f, 32.0f };

    void Sprite::init( Game & game )
    {
        graphic_ = game.render().addSprite( "sprites/autumn.png", 0.0f, 0.0f, pos_.w, pos_.h, pos_.x, pos_.y, { { "palette", 128 } } );
    };

    void Sprite::update( Game & game, Map & map, float dt )
    {
        // Horizontal Movement.
        start_speed_ = Input::heldRun() ? START_SPEED * 2.0f : START_SPEED;
        max_speed_ = Input::heldRun() ? MAX_SPEED * 2.0f : MAX_SPEED;
        if ( Input::heldRight() )
        {
            goRight();
        }
        else if ( Input::heldLeft() )
        {
            goLeft();
        }
        else
        {
            accx_ = 0.0f;
            is_moving_ = false;
        }

        vx_ += accx_ * dt;
        if ( vx_ > max_speed_ )
        {
            vx_ = max_speed_;
        }
        else if ( vx_ < -max_speed_ )
        {
            vx_ = -max_speed_;
        }
        if ( accx_ == 0.0f )
        {
            vx_ /= ( 1.0f + 0.5f * dt );
        }
        pos_.x += vx_ * dt;

        // Falling & Jumping
        const bool going_fast = isAutumnGoingFast();
        const float gravity = Input::heldJump() ? GRAVITY / 1.5f : GRAVITY;
        const float max_jump = going_fast ? max_jump_ * 1.1f : max_jump_;
        const bool can_start_jump = misc_.autumn.jump_padding > 0.0f &&
            !misc_.autumn.jump_lock &&
            Input::heldJump();

        // Continue jump.
        if ( is_jumping_ )
        {
            if ( Input::heldJump() )
            {
                accy_ = -JUMP_ACC;
            }
            else
            {
                is_jumping_ = false;
                accy_ = 0.0f;
            }
        }
        // Start jump.
        else if ( can_start_jump )
        {
            is_jumping_ = true;
            vy_ = -JUMP_INIT;
            accy_ = -JUMP_ACC;
        }
        // Else, fall.
        else
        {
            accy_ = FALL_SPEED;
        }

        vy_ += accy_ * dt;
        if ( vy_ > gravity )
        {
            vy_ = gravity;
        }
        else if ( vy_ < -max_jump )
        {
            max_jump_ = JUMP_MAX;
            vy_ = -max_jump;
            is_jumping_ = false;
            accy_ = 0.0f;
        }

        float ychange = vy_ * dt;

        on_ground_ = false;
        misc_.autumn.jump_padding = std::max( 0.0f, misc_.autumn.jump_padding - 1.0f * dt);

        // Handle downward collision.
        if ( ychange > 0.0f )
        {
            const float ystep = 4.0f;
            while ( ychange != 0.0f )
            {
                pos_.y += std::min( ystep, ychange );
                ychange = std::max( 0.0f, ychange - ystep );

                const int bl = pixelsToBlocks( pos_.x + 2.0f );
                const int br = pixelsToBlocks( pos_.right() - 2.0f );
                const int by = pixelsToBlocks( pos_.bottom() - 1.0f );
                const bool blc = map.testCollision( bl, by );
                const bool brc = map.testCollision( br, by );
                const bool blc2 = map.testCollision( bl, by, CollisionType::SOLID_ABOVE );
                const bool brc2 = map.testCollision( br, by, CollisionType::SOLID_ABOVE );
                const float bydiff = pos_.bottom() - 1.0f - static_cast<float>( blocksToPixels( by ) );
                const bool flat_top_coll = ( blc2 || brc2 ) && bydiff <= 4.0f;
                if ( blc || brc || flat_top_coll )
                {
                    pos_.y -= pos_.bottom() - 1.0f - blocksToPixels( by );
                    autumnLanding();
                }

                /*
                for ( Sprite & sprite : sprites )
                {
                    sprite.collideDown( *this, dt, level, game );
                }*/
            }
        }
        // Handle upward collision.
        else if ( ychange < 0.0f )
        {
            const float ystep = -12.0f;
            while ( ychange != 0.0f )
            {
                pos_.y += std::max( ystep, ychange );
                ychange = std::min( 0.0f, ychange - ystep );

                const int tl = pixelsToBlocks( pos_.x + 4.0f );
                const int tr = pixelsToBlocks( pos_.right() - 4.0f );
                const int ty = pixelsToBlocks( pos_.y );
                const bool tlc = map.testCollision( tl, ty );
                const bool trc = map.testCollision( tr, ty );
                if ( tlc || trc )
                {
                    pos_.y = blocksToPixels( ty + 1 );
                    vy_ *= -0.25f;
                    accy_ = 0.0f;
                    is_jumping_ = false;
                }

                /*
                for ( Sprite & sprite : sprites )
                {
                    sprite.collideUp( *this, dt, level, game );
                }*/
            }
        }

        // Handle right collision.
        const int rx = pixelsToBlocks( pos_.right() );
        const int xt = pixelsToBlocks( pos_.y + 2.0f );
        const int xb = pixelsToBlocks( pos_.bottom() - 4.0f );
        const bool rtc = map.testCollision( rx, xt );
        const bool rbc = map.testCollision( rx, xb );
        if ( rtc || rbc )
        {
            collision_.right = pos_.right() - blocksToPixels( rx );
            pos_.x -= collision_.right;
            vx_ *= -0.25f;
            accx_ = 0.0f;
        }
        // Handle left collision.
        const int lx = pixelsToBlocks( pos_.x );
        const bool ltc = map.testCollision( lx, xt );
        const bool lbc = map.testCollision( lx, xb );
        if ( ltc || lbc )
        {
            collision_.left = 1.0f;
            pos_.x = blocksToPixels( lx + 1 );
            vx_ *= -0.25f;
            accx_ = 0.0f;
        }

        // Rotation
        rotateOnDirectionChange( dt );

        // Update graphics.
        if ( !on_ground_ )
        {
            graphic_.setSrcX( 48.0f );
            graphic_.setSrcY( 0.0f );
        }
        else
        {
            if ( is_moving_ )
            {
                if ( updateAnimationTimer( dt ) )
                {
                    ++misc_.autumn.walk_frame;
                    if ( misc_.autumn.walk_frame > 3 )
                    {
                        misc_.autumn.walk_frame = 0;
                    }
                }
                graphic_.setSrcX( autumn_walk_frames[ misc_.autumn.walk_frame ] );
                graphic_.setSrcY( 0.0f );
            }
            else
            {
                graphic_.setSrcX( 0.0f );
                graphic_.setSrcY( 0.0f );
            }
        }
        updatePositionGraphics();

        // Update camera.
        NasrRect r = graphic_.getDest();
        NasrAdjustCamera( &r, map.getWidthPixels(), map.getHeightPixels() );

        // Update jump lock.
        misc_.autumn.jump_lock = Input::heldJump();
    };

    void Sprite::goLeft()
    {
        dir_x_ = Dir::X::LEFT;
        accx_ = -start_speed_;
        is_moving_ = true;
    };

    void Sprite::goRight()
    {
        dir_x_ = Dir::X::RIGHT;
        accx_ = start_speed_;
        is_moving_ = true;
    };

    void Sprite::updatePositionGraphics( float xoffset, float yoffset )
    {
        if ( pos_.x != prevx_ )
        {
            graphic_.setDestX( pos_.x - xoffset );
        }
        if ( pos_.y != prevy_ )
        {
            graphic_.setDestY( pos_.y - yoffset );
        }
        prevx_ = pos_.x;
        prevy_ = pos_.y;
        collision_ = { 0.0f, 0.0f, 0.0f, 0.0f };
    };

    bool Sprite::isAutumnGoingFast() const
    {
        return std::abs( vx_ ) >= MAX_SPEED * 1.5f;
    };

    void SpriteSystem::init( Game & game, Map & map, float x, float y )
    {
        hero_.pos_ = { x, y, 16.0f, 25.0f };
        hero_.init( game );
    };

    void SpriteSystem::update( Game & game, Map & map, float dt )
    {
        hero_.update( game, map, dt );
    };

    void Sprite::autumnLanding()
    {
        on_ground_ = true;
        misc_.autumn.jump_padding = isAutumnGoingFast() ? 8.0f : 2.0f;
        max_jump_ = JUMP_MAX;
    };

    bool Sprite::updateAnimationTimer( float dt, float time )
    {
        if ( ( animation_timer_ += dt ) >= time )
        {
            animation_timer_ -= time;
            return true;
        }
        return false;
    };

    void Sprite::rotateOnDirectionChange( float dt )
    {
        float rotx = graphic_.getRotationX();
        switch ( dir_x_ )
        {
            case ( Dir::X::LEFT ):
            {
                if ( rotx > 0.0f )
                {
                    rotx = std::max( rotx - ( 16.0f * dt ), 0.0f );
                    graphic_.setRotationX( rotx );
                }
            }
            break;
            case ( Dir::X::RIGHT ):
            {
                if ( rotx < 180.0f )
                {
                    rotx = std::min( rotx + ( 16.0f * dt ), 180.0f );
                    graphic_.setRotationX( rotx );
                }
            }
            break;
        }
    };
}