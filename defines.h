#pragma once

//#define Release
//#define Fps

#define windowWidth 800
#define windowHeight 600

#define GLCALL(call) call;

#define textureDir "/home/medusa/projekte/opengl/main.png"

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