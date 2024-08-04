#include "../include/Nature.h"
#include <iostream>

Nature::Nature(Vector2 _position, int _type, Texture2D* _textures)
{
    position = _position;
    type = _type;
    textures = _textures;
}

void Nature::Draw() const
{
    DrawTexture(textures[type], position.x, position.y, WHITE);
}