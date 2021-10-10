#version 330 core

uniform float zoomScale;
uniform float defaultScale;

in vec2 fragPos;
flat in vec2 unitCenter;

out vec4 FragColor;

void main() {
  if (length(fragPos - unitCenter) > (zoomScale * defaultScale) * 2) {
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }
  else {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  } 
}
