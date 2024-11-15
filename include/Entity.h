#ifndef ENTITY_H
#define ENTITY_H
#define FPS 60

#include "../include/raylib_includes.h"
#include "../include/Assets.h"

// Entity position in the grid for spatial partitioning

// Forward the definition of grid first
class Grid;

enum class State
{
    idle,
    running,
    attacking,
    dying,
};

class Entity
{
    // Decalre as friend class for easier access
    friend class Grid;

protected:
    Vector2 position;
    // Direction is in x and y (-1.0 for up and left, 1.0 for down and right, 0 for neither up down or left right [lets call it as neutral])
    Vector2 direction;
    int width, height;
    int type;
    int id;
    Rectangle hitBox;
    Rectangle frameRec;
    Texture2D* textures;
    Animation* animations;
    bool isUncollidable;
    bool isCollidingWithMouse = false;
    float rotation;
    int frameCounter = 0;
    int curFrame = 0;
    State curState;
    
    // Pointer to the grid
    Grid* grid;

public:
    virtual void Draw() const = 0;
    virtual void Update() = 0;
    Vector2 GetPosition() const;
    Vector2 GetDirection() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetID() const;
    Rectangle GetHitBox() const;
    bool GetIsUncollidable() const;
    Rectangle FlipTexture(Rectangle frameRec) const;

    // Linked list for faster insertion and deletion
    Entity *prev;
    Entity *next;
};

#endif