#include "../include/Collectible.h"
#include "../include/raymath.h"
#include <cmath>
#include <iostream>

Collectible::Collectible(Vector2 _position, int _type)
{
    position = _position;
    type = _type;
    width = 16, height = 16;
    hitBox = Rectangle{position.x, position.y, (float)width, (float)height};
    // speed = 10;
}

void Collectible::Movements()
{
    // Calculate direction and rotation to make collectible move to player
    direction = {playerPos.x - position.x, playerPos.y - position.y};
    rotation = atan2f(direction.y, direction.x) * RAD2DEG;

    // Movement
    direction.x = cosf(rotation * DEG2RAD) * speed;
    direction.y = sinf(rotation * DEG2RAD) * speed;

    position = Vector2Add(position, direction);
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
    DrawRectangleRec(hitBox, ORANGE);
}

void Collectible::UpdatePlayerPosition(Vector2 _playerPos)
{
    playerPos = _playerPos;
}