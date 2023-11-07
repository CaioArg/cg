#version 410

layout(location = 0) in vec3 inPosition;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 fragColor;

void main() {
  vec4 posWorldSpace = modelMatrix * vec4(inPosition, 1);

  float i;

  if (posWorldSpace.y <= 0.0) {
    i = 0.75;
  } if (posWorldSpace.y >= 1.0) {
    i = 1.0;
  } else {
    i = 0.75 + posWorldSpace.y / 4.0;
  }

  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projectionMatrix * viewMatrix * posWorldSpace;
}
