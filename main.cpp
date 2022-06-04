#include "constants.h"
#include "Cell.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>


bool init();
void close();

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

Cell grid[GRID_WIDTH][GRID_HEIGHT];
int updateOrder[GRID_HEIGHT * GRID_WIDTH];

struct Stats {
    int epoch;
    int count[4];
};

bool init() {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y].initialize(x, y);
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << SDL_GetError();
        return false;
    }

    if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" )) {
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    gWindow = SDL_CreateWindow(
            "Epidemic Simulation",
            2000,
            0,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
            );

    if (gWindow == nullptr) {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if(gRenderer == nullptr) {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    return true;
}

void close()
{
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gRenderer = nullptr;

    SDL_Quit();
}

void clearWindow() {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
}

void drawGrid() {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            SDL_Rect* rect = grid[x][y].getRect();
            SDL_Color* color = grid[x][y].getColor();

            SDL_SetRenderDrawColor(gRenderer, color->r, color->g, color->b, color->a);
            SDL_RenderFillRect(gRenderer, rect);
        }
    }

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    for (int x = CELL_WIDTH; x <= CANVAS_WIDTH - CELL_WIDTH; x += CELL_WIDTH) {
        SDL_RenderDrawLine(gRenderer, x, 0, x, CANVAS_HEIGHT);
    }

    for (int y = CELL_HEIGHT; y <= CANVAS_HEIGHT - CELL_HEIGHT; y += CELL_HEIGHT) {
        SDL_RenderDrawLine(gRenderer, 0, y, CANVAS_WIDTH, y);
    }

    SDL_RenderPresent(gRenderer);
}

void initializeGrid() {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            if (y > 0) grid[x][y].setNeighbour(&grid[x][y-1], UP);
            if (x < GRID_WIDTH - 1) grid[x][y].setNeighbour(&grid[x+1][y], RIGHT);
            if (y < GRID_HEIGHT - 1) grid[x][y].setNeighbour(&grid[x][y+1], DOWN);
            if (x > 0) grid[x][y].setNeighbour(&grid[x-1][y], LEFT);
        }
    }

    for (int i = 0; i < GRID_HEIGHT * GRID_WIDTH; i++) {
        updateOrder[i] = i;
    }
}

void initializeSimulation() {
    int x = rand() % GRID_WIDTH;
    int y = rand() % GRID_HEIGHT;
    grid[x][y].setType(INFECTED);
}

void shuffle(int arr[], int length) {
    for (int i = 0; i < length; i++) {
        int t = (rand() % (length - i)) + i;
        std::swap(arr[i], arr[t]);
    }
}

int main(int argc, char* args[]) {
    if (argc < 4) return 0;
    Cell::RECOVERY_CHANCE = strtod(args[1], nullptr);
    Cell::DEATH_CHANCE = strtod(args[2], nullptr);
    Cell::IMMUNITY_LOSS_CHANCE = strtod(args[3], nullptr);

    srand(time(nullptr));

    if (!init()) {
        printf( "Failed to initialize!\n" );
        close();
        return 0;
    }

    initializeGrid();
    initializeSimulation();

    bool quit = false;
    SDL_Event e;
    int epoch = 0;
    std::list<Stats> stats = {};

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Tick cells
        for (int i = 0; i < GRID_SIZE; i++) {
            grid[i % GRID_WIDTH][i / GRID_WIDTH].clearUpdated();
        }
        shuffle(updateOrder, GRID_SIZE);
        for (int i = 0; i < GRID_SIZE; i++) {
            Cell* cell = &grid[i % GRID_WIDTH][i / GRID_WIDTH];
            if (cell->wasUpdated()) continue;
            cell->onTick();
        }

        // Count cells of each type
        Stats epochStats = {
                epoch++,
                {0, 0, 0, 0}
        };
        for (int i = 0; i < GRID_SIZE; i++) {
            epochStats.count[grid[i % GRID_WIDTH][i / GRID_WIDTH].getType()]++;
        }
        stats.push_back(epochStats);
        if (epoch > SCREEN_WIDTH)
            stats.pop_front();

        // Plot
        clearWindow();
        int x = 0;
        for (Stats stat : stats) {
            int y = CANVAS_HEIGHT;
            for (int type = 0; type < 4; type++) {
                int delta = (int) ((double) stat.count[type] / GRID_SIZE * PLOT_HEIGHT) + 1;
                SDL_Color color = Cell::getColorBtType(type);
                SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
                SDL_RenderDrawLine(gRenderer, x, y, x, y + delta);
                y += delta;
            }
            x++;
        }


        SDL_SetWindowTitle(gWindow, std::to_string(epoch).c_str());
        drawGrid();
        SDL_Delay(20);
    }


    //Free resources and close SDL
    close();

    return 0;
}