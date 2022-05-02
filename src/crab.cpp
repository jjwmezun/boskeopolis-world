#include "crab.hpp"
#include "level_state.hpp"
#include "map.hpp"
#include "render.hpp"

namespace Crab
{
    Sprite create( unsigned int state, int x, int y )
    {
        Sprite crab;
        crab.position = { ( float )( x * 16 ), ( float )( y * 16 ), 16.0f, 16.0f };
        crab.vx = 0.0f;
        crab.accx = 0.0f;
        crab.top_speed = 1.0f;
        crab.vy = 0.0f;
        crab.accy = 0.0f;
        crab.gravity = 6.0f;
        crab.jump_lock = false;
        crab.state = SpriteState::NORMAL;
        crab.props =
        {
            { "animation_timer", 0 }
        };
        crab.update = []( Sprite & self, LevelState & level )
        {
            const Sprite & hero = level.getHero();

            if ( hero.position.x < self.position.x )
            {
                self.accx = -0.1f;
            }
            else
            {
                self.accx = 0.1f;
            }

            self.vx += self.accx;
            if ( self.vx > self.top_speed )
            {
                self.vx = self.top_speed;
            }
            else if ( self.vx < -self.top_speed )
            {
                self.vx = -self.top_speed;
            }
            self.position.x += self.vx;

            self.accy = 0.25f;
            self.vy += self.accy;
            if ( self.vy > self.gravity )
            {
                self.vy = self.gravity;
            }
            self.position.y += self.vy;

            // Handle basic downward collision.
            const Map & map = level.map;
            const int dlx = Unit::pixelsToBlocks( self.position.x + 2.0f );
            const int drx = Unit::pixelsToBlocks( self.position.right() - 2.0f );
            const int dy = Unit::pixelsToBlocks( self.position.bottom() - 1.0f );
            const int dli = map.getIFromXAndY( dlx, dy );
            const int dri = map.getIFromXAndY( drx, dy );
            if
            (
                map.testTile( dli, MapCollisionType::SOLID ) || map.testTile( dri, MapCollisionType::SOLID ) ||
                (
                    self.state != SpriteState::ON_LADDER && self.vy >= 0.0f && ( self.position.bottom() - Unit::blocksToPixels( dy ) < 8.0f ) &&
                    (
                        map.testTile( dli, MapCollisionType::SOLID_TOP ) ||
                        map.testTile( dri, MapCollisionType::SOLID_TOP )
                    )
                )
            )
            {
                self.accy = 0.0f;
                self.vy = 0.0f;
                self.position.y -= self.position.bottom() - 1.0f - Unit::blocksToPixels( dy );
            }

            Graphic * gfx = Render::getGraphic( self.gfx );
            auto & g = std::get<SpriteGraphics>( gfx->data );
            g.dest = self.position;

            int animation_timer = std::get<int> ( self.props[ "animation_timer" ] );
            if ( animation_timer == 7 )
            {
                g.flip_x = !g.flip_x;
                self.props[ "animation_timer" ] = 0;
            }
            else
            {
                self.props[ "animation_timer" ] = animation_timer + 1;
            }
        };
        crab.gfx = Render::addGraphic( Graphic::createSprite( Render::getTextureID( "sprites/crab.png" ), 126, crab.position, 0.0f, 0.0f ), state, Layer::SPRITES_1 );
        return crab;
    };
};