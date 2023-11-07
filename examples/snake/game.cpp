#include "game.hpp"

Game::Game() {
  for (int x = -m_boardRadius; x <= m_boardRadius; x++) {
    for (int z = -m_boardRadius; z <= m_boardRadius; z++) {
      glm::ivec3 boardPosition{x, 0, z};
      m_allBoardPositions.emplace_back(boardPosition);
    }
  }
}

void Game::tick() {
  if (m_timer.elapsed() < .25) return;

  m_timer.restart();

  updateSnakePosition();
}

void Game::updateSnakeDirection(Direction direction) {
  if ((m_snakeDirection == Direction::UP && direction == Direction::DOWN) ||
      (m_snakeDirection == Direction::DOWN && direction == Direction::UP) ||
      (m_snakeDirection == Direction::LEFT && direction == Direction::RIGHT) ||
      (m_snakeDirection == Direction::RIGHT && direction == Direction::LEFT))
    return;

  m_snakeDirection = direction;
}

void Game::updateSnakePosition() {
  auto const front = m_snakePositions.front();
  glm::ivec3 nextSnakePosition;

  if (m_snakeDirection == Direction::UP) nextSnakePosition = {front.x, front.y,  front.z - 1};
  if (m_snakeDirection == Direction::DOWN) nextSnakePosition = {front.x, front.y,  front.z + 1};
  if (m_snakeDirection == Direction::LEFT) nextSnakePosition = {front.x - 1, front.y,  front.z};
  if (m_snakeDirection == Direction::RIGHT) nextSnakePosition = {front.x + 1, front.y,  front.z};

  if (hasLost(nextSnakePosition)) {
    reset();
  } else if (hasEatenTheApple(nextSnakePosition)) {
    m_snakePositions.insert(m_snakePositions.begin(), nextSnakePosition);
    respawnApple();
  } else {
    m_snakePositions.insert(m_snakePositions.begin(), nextSnakePosition);
    m_snakePositions.pop_back();
  }
}

void Game::respawnApple() {
  std::vector<glm::ivec3> emptyBoardPositions{};

  for (auto const &boardPosition : m_allBoardPositions) {
    auto isBoardPositionOccupied = false;

    for (auto const &snakePosition : m_snakePositions) {
      if (snakePosition == boardPosition) {
        isBoardPositionOccupied = true;
      }
    }

    if (!isBoardPositionOccupied) emptyBoardPositions.emplace_back(boardPosition);
  }

  std::uniform_int_distribution<int> intDistribution(0, (int)emptyBoardPositions.size() - 1);

  m_applePosition = emptyBoardPositions[intDistribution(m_randomEngine)];
}

void Game::reset() {
  m_applePosition = {0, 0, 0};
  m_snakeDirection = Direction::RIGHT;
  m_snakePositions = {{ -3, 0, 0 }};
}

bool Game::hasEatenTheApple(glm::ivec3 nexSnakePosition) {
  return nexSnakePosition == m_applePosition;
}

bool Game::hasLost(glm::ivec3 nexSnakePosition) {
  auto const hasGoneOutOfBoard = nexSnakePosition.x > m_boardRadius ||
                                 nexSnakePosition.x < -m_boardRadius ||
                                 nexSnakePosition.z > m_boardRadius ||
                                 nexSnakePosition.z < -m_boardRadius;

  auto hasCrashedOnItself = false;

  for (auto const &position : m_snakePositions) {
    if (position.x == nexSnakePosition.x && position.z == nexSnakePosition.z) {
      hasCrashedOnItself = true;
    }
  }

  return hasGoneOutOfBoard || hasCrashedOnItself;
}

std::vector<glm::ivec3> Game::getSnakePositions() {
  return m_snakePositions;
}

glm::ivec3 Game::getApplePosition() {
  return m_applePosition;
}

int Game::getBoardRadius() {
  return m_boardRadius;
}
