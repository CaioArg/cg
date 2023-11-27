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
  int m_dpr{};

  GLuint m_program{};
  GLuint m_apple_program{};

  Game m_game;

  Camera m_camera{m_game};
  float m_pedestalSpeed{0.0f};
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};
  float m_tiltSpeed{0.0f};

  Snake m_snake{m_game, m_camera};
  Apple m_apple{m_game, m_camera};
  Ground m_ground{m_game, m_camera};
};

#endif
