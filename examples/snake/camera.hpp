#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "abcgOpenGL.hpp"

#include "game.hpp"

class Camera {
public:
  Camera(Game& game);

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
  Game& m_game;

  glm::vec3 m_eye{0.0f, 12.25f, 6.125f};
  glm::vec3 m_at{0.0f, 0.0f, 0.0f};
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};

  glm::mat4 m_viewMatrix;
  glm::mat4 m_projectionMatrix;
};

#endif
