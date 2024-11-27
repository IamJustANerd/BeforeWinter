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
extern Animation playerAnimation[5][10]; // [animation type (running, attacking, etc)][variety (light attack, heavy attack, etc)]
extern Animation natureAnimation[10][5][10]; // [nature type (tree, bush, etc)][animation type (running, attacking, etc)][variety (light attack, heavy attack, etc)]
extern Animation NPCAnimation[10][5][10]; // [NPC type (pawn, goblin, etc)][animation type (running, attacking, etc)][variety (chopping, building, etc)]

// Images
extern Image playerImg[5];
extern Image natureImg[10];
extern Image NPCImg[5];
extern Image buildingImg[5];

// Textures
extern Texture2D playerTex[5];
extern Texture2D natureTex[10];
extern Texture2D NPCTex[5];
extern Texture2D buildingTex[5];

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