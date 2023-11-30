#include "window.hpp"

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(5.0f / 255.0f, 30.0f / 255.0f, 30.0f / 255.0f, 1);

  abcg::glEnable(GL_DEPTH_TEST);

  m_program = abcg::createOpenGLProgram({
      {.source = assetsPath + "shaders/main.vert", .stage = abcg::ShaderStage::Vertex},
      {.source = assetsPath + "shaders/main.frag", .stage = abcg::ShaderStage::Fragment},
  });

  m_apple_program = abcg::createOpenGLProgram({
      {.source = assetsPath + "shaders/apple.vert", .stage = abcg::ShaderStage::Vertex},
      {.source = assetsPath + "shaders/apple.frag", .stage = abcg::ShaderStage::Fragment},
  });

  m_skybox_program = abcg::createOpenGLProgram({
      {.source = assetsPath + "shaders/skybox.vert", .stage = abcg::ShaderStage::Vertex},
      {.source = assetsPath + "shaders/skybox.frag", .stage = abcg::ShaderStage::Fragment},
  });

  m_snake.create(m_program);
  m_apple.create(m_apple_program);
  m_ground.create(m_program);
  m_skybox.create(m_skybox_program);
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

  m_snake.paint();
  m_apple.paint((float) getDeltaTime());
  m_ground.paint();
  m_skybox.paint();
}

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (m_game.getGameState() == GameState::PLAYING) {
      if (event.key.keysym.sym == SDLK_UP) m_game.updateSnakeDirection(Direction::UP);
      if (event.key.keysym.sym == SDLK_DOWN) m_game.updateSnakeDirection(Direction::DOWN);
      if (event.key.keysym.sym == SDLK_LEFT) m_game.updateSnakeDirection(Direction::LEFT);
      if (event.key.keysym.sym == SDLK_RIGHT) m_game.updateSnakeDirection(Direction::RIGHT);
    }

    if (event.key.keysym.sym == SDLK_ESCAPE) m_game.toggleGameState();
    if (event.key.keysym.sym == SDLK_TAB) m_camera.reset();

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

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  ImGui::GetIO().WantCaptureKeyboard = false;

  {
    ImGui::SetNextWindowSize(ImVec2(250, 100));
    ImGui::SetNextWindowPos(ImVec2(20, 20));

    auto const windowFlags{
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground
    };

    ImGui::Begin("Score", nullptr, windowFlags);

    auto const score{m_game.getSnakePositions().size() - 1};
    static auto bestScore{score};
    bestScore = score > bestScore ? score : bestScore;
    auto const isBreakingRecords{score == bestScore && score != 0};

    if (isBreakingRecords) {
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 50, 255));
    }

    ImGui::Text("Score: %lu", score);
    ImGui::Text("Best score: %lu", bestScore);

    if (isBreakingRecords) {
      ImGui::PopStyleColor();
    }

    ImGui::End();
  }

  if (m_game.getGameState() != GameState::PAUSED) return;

  ImGui::SetNextWindowSize(ImVec2(300, 200));
  ImGui::SetNextWindowPos(ImVec2(
      (m_viewportSize.x / m_dpr / 2) - 150,
      (m_viewportSize.y / m_dpr / 2) - 100
  ));

  auto const windowFlags{
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoScrollbar
  };

  ImGui::Begin("Snake", nullptr, windowFlags);

  {
    using enum GameSpeed;
    auto const gameSpeed{m_game.getGameSpeed()};

    if (ImGui::BeginCombo("Speed", gameSpeedToString(gameSpeed).c_str())) {
      for (auto const &option : {SLOW, MEDIUM, FAST}) {
        auto const isSelected{option == gameSpeed};

        if (ImGui::Selectable(gameSpeedToString(option).c_str(), isSelected) && option != gameSpeed) {
          m_game.setGameSpeed(option);
        }

        if (isSelected) {
          ImGui::SetItemDefaultFocus();
        }
      }

      ImGui::EndCombo();
    }
  }

  {
    using enum GameSize;
    auto const gameSize{m_game.getGameSize()};

    if (ImGui::BeginCombo("Size", gameSizeToString(gameSize).c_str())) {
      for (auto const &option : {SMALL, MEDIUM, LARGE}) {
        auto const isSelected{option == gameSize};

        if (ImGui::Selectable(gameSizeToString(option).c_str(), isSelected) && option != gameSize) {
          m_game.setGameSize(option);
          m_camera.reset();
        }

        if (isSelected) {
          ImGui::SetItemDefaultFocus();
        }
      }

      ImGui::EndCombo();
    }
  }

  {
    if (ImGui::Button("Reset camera")) {
      m_camera.reset();
    }
  }

  {
    if (ImGui::Button("Restart game")) {
      m_game.reset();
    }
  }

  ImGui::End();
}

void Window::onResize(glm::ivec2 const &size) {
  static auto isInitialRender{true};

  if (isInitialRender) {
    m_dpr = size.x / 600;
    isInitialRender = false;
  }

  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_snake.destroy();
  m_apple.destroy();
  m_ground.destroy();
  m_skybox.destroy();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteProgram(m_apple_program);
  abcg::glDeleteProgram(m_skybox_program);
}
