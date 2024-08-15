#ifndef INVENTORY_H
#define INVENTORY_H

#include "raylib.h"
#include "Screen.h"

struct inventoryItem
{
    // Item id
    int id;

    // Item type (I don't know what this is for yet)
    int type;

    // Amount
    /*
        Note: The max amount of an item type in the same slot might vary. For example,
        only one tool (like pickaxe) can exist silmutaneously at a same single slot,
        while some consumeable (like potions) can stacked up to 100
    */
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

    // Return either the inventory is called or not
    bool IsCalled();

    // Find available slot
    itemPosition FindSlot(int _id, int _amount);

    // Add item when player collect one
    void AddItem(int _id, int _amount);

    // Return either inventory is full or not
    bool IsFull();
};

#endif