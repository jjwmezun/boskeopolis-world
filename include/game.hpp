#ifndef GAME_H
#define GAME_H

#include "config.hpp"
#include "inventory.hpp"
#include "map.hpp"
#include "renderer.hpp"
#include "sprite_system.hpp"

namespace BSW
{
    enum class GameStateType
    {
        FADE_IN,
        FADE_TO_AREA,
        AREA_NAME,
        MAIN
    };

    struct FadeState
    {
        RectGraphic graphic;
        float speed;
        float opacity;
    };

    struct AreaNameState
    {
        TextGraphic text;
        float timer;
    };

    struct MainState
    {
        RectGraphic bg;
        SpriteSystem sprites;
        Map map;
        Inventory inventory;
    };

    union GameStateData
    {
        FadeState fade;
        AreaNameState areaname;
        MainState main;
    };

    struct GameState
    {
        GameStateType type;
        GameStateData data;
    };

    class Game
    {
        public:
            void init();
            void close();
            void update( float dt );
            void popState();
            void closeState( unsigned int n );
            constexpr const Renderer & render() const { return renderer_; };

        private:
            GameState states_[ MAX_STATES ];
            Renderer renderer_;
            unsigned int num_o_states_;
    };
}

#endif // GAME_H