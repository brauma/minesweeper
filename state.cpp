#include "state.h"

State* State::instance = nullptr;

State::State()
{
    defaultTiles = std::vector<std::vector<Tile::Type>> (rows, std::vector<Tile::Type>(columns, Tile::Type::Empty_Tile));
    defaultTileStates = std::vector<std::vector<Tile::State>> (rows, std::vector<Tile::State>(columns, Tile::State::Unclicked));
    tiles = defaultTiles;
    tileStates = defaultTileStates;
    buttonState = Button::State::Unclicked;
    remainingBombs = 99;
    play = true;
}

bool State::canPlay()
{
    return play;
}

void State::setPlay(bool _play)
{
    play = _play;
}

std::vector<std::vector<Tile::Type>>& State::getTiles()
{
    return tiles;
}

std::vector<std::vector<Tile::State>>& State::getTileStates()
{
    return tileStates;
}

Button::State& State::getButtonState()
{
    return buttonState;
}

// This may be unnecessary 
void State::setTiles(std::vector<std::vector<Tile::Type>>& newTiles)
{
    tiles = newTiles;
}

void State::resetState() 
{
    tiles = defaultTiles;
    tileStates = defaultTileStates;
    remainingBombs = 99;
    play = true;
}

State* State::getInstance(){
    if (!instance){
        instance = new State();
    } 
    return instance;
}

void State::destroy()
{
    delete instance;
}