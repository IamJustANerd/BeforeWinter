#ifndef ASSETS_H
#define ASSETS_H

#include "../include/raylib.h"

// Images
extern Image NatureImg[100];

// Textures
extern Texture2D NatureTex[100];

// Load all images
void LoadAllImage();

// Resize all images
void ResizeAllImage();

// Load all textures from images
void LoadAllTexture();

// Unload all images
void UnloadAllImage();

// Unload all textures
void UnloadAllTexture();

// Setup assets (images, textures, audios, etc)
void SetupAssets();

#endif