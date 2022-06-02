//
// Created by oilymacaroni on 02.06.22.
//

#ifndef EPIDEMICCA_CELL_H
#define EPIDEMICCA_CELL_H

#include <SDL2/SDL.h>
#include "constants.h"

class Cell {
private:
    int _x{0}, _y{0};
    bool updated{false};
    SDL_Rect rectangle{};
    SDL_Color color{};
    CellType type{SUSCEPTIBLE};
    Cell* neighbours[4] = {nullptr, nullptr, nullptr, nullptr};

public:
    Cell() = default;
    void initialize(int x, int y);

    void setColor(SDL_Color c) {
        color = c;
    }

    SDL_Color* getColor() {
        return &color;
    }

    void setColorByType();

    static SDL_Color getColorBtType(int t);

    SDL_Rect* getRect() {
        return &rectangle;
    }

    void setType(CellType t);

    CellType getType() {
        return type;
    }

    void setNeighbour(Cell* cell, int dir) {
        neighbours[dir] = cell;
    }

    Cell* getNeighbour(int dir) {
        return neighbours[dir];
    }

    bool wasUpdated() {
        return updated;
    }

    void setUpdated() {
        this->updated = true;
    }

    void clearUpdated() {
        this->updated = false;
    }

    void onTick();
};


#endif //EPIDEMICCA_CELL_H
