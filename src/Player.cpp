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
    
    rotation = 0;

    isAttacking = false;

    // The starting state is idle
    curState = State::idle;

    // Set the frame rec according to the current state
    frameRec = playerAnimation[(int)curState][0].sourceFrame;

    // Player is uncollidable
    isUncollidable = true;

    // Set starting direction as neutral right
    direction = Vector2({1.0f, 0.0f});

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
        int i = 0;

        while ((i < speed && hitBox.x > minBorderX) &&
               !IsCollidingWithUncollidable("Left"))
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
        int i = 0;
        while ((i < speed && (hitBox.x + hitBox.width) < maxBorderX) &&
               !IsCollidingWithUncollidable("Right"))
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
        int i = 0;
        while ((i < speed && hitBox.y > minBorderY) &&
               !IsCollidingWithUncollidable("Up"))
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
        // Add player position while is still within the border
        int i = 0;
        while ((i < speed && (hitBox.y + hitBox.height) < maxBorderY) &&
               !IsCollidingWithUncollidable("Down"))
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
        curState = State::running;
        frameRec = playerAnimation[(int)curState][0].sourceFrame;
        
        // Reset frame counter
        frameCounter = 0;
    }
    
    // Switch to idle animation
    if(!isMoving && curState != State::idle)
    {
        curState = State::idle;
        frameRec = playerAnimation[(int)curState][0].sourceFrame;

        // Reset frame counter
        frameCounter = 0;
    }

    // Update collect radius position
    collectRadius.x = position.x - collectRadiusLength;
    collectRadius.y = position.y - collectRadiusLength;

    // Update interaction radius position
    interactionRadius.x = position.x - interactionRadiusLength;
    interactionRadius.y = position.y - interactionRadiusLength;

    // Update player's cell
    grid->Move(this, change);
}

void Player::Draw() const
{
    // Draw body
    DrawRectangle(position.x, position.y, width, height, {230, 41, 55, 128});

    // Draw texture
    if(direction.x >= 0)
    {
        DrawTextureRec(playerTex[0], frameRec, position, WHITE); 
    }
    else if(direction.x <= -1)
    {
        DrawTextureRec(playerTex[0], FlipTexture(frameRec), {position.x, position.y}, WHITE);
    }
    
    // Draw collect radius box
    DrawRectangleRec(collectRadius, Color{0, 121, 241, 120});

    // Draw interaction radius box
    DrawRectangleRec(interactionRadius, Color{253, 249, 0, 128});

    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
}

void Player::Update()
{
    Movements();

    Attack();

    UpdateSpriteFrame();

    inventory.Update();
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

                curState = State::idle;
            
                frameRec = playerAnimation[(int)curState][0].sourceFrame;
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
            curState = State::light_attacking;

            // The animation depends on the direction the player is facing (will prioritize x axis direction first)
            // Note: check Assets.cpp for player animation's reference
            
            // Note: Need to change the attack direction based on the mouse position instead
            if(direction.y == 1)
            {
                frameRec = playerAnimation[(int)curState][1].sourceFrame;
            }
            else if(direction.y == -1)
            {
                frameRec = playerAnimation[(int)curState][2].sourceFrame;
            }
            else if (direction.x == 1 || direction.x == -1)
            {
                frameRec = playerAnimation[(int)curState][0].sourceFrame;
            }

            // Reset frame counter
            frameCounter = 0;
        }
        // Heavy attack
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            // Set player state as attacking to prevent player from doing other action (for example: running)
            isAttacking = true;
            curState = State::heavy_attacking;

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

            // Reset frame counter
            frameCounter = 0;
        }

        
    }
}