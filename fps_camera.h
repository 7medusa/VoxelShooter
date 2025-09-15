#include "camera.h"
#include "defines.h"

class FPSCamera : public Camera {
public:
    FPSCamera(float fov, float width, float height) : Camera(fov, width, height) {
        yaw = -90.0f;
        pitch = 0.0f;
        onMouseMove(0.0f, 0.0f);
        update();
    }

    //funktion für maus bewegung
    void onMouseMove(float xRel, float yRel) {
        yaw += xRel * mouseSens;
        pitch -= yRel * mouseSens;

        //verhindert zu weit nach oben oder unten zu schauen
        if(pitch > 89.0f) {pitch = 89.0f;}
        if(pitch < -89.0f) {pitch = -89.0f;}

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        lookAt = glm::normalize(front);
        update();
    }

    void update() override {
        view = glm::lookAt(position, position+lookAt, up);
        Camera::update();
    }

    void moveFront(float amount) {
        translate((glm::vec3(1.3f, 0.0f, 1.3f) * lookAt) * amount);//vec damit man beim oben schauen und vorwärts gehen nicht nach oben geht
        update();
    }

    void moveSideways(float amount) {
        translate(glm::cross(lookAt, up) * amount);
        update();
    }
protected:
    glm::vec3 lookAt;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw;//rotation um y achse
    float pitch;//rotation um x achse
};