#include <iostream>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

class View
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint32 init();
public:
    static View* instance;

    void destroy();
    void clearScreen();

    static View* GetInstance();
};