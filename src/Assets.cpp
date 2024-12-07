#include "../include/Assets.h"
#include <iostream>

Animation playerAnimation[5][10]; 
Animation natureAnimation[10][5][10];
Animation NPCAnimation[10][5][10];

Image playerImg[5];
Image natureImg[10];
Image NPCImg[5];
Image buildingImg[5];
Image resourceImg[5];

Texture2D playerTex[5];
Texture2D natureTex[10];
Texture2D NPCTex[5];
Texture2D buildingTex[5];
Texture2D resourceTex[5];

// Load all images
void LoadAllImage()
{
    // Player
    // -> Knight
    playerImg[0] = LoadImage("../graphics/player/knightBlue.png");

    // Nature
    // -> Trees
    natureImg[0] = LoadImage("../graphics/nature/tree.png");
    natureImg[1] = LoadImage("../graphics/tree_green_2.png");
    natureImg[2] = LoadImage("../graphics/tree_green_3.png");
    
    // -> Bushes
    natureImg[3] = LoadImage("../graphics/bush_green_1.png");
    natureImg[4] = LoadImage("../graphics/bush_green_2.png");

    // NPC
    // -> Pawn
    NPCImg[0] = LoadImage("../graphics/npc/knights/pawnBlue.png");
    // -> Enemy (Torch)
    NPCImg[1] = LoadImage("../graphics/npc/goblins/torchRed.png");

    // Building
    // -> House
    buildingImg[0] = LoadImage("../graphics/building/houseBlue.png");

    // Resource
    // -> Wood
    resourceImg[0] = LoadImage("../graphics/resources/wood.png");
}

// Resize all images
void ResizeAllImage()
{
    // Player
    // -> Knight
    ImageResizeNN(&playerImg[0], playerImg[0].width * 2 / 3, playerImg[0].height * 2 / 3);

    // Nature
    // -> Trees
    ImageResizeNN(&natureImg[0], natureImg[0].width * 2 / 3, natureImg[0].height * 2 / 3);
    for (int i = 1; i < 3; i++)
    {
        ImageResizeNN(&natureImg[i], 144, 144);
    }

    // -> Bushes
    for (int i = 3; i < 5; i++)
    {
        ImageResizeNN(&natureImg[i], 64, 64);
    }

    // NPC
    // -> Pawn
    ImageResizeNN(&NPCImg[0], NPCImg[0].width * 2 / 3, NPCImg[0].height * 2 / 3);
    // -> Enemy (Torch)
    ImageResizeNN(&NPCImg[1], NPCImg[1].width * 2 / 3, NPCImg[1].height * 2 / 3);

    // Building
    // -> House
    ImageResizeNN(&buildingImg[0], buildingImg[0].width, buildingImg[0].height);

    // Resource
    // -> Wood
    ImageResizeNN(&resourceImg[0], resourceImg[0].width * 2 / 3, resourceImg[0].width * 2 / 3);
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

    // NPC
    // -> Pawn
    NPCTex[0] = LoadTextureFromImage(NPCImg[0]);
    // -> Enemy (Torch)
    NPCTex[1] = LoadTextureFromImage(NPCImg[1]);

    // Building
    // -> House
    buildingTex[0] = LoadTextureFromImage(buildingImg[0]);

    // Resource
    // -> Wood
    resourceTex[0] = LoadTextureFromImage(resourceImg[0]);
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

    // NPC
    // -> Pawn
    UnloadImage(NPCImg[0]);
    // -> Enemy (Torch)
    UnloadImage(NPCImg[1]);

    // Building
    // -> House
    UnloadImage(buildingImg[0]);

    // Resource
    // -> Wood
    UnloadImage(resourceImg[0]);
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

    // NPC
    // -> Pawn
    UnloadTexture(NPCTex[0]);
    // -> Enemy (Torch)
    UnloadTexture(NPCTex[1]);

    // Building
    // -> House
    UnloadTexture(buildingTex[0]);

    // Resource
    // -> Wood
    UnloadTexture(resourceTex[0]);
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
    playerAnimation[3][0].frameTime = 60;
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

    // Nature
    // -> Tree
    // --> Idle
    natureAnimation[0][0][0].frameTime = 120;
    natureAnimation[0][0][0].totalFrames = 4;
    natureAnimation[0][0][0].sourceFrame = {0, 0, 128, 128};

    // --> Hit
    natureAnimation[0][4][0].frameTime = 16;
    natureAnimation[0][4][0].totalFrames = 2;
    natureAnimation[0][4][0].sourceFrame = {0, 129, 128, 128};

    // --> Chopped (Keknya ga perlu?)
    // natureAnimation[0][5][0].frameTime = 30;
    // natureAnimation[0][5][0].totalFrames = 2;
    // natureAnimation[0][5][0].sourceFrame = {0, 129, 128, 128};

    // NPC
    // -> Pawn
    // --> Idle
    NPCAnimation[0][0][0].frameTime = 60;
    NPCAnimation[0][0][0].totalFrames = 6;
    NPCAnimation[0][0][0].sourceFrame = {0, 0, 128, 128};

    // --> Running
    NPCAnimation[0][1][0].frameTime = 60;
    NPCAnimation[0][1][0].totalFrames = 6;
    NPCAnimation[0][1][0].sourceFrame = {0, 129, 128, 128};

    // --> Attack type 1 (right) - light attack (building)
    NPCAnimation[0][2][0].frameTime = 42;
    NPCAnimation[0][2][0].totalFrames = 6;
    NPCAnimation[0][2][0].sourceFrame = {0, 257, 128, 128};

    // --> Attack type 2 (right) - heavy attack (chopping)
    NPCAnimation[0][3][0].frameTime = 42;
    NPCAnimation[0][3][0].totalFrames = 6;
    NPCAnimation[0][3][0].sourceFrame = {0, 385, 128, 128};

    // --> Idle + Carrying
    NPCAnimation[0][0][1].frameTime = 60;
    NPCAnimation[0][0][1].totalFrames = 6;
    NPCAnimation[0][0][1].sourceFrame = {0, 513, 128, 128};

    // --> Running + Carrying
    NPCAnimation[0][1][1].frameTime = 60;
    NPCAnimation[0][1][1].totalFrames = 6;
    NPCAnimation[0][1][1].sourceFrame = {0, 641, 128, 128};

    // -> Enemy (Torch)
    // --> Idle
    NPCAnimation[1][0][0].frameTime = 60;
    NPCAnimation[1][0][0].totalFrames = 7;
    NPCAnimation[1][0][0].sourceFrame = {0, 0, 128, 128};

    // --> Running
    NPCAnimation[1][1][0].frameTime = 60;
    NPCAnimation[1][1][0].totalFrames = 6;
    NPCAnimation[1][1][0].sourceFrame = {0, 129, 128, 128};

    // --> Light attack (right)
    NPCAnimation[1][2][0].frameTime = 42;
    NPCAnimation[1][2][0].totalFrames = 6;
    NPCAnimation[1][2][0].sourceFrame = {0, 257, 128, 128};

    // --> Light attack (down)
    NPCAnimation[1][2][0].frameTime = 42;
    NPCAnimation[1][2][0].totalFrames = 6;
    NPCAnimation[1][2][0].sourceFrame = {0, 385, 128, 128};

    // --> Light attack (up)
    NPCAnimation[1][2][0].frameTime = 42;
    NPCAnimation[1][2][0].totalFrames = 6;
    NPCAnimation[1][2][0].sourceFrame = {0, 513, 128, 128};
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