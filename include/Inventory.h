#ifndef INVENTORY_H
#define INVENTORY_H

#include "raylib.h"
#include "Screen.h"

struct inventoryItem
{
    int id;
    int type;
    int amount;
};

struct itemPosition
{
    int x, y;
};

class Inventory
{
private:
    bool CheckMouseHover(int slotX, int slotY, int slotW, int slotH) const;

public :
    // Constructor
    Inventory();

    // Draw inventory
    void Draw() const;
    
    // Update the state
    void Update();

    // Add item when player collect one
    void AddItem();

    // Return either the inventory is called or not
    bool IsCalled();
};

#endif