#include "../include/Enemy.h"
#include "../include/GameManager.h"
#include "../include/Pawn.h"
#include <iostream>
#include <typeinfo>

Enemy::Enemy(Vector2 _position, int _type, Grid *_grid)
{
    position = _position;

    width = 128, height = 128;

    type = _type;

    // base value, can change depending on the enemy's type
    healthPoint = 50;
    attackPoint = 10;

    // Assign Enemy hitbox
    hitBox = Rectangle{position.x + (float)width / 3, position.y + (float)height * 0.6f, (float)width / 3, (float)height / 8};

    // Assign Enemy attack boxes (similar to Player footprint)
    attackBox[0] = Rectangle{position.x, position.y + height / 6, (float)width / 2, (float)height * 2 / 3};
    attackBox[1] = Rectangle{position.x + width / 6, position.y, (float)width * 2 / 3, (float)height / 2};
    attackBox[2] = Rectangle{position.x + width / 2, position.y + height / 6, (float)width / 2, (float)height * 2 / 3};
    attackBox[3] = Rectangle{position.x + width / 6, position.y + height / 2, (float)width * 2 / 3, (float)height / 2};

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
        // If the Enemy already reach the destination, stop moving
        if (ReachDestination())
        {
            ChangeAnimation(State::idle);
            withinAttackRange = true;

            // Make sure the direction to the target is correct (for the case where the entity (for example player) approach from behind)
            if(target->GetHitBoxPosition().x >= GetHitBox().x)
            {
                direction.x = 1;
            }
            else
            {
                direction.x = -1;
            }
        }
        else
        {
            Movements();
        }

        // Switch to running animation
        if (isMoving && curState != State::running)
        {
            ChangeAnimation(State::running);
        }
    }
}

void Enemy::Draw() const
{
    Color tint = WHITE; // base color
    if (hitTimer > 0)
    {
        tint = Color{255, 0, 0, 128}; // red tint when hit
    }

    // Draws texture based on either the enemy is alive or not
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

    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});

    // Draw attack range boxes
    DrawRectangleRec(attackBox[0], {230, 41, 55, 32});
    DrawRectangleRec(attackBox[1], {255, 161, 0, 32});
    DrawRectangleRec(attackBox[2], {253, 249, 0, 32});
    DrawRectangleRec(attackBox[3], {0, 228, 48, 32});
}

void Enemy::Update()
{
    // Decrease the animation timer for the hitTimer
    if (hitTimer > 0) hitTimer--;

    if (isAlive)
    {
        SetDestination();
        HandleMovements();
        UpdateAttackBox();
        Attack();
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
            // 2. Reduce health of ALL entities hit within the attack box,
            // 3. Trigger the hit animation when hit (if it has no more health, go for death animation instead)
            if (curFrame == 4)
            {
                // Define which entity types the enemy can damage
                std::vector<const std::type_info*> enemyTargets = {&typeid(Player), &typeid(Pawn), &typeid(Building)};

                // Attack all entities in range using grid-based spatial query
                AttackEntitiesInRange(GetActiveAttackBox(), attackPoint, enemyTargets);

                std::cout << "ENEMY: " << GetHitBox().x << ' ' << GetHitBox().y << '\n';
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

            // Assume that the target is no longer on the attack range
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
        std::cout << "CARI TARGET" << '\n';
        // Look for the closest between player and house
        target = FindTarget(typeid(Building), 0, true);
        Entity* temp = FindTarget(typeid(Player), 0, true);

        if(temp != NULL && target != NULL)
        {
            Vector2 thisPos = GetHitBoxPosition();
            float playerDist = sqrt(pow(thisPos.x - temp->GetHitBox().x, 2) + pow(thisPos.y - temp->GetHitBox().y, 2));
            float houseDist = sqrt(pow(thisPos.x - target->GetHitBox().x, 2) + pow(thisPos.y - target->GetHitBox().y, 2));

            std::cout << playerDist << ' ' << houseDist << '\n';

            if(playerDist <= houseDist)
            {
                std::cout << "PLAYER" << '\n';
                destination = temp->GetHitBox();
                target = temp;
            }
            else
            {
                std::cout << "HOUSE" << '\n';
                destination = target->GetHitBox();
            }
        }
        else
        {
            if (temp != NULL)
            {
                destination = temp->GetHitBox();
                target = temp;
            }
            else if(target != NULL)
            {
                destination = target->GetHitBox();
            }
            else
            {
                // Handle case when there are no targets at all (kinda lazy ngl)
            }
        }

        // Add this entity to followers list
        target->AddFollower(this);

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
        // The animation depends on the direction the enemy is facing (will prioritize x axis direction first) --- THIS IS WRONG, IT WON'T WORK PROPERLY
        // The animation will depends on the difference of the x and y position
        // Note: check Assets.cpp for player animation's reference
        Vector2 thisPos = GetHitBoxPosition();
        Vector2 targetPos = target->GetHitBoxPosition();

        if(abs(thisPos.y - targetPos.y) > abs(thisPos.x - targetPos.x))
        {
            if (thisPos.y < targetPos.y)
            {
                frameRec = NPCAnimation[type][(int)curState][1].sourceFrame;
            }
            else
            {
                frameRec = NPCAnimation[type][(int)curState][2].sourceFrame;
            }
        }
        else
        {
            frameRec = NPCAnimation[type][(int)curState][0].sourceFrame;
        }
    }
    else
    {
        frameRec = NPCAnimation[type][(int)curState][0].sourceFrame;
    }
}

void Enemy::UpdateAttackBox()
{
    attackBox[0].x = position.x;
    attackBox[0].y = position.y + height / 6;
    attackBox[1].x = position.x + width / 6;
    attackBox[1].y = position.y;
    attackBox[2].x = position.x + width / 2;
    attackBox[2].y = position.y + height / 6;
    attackBox[3].x = position.x + width / 6;
    attackBox[3].y = position.y + height / 2;
}

Rectangle Enemy::GetActiveAttackBox() const
{
    if (target == NULL) return attackBox[2]; // Default Right

    Vector2 thisPos = GetHitBoxPosition();
    Vector2 targetPos = target->GetHitBoxPosition();

    if(abs(thisPos.y - targetPos.y) > abs(thisPos.x - targetPos.x))
    {
        if (thisPos.y < targetPos.y) return attackBox[3]; // Target is Below -> Down attack box
        else return attackBox[1]; // Target is Above -> Up attack box
    }
    else
    {
        if (thisPos.x < targetPos.x) return attackBox[2]; // Target is Right -> Right attack box
        else return attackBox[0]; // Target is Left -> Left attack box
    }
}