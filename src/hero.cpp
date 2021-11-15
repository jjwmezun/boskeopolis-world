#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "map.hpp"
#include "render.hpp"
#include "tile.hpp"
#include "title_state.hpp"
#include "treasure_message_state.hpp"
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
        hero.jump_lock = false;
        hero.state = SpriteState::NORMAL;
        hero.props = { { "traction", 1.15f } };
        hero.update = []( Sprite & self, LevelState & level )
        {
            if ( Input::pressedMenu() )
            {
                GameStateMachine::changeState( TitleState() );
            }

            switch ( self.state )
            {
                case ( SpriteState::NORMAL ):
                {
                    if ( Input::heldJump() )
                    {
                        if ( !self.jump_lock && testOnGround( self, level ) )
                        {
                            self.state = SpriteState::JUMPING;
                        }
                        self.jump_lock = true;
                    }
                    else
                    {
                        self.jump_lock = false;
                    }

                    if ( level.inventory.hasTreasure( TreasureType::GLOVES ) )
                    {
                        if ( Input::heldUp() )
                        {
                            const int climbleft = Unit::pixelsToBlocks( self.position.x );
                            const int climbup = Unit::pixelsToBlocks( self.position.y );
                            const int climbdown = Unit::pixelsToBlocks( self.position.bottom() - 8.0f );
                            const int climbright = Unit::pixelsToBlocks( self.position.right() );
                            const int climblu = level.map.getIFromXAndY( climbleft, climbup );
                            const int climbru = level.map.getIFromXAndY( climbright, climbup );
                            const int climbld = level.map.getIFromXAndY( climbleft, climbdown );
                            const int climbrd = level.map.getIFromXAndY( climbright, climbdown );
                            if
                            (
                                level.map.testTile( climblu, MapCollisionType::CLIMB ) ||
                                level.map.testTile( climbru, MapCollisionType::CLIMB ) ||
                                level.map.testTile( climbld, MapCollisionType::CLIMB ) ||
                                level.map.testTile( climbrd, MapCollisionType::CLIMB )
                            )
                            {
                                self.state = SpriteState::ON_LADDER;
                                self.vy = 0;
                                self.accy = 0;
                            }
                        }
                        else if ( Input::heldDown() )
                        {
                            const int climbleft = Unit::pixelsToBlocks( self.position.x );
                            const int climbup = Unit::pixelsToBlocks( self.position.y + 8.0f );
                            const int climbdown = Unit::pixelsToBlocks( self.position.bottom() + 2.0f );
                            const int climbright = Unit::pixelsToBlocks( self.position.right() );
                            const int climblu = level.map.getIFromXAndY( climbleft, climbup );
                            const int climbru = level.map.getIFromXAndY( climbright, climbup );
                            const int climbld = level.map.getIFromXAndY( climbleft, climbdown );
                            const int climbrd = level.map.getIFromXAndY( climbright, climbdown );
                            if
                            (
                                level.map.testTile( climblu, MapCollisionType::CLIMB ) ||
                                level.map.testTile( climbru, MapCollisionType::CLIMB ) ||
                                level.map.testTile( climbld, MapCollisionType::CLIMB ) ||
                                level.map.testTile( climbrd, MapCollisionType::CLIMB )
                            )
                            {
                                self.state = SpriteState::ON_LADDER;
                                self.vy = 0;
                                self.accy = 0;
                            }

                        }
                    }

                    moveHorizontally( self, 0.25f, self.top_speed, std::get<float>( self.props[ "traction" ] ) );

                    self.accy = 0.25f;
                    self.vy += self.accy;
                    if ( self.vy > self.gravity )
                    {
                        self.vy = self.gravity;
                    }
                    self.position.y += self.vy;
                }
                break;
                case ( SpriteState::JUMPING ):
                {
                    if ( Input::heldJump() )
                    {
                        self.jump_lock = true;
                    }
                    else
                    {
                        self.state = SpriteState::NORMAL;
                        self.jump_lock = false;
                    }

                    moveHorizontally( self, 0.25f, self.top_speed, std::get<float>( self.props[ "traction" ] ) );

                    self.accy = -0.25f;
                    self.vy += self.accy;
                    if ( self.vy < -4.0f )
                    {
                        self.vy = -4.0f;
                        self.state = SpriteState::NORMAL;
                    }
                    self.position.y += self.vy;
                }
                break;
                case ( SpriteState::ON_LADDER ):
                {
                    self.jump_lock = false;
                    const int climbleft = Unit::pixelsToBlocks( self.position.x );
                    const int climbup = Unit::pixelsToBlocks( self.position.y );
                    const int climbdown = Unit::pixelsToBlocks( self.position.bottom() );
                    const int climbright = Unit::pixelsToBlocks( self.position.right() );
                    const int climblu = level.map.getIFromXAndY( climbleft, climbup );
                    const int climbru = level.map.getIFromXAndY( climbright, climbup );
                    const int climbld = level.map.getIFromXAndY( climbleft, climbdown );
                    const int climbrd = level.map.getIFromXAndY( climbright, climbdown );
                    if
                    (
                        !level.map.testTile( climblu, MapCollisionType::CLIMB ) &&
                        !level.map.testTile( climbru, MapCollisionType::CLIMB ) &&
                        !level.map.testTile( climbld, MapCollisionType::CLIMB ) &&
                        !level.map.testTile( climbrd, MapCollisionType::CLIMB )
                    )
                    {
                        self.state = SpriteState::NORMAL;
                    }
                    else if ( Input::heldJump() )
                    {
                        self.state = SpriteState::JUMPING;
                    }
                    else
                    {

                        const float ladder_start_speed = 0.25;
                        const float ladder_top_speed = 1.0;
                        const float ladder_traction = 1.25;

                        moveHorizontally( self, ladder_start_speed, ladder_top_speed, ladder_traction );

                        if ( Input::heldUp() )
                        {
                            self.accy = -ladder_start_speed;
                        }
                        else if ( Input::heldDown() )
                        {
                            self.accy = ladder_start_speed;
                        }
                        else
                        {
                            self.accy = 0.0f;
                            self.vy /= ladder_traction;
                        }
                        self.vy += self.accy;
                        if ( self.vy > ladder_top_speed )
                        {
                            self.vy = ladder_top_speed;
                        }
                        else if ( self.vy < -ladder_top_speed )
                        {
                            self.vy = -ladder_top_speed;
                        }
                        self.position.y += self.vy;
                    }
                }
                break;
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

        // Handle basic downward collision.
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

        
        // Handle basic upward collision.
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

        
        // Handle basic leftward collision.
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

        // Handle basic rightward collision.
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


        // Handle special object collision.
        for ( int sx = lx; sx <= rx; ++sx ) // From left tile touching to right tile.
        {
            for ( int sy = uy; sy <= dy; ++sy ) // From top tile touching to bottom.
            {
                const int i = map.getIFromXAndY( sx, sy );
                const int ox = i % map.width;
                const int oy = std::floor( ( double )( i ) / ( double )( map.width ) );

                if ( i < ( int )( map.objs.size() ) )
                {
                    auto it = map.objs[ i ].begin();
                    while ( it != map.objs[ i ].end() )
                    {
                        auto & obj = *it;
                        const TilemapGraphics & tilemapgfx = std::get<TilemapGraphics>( Render::getGraphic( obj.tilemap )->data );
                        switch ( obj.type )
                        {
                            case ( MapObjType::GEM ):
                            {
                                // Add amount to inventory gems.
                                const auto seek = obj.props.find( "amount" );
                                if ( seek != obj.props.end() )
                                {
                                    const int amount = std::get<int>( seek->second );
                                    level.inventory.addGems( amount );
                                }

                                // Remove gem tile graphics.
                                Render::changeTilemap( tilemapgfx, ox, oy, { 255, 255, 255, 255 } );

                                // Remove gem & set current iterator to next.
                                it = map.objs[ i ].erase( it );
                            }
                            break;
                            default:
                            {
                                if ( Input::pressedUp() )
                                {
                                    const auto seek = obj.props.find( "id" );
                                    int id = -1;
                                    if ( seek != obj.props.end() )
                                    {
                                        id = std::get<int>( seek->second );
                                        level.inventory.getTreasure( id );
                                    }

                                    // Change graphics to opened chest.
                                    Render::changeTilemap( tilemapgfx, ox, oy, { 7, 0, 1, 0 } );

                                    // Remove object & set current iterator to next.
                                    it = map.objs[ i ].erase( it );

                                    GameStateMachine::pushState( TreasureMessageState( id ) );
                                }
                                else
                                {
                                    ++it;
                                }
                            }
                            break;
                        }
                    }
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

    void moveHorizontally( Sprite & self, float start_speed, float top_speed, float traction )
    {
        if ( Input::heldRight() )
        {
            self.accx = start_speed;
        }
        else if ( Input::heldLeft() )
        {
            self.accx = -start_speed;
        }
        else
        {
            self.accx = 0.0f;
            self.vx /= traction;
        }

        self.vx += self.accx;
        if ( self.vx > top_speed )
        {
            self.vx = top_speed;
        }
        else if ( self.vx < -top_speed )
        {
            self.vx = -top_speed;
        }

        self.position.x += self.vx;
    }
};