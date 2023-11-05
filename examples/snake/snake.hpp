#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include "abcgOpenGL.hpp"

class Snake {
public:
  void create(GLuint program);
  void paint() const;
  void destroy();

private:
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_VAO{};
  GLuint m_program{};

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

  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};

  std::vector<glm::ivec3> m_snakePositions{
      { -5, 0, -5 },
      { -4, 0, -5 },
      { -3, 0, -5 },
      { -3, 0, -4 },
      { -3, 0, -3 },
      { -3, 0, -2 },
      { -3, 0, -1 },
      { -2, 0, -1 },
  };
};

#endif
