#include "../include/Nature.h"
#include <iostream>

Nature::Nature(Vector2 _position, int _type, Texture2D* _textures)
{
    position = _position;
    type = _type;
    textures = _textures;

    width = textures->width;
    height = textures->height;
}

void Nature::Draw() const
{
    DrawRectangle(position.x, position.y, width, height, PURPLE);
    DrawTexture(textures[type], position.x, position.y, WHITE);
}