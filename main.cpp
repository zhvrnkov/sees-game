#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <chrono>
#include "unit.h"
#include "parser.h"
#include "renderer/renderer.h"

#define UNITS_COUNT 10000
#define VISIBLE_ANGLE 135.0f
#define VISIBLE_DISTANCE 1.0f

float VISIBLE_COEF = cos(VISIBLE_ANGLE / 2 * (M_PI / 180)) * VISIBLE_DISTANCE;
float VISIBLE_DISTANCE_SQUARE = VISIBLE_DISTANCE * VISIBLE_DISTANCE;

using namespace glm;
using namespace std;

Unit units[UNITS_COUNT];
RenderingContext context;
Renderer unitsRenderer;

bool sees(const Unit *src, const Unit *dest, const float c, const float d) {
  float dott = dot(src->dir, dest->pos - src->pos);
  return dott < d && dott >= c;
}

void scrollCallback(double xoffset, double yoffset) {
  unitsRenderer.zoomScale += (float)yoffset/10.0f;
  if (unitsRenderer.zoomScale < 0.1)
    unitsRenderer.zoomScale = 0.1;
}

void mouseButtonCallback(double x, double y) {
  vec2 click = vec2(x, y) / unitsRenderer.zoomScale + context.currentCameraPos;
  for (int i = 0; i < UNITS_COUNT; i++) {
    units[i].isSelected = length(click - units[i].pos) < 0.025f;
    if (units[i].isSelected) {
      context.targetCameraPos = units[i].pos;
    }
  }
}

int main() {
  parse_units(units, VISIBLE_DISTANCE, "units.csv");

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  auto t1 = high_resolution_clock::now();
  #pragma omp parallel for
  for (int i = 0; i < UNITS_COUNT; i++) {
    for (int j = 0; j < UNITS_COUNT; j++) {
      if (sees(&units[i], &units[j], VISIBLE_COEF, VISIBLE_DISTANCE_SQUARE)) {
        units[i].counter += 1;
      }
    }
  }
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << ms_double.count() << "ms" << endl;
  int counter = 0;
  for (int i = 0; i < UNITS_COUNT; i++) {
    counter += units[i].counter;
  }
  std::cout << counter << endl;

  setupWindow();
  unitsRenderer = makeRenderer();
  unitsRenderer.context = &context;
  windowPresenter.mouseButtonCallback = mouseButtonCallback;
  windowPresenter.scrollCallback = scrollCallback;

  context.units = units;
  context.units_count = UNITS_COUNT;
  context.currentCameraPos = vec2(0.0);
  context.targetCameraPos = context.currentCameraPos;
  context.visibleDistance = 0.025f * VISIBLE_DISTANCE;
  context.visibleSectorAngle = VISIBLE_ANGLE;

  for (size_t i = 0; should_close(); i++) {
    if (context.currentCameraPos != context.targetCameraPos) {
      vec2 diff = (context.targetCameraPos - context.currentCameraPos) / 10.0f;
      context.currentCameraPos += diff;
    }
    render(&unitsRenderer);
  }

	glfwTerminate();
}
