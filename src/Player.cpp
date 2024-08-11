#include "../include/Player.h"
#include "../include/Inventory.h"
#include "../include/Collectible.h"
#include <iostream>

Player::Player(Vector2 _position)
{
    position = _position;
    width = 32, height = 32;
    type = 0;
    hitBox = Rectangle{ position.x, position.y + (float)height * 0.75f, (float)width, (float)height / 4 };
    speed = 2.5f;
    collectRadius = Rectangle{position.x - collectRadiusLength,
                              position.y - collectRadiusLength,
                              (float)width + 2 * collectRadiusLength,
                              (float)height + 2 * collectRadiusLength};
}

void Player::Movements()
{
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
    
    if (IsKeyDown(KEY_A))
    {
        position.x -= speed;
    }
    if (IsKeyDown(KEY_D))
    {
        position.x += speed;
    }
    if (IsKeyDown(KEY_W))
    {
        position.y -= speed;
    }
    if (IsKeyDown(KEY_S))
    {
        position.y += speed;
    }

    // Update hitBox position
    hitBox.x = position.x;
    hitBox.y = position.y + (float)height * 0.75f;

    // // Update collect radius position
    collectRadius.x = position.x - collectRadiusLength;
    collectRadius.y = position.y - collectRadiusLength;
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

Rectangle Player::GetCollectRadiusRectangle()
{
    return collectRadius;
}

Vector2 Player::GetHitBoxPosition()
{
    return Vector2{hitBox.x, hitBox.y};
}