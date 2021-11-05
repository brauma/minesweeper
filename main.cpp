#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vector>
#include "tile.h"

#define WIDTH 640
#define HEIGHT 480

#define GRID_WIDTH 30
#define GRID_HEIGHT 16

void clearScreen(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void init(std::vector<std::vector<Tile>> &tiles)
{

}

int main()
{

    std::vector<std::vector<Tile>> tiles;

    /** Initialization */
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initalizing SDL: " << SDL_GetError() << std::endl;
        return 0;
    }
    std::cout << "SDL successfully initalized!" << std::endl;

    /** Creating window */
    SDL_Window* window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT, 0);
                                        
    if(!window)
    {
        std::cerr << "Error creating window!" << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    /** Creating renderer */
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);

    if(!renderer)
    {
        std::cerr << "Error creating renderer!" << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    /** Main loop */
    bool running = true;
    SDL_Event event;
    while (running)
    {
        /** Process events */
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
    }
    /** Release resources */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}