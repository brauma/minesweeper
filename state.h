#ifndef STATE_H
#define STATE_H

#include <vector>

namespace Tile{

enum Type {
    Empty_Tile,
    One_Tile,
    Two_Tile,
    Three_Tile,
    Four_Tile,
    Five_Tile,
    Six_Tile,
    Seven_Tile,
    Eight_Tile,
    Unexploded_Bomb_Tile,
    Unclicked_Tile,
    Flag_Tile,
    Exploded_Bomb_Tile,
    Missed_Bomb_Tile
};

enum State {
    Unclicked,
    Clicked,
    Flagged,
    Pressed,
    Exploded,
    Missed
};

} // namespace Tile

namespace Button {

enum State {
    Unclicked,
    Clicked,
    Progressed,
    Won,
    Lost
};

} // namespace Button

class State
{
private:
    static State* instance;

    std::vector<std::vector<Tile::Type>> defaultTiles;
    std::vector<std::vector<Tile::State>> defaultTileStates;
    std::vector<std::vector<Tile::Type>> tiles;
    std::vector<std::vector<Tile::State>> tileStates;
    Button::State buttonState;

    bool play;
public:
    int remainingBombs;
    int rows = 16, columns = 30;

    State();

    static State* getInstance();
    
    bool canPlay();
    void setPlay(bool _play);

    std::vector<std::vector<Tile::Type>>& getTiles();
    std::vector<std::vector<Tile::State>>& getTileStates();
    Button::State& getButtonState();
    void setTiles(std::vector<std::vector<Tile::Type>>& newTiles);

    void resetState();
    void destroy();
};

#endif
