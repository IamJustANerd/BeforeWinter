#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "raylib.h"
#include "Entity.h"

class Dynamic : public Entity
{
protected:
    float speed = 0;
    float rotation = 0;

public:
    // Movements
    virtual void Movements() = 0;
};

#endif // PLAYER_H