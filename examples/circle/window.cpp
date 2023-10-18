#include "window.hpp"

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es
    layout(location = 0) in vec2 inPosition;

    void main() { gl_Position = vec4(inPosition, 0, 1); }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es
    precision mediump float;

    out vec4 outColor;

    void main() { outColor = vec4(1, 1, 1, 1); }
  )gl"};

  m_program = abcg::createOpenGLProgram({
      {.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
      {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment},
  });

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  updateCirclePositionAndSpeedDirection();
  updateCircleSpeed();
  updateLastFrame();
  updateCursor();
  setupModel();

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 100 + 2);
  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_MOUSEBUTTONDOWN &&
      event.button.button == SDL_BUTTON_LEFT &&
      isMouseIntersectingBall()) {
    m_isDragging = true;
    m_circleSpeed = {0, 0};
    m_circlePosition = getNdcMousePosition();
  }

  if (event.type == SDL_MOUSEBUTTONUP &&
      event.button.button == SDL_BUTTON_LEFT &&
      m_isDragging) {
    m_isDragging = false;
    m_circleSpeed = 10.0f * (m_circlePosition - m_circleLastFramePosition);
  }

  if (event.type == SDL_MOUSEMOTION && m_isDragging) {
    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    auto const ndcMousePosition = getNdcCoordinates(mousePosition.x, mousePosition.y);

    auto const isXPositionInvalid = isCircleXPositionInvalid(ndcMousePosition.x);
    auto const isYPositionInvalid = isCircleYPositionInvalid(ndcMousePosition.y);

    if (isXPositionInvalid && isYPositionInvalid) return;
    else if (isXPositionInvalid) m_circlePosition = {m_circlePosition.x, ndcMousePosition.y};
    else if (isYPositionInvalid) m_circlePosition = {ndcMousePosition.x, m_circlePosition.y};
    else m_circlePosition = ndcMousePosition;
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::setupModel() {
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  std::vector<glm::vec2> positions;

  positions.emplace_back(m_circlePosition.x, m_circlePosition.y);

  auto const sides{100};
  auto const step{M_PI * 2 / sides};

  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(m_circlePosition.x + std::cos(angle) * m_circleRadius, m_circlePosition.y + std::sin(angle) * m_circleRadius);
  }

  positions.push_back(positions[1]);

  abcg::glGenBuffers(1, &m_VBOVertices);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  auto const positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  abcg::glGenVertexArrays(1, &m_VAO);

  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);
}

void Window::updateCirclePositionAndSpeedDirection() {
  glm::vec2 const newCirclePosition = m_circlePosition + (m_circleSpeed * (float) getDeltaTime());

  auto const invalidXPosition = isCircleXPositionInvalid(newCirclePosition.x);
  auto const invalidYPosition = isCircleYPositionInvalid(newCirclePosition.y);

  if (invalidXPosition) invertAndDecreaseXSpeed();
  if (invalidYPosition) invertAndDecreaseYSpeed();

  if (!invalidXPosition && !invalidYPosition) m_circlePosition = newCirclePosition;
}

void Window::invertAndDecreaseXSpeed() {
  m_circleSpeed.x *= -.25;
}

void Window::invertAndDecreaseYSpeed() {
  m_circleSpeed.y *= -.25;
}

void Window::updateCircleSpeed() {
  m_circleSpeed -= 0.2f * m_circleSpeed * (float)getDeltaTime();
}

void Window::updateLastFrame() {
  m_circleLastFramePosition = m_circlePosition;
}

void Window::updateCursor() const {
  SDL_SetCursor(SDL_CreateSystemCursor(isMouseIntersectingBall() ? SDL_SYSTEM_CURSOR_HAND : SDL_SYSTEM_CURSOR_ARROW));
}

[[nodiscard]] bool Window::isMouseIntersectingBall() const {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
  auto const ndcMousePosition = getNdcCoordinates(mousePosition.x, mousePosition.y);
  return m_circleRadius >= glm::distance(ndcMousePosition, m_circlePosition);
}

[[nodiscard]] bool Window::isCircleXPositionInvalid(float x) const {
  return x > 1 - m_circleRadius || x < -1 + m_circleRadius;
}

[[nodiscard]] bool Window::isCircleYPositionInvalid(float y) const {
  return y > 1 - m_circleRadius || y < -1 + m_circleRadius;
}

[[nodiscard]] glm::vec2 Window::getNdcMousePosition() const {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
  return getNdcCoordinates(mousePosition.x, mousePosition.y);
}

[[nodiscard]] glm::vec2 Window::getNdcCoordinates(int viewportX, int viewportY) const {
  auto const xPercentage{(float)viewportX / ((float)m_viewportSize.x)};
  auto const yPercentage{(float)viewportY / ((float)m_viewportSize.y)};

  auto const ndcX{-1 + (xPercentage * 2)};
  auto const ndcY{1 - (yPercentage * 2)};

  return {ndcX, ndcY};
}
