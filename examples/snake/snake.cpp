#include "snake.hpp"

std::tuple<float, float, float> Snake::getSnakeColor(unsigned long position, unsigned long snakeSize) const {
  float coefficient;

  if (position == 0) coefficient = 0;
  else if (position == snakeSize - 1) coefficient = 1;
  else coefficient = (float) position / (float) (snakeSize - 1);

  auto const [headRed, headGreen, headBlue] = m_headColor;
  auto const [tailRed, tailGreen, tailBlue] = m_tailColor;

  auto const red{(float) (headRed + ((tailRed - headRed) * coefficient)) / 255.0f};
  auto const green{(float) (headGreen + ((tailGreen - headGreen) * coefficient)) / 255.0f};
  auto const blue{(float) (headBlue + ((tailBlue - headBlue) * coefficient)) / 255.0f};

  return {red, green, blue};
}

void Snake::create(GLuint program) {
  m_modelMatrixLocation = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(program, "color");

  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices.at(0)) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices.at(0)) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  abcg::glGenVertexArrays(1, &m_VAO);

  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  abcg::glBindVertexArray(0);
}

void Snake::paint() const {
  abcg::glBindVertexArray(m_VAO);

  auto const snakePositions{m_game.getSnakePositions()};
  auto const snakeSize{snakePositions.size()};

  for (unsigned long index = 0; index < snakeSize; index++) {
    auto const &position{snakePositions.at(index)};
    auto const [red, green, blue] = getSnakeColor(index, snakeSize);

    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(position.x, 0.751, position.z));

    abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    abcg::glUniform4f(m_colorLocation, red, green, blue, 1.0f);

    abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
  }

  abcg::glBindVertexArray(0);
}

void Snake::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}
