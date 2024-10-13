#ifndef GRID_H
#define GRID_H

#include "Entity.h"

class Grid
{
public:
    Grid();

    static const int NUM_CELLS = 100;
    static const int CELL_SIZE = 128;

    // Return read only cells
    const Entity *const (&GetReadOnlyCells() const)[NUM_CELLS][NUM_CELLS];

    // Add object to the grid
    void Add(Entity* entity);

    // Handle each cell in a grid
    void UpdateGrid();

    // Handle each object in a cell
    void HandleCell(Entity* entity);

    // Move object's cell if it crosses a cell's boundary lines
    void Move(Entity* entity, Vector2 addPos);

    // Handle player
    void HandlePlayer(Entity* entity);

    // Handle collectible
    void HandleCollectible(Entity *entity);

    // Draw objects visible to player
    void DrawVisibleObjects(Vector2 cameraPos);

    // Draw outlined objects visible to player
    void DrawOutlinedObjects(Vector2 cameraPos);

    // Handle mouse
    void HandleMouse();

private:
    Entity *cells[NUM_CELLS][NUM_CELLS];
};

#endif