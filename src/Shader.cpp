#include "../include/Shader.h"
#include <iostream>
Shader shdrOutline;

float outlineSize = 1.0f;
float outlineColor[4] = {1.0f, 0.0f, 0.0f, 1.0f}; // Normalized RED color
float textureSize[2];

int outlineSizeLoc;
int outlineColorLoc;
int textureSizeLoc;

void SetupShader()
{
    // Shaders for texture outline
    shdrOutline = LoadShader(0, TextFormat("..\\shaders\\resources\\glsl%i\\outline.fs", GLSL_VERSION));

    // Change this later on for other entities
    textureSize[0] = (float)natureTex[0].width;
    textureSize[1] = (float)natureTex[0].height;

    // Get shader locations
    outlineSizeLoc = GetShaderLocation(shdrOutline, "outlineSize");
    outlineColorLoc = GetShaderLocation(shdrOutline, "outlineColor");
    textureSizeLoc = GetShaderLocation(shdrOutline, "textureSize");

    // Set shader values (they can be changed later)
    SetShaderValue(shdrOutline, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shdrOutline, outlineColorLoc, outlineColor, SHADER_UNIFORM_VEC4);
    SetShaderValue(shdrOutline, textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);

    if (outlineSizeLoc == -1 || outlineColorLoc == -1 || textureSizeLoc == -1)
    {
        std::cerr << "Error: Failed to locate shader uniforms" << std::endl;
    }
    if (shdrOutline.id == 0)
    {
        std::cerr << "Error: Failed to load shader" << std::endl;
        return;
    }
    std::cout << textureSize[0] << ' ' << textureSize[1] << '\n';
    std::cout << "DONE" << '\n';
}