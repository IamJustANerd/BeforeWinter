#include "../include/Building.h"
#include "../include/Grid.h"
#include "../include/Mouse.h"
#include <iostream>

Building::Building(Vector2 _position, int _type, Grid *_grid)
{
    position = _position;
    type = _type;

    width = 128;
    height = 128;

    // Building is uncollidable
    isUncollidable = true;

    // Assign hitbox (the size depends on the type)
    if (type == 0) // House
    {
        hitBox = Rectangle{position.x + (float)width * 0.1f, position.y + (float)height * 0.6f, (float)width * 0.8f, (float)height * 0.7f};
    }
    else
    {
        hitBox = Rectangle{position.x, position.y, (float)width, (float)height};
    }

    // Insert building into the grid
    grid = _grid;
    grid->Add(this);
}

void Building::Draw() const
{
    // Draw body
    DrawTexture(buildingTex[type], position.x, position.y, WHITE);

    // Draw hitbox
    if (!isCollidingWithMouse)
    {
        DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
    }
}

void Building::Update()
{

}

void Building::UpdateSpriteFrame()
{
    // Building has no animation
}