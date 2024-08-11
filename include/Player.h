#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Dynamic.h"
#include "Inventory.h"

class Player : public Dynamic
{
private:
    Inventory inventory;
    float sprintSpeed = 8.0f;
    bool isSprinting = false;
    Rectangle collectRadius;
    float collectRadiusLength = 32.0f;

public:
    // Constructor
    Player(Vector2 _position);

    // Player movements
    void Movements() override;

    // Update player
    void Update() override;

    // Draw
    void Draw() const override;

    // Player inventory current status
    bool IsInventoryCalled();

    // Draw player inventory
    void DrawInventory() const;

    // Return player collect radius rectangle
    Rectangle GetCollectRadiusRectangle();

    // Return player hitbox position
    Vector2 GetHitBoxPosition();
};

#endif // PLAYER_H