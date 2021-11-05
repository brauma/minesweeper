#include <vector>
#include "tile.h"

#define GRID_WIDTH 30
#define GRID_HEIGHT 16

class Game 
{
public:
    Game();

private:
    std::vector<std::vector<Tile>> tiles;

};