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
    running = true;
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            std::cout << state->getTiles()[i][j] << " ";
        }
        std::cout << std::endl;
    }
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
                default:
                    handleMouseInput(event);
                    break;
            }
        }

        view->clearScreen();
        view->drawScreen();
    }
}

void Game::restart()
{
    state->resetState();
    buildBoard();
}

void Game::gameOver()
{
    std::vector<std::vector<Tile::Type>> tiles = state->getTiles();
    for (int i = 0; i < state->rows; i++) 
    {
        for (int j = 0; j < state->columns; j++)
        {
            if (tiles[i][j] == Tile::Type::Unexploded_Bomb_Tile && state->getTileStates()[i][j] != Tile::State::Exploded)
            {
                state->getTileStates()[i][j] = Tile::State::Clicked;
            }

            if (state->getTileStates()[i][j] == Tile::State::Flagged && tiles[i][j] != Tile::Type::Unexploded_Bomb_Tile)
            {
                state->getTileStates()[i][j] = Tile::State::Missed;
            }
        }
    }

    state->getButtonState() = Button::State::Lost;
    state->setPlay(false);
}

void Game::buildBoard()
{
    placeBombs();
    addNumbers();
}

void Game::placeBombs()
{
    std::vector<std::vector<Tile::Type>> tiles = state->getTiles();
    
    srand(time(0));
    int bombsToPlace = 99;
    int i, j;
    while (bombsToPlace > 0)
    {
        i = rand() % state->rows;
        j = rand() % state->columns;
        
        if (tiles[i][j] == Tile::Type::Unexploded_Bomb_Tile)
            continue;
        
        tiles[i][j] = Tile::Type::Unexploded_Bomb_Tile;
        bombsToPlace--;
    }

    state->setTiles(tiles);
}

void Game::addNumbers()
{
    int adjacentBombs;
    std::vector<std::vector<Tile::Type>> tiles = state->getTiles();

    for (int i = 0; i < state->rows; i++) 
    {
        for (int j = 0; j < state->columns; j++)
        {
            if (tiles[i][j] == Tile::Type::Unexploded_Bomb_Tile)
                continue;

            adjacentBombs = 0;
            for (auto adjacent : getAdjacentTiles(i, j))
            {
                if (tiles[std::get<0>(adjacent)][std::get<1>(adjacent)] == Tile::Type::Unexploded_Bomb_Tile)
                    adjacentBombs++;
            }
            tiles[i][j] = static_cast<Tile::Type>(adjacentBombs);
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
    std::vector<std::vector<Tile::Type>> tiles = state->getTiles();

    state->getTileStates()[i][j] = Tile::State::Clicked;
    auto adjacentTiles = getAdjacentTiles(i, j);
    int current_i, current_j;

    for (auto adjacent : adjacentTiles)
    {
        current_i = std::get<0>(adjacent);
        current_j = std::get<1>(adjacent);

        if (state->getTileStates()[current_i][current_j] == Tile::State::Clicked)
            continue;

        if (tiles[current_i][current_j] > Tile::Type::Empty_Tile &&
            tiles[current_i][current_j] < Tile::Type::Unexploded_Bomb_Tile) 
        {
            state->getTileStates()[current_i][current_j] = Tile::State::Clicked;
        }

        if (tiles[current_i][current_j] == Tile::Type::Empty_Tile)
        {
            clickEmptyTiles(current_i, current_j);
        }
    }
}

void Game::clickSurroundingTiles(int i, int j)
{
    std::vector<std::vector<Tile::Type>> tiles = state->getTiles();

    auto adjacentTiles = getAdjacentTiles(i, j);

    int current_i, current_j, flags = 0;

    for (auto adjacent : adjacentTiles)
    {
        current_i = std::get<0>(adjacent);
        current_j = std::get<1>(adjacent);

        if (state->getTileStates()[current_i][current_j] == Tile::State::Flagged)
            flags++;
    }

    if (flags != tiles[i][j])
        return;

    for (auto adjacent : adjacentTiles)
    {
        current_i = std::get<0>(adjacent);
        current_j = std::get<1>(adjacent);

        
        if (state->getTileStates()[current_i][current_j] != Tile::State::Flagged)
        {
            if (tiles[current_i][current_j] == Tile::Type::Unexploded_Bomb_Tile)
            {
                state->getTileStates()[current_i][current_j] = Tile::State::Exploded;
                gameOver();
                return;       
            }

            state->getTileStates()[current_i][current_j] = Tile::State::Clicked;
            if (tiles[current_i][current_j] == Tile::Type::Empty_Tile)
                clickEmptyTiles(current_i, current_j);
        }    
    }
}

void Game::handleMouseInput(SDL_Event event)
{
    if (event.type != SDL_MOUSEBUTTONUP && event.type != SDL_MOUSEBUTTONDOWN)
        return;

    // Check if left clicked on the button
    if (event.button.button == SDL_BUTTON_LEFT && View::GetInstance()->isButtonClicked(event.button.x, event.button.y))
    {
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            state->getButtonState() = Button::State::Clicked;
        }
        if (event.type == SDL_MOUSEBUTTONUP)
        {
            state->getButtonState() = Button::State::Unclicked;
            restart();
        }
        return;        
    }

    if (!state->canPlay())
        return;

    // Get clicked tiles coordinates
    std::tuple<int, int> clickedTile = View::GetInstance()->getClickedTile(event.button.x, event.button.y);
    int i = std::get<0>(clickedTile);
    int j = std::get<1>(clickedTile);

    // If we clicked somewhere not handled, do nothing
    if (i == -1)
        return;

    /* 
    if (pressed_i != i || pressed_j != j){
        state->getTileStates()[pressed_i][pressed_j] = Tile::State::Unclicked;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        pressed_i = i;
        pressed_j = j;
        state->getTileStates()[i][j] = Tile::State::Pressed;
    }  */
    
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        switch (event.button.button){
            case SDL_BUTTON_LEFT:
                // If tile is already clicked, do nothing
                if (state->getTileStates()[i][j] == Tile::State::Clicked)
                    return;

                if (state->getTileStates()[i][j] == Tile::State::Flagged) 
                    break;

                state->getTileStates()[i][j] = Tile::State::Clicked;
                
                switch(state->getTiles()[i][j])
                {
                    case Tile::Type::Unexploded_Bomb_Tile:
                        state->getTileStates()[i][j] = Tile::State::Exploded;
                        gameOver();
                        break;
                    case Tile::Type::Empty_Tile:
                        clickEmptyTiles(i, j);
                        break;
                    default:
                        break;
                } 
                break;
            case SDL_BUTTON_RIGHT:
                if (state->getTileStates()[i][j] == Tile::State::Unclicked)
                {
                    state->getTileStates()[i][j] = Tile::State::Flagged;
                }
                else if (state->getTileStates()[i][j] == Tile::State::Flagged)
                {
                    state->getTileStates()[i][j] = Tile::State::Unclicked;
                }
                break;
            case SDL_BUTTON_MIDDLE:
                if (state->getTileStates()[i][j] == Tile::State::Clicked)
                    clickSurroundingTiles(i, j);
                break;
            default:
                break;
        }
    }
}
