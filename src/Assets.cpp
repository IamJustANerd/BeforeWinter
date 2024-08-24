#include "../include/Assets.h"
#include <iostream>

Image NatureImg[100];
Texture2D NatureTex[100];

// Load all images
void LoadAllImage()
{
    // Nature
    NatureImg[0] = LoadImage("../graphics/tree_green_1.png");
    NatureImg[1] = LoadImage("../graphics/tree_green_2.png");
    NatureImg[2] = LoadImage("../graphics/tree_green_3.png");
    NatureImg[3] = LoadImage("../graphics/bush_green_1.png");
    NatureImg[4] = LoadImage("../graphics/bush_green_2.png");
}

// Resize all images
void ResizeAllImage()
{
    // Nature
    // Trees
    for (int i = 0; i < 3; i++)
    {
        ImageResizeNN(&NatureImg[i], 144, 144);
    }

    // Bushes
    for (int i = 3; i < 5; i++)
    {
        ImageResizeNN(&NatureImg[i], 64, 64);
    }
}

// Load all textures from images
void LoadAllTexture()
{
    for (int i = 0; i < 5; i++)
    {
        NatureTex[i] = LoadTextureFromImage(NatureImg[i]);
        std::cout << NatureTex[i].width << ' ' << NatureTex[i].height << '\n';
    }
}

// Unload all images
void UnloadAllImage()
{
    for (int i = 0; i < 5; i++)
    {
        UnloadImage(NatureImg[i]);
    }
}

// Unload all textures
void UnloadAllTexture()
{
    for (int i = 0; i < 5; i++)
    {
        UnloadTexture(NatureTex[i]);
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