#pragma once
#include <glm/glm.hpp>

class Camera;
class Font;
class Shader;
struct SDL_Window;
union SDL_Event;
class Character;
class Weapon;

class Control {
public:
    Control();
    void handle(SDL_Event* event, Camera* camera);
    void control(Camera* camera, Character* player, float delta, const unsigned int* level, SDL_Event* event, glm::mat4* projection, float gameTime, Font* font, Shader* fontShader, float windowWidth, float windowHeight, SDL_Window* window, Shader* shader, Weapon* weapon);
    bool eBool;
private:
    bool wBool;
    bool sBool;
    bool aBool;
    bool dBool;
    bool shootRightBool;
    bool shootLeftBool;
    bool reloadBool;
    bool shieldBool;
    bool jumpOnProgress;
    bool up;
    bool pause;
    float prevTimePause;
    float prevTimeShoot;
    float prevTimeReload;
    bool blockFunction;
};