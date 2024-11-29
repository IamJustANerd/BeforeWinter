#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "../include/raylib_includes.h"
#include "Entity.h"
#include <string>
#include <typeinfo>

struct cellPos
{
    int x, y;
};

class Dynamic : public Entity
{
protected:
    float speed = 0;
    float rotation = 0;
    bool isMoving = false;
    bool isAttacking = false;

public:
    // Movements
    virtual void Movements() = 0;

    // Attack
    virtual void Attack() = 0;

    // Check if the dynamic entity is colliding with uncollidable
    bool IsCollidingWithUncollidable();

    // Find a certain target (tree, player, etc)
    Entity* FindTarget(const std::type_info& targetClass, int targetType);
};

#endif // PLAYER_H