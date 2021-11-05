#include "tile.h"

Tile::Tile(Uint32 _x, Uint32 _y, Uint32 bombCount, bool _hasBomb)
{
    x = _x;
    y = _y;
    surroundingBombs = bombCount;
    hasBomb = _hasBomb;
    rect = {(int) x, (int) y, SIZE, SIZE};
}

void Tile::draw(SDL_Renderer* renderer) 
{

}