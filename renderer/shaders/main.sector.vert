#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 model;
uniform float zoomScale;
uniform float defaultScale;
uniform vec2 cameraPos;

void main() {
  vec2 unitCenter = ((model - cameraPos) * zoomScale);
  vec2 fragPos = (aPos * defaultScale * zoomScale) + unitCenter;
  
  gl_Position = vec4(fragPos, 0, 1);
}

