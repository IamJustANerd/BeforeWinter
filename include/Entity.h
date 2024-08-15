#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"

class Entity
{
protected:
    Vector2 position;
    int width, height;
    int type;
    int id;
    Rectangle hitBox;
    Texture2D* textures;

public:
    virtual void Draw() const = 0;
    virtual void Update() = 0;
    Vector2 GetPosition() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetID() const;
    Rectangle GetHitBox() const;
};

#endif