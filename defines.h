#pragma once
#include <glm/glm.hpp>

//defines
//#define Release
//#define Fps
//#define orthoMode
//#define fpsMode

using namespace std;

#define cameraSpeed 6.0f
#define mouseSens 0.4f

#define GLCALL(call) call;

struct Vertex {
    //koordinaten
    glm::vec3 position;
    //farben
    glm::vec3 normal;
};

/*
textur koordinaten mit 4 vertices
(0, 1) ---------------------- (1, 1)
   |                              |
   |                              |
   |                              |
(0, 0) ---------------------- (1, 0)
*/