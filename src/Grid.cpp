#include "../include/Grid.h"
#include "../include/Entity.h"
#include "../include/Player.h"
#include "../include/Collectible.h"
#include "../include/Nature.h"
#include "../include/Mouse.h"
#include "../include/raylib.h"
#include <cstddef>
#include <algorithm>
#include <vector>
#include <iostream>

// Custom compare bool
struct CompareObjectPosition
{
    bool operator()(const Entity *a, const Entity *b) const
    {
        if (a->GetPosition().y + a->GetHeight() != b->GetPosition().y + b->GetHeight())
            return a->GetPosition().y + a->GetHeight() < b->GetPosition().y + b->GetHeight(); // Compare by y first
        return a->GetPosition().x < b->GetPosition().x;                                       // If y is the same, compare by x
    }
};

Grid::Grid()
{
    // Clear the grid.
    for (int x = 0; x < NUM_CELLS; x++)
    {
        for (int y = 0; y < NUM_CELLS; y++)
        {
            cells[x][y] = NULL;
        }
    }
}

const Entity *const (&Grid::GetReadOnlyCells() const)[NUM_CELLS][NUM_CELLS]
{
    return cells;
}

void Grid::Add(Entity *entity)
{
    // Determine which grid cell it's in
    int cellX = (int)(entity->GetPosition().x / CELL_SIZE);
    int cellY = (int)(entity->GetPosition().y / CELL_SIZE);

    // Add to the front of list for the cell it's in
    entity->prev = NULL;
    entity->next = cells[cellX][cellY];
    cells[cellX][cellY] = entity;

    if (entity->next != NULL)
    {
        entity->next->prev = entity;
    }
}

void Grid::UpdateGrid()
{
    // Handle each cell (update etc)
    for(int i = 0; i < NUM_CELLS; i++)
    {
        for(int j = 0; j < NUM_CELLS; j++)
        {
            // Handle entities
            HandleCell(cells[i][j]);
        
            // Handle mouse
            int mousePosX = (int)GetMouseRect().x / CELL_SIZE, mousePosY = (int)GetMouseRect().y / CELL_SIZE;
            HandleMouse(cells[i][i]);
        }
    }
}

void Grid::HandleCell(Entity* entity)
{
    // Handle collisions on a cell
    // Check collisions of each entity with the others inside the cell
    while (entity != NULL)
    {   
        // Update the entity
        entity->Update();
            
        // Handling player collision
        if (typeid(*entity) == typeid(Player))
        {
            HandlePlayer(entity);
        }

        // Handling collectible collision
        if (typeid(*entity) == typeid(Collectible))
        {
            // HandleCollectible(entity);
        }

        // Handling mouse collision by checking if this entity is colliding with the mouse
        

        // Static entites (like nature for example) doesn't need to do collision check,
        // considering it would never touch other entity
        
        entity = entity->next;
    }
}

void Grid::Move(Entity *entity, Vector2 addPos)
{
    // See which cell it was in before moving
    int oldCellX = (int)((entity->GetPosition().x - addPos.x) / CELL_SIZE);
    int oldCellY = (int)((entity->GetPosition().y - addPos.y) / CELL_SIZE);

    // See which cell it's moving to
    int cellX = (int)(entity->GetPosition().x / CELL_SIZE);
    int cellY = (int)(entity->GetPosition().y / CELL_SIZE);

    // If it didn't change cells, we're done
    if (oldCellX == cellX && oldCellY == cellY)
        return;

    // If it does change, unlink it from the list of its old cell
    if (entity->prev != NULL)
    {
        entity->prev->next = entity->next;
    }

    if (entity->next != NULL)
    {
        entity->next->prev = entity->prev;
    }

    // If it's the head of a list, remove it
    if (cells[oldCellX][oldCellY] == entity)
    {
        cells[oldCellX][oldCellY] = entity->next;
    }

    // Reset entity pointers
    entity->prev = NULL;
    entity->next = NULL;

    // Add it back to the grid at its new cell
    Add(entity);
}

void Grid::HandlePlayer(Entity* entity)
{
    // To do Player class specific things
    Player *player = static_cast<Player *>(entity);

    // Loop List:
    // 1. Loop to detect collectibles within player collect radius
    // Note: Might need to increase the collision detection radius in order to handle objects that are bigger than the cell itself
    Rectangle collectRadius = player->GetCollectRadiusRectangle();
    int minX = (int)collectRadius.x / CELL_SIZE;
    int minY = (int)collectRadius.y / CELL_SIZE;
    int maxX = std::min((int)(collectRadius.x + collectRadius.width) / CELL_SIZE, NUM_CELLS);
    int maxY = std::min((int)(collectRadius.y + collectRadius.height) / CELL_SIZE, NUM_CELLS);

    for(int x = minX; x <= maxX; x++)
    {
        for(int y = minY; y <= maxY; y++)
        {
            Entity *other = cells[x][y];
            while (other != NULL)
            {
                // If it is the player, skip it
                if(other == entity)
                {
                    other = other->next;
                    continue;
                }

                // Player and Collectible class collision
                if (typeid(*other) == typeid(Collectible))
                {
                    // To do Collectible class specific things
                    Collectible *collectible = static_cast<Collectible *>(other);
                    int cellX = collectible->GetPosition().x / CELL_SIZE;
                    int cellY = collectible->GetPosition().y / CELL_SIZE;

                    collectible->UpdatePlayerPosition(player->GetHitBoxPosition());

                    // If within radius, then change the state
                    if (CheckCollisionRecs(collectible->GetHitBox(), player->GetCollectRadiusRectangle()) &&
                        player->CanItemFitIntoInventory(collectible->GetID(), 1))
                    {
                        collectible->withinRadius = true;
                    }
                    else
                    {
                        collectible->withinRadius = false;
                    }

                    // If collide with player hit box, then remove it
                    if (CheckCollisionRecs(collectible->GetHitBox(), player->GetHitBox()) &&
                        player->CanItemFitIntoInventory(collectible->GetID(), 1))
                    {
                        // Add the item to player inventory
                        player->AddItemToInventory(collectible->GetID(), 1);

                        // Erase object from the linked list
                        if (other->prev != NULL)
                        {
                            other->prev->next = other->next;
                        }

                        if (other->next != NULL)
                        {
                            other->next->prev = other->prev;
                        }

                        // If it's the head of a list, remove it
                        if (cells[cellX][cellY] == other)
                        {
                            cells[cellX][cellY] = other->next;
                        }

                        // Reset entity pointers
                        other->prev = NULL;
                        other->next = NULL;
                    }
                }

                // Move to next other entity
                other = other->next;
            }
        }
    }

    // 2. Collision check with other uncollidable entities
}

// Note: It is not required at the moment since HandlePlayer already include this. Might delete this in the future
void Grid::HandleCollectible(Entity *entity)
{
    // To do Collectible class specific things
    Collectible *collectible = static_cast<Collectible *>(entity);

    // std::cout << "Ada collectible" << '\n';

    // Loop for others entities within cell
    Entity *other = entity->next;
    while (other != NULL)
    {
        // Collectible and Player class collision
        if (typeid(*other) == typeid(Player))
        {
            // To do Player class specific things
            Player *player = static_cast<Player *>(other);

            // std::cout << "Dan ada player" << '\n';

            collectible->UpdatePlayerPosition(player->GetHitBoxPosition());

            // If within radius, then change the state
            if (CheckCollisionRecs(collectible->GetHitBox(), player->GetCollectRadiusRectangle()) &&
                player->CanItemFitIntoInventory(collectible->GetID(), 1))
            {
                collectible->withinRadius = true;
            }
            else
            {
                collectible->withinRadius = false;
            }

            // If collide with player hit box, then remove it
            if (CheckCollisionRecs(collectible->GetHitBox(), player->GetHitBox()) &&
                player->CanItemFitIntoInventory(collectible->GetID(), 1))
            {
                // Add the item to player inventory
                player->AddItemToInventory(collectible->GetID(), 1);

                // Erase object from the linked list
                if (entity->prev != NULL)
                {
                    entity->prev->next = entity->next;
                }

                if (entity->next != NULL)
                {
                    entity->next->prev = entity->prev;
                }

                // If it's the head of a list, remove it
                int cellX = (int)(entity->GetPosition().x / CELL_SIZE);
                int cellY = (int)(entity->GetPosition().y / CELL_SIZE);

                if (cells[cellX][cellY] == entity)
                {
                    cells[cellX][cellY] = entity->next;
                }

                // Finish the check since this item no longer exist
                return;
            }
        }

        // Move to next other entity
        other = other->next;
    }
}

void Grid::DrawVisibleObjects(Vector2 cameraPos)
{
    // Calculate the boundaries for the drawing (which is the visible view + an offset of the player camera)
    int minX = std::max((int)(cameraPos.x - screenWidth / scale) / CELL_SIZE, 0);
    int minY = std::max((int)(cameraPos.y - screenHeight / scale) / CELL_SIZE, 0);
    int maxX = std::min((int)(cameraPos.x + screenWidth / scale) / CELL_SIZE, NUM_CELLS - 1);
    int maxY = std::min((int)(cameraPos.y + screenHeight / scale) / CELL_SIZE, NUM_CELLS - 1);

    // Collect all entities in the cell and sort them before drawing for consistent order
    std::vector<Entity *> drawList;
    for(int x = minX; x <= maxX; x++)
    {
        for(int y = minY; y <= maxY; y++)
        {
            Entity* entity = cells[x][y];

            while(entity != NULL)
            {
                drawList.push_back(entity);
                entity = entity->next;
            }
        }
    }

    // Sort them based on their position in the world
    std::sort(drawList.begin(), drawList.end(), CompareObjectPosition());

    // Draw them on order
    for (long long unsigned int i = 0; i < drawList.size(); i++)
    {
        drawList[i]->Draw();
    }
}

void Grid::HandleMouse(Entity *entity)
{

}