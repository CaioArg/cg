#include "camera.hpp"

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

glm::mat4 const &Camera::getViewMatrix() const {
  return m_viewMatrix;
}

void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projectionMatrix = glm::perspective(glm::radians(70.0f), size.x / size.y, 0.1f, 100.0f);
}

glm::mat4 const &Camera::getProjectionMatrix() const {
  return m_projectionMatrix;
}

void Camera::pedestal(float speed) {
  m_eye += m_up * speed;
  m_at += m_up * speed;

  computeViewMatrix();
}

void Camera::dolly(float speed) {
  auto const forward{glm::normalize(m_at - m_eye)};

  m_eye += forward * speed;
  m_at += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  auto const forward{glm::normalize(m_at - m_eye)};
  auto const left{glm::cross(m_up, forward)};

  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}

void Camera::pan(float speed) {
  glm::mat4 transform{1.0f};

  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}

// FIXME - buggy
void Camera::tilt(float speed) {
  glm::mat4 transform{1.0f};

  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, speed, {1.0f, 0.0f, 0.0f});
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}
