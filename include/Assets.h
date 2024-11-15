#ifndef ASSETS_H
#define ASSETS_H

#include "../include/raylib_includes.h"

// Animation
struct Animation
{
    int frameTime; // In fps
    int totalFrames; // The amount of frames for an animation
    Rectangle sourceFrame; // The starting point of a frame (in sprite sheet)
};

// Animation
extern Animation playerAnimation[5][10];

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

// Load animations
void LoadAnimation();

// Unload all textures
void UnloadAllTexture();

// Setup assets (images, textures, audios, etc)
void SetupAssets();

#endif