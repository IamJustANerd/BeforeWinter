#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Dynamic.h"

class Player : public Dynamic
{
private:
    float stamina = 100.0f;

public:
    // Constructor
    Player(Vector2 _position);

    // Player movements
    void Movements() override;

    // Draw
    void Draw() const override;
};

#endif // PLAYER_H