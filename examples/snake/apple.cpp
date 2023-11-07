#include "apple.hpp"
#include "util.hpp"

void Apple::create(GLuint program) {
  m_modelMatrixLocation = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(program, "color");

  auto const [vertices, indices] = loadModelFromFile(abcg::Application::getAssetsPath() + "apple.obj");
  m_vertices = vertices;
  m_indices = indices;

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

void Apple::paint(float deltaTime) {
  updateAppleAnimation(deltaTime);

  auto const applePosition{m_game.getApplePosition()};

  abcg::glBindVertexArray(m_VAO);

  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(applePosition.x, 0.750f + m_appleAnimationTranslation, applePosition.z));
  model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.90f, 0.27f, 0.11f, 1.0f);

  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}

void Apple::updateAppleAnimation(float deltaTime) {
  if (m_appleAnimationUp) {
    if (m_appleAnimationTranslation > 0.2f) {
      m_appleAnimationUp = false;
    } else {
      m_appleAnimationTranslation += 0.6 * deltaTime;
    }
  } else {
    if (m_appleAnimationTranslation < 0.0f) {
      m_appleAnimationUp = true;
    } else {
      m_appleAnimationTranslation -= 0.6 * deltaTime;
    }
  }
}

void Apple::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}
