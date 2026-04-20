#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../include/raylib_includes.h"

struct GameTime {
    int seconds;
    int minutes;
    int hours;
    int days;
};

struct Resources {
    int gold;
    int wood;
};

extern GameTime gameTime;
extern Resources currentResources;

void UpdateTime();

void PrintTime();

void UpdateResources(Resources _resources);

void PrintResources();

void DrawTimePhase(int cellSize, int cellNumber, Camera2D camera, const int gridSize);

#endif