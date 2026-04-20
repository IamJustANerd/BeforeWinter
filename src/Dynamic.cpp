#include "../include/Dynamic.h"
#include "../include/Grid.h"
#include "../include/Player.h"
#include "../include/Nature.h"
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <queue>

bool Dynamic::IsCollidingWithUncollidable()
{
    // Calculate the cells needed to be check on
    int minX = std::max((int)hitBox.x / grid->CELL_SIZE - 2, 0);
    int minY = std::max((int)hitBox.y / grid->CELL_SIZE - 2, 0);
    int maxX = std::min((int)(hitBox.x + hitBox.width) / grid->CELL_SIZE + 2, grid->NUM_CELLS - 1);
    int maxY = std::min((int)(hitBox.y + hitBox.height) / grid->CELL_SIZE + 2, grid->NUM_CELLS - 1);

    // Getting the read only cell
    const Entity *const(&cells)[Grid::NUM_CELLS][Grid::NUM_CELLS] = grid->GetReadOnlyCells();

    bool isColliding = false;

    // Rectangle to check collision based on the move direction
    Rectangle collisionCheck = this->GetHitBox();
    // Increased the collision check radius to prevent missed obstacles
    collisionCheck.x += moveDirection[moveDirectionIndex].x * 2;
    collisionCheck.y += moveDirection[moveDirectionIndex].y * 2;

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            const Entity *entity = cells[x][y];
            while (entity != NULL)
            {
                if (this == entity || !entity->GetIsUncollidable() || !entity->GetIsAlive())
                {
                    entity = entity->next;
                    continue;
                }

                if (CheckCollisionRecs(collisionCheck, entity->GetHitBox()))
                {
                    isColliding = true;
                }

                entity = entity->next;
            }
        }
    }

    // Return the check value
    return isColliding;
}

Entity* Dynamic::FindTarget(const std::type_info& targetClass, int targetType, bool shareTarget)
{
    // Count initial cell position
    int cellX = (int)(position.x / grid->CELL_SIZE);
    int cellY = (int)(position.y / grid->CELL_SIZE);

    // Get the grid cells
    const Entity *const(&cells)[Grid::NUM_CELLS][Grid::NUM_CELLS] = grid->GetReadOnlyCells();

    // Mark visited cell to avoid repetition
    bool visited[grid->NUM_CELLS][grid->NUM_CELLS];

    // The initial state for all cells is not visited
    for(int i = 0; i < grid->NUM_CELLS; i++)
    {
        for(int j = 0; j < grid->NUM_CELLS; j++)
        {
            visited[i][j] = false;
        }
    }

    // Mark the initial cell as visited
    visited[cellX][cellY] = true;

    // Use BFS to find the target
    std::queue<cellPos> nextCell;

    nextCell.push({cellX, cellY});

    while(!nextCell.empty())
    {
        // Get the current cell position
        cellPos curCellPos = nextCell.front();
        const Entity *curCell = cells[curCellPos.x][curCellPos.y];

        // Delete it
        nextCell.pop();

        // Check if the target is within this cell
        while (curCell != NULL)
        {
            // Check if this entity fit the target criteria
            if (typeid(*curCell) == targetClass)
            {
                // Can't lock on target that is not shareable (for example, pawn can't share the same tree)
                if ((curCell->GetType() == targetType && !curCell->GetIsTargeted()) || shareTarget)
                {
                    // std::cout << curCell->GetHitBoxPosition().x << ' ' << curCell->GetHitBoxPosition().y << '\n';

                    // Mark this object as targeted
                    const_cast<Entity *>(curCell)->SetIsTargeted(true);
                    
                    return const_cast<Entity *>(curCell);
                }
            }
            curCell = curCell->next;
        }

        // If it is not in this cell, then look for it on adjacent cells
        // Top
        if(curCellPos.y - 1 >= 1)
        {
            if (!visited[curCellPos.x][curCellPos.y - 1])
            {
                nextCell.push({curCellPos.x, curCellPos.y - 1});
                visited[curCellPos.x][curCellPos.y - 1] = true;
            }
        }

        // Bottom
        if (curCellPos.y + 1 <= grid->NUM_CELLS - 1)
        {
            if (!visited[curCellPos.x][curCellPos.y + 1])
            {
                nextCell.push({curCellPos.x, curCellPos.y + 1});
                visited[curCellPos.x][curCellPos.y + 1] = true;
            }
        }

        // Left
        if (curCellPos.x - 1 >= 1)
        {
            if (!visited[curCellPos.x - 1][curCellPos.y])
            {
                nextCell.push({curCellPos.x - 1, curCellPos.y});
                visited[curCellPos.x - 1][curCellPos.y] = true;
            }
        }

        // Right
        if (curCellPos.x + 1 <= grid->NUM_CELLS - 1)
        {
            if (!visited[curCellPos.x + 1][curCellPos.y])
            {
                nextCell.push({curCellPos.x + 1, curCellPos.y});
                visited[curCellPos.x + 1][curCellPos.y] = true;
            }
        }
    }    

    // If it doesn't exist, return NULL
    return NULL;
}

void Dynamic::AttackEntitiesInRange(Rectangle attackRect, int damage, const std::vector<const std::type_info*>& targetTypes)
{
    // Calculate the cells that the attackRect intersects, with +1 padding
    // to catch entities whose hitbox extends beyond their registered cell
    int minX = std::max((int)attackRect.x / grid->CELL_SIZE - 1, 0);
    int minY = std::max((int)attackRect.y / grid->CELL_SIZE - 1, 0);
    int maxX = std::min((int)(attackRect.x + attackRect.width) / grid->CELL_SIZE + 1, grid->NUM_CELLS - 1);
    int maxY = std::min((int)(attackRect.y + attackRect.height) / grid->CELL_SIZE + 1, grid->NUM_CELLS - 1);

    // Getting the read only cells
    const Entity *const(&cells)[Grid::NUM_CELLS][Grid::NUM_CELLS] = grid->GetReadOnlyCells();

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            const Entity *entity = cells[x][y];
            while (entity != NULL)
            {
                // Skip self
                if (entity == this)
                {
                    entity = entity->next;
                    continue;
                }

                // Skip dead entities
                if (!entity->GetIsAlive())
                {
                    entity = entity->next;
                    continue;
                }

                // Check if this entity's type is in the target whitelist
                bool isValidTarget = false;
                for (const std::type_info* targetType : targetTypes)
                {
                    if (typeid(*entity) == *targetType)
                    {
                        isValidTarget = true;
                        break;
                    }
                }

                if (!isValidTarget)
                {
                    entity = entity->next;
                    continue;
                }

                // Check if the entity's hitbox collides with the attack rectangle
                if (CheckCollisionRecs(attackRect, entity->GetHitBox()))
                {
                    const_cast<Entity*>(entity)->ReduceHealthPoint(damage);
                }

                entity = entity->next;
            }
        }
    }
}