#version 330 core

uniform float zoomScale;
uniform float defaultScale;
uniform bool isSelected;
uniform bool isHighlighted;

out vec4 FragColor;

in vec2 fragPos;
flat in vec2 unitCenter;

void main() {
  if (length(fragPos - unitCenter) > (zoomScale * defaultScale)) {
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }
  else if (isSelected) {
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
  } 
  else if (isHighlighted) {
    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
  }
  else {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  }
}
