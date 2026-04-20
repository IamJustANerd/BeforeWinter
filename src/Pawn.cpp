#include "../include/Pawn.h"
#include "../include/GameManager.h"
#include <iostream>
#include <typeinfo>

Pawn::Pawn(Vector2 _position, int _type, Grid *_grid)
{
    position = _position;

    width = 128, height = 128;

    type = _type;
    healthPoint = 50;

    // Assign Pawn hitbox
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

    // Pawn is uncollidable
    isUncollidable = true;

    // Set starting direction as neutral right
    direction = Vector2({1.0f, 0.0f});
    moveDirectionIndex = 2;

    // Insert Pawn into the grid
    grid = _grid;
    grid->Add(this);
}

void Pawn::HandleMovements()
{
    // Note: For now, Pawn doesn't need to constantly update the newest location of the target since
    // the target itself can't move

    // Pawn can't move if it is working
    if (isWorking)
    {
        return;
    }

    // If the Pawn already reach the destination, stop moving
    if (ReachDestination() && hasDestination)
    {
        isWorking = true;

        ChangeAnimation(State::idle);
    }

    Movements();

    // Switch to running animation
    if (isMoving && curState != State::running)
    {
        ChangeAnimation(State::running);
    }
}

void Pawn::Draw() const
{
    Color tint = WHITE; // base color
    if (hitTimer > 0)
    {
        tint = Color{255, 0, 0, 128}; // red tint when hit
    }

    if (isAlive)
    {
        // Draw texture
        if (direction.x >= 0)
        {
            DrawTextureRec(NPCTex[type], frameRec, position, tint);
        }
        else if (direction.x <= -1)
        {
            DrawTextureRec(NPCTex[type], FlipTexture(frameRec), {position.x, position.y}, tint);
        }
    }
    else if (curState == State::dying || curState == State::decaying)
    {
        if (direction.x >= 0)
        {
            DrawTextureRec(deathTex[0], frameRec, position, WHITE);
        }
        else if (direction.x <= -1)
        {
            DrawTextureRec(deathTex[0], FlipTexture(frameRec), {position.x, position.y}, WHITE);
        }
    }

    // Draw resource (if carrying any)
    if (isCarrying)
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
    if (hitTimer > 0) hitTimer--;

    if (isAlive)
    {
        SetDestination();
        HandleMovements();
        Attack();
        Submit();
        UpdateSpriteFrame();
    }
    else
    {
        if (isDecay)
        {
            DecayAnimation(0);
        }
        else
        {
            DeathAnimation(0);
        }
    }
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
            if (curFrame == 3)
            {
                carry += attackPoint;

                target->ReduceHealthPoint(attackPoint);

                target->ChangeAnimation(State::hit);

                // If the pawn reached the maximum amount of value it can carry, stop working
                if (carry >= maxCarry)
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
    // If Pawn Reaches house, return the harvested resource
    if (isWorking && isCarrying)
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
    // Pawn can only attack once it reaches the destination
    if (isWorking && !isCarrying)
    {
        // For pawn, it will reduce the health of the tree and gain one carry
        // Enter heavy attack animation if not doing one
        if (!isAttacking && attackCooldownCounter <= 0)
        {
            std::cout << "Attack" << '\n';
            isAttacking = true;

            // Attack on cooldown
            attackCooldownCounter = attackCooldown;

            ChangeAnimation(State::heavy_attacking);
        }
    }

    // While not attacking, it will decrease the attack cooldown counter
    if (!isAttacking && attackCooldownCounter > 0)
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
            target = FindTarget(typeid(Building), 0, true);

            if (target != NULL)
            {
                destination = target->GetHitBox();

                hasDestination = true;
            }
        }
        // Look for the closest tree
        else
        {
            target = FindTarget(typeid(Nature), 0, false);

            if (target != NULL)
            {
                destination = target->GetHitBox();

                hasDestination = true;
            }

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

    // Set the direction of animation based on the Pawn direction
    frameRec = NPCAnimation[type][(int)curState][(int)isCarrying].sourceFrame;
}