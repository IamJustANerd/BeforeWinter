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
    int attackCooldown = 60;
    int attackCooldownCounter = 0;
    Rectangle attackBox[4];

public:
    // Movements
    virtual void Movements() = 0;

    // Attack
    virtual void Attack() = 0;

    // Check if the dynamic entity is colliding with uncollidable
    bool IsCollidingWithUncollidable();

    // Find a certain target (tree, player, etc)
    Entity* FindTarget(const std::type_info& targetClass, int targetType, bool shareTarget);

    // Attack all entities within the attack rectangle by checking intersected grid cells
    void AttackEntitiesInRange(Rectangle attackRect, int damage, const std::vector<const std::type_info*>& targetTypes);
};

#endif // PLAYER_H