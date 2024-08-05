#include "../include/Mouse.h"
#include "../include/raymath.h"

// Mouse collision check
bool mouseCollision;
int mouseSize = 20;

void UpdateMouse(Rectangle &mouseRect, Camera2D camera, float scale, int gameScreenWidth, int gameScreenHeight)
{
    // Get mouse position in screen space
    Vector2 mousePosition = GetMousePosition();

    // Transform mouse position to world space with scaling
    mousePosition.x = (mousePosition.x - (GetScreenWidth() - gameScreenWidth * scale) * 0.5f) / scale;
    mousePosition.y = (mousePosition.y - (GetScreenHeight() - gameScreenHeight * scale) * 0.5f) / scale;

    // Transform mouse position to world space using the camera transformation
    Matrix matCamera = GetCameraMatrix2D(camera);
    Matrix matInvCamera = MatrixInvert(matCamera);
    Vector3 mousePosScreen = {mousePosition.x, mousePosition.y, 0};
    Vector3 mousePosWorld3D = Vector3Transform(mousePosScreen, matInvCamera);
    Vector2 mousePosWorld = {mousePosWorld3D.x, mousePosWorld3D.y};

    // Update the mouse rectangle position
    mouseRect.x = mousePosWorld.x;
    mouseRect.y = mousePosWorld.y;
}

void DrawMouse(bool mouseCollision, float scale)
{
    Vector2 mouseScreenPos = GetMousePosition();
    if (mouseCollision)
    {
        DrawRectangle(mouseScreenPos.x / scale, mouseScreenPos.y / scale, 20, 20, RED);
    }
    else
    {
        DrawRectangle(mouseScreenPos.x / scale, mouseScreenPos.y / scale, 20, 20, GREEN);
    }
}