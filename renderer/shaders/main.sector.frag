#version 330 core

uniform float zoomScale;
uniform float defaultScale;
uniform float visibleSectorAngle;

in vec2 fragPos;
flat in vec2 unitCenter;
flat in float radius;

out vec4 FragColor;

void main() {
  vec2 point = fragPos - unitCenter;
  float pi = 3.1415926535897;
  float arc = visibleSectorAngle * pi * radius / 180.0;
  vec2 t = normalize(point);
  float pointArc = radius * acos(t.x);
  if (length(point) > (defaultScale * zoomScale * 2.0)) {
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }
  else if (t.y > 0 && pointArc < arc) {
    FragColor = vec4(1.0, 1.0, 1.0, 0.5);
  }
  else {
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }
}
