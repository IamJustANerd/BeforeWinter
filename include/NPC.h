#ifndef NPC_H
#define NPC_H

#include "raylib_includes.h"
#include "../include/Dynamic.h"
#include "../include/Player.h"

class NPC : public Dynamic
{
private:
    bool isWorking;
    bool isCarrying;
    int carriedObjectValue;
    Player* targetPlayer;

public:
    NPC(Vector2 _position, int type, Player *_player, Grid *_grid);
    // NPC movements
    void Movements() override;

    // Update NPC
    void Update() override;

    // Update sprite frame
    void UpdateSpriteFrame() override;

    // Draw
    void Draw() const override;

    // Attack
    void Attack() override;
};

#endif