#include "../include/Dynamic.h"
#include "../include/Grid.h"
#include <algorithm>
#include <iostream>

bool Dynamic::IsCollidingWithUncollidable(std::string moveDir)
{
    // Calculate the cells needed to be check on
    int minX = (int)hitBox.x / grid->CELL_SIZE;
    int minY = (int)hitBox.y / grid->CELL_SIZE;
    int maxX = std::min((int)(hitBox.x + hitBox.width) / grid->CELL_SIZE, grid->NUM_CELLS);
    int maxY = std::min((int)(hitBox.y + hitBox.height) / grid->CELL_SIZE, grid->NUM_CELLS);

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
                    std::cout << "HIT" << '\n';
                    std::cout << collisionCheck.x << ' ' << collisionCheck.y << ' ' << collisionCheck.width << ' ' << collisionCheck.height << '\n';
                    std::cout << entity->GetHitBox().x << ' ' << entity->GetHitBox().y << ' ' << entity->GetHitBox().width << ' ' << entity->GetHitBox().height << '\n';
                    isColliding = true;
                }

                entity = entity->next;
            }
        }
    }

    // Return the check value
    return isColliding;
}