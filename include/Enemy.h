#ifndef ENEMY_H
#define ENEMY_H
#include "../include/NPC.h"

class Enemy : public NPC
{
private:
    bool isWorking;
    bool isCarrying;
    int carriedObjectValue;
    int carry = 0;
    int maxCarry = 10;

public:
    Enemy(Vector2 _position, int type, Grid *_grid);

    // Enemy Movements
    void Movements() override;

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

    // To submit harvest
    void Submit();

    // Set destination
    void SetDestination() override;
};

#endif