#include "state.h"

State* State::instance = nullptr;

State::State()
{
    // this needs fixing
    defaultTiles = std::vector<std::vector<int>> (rows, std::vector<int>(columns, TileType::Empty_Tile));
    defaultTileStates = std::vector<std::vector<int>> (rows, std::vector<int>(columns, TileState::Unclicked));
    tiles = defaultTiles;
    tileStates = defaultTileStates;
    remainingBombs = 99;
}

std::vector<std::vector<int>>& State::getTiles()
{
    return tiles;
}

std::vector<std::vector<int>>& State::getTileStates()
{
    return tileStates;
}

// This may be unnecessary 
void State::setTiles(std::vector<std::vector<int>>& newTiles)
{
    tiles = newTiles;
}

void State::resetState() 
{
    tiles = defaultTiles;
    tileStates = defaultTileStates;
    remainingBombs = 99;
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