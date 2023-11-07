#ifndef GAME_HPP_
#define GAME_HPP_

#include <random>

#include "abcgOpenGL.hpp"

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Game {
public:
  Game();
  void tick();
  void updateSnakeDirection(Direction direction);
  void updateSnakePosition();
  void respawnApple();
  void reset();
  bool hasEatenTheApple(glm::ivec3 nexSnakePosition);
  bool hasLost(glm::ivec3 nexSnakePosition);

  std::vector<glm::ivec3> getSnakePositions();
  glm::ivec3 getApplePosition();
  int getBoardRadius();

private:
  abcg::Timer m_timer;
  std::default_random_engine m_randomEngine;

  int const m_boardRadius{5};
  std::vector<glm::ivec3> m_allBoardPositions{};

  glm::ivec3 m_applePosition{0, 0, 0};

  Direction m_snakeDirection{Direction::RIGHT};
  std::vector<glm::ivec3> m_snakePositions{{ -3, 0, 0 }};
};

#endif
