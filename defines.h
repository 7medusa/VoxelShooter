#pragma once
#include <glm/glm.hpp>

using namespace std;

//defines
//#define Release
//#define Debug

#define jumpSpeed 450
#define walkSpeed 2.6f
#define zoomSpeed 5.7f
#define jumpHeight 0.0f
//#define zoomOut 9.75f
#define zoomOut 20
#define zoomIn 1.7f
#define characterScale 0.01f
#define pistolShootTimeCharacter 0.8f
#define characterDamage 1
#define characterLife 3

#define porjectileSpeed 12
#define reloadTime 1.25f

#define ground -1.3f
#define cameraFov 90
#define pauseTime 7.5f

#define soldierLife 1
#define soldierDamage 1
#define walkSpeedSoldier 2.3f
#define pistolShootTimeSoldier 2
#define soldierRange 8
#define soldierShootRange 3.25f

#define tankLife 4
#define tankDamage 2
#define walkSpeedTank 1.5f
#define pistolShootTimeTank 2
#define tankRange 5
#define tankShootRange 3.25f

//level1
#define borderLeftLevel1 0
#define borderRightLevel1 20.75f
#define borderLeftLevel2 0
#define borderRightLevel2 20.75f

#define GLCALL(call) call;

struct Vertex {
    //koordinaten
    glm::vec3 position;
    //farben
    glm::vec3 normal;
    //texturkoordinaten
    glm::vec2 textureCoord;
};