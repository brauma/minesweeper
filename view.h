#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include "state.h"

#define WIDTH 625
#define HEIGHT 400
#define FPS 30

#define TILE_TEX_SIZE 17
#define TILE_SIZE 20
#define BUTTON_TEX_SIZE 25
#define BUTTON_SIZE 30

class View
{
private:
    State* state; 

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* buttonAtlas;
    SDL_Texture* boardTexture;

    std::vector<SDL_Rect> textures = std::vector<SDL_Rect>(11);
    std::vector<SDL_Rect> buttonTextures = std::vector<SDL_Rect>(5);
    SDL_Rect buttonPosition;
    std::vector<std::vector<SDL_Rect>> positions;

    int init();
public:
    static View* instance;

    static View* GetInstance();
    
    SDL_Texture* loadTexture(const std::string& file);
    void renderTexture(int x, int y, int w, int h);

    void clearScreen();
    void drawScreen();
    void drawBoard();
    void drawUI();
    void drawTile(SDL_Rect clip, SDL_Rect pos);

    void getTexturePositions();
    void getDrawPositions();

    std::tuple<int, int> getClickedTile(int x, int y);
    bool isButtonClicked(int x, int y);

    void gameOverScreen();
    void destroy();
};