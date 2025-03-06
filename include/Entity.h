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
    light_attacking,
    heavy_attacking,
    hit,
    dying,
    decaying
};

class Entity
{
    // Decalre as friend class for easier access
    friend class Grid;

protected:
    Vector2 position;
    // Direction is in x and y (-1.0 for up and left, 1.0 for down and right, 0 for neither up down or left right [lets call it as neutral])
    Vector2 direction; // For animation
    // Left, Up, Right, Down
    Vector2 moveDirection[4] = {
        {-1.0f, 0.0f}, {0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}
    };
    int moveDirectionIndex; // For movement (not to be confused with Vector2 direction)
    int width, height;
    int type;
    int id;
    Rectangle hitBox;
    Rectangle frameRec;
    bool isUncollidable;
    bool isCollidingWithMouse = false;
    bool isAlive = true;
    bool isDecay = false;
    float rotation;
    int frameCounter = 0;
    int curFrame = 0;
    State curState;
    int healthPoint = 0;
    int attackPoint = 1;
    bool isTargeted = false;

    // Pointer to the grid
    Grid* grid;

public:
    virtual void Draw() const = 0;
    virtual void Update() = 0;
    // Update sprite frame (which will depends on the animation and state)
    virtual void UpdateSpriteFrame() = 0;
    virtual void ChangeAnimation(State newState);
    Vector2 GetPosition() const;
    Vector2 GetDirection() const;
    Vector2 GetHitBoxPosition() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetID() const;
    int GetType() const;
    Rectangle GetHitBox() const;
    bool GetIsUncollidable() const;
    Rectangle FlipTexture(Rectangle frameRec) const;
    bool GetIsTargeted() const;
    bool GetIsAlive() const;
    void SetIsTargeted(bool newState);
    void ReduceHealthPoint(int decrease);
    void DeathAnimation(int deathType);
    void DecayAnimation(int decayType);

    // Linked list for faster insertion and deletion
    Entity *prev;
    Entity *next;
    Entity *target;
};

#endif