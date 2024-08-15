#include "../include/Inventory.h"
#include "../include/Mouse.h"
#include <iostream>

#define slotVertical 6
#define slotHorizontal 10

// Struct for each inventory items
inventoryItem inventoryItems[slotVertical][slotHorizontal];

// Variables for inventory calculation
itemPosition position;
int slot = 60;
int inventoryWidth, inventoryHeight;
int slotWidth, slotHeight;
int verticalOffset, horizontalOffset;

// Variables for state
bool isCalled = false;
bool isHover = false;
bool isDrag = false;
bool isDragHovered = false;

// Variables for swapping items
itemPosition origin, destination;

// Constructor
Inventory::Inventory()
{
    // Set inventory to be empty
    for(int i = 0; i < slotVertical; i++)
    {
        for(int j = 0; j < slotHorizontal; j++)
        {
            inventoryItems[i][j].id = -1;
            inventoryItems[i][j].type = -1;
            inventoryItems[i][j].amount = -1;
        }
    }

    // Testing item placement in inventory
    inventoryItems[5][9].id = 1;
    inventoryItems[5][9].type = 1;
    inventoryItems[5][9].amount = 1;

    inventoryItems[0][0].id = 7;
    inventoryItems[0][0].type = 1;
    inventoryItems[0][0].amount = 999;

    inventoryItems[5][3].id = 355;
    inventoryItems[5][3].type = 1;
    inventoryItems[5][3].amount = 99999;
}

// Swap two items
void SwapItem(inventoryItem &a, inventoryItem &b)
{
    inventoryItem temp = a;
    a = b;
    b = temp;
}

// Count a number digit
int DigitLength(int x)
{
    int digitLength = 0;

    while(x > 0)
    {
        x /= 10;
        digitLength += 1;
    }

    return digitLength;
}

void Inventory::Draw() const
{
    // Calculate variables
    position.x = screenWidth / 4 / scale, position.y = screenHeight / 4 / scale;
    inventoryWidth = screenWidth / 2 / scale;
    inventoryHeight = screenHeight / 2 / scale;
    slotWidth = inventoryWidth / (slotHorizontal + 1);
    slotHeight = slotWidth;
    verticalOffset = (inventoryHeight - slotHeight * (slotVertical)) / 2;
    horizontalOffset = (inventoryWidth - slotWidth * (slotHorizontal)) / 2;

    // Reset hover and drag hover flags
    isHover = false;
    isDragHovered = false;

    // To keep track of the slot position
    itemPosition pos;
    pos.x = 0, pos.y = 0;

    // Draw the slots
    for (int i = position.y; i < position.y + slotHeight * slotVertical; i += slotHeight, pos.x += 1, pos.y = 0)
    {
        for (int j = position.x; j < position.x + slotWidth * slotHorizontal; j += slotWidth, pos.y += 1)
        {
            // Gray for the default color
            Color slotColor = Color{130, 130, 130, 127};

            // Print the items properties (texture, amount, etc)
            if (inventoryItems[pos.x][pos.y].id != -1)
            {
                slotColor = Color{0, 228, 48, 127};
                
                // Item ID
                DrawText(TextFormat("ID: %i", inventoryItems[pos.x][pos.y].id),
                                    j + horizontalOffset + 5,
                                    i + verticalOffset + 5,
                                    7,
                                    WHITE);

                // Item amount
                DrawText(TextFormat("%i", inventoryItems[pos.x][pos.y].amount),
                                    j + horizontalOffset + slotWidth - DigitLength(inventoryItems[pos.x][pos.y].amount) * 7,
                                    i + verticalOffset + slotHeight - 10,
                                    7,
                                    WHITE);
            }

            // Handle mouse hover
            if (CheckMouseHover(j + horizontalOffset, i + verticalOffset, slotWidth, slotHeight) && !isHover)
            {
                slotColor = Color{200, 200, 0, 127};
                isHover = true;

                // If the mouse was dragging an item and the player releases it over another slot,
                // then swap it
                if (isDrag && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    destination.x = pos.x;
                    destination.y = pos.y;

                    // Swap the items at origin and destination
                    if (inventoryItems[origin.x][origin.y].id != -1)
                    {
                        SwapItem(inventoryItems[origin.x][origin.y], inventoryItems[destination.x][destination.y]);
                    }

                    // Reset dragging state after swapping
                    isDrag = false;
                }
            }

            // Handle mouse drag
            if (isHover && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                // Only set the origin once when dragging starts
                if (!isDrag)
                {
                    origin.x = pos.x;
                    origin.y = pos.y;
                    isDrag = true;
                }

                // Give indication color for dragging (using flag to prevent multiple slots from being highlighted)
                if (!isDragHovered)
                {
                    slotColor = Color{230, 41, 55, 127};
                    isDragHovered = true;
                }
            }

            // Draw the slot
            DrawRectangle(j + horizontalOffset, i + verticalOffset, slotWidth, slotHeight, slotColor);
            DrawRectangleLines(j + horizontalOffset, i + verticalOffset, slotWidth, slotHeight, BLACK);

            // If the inventory is closed when dragging item, reset the drag flag
            // if(!isCalled)
            // {
            //     origin.x = pos.x;
            //     origin.y = pos.y;
            //     isDrag = false;
            // }
        }
    }
}

void Inventory::Update()
{
    if (IsKeyPressed(KEY_I))
    {
        isCalled = !isCalled;
    }
}

bool Inventory::CheckMouseHover(int slotX, int slotY, int slotW, int slotH) const
{
    Vector2 mousePos = GetMousePosition();
    mousePos.x = (mousePos.x - (GetScreenWidth() - gameScreenWidth * scale) * 0.5f) / scale;
    mousePos.y = (mousePos.y - (GetScreenHeight() - gameScreenHeight * scale) * 0.5f) / scale;

    return mousePos.x >= slotX && mousePos.x <= slotX + slotW &&
           mousePos.y >= slotY && mousePos.y <= slotY + slotH;
}

bool Inventory::IsCalled()
{
    return isCalled;
}

itemPosition Inventory::FindSlot(int _id, int _amount)
{
    // Find available slot to keep or stack item (stack will always be prioritized when collecting items)
    itemPosition pos;
    pos.x = -1, pos.y = -1;
    bool available = false;

    // First, try to stack item
    for(int i = 0; i < slotVertical && !available; i++)
    {
        for(int j = 0; j < slotHorizontal && !available; j++)
        {
            if(inventoryItems[i][j].id == _id)
            {
                pos.x = i, pos.y = j;
                available = true;
            }
        }
    }

    // If stacking is not possible, find an empty slot
    for(int i = 0; i < slotVertical && !available; i++)
    {
        for(int j = 0; j < slotHorizontal && !available; j++)
        {
            if(inventoryItems[i][j].id == -1)
            {
                pos.x = i, pos.y = j;
                available = true;
            }
        }
    }

    return pos;
}

void Inventory::AddItem(int _id, int _amount)
{
    // Find available slot
    itemPosition pos = FindSlot(_id, _amount);

    // If full
    if(pos.x == -1 && pos.y == -1)
    {
        return;
    }

    // Stacking item
    if(inventoryItems[pos.x][pos.y].id == _id)
    {
        inventoryItems[pos.x][pos.y].amount += 1;
    }
    // Adding item
    else if (inventoryItems[pos.x][pos.y].id == -1)
    {
        inventoryItems[pos.x][pos.y].id = _id;
        inventoryItems[pos.x][pos.y].amount = 1;
    }
}

bool Inventory::IsFull()
{
    bool isFull = true;
    
    for(int i = 0; i < slotVertical; i++)
    {
        for(int j = 0; j < slotHorizontal; j++)
        {
            if(inventoryItems[i][j].id == -1)
            {
                isFull = false;
            }
        }
    }

    return isFull;
}