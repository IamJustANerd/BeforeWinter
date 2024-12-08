#include "../include/Entity.h"
#include "Grid.h"
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
    return {(float)hitBox.x + hitBox.width / 2, (float)hitBox.y + hitBox.height / 2};
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

bool Entity::GetIsTargeted() const
{
    return isTargeted;
}

void Entity::SetIsTargeted(bool newState) 
{
    isTargeted = newState;
}

void Entity::ReduceHealthPoint(int decrease)
{
    healthPoint -= decrease;
}

void Entity::DeathAnimation(int deathType)
{
    frameCounter += 1;
    if (frameCounter >= deathAnimation[deathType][0].frameTime / deathAnimation[deathType][0].totalFrames)
    {
        curFrame = (curFrame + 1) % deathAnimation[deathType][0].totalFrames;

        frameCounter = 0;

        frameRec.x = ((int)(frameRec.x + width) % (int)(deathAnimation[deathType][0].totalFrames * width));

        // Enter decay animation once the death animation is done
        if(curFrame == 0)
        {
            isDecay = true;
            curState = State::decaying;
            frameRec = deathAnimation[deathType][1].sourceFrame;
        }

    }
}

void Entity::DecayAnimation(int decayType)
{
    frameCounter += 1;
    if (frameCounter >= deathAnimation[decayType][1].frameTime / deathAnimation[decayType][1].totalFrames)
    {
        curFrame = (curFrame + 1) % deathAnimation[decayType][1].totalFrames;

        frameCounter = 0;

        frameRec.x = ((int)(frameRec.x + width) % (int)(deathAnimation[decayType][1].totalFrames * width));

        // After the decay animation is done, delete this entity
        if (curFrame == 0)
        {
            // Remove it from the grid
            grid->Remove(this);
        
            // Delete it completely to free space
            delete(this);
        }
    }
}

bool Entity::GetIsAlive() const
{
    return isAlive;
}