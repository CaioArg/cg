#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "abcgOpenGL.hpp"

#include "game.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

std::tuple<std::vector<Vertex>, std::vector<GLuint>> loadModelFromFile(std::string_view path);

std::string gameSpeedToString(GameSpeed gameSpeed);

std::string gameSizeToString(GameSize gameSize);

#endif
