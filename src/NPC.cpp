#include "../include/NPC.h"
#include "../include/GameManager.h"
#include <iostream>
#include <typeinfo>

void NPC::Movements()
{
    // If there is no more target around, stop moving
    if (target == NULL)
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

    bool alreadyMoveToXDirection = false;
    bool alreadyMoveToYDirection = false;

    // If there is no obstacle around, keep moving towards destination
    if (!isCollidingWithOther)
    {
        // Priority movement: To prevent NPC from going back into the spot that will block the NPC from going to it's destination,
        // everytime Pledge's Algorithm is finished, we will move it to the moveDirection first before handling other movement direction.
        // Also, make sure to disable the movement of the same axis after that
        if (priorityMove)
        {
            priorityMove = false;

            if (moveDirectionIndex == 0 || moveDirectionIndex == 2)
            {
                alreadyMoveToXDirection = true;
            }
            else if (moveDirectionIndex == 1 || moveDirectionIndex == 3)
            {
                alreadyMoveToYDirection = true;
            }

            int i = 0;

            while ((i < speed && (hitBox.y + hitBox.height) < maxBorderY &&
                    (hitBox.x + hitBox.width) < maxBorderX &&
                    (hitBox.y > minBorderY && hitBox.x > minBorderX)) &&
                   !IsCollidingWithUncollidable())
            {

                i += 1;

                // Update NPC position (in the grid as well)
                position.x += moveDirection[moveDirectionIndex].x;
                position.y += moveDirection[moveDirectionIndex].y;
                change.x += moveDirection[moveDirectionIndex].x;
                change.y += moveDirection[moveDirectionIndex].y;

                // Update hitBox position
                hitBox.x = position.x + (float)width / 3;
                hitBox.y = position.y + (float)height * 0.6f;
            }
        }

        // Left movement
        if (hitBox.x - destination.x >= destination.width && !isCollidingWithOther && !alreadyMoveToXDirection)
        {
            moveDirectionIndex = 0;

            int i = 0;
            isCollidingWithOther = IsCollidingWithUncollidable();

            while ((i < speed && hitBox.x > minBorderX) &&
                   !isCollidingWithOther)
            {
                i += 1;
                isCollidingWithOther = IsCollidingWithUncollidable();

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
        else if (destination.x - hitBox.x >= hitBox.width && !isCollidingWithOther && !alreadyMoveToXDirection)
        {
            moveDirectionIndex = 2;

            int i = 0;
            isCollidingWithOther = IsCollidingWithUncollidable();

            while ((i < speed && (hitBox.x + hitBox.width) < maxBorderX) &&
                   !isCollidingWithOther)
            {
                i += 1;
                isCollidingWithOther = IsCollidingWithUncollidable();

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
        if (hitBox.y - destination.y >= destination.height && !isCollidingWithOther && !alreadyMoveToYDirection)
        {
            moveDirectionIndex = 1;

            int i = 0;
            isCollidingWithOther = IsCollidingWithUncollidable();

            while ((i < speed && hitBox.y > minBorderY) &&
                   !isCollidingWithOther)
            {
                i += 1;
                isCollidingWithOther = IsCollidingWithUncollidable();

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
        else if (destination.y - hitBox.y >= hitBox.height && !isCollidingWithOther && !alreadyMoveToYDirection)
        {
            moveDirectionIndex = 3;

            int i = 0;
            isCollidingWithOther = IsCollidingWithUncollidable();

            while ((i < speed && (hitBox.y + hitBox.height) < maxBorderY) &&
                   !isCollidingWithOther)
            {
                i += 1;
                isCollidingWithOther = IsCollidingWithUncollidable();

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

        if (isCollidingWithOther)
        {
            rotationCounter = 0;
        }
    }
    // If the NPC collide with obstacle along the way, enter the Pledge's Algorithm
    else
    {
        // If it is colliding with obstacle, rotate 90 degree (clockwise)
        if (IsCollidingWithUncollidable())
        {
            moveDirectionIndex = (moveDirectionIndex + 1) % 4;
            rotationCounter += 1;
        }

        // Follow the obstacle's edge
        int i = 0;

        while ((i < speed && (hitBox.y + hitBox.height) < maxBorderY &&
                (hitBox.x + hitBox.width) < maxBorderX &&
                (hitBox.y > minBorderY && hitBox.x > minBorderX)) &&
               !IsCollidingWithUncollidable())
        {
            i += 1;

            // Update NPC position (in the grid as well)
            position.x += moveDirection[moveDirectionIndex].x;
            position.y += moveDirection[moveDirectionIndex].y;
            change.x += moveDirection[moveDirectionIndex].x;
            change.y += moveDirection[moveDirectionIndex].y;

            // Update hitBox position
            hitBox.x = position.x + (float)width / 3;
            hitBox.y = position.y + (float)height * 0.6f;
        }

        // Change NPC facing direction (for animation)
        if (moveDirectionIndex == 0)
        {
            direction.x = -1.0f;
            direction.y = 0.0f;
        }
        else if (moveDirectionIndex == 1)
        {
            direction.x = 1.0f;
            direction.y = 0.0f;
        }
        else if (moveDirectionIndex == 2)
        {
            direction.y = -1.0f;
        }
        else if (moveDirectionIndex == 3)
        {
            direction.y = 1.0f;
        }

        // If the obstacle's edge is no longer there, rotate 90 degree to left (counterclockwise)
        // Temporarily change the moveDirectionIndex to check collision
        moveDirectionIndex = (moveDirectionIndex - 1 + 4) % 4;
        if (!IsCollidingWithUncollidable())
        {
            rotationCounter -= 1;
        }
        else
        {
            // Return to initial direction (this effect will only take place if the obstacle's edge is no more)
            moveDirectionIndex = (moveDirectionIndex + 1) % 4;
        }

        // If the rotationCounter reach 0, it means that NPC can resume their way to their destination
        if (rotationCounter == 0)
        {
            isCollidingWithOther = false;
            priorityMove = true;
        }

        // To handle a case where NPC is colliding with dynamic entities (which has a chance to cause a bug),
        // implement a fallback mechanism that will make NPC continue back to their destination
        if (abs(rotationCounter) >= rotationCounterTolerance)
        {
            rotationCounter = 0;
            isCollidingWithOther = false;
        }
    }

    // Update Pawn's cell
    grid->Move(this, change);
}

bool NPC::ReachDestination()
{
    return CheckCollisionRecs({hitBox.x - distanceTolerance, hitBox.y - distanceTolerance, hitBox.width + distanceTolerance * 2, hitBox.height + distanceTolerance * 2}, destination);
}

void NPC::ChangeAnimation(State newState)
{
    // Will be implemented by the child
}