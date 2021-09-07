#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include <string>
#include <variant>

class TitleState
{
    public:
        constexpr TitleState( float x ) : x_ ( x ) {};
        constexpr float x() const { return x_; };
        constexpr void inc() { x_ += 1.0f; };

    private:
        float x_;
};

struct LevelState
{
    public:
        LevelState( std::string msg ) : msg_ ( msg ) {};
        void print() { printf( "%s\n", msg_.c_str() ); }

    private:
        std::string msg_;
};

typedef std::variant<std::monostate, TitleState, LevelState> GameState;

namespace GameStateMachine
{
    void init();
    void update();
    GameState createTitleState();
    GameState createLevelState();
    void changeState( GameState state );
};

#endif // GAME_STATE_MACHINE_H