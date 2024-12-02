#include "../include/Collectible.h"
#include "../include/Grid.h"
#include <cmath>
#include <iostream>

Collectible::Collectible(Vector2 _position, int _id, Grid* _grid)
{
    position = _position;
    id = _id;
    width = 16, height = 16;
    hitBox = Rectangle{position.x, position.y, (float)width, (float)height};
    // speed = 10;

    // Collectible is not uncollidable
    isUncollidable = false;

    // Insert collectible into the grid
    grid = _grid;
    grid->Add(this);
}

void Collectible::Movements()
{
    // Calculate direction and rotation to make collectible move to player
    direction = {(playerPos.x - position.x), (playerPos.y - position.y)};
    rotation = atan2f(direction.y, direction.x) * RAD2DEG;

    // Movement
    direction.x = cosf(rotation * DEG2RAD) * speed;
    direction.y = sinf(rotation * DEG2RAD) * speed;

    // Update collectible's position
    position = Vector2Add(position, direction);

    // Update collectible position in the grid
    grid->Move(this, direction);
}

void Collectible::Update()
{
    // If within radius, increase speed over time up to max speed limit
    if(withinRadius)
    {
        // Increase movement speed over time
        if (speed < maxSpeed)
        {
            speed += acceleration;
        }
    }
    // If not within radius, decrease speed over time down to 0
    else if(speed > 0)
    {
        speed -= acceleration;

        if(speed < 0)
        {
            speed = 0;
        }
    }

    // Move
    Movements();

    // Update new position for hitbox
    hitBox.x = position.x;
    hitBox.y = position.y;
}

void Collectible::Draw() const
{
    // Draw body
    DrawRectangleRec(hitBox, ORANGE);

    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
}

void Collectible::UpdatePlayerPosition(Vector2 _playerPos)
{
    playerPos = _playerPos;
}

void Collectible::UpdateSpriteFrame()
{
    
}

void Collectible::Attack()
{

}

void Collectible::HitAnimation()
{
    
}