#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Dynamic.h"
#include "Inventory.h"

class Player : public Dynamic
{
private:
    Inventory inventory;

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
};

#endif // PLAYER_H