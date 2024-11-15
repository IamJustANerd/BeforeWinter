#include "../include/Assets.h"
#include <iostream>

Animation playerAnimation[5][10]; // [player type (knight, archer, etc)][animation type (running, attacking, etc)]

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
    playerAnimation[0][1].frameTime = 60;
    playerAnimation[0][1].totalFrames = 6;
    playerAnimation[0][1].sourceFrame = {0, 129, 128, 128};

    // --> Attack type 1 (right)
    playerAnimation[0][2].frameTime = 60;
    playerAnimation[0][2].totalFrames = 6;
    playerAnimation[0][2].sourceFrame = {0, 257, 128, 128};

    // --> Attack type 2 (right)
    playerAnimation[0][3].frameTime = 60;
    playerAnimation[0][3].totalFrames = 6;
    playerAnimation[0][3].sourceFrame = {0, 385, 128, 128};

    // --> Attack type 1 (down)
    playerAnimation[0][4].frameTime = 60;
    playerAnimation[0][4].totalFrames = 6;
    playerAnimation[0][4].sourceFrame = {0, 513, 128, 128};

    // --> Attack type 2 (down)
    playerAnimation[0][5].frameTime = 60;
    playerAnimation[0][5].totalFrames = 6;
    playerAnimation[0][5].sourceFrame = {0, 641, 128, 128};

    // --> Attack type 1 (up)
    playerAnimation[0][6].frameTime = 60;
    playerAnimation[0][6].totalFrames = 6;
    playerAnimation[0][6].sourceFrame = {0, 769, 128, 128};

    // --> Attack type 2 (up)
    playerAnimation[0][7].frameTime = 60;
    playerAnimation[0][7].totalFrames = 6;
    playerAnimation[0][7].sourceFrame = {0, 897, 128, 128};
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