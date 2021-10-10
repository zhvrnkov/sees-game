#version 330 core

uniform vec2 model;
uniform float zoomScale;
uniform float defaultScale;
uniform bool isSelected;

out vec4 FragColor;

in vec2 fragPos;
flat in vec2 unitCenter;

void main() {
  if (length(fragPos - unitCenter) > (zoomScale * defaultScale)) {
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }
  else {
    FragColor = isSelected ? vec4(0.0, 1.0, 0.0, 1.0) : vec4(1.0, 0.0, 0.0, 1.0);
  } 
}
