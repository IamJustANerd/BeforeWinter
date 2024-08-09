// Camera.cpp
#include "../include/Camera.h"
#include "../include/raymath.h"

void UpdateCamera(Camera2D &camera, Vector2 playerPos, int playerWidth, int playerHeight, float scale)
{
    // Update camera position to follow the player
    camera.target = Vector2{playerPos.x + playerWidth / 2, playerPos.y + playerHeight / 2};

    // Update camera offset to keep the player in the center of the screen
    camera.offset = Vector2{(float)GetScreenWidth() / scale / 2, (float)GetScreenHeight() / scale / 2};

    // Camera zoom controls
    camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

    if (camera.zoom > 1.5f)
        camera.zoom = 1.5f;
    else if (camera.zoom < 0.5f)
        camera.zoom = 0.5f;

    // Reset zoom value
    if (IsKeyPressed(KEY_R))
    {
        camera.zoom = 1.0f;
    }
}