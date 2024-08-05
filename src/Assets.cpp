#include "../include/Assets.h"

Image NatureImg[100];
Texture2D NatureTex[100];

// Load all images
void LoadAllImage()
{
    NatureImg[0] = LoadImage("../graphics/earth.png");
    NatureImg[1] = LoadImage("../graphics/moon.png");
}

// Resize all images
void ResizeAllImage()
{
    for (int i = 0; i < 2; i++)
    {
        ImageResizeNN(&NatureImg[i], 64, 64);
    }
}

// Load all textures from images
void LoadAllTexture()
{
    for (int i = 0; i < 2; i++)
    {
        NatureTex[i] = LoadTextureFromImage(NatureImg[i]);
    }
}

// Unload all images
void UnloadAllImage()
{
    for (int i = 0; i < 2; i++)
    {
        UnloadImage(NatureImg[i]);
    }
}

// Unload all textures
void UnloadAllTexture()
{
    for (int i = 0; i < 2; i++)
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