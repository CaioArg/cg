#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "abcgOpenGL.hpp"

class Camera {
public:
  void computeViewMatrix();
  glm::mat4 const &getViewMatrix() const;

  void computeProjectionMatrix(glm::vec2 const &size);
  glm::mat4 const &getProjectionMatrix() const;

  void pedestal(float speed);
  void dolly(float speed);
  void truck(float speed);
  void pan(float speed);
  void tilt(float speed);

  void reset();

private:
  glm::vec3 m_eye{0.0f, 10.0f, 5.0f};
  glm::vec3 m_at{0.0f, 0.0f, 0.0f};
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};

  glm::mat4 m_viewMatrix;
  glm::mat4 m_projectionMatrix;
};

#endif
