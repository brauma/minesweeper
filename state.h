#ifndef STATE_H
#define STATE_H

#include <vector>

class State
{
private:
    static State* instance;

    /**
     * 1-8 adjacent bomb count
     * 9 bomb 
     * -1 flag
     * 0 no bombs adjacent
     */
    std::vector<std::vector<int>> defaultTiles;
    std::vector<std::vector<bool>> defaultTileStates;
    std::vector<std::vector<int>> tiles;
    std::vector<std::vector<bool>> tileStates;
public:
    int remainingBombs;
    int rows = 16, columns = 30;

    State();

    static State* getInstance();
    
    std::vector<std::vector<int>>& getTiles();
    void setTiles(std::vector<std::vector<int>>& newTiles);
    void resetState();
    
};

#endif
