#include "../include/Assets.h"
#include <iostream>

Animation playerAnimation[5][10]; // [animation type (running, attacking, etc)][variety (light attack, heavy attack, etc)]

Image playerImg[5];
Image natureImg[100];

Texture2D natureTex[100];
Texture2D playerTex[5];

// Load all images
void LoadAllImage()
{
    // Player
    // -> Knight
    playerImg[0] = LoadImage("../graphics/player/knightBlue.png");

    // Nature
    // -> Trees
    natureImg[0] = LoadImage("../graphics/tree_green_1.png");
    natureImg[1] = LoadImage("../graphics/tree_green_2.png");
    natureImg[2] = LoadImage("../graphics/tree_green_3.png");
    
    // -> Bushes
    natureImg[3] = LoadImage("../graphics/bush_green_1.png");
    natureImg[4] = LoadImage("../graphics/bush_green_2.png");
}

// Resize all images
void ResizeAllImage()
{
    // Player
    // -> Knight
    ImageResizeNN(&playerImg[0], playerImg->width * 2 / 3, playerImg->height * 2 / 3);

    // Nature
    // -> Trees
    for (int i = 0; i < 3; i++)
    {
        ImageResizeNN(&natureImg[i], 144, 144);
    }

    // -> Bushes
    for (int i = 3; i < 5; i++)
    {
        ImageResizeNN(&natureImg[i], 64, 64);
    }
}

// Load all textures from images
void LoadAllTexture()
{
    // Player
    // -> Knight
    playerTex[0] = LoadTextureFromImage(playerImg[0]);

    // Nature
    // -> Trees and bushes
    for (int i = 0; i < 5; i++)
    {
        natureTex[i] = LoadTextureFromImage(natureImg[i]);
    }
}

// Unload all images
void UnloadAllImage()
{
    // Player
    // -> Knight
    UnloadImage(playerImg[0]);

    // Nature
    // -> Trees and Bushes
    for (int i = 0; i < 5; i++)
    {
        UnloadImage(natureImg[i]);
    }
}

// Unload all textures
void UnloadAllTexture()
{
    // PLayer
    // Knight
    UnloadTexture(playerTex[0]);

    // Nature
    // -> Trees and Bushes
    for (int i = 0; i < 5; i++)
    {
        UnloadTexture(natureTex[i]);
    }
}

// Load animation for all entities
void LoadAnimation()
{
    // Player
    // -> Knight
    // --> Idle
    playerAnimation[0][0].frameTime = 60;
    playerAnimation[0][0].totalFrames = 6;
    playerAnimation[0][0].sourceFrame = {0, 0, 128, 128};

    // --> Running
    playerAnimation[1][0].frameTime = 60;
    playerAnimation[1][0].totalFrames = 6;
    playerAnimation[1][0].sourceFrame = {0, 129, 128, 128};

    // --> Attack type 1 (right) - light attack
    playerAnimation[2][0].frameTime = 30;
    playerAnimation[2][0].totalFrames = 6;
    playerAnimation[2][0].sourceFrame = {0, 257, 128, 128};

    // --> Attack type 2 (right) - heavy attack
    playerAnimation[3][0].frameTime = 600;
    playerAnimation[3][0].totalFrames = 6;
    playerAnimation[3][0].sourceFrame = {0, 385, 128, 128};

    // --> Attack type 1 (down) - light attack
    playerAnimation[2][1].frameTime = 30;
    playerAnimation[2][1].totalFrames = 6;
    playerAnimation[2][1].sourceFrame = {0, 513, 128, 128};

    // --> Attack type 2 (down) - heavy attack
    playerAnimation[3][1].frameTime = 60;
    playerAnimation[3][1].totalFrames = 6;
    playerAnimation[3][1].sourceFrame = {0, 641, 128, 128};

    // --> Attack type 1 (up) - light attack
    playerAnimation[2][2].frameTime = 30;
    playerAnimation[2][2].totalFrames = 6;
    playerAnimation[2][2].sourceFrame = {0, 769, 128, 128};

    // --> Attack type 2 (up) - heavy attack
    playerAnimation[3][2].frameTime = 60;
    playerAnimation[3][2].totalFrames = 6;
    playerAnimation[3][2].sourceFrame = {0, 897, 128, 128};
}

// Setup assets (images, textures, etc)
void SetupAssets()
{
    LoadAllImage();
    ResizeAllImage();
    LoadAllTexture();
    LoadAnimation();
    UnloadAllImage();
}