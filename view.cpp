#include "view.h"

View* View::instance = nullptr;

int View::init()
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

    state = State::getInstance();

    buttonAtlas = loadTexture("buttons.bmp");
    boardTexture = loadTexture("board.bmp");

    getTexturePositions();
    getDrawPositions();

    return 1;
}

SDL_Texture* View::loadTexture(const std::string& file)
{
    SDL_Surface* image = SDL_LoadBMP(file.c_str());  
    if (image == nullptr)
    {
        std::cerr << "Error loading image! " << SDL_GetError() << std::endl;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
	if (texture == nullptr){
		std::cerr << "Error loading texture! " << SDL_GetError() << std::endl;
	}

    SDL_FreeSurface(image);
	return texture;
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

void View::drawScreen()
{
    drawBoard();

    std::vector<std::vector<int>> tiles = state->getTiles();
    for (size_t i = 0; i < tiles.size(); i++)
    {
        for (size_t j = 0; j < tiles[0].size(); j++)
        {
            if (state->getTileStates()[i][j] == TileState::Unclicked)
            {
                drawTile(textures[TileType::Unclicked_Tile], positions[i][j]);
                continue;
            } 

            if (state->getTileStates()[i][j] == TileState::Flagged)
            {
                drawTile(textures[TileType::Flag_Tile], positions[i][j]);
                continue;
            }
            else {
                drawTile(textures[tiles[i][j]], positions[i][j]);
            }
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1000/FPS);
}

void View::drawBoard()
{
    SDL_Rect rect = {(int) 0, (int) 0};
    rect.w = WIDTH;
    rect.h = HEIGHT;
    SDL_RenderCopy(renderer, boardTexture, NULL, &rect);
}

void View::drawUI()
{

}

void View::drawTile(SDL_Rect clip, SDL_Rect pos) 
{
    SDL_RenderCopy(renderer, buttonAtlas, &clip, &pos);
}

void View::getTexturePositions()
{
    textures[TileType::Empty_Tile] = {(int) TILE_TEX_SIZE, (int) 48, TILE_TEX_SIZE, TILE_TEX_SIZE};
    textures[TileType::Bomb_Tile] = {(int) 84, (int) 48, TILE_TEX_SIZE, TILE_TEX_SIZE};
    textures[TileType::Unclicked_Tile] = {(int) 0, (int) 48, TILE_TEX_SIZE, TILE_TEX_SIZE};
    textures[TileType::Flag_Tile] = {(int) 2*TILE_TEX_SIZE, (int) 48, TILE_TEX_SIZE, TILE_TEX_SIZE};

    for (int i = 1; i < 9; i++)
    {
        textures[i] = {(int) (i-1) * TILE_TEX_SIZE, (int) 65, TILE_TEX_SIZE, TILE_TEX_SIZE};
    }
}

void View::getDrawPositions()
{
    // Where to start drawing the tiles from
    int startX = 15;
    int startY = 70;

    // Set tile size
    SDL_Rect pos;
    pos.w = TILE_SIZE;
    pos.h = TILE_SIZE;

    std::vector<SDL_Rect> row;

    std::vector<std::vector<int>> tiles = state->getTiles();
    for (size_t i = 0; i < tiles.size(); i++)
    {
        row.clear();
        for (size_t j = 0; j < tiles[0].size(); j++)
        {
            pos.x = startX + j*TILE_SIZE;
            pos.y = startY + i*TILE_SIZE;
            row.push_back(pos);
        }
        positions.push_back(row);
    }
}

std::tuple<int, int> View::getClickedTile(int x, int y)
{
    for (size_t i = 0; i < positions.size(); i++)
    {
        for (size_t j = 0; j < positions[0].size(); j++)
        {
            if (x >= positions[i][j].x &&
                x <= positions[i][j].x + TILE_SIZE &&
                y >= positions[i][j].y &&
                y <= positions[i][j].y + TILE_SIZE)
                {
                  return std::tuple<int, int>(i, j);  
                }
        }
    }
    return std::tuple<int, int>(-1, -1);
}

void View::gameOverScreen()
{
    SDL_ShowSimpleMessageBox(0, "Game Over", "You lost! :(", window);
}

void View::destroy()
{
    /** Release resources */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete instance;
}