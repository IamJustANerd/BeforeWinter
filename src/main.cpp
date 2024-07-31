#include <vector>
#include <algorithm>
#include "../include/raylib.h"
#include "../include/raymath.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct GameObject
{
    Vector2 position;
    int type;
    float depth;

    GameObject(Vector2 pos, int t, float d) : position(pos), type(t), depth(d) {}
};

int screenWidth = 1600;
int screenHeight = 900;
const int gridSize = 16;

int minX;
int minY;
int maxX;
int maxY;

std::vector<GameObject> gameObjects;
std::vector<GameObject> visibleObjects;

void UpdateGameObjectDepths(std::vector<GameObject> &objects)
{
    for (auto &obj : objects)
    {
        obj.depth = obj.position.y;
    }
}

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
        int objGridX = (int)obj.position.x / gridSize;
        int objGridY = (int)obj.position.y / gridSize;
        if (objGridX >= minX && objGridX <= maxX && objGridY >= minY && objGridY <= maxY)
        {
            visibleObjects.push_back(obj);
        }
    }
}

void DrawGrid(int gridSize, Camera2D camera)
{
    Vector2 cameraPos = camera.target;
    int firstVerticalLine = (int)(cameraPos.x - screenWidth / 2) / gridSize * gridSize;
    int firstHorizontalLine = (int)(cameraPos.y - screenHeight / 2) / gridSize * gridSize;

    for (int x = firstVerticalLine; x < cameraPos.x + screenWidth / 2; x += gridSize)
    {
        DrawLine(x - cameraPos.x + screenWidth / 2, 0, x - cameraPos.x + screenWidth / 2, screenHeight, DARKGRAY);
    }

    for (int y = firstHorizontalLine; y < cameraPos.y + screenHeight / 2; y += gridSize)
    {
        DrawLine(0, y - cameraPos.y + screenHeight / 2, screenWidth, y - cameraPos.y + screenHeight / 2, DARKGRAY);
    }
}

Image earthImg;
Image moonImg;
Texture2D earthTex;
Texture2D moonTex;

void LoadAllImage()
{
    earthImg = LoadImage("../graphics/earth.png");
    moonImg = LoadImage("../graphics/moon.png");
}

void ResizeAllImage()
{
    ImageResizeNN(&earthImg, 16, 16);
    ImageResizeNN(&moonImg, 16, 16);
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Raylib Game");
    SetWindowMinSize(320, 180);

    int gameScreenWidth = 960;
    int gameScreenHeight = 540;

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    Rectangle player = {screenWidth / 2, screenHeight / 2, 40, 40};

    // Load images
    LoadAllImage();
    ResizeAllImage();

    // Load textures
    Texture2D textures[2];
    textures[0] = LoadTextureFromImage(earthImg);
    textures[1] = LoadTextureFromImage(moonImg);

    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR); // Texture scale filter to use

    // Initialize game objects
    for (int i = 0; i < 100000; i++)
    {
        Vector2 pos = {GetRandomValue(0, 5000), GetRandomValue(0, 5000)};
        float d = 0;
        int type = GetRandomValue(0, 1);
        gameObjects.emplace_back(pos, type, d);
    }
    gameObjects.emplace_back(Vector2{100, 200}, 1, 0);

    Camera2D camera = {0};
    camera.target = (Vector2){player.x, player.y};
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float scale = MIN((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        // Update game logic
        UpdateGameObjectDepths(gameObjects);
        GetVisibleObjects(camera);
        std::sort(visibleObjects.begin(), visibleObjects.end(), [](const GameObject &a, const GameObject &b)
                  { return a.depth < b.depth; });

        if (IsKeyDown(KEY_A))
        {
            player.x -= 10;
        }
        if (IsKeyDown(KEY_D))
        {
            player.x += 10;
        }
        if (IsKeyDown(KEY_W))
        {
            player.y -= 10;
        }
        if (IsKeyDown(KEY_S))
        {
            player.y += 10;
        }

        camera.target = (Vector2){player.x + player.width / 2, player.y + player.height / 2};

        // Update camera offset to keep the player in the center of the screen
        camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};

        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = {0};
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth * scale)) * 0.5f) / scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight * scale)) * 0.5f) / scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){0, 0}, (Vector2){(float)gameScreenWidth, (float)gameScreenHeight});

        // Draw
        BeginDrawing();

        ClearBackground(BLACK);

        DrawGrid(gridSize, camera);

        for (const auto &obj : visibleObjects)
        {
            DrawTexture(textures[obj.type], obj.position.x - camera.target.x + screenWidth / 2, obj.position.y - camera.target.y + screenHeight / 2, WHITE);
        }
        
        BeginMode2D(camera);

        DrawRectangleRec(player, RED);

        EndMode2D();

        DrawText(TextFormat("Min X Y: [%i , %i]", (int)minX, (int)minY), 0, 25, 20, GREEN);
        DrawText(TextFormat("Max X Y: [%i , %i]", (int)maxX, (int)maxY), 0, 55, 20, GREEN);
        DrawText(TextFormat("Camera X Y: [%i , %i]", (int)camera.target.x, (int)camera.target.y), 0, 85, 20, GREEN);
        DrawText(TextFormat("Screen Size: [%i , %i]", (int)screenWidth, (int)screenHeight), 0, 115, 20, GREEN);

        DrawTexturePro(target.texture,
                       (Rectangle){0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                       (Rectangle){(GetScreenWidth() - ((float)gameScreenWidth * scale)) * 0.5f,
                                   (GetScreenHeight() - ((float)gameScreenHeight * scale)) * 0.5f,
                                   (float)gameScreenWidth * scale,
                                   (float)gameScreenHeight * scale},
                       (Vector2){0, 0}, 0.0f, WHITE);

        EndDrawing();
    }

    // Unload textures
    for (auto &tex : textures)
    {
        UnloadTexture(tex);
    }

    CloseWindow();

    return 0;
}