#ifndef MOUSE_H
#define MOUSE_H

#include "../include/raylib_includes.h"
#include "../include/Screen.h"

// Mouse collision check
extern bool mouseCollision;
extern bool mouseGrab;
extern Vector2 mousePosition;
extern int mouseSize;

void InitializeMouse(Camera2D &camera);
void UpdateMouse(Camera2D camera);
void DrawMouse(bool mouseCollision, float scale);
Rectangle GetMouseRect();

#endif 