//
// Created by oilymacaroni on 02.06.22.
//

#ifndef EPIDEMICCA_CONSTANTS_H
#define EPIDEMICCA_CONSTANTS_H

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

const int CANVAS_WIDTH = 1000;
const int CANVAS_HEIGHT = 760;

constexpr int PLOT_HEIGHT = SCREEN_HEIGHT - CANVAS_HEIGHT;

const int GRID_WIDTH = 50;
const int GRID_HEIGHT = 38;
constexpr int GRID_SIZE = GRID_HEIGHT * GRID_WIDTH;

constexpr int CELL_WIDTH = CANVAS_WIDTH / GRID_WIDTH;
constexpr int CELL_HEIGHT = CANVAS_HEIGHT / GRID_HEIGHT;

const double RECOVERY_CHANCE = 0.05;
const double DEATH_CHANCE = 0.002;
const double IMMUNITY_LOSS_CHANCE = 0.007;

enum CellType {
    SUSCEPTIBLE,
    INFECTED,
    IMMUNE,
    DEAD
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

#endif //EPIDEMICCA_CONSTANTS_H
