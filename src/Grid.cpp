#include "../include/Grid.h"
#include "../include/Entity.h"
#include "../include/Player.h"
#include "../include/Collectible.h"
#include "../include/Nature.h"
#include "../include/raylib.h"
#include <cstddef>
#include <algorithm>
#include <vector>
#include <iostream>

// NOTE: NEED TO FIX DOUBLED ITEM COLLECTION (DONE)

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
    // Calculate the grid cells that the entity spans
    int minX = (int)(entity->GetPosition().x / Grid::CELL_SIZE);
    int minY = (int)(entity->GetPosition().y / Grid::CELL_SIZE);
    int maxX = std::min((int)((entity->GetPosition().x + entity->GetWidth()) / Grid::CELL_SIZE), Grid::NUM_CELLS - 1);
    int maxY = std::min((int)((entity->GetPosition().y + entity->GetHeight()) / Grid::CELL_SIZE), Grid::NUM_CELLS - 1);

    // Add the entity to all the cells it spans
    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            Entity *currentHead = cells[x][y];

            // If the cell is empty, just place the entity
            if (currentHead == NULL)
            {
                cells[x][y] = entity;
                entity->next = NULL;
                entity->prev = NULL;
            }
            else
            {
                // Insert the entity at the beginning of the list
                entity->next = currentHead;
                entity->prev = NULL;
                currentHead->prev = entity;
                cells[x][y] = entity;
            }
        }
    }
}

void Grid::UpdateGrid()
{
    // Reset the hasBeenUpdated flag for all entities
    for (int i = 0; i < NUM_CELLS; i++)
    {
        for (int j = 0; j < NUM_CELLS; j++)
        {
            Entity *entity = cells[i][j];
            while (entity != NULL)
            {
                entity->hasBeenUpdated = false;
                entity = entity->next;
            }
        }
    }

    // Handle each cell (update etc)
    for(int i = 0; i < NUM_CELLS; i++)
    {
        for(int j = 0; j < NUM_CELLS; j++)
        {
            HandleCell(cells[i][j]);
        }
    }
}

void Grid::HandleCell(Entity* entity)
{
    // Handle collisions on a cell
    // Check collisions of each entity with the others inside the cell
    while (entity != NULL)
    {   
        if(!entity->hasBeenUpdated)
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

            // Static entites (like nature for example) doesn't need to do collision check,
            // considering it would never touch other entity

            // Mark this entity hasBeenUpdated as true to prevent a single entity being updated twice
            entity->hasBeenUpdated = true;
        }

        entity = entity->next;
    }
}

void Grid::Move(Entity *entity, Vector2 addPos)
{
    // Calculate the grid cells the entity was in before moving
    int oldMinX = (int)((entity->GetPosition().x - addPos.x) / CELL_SIZE);
    int oldMinY = (int)((entity->GetPosition().y - addPos.y) / CELL_SIZE);
    int oldMaxX = std::min((int)((entity->GetPosition().x - addPos.x + entity->GetWidth()) / CELL_SIZE), NUM_CELLS - 1);
    int oldMaxY = std::min((int)((entity->GetPosition().y - addPos.y + entity->GetHeight()) / CELL_SIZE), NUM_CELLS - 1);

    // Calculate the grid cells the entity is moving to
    int minX = (int)(entity->GetPosition().x / CELL_SIZE);
    int minY = (int)(entity->GetPosition().y / CELL_SIZE);
    int maxX = std::min((int)((entity->GetPosition().x + entity->GetWidth()) / CELL_SIZE), NUM_CELLS - 1);
    int maxY = std::min((int)((entity->GetPosition().y + entity->GetHeight()) / CELL_SIZE), NUM_CELLS - 1);

    // If it didn't change cells, we're done
    if (oldMinX == minX && oldMinY == minY && oldMaxX == maxX && oldMaxY == maxY)
        return;

    // If it does change, unlink it from the list of its old cells
    for (int x = oldMinX; x <= oldMaxX; x++)
    {
        for (int y = oldMinY; y <= oldMaxY; y++)
        {
            Entity *temp = cells[x][y];
            Entity *prev = NULL;
            while (temp != NULL && temp != entity)
            {
                if (typeid(*temp) == typeid(Collectible))
                {
                    std::cout << "Huh" << '\n';
                }
                prev = temp;
                temp = temp->next;
            }
            
            // If the entity was found
            if (temp == entity)
            {
                if(typeid(*temp) == typeid(Collectible))
                {
                    std::cout << "Huh" << '\n';
                }
                else if(typeid(*temp) == typeid(Player))
                {
                    std::cout << "Oke" << '\n';
                }
                // Unlink the entity
                if (prev != NULL)
                {
                    prev->next = temp->next;
                }
                // If the entity was the head of the list, remove it
                else
                {
                    cells[x][y] = temp->next;
                }

                if (temp->next != NULL)
                {
                    temp->next->prev = prev;
                }
            }
            else
            {
                std::cout << "Kok iso" << '\n';
            }
        }
    }

    // Add it back to the grid at its new cell(s)
    Add(entity);
}

void Grid::HandlePlayer(Entity* entity)
{
    // To do Player class specific things
    Player *player = static_cast<Player *>(entity);

    // Loop List:
    // 1. Loop to detect collectibles within player collect radius
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
                    // std::cout << "ADA COLLECTIBLE" << '\n';
                    // To do Collectible class specific things
                    Collectible *collectible = static_cast<Collectible *>(other);

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
                        std::cout << "HIT" << '\n';
                        std::cout << player->GetHitBox().x << ' ' << player->GetHitBox().y << ' ' << player->GetHitBox().width << ' ' << player->GetHitBox().height << '\n';
                        std::cout << collectible->GetHitBox().x << ' ' << collectible->GetHitBox().y << ' ' << collectible->GetHitBox().width << ' ' << collectible->GetHitBox().height << '\n';
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
                    }
                }

                // Move to next other entity
                other = other->next;
            }
        }
    }

    // 2. Collision check with other uncollidable entities

}

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
            // To do Collectible class specific things
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
                std::cout << "HIT" << '\n';
                std::cout << player->GetHitBox().x << ' ' << player->GetHitBox().y << ' ' << player->GetHitBox().width << ' ' << player->GetHitBox().height << '\n';
                std::cout << collectible->GetHitBox().x << ' ' << collectible->GetHitBox().y << ' ' << collectible->GetHitBox().width << ' ' << collectible->GetHitBox().height << '\n';

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
    // Calculate the boundaries for the drawing
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
                entity->hasBeenDrawn = false;
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
        if(!drawList[i]->hasBeenDrawn)
        {
            drawList[i]->Draw();
            drawList[i]->hasBeenDrawn = true;
        }
    }
}