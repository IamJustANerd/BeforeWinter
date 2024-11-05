#ifndef SHADER_H
#define SHADER_H

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

#include "../include/raylib_includes.h"
#include "../include/Assets.h"

extern Shader shdrOutline;

extern float outlineSize;
extern float outlineColor[4];
extern float textureSize[2];

extern int outlineSizeLoc;
extern int outlineColorLoc;
extern int textureSizeLoc;

// Function for shader setup
void SetupShader();

#endif