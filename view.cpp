#include "view.h"

View* View::instance = nullptr;

Uint32 View::init()
{
    /** Initialization */
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initalizing SDL: " << SDL_GetError() << std::endl;
        return 0;
    }
    std::cout << "SDL successfully initalized!" << std::endl;

    /** Creating window */
    window = SDL_CreateWindow("Minesweeper",
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
    renderer = SDL_CreateRenderer(window, -1, render_flags);

    if(!renderer)
    {
        std::cerr << "Error creating renderer!" << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }
    return 1;
}

View* View::GetInstance()
{
    if (!instance) {
        instance = new View();

        if (instance->init() == 0)
        {
            std::cerr << "Error initalizing view!" << std::endl;
        }
    }
    return instance;
}

void View::clearScreen()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
} 

void View::destroy()
{
    /** Release resources */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete instance;
    SDL_Quit();
}