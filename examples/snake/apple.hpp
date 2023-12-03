#ifndef APPLE_HPP_
#define APPLE_HPP_

#include "abcgOpenGL.hpp"

#include "game.hpp"
#include "camera.hpp"
#include "util.hpp"

class Apple {
public:
  Apple(Game& game, Camera& camera) : m_game(game), m_camera(camera) {}
  void create(GLuint program);
  void paint(float deltaTime);
  void updateAppleAnimation(float deltaTime);
  void destroy();

private:
  Game& m_game;
  Camera& m_camera;

  GLuint m_program{};
  GLuint m_diffuseTexture{};
  GLuint m_normalTexture{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_VAO{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 m_Id{0.25f, 0.25f, 0.25f, 1.0f};
  glm::vec4 m_Is{0.25f, 0.25f, 0.25f, 1.0f};
  glm::vec4 m_Ka{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 m_Kd{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 m_Ks{1.0f, 1.0f, 1.0f, 1.0f};
  float m_shininess{10.0f};

  bool m_appleAnimationUp{true};
  float m_appleAnimationTranslation{0};
  float m_appleAnimationRotation{0};
  float m_appleAnimationScale{0};
};

#endif
