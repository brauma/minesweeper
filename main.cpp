#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "game.h"

int main()
{
    Game* game = new Game();
    game->run();

    return 0;
}