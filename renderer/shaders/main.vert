#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 model;

void main() {
  gl_Position = vec4(aPos / 100 + model, 0, 1);
}
