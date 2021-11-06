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

public:
    bool running;

    Game();

    void init();
    void run();
    void restart();

    void buildBoard();
    void placeBombs();
    void addNumbers();

    std::vector<std::tuple<int, int>> getAdjacentTiles(int i, int j);

    void clickEmptyTiles(int i, int j);

    void handleMouseInput(SDL_MouseButtonEvent mouseButtonEvent);
};
