// {---------------------------------------------- Libraries ----------------------------------------------}
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

// {---------------------------------------------- Raylib ----------------------------------------------}

#include "../include/raylib.h"
#include "../include/raymath.h"

// {---------------------------------------------- Headers ----------------------------------------------}

// Player
#include "../include/Player.h"
// Entity
#include "../include/Entity.h"
// Nature
#include "../include/Nature.h"

// {---------------------------------------------- Global Variables ----------------------------------------------}

// Screen
// int screenWidth = 1340;
// int screenHeight = 810;
int screenWidth = 0;
int screenHeight = 0;

// Game Screen (for scaling)
int gameScreenWidth = 960;
int gameScreenHeight = 540;

// Grid size
const int gridSize = 32;

// Minimum and maximum value for visible grid (set as global for debugging purpose)
int minX;
int minY;
int maxX;
int maxY;

// Store entites as game objects
std::vector<Entity*> gameObjects;
// Store visible objects for grid partitioning
std::vector<Entity*> visibleObjects;

// Images
Image NatureImg[100];

// Textures
Texture2D NatureTex[100];

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

void UpdateCamera(Camera2D &camera, Vector2 playerPos, int playerWidth, int playerHeight, float scale)
{
    // Update camera position to follow the player
    camera.target = Vector2{playerPos.x + playerWidth / 2, playerPos.y + playerHeight / 2};

    // Update camera offset to keep the player in the center of the screen
    camera.offset = Vector2{(float)screenWidth / scale / 2, (float)screenHeight / scale / 2};

    // Camera zoom controls
    camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

    if (camera.zoom > 2.0f)
        camera.zoom = 2.0f;
    else if (camera.zoom < 0.1f)
        camera.zoom = 0.1f;

    // Reset zoom value
    if (IsKeyPressed(KEY_R))
    {
        camera.zoom = 1.0f;
    }
}

// Load all images
void LoadAllImage()
{
    NatureImg[0] = LoadImage("../graphics/earth.png");
    NatureImg[1] = LoadImage("../graphics/moon.png");
}

// Resize all images
void ResizeAllImage()
{
    for(int i = 0; i < 2; i++)
    {
        ImageResizeNN(&NatureImg[i], 64, 64);
    }
}

// Load all textures from images
void LoadAllTexture()
{
    for(int i = 0; i < 2; i++)
    {
        NatureTex[i] = LoadTextureFromImage(NatureImg[i]);
    }
}

// Unload all images
void UnloadAllImage()
{
    for(int i = 0; i < 2; i++)
    {
        UnloadImage(NatureImg[i]);
    }
}

// Unload all textures
void UnloadAllTexture()
{
    for(int i = 0; i < 2; i++)
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

// {---------------------------------------------- Main Code ----------------------------------------------}
int main()
{
    // Initialize window
    InitWindow(screenWidth, screenHeight, "Before Winter");

    // Set window minimum size
    SetWindowMinSize(320, 180);

    // Toggle full screen
    ToggleFullscreen();

    // Update screen width and height to the current window size
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    // Declare player
    Player player(Vector2{(float)screenWidth / 2, (float)screenHeight / 2});

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

    // Initialize random game objects for testing grid partitioning
    int ty = 0;
    for (int i = 0; i <= 5000; i += GetRandomValue(0, 128))
    {
        for (int j = 0; j <= 5000; j += GetRandomValue(0, 128))
        {
            gameObjects.emplace_back(new Nature(Vector2{(float)i, (float)j}, ty, NatureTex));
            ty += 1;
            ty %= 2;
        }
        ty += 1;
        ty %= 2;
    }

    // Setting up camera to follow the player
    Camera2D camera = {0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Setting game FPS
    SetTargetFPS(60);

    // Scale the content based on the window size
    float scale = std::min((float)screenWidth / gameScreenWidth, (float)screenHeight / gameScreenHeight);

    // Window loop
    while (!WindowShouldClose())
    {
        /*
            Since there might be too many objects in the game, grid-partitioning is used in order to handle draw order,
            hitbox collision checks, etc. This method is selected for reducing the game complexity, which result with a
            better performance since the number of the computation goes down.
        */
        // Get all visible objects within the sight of the camera
        GetVisibleObjects(camera);
        // Sort all of them based on their position
        std::sort(visibleObjects.begin(), visibleObjects.end(), CompareObjectPosition());

        // Player movement
        player.Movements();

        // Update Camera
        UpdateCamera(camera, player.GetPosition(), player.GetWidth(), player.GetHeight(), scale);

        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = {0};
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth * scale)) * 0.5f) / scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight * scale)) * 0.5f) / scale;
        virtualMouse = Vector2Clamp(virtualMouse, Vector2{0, 0}, Vector2{(float)gameScreenWidth, (float)gameScreenHeight});

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
        Rectangle testBlue = {player.GetPosition().x - gridSize, player.GetPosition().y - gridSize, gridSize * 3, gridSize * 3};
        // if (CheckCollisionPointRec(mouse, testBlue))
        // {
        //     DrawRectangleRec(testBlue, Color{230, 41, 55, 127});
        // }
        // else
        // {
        //     DrawRectangleRec(testBlue, Color{0, 121, 241, 127});
        // }

        DrawRectangleRec(testBlue, Color{0, 121, 241, 127});
        player.Draw();

        // End 2D mode
        EndMode2D();

        // DrawRectangle(virtualMouse.x, virtualMouse.y, 20, 20, GREEN);

        // Debugging Information
        DrawText(TextFormat("Min X Y: [%i , %i]", (int)minX, (int)minY), 0, 5, 15, GREEN);
        DrawText(TextFormat("Max X Y: [%i , %i]", (int)maxX, (int)maxY), 0, 35, 15, GREEN);
        DrawText(TextFormat("Camera X Y: [%i , %i]", (int)camera.target.x, (int)camera.target.y), 0, 65, 15, GREEN);
        DrawText(TextFormat("Screen Size: [%i , %i]", (int)screenWidth, (int)screenHeight), 0, 95, 15, GREEN);
        DrawText(TextFormat("Player Position: [%f , %f]", (float)player.GetPosition().x, (float)player.GetPosition().y), 0, 125, 15, GREEN);
        DrawText(TextFormat("Scale: [%f]", (float)scale), 0, 155, 15, GREEN);
        if(visibleObjects.size() > 0) {
            DrawText(TextFormat("Top Left: [%f , %f]", visibleObjects[0]->GetPosition().x, visibleObjects[0]->GetPosition().y), 0, 185, 15, GREEN);
            DrawText(TextFormat("Bottom Right: [%f , %f]", visibleObjects[visibleObjects.size() - 1]->GetPosition().x, visibleObjects[visibleObjects.size() - 1]->GetPosition().y), 0, 215, 15, GREEN);
        }
        DrawText(TextFormat("[%f , %f , %f , %f]", (float)player.GetPosition().x - screenWidth / 2, (float)player.GetPosition().y - screenHeight / 2, (float)screenWidth, (float)screenHeight), 0, 245, 15, GREEN);
        
        // Finish draw on texture
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