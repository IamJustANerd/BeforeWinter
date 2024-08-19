#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "raylib.h"
#include "Dynamic.h"

class Collectible : public Dynamic
{
    private:
    float acceleration = 0.01f;
    float maxSpeed = 1.0f;
    Vector2 direction;
    Vector2 playerPos;
    int time = 0;

    public:
    bool withinRadius = false;
    // Constructur
    Collectible(Vector2 _position, int _id, Grid *_grid);

    // Update
    void Update() override;

    // Movement
    void Movements() override;
    
    // Draw
    void Draw() const override;

    // Update player pos
    void UpdatePlayerPosition(Vector2 _playerPos);
};

#endif