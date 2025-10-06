#pragma once
#include <glm/glm.hpp>

class Camera;
class Font;
class Shader;
struct SDL_Window;
union SDL_Event;

class Control {
public:
    Control();
    void handle(SDL_Event* event, Camera* camera);
    void control(Camera* camera, glm::mat4* characterModel, glm::mat4* characterModelViewProj, float delta, const unsigned int* level, SDL_Event* event, glm::mat4* projectionPointer, float gameTime, Font* font, Shader* fontShader, float windowWidth, float windowHeight, SDL_Window* window);
    bool eBool;
private:
    bool wBool;
    bool sBool;
    bool aBool;
    bool dBool;
    bool shieldBool;
    bool jumpOnProgress;
    bool up;
    bool pause;
    float prevTime;
};