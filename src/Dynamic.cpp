#include "../include/Dynamic.h"
#include "../include/Grid.h"
#include "../include/Player.h"
#include "../include/Nature.h"
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <queue>

bool Dynamic::IsCollidingWithUncollidable(std::string moveDir)
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
    if (moveDir == "Left")
    {
        collisionCheck.x -= 1;
    }
    else if (moveDir == "Right")
    {
        collisionCheck.x += 1;
    }
    else if (moveDir == "Up")
    {
        collisionCheck.y -= 1;
    }
    else if (moveDir == "Down")
    {
        collisionCheck.y += 1;
    }

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            const Entity *entity = cells[x][y];
            while (entity != NULL)
            {
                if (this == entity || !entity->GetIsUncollidable())
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

Rectangle Dynamic::FindTarget(const std::type_info& targetClass, int targetType)
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
                if (curCell->GetType() == targetType)
                {
                    // Note: might want to mark the targeted entity in the future (for example, to prevent more than one pawn to cut a single tree)
                    std::cout << curCell->GetHitBoxPosition().x << ' ' << curCell->GetHitBoxPosition().y << '\n';
                    return curCell->GetHitBox();
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

    // If it doesn't exist, return {-1, -1}
    return {-1.0, -1.0};
}