#ifndef GRID_H
#define GRID_H

#include "Entity.h"

class Grid
{
public:
    Grid();

    static const int NUM_CELLS = 100;
    static const int CELL_SIZE = 128;

    // Add object to the grid
    void Add(Entity* entity);

    // Handle each cell in a grid
    void UpdateGrid();

    // Handle each object in a cell
    void HandleCell(Entity* entity);

    // Move object's cell if it crosses a cell's boundary lines
    void Move(Entity* entity, Vector2 addPos);

    // Handle player collision
    void HandlePlayerCollisions(Entity* entity);

    // Handle collectible collision
    void HandleCollectibleCollisions(Entity *entity);

    // private:
    Entity *cells[NUM_CELLS][NUM_CELLS];
};

#endif