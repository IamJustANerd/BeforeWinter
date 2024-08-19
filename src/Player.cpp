#include "../include/Player.h"
#include "../include/Inventory.h"
#include "../include/Grid.h"
#include "../include/Collectible.h"
#include <iostream>

Player::Player(Vector2 _position, Grid* _grid)
{
    position = _position;
    width = 32, height = 32;
    id = 0;
    hitBox = Rectangle{ position.x, position.y + (float)height * 0.75f, (float)width, (float)height / 4 };
    speed = 2.5f;
    collectRadius = Rectangle{position.x - collectRadiusLength,
                              position.y - collectRadiusLength,
                              (float)width + 2 * collectRadiusLength,
                              (float)height + 2 * collectRadiusLength};

    // Insert player into the grid
    grid = _grid;
    grid->Add(this);
}

void Player::Movements()
{
    // Variables for calculating cell changes
    Vector2 change = {0, 0};

    if(IsKeyPressed(KEY_LEFT_SHIFT) && !isSprinting)
    {
        speed += sprintSpeed;
        isSprinting = true;
    }
    else if(IsKeyPressed(KEY_LEFT_SHIFT) && isSprinting)
    {
        speed -= sprintSpeed;
        isSprinting = false;
    }

    // Note: Avoid adding the value directly to prevent jumping movement
    if (IsKeyDown(KEY_A))
    {
        for(int i = 0; i < speed; i++)
        {
            position.x -= 1;
            change.x -= 1;
        }
    }
    if (IsKeyDown(KEY_D))
    {
        for (int i = 0; i < speed; i++)
        {
            position.x += 1;
            change.x += 1;
        }
    }
    if (IsKeyDown(KEY_W))
    {
        for (int i = 0; i < speed; i++)
        {
            position.y -= 1;
            change.y -= 1;
        }
    }
    if (IsKeyDown(KEY_S))
    {
        for (int i = 0; i < speed; i++)
        {
            position.y += 1;
            change.y += 1;
        }
    }

    // Update hitBox position
    hitBox.x = position.x;
    hitBox.y = position.y + (float)height * 0.75f;

    // Update collect radius position
    collectRadius.x = position.x - collectRadiusLength;
    collectRadius.y = position.y - collectRadiusLength;

    // Update player's cell
    grid->Move(this, change);
}

void Player::Draw() const
{
    // Draw body
    DrawRectangle(position.x, position.y, width, height, WHITE);
    
    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});

    // Draw collect radius box
    DrawRectangleRec(collectRadius, Color{0, 121, 241, 120});
}

void Player::Update()
{
    Movements();

    // TakeCollectibles();

    inventory.Update();
}

bool Player::IsInventoryCalled()
{
    return inventory.IsCalled();
}

void Player::DrawInventory() const
{
    inventory.Draw();
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

    if(pos.x != -1 && pos.y != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}