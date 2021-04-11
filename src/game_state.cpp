#include "game_state.hpp"

GameState::~GameState() {};

void GameState::init() {};

void GameState::close() {};

void GameState::render() const
{
    graphics.render();
};