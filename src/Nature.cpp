#include "../include/Nature.h"
#include "../include/Grid.h"
#include <iostream>

Nature::Nature(Vector2 _position, int _id, Texture2D* _textures, Grid* _grid)
{
    position = _position;
    id = _id;
    textures = _textures;

    width = textures[id].width;
    height = textures[id].height;

    // Nature is uncollidable
    isUncollidable = true;

    // Testing hitbox
    hitBox = Rectangle{position.x, position.y, (float)width, (float)height};

    // Insert nature into the grid
    grid = _grid;
    grid->Add(this);
}

void Nature::Draw() const
{
    // Draw body
    DrawTexture(textures[id], position.x, position.y, WHITE);

    // Draw hitbox
    if(isCollidingWithMouse)
    {
        DrawRectangleRec(hitBox, Color{230, 41, 55, 120});
    }
    else
    {
        DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
    }
}

void Nature::Update()
{
    
}