#ifndef NATURE_H
#define NATURE_H

#include "../include/raylib_includes.h"
#include "Static.h"

class Nature : public Entity
{
protected:

public:
    // Constructor
    Nature(Vector2 _position, int _type, Grid* _grid);
    // Draw
    void Draw() const override;
    // Update
    void Update() override;
    // Update sprite frame/
    void UpdateSpriteFrame() override;
};

#endif