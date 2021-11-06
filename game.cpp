#include <iostream>
#include "game.h"

Game::Game()
{
    
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
            adjacentBombs = 0;
            if (tiles[i][j] == 9)
                continue;
            if (i-1 >= 0 && tiles[i-1][j] == 9)
                adjacentBombs++;
            if (i+1 < state->rows && tiles[i+1][j] == 9)
                adjacentBombs++;
            if (j-1 >= 0 && tiles[i][j-1] == 9)
                adjacentBombs++;            
            if (j+1 < state->columns && tiles[i][j+1] == 9)
                adjacentBombs++;            
            if (i-1 >= 0 && j-1 >= 0 && tiles[i-1][j-1] == 9)
                adjacentBombs++;            
            if (i-1 >= 0 && j+1 < state->columns && tiles[i-1][j+1] == 9)
                adjacentBombs++;
            if (i+1 < state->rows && j-1 >= 0 && tiles[i+1][j-1] == 9)
                adjacentBombs++;
            if (i+1 < state->rows && j+1 < state->columns && tiles[i+1][j+1] == 9)
                adjacentBombs++;
            tiles[i][j] = adjacentBombs;
        }
    }
    state->setTiles(tiles);
}

void Game::buildBoard()
{
   placeBombs();
   addNumbers();
}


void Game::init() 
{
    state = State::getInstance();

    buildBoard();

    std::cout << "Bombs placed" << std::endl;

    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            std::cout << state->getTiles()[i][j] << " ";
        }
        std::cout << std::endl;
    } 

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
                    running = false;
                    break;
                default:
                    break;
            }
        }

        view->clearScreen();
        view->drawTile();
    }
}
