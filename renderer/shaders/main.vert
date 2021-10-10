#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 model;
uniform float zoomScale;
uniform float defaultScale;
uniform vec2 cameraPos;

out vec2 fragPos;
flat out vec2 unitCenter;

void main() {
  unitCenter = ((model - cameraPos) * zoomScale);
  fragPos = (aPos * defaultScale * zoomScale) + unitCenter;
  
  gl_Position = vec4(fragPos, 0, 1);
}
