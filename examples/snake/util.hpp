#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "abcgOpenGL.hpp"

std::tuple<std::vector<glm::vec3>, std::vector<GLuint>> loadModelFromFile(std::string_view path);

#endif
