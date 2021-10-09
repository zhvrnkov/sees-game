#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 model;
uniform float zoomScale;
uniform float defaultScale;

out vec2 fragPos;

void main() {
  fragPos = aPos * defaultScale * zoomScale + (model * zoomScale);
  gl_Position = vec4(fragPos, 0, 1);
}
