#include "../include/GameManager.h"
#include "../include/Screen.h"
#include "../include/raylib.h"
#include <iostream>
#include <algorithm>

Color noonLight = {0, 0, 0, 0};
Color sunsetOrange = {255, 154, 0, 0};
Color nightBlue = {6, 21, 88, 155};

unsigned char sunsetOrangeMaxA = 80;
unsigned char nightBlueMaxA = 155;

GameTime gameTime = {0, 0, 0, 0};

void UpdateTime()
{
    gameTime.seconds += 1;

    if(gameTime.seconds >= 60)
    {
        gameTime.minutes += 1;
        gameTime.seconds = 0;
    }

    if (gameTime.minutes >= 60)
    {
        gameTime.hours += 1;
        gameTime.minutes = 0;
    }

    if (gameTime.hours >= 24)
    {
        gameTime.days += 1;
        gameTime.hours = 0;
    }
}

void PrintTime()
{
    DrawText(TextFormat("Hari ke-%d", gameTime.days), gameScreenWidth - 90, 5, 15, GREEN);
    DrawText(TextFormat("Jam ke-%d", gameTime.hours), gameScreenWidth - 90, 35, 15, GREEN);
    DrawText(TextFormat("Menit ke-%d", gameTime.minutes), gameScreenWidth - 90, 65, 15, GREEN);
}

void DrawTimePhase(int cellSize, int cellNumber, Camera2D camera, const int gridSize)
{
    // Variables for calculation
    Vector2 cameraPos = camera.target;
    int minX = std::max((int)(cameraPos.x - screenWidth / scale) / cellSize, 0);
    int minY = std::max((int)(cameraPos.y - screenHeight / scale) / cellSize, 0);
    int maxX = std::min((int)(cameraPos.x + screenWidth / scale) / cellSize, cellNumber - 1);
    int maxY = std::min((int)(cameraPos.y + screenHeight / scale) / cellSize, cellNumber - 1);

    // Afternoon
    if(gameTime.hours >= 14 && gameTime.hours <= 17)
    {
        if(sunsetOrange.a < sunsetOrangeMaxA)
        {
            sunsetOrange.a += 1;
        }
        
        if(nightBlue.a > 0)
        {
            nightBlue.a -= 1;
        }
    }
    // Night
    else if ((gameTime.hours >= 0 && gameTime.hours <= 5) || (gameTime.hours >= 18 && gameTime.hours <= 23))
    {
        if (nightBlue.a < nightBlueMaxA)
        {
            nightBlue.a += 1;
        }

        if (sunsetOrange.a > 0)
        {
            sunsetOrange.a -= 1;
        }
    }
    // Noon
    else
    {
        if (nightBlue.a > 0)
        {
            nightBlue.a -= 1;
        }

        if (sunsetOrange.a > 0)
        {
            sunsetOrange.a -= 1;
        }
    }

    // Make a phase time effect by increasing or decreasing the phase time alpha value
    DrawRectangle(minX * gridSize, minY * gridSize, maxX * gridSize - minX * gridSize + gridSize, maxY * gridSize - minY * gridSize + gridSize, noonLight);

    DrawRectangle(minX * gridSize, minY * gridSize, maxX * gridSize - minX * gridSize + gridSize, maxY * gridSize - minY * gridSize + gridSize, sunsetOrange);

    DrawRectangle(minX * gridSize, minY * gridSize, maxX * gridSize - minX * gridSize + gridSize, maxY * gridSize - minY * gridSize + gridSize, nightBlue);
}