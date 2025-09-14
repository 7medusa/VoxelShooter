#pragma once

//#define Release
//#define Fps

using namespace std;

#define windowWidth 800.0f
#define windowHeight 600.0f

#define GLCALL(call) call;

#define textureDir "../main.png"
#define vertexShaderDir "../shaders/basic.vs"
#define fragmentShaderDir "../shaders/basic.fs"

struct Vertex {
    //koordinaten
    float x;
    float y;
    float z;
    //texturkoordinaten
    float u;
    float v;
    //farben
    float r;
    float g;
    float b;
    float a;
};

/*
textur koordinaten mit 4 vertices
(0, 1) ---------------------- (1, 1)
   |                              |
   |                              |
   |                              |
(0, 0) ---------------------- (1, 0)
*/