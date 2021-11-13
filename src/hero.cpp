#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "map.hpp"
#include "render.hpp"
#include "tile.hpp"
#include "title_state.hpp"
#include "hero.hpp"

namespace Hero
{
    Sprite create()
    {
        Sprite hero;
        hero.position = { 300.0f, 348.0f, 16.0f, 26.0f };
        hero.vx = 0.0f;
        hero.accx = 0.0f;
        hero.top_speed = 2.0f;
        hero.vy = 0.0f;
        hero.accy = 0.0f;
        hero.gravity = 6.0f;
        hero.is_jumping = false;
        hero.jump_lock = false;
        hero.props = { { "traction", 1.15f } };
        hero.update = []( Sprite & self, LevelState & level )
        {
            if ( Input::heldJump() )
            {
                if ( !self.jump_lock && testOnGround( self, level ) )
                {
                    self.is_jumping = true;
                }
                self.jump_lock = true;
            }
            else
            {
                self.is_jumping = false;
                self.jump_lock = false;
            }

            if ( Input::heldRight() )
            {
                self.accx = 0.25f;
            }
            else if ( Input::heldLeft() )
            {
                self.accx = -0.25f;
            }
            else
            {
                self.accx = 0.0f;
                self.vx /= std::get<float>( self.props[ "traction" ] );
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

            if ( self.is_jumping )
            {
                self.accy = -0.25f;
                self.vy += self.accy;
                if ( self.vy < -4.0f )
                {
                    self.vy = -4.0f;
                    self.is_jumping = false;
                }
                self.position.y += self.vy;
            }
            else
            {
                self.accy = 0.25f;
                self.vy += self.accy;
                if ( self.vy > self.gravity )
                {
                    self.vy = self.gravity;
                }
                self.position.y += self.vy;
            }

            mapInteract( self, level );

            Graphic * gfx = Render::getGraphic( self.gfx );
            auto & g = std::get<SpriteGraphics>( gfx->data );
            g.dest = self.position;
        };
        return hero;
    };

    void mapInteract( Sprite & self, LevelState & level )
    {
        Map & map = level.map;
        const int sx = Unit::pixelsToBlocks( self.position.centerX() );
        const int sy = Unit::pixelsToBlocks( self.position.centerY() );
        const int i = map.getIFromXAndY( sx, sy );
        const int ox = i % map.width;
        const int oy = std::floor( ( double )( i ) / ( double )( map.width ) );

        // Handle basic collision interaction.
        const int dlx = Unit::pixelsToBlocks( self.position.x + 2.0f );
        const int drx = Unit::pixelsToBlocks( self.position.right() - 2.0f );
        const int dy = Unit::pixelsToBlocks( self.position.bottom() - 1.0f );
        const int dli = map.getIFromXAndY( dlx, dy );
        const int dri = map.getIFromXAndY( drx, dy );
        if
        (
            map.testTile( dli, MapCollisionType::SOLID ) || map.testTile( dri, MapCollisionType::SOLID ) ||
            (
                self.vy >= 0.0f && ( self.position.bottom() - Unit::blocksToPixels( dy ) < 8.0f ) &&
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

        
        const int ulx = Unit::pixelsToBlocks( self.position.x + 2.0f );
        const int urx = Unit::pixelsToBlocks( self.position.right() - 2.0f );
        const int uy = Unit::pixelsToBlocks( self.position.y );
        const int uli = map.getIFromXAndY( ulx, uy );
        const int uri = map.getIFromXAndY( urx, uy );
        if ( map.testTile( uli, MapCollisionType::SOLID ) || map.testTile( uri, MapCollisionType::SOLID ) )
        {
            self.accy = 0.0f;
            self.vy *= -0.25f;
            self.position.y += Unit::blocksToPixels( uy + 1 ) - self.position.y;
        }

        
        const int lx = Unit::pixelsToBlocks( self.position.x );
        const int lty = Unit::pixelsToBlocks( self.position.y + 2.0f );
        const int lby = Unit::pixelsToBlocks( self.position.bottom() - 4.0f );
        const int lti = map.getIFromXAndY( lx, lty );
        const int lbi = map.getIFromXAndY( lx, lby );
        if ( map.testTile( lti, MapCollisionType::SOLID ) || map.testTile( lbi, MapCollisionType::SOLID ) )
        {
            self.accx = 0.0f;
            self.vx *= -0.25f;
            self.position.x += Unit::blocksToPixels( lx + 1 ) - self.position.x;
        }

        const int rx = Unit::pixelsToBlocks( self.position.right() );
        const int rty = Unit::pixelsToBlocks( self.position.y + 2.0f );
        const int rby = Unit::pixelsToBlocks( self.position.bottom() - 4.0f );
        const int rti = map.getIFromXAndY( rx, rty );
        const int rbi = map.getIFromXAndY( rx, rby );
        if ( map.testTile( rti, MapCollisionType::SOLID ) || map.testTile( rbi, MapCollisionType::SOLID ) )
        {
            self.accx = 0.0f;
            self.vx *= -0.25f;
            self.position.x -= self.position.right() - Unit::blocksToPixels( rx );
        }

        // Handle object interaction.
        if ( i < map.objs.size() )
        {
            for ( MapObj & obj : map.objs[ i ] )
            {
                const TilemapGraphics & tilemapgfx = std::get<TilemapGraphics>( Render::getGraphic( obj.tilemap )->data );
                switch ( obj.type )
                {
                    case ( MapObjType::GEM ):
                    {
                        const auto seek = obj.props.find( "amount" );
                        if ( seek != obj.props.end() )
                        {
                            const int amount = std::get<int>( seek->second );
                            level.inventory.addGems( amount );
                            Render::changeTilemap( tilemapgfx, ox, oy, { 255, 255, 255, 255 } );
                            obj.type = MapObjType::__NULL;
                        }
                    }
                    break;
                }
            }
        }
    }

    bool testOnGround( const Sprite & self, const LevelState & level )
    {
        const Map & map = level.map;
        const int dlx = Unit::pixelsToBlocks( self.position.x );
        const int drx = Unit::pixelsToBlocks( self.position.right() );
        const int dy = Unit::pixelsToBlocks( self.position.bottom() - 1.0f );
        const int dli = map.getIFromXAndY( dlx, dy );
        const int dri = map.getIFromXAndY( drx, dy );
        return map.testTileMulti( dli, { MapCollisionType::SOLID, MapCollisionType::SOLID_TOP } ) ||
            map.testTileMulti( dri, { MapCollisionType::SOLID, MapCollisionType::SOLID_TOP } );
    };
};