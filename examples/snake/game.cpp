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
  if (m_snakePositions.size() == 1) {
    m_snakeDirection = direction;
    return;
  }

  auto const snakeHead{m_snakePositions.at(0)};
  auto const snakeNeck{m_snakePositions.at(1)};

  if ((direction == Direction::UP && snakeHead.x == snakeNeck.x && snakeHead.z - 1 == snakeNeck.z) ||
      (direction == Direction::DOWN && snakeHead.x == snakeNeck.x && snakeHead.z == snakeNeck.z - 1) ||
      (direction == Direction::LEFT && snakeHead.x - 1 == snakeNeck.x && snakeHead.z == snakeNeck.z) ||
      (direction == Direction::RIGHT && snakeHead.x == snakeNeck.x - 1 && snakeHead.z == snakeNeck.z))
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
  m_shouldAnimateAppleSpawning = true;
}

void Game::reset() {
  m_shouldAnimateAppleSpawning = true;
  m_applePosition = {0, 0, 0};
  m_snakeDirection = Direction::RIGHT;
  m_snakePositions = {{ -3, 0, 0 }};
}

bool Game::hasEatenTheApple(glm::ivec3 nextSnakePosition) {
  return nextSnakePosition == m_applePosition;
}

bool Game::hasLost(glm::ivec3 nextSnakePosition) {
  auto const hasGoneOutOfBoard = nextSnakePosition.x > m_boardRadius ||
                                 nextSnakePosition.x < -m_boardRadius ||
                                 nextSnakePosition.z > m_boardRadius ||
                                 nextSnakePosition.z < -m_boardRadius;

  auto hasCrashedOnItself = false;

  for (auto const &position : m_snakePositions) {
    if (position.x == nextSnakePosition.x &&
        position.z == nextSnakePosition.z &&
        position != m_snakePositions.back()) {
      hasCrashedOnItself = true;
    }
  }

  return hasGoneOutOfBoard || hasCrashedOnItself;
}

int Game::getBoardRadius() {
  return m_boardRadius;
}

bool Game::getShouldAnimateAppleSpawning() {
  return m_shouldAnimateAppleSpawning;
}

void Game::setShouldAnimateAppleSpawning(bool shouldAnimateAppleSpawning) {
  m_shouldAnimateAppleSpawning = shouldAnimateAppleSpawning;
}

glm::ivec3 Game::getApplePosition() {
  return m_applePosition;
}

std::vector<glm::ivec3> Game::getSnakePositions() {
  return m_snakePositions;
}
