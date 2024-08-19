#include "../include/Grid.h"
#include "../include/Entity.h"
#include "../include/Player.h"
#include "../include/Collectible.h"
#include "../include/raylib.h"
#include <cstddef>
#include <iostream>

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

void Grid::Add(Entity* entity)
{
    // Determine which grid cell it's in.
    int cellX = (int)(entity->GetPosition().x / Grid::CELL_SIZE);
    int cellY = (int)(entity->GetPosition().y / Grid::CELL_SIZE);

    // Add to the front of list for the cell it's in.
    entity->prev = NULL;
    entity->next = cells[cellX][cellY];
    cells[cellX][cellY] = entity;

    if (entity->next != NULL)
    {
        entity->next->prev = entity;
    }

    std::cout << cellX << ' ' << cellY << '\n';
}

void Grid::UpdateGrid()
{
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
        // Handling player collision
        if (typeid(*entity) == typeid(Player))
        {
            HandlePlayerCollisions(entity);            
        }

        // Handling collectible collision
        if (typeid(*entity) == typeid(Collectible))
        {
            HandleCollectibleCollisions(entity);
        }

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

    // Add it back to the grid at its new cell
    Add(entity);
}

void Grid::HandlePlayerCollisions(Entity* entity)
{
    // To do Player class specific things
    Player *player = static_cast<Player *>(entity);
    // Loop for others entities within cell
    Entity *other = entity->next;
    while (other != NULL)
    {
        // Player and Collectible class collision
        if (typeid(*other) == typeid(Collectible))
        {
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
            collectible->Update();
        }

        // Move to next other entity
        other = other->next;
    }
}

void Grid::HandleCollectibleCollisions(Entity *entity)
{
    // To do Collectible class specific things
    Collectible *collectible = static_cast<Collectible *>(entity);

    // std::cout << "Ada collectible" << '\n';

    // Loop for others entities within cell
    Entity *other = entity->next;
    collectible->Update();
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