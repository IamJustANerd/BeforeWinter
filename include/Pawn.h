#ifndef PAWN_H
#define PAWN_H

#include "../include/NPC.h"
#include "../include/raylib_includes.h"

class Pawn : public NPC
{
private:
    bool isWorking;
    bool isCarrying;
    int carriedObjectValue;
    int carry = 0;
    int maxCarry = 1;
public:
    Pawn(Vector2 _position, int type, Grid *_grid);

    // Pawn Movements
    void HandleMovements();

    // Update Pawn
    void Update() override;

    // Update sprite frame
    void UpdateSpriteFrame() override;

    // Change animation
    void ChangeAnimation(State newState) override;

    // Draw
    void Draw() const override;

    // Attack
    void Attack() override;

    // To submit harvest
    void Submit();

    // Set destination
    void SetDestination() override;
};

#endif