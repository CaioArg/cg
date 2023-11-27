#include <unordered_map>

#include "util.hpp"

template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    auto const h2{std::hash<glm::vec3>()(vertex.normal)};
    auto const h3{std::hash<glm::vec2>()(vertex.texCoord)};
    return abcg::hashCombine(h1, h2, h3);
  }
};

std::tuple<std::vector<Vertex>, std::vector<GLuint>> loadModelFromFile(std::string_view path) {
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

      auto const vertexStartIndex{3 * index.vertex_index};
      glm::vec3 position{
          attributes.vertices.at(vertexStartIndex + 0),
          attributes.vertices.at(vertexStartIndex + 1),
          attributes.vertices.at(vertexStartIndex + 2),
      };

      auto const normalStartIndex{3 * index.normal_index};
      glm::vec3 normal{
          attributes.normals.at(normalStartIndex + 0),
          attributes.normals.at(normalStartIndex + 1),
          attributes.normals.at(normalStartIndex + 2),
      };

      auto const texCoordStartIndex{2 * index.texcoord_index};
      glm::vec2 texCoord{
          attributes.texcoords.at(texCoordStartIndex + 0),
          attributes.texcoords.at(texCoordStartIndex + 1),
      };

      Vertex const vertex{.position = position, .normal = normal, .texCoord = texCoord};

      if (!hash.contains(vertex)) {
        hash[vertex] = vertices.size();
        vertices.push_back(vertex);
      }

      indices.push_back(hash[vertex]);
    }
  }

  return {vertices, indices};
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
