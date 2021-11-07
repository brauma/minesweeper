#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <tuple>
#include "view.h"
#include "state.h"
#include "time.h"

#define GRID_WIDTH 30
#define GRID_HEIGHT 16

class Game 
{
private:
    State* state;

    int pressed_i, pressed_j;
    bool running;
public:
    Game();

    void init();
    void run();
    void restart();

    void gameOver();

    void buildBoard();
    void placeBombs();
    void addNumbers();

    std::vector<std::tuple<int, int>> getAdjacentTiles(int i, int j);

    void clickEmptyTiles(int i, int j);
    void clickSurroundingTiles(int i, int j);

    void handleMouseInput(SDL_Event event);
};
