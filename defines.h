#pragma once
#include <glm/glm.hpp>

using namespace std;

//defines
//#define Release

#define ground -1.3f
#define cameraFov 90.0f

#define GLCALL(call) call;

struct Vertex {
    //koordinaten
    glm::vec3 position;
    //farben
    glm::vec3 normal;
    //texturkoordinaten
    glm::vec2 textureCoord;
};

/*
textur koordinaten mit 4 vertices
(0, 1) ---------------------- (1, 1)
   |                              |
   |                              |
   |                              |
(0, 0) ---------------------- (1, 0)
*/