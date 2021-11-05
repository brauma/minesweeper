#include <SDL2/SDL.h>

#define SIZE 10

class Tile 
{
    Uint32 x, y;
    Uint32 surroundingBombs;
    bool hasBomb;
    SDL_Rect rect;

    Tile(Uint32 _x, Uint32 _y, Uint32 bombCount, bool _hasBomb);

    bool clicked();
    void draw(SDL_Renderer* renderer);
};