#include "../include/Player.h"
#include "../include/Inventory.h"
#include "../include/Grid.h"
#include "../include/Collectible.h"
#include <iostream>

Player::Player(Vector2 _position, Grid *_grid)
{
    position = _position;
    
    width = 128, height = 128;
    
    id = 0;

    isAlive = true;

    type = 0;
    
    // Assign player hitbox
    hitBox = Rectangle{position.x + (float)width / 3, position.y + (float)height * 0.6f, (float)width / 3, (float)height / 8};
    
    speed = 2.5f;

    // Assign player collect radius
    collectRadius = Rectangle{position.x - collectRadiusLength,
                              position.y - collectRadiusLength,
                              (float)width + 2 * collectRadiusLength,
                              (float)height + 2 * collectRadiusLength};
    
    // Assign player interaction radius
    interactionRadius = Rectangle{position.x - interactionRadiusLength,
                                  position.y - interactionRadiusLength,
                                  (float)width + 2 * interactionRadiusLength,
                                  (float)height + 2 * interactionRadiusLength};
    
    // Assign player attack range
    // Left
    attackBox[0] = Rectangle{position.x, position.y + height / 6, (float)width / 2, (float)height * 2 / 3};
    // Up
    attackBox[1] = Rectangle{position.x + width / 6, position.y, (float)width * 2 / 3, (float)height / 2};
    // Right
    attackBox[2] = Rectangle{position.x + width / 2, position.y + height / 6, (float)width / 2, (float)height * 2 / 3};
    // Down
    attackBox[3] = Rectangle{position.x + width / 6, position.y + height / 2, (float)width * 2 / 3, (float)height / 2};

    rotation = 0;

    isAttacking = false;

    // The starting state is idle
    ChangeAnimation(State::idle);

    // Testing death animation
    ChangeAnimation(State::dying);

    // Player is uncollidable
    isUncollidable = true;

    // Set starting direction as neutral right
    direction = Vector2({1.0f, 0.0f});
    moveDirectionIndex = 2;

    // Insert player into the grid
    grid = _grid;
    grid->Add(this);
}

void Player::Movements()
{
    // Can only move if player is not attacking
    if(isAttacking)
    {
        return;
    }

    // Variables for calculating cell changes
    Vector2 change = {0, 0};

    // Border for player movement to prevent player from moving out of the grid
    int minBorderX = grid->CELL_SIZE;
    int maxBorderX = (grid->NUM_CELLS - 1) * grid->CELL_SIZE;
    int minBorderY = grid->CELL_SIZE;
    int maxBorderY = (grid->NUM_CELLS - 1) * grid->CELL_SIZE;

    // Toggle sprint
    if (IsKeyPressed(KEY_LEFT_SHIFT) && !isSprinting)
    {
        speed += sprintSpeed;
        isSprinting = true;
    }
    else if (IsKeyPressed(KEY_LEFT_SHIFT) && isSprinting)
    {
        speed -= sprintSpeed;
        isSprinting = false;
    }

    // Note: Avoid adding the value directly to prevent jumping movement
    // Bug infinite loop ketika menyentuh border: SOLVED
    // Penjelasan: Terjadi karena nilai change tidak ikut diganti ketika sudah mencapai batas, menyebabkan
    // suatu objek memiliki dua posisi grid yang berbeda. Hal ini dapat menyebabkan circular reference, thus
    // infinite loop.
    // Solusi: Mencegah nilai dari position ditambah jika menambahkan akan membuatnya keluar dari batas

    // Only add player position while:
    // 1. Is still within the border
    // 2. Wouldn't collide with other uncollidable entity

    // To check if the player is moving or not (will be set as true if the player moves in any direction)
    isMoving = false;
    
    // Left movement
    if (IsKeyDown(KEY_A))
    {
        moveDirectionIndex = 0;

        int i = 0;

        while ((i < speed && hitBox.x > minBorderX) &&
               !IsCollidingWithUncollidable())
        {
            i += 1;

            // Update player position (in the grid as well)
            position.x -= 1;
            change.x -= 1;

            // Update hitBox position
            hitBox.x = position.x + (float)width / 3;
        }

        // Update player direction
        direction.x = -1.0f;
        direction.y = 0.0f;

        isMoving = true;
    }
    // Right movement
    else if (IsKeyDown(KEY_D))
    {
        moveDirectionIndex = 2;

        int i = 0;
        while ((i < speed && (hitBox.x + hitBox.width) < maxBorderX) &&
               !IsCollidingWithUncollidable())
        {
            i += 1;

            // Update player position (in the grid as well)
            position.x += 1;
            change.x += 1;

            // Update hitBox position
            hitBox.x = position.x + (float)width / 3;
        }

        // Update player direction
        direction.x = 1.0f;
        direction.y = 0.0f;

        isMoving = true;
    }

    // Up movement
    if (IsKeyDown(KEY_W))
    {
        moveDirectionIndex = 1;

        int i = 0;
        while ((i < speed && hitBox.y > minBorderY) &&
               !IsCollidingWithUncollidable())
        {
            i += 1;

            // Update player position (in the grid as well)
            position.y -= 1;
            change.y -= 1;

            // Update hitBox position
            hitBox.y = position.y + (float)height * 0.6f;
        }

        // Update player direction
        direction.y = -1.0f;

        isMoving = true;
    }
    // Down movement
    else if (IsKeyDown(KEY_S))
    {
        moveDirectionIndex = 3;

        // Add player position while is still within the border
        int i = 0;
        while ((i < speed && (hitBox.y + hitBox.height) < maxBorderY) &&
               !IsCollidingWithUncollidable())
        {
            i += 1;

            // Update player position (in the grid as well)
            position.y += 1;
            change.y += 1;

            // Update hitBox position
            hitBox.y = position.y + (float)height * 0.6f;
        }

        // Update player direction
        direction.y = 1.0f;

        isMoving = true;
    }

    // Switch to running animation
    if(isMoving && curState != State::running)
    {
        ChangeAnimation(State::running);
    }
    
    // Switch to idle animation
    if(!isMoving && curState != State::idle)
    {
        ChangeAnimation(State::idle);
    }

    // Update collect radius position
    collectRadius.x = position.x - collectRadiusLength;
    collectRadius.y = position.y - collectRadiusLength;

    // Update interaction radius position
    interactionRadius.x = position.x - interactionRadiusLength;
    interactionRadius.y = position.y - interactionRadiusLength;

    // Update attack range position
    // -> Left
    attackBox[0].x = position.x;
    attackBox[0].y = position.y + height / 6;
    // -> Up
    attackBox[1].x = position.x + width / 6;
    attackBox[1].y = position.y;
    // -> Right
    attackBox[2].x = position.x + width / 2;
    attackBox[2].y = position.y + height / 6;
    // -> Down
    attackBox[3].x = position.x + width / 6;
    attackBox[3].y = position.y + height / 2;
    
    // Update player's cell
    grid->Move(this, change);
}

void Player::Draw() const
{
    // Draw body
    // DrawRectangle(position.x, position.y, width, height, {230, 41, 55, 128});

    // Draw texture
    if(isAlive)
    {
        if (direction.x >= 0)
        {
            DrawTextureRec(playerTex[0], frameRec, position, WHITE);
        }
        else if (direction.x <= -1)
        {
            DrawTextureRec(playerTex[0], FlipTexture(frameRec), {position.x, position.y}, WHITE);
        }
    }
    else if(curState == State::dying || curState == State::decaying)
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
    

    // Draw attack range
    DrawRectangleRec(attackBox[0], {230, 41, 55, 128});
    DrawRectangleRec(attackBox[1], {255, 161, 0, 128});
    DrawRectangleRec(attackBox[2], {253, 249, 0, 128});
    DrawRectangleRec(attackBox[3], {0, 228, 48, 128});

    // Draw collect radius box
    // DrawRectangleRec(collectRadius, Color{0, 121, 241, 120});

    // Draw interaction radius box
    // DrawRectangleRec(interactionRadius, Color{253, 249, 0, 128});

    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
}

void Player::Update()
{
    if(isAlive)
    {
        Movements();

        Attack();

        UpdateSpriteFrame();

        inventory.Update();
    }
    else
    {
        if(isDecay)
        {
            DecayAnimation(0);
        }
        else
        {
            DeathAnimation(0);
        }
    }
}

bool Player::IsInventoryCalled()
{
    return inventory.IsCalled();
}

void Player::DrawInventory() const
{
    inventory.DrawInventory();
}

void Player::DrawToolbar() const
{
    inventory.DrawToolbar();
}

void Player::UpdateToolbar()
{
    inventory.ToolbarShortcut();
    Player::DrawToolbar();
}

void Player::AddItemToInventory(int _id, int _amount)
{
    inventory.AddItem(_id, _amount);
}

Rectangle Player::GetCollectRadiusRectangle()
{
    return collectRadius;
}

Vector2 Player::GetHitBoxPosition()
{
    return Vector2{hitBox.x, hitBox.y};
}

bool Player::IsInventoryFull()
{
    return inventory.IsFull();
}

bool Player::CanItemFitIntoInventory(int _id, int _amount)
{
    itemPosition pos = inventory.FindSlot(_id, _amount);

    if (pos.x != -1 && pos.y != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Player::UpdateSpriteFrame()
{
    frameCounter += 1;
    if (frameCounter >= playerAnimation[(int)curState][0].frameTime / playerAnimation[(int)curState][0].totalFrames)
    {
        curFrame = (curFrame + 1) % playerAnimation[(int)curState][0].totalFrames;

        frameCounter = 0;

        frameRec.x = ((int)(frameRec.x + width) % (int)(playerAnimation[(int)curState][0].totalFrames * width));

        // Check if this is an attack animation
        if(curState == State::light_attacking || curState == State::heavy_attacking)
        {
            // If it is, make sure to stop the attack animation once it reaches back to the first frame
            if(frameRec.x <= 0)
            {
                // Return back to idle animation
                isAttacking = false;

                ChangeAnimation(State::idle);
            }

            // For player
            // If this is the 4th frame, activate the attack box in that direction
            if(curFrame == 3)
            {
                // Down
                if (direction.y == 1)
                {
                    std::cout << "BAWAH" << "\n";
                }
                // Up
                else if (direction.y == -1)
                {
                    std::cout << "ATAS" << "\n";
                }
                // Right
                else if (direction.x == 1)
                {
                    std::cout << "KANAN" << "\n";
                }
                // Left
                else if(direction.x == -1)
                {
                    std::cout << "KIRI" << "\n";
                }
            }
        }
    }
}

void IsColliding()
{
}

void Player::Attack()
{
    // Player can only attack if it is currently not attacking
    if(!isAttacking)
    {
        // Light attack
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // Set player state as attacking to prevent player from doing other action (for example: running)
            isAttacking = true;
            
            ChangeAnimation(State::light_attacking);
        }
        // Heavy attack
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            // Set player state as attacking to prevent player from doing other action (for example: running)
            isAttacking = true;
            
            ChangeAnimation(State::heavy_attacking);
        }
    }
}

void Player::ChangeAnimation(State newState)
{
    // Reset frame counter
    frameCounter = 0;

    curState = newState;

    curFrame = 0;

    // Handle animation direction for attack animations
    if(curState == State::light_attacking || curState == State::heavy_attacking)
    {
        // The animation depends on the direction the player is facing (will prioritize x axis direction first)
        // Note: check Assets.cpp for player animation's reference
        if (direction.y == 1)
        {
            frameRec = playerAnimation[(int)curState][1].sourceFrame;
        }
        else if (direction.y == -1)
        {
            frameRec = playerAnimation[(int)curState][2].sourceFrame;
        }
        else if (direction.x == 1 || direction.x == -1)
        {
            frameRec = playerAnimation[(int)curState][0].sourceFrame;
        }
    }
    // Handle animations for dyinh and decaying animation
    else if(curState == State::dying)
    {
        frameRec = deathAnimation[0][0].sourceFrame;
    }
    else if(curState == State::decaying)
    {
        frameRec = deathAnimation[0][1].sourceFrame;
    }
    else
    {
        frameRec = playerAnimation[(int)curState][0].sourceFrame;
    }
}