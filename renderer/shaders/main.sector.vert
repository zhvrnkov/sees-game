#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 model;
uniform float zoomScale;
uniform float defaultScale;
uniform vec2 cameraPos;
uniform float visibleDistance;
uniform float visibleSectorAngle;

out vec2 fragPos;
flat out vec2 unitCenter;
flat out float radius;

float sign(float x) {
  if (x > 0) 
    return 1.0;
  else 
    return -1.0;
}

void main() {
  unitCenter = ((model - cameraPos) * zoomScale);
  radius = (defaultScale * zoomScale * 2.0);
  fragPos = (aPos * radius) + unitCenter;
  
  gl_Position = vec4(fragPos, 0, 1);
}

