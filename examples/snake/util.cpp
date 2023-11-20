#include <unordered_map>

#include "util.hpp"

struct Vertex {
  glm::vec3 position{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

std::tuple<std::vector<glm::vec3>, std::vector<GLuint>> loadModelFromFile(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attributes{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  std::vector<Vertex> vertices{};
  std::vector<GLuint> indices{};

  std::unordered_map<Vertex, GLuint> hash{};

  for (auto const &shape : shapes) {
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      auto const index{shape.mesh.indices.at(offset)};

      auto const startIndex{3 * index.vertex_index};
      auto const vx{attributes.vertices.at(startIndex + 0)};
      auto const vy{attributes.vertices.at(startIndex + 1)};
      auto const vz{attributes.vertices.at(startIndex + 2)};

      Vertex const vertex{.position = {vx, vy, vz}};

      if (!hash.contains(vertex)) {
        hash[vertex] = vertices.size();
        vertices.push_back(vertex);
      }

      indices.push_back(hash[vertex]);
    }
  }

  std::vector<glm::vec3> glmVectors;

  for (auto const &vertex : vertices) glmVectors.push_back(vertex.position);

  return {glmVectors, indices};
}

std::string gameSpeedToString(GameSpeed gameSpeed) {
  using enum GameSpeed;

  if (gameSpeed == SLOW) return "Slow";
  else if (gameSpeed == MEDIUM) return "Medium";
  else return "Fast";
}

std::string gameSizeToString(GameSize gameSize) {
  using enum GameSize;

  if (gameSize == SMALL) return "Small";
  else if (gameSize == MEDIUM) return "Medium";
  else return "Large";
}
