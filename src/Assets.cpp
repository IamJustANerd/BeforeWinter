#include "../include/Assets.h"
#include <iostream>

Image playerImg[5];
Image natureImg[100];

Texture2D natureTex[100];
Texture2D playerTex[5];

// Load all images
void LoadAllImage()
{
    // Player
    playerImg[0] = LoadImage("../graphics/player/knightBlue.png");

    // Nature
    natureImg[0] = LoadImage("../graphics/tree_green_1.png");
    natureImg[1] = LoadImage("../graphics/tree_green_2.png");
    natureImg[2] = LoadImage("../graphics/tree_green_3.png");
    natureImg[3] = LoadImage("../graphics/bush_green_1.png");
    natureImg[4] = LoadImage("../graphics/bush_green_2.png");
}

// Resize all images
void ResizeAllImage()
{
    // Nature
    // Trees
    for (int i = 0; i < 3; i++)
    {
        ImageResizeNN(&natureImg[i], 144, 144);
    }

    // Bushes
    for (int i = 3; i < 5; i++)
    {
        ImageResizeNN(&natureImg[i], 64, 64);
    }
}

// Load all textures from images
void LoadAllTexture()
{
    for (int i = 0; i < 5; i++)
    {
        natureTex[i] = LoadTextureFromImage(natureImg[i]);
        std::cout << natureTex[i].width << ' ' << natureTex[i].height << '\n';
    }
}

// Unload all images
void UnloadAllImage()
{
    for (int i = 0; i < 5; i++)
    {
        UnloadImage(natureImg[i]);
    }
}

// Unload all textures
void UnloadAllTexture()
{
    for (int i = 0; i < 5; i++)
    {
        UnloadTexture(natureTex[i]);
    }
}

// Setup assets (images, textures, etc)
void SetupAssets()
{
    LoadAllImage();
    ResizeAllImage();
    LoadAllTexture();
    UnloadAllImage();
}