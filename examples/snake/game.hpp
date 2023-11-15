#ifndef GAME_HPP_
#define GAME_HPP_

#include <random>

#include "abcgOpenGL.hpp"

enum class GameState { PAUSED, PLAYING };

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Game {
public:
  Game();
  void toggleGameState();
  void tick();
  void updateSnakeDirection(Direction direction);
  void updateSnakePosition();
  void respawnApple();
  void reset();
  bool hasEatenTheApple(glm::ivec3 nexSnakePosition);
  bool hasLost(glm::ivec3 nexSnakePosition);

  int getBoardRadius();
  GameState getGameState();
  bool getShouldAnimateAppleSpawning();
  void setShouldAnimateAppleSpawning(bool shouldAnimateAppleSpawning);
  glm::ivec3 getApplePosition();
  std::vector<glm::ivec3> getSnakePositions();

private:
  abcg::Timer m_timer;
  std::default_random_engine m_randomEngine;

  int const m_boardRadius{5};
  std::vector<glm::ivec3> m_allBoardPositions{};

  GameState m_gameState{GameState::PLAYING};

  bool m_shouldAnimateAppleSpawning{true};
  glm::ivec3 m_applePosition{0, 0, 0};

  Direction m_snakeDirection{Direction::RIGHT};
  std::vector<glm::ivec3> m_snakePositions{{ -3, 0, 0 }};
};

#endif
