#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "game.h"
#include "view.h"

int main()
{
    std::vector<std::vector<Tile>> tiles;

    /** Main loop */
    bool running = true;

    View* view = View::GetInstance();

    SDL_Event event;
    while (running)
    {
        /** Process events */
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    //running = false;
                   // break; 
                   view->destroy();
                   return 0;
            }
        }
    }
    return 0;
}