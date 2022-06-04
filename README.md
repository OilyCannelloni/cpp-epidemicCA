# EpidemicCA
This repository contains the project made for AGH C++ language course.

## Abstract
The project enables to conduct a simulation of an epidemic using a Cellular Automatum (i. e. 
a grid of cells which interact with each other). The cells are divided into four types which define
their behavior:
- Susceptible - is prone to getting infected (does nothing)
- Infected - during each tick, picks a neighbour at random and if it is susceptible, makes it infected.
After doing that, it has a small chance of becoming Immune or Dead
- Immune - cannot be infected. During each tick has a small chance of losing its immunity and becoming susceptible
- Dead - cannot be infected (does nothing)

## Installation
```
git clone https://github.com/oilycannelloni/cpp-epidemicCA.git
cd cpp-epidemicCA
make
```

## Execution

```
./main 0.05 0.001 0.007
```

The three parameters are the following:
- Chance of recovery of infected cells
- Chance of death of infected cells
- Chance of losing immunity of Immune cells