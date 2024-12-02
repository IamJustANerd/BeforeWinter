#include "../include/Nature.h"
#include "../include/Grid.h"
#include "../include/Mouse.h"
#include <iostream>

Nature::Nature(Vector2 _position, int _type, Grid *_grid)
{
    position = _position;
    type = _type;

    width = 128;
    height = 128;

    // Nature is uncollidable
    isUncollidable = true;

    // Assign some values based on the nature type
    if(type == 0) // Tree
    {
        hitBox = Rectangle{position.x + (float)width * 0.4f, position.y + (float)height * 0.8f, (float)width * 0.2f, (float)height / 8};
        
        healthPoint = 30;
    }
    else
    {
        hitBox = Rectangle{position.x, position.y, (float)width, (float)height};

        healthPoint = 10;
    }

    // The starting state is idle
    curState = State::idle;

    // Set the frame rec according to the current state
    frameRec = natureAnimation[type][(int)curState][0].sourceFrame;

    // Insert nature into the grid
    grid = _grid;
    grid->Add(this);
}

void Nature::Draw() const
{
    // Draw body
    DrawTextureRec(natureTex[type], frameRec, position, WHITE);

    // Draw hitbox
    if(!isCollidingWithMouse)
    {
        DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
    }
    // else
    // {
    //     DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
    // }
}

void Nature::Update()
{
    UpdateSpriteFrame();
}

void Nature::UpdateSpriteFrame()
{
    frameCounter += 1;
    if (frameCounter >= natureAnimation[type][(int)curState][0].frameTime / natureAnimation[type][(int)curState][0].totalFrames)
    {
        frameCounter = 0;

        frameRec.x = ((int)(frameRec.x + width) % (natureAnimation[type][(int)curState][0].totalFrames * width));

        // Check if this is a hit animation
        if (curState == State::hit)
        {
            // If it is, make sure to stop the hit animation once it reaches back to the first frame
            if (frameRec.x <= 0)
            {
                // Return back to idle animation
                curState = State::idle;

                frameRec = natureAnimation[type][(int)curState][0].sourceFrame;
            }
        }
    }
}

void Nature::HitAnimation()
{
    // Reset frame counter
    frameCounter = 0;

    // Change state into hit
    curState = State::hit;

    // Change frame
    frameRec = natureAnimation[type][(int)curState][0].sourceFrame;
}