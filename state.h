#ifndef STATE_H
#define STATE_H

#include <vector>

enum TileType {
    Empty_Tile,
    One_Tile,
    Two_Tile,
    Three_Tile,
    Four_Tile,
    Five_Tile,
    Six_Tile,
    Seven_Tile,
    Eight_Tile,
    Bomb_Tile,
    Unclicked_Tile,
    Flag_Tile
};

enum TileState {
    Unclicked,
    Clicked,
    Flagged
};

class State
{
private:
    static State* instance;

    /**
     * 1-8 adjacent bomb count
     * 9 bomb 
     * 10 flag
     * 0 no bombs adjacent
     */
    std::vector<std::vector<int>> defaultTiles;
    std::vector<std::vector<int>> defaultTileStates;
    std::vector<std::vector<int>> tiles;
    std::vector<std::vector<int>> tileStates;
public:
    int remainingBombs;
    int rows = 16, columns = 30;

    State();

    static State* getInstance();
    
    std::vector<std::vector<int>>& getTiles();
    std::vector<std::vector<int>>& getTileStates();
    void setTiles(std::vector<std::vector<int>>& newTiles);
    void resetState();
    void destroy();
};

#endif
