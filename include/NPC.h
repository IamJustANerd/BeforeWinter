#ifndef NPC_H
#define NPC_H

#include "raylib_includes.h"
#include "../include/Dynamic.h"
#include "../include/Player.h"
#include "../include/Grid.h"
#include "../include/Nature.h"
#include "../include/Building.h"

class NPC : public Dynamic
{
private:
    bool isWorking;
    bool isCarrying;
    bool hasDestination;
    bool isCollidingWithOther;
    bool priorityMove;
    int carriedObjectValue;
    int rotationCounter;
    int count = 0;
    int distanceTolerance = 5;
    int rotationCounterTolerance = 50;
    int carry = 0;
    int maxCarry = 1;
    Rectangle destination;
    Entity* target;

public:
    NPC(Vector2 _position, int type, Grid *_grid);
    // NPC movements
    void Movements() override;

    // Update NPC
    void Update() override;

    // Update sprite frame
    void UpdateSpriteFrame() override;

    // Hit animation
    void HitAnimation() override;

    // Draw
    void Draw() const override;

    // Attack
    void Attack() override;

    // Set destination
    void SetDestination();
};

#endif