#include "../include/Player.h"
#include "../include/Inventory.h"

Player::Player(Vector2 _position)
{
    position = _position;
    width = 32, height = 32;
    type = 0;
    hitBox = Rectangle{position.x + (float)width / 2, position.y + (float)height / 2, (float)width / 2, (float)height / 2};
    speed = 10;
}

void Player::Movements()
{
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
    hitBox.x = position.x + (float)width / 2;
    hitBox.y = position.y + (float)height / 2;
}

void Player::Draw() const
{
    // Draw body
    DrawRectangle(position.x, position.y, width, height, WHITE);
    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
}

void Player::Update()
{
    Movements();

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