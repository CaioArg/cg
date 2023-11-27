#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"

#include "game.hpp"
#include "camera.hpp"

class Ground {
public:
  Ground(Game& game, Camera& camera) : m_game(game), m_camera(camera) {}
  void create(GLuint program);
  void paint() const;
  void destroy();

private:
  Game& m_game;
  Camera& m_camera;

  GLuint m_program{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_VAO{};

  std::array<glm::vec3, 8> m_vertices{{
      {-0.5f, -0.5f, +0.5f},
      {+0.5f, -0.5f, +0.5f},
      {+0.5f, +0.5f, +0.5f},
      {-0.5f, +0.5f, +0.5f},
      {-0.5f, -0.5f, -0.5f},
      {+0.5f, -0.5f, -0.5f},
      {+0.5f, +0.5f, -0.5f},
      {-0.5f, +0.5f, -0.5f},
  }};
  std::array<GLuint, 36> m_indices{
      0, 1, 2,
      2, 3, 0,
      1, 5, 6,
      6, 2, 1,
      7, 6, 5,
      5, 4, 7,
      4, 0, 3,
      3, 7, 4,
      4, 5, 1,
      1, 0, 4,
      3, 2, 6,
      6, 7, 3,
  };

  GLint m_viewMatrixLocation{};
  GLint m_projectionMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};
};

#endif
