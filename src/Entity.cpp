#include "../include/Entity.h"
#include <iostream>

Vector2 Entity::GetPosition() const
{
    return position;
}

int Entity::GetWidth() const
{
    return width;
}

int Entity::GetHeight() const
{
    return height;
}

int Entity::GetID() const
{
    return id;
}

Rectangle Entity::GetHitBox() const
{
    return hitBox;
}