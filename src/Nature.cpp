#include "../include/Nature.h"
#include <iostream>

Nature::Nature(Vector2 _position, int _id, Texture2D* _textures)
{
    position = _position;
    id = _id;
    textures = _textures;

    width = textures->width;
    height = textures->height;
}

void Nature::Draw() const
{
    // DrawRectangle(position.x, position.y, width, height, PURPLE);
    DrawTexture(textures[id], position.x, position.y, WHITE);
}

void Nature::Update()
{
    
}