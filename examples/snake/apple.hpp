#ifndef APPLE_HPP_
#define APPLE_HPP_

#include "abcgOpenGL.hpp"

class Apple {
public:
  void create(GLuint program);
  void paint() const;
  void destroy();

private:
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_VAO{};
  GLuint m_program{};

  std::vector<glm::vec3> m_vertices;
  std::vector<GLuint> m_indices;

  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};
};

#endif
