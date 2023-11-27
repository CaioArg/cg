#include "ground.hpp"

void Ground::create(GLuint program) {
  m_program = program;

  m_viewMatrixLocation = abcg::glGetUniformLocation(program, "viewMatrix");
  m_projectionMatrixLocation = abcg::glGetUniformLocation(program, "projectionMatrix");
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

void Ground::paint() const {
  abcg::glUseProgram(m_program);

  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, &m_camera.getProjectionMatrix()[0][0]);

  abcg::glBindVertexArray(m_VAO);

  auto const boardRadius{static_cast<int>(m_game.getGameSize())};

  for (auto const z : iter::range(-boardRadius, boardRadius + 1)) {
    for (auto const x : iter::range(-boardRadius, boardRadius + 1)) {
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(x, 0.125f, z));
      model = glm::scale(model, glm::vec3(1.0f, 0.25f, 1.0f));
      abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);

      auto const i{(z + x) % 2 == 0 ? 1.0f : 0.85f};
      abcg::glUniform4f(m_colorLocation, 0.39f * i, 0.78f * i, 0.29f * i, 1.0f);

      abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }
  }

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Ground::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}
