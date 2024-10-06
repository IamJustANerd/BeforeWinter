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

    // Player is uncollidable
    isUncollidable = true;

    // Insert player into the grid
    grid = _grid;
    grid->Add(this);
}

void Player::Movements()
{
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
    else if(IsKeyPressed(KEY_LEFT_SHIFT) && isSprinting)
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

    // Left movement
    if (IsKeyDown(KEY_A))
    {
        int i = 0;
        while((i < speed && hitBox.x > minBorderX) &&
              !IsCollidingWithUncollidable("Left"))
        {
            i += 1;

            // Update player position (in the grid as well)
            position.x -= 1;
            change.x -= 1;

            // Update hitBox position
            hitBox.x = position.x;
        }
    }

    // Right movement
    if (IsKeyDown(KEY_D))
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
            hitBox.x = position.x;
        }
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
            hitBox.y = position.y + (float)height * 0.75f;
        }
    }

    // Down movement
    if (IsKeyDown(KEY_S))
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
            hitBox.y = position.y + (float)height * 0.75f;
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
    DrawRectangle(position.x, position.y, width, height, RED);
    
    // Draw collect radius box
    DrawRectangleRec(collectRadius, Color{0, 121, 241, 120});

    // Draw hitbox
    DrawRectangleRec(hitBox, Color{0, 228, 48, 120});
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

    if(pos.x != -1 && pos.y != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void IsColliding()
{

}