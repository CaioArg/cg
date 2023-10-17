#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaint() override;
  void onEvent(SDL_Event const &event) override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBOVertices{};
  GLuint m_program{};

  bool m_isDragging{false};
  float const m_circleRadius{.1};
  glm::vec2 m_circleLastFramePosition{0, 0};
  glm::vec2 m_circlePosition{0, 0};
  glm::vec2 m_circleSpeed{0, 0};

  void setupModel();
  void updateCirclePositionAndSpeedDirection();
  void invertYSpeed();
  void invertXSpeed();
  void updateCircleSpeed();
  void updateLastFrame();
  void updateCursor() const;
  [[nodiscard]] bool isMouseIntersectingBall() const;
  [[nodiscard]] bool isCircleXPositionInvalid(float x) const;
  [[nodiscard]] bool isCircleYPositionInvalid(float y) const;
  [[nodiscard]] glm::vec2 getNdcMousePosition() const;
  [[nodiscard]] glm::vec2 getNdcCoordinates(int viewportX, int viewportY) const;
};

#endif
