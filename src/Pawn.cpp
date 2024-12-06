#include "../include/Pawn.h"
#include "../include/GameManager.h"
#include <iostream>
#include <typeinfo>

Pawn::Pawn(Vector2 _position, int _type, Grid *_grid)
{
    position = _position;

    width = 128, height = 128;

    type = _type;

    // Assign NPC hitbox
    hitBox = Rectangle{position.x + (float)width / 3, position.y + (float)height * 0.6f, (float)width / 3, (float)height / 8};

    speed = 2.0f;

    rotation = 0;

    isAttacking = false;

    isWorking = false;

    isCarrying = false;

    hasDestination = false;

    isCollidingWithOther = false;

    priorityMove = false;

    rotationCounter = 0;

    // The starting state is idle
    ChangeAnimation(State::idle);

    // NPC is uncollidable
    isUncollidable = true;

    // Set starting direction as neutral right
    direction = Vector2({1.0f, 0.0f});
    moveDirectionIndex = 2;

    // Insert NPC into the grid
    grid = _grid;
    grid->Add(this);
}

void Pawn::Movements()
{
    // Pawn can't move if it is working
    if (isWorking)
    {
        return;
    }

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

    // Switch to running animation
    if (isMoving && curState != State::running)
    {
        ChangeAnimation(State::running);
    }

    // If the NPC already reach the destination, stop moving
    if (ReachDestination())
    {
        isWorking = true;

        ChangeAnimation(State::idle);
    }

    // Update NPC's cell
    grid->Move(this, change);
}

void Pawn::Draw() const
{
    // Draw texture
    if (direction.x >= 0)
    {
        DrawTextureRec(NPCTex[0], frameRec, position, WHITE);
    }
    else if (direction.x <= -1)
    {
        DrawTextureRec(NPCTex[0], FlipTexture(frameRec), {position.x, position.y}, WHITE);
    }

    // Draw resource (if carrying any)
    if(isCarrying)
    {
        DrawTexture(resourceTex[0], position.x + resourceTex[0].width / 5, position.y - resourceTex[0].height / 5, WHITE);
        DrawTexture(resourceTex[0], position.x + resourceTex[0].width / 5 + 8, position.y - resourceTex[0].height / 5, WHITE);
        DrawTexture(resourceTex[0], position.x + resourceTex[0].width / 5 + 4, position.y - resourceTex[0].height / 5 - 8, WHITE);
    }

    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
}

void Pawn::Update()
{
    SetDestination();

    // Can only move if the NPC is not working
    if(!isWorking)
    {
        Movements();
    }

    Attack();

    Submit();

    UpdateSpriteFrame();
}

void Pawn::UpdateSpriteFrame()
{
    frameCounter += 1;
    // Change frame
    if (frameCounter >= NPCAnimation[type][(int)curState][0].frameTime / NPCAnimation[type][(int)curState][0].totalFrames)
    {
        curFrame = (curFrame + 1) % NPCAnimation[type][(int)curState][0].totalFrames;

        frameCounter = 0;

        frameRec.x = ((int)(frameRec.x + width) % (int)(NPCAnimation[type][(int)curState][0].totalFrames * width));

        // Check if this is an attack animation
        if (curState == State::light_attacking || curState == State::heavy_attacking)
        {
            // If it is, make sure to stop the attack animation once it reaches back to the first frame
            if (frameRec.x <= 0)
            {
                // Return back to idle animation
                isAttacking = false;

                ChangeAnimation(State::idle);
            }

            // If the pawn reached the fourth frame of heavy attack animation, make sure to:
            // 1. Give the pawn one carry,
            // 2. Reduce target health,
            // 3. Trigger the hit animation (if it has no more health, go for death animation instead)
            if(curFrame == 4)
            {
                carry += attackPoint;

                target->ReduceHealthPoint(attackPoint);

                target->ChangeAnimation(State::hit);

                // If the pawn reached the maximum amount of value it can carry, stop working
                if(carry >= maxCarry)
                {
                    isWorking = false;

                    isCarrying = true;

                    isAttacking = false;

                    // Reset the destination
                    hasDestination = false;

                    // Unmark this tree
                    target->SetIsTargeted(false);
                }
            }
        }
    }
}

void Pawn::Submit()
{
    // If NPC Reaches house, return the harvested resource
    if(isWorking && isCarrying)
    {
        // Reset the pawn state
        isCarrying = false;
        isWorking = false;

        Resources harvest = {0, maxCarry};

        carry = 0;

        UpdateResources(harvest);

        target->SetIsTargeted(false);

        hasDestination = false;
    }
}

void Pawn::Attack()
{
    // NPC can only attack once it reaches the destination
    if(isWorking && !isCarrying)
    {
        // For pawn, it will reduce the health of the tree and gain one carry
        // Enter heavy attack animation if not doing one
        if (!isAttacking && attackCooldownCounter <= 0)
        {
            isAttacking = true;

            // Attack on cooldown
            attackCooldownCounter = attackCooldown;

            ChangeAnimation(State::heavy_attacking);
        }
    }

    // While not attacking, it will decrease the attack cooldown counter
    if(!isAttacking && attackCooldownCounter > 0)
    {
        attackCooldownCounter -= 1;
    }
}

void Pawn::SetDestination()
{
    // If the pawn is not working, then it should be moving
    if (!isWorking && !hasDestination)
    {
        // Return harvest to home
        if (isCarrying)
        {
            target = FindTarget(typeid(Building), 0);

            if (target != NULL)
            {
                destination = target->GetHitBox();
            }

            hasDestination = true;
        }
        // Look for the closest tree
        else
        {
            target = FindTarget(typeid(Nature), 0);

            if (target != NULL)
            {
                destination = target->GetHitBox();
            }

            hasDestination = true;
        }
    }
}

void Pawn::ChangeAnimation(State newState)
{
    // Change the state
    curState = newState;

    // Reset the frame
    curFrame = 0;

    // Reset frame counter
    frameCounter = 0;

    // Set the direction of animation based on the NPC direction
    frameRec = NPCAnimation[type][(int)curState][(int)isCarrying].sourceFrame;
}