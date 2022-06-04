#include <stdlib.h>
#include <iostream>

#include "Cell.h"
#include "constants.h"

double Cell::RECOVERY_CHANCE, Cell::DEATH_CHANCE, Cell::IMMUNITY_LOSS_CHANCE;

void Cell::initialize(int x, int y) {
    _x = x;
    _y = y;
    this->rectangle = {
            x * CELL_WIDTH,
            y * CELL_HEIGHT,
            CELL_WIDTH,
            CELL_HEIGHT
    };

    setType(SUSCEPTIBLE);
}

void Cell::setColorByType() {
    setColor(getColorBtType(type));
}

void Cell::setType(CellType t) {
    type = t;
    setColorByType();
}

void Cell::onTick() {
    if (this->wasUpdated()) return;
    this->setUpdated();
    
    double seed = (double) rand() / RAND_MAX;
    switch (type) {
        case DEAD:
        case SUSCEPTIBLE:
            break;
        case IMMUNE:
            if (seed < IMMUNITY_LOSS_CHANCE)
                this->setType(SUSCEPTIBLE);
            break;
        case INFECTED:
            int randomDir = rand() % 4;
            Cell* target = this->getNeighbour(randomDir);
            if (target != nullptr && target->getType() == SUSCEPTIBLE)
                target->setType(INFECTED);
            
            if (seed < RECOVERY_CHANCE)
                this->setType(IMMUNE);
            
            if (seed < DEATH_CHANCE)
                this->setType(DEAD);

            break;
    }
}

SDL_Color Cell::getColorBtType(int t) {
    switch (t) {
        case SUSCEPTIBLE:
            return {0xFF, 0xFF, 0xFF, 0xFF};
        case INFECTED:
            return {0xFF, 0x55, 0x55, 0xFF};
        case IMMUNE:
            return {0xAA, 0xAA, 0xFF, 0xFF};
        case DEAD:
            return {0x00, 0x00, 0x00, 0xFF};
        default:
            return {0xDD, 0xDD, 0xDD, 0xFF};
    };
}

