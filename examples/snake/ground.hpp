#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"

class Ground {
public:
  void create(GLuint program);
  void paint() const;
  void destroy();

private:
  GLuint m_VBO{};
  GLuint m_VAO{};

  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};
};

#endif
