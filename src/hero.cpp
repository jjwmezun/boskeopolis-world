#include "direction.hpp"
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
        hero.props =
        {
            { "traction", 1.15f },
            { "direction", ( int )( Direction::LEFT ) },
            { "walk_timer", 0 },
            { "walk_animation", 0 }
        };
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

                    self.accy = -0.75f;
                    self.vy += self.accy;
                    if ( self.vy < -5.0f )
                    {
                        self.vy = -5.0f;
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
            updateGraphics( self, level );
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
                                    Render::changeTilemap( tilemapgfx, ox, oy, { 7, 0, 124, 0 } );

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
    };

    void updateGraphics( Sprite & self, LevelState & level )
    {
        Graphic * gfx = Render::getGraphic( self.gfx );
        auto & g = std::get<SpriteGraphics>( gfx->data );
        g.dest = self.position;

        switch ( ( Direction )( std::get<int> ( self.props[ "direction" ] ) ) )
        {
            case ( Direction::LEFT ):
            {
                if ( g.rotation_x > 0.0f )
                {
                    g.rotation_x = std::max( 0.0f, g.rotation_x - 10.0f );
                }
            }
            break;
            case ( Direction::RIGHT ):
            {
                if ( g.rotation_x < 180.0f )
                {
                    g.rotation_x = std::min( 180.f, g.rotation_x + 10.0f );
                }
            }
            break;
        }

        switch ( self.state )
        {
            case ( SpriteState::NORMAL ):
            case ( SpriteState::JUMPING ):
            {
                if ( testOnGround( self, level ) )
                {
                    if ( self.accx != 0.0f )
                    {
                        int walk_timer = std::get<int> ( self.props[ "walk_timer" ] );
                        int walk_animation = std::get<int> ( self.props[ "walk_animation" ] );
                        if ( walk_timer >= 7 )
                        {
                            self.props[ "walk_timer" ] = 0;
                            ++walk_animation;
                            if ( walk_animation >= 4 )
                            {
                                walk_animation = 0;
                            }
                            self.props[ "walk_animation" ] = walk_animation;
                        }
                        else
                        {
                            self.props[ "walk_timer" ] = walk_timer + 1;
                        }
                        static constexpr float WALK_FRAMES[ 4 ] = { 0.0f, 16.0f, 0.0f, 32.0f };
                        g.src.x = WALK_FRAMES[ walk_animation ];
                    }
                    else
                    {
                        g.src.x = 0.0f;
                        self.props[ "walk_timer" ] = 0;
                        self.props[ "walk_animation" ] = 0;
                    }
                }
                else
                {
                    g.src.x = 48.0f;
                    self.props[ "walk_timer" ] = 0;
                    self.props[ "walk_animation" ] = 0;
                }
            }
            break;
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
            self.props[ "direction" ] = ( int )( Direction::RIGHT );
        }
        else if ( Input::heldLeft() )
        {
            self.accx = -start_speed;
            self.props[ "direction" ] = ( int )( Direction::LEFT );
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