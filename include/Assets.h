#ifndef ASSETS_H
#define ASSETS_H

#include "../include/raylib.h"

// Images
extern Image playerImg[5];
extern Image natureImg[100];

// Textures
extern Texture2D playerTex[5];
extern Texture2D natureTex[100];

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