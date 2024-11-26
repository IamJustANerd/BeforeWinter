#include "../include/NPC.h"
#include <iostream>
#include <typeinfo>

NPC::NPC(Vector2 _position, int _type, Player *_NPC, Grid *_grid)
{
    position = _position;

    width = 128, height = 128;

    id = 0;

    type = _type;

    // Assign NPC hitbox
    hitBox = Rectangle{position.x + (float)width / 3, position.y + (float)height * 0.6f, (float)width / 3, (float)height / 8};

    targetPlayer = _NPC;

    speed = 2.0f;

    rotation = 0;

    isAttacking = false;

    isWorking = false;

    isCarrying = false;

    hasDestination = false;

    // The starting state is idle
    curState = State::idle;

    // Set the frame rec according to the current state
    frameRec = NPCAnimation[type][(int)curState][0].sourceFrame;

    // NPC is uncollidable
    isUncollidable = true;

    // Set starting direction as neutral right
    direction = Vector2({1.0f, 0.0f});

    // Insert NPC into the grid
    grid = _grid;
    grid->Add(this);
}

void NPC::Movements()
{
    // Can only move if NPC is not working
    if (isWorking)
    {
        return;
    }

    // Variables for calculating cell changes
    Vector2 change = {0, 0};

    // Border for NPC movement to prevent NPC from moving out of the grid
    int minBorderX = grid->CELL_SIZE;
    int maxBorderX = (grid->NUM_CELLS - 1) * grid->CELL_SIZE;
    int minBorderY = grid->CELL_SIZE;
    int maxBorderY = (grid->NUM_CELLS - 1) * grid->CELL_SIZE;

    // To check if the NPC is moving or not (will be set as true if the NPC moves in any direction)
    isMoving = false;

    // Left movement
    if (hitBox.x > destination.x)
    {
        // std::cout << "LEFT: " << position.x << " " << destination.x << '\n';
        int i = 0;

        while ((i < speed && hitBox.x > minBorderX) &&
               !IsCollidingWithUncollidable("Left"))
        {
            i += 1;

            // Update NPC position (in the grid as well)
            position.x -= 1;
            change.x -= 1;

            // Update hitBox position
            hitBox.x = position.x + (float)width / 3;
        }

        // Update NPC direction
        direction.x = -1.0f;
        direction.y = 0.0f;

        isMoving = true;
    }
    // Right movement
    else if (hitBox.x < destination.x)
    {
        // std::cout << "RIGHT: " << position.x - destination.x << '\n';
        int i = 0;
        while ((i < speed && (hitBox.x + hitBox.width) < maxBorderX) &&
               !IsCollidingWithUncollidable("Right"))
        {
            i += 1;

            // Update NPC position (in the grid as well)
            position.x += 1;
            change.x += 1;

            // Update hitBox position
            hitBox.x = position.x + (float)width / 3;
        }

        // Update NPC direction
        direction.x = 1.0f;
        direction.y = 0.0f;

        isMoving = true;
    }

    // Up movement
    if (hitBox.y - destination.y > 5)
    {
        // std::cout << "UP: " << position.y - destination.y << '\n';
        int i = 0;
        while ((i < speed && hitBox.y > minBorderY) &&
               !IsCollidingWithUncollidable("Up"))
        {
            i += 1;

            // Update NPC position (in the grid as well)
            position.y -= 1;
            change.y -= 1;

            // Update hitBox position
            hitBox.y = position.y + (float)height * 0.6f;
        }

        // Update NPC direction
        direction.y = -1.0f;

        isMoving = true;
    }
    // Down movement
    else if (hitBox.y - destination.y < 5)
    {
        // std::cout << "DOWN: " << position.y - destination.y << '\n';
        int i = 0;
        while ((i < speed && (hitBox.y + hitBox.height) < maxBorderY) &&
               !IsCollidingWithUncollidable("Down"))
        {
            i += 1;

            // Update NPC position (in the grid as well)
            position.y += 1;
            change.y += 1;

            // Update hitBox position
            hitBox.y = position.y + (float)height * 0.6f;
        }

        // Update NPC direction
        direction.y = 1.0f;

        isMoving = true;
    }

    // Switch to idle animation
    // if (!isMoving && curState != State::idle)
    // {
    //     curState = State::idle;
    //     frameRec = frameRec = NPCAnimation[type][(int)curState][0].sourceFrame;

    //     // Reset frame counter
    //     frameCounter = 0;
    // }

    // Update NPC's cell
    grid->Move(this, change);
}

void NPC::Draw() const
{
    // Draw body
    DrawRectangle(position.x, position.y, width, height, {230, 41, 55, 128});

    // Draw texture
    if (direction.x >= 0)
    {
        DrawTextureRec(NPCTex[0], frameRec, position, WHITE);
    }
    else if (direction.x <= -1)
    {
        DrawTextureRec(NPCTex[0], FlipTexture(frameRec), {position.x, position.y}, WHITE);
    }

    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
}

int x = 0;

void NPC::Update()
{
    SetDestination();

    Movements();

    Attack();

    UpdateSpriteFrame();
}

void NPC::UpdateSpriteFrame()
{
    frameCounter += 1;
    if (frameCounter >= NPCAnimation[type][(int)curState][0].frameTime / NPCAnimation[type][(int)curState][0].totalFrames)
    {
        frameCounter = 0;

        frameRec.x = ((int)(frameRec.x + width) % (int)(NPCAnimation[type][(int)curState][0].totalFrames * width));

        // Check if this is an attack animation
        // if (curState == State::light_attacking || curState == State::heavy_attacking)
        // {
        //     // If it is, make sure to stop the attack animation once it reaches back to the first frame
        //     if (frameRec.x <= 0)
        //     {
        //         // Return back to idle animation
        //         isAttacking = false;

        //         curState = State::idle;

        //         frameRec = NPCAnimation[type][(int)curState][0].sourceFrame;
        //     }
        // }
    }
}

void NPC::Attack()
{

}

void NPC::SetDestination()
{
    // If the pawn is not working, then it should be moving
    if (!isWorking && !hasDestination)
    {
        hasDestination = true;

        // Return harvest to home
        if (isCarrying)
        {

        }
        // Look for the closest tree
        else
        {
            // std::cout << "POHON" << '\n';
            destination = FindTarget(typeid(Nature), 0);
            std::cout << FindTarget(typeid(Nature), 0).x << ' ' << FindTarget(typeid(Nature), 0).y << '\n';
            // std::cout << destination.x << ' ' << destination.y << '\n';
        }
    }
}