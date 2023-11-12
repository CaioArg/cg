#ifndef APPLE_HPP_
#define APPLE_HPP_

#include "abcgOpenGL.hpp"

#include "game.hpp"

class Apple {
public:
  Apple(Game& game) : m_game(game) {}
  void create(GLuint program);
  void paint(float deltaTime);
  void updateAppleAnimation(float deltaTime);
  void destroy();

private:
  Game& m_game;

  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_VAO{};
  GLuint m_program{};

  std::vector<glm::vec3> m_vertices;
  std::vector<GLuint> m_indices;

  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};

  bool m_appleAnimationUp{true};
  float m_appleAnimationTranslation{0};
  float m_appleAnimationRotation{0};
  float m_appleAnimationScale{0};
};

#endif
