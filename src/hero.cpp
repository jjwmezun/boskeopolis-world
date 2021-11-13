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
        hero.props = { { "traction", 1.15f }, { "on_ground", false } };
        hero.update = []( Sprite & self, LevelState & level )
        {
            if ( Input::pressedConfirm() )
            {
                GameStateMachine::changeState( TitleState() );
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

            /*
            self.accy = 0.25f;
            self.vy += self.accy;
            if ( self.vx > self.gravity )
            {
                self.vx = self.gravity;
            }
            self.position.y += self.vy;*/

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
        const int i = ( sy * map.width ) + ( sx % map.width );

        if ( i < map.objs.size() )
        {
            const int ox = i % map.width;
            const int oy = std::floor( ( double )( i ) / ( double )( map.width ) );
            for ( MapObj & obj : map.objs[ i ] )
            {
                const TilemapGraphics & tilemapgfx = std::get<TilemapGraphics>( Render::getGraphic( obj.tilemap )->data );
                switch ( obj.type )
                {
                    case ( MapObjType::GEM ):
                    {
                        Render::changeTilemap( tilemapgfx, ox, oy, { 255, 255, 255, 255 } );
                    }
                    break;
                }
            }
        }
    }
};