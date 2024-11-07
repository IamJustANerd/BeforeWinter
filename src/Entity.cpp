#include "../include/Entity.h"
#include <iostream>

Rectangle Entity::FlipTexture(Rectangle frameRec) const
{
    return {frameRec.x + frameRec.width, frameRec.y, -frameRec.width, frameRec.height};
}

Vector2 Entity::GetPosition() const
{
    return position;
}

Vector2 Entity::GetDirection() const
{
    return direction;
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

bool Entity::GetIsUncollidable() const
{
    return isUncollidable;
}