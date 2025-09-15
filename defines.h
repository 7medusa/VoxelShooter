#pragma once

//defines zu bearbeiten
//#define Release
//#define Fps
//#define orthoMode
#define fpsMode


#ifdef fpsMode
    #define Release2
#endif


using namespace std;

#define windowWidth 800.0f
#define windowHeight 600.0f

#define cameraSpeed 6.0f
#define mouseSens 0.4f

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