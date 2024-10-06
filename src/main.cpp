/*
    Dev note:
    1. Fixed drawing order by initializing entites width and height by their textures
    2. Need to fix mouse and objects collision (tips: mouse position is between 0 to screen size, while game objects are not)
    3. Need to ensure that the spatial partitioning is working well
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

// Grid
#include "../include/Grid.h"

// Game Manager
#include "../include/GameManager.h"

// {---------------------------------------------- Global Variables ----------------------------------------------}

// Grid size
const int gridSize = 128;

// Minimum and maximum value for visible grid (set as global for debugging purpose)
int minX;
int minY;
int maxX;
int maxY;

// For debugging
bool showInformation = true;

// Store entites as game objects
std::vector<Entity*> gameObjects;
// Store visible objects for grid partitioning
std::vector<Entity*> visibleObjects;

// {---------------------------------------------- Functions ----------------------------------------------}

// Draw grid for debugging purpose
void DrawGrid(int cellSize, int cellNumber, Camera2D camera)
{
    // Variables for calculation
    Vector2 cameraPos = camera.target;
    minX = std::max((int)(cameraPos.x - screenWidth / scale) / cellSize, 0);
    minY = std::max((int)(cameraPos.y - screenHeight / scale) / cellSize, 0);
    maxX = std::min((int)(cameraPos.x + screenWidth / scale) / cellSize, cellNumber - 1);
    maxY = std::min((int)(cameraPos.y + screenHeight / scale) / cellSize, cellNumber - 1);

    // World
    for (int x = -5000; x <= 15000; x += gridSize)
    {
        DrawLine(x + 8, -5000, x + 8, 15000, DARKGRAY);
    }

    for (int y = -5000; y <= 15000; y += gridSize)
    {
        DrawLine(-5000, y + 8, 15000, y + 8, DARKGRAY);
    }

    // Visible view
    for (int x = minX; x <= maxX + 1; x += 1)
    {
        DrawLine(x * gridSize, minY * gridSize, x * gridSize, maxY * gridSize + gridSize, RED);
    }

    for(int y = minY; y <= maxY + 1; y += 1)
    {
        DrawLine(minX * gridSize, y * gridSize, maxX * gridSize + gridSize, y * gridSize, RED);
    }

    // for (int y = -10000; y <= 10000; y += gridSize)
    // {
    //     DrawLine(-10000, y, 10000, y, RED);
    // }
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

    // Grid
    Grid grid;

    // Declare player
    Player* player = new Player(Vector2{(float)screenWidth, (float)screenHeight}, &grid);
    new Nature(Vector2{384, 384}, 1, NatureTex, &grid);

    new Collectible(Vector2{(float)screenWidth / 2 + 50, (float)screenHeight / 2 + 50}, 1, &grid);
    new Collectible(Vector2{(float)screenWidth / 2 + 200, (float)screenHeight / 2 + 200}, 1, &grid);

    //Testing collectibles
    for (int i = 0; i <= 100; i += 1)
    {
        for (int j = 0; j <= 100; j += 1)
        {
            // new Nature(Vector2{(float)GetRandomValue(0, 10000), (float)GetRandomValue(0, 10000)}, GetRandomValue(0, 4), NatureTex, &grid);
            new Collectible(Vector2{(float)GetRandomValue(0, 10000), (float)GetRandomValue(0, 10000)}, GetRandomValue(1, 9999), &grid);
        }
    }

    // Setting up camera to follow the player
    Camera2D camera = {0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Setting game FPS
    SetTargetFPS(60);

    // Scale the content based on the window size
    scale = std::min((float)screenWidth / gameScreenWidth, (float)screenHeight / gameScreenHeight);

    // Counting time for debugging
    int cnt = 0;

    // Window loop
    while (!WindowShouldClose())
    {
        // Set mouse collision as false
        mouseCollision = false;
        
        // Update all objects in the grid
        grid.UpdateGrid();

        // Check how many entities on screen (for debugging)
        if(IsKeyPressed(KEY_ENTER))
        {
            int ada = 1;
            const Entity *const(&cells)[Grid::NUM_CELLS][Grid::NUM_CELLS] = grid.GetReadOnlyCells();
            Vector2 cameraPos = camera.target;
            int minX = std::max((int)(cameraPos.x - screenWidth / scale) / grid.CELL_SIZE, 0);
            int minY = std::max((int)(cameraPos.y - screenHeight / scale) / grid.CELL_SIZE, 0);
            int maxX = std::min((int)(cameraPos.x + screenWidth / scale) / grid.CELL_SIZE, grid.NUM_CELLS - 1);
            int maxY = std::min((int)(cameraPos.y + screenHeight / scale) / grid.CELL_SIZE, grid.NUM_CELLS - 1);

            for (int i = minX; i <= maxX; i++)
            {
                for(int j = minY; j <= maxY; j++)
                {
                    const Entity* entity = cells[i][j];

                    while(entity != NULL)
                    {
                        ada++;
                        entity = entity->next;
                    }
                }
            }

            std::cout << "There are " << ada << " entities" << '\n';
        }

        // Update Camera
        UpdateCamera(camera, player->GetPosition(), player->GetWidth(), player->GetHeight(), scale);

        // Update mouse according to world position
        Rectangle mouseRect = {0, 0, mouseSize / camera.zoom, mouseSize / camera.zoom};
        UpdateMouse(mouseRect, camera);

        // Update time
        UpdateTime();

        // Toggle show information
        if(IsKeyPressed(KEY_TAB))
        {
            showInformation = !showInformation;
        }

        // <---------- START DRAWING ---------->
        // Draw on texture
        BeginTextureMode(target);

        ClearBackground(WHITE);

        // 2D mode
        BeginMode2D(camera);

        // Grid for debugging
        DrawGrid(grid.CELL_SIZE, grid.NUM_CELLS, camera);

        // Draw objects visible by player
        grid.DrawVisibleObjects(camera.target);

        // Draw player and rectangle (for debugging)

        // Mouse collision testing
        // Rectangle testBlue = {player->GetPosition().x - gridSize, player->GetPosition().y - gridSize, gridSize * 2 + player->GetWidth(), gridSize * 2 + player->GetHeight()};
        // // Only hover when the inventory is not called
        // if (CheckCollisionRecs(testBlue, mouseRect) && !player->IsInventoryCalled())
        // {
        //     DrawRectangleRec(testBlue, Color{230, 41, 55, 127});
        //     mouseCollision = true;
        // }
        // else
        // {
        //     DrawRectangleRec(testBlue, Color{0, 121, 241, 127});
        // }

        // Collision test with world objects
        // for (const auto &obj : visibleObjects)
        // {
        //     if(CheckCollisionRecs(mouseRect,
        //                           Rectangle{obj->GetPosition().x, obj->GetPosition().y, (float)obj->GetWidth(), (float)obj->GetHeight()}))
        //     {
        //         mouseCollision = true;
        //     }
        // }

        // Draw time phase
        DrawTimePhase(grid.CELL_SIZE, grid.NUM_CELLS, camera, gridSize);

        // End 2D mode
        EndMode2D();

        // Draw inventory
        if(player->IsInventoryCalled())
        {
            player->DrawInventory();
        }

        // Print game time
        PrintTime();

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
            DrawText(TextFormat("FPS: [%i]", GetFPS()), 0, 275, 15, GREEN);
            DrawText(TextFormat("Number of Visible Objects: [%i]", visibleObjects.size()), 0, 305, 15, GREEN);
            DrawText(TextFormat("This is grid: [%i, %i]", (int)mouseRect.x / grid.CELL_SIZE, (int)mouseRect.y / grid.CELL_SIZE), 0, 335, 15, GREEN);
        }

        // Draw the mouse according to screen position
        DrawMouse(mouseCollision, scale);

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