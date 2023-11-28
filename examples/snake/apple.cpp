#include <math.h>

#include "apple.hpp"

void Apple::create(GLuint program) {
  m_program = program;

  m_diffuseTexture = abcg::loadOpenGLTexture({.path = abcg::Application::getAssetsPath() + "/textures/Apple_BaseColor.png"});

  auto const [vertices, indices] = loadModelFromFile(abcg::Application::getAssetsPath() + "/models/apple.obj");
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

  auto const positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

  auto const normalAttribute{abcg::glGetAttribLocation(m_program, "inNormal")};
  abcg::glEnableVertexAttribArray(normalAttribute);
  abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));

  auto const texCoordAttribute{abcg::glGetAttribLocation(m_program, "inTexCoord")};
  abcg::glEnableVertexAttribArray(texCoordAttribute);
  abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoord)));

  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  abcg::glBindVertexArray(0);
}

void Apple::paint(float deltaTime) {
  updateAppleAnimation(deltaTime);

  auto const applePosition{m_game.getApplePosition()};

  auto const viewMatrix{m_camera.getViewMatrix()};
  auto const projectionMatrix{m_camera.getProjectionMatrix()};

  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(applePosition.x, 0.750f + m_appleAnimationTranslation, applePosition.z));
  model = glm::rotate(model, m_appleAnimationRotation, {0.0f, 1.0f, 0.0f});
  model = glm::scale(model, m_appleAnimationScale * glm::vec3(0.01f));

  auto const normalMatrix{glm::inverseTranspose(glm::mat3(viewMatrix * model))};

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  abcg::glUniformMatrix4fv(abcg::glGetUniformLocation(m_program, "viewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(abcg::glGetUniformLocation(m_program, "projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
  abcg::glUniformMatrix4fv(abcg::glGetUniformLocation(m_program, "modelMatrix"), 1, GL_FALSE, &model[0][0]);
  abcg::glUniformMatrix3fv(abcg::glGetUniformLocation(m_program, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform1i(abcg::glGetUniformLocation(m_program, "diffuseTex"), 0);

  abcg::glUniform4fv(abcg::glGetUniformLocation(m_program, "lightDirWorldSpace"), 1, &m_lightDir.x);
  abcg::glUniform4fv(abcg::glGetUniformLocation(m_program, "Ia"), 1, &m_Ia.x);
  abcg::glUniform4fv(abcg::glGetUniformLocation(m_program, "Id"), 1, &m_Id.x);
  abcg::glUniform4fv(abcg::glGetUniformLocation(m_program, "Is"), 1, &m_Is.x);
  abcg::glUniform4fv(abcg::glGetUniformLocation(m_program, "Ka"), 1, &m_Ka.x);
  abcg::glUniform4fv(abcg::glGetUniformLocation(m_program, "Kd"), 1, &m_Kd.x);
  abcg::glUniform4fv(abcg::glGetUniformLocation(m_program, "Ks"), 1, &m_Ks.x);
  abcg::glUniform1f(abcg::glGetUniformLocation(m_program, "shininess"), m_shininess);

  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Apple::updateAppleAnimation(float deltaTime) {
  m_appleAnimationRotation += 2 * M_PI * deltaTime;

  if (m_game.getShouldAnimateAppleSpawning()) {
    m_game.setShouldAnimateAppleSpawning(false);
    m_appleAnimationScale = 0;
  }

  if (m_appleAnimationScale < 1) {
    m_appleAnimationScale = std::min(m_appleAnimationScale + (4 * deltaTime), 1.0f);
  }

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
  abcg::glDeleteTextures(1, &m_diffuseTexture);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}
