#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../include/raylib.h"

struct GameTime {
    int seconds;
    int minutes;
    int hours;
    int days;
};

extern GameTime gameTime;

void UpdateTime();

void PrintTime();

void DrawTimePhase(int cellSize, int cellNumber, Camera2D camera, const int gridSize);

#endif