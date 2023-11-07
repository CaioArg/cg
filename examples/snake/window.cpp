#include "window.hpp"

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0.74f, 0.92f, 1.0f, 1);

  abcg::glEnable(GL_DEPTH_TEST);

  m_program = abcg::createOpenGLProgram({
      {.source = assetsPath + "main.vert", .stage = abcg::ShaderStage::Vertex},
      {.source = assetsPath + "main.frag", .stage = abcg::ShaderStage::Fragment},
  });

  m_snake.create(m_program);
  m_apple.create(m_program);
  m_ground.create(m_program);

  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projectionMatrixLocation = abcg::glGetUniformLocation(m_program, "projectionMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  m_camera.pedestal(m_pedestalSpeed * deltaTime);
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.tilt(m_tiltSpeed * deltaTime);

  m_game.tick();
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, &m_camera.getProjectionMatrix()[0][0]);

  m_snake.paint();
  m_apple.paint((float) getDeltaTime());
  m_ground.paint();

  abcg::glUseProgram(0);
}

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP) m_game.updateSnakeDirection(Direction::UP);
    if (event.key.keysym.sym == SDLK_DOWN) m_game.updateSnakeDirection(Direction::DOWN);
    if (event.key.keysym.sym == SDLK_LEFT) m_game.updateSnakeDirection(Direction::LEFT);
    if (event.key.keysym.sym == SDLK_RIGHT) m_game.updateSnakeDirection(Direction::RIGHT);

    if (event.key.keysym.sym == SDLK_SPACE) m_pedestalSpeed = 3.0f;
    if (event.key.keysym.sym == SDLK_LSHIFT) m_pedestalSpeed = -3.0f;
    if (event.key.keysym.sym == SDLK_w) m_dollySpeed = 3.0f;
    if (event.key.keysym.sym == SDLK_s) m_dollySpeed = -3.0f;
    if (event.key.keysym.sym == SDLK_a) m_truckSpeed = -3.0f;
    if (event.key.keysym.sym == SDLK_d) m_truckSpeed = 3.0f;
    if (event.key.keysym.sym == SDLK_q) m_panSpeed = -3.0f;
    if (event.key.keysym.sym == SDLK_e) m_panSpeed = 3.0f;
    if (event.key.keysym.sym == SDLK_r) m_tiltSpeed = 3.0f;
    if (event.key.keysym.sym == SDLK_f) m_tiltSpeed = -3.0f;
  }

  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE && m_pedestalSpeed > 0) m_pedestalSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_LSHIFT && m_pedestalSpeed < 0) m_pedestalSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_w && m_dollySpeed > 0) m_dollySpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_s && m_dollySpeed < 0) m_dollySpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_a && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_d && m_truckSpeed > 0) m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_panSpeed < 0) m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_panSpeed > 0) m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_r && m_tiltSpeed > 0) m_tiltSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_f && m_tiltSpeed < 0) m_tiltSpeed = 0.0f;
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_snake.destroy();
  m_apple.destroy();
  m_ground.destroy();

  abcg::glDeleteProgram(m_program);
}
