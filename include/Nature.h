#ifndef NATURE_H
#define NATURE_H

#include "../include/raylib_includes.h"
#include "Static.h"

class Nature : public Entity
{
protected:
    bool isHarvested = false; // It will be true when it lost all of it's hp

public:
    // Constructor
    Nature(Vector2 _position, int _type, Grid* _grid);
    // Draw
    void Draw() const override;
    // Update
    void Update() override;
    // Update sprite frame
    void UpdateSpriteFrame() override;
    // Change animation
    void ChangeAnimation(State newState) override;
};

#endif