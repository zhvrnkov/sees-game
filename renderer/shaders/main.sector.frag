#version 330 core

uniform float zoomScale;
uniform float defaultScale;
uniform float visibleSectorAngle;
uniform vec2 direction;

in vec2 fragPos;
flat in vec2 unitCenter;
flat in float radius;

out vec4 FragColor;

void main() {
  vec2 point = fragPos - unitCenter;
  float pi = 3.1415926535897;
  vec2 normPoint = normalize(point);
  vec2 normDir = normalize(direction);
  // idk why 360.0f and not 180.0f, but it works
  float visibleSectorAngleRad = visibleSectorAngle * pi/360.0f;
  if (length(point) > (defaultScale * zoomScale)) {
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }
  else if (dot(normPoint, normDir) > cos(visibleSectorAngleRad)) {
    FragColor = vec4(1.0, 1.0, 1.0, 0.5);
  }
  else {
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }
}
