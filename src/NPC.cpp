#include "../include/NPC.h"
#include "../include/GameManager.h"
#include <iostream>
#include <typeinfo>

bool NPC::ReachDestination()
{
    return CheckCollisionRecs({hitBox.x - distanceTolerance, hitBox.y - distanceTolerance, hitBox.width + distanceTolerance * 2, hitBox.height + distanceTolerance * 2}, destination);
}