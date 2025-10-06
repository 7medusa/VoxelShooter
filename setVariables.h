#pragma once
#include <glm/glm.hpp>

class VertexBuffer;
class IndexBuffer;
class Camera;

void setVariables(glm::mat4 modelViewProj,
                  glm::mat4 projection,
                  glm::mat4 model,
                  int modelViewProjLocation,
                  const VertexBuffer* vertexBuffer,
                  const IndexBuffer* indexBuffer,
                  int modelViewLocation,
                  int invModelViewLocation,
                  glm::mat4 modelView,
                  glm::mat4 invModelView,
                  Camera* camera,
                  float time = 0.0f);

void resetPosition(Camera* camera);