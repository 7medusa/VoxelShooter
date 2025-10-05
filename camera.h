#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float fov, float width, float height);
    ~Camera();
    void update();//updatet die kamera
    void translate(glm::vec3 v);//bewegt die kamera
    glm::mat4 getViewProjection() const;
    glm::vec3 getPosition() const;
    void reset();//reset the zoom
    glm::mat4 getView();
    void resetMatrix();
private:
    glm::mat4 projection{};//matrix zur abbildung der objekte
    glm::mat4 view{};//position der kamera
    glm::mat4 viewProj{};//kombinierte matrix
    glm::vec3 position{};//position der kamera
    struct FontVertex;
};