#version 330 core

uniform vec2 model;
uniform float zoomScale;
uniform float defaultScale;

out vec4 FragColor;

in vec2 fragPos;

void main() {
  if (length(fragPos - model*zoomScale) > (zoomScale * defaultScale)) {
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }
  else {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  }
}
