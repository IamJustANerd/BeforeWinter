#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "../include/raylib_includes.h"
#include "Entity.h"
#include <string>

class Dynamic : public Entity
{
protected:
    float speed = 0;
    float rotation = 0;

public:
    // Movements
    virtual void Movements() = 0;

    // Check if the dynamic entity is colliding with uncollidable
    bool IsCollidingWithUncollidable(std::string moveDir);
};

#endif // PLAYER_H