#ifndef GAME_HPP_
#define GAME_HPP_

#include <random>

#include "abcgOpenGL.hpp"

enum class GameState { PAUSED, PLAYING };
enum class GameSpeed { SLOW = 2, MEDIUM = 4, FAST = 6 };
enum class GameSize { SMALL = 4, MEDIUM = 5, LARGE = 6 };
enum class Direction { UP, DOWN, LEFT, RIGHT };

class Game {
public:
  Game();
  void computeAllBoardPositions();
  void toggleGameState();
  void tick();
  void updateSnakeDirection(Direction direction);
  void updateSnakePosition();
  void respawnApple();
  void reset();
  bool hasEatenTheApple(glm::ivec3 nexSnakePosition);
  bool hasLost(glm::ivec3 nexSnakePosition);

  GameSpeed getGameSpeed();
  void setGameSpeed(GameSpeed gameSpeed);
  GameSize getGameSize();
  void setGameSize(GameSize gameSize);
  GameState getGameState();
  bool getShouldAnimateAppleSpawning();
  void setShouldAnimateAppleSpawning(bool shouldAnimateAppleSpawning);
  glm::ivec3 getApplePosition();
  std::vector<glm::ivec3> getSnakePositions();

private:
  abcg::Timer m_timer;
  std::default_random_engine m_randomEngine;

  GameSpeed m_gameSpeed{GameSpeed::MEDIUM};
  GameSize m_gameSize{GameSize::MEDIUM};

  std::vector<glm::ivec3> m_allBoardPositions{};

  GameState m_gameState{GameState::PLAYING};

  bool m_shouldAnimateAppleSpawning{true};
  glm::ivec3 m_applePosition{0, 0, 0};

  Direction m_snakeDirection{Direction::RIGHT};
  std::vector<glm::ivec3> m_snakePositions{{ -3, 0, 0 }};
};

#endif
