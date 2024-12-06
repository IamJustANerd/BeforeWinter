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
protected:
    bool hasDestination;
    bool isCollidingWithOther;
    bool priorityMove;
    int rotationCounter;
    int distanceTolerance = 5;
    int rotationCounterTolerance = 50;
    Rectangle destination;
    Entity* target;

public:
    // Set destination
    virtual void SetDestination() = 0;

    bool ReachDestination();
};

#endif