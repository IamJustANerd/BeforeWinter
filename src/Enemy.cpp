#include "../include/Enemy.h"
#include "../include/GameManager.h"
#include <iostream>
#include <typeinfo>

Enemy::Enemy(Vector2 _position, int _type, Grid *_grid)
{
    position = _position;

    width = 128, height = 128;

    type = _type;

    // Assign Enemy hitbox
    hitBox = Rectangle{position.x + (float)width / 3, position.y + (float)height * 0.6f, (float)width / 3, (float)height / 8};

    speed = 2.0f;

    rotation = 0;

    isAttacking = false;

    hasDestination = false;

    isCollidingWithOther = false;

    withinAttackRange = false;

    priorityMove = false;

    rotationCounter = 0;

    // The starting state is idle
    ChangeAnimation(State::idle);

    // Enemy is uncollidable
    isUncollidable = true;

    // Set starting direction as neutral right
    direction = Vector2({1.0f, 0.0f});
    moveDirectionIndex = 2;

    // Insert Enemy into the grid
    grid = _grid;
    grid->Add(this);
}

void Enemy::HandleMovements()
{
    // Update target new position
    destination = target->GetHitBox();

    // Enemy can't move if it is attacking
    if (isAttacking)
    {
        return;
    }

    // If not being stunned, move to the target
    if(stunCounter <= 0)
    {
        Movements();

        // Switch to running animation
        if (isMoving && curState != State::running)
        {
            ChangeAnimation(State::running);
        }

        // If the Enemy already reach the destination, stop moving
        if (ReachDestination())
        {
            ChangeAnimation(State::idle);
            withinAttackRange = true;
        }
    }
}

void Enemy::Draw() const
{
    // Draw texture
    if (direction.x >= 0)
    {
        DrawTextureRec(NPCTex[type], frameRec, position, WHITE);
    }
    else if (direction.x <= -1)
    {
        DrawTextureRec(NPCTex[type], FlipTexture(frameRec), {position.x, position.y}, WHITE);
    }

    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
}

void Enemy::Update()
{
    SetDestination();
    HandleMovements();
    Attack();
    UpdateSpriteFrame();
}

void Enemy::UpdateSpriteFrame()
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

            // If the Enemy reached the fourth frame of attack animation, make sure to:
            // 1. Activate attack rectangle
            // 2. Reduce target health if hit,
            // 3. Trigger the hit animation when hit (if it has no more health, go for death animation instead)
            if (curFrame == 4)
            {
                
            }
        }
    }
}

void Enemy::Attack()
{
    // Enemy can only attack once it reaches the destination
    if (withinAttackRange && stunCounter <= 0)
    {
        // Enter light attack animation if not doing one
        if (!isAttacking && attackCooldownCounter <= 0)
        {
            isAttacking = true;

            // Attack on cooldown
            attackCooldownCounter = attackCooldown;

            // Enter stun state
            stunCounter = stunDuration;

            // Assume that the player is no longer on the attack range
            withinAttackRange = false;

            ChangeAnimation(State::light_attacking);
        }
    }

    // While not attacking, it will decrease the attack cooldown counter and stun counter
    if (!isAttacking)
    {
        if(attackCooldownCounter > 0)
        {
            attackCooldownCounter -= 1;
        }

        if(stunCounter > 0)
        {
            stunCounter -= 1;
        }
    }
}

void Enemy::SetDestination()
{
    // If the Enemy is not working, then it should be moving
    if (!hasDestination)
    {
        // Look for the closest between player and house
        target = FindTarget(typeid(Building), 0, true);
        Entity* temp = FindTarget(typeid(Player), 0, true);

        if (target == NULL)
        {
            destination = target->GetHitBox();
        }
        else
        {
            destination = temp->GetHitBox();
            target = temp;
        }
        std::cout << destination.x << ' ' << destination.y << '\n';

        hasDestination = true;
    }
}

void Enemy::ChangeAnimation(State newState)
{
    // Change the state
    curState = newState;

    // Reset the frame
    curFrame = 0;

    // Reset frame counter
    frameCounter = 0;

    // Handle animation direction for attack animations
    if (curState == State::light_attacking || curState == State::heavy_attacking)
    {
        // The animation depends on the direction the enemy is facing (will prioritize x axis direction first)
        // Note: check Assets.cpp for player animation's reference
        if (direction.y == 1)
        {
            frameRec = NPCAnimation[type][(int)curState][1].sourceFrame;
        }
        else if (direction.y == -1)
        {
            frameRec = NPCAnimation[type][(int)curState][2].sourceFrame;
        }
        else if (direction.x == 1 || direction.x == -1)
        {
            frameRec = NPCAnimation[type][(int)curState][0].sourceFrame;
        }
    }
    else
    {
        frameRec = NPCAnimation[type][(int)curState][0].sourceFrame;
    }
}