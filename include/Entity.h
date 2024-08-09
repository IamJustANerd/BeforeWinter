#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"

class Entity
{
protected:
    Vector2 position;
    int width, height;
    int type;
    Rectangle hitBox;
    Texture2D* textures;

public:
    virtual void Draw() const = 0;
    virtual void Update();
    Vector2 GetPosition() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetType() const;
    Rectangle GetHitBox() const;
};

#endif