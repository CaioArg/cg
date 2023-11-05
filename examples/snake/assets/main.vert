#version 410

layout(location = 0) in vec3 inPosition;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 fragColor;

void main() {
  fragColor = color;
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1);
}
