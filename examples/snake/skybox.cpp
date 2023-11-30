#include "skybox.hpp"

void Skybox::create(GLuint program) {
  m_program = program;

  auto const cubeMapPath{abcg::Application::getAssetsPath() + "maps/top-of-mountain-cubemap/"};

  m_cubeTexture = abcg::loadOpenGLCubemap({.paths = {
     cubeMapPath + "posx.jpg",
     cubeMapPath + "negx.jpg",
     cubeMapPath + "posy.jpg",
     cubeMapPath + "negy.jpg",
     cubeMapPath + "posz.jpg",
     cubeMapPath + "negz.jpg",
 }});

  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenVertexArrays(1, &m_VAO);

  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  auto const positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);
}

void Skybox::paint() {
  auto const viewMatrix{m_camera.getViewMatrix()};
  auto const projectionMatrix{m_camera.getProjectionMatrix()};

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);

  abcg::glUniformMatrix4fv(abcg::glGetUniformLocation(m_program, "viewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(abcg::glGetUniformLocation(m_program, "projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
  abcg::glUniform1i(abcg::glGetUniformLocation(m_program, "skyTex"), 0);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTexture);

  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);

  abcg::glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

  abcg::glDepthFunc(GL_LESS);
  abcg::glFrontFace(GL_CCW);
  abcg::glDisable(GL_CULL_FACE);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Skybox::destroy() {
  abcg::glDeleteTextures(1, &m_cubeTexture);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}
