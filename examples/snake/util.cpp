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

  std::vector bitangents(vertices.size(), glm::vec3(0));

  for (auto const offset : iter::range(0UL, indices.size(), 3UL)) {
    auto const i1{indices.at(offset + 0)};
    auto const i2{indices.at(offset + 1)};
    auto const i3{indices.at(offset + 2)};

    auto &v1{vertices.at(i1)};
    auto &v2{vertices.at(i2)};
    auto &v3{vertices.at(i3)};

    auto const e1{v2.position - v1.position};
    auto const e2{v3.position - v1.position};
    auto const delta1{v2.texCoord - v1.texCoord};
    auto const delta2{v3.texCoord - v1.texCoord};

    glm::mat2 M;
    M[0][0] = delta2.t;
    M[0][1] = -delta1.t;
    M[1][0] = -delta2.s;
    M[1][1] = delta1.s;
    M *= (1.0f / (delta1.s * delta2.t - delta2.s * delta1.t));

    auto const tangent{glm::vec4(M[0][0] * e1.x + M[0][1] * e2.x,
                                 M[0][0] * e1.y + M[0][1] * e2.y,
                                 M[0][0] * e1.z + M[0][1] * e2.z, 0.0f)};

    auto const bitangent{glm::vec3(M[1][0] * e1.x + M[1][1] * e2.x,
                                   M[1][0] * e1.y + M[1][1] * e2.y,
                                   M[1][0] * e1.z + M[1][1] * e2.z)};

    v1.tangent += tangent;
    v2.tangent += tangent;
    v3.tangent += tangent;

    bitangents.at(i1) += bitangent;
    bitangents.at(i2) += bitangent;
    bitangents.at(i3) += bitangent;
  }

  for (auto &&[i, vertex] : iter::enumerate(vertices)) {
    auto const &n{vertex.normal};
    auto const &t{glm::vec3(vertex.tangent)};

    auto const tangent{t - n * glm::dot(n, t)};
    vertex.tangent = glm::vec4(glm::normalize(tangent), 0);

    auto const b{glm::cross(n, t)};
    auto const handedness{glm::dot(b, bitangents.at(i))};
    vertex.tangent.w = (handedness < 0.0f) ? -1.0f : 1.0f;
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
