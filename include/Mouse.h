#ifndef MOUSE_H
#define MOUSE_H

#include "../include/raylib.h"

// Mouse collision check
extern bool mouseCollision;
extern int mouseSize;

void UpdateMouse(Rectangle &mouseRect, Camera2D camera, float scale, int gameScreenWidth, int gameScreenHeight);
void DrawMouse(bool mouseCollision, float scale);

#endif 