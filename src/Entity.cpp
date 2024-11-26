#include "../include/Entity.h"
#include <iostream>

Rectangle Entity::FlipTexture(Rectangle frameRec) const
{
    return {frameRec.x, frameRec.y, -frameRec.width, frameRec.height};
}

Vector2 Entity::GetPosition() const
{
    return position;
}

Vector2 Entity::GetDirection() const
{
    return direction;
}

Vector2 Entity::GetHitBoxPosition() const
{
    return {hitBox.x + width / 2, hitBox.y + height / 2};
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

int Entity::GetType() const
{
    return type;
}

Rectangle Entity::GetHitBox() const
{
    return hitBox;
}

bool Entity::GetIsUncollidable() const
{
    return isUncollidable;
}