#ifndef PLAYER_H
#define PLAYER_H

#include "../include/raylib_includes.h"
#include "Dynamic.h"
#include "Inventory.h"

class Player : public Dynamic
{
private:
    Inventory inventory;
    // Note: There is a potential bug of infinity loop(?) when the player is moving too fast while it still has some slot in inventory
    float sprintSpeed = 40.0f;
    bool isSprinting = false;
    Rectangle collectRadius;
    Rectangle interactionRadius;
    float interactionRadiusLength = 16.0f;
    float collectRadiusLength = 32.0f;

public:
    // Constructor
    Player(Vector2 _position, Grid *_grid);

    // Player movements
    void Movements() override;

    // Update player
    void Update() override;

    // Update sprite frame
    void UpdateSpriteFrame() override;

    // Attack
    void Attack() override;

    // Draw
    void Draw() const override;

    // Player inventory current status
    bool IsInventoryCalled();

    // Draw player inventory
    void DrawInventory() const;

    // Draw player toolbar
    void DrawToolbar() const;

    // Update player toolbar
    void UpdateToolbar();

    // Add item to inventory
    void AddItemToInventory(int _id, int _amount);

    // Return player collect radius rectangle
    Rectangle GetCollectRadiusRectangle();

    // Return player hitbox position
    Vector2 GetHitBoxPosition();

    // Return either player's inventory is full or not
    bool IsInventoryFull();

    // Return either item can fit into inventory or not
    bool CanItemFitIntoInventory(int _id, int _amount);
};

#endif // PLAYER_H