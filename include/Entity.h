#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"

// Entity position in the grid for spatial partitioning

// Forward the definition of grid first
class Grid;

class Entity
{
    // Decalre as friend class for easier access
    friend class Grid;

protected:
    Vector2 position;
    int width, height;
    int type;
    int id;
    Rectangle hitBox;
    Texture2D* textures;
    bool isUncollidable;
    bool isCollidingWithMouse = false;

    // Pointer to the grid
    Grid* grid;
    

public:
    virtual void Draw() const = 0;
    virtual void Update() = 0;
    Vector2 GetPosition() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetID() const;
    Rectangle GetHitBox() const;
    bool GetIsUncollidable() const;

    // Linked list for faster insertion and deletion
    Entity *prev;
    Entity *next;
};

#endif