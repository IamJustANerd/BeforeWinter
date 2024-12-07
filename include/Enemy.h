#ifndef ENEMY_H
#define ENEMY_H
#include "../include/NPC.h"

class Enemy : public NPC
{
private:
    bool withinAttackRange;
    int stunCounter = 0;
    int stunDuration = 60; // After doing an attack animation, enemy type will stand still for a certain period amount of time

public:
    Enemy(Vector2 _position, int type, Grid *_grid);

    // Enemy Movements
    void HandleMovements();

    // Update Enemy
    void Update() override;

    // Update sprite frame
    void UpdateSpriteFrame() override;

    // Change animation
    void ChangeAnimation(State newState) override;

    // Draw
    void Draw() const override;

    // Attack
    void Attack() override;

    // Set destination
    void SetDestination() override;
};

#endif