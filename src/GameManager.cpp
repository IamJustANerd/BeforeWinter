#include "../include/GameManager.h"
#include "../include/Screen.h"
#include <iostream>
#include <algorithm>

Color noonLight = {0, 0, 0, 0};
Color sunsetOrange = {255, 154, 0, 0};
Color nightBlue = {6, 21, 88, 155};

unsigned char sunsetOrangeMaxA = 80;
unsigned char nightBlueMaxA = 155;

// To change the alpha of the current light every second in real life time (or every minute in game time)
bool changeA = false;

GameTime gameTime = {0, 0, 4, 0};

void UpdateTime()
{
    gameTime.seconds += 1;

    if(gameTime.seconds >= 60)
    {
        gameTime.minutes += 1;
        gameTime.seconds = 0;

        changeA = true;
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
    DrawText(TextFormat("%02d:%02d", gameTime.hours, gameTime.minutes), gameScreenWidth - 90, 35, 15, GREEN);
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
    if(gameTime.hours >= 12 && gameTime.hours <= 15 && changeA)
    {
        changeA = false;

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
    else if (((gameTime.hours >= 0 && gameTime.hours <= 4) || (gameTime.hours >= 16 && gameTime.hours <= 23)) && changeA)
    {
        changeA = false;

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
    else if(changeA)
    {
        changeA = false;

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