#ifndef NATURE_H
#define NATURE_H

#include "raylib.h"
#include "Static.h"

class Nature : public Entity
{
protected:

public:
    // Constructor
    Nature(Vector2 _position, int _type, Texture2D *_textures);
    // Draw
    void Draw() const override;
    // Update
    void Update() override;
};

#endif // PLAYER_H