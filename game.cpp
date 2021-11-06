#include <iostream>
#include "game.h"

Game::Game()
{
    init();
}

void Game::init() 
{
    state = State::getInstance();

    buildBoard();

    std::cout << "Bombs placed" << std::endl;

    /* for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            std::cout << state->getTiles()[i][j] << " ";
        }
        std::cout << std::endl;
    }  */

    running = true;
}

void Game::restart()
{
    state->resetState();
    buildBoard();
}

void Game::run()
{
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
                    view->destroy();
                    state->destroy();

                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    handleMouseInput(event.button);
                    break;
                default:
                    break;
            }
        }

        view->clearScreen();
        view->drawScreen();
    }
}

void Game::buildBoard()
{
   placeBombs();
   addNumbers();
}

void Game::placeBombs()
{
    std::vector<std::vector<int>> tiles = state->getTiles();
    
    srand(time(0));
    int bombsToPlace = 99;
    int i, j;
    while (bombsToPlace > 0)
    {
        i = rand() % state->rows;
        j = rand() % state->columns;
        
        if (tiles[i][j] == 9)
            continue;
        
        tiles[i][j] = 9;
        bombsToPlace--;
    }

    state->setTiles(tiles);
}

void Game::addNumbers()
{
    int adjacentBombs;
    std::vector<std::vector<int>> tiles = state->getTiles();

    for (int i = 0; i < state->rows; i++) 
    {
        for (int j = 0; j < state->columns; j++)
        {
            if (tiles[i][j] == TileType::Bomb_Tile)
                continue;

            adjacentBombs = 0;
            for (auto adjacent : getAdjacentTiles(i, j))
            {
                if (tiles[std::get<0>(adjacent)][std::get<1>(adjacent)] == TileType::Bomb_Tile)
                    adjacentBombs++;
            }
            tiles[i][j] = adjacentBombs;
        }
    }
    state->setTiles(tiles);
}

std::vector<std::tuple<int, int>> Game::getAdjacentTiles(int i, int j)
{
    std::vector<std::tuple<int, int>> adjacentTiles;

    if (i-1 >= 0)
        adjacentTiles.push_back(std::tuple<int, int>(i-1, j));
    if (i+1 < state->rows)
        adjacentTiles.push_back(std::tuple<int, int>(i+1, j));
    if (j-1 >= 0)
        adjacentTiles.push_back(std::tuple<int, int>(i, j-1));
    if (j+1 < state->columns)
        adjacentTiles.push_back(std::tuple<int, int>(i, j+1));
    if (i-1 >= 0 && j-1 >= 0)
        adjacentTiles.push_back(std::tuple<int, int>(i-1, j-1));
    if (i-1 >= 0 && j+1 < state->columns)
        adjacentTiles.push_back(std::tuple<int, int>(i-1, j+1));
    if (i+1 < state->rows && j-1 >= 0)
        adjacentTiles.push_back(std::tuple<int, int>(i+1, j-1));
    if (i+1 < state->rows && j+1 < state->columns)
        adjacentTiles.push_back(std::tuple<int, int>(i+1, j+1));
    
    return adjacentTiles;
}

void Game::clickEmptyTiles(int i, int j)
{
    std::vector<std::vector<int>> tiles = state->getTiles();

    state->getTileStates()[i][j] = true;
    auto adjacentTiles = getAdjacentTiles(i, j);
    int current_i, current_j;

    for (auto adjacent : adjacentTiles)
    {
        current_i = std::get<0>(adjacent);
        current_j = std::get<1>(adjacent);

        if (state->getTileStates()[current_i][current_j])
            continue;

        if (tiles[current_i][current_j] > TileType::Empty_Tile &&
            tiles[current_i][current_j] < TileType::Bomb_Tile) 
        {
            state->getTileStates()[current_i][current_j] = true;
        }

        if (tiles[current_i][current_j] == TileType::Empty_Tile)
        {
            clickEmptyTiles(current_i, current_j);
        }
    }
}

void Game::handleMouseInput(SDL_MouseButtonEvent mouseButtonEvent)
{
    std::tuple<int, int> clickedTile = View::GetInstance()->getClickedTile(mouseButtonEvent.x, mouseButtonEvent.y);
    int i = std::get<0>(clickedTile);
    int j = std::get<1>(clickedTile);

    if (i == -1)
        return;

    switch (mouseButtonEvent.button)
    {
        case SDL_BUTTON_LEFT:
            if (!state->getTileStates()[i][j]){
                state->getTileStates()[i][j] = true;
                switch(state->getTiles()[i][j])
                {
                    case TileType::Bomb_Tile:
                        // temporary
                        // View::GetInstance()->gameOverScreen();
                        //restart();
                        break;
                    case TileType::Empty_Tile:
                        clickEmptyTiles(i, j);
                        break;
                    default:
                        break;
                }
            }
            break;
        case SDL_BUTTON_RIGHT:
            std::cerr << "Right mouse button!" << std::endl;
            break;
        default:
            std::cerr << "Some mouse button!" << std::endl;
            break;
    }
}
