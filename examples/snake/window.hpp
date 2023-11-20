#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "game.hpp"
#include "camera.hpp"
#include "snake.hpp"
#include "apple.hpp"
#include "ground.hpp"
#include "util.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onEvent(SDL_Event const &event) override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_program{};

  GLint m_modelMatrixLocation{};
  GLint m_viewMatrixLocation{};
  GLint m_projectionMatrixLocation{};
  GLint m_colorLocation{};

  Camera m_camera;
  float m_pedestalSpeed{0.0f};
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};
  float m_tiltSpeed{0.0f};

  Game m_game;
  Snake m_snake{m_game};
  Apple m_apple{m_game};
  Ground m_ground{m_game};
};

#endif
