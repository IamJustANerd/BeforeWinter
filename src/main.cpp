/*
    Dev note:
    1. Fixed drawing order by initializing entites width and height by their textures
    2. Need to fix mouse and objects collision (tips: mouse position is between 0 to screen size, while game objects are not)
*/
// {---------------------------------------------- Libraries ----------------------------------------------}
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

// {---------------------------------------------- Headers ----------------------------------------------}

// Raylib
#include "../include/raylib.h"
#include "../include/raymath.h"

// Screen
#include "../include/Screen.h"

// Camera
#include "../include/Camera.h"

// Mouse
#include "../include/Mouse.h"

// Assets
#include "../include/Assets.h"

// Player
#include "../include/Player.h"

// Entity
#include "../include/Entity.h"

// Nature
#include "../include/Nature.h"

// Collectible
#include "../include/Collectible.h"

// {---------------------------------------------- Global Variables ----------------------------------------------}

// Grid size
const int gridSize = 128;

// Minimum and maximum value for visible grid (set as global for debugging purpose)
int minX;
int minY;
int maxX;
int maxY;

// For debugging
bool showInformation = false;

// Store entites as game objects
std::vector<Entity*> gameObjects;
// Store visible objects for grid partitioning
std::vector<Entity*> visibleObjects;

// {---------------------------------------------- Functions ----------------------------------------------}

// Custom compare bool
struct CompareObjectPosition
{
    bool operator()(const Entity *a, const Entity *b) const
    {
        if (a->GetPosition().y + a->GetHeight() != b->GetPosition().y + b->GetHeight())
            return a->GetPosition().y + a->GetHeight() < b->GetPosition().y + b->GetHeight(); // Compare by y first
        return a->GetPosition().x < b->GetPosition().x;                                       // If y is the same, compare by x
    }
};

// To get visible objects based on camera for better performance
void GetVisibleObjects(Camera2D camera)
{
    visibleObjects.clear();
    Vector2 cameraPos = camera.target;
    minX = (int)(cameraPos.x - screenWidth) / gridSize;
    minY = (int)(cameraPos.y - screenHeight) / gridSize;
    maxX = (int)(cameraPos.x + screenWidth) / gridSize;
    maxY = (int)(cameraPos.y + screenHeight) / gridSize;

    for (const auto &obj : gameObjects)
    {
        int objGridX = (int)obj->GetPosition().x / gridSize;
        int objGridY = (int)obj->GetPosition().y / gridSize;
        if (objGridX >= minX && objGridX <= maxX && objGridY >= minY && objGridY <= maxY)
        {
            visibleObjects.push_back(obj);
        }

        // Push without grid partitioning (for debugging purpose)
        // visibleObjects.push_back(obj);
    }
}

// Draw grid for debugging purpose
void DrawGrid(int gridSize, Camera2D camera)
{
    for (int x = -10000; x <= 10000; x += gridSize)
    {
        DrawLine(x, -10000, x, 10000, DARKGRAY);
    }

    for (int y = -10000; y <= 10000; y += gridSize)
    {
        DrawLine(-10000, y, 10000, y, DARKGRAY);
    }
}

// {---------------------------------------------- Main Code ----------------------------------------------}
int main()
{
    // Initialize window
    InitWindow(screenWidth, screenHeight, "Before Winter");

    // Set window minimum size
    SetWindowMinSize(320, 180);

    // Toggle full screen
    //ToggleFullscreen();

    // Update screen width and height to the current window size
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    // Setup Assets
    SetupAssets();

    /*
        To make the content of the game scales based on the window size, we will use render-to-texture technique.
        This is achievable by making a target texture and draw on it instead of directly on the screen. Eventually,
        this target texture will be drawn to the screen (after scaled by game screen width and height).
    */
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    
    // Texture scale filter to use
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    // Declare player
    Player *player = new Player(Vector2{(float)screenWidth / 2, (float)screenHeight / 2});

    // Initialize random game objects for testing grid partitioning
    // for (int i = 0; i <= 5000; i += GetRandomValue(0, 128))
    // {
    //     for (int j = 0; j <= 5000; j += GetRandomValue(0, 128))
    //     {
    //         gameObjects.emplace_back(new Nature(Vector2{(float)i, (float)j}, GetRandomValue(0, 4), NatureTex));
    //     }
    // }

    gameObjects.emplace_back(player);

    // Testing collectibles
    for (int i = 0; i <= 5000; i += GetRandomValue(0, 128))
    {
        for (int j = 0; j <= 5000; j += GetRandomValue(0, 128))
        {
            gameObjects.emplace_back(new Nature(Vector2{(float)i, (float)j}, GetRandomValue(0, 4), NatureTex));
            gameObjects.emplace_back(new Collectible(Vector2{(float)i, (float)j}, GetRandomValue(0, 4)));
        }
    }

    // Setting up camera to follow the player
    Camera2D camera = {0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Setting game FPS
    SetTargetFPS(120);

    // Scale the content based on the window size
    scale = std::min((float)screenWidth / gameScreenWidth, (float)screenHeight / gameScreenHeight);

    // Window loop
    while (!WindowShouldClose())
    {
        // Get all visible objects within the sight of the camera
        GetVisibleObjects(camera);

        // Set mouse collision as false
        mouseCollision = false;

        // Sort all of them based on their position
        std::sort(visibleObjects.begin(), visibleObjects.end(), CompareObjectPosition());

        // Update Player
        player->Update();

        // Temporary loop to check if collectibles are inside player collect radius
        for (auto it = visibleObjects.begin(); it != visibleObjects.end();)
        {
            if (typeid(**it) == typeid(Collectible))
            {
                Collectible *collectible = static_cast<Collectible *>(*it);

                collectible->UpdatePlayerPosition(player->GetHitBoxPosition());

                // If within radius, then change the state
                if (CheckCollisionRecs(collectible->GetHitBox(), player->GetCollectRadiusRectangle()))
                {
                    collectible->withinRadius = true;
                }
                else
                {
                    collectible->withinRadius = false;
                }

                // If collide with player hit box, then remove it
                if (CheckCollisionRecs(collectible->GetHitBox(), player->GetHitBox()))
                {
                    it = visibleObjects.erase(it); // Erase returns the next valid iterator
                }
                else
                {
                    ++it; // Only increment if not removing
                }

                collectible->Update();
            }
            else
            {
                ++it;
            }
        }

        // Update Camera
        UpdateCamera(camera, player->GetPosition(), player->GetWidth(), player->GetHeight(), scale);

        // Update mouse according to world position
        Rectangle mouseRect = {0, 0, mouseSize / camera.zoom, mouseSize / camera.zoom};
        UpdateMouse(mouseRect, camera);

        // Toggle show information
        if(IsKeyPressed(KEY_TAB))
        {
            showInformation = !showInformation;
        }

        // Draw on texture
        BeginTextureMode(target);

        ClearBackground(BLACK);

        // 2D mode
        BeginMode2D(camera);

        // Grid for debugging
        DrawGrid(gridSize, camera);

        // Draw objects
        for (const auto &obj : visibleObjects)
        {
            obj->Draw();
        }

        // Draw player and rectangle (for debugging)

        Rectangle testBlue = {player->GetPosition().x - gridSize, player->GetPosition().y - gridSize, gridSize * 2 + player->GetWidth(), gridSize * 2 + player->GetHeight()};
        // Only hover when the inventory is not called
        if (CheckCollisionRecs(testBlue, mouseRect) && !player->IsInventoryCalled())
        {
            DrawRectangleRec(testBlue, Color{230, 41, 55, 127});
            mouseCollision = true;
        }
        else
        {
            DrawRectangleRec(testBlue, Color{0, 121, 241, 127});
        }

        // Collision test with world objects
        // for (const auto &obj : visibleObjects)
        // {
        //     if(CheckCollisionRecs(mouseRect,
        //                           Rectangle{obj->GetPosition().x, obj->GetPosition().y, (float)obj->GetWidth(), (float)obj->GetHeight()}))
        //     {
        //         mouseCollision = true;
        //     }
        // }

        // End 2D mode
        EndMode2D();

        // Draw inventory
        if(player->IsInventoryCalled())
        {
            player->DrawInventory();
        }

        // Draw the mouse according to screen position
        DrawMouse(mouseCollision, scale);

        // Debugging Information
        if(showInformation)
        {
            DrawText(TextFormat("Min X Y: [%i , %i]", (int)minX, (int)minY), 0, 5, 15, GREEN);
            DrawText(TextFormat("Max X Y: [%i , %i]", (int)maxX, (int)maxY), 0, 35, 15, GREEN);
            DrawText(TextFormat("Camera X Y: [%i , %i]", (int)camera.target.x, (int)camera.target.y), 0, 65, 15, GREEN);
            DrawText(TextFormat("Screen Size: [%i , %i]", (int)screenWidth, (int)screenHeight), 0, 95, 15, GREEN);
            DrawText(TextFormat("Player Position: [%f , %f]", (float)player->GetPosition().x, (float)player->GetPosition().y), 0, 125, 15, GREEN);
            DrawText(TextFormat("Player Grid: [%i , %i]", (int)player->GetPosition().x / gridSize, (int)player->GetPosition().y / gridSize), 0, 155, 15, GREEN);
            DrawText(TextFormat("Scale: [%f]", (float)scale), 0, 185, 15, GREEN);
            DrawText(TextFormat("Mouse Screen Position: [%f , %f]", (float)GetMousePosition().x, (float)GetMousePosition().y), 0, 215, 15, GREEN);
            DrawText(TextFormat("Mouse World Position: [%f , %f]", (float)mouseRect.x, (float)mouseRect.y), 0, 245, 15, GREEN);
        }
        // Finish drawing on texture
        EndTextureMode();

        // Start drawing on the window screen
        BeginDrawing();

        // Draw the scaled texture
        DrawTexturePro(target.texture,
                       Rectangle{0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                       Rectangle{(GetScreenWidth() - ((float)gameScreenWidth * scale)) * 0.5f,
                                 (GetScreenHeight() - ((float)gameScreenHeight * scale)) * 0.5f,
                                 (float)gameScreenWidth * scale,
                                 (float)gameScreenHeight * scale},
                       Vector2{0, 0}, 0.0f, WHITE);

        // End drawing
        EndDrawing();
    }

    // Unload Texture
    UnloadAllTexture();

    // Close window
    CloseWindow();

    return 0;
}