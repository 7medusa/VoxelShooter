#pragma once
#include <glm/glm.hpp>

using namespace std;

//defines
#define Release
//#define Debug

#define jumpSpeed 450.0f
#define walkSpeed 2.6f
#define zoomSpeed 5.7f
#define jumpHeight 0.0f
#define zoomOut 9.75f
#define zoomIn 1.7f
#define ground -1.3f
#define cameraFov 90.0f
#define characterScale 0.011f
#define pauseTime 7.5f

//level1
#define borderLeftLevel1 0.3f
#define borderRightLevel1 20.75f

#define GLCALL(call) call;

struct Vertex {
    //koordinaten
    glm::vec3 position;
    //farben
    glm::vec3 normal;
    //texturkoordinaten
    glm::vec2 textureCoord;
};