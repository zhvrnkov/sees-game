#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <chrono>
#include "unit.h"
#include "parser.h"
#include "renderer/renderer.h"

#define VISIBLE_ANGLE 135.0f
#define VISIBLE_DISTANCE 1.0f

float VISIBLE_COEF = cos(VISIBLE_ANGLE / 2 * (M_PI / 180)) * VISIBLE_DISTANCE;
float VISIBLE_DISTANCE_SQUARE = VISIBLE_DISTANCE * VISIBLE_DISTANCE;

using namespace glm;
using namespace std;

Unit *units;
RenderingContext context;
Renderer unitsRenderer;

typedef struct {
  bool val;
  bool yal;
} S;

S sees(const Unit *src, const Unit *dest, const float c, const float d) {
  vec2 destPosFromSrcPos = dest->pos - src->pos;
  float l = length(destPosFromSrcPos);
  vec2 normDestPosFromSrcPos = destPosFromSrcPos / l;
  float dott = dot(src->dir, normDestPosFromSrcPos);
  float dottt = dot(dest->dir, -normDestPosFromSrcPos);                          
  return {
    .val = l < d && dott > c,
    .yal = l < d && dottt > c
  };
}

void scrollCallback(double xoffset, double yoffset) {
  unitsRenderer.zoomScale += (float)yoffset/10.0f;
  if (unitsRenderer.zoomScale < 0.1)
    unitsRenderer.zoomScale = 0.1;
}

int countVisibleUnits(short *counter) {
  int count = 0;
  for (int i = 0; i < UNITS_COUNT; i++)
    count += (int)counter[i];
  return count;
}

void mouseButtonCallbackVec(vec2 click) {
  bool foundUnitOnClick = false;
  bool isSelected;
  for (int i = 0; i < UNITS_COUNT; i++) {
    if (!foundUnitOnClick) {
      isSelected = length(click - units[i].pos) < 0.025f;
      foundUnitOnClick = isSelected;
      units[i].isSelected = isSelected;
    }
    else {
      units[i].isSelected = false;
    }
    if (units[i].isSelected) {
      context.targetCameraPos = units[i].pos;
      cout << "Clicked on unit at " << i << " index. ";
      cout << "It sees " << countVisibleUnits(units[i].counter) << " units." << endl;
    }
  }
}

void mouseButtonCallback(double x, double y) {
  vec2 click = vec2(x, y) / unitsRenderer.zoomScale + context.currentCameraPos;
  mouseButtonCallbackVec(click);
}

int main() {
  units = (Unit *)malloc(sizeof(Unit) * UNITS_COUNT);
  parse_units(units, VISIBLE_DISTANCE, "units.csv", UNITS_COUNT);

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  auto t1 = high_resolution_clock::now();
#pragma omp parallel for
  for (int i = 0; i < UNITS_COUNT; i++) {
    for (int j = i; j < UNITS_COUNT; j++) {
      S x = sees(&units[i], &units[j], VISIBLE_COEF, VISIBLE_DISTANCE);
      if (x.val) {
        units[i].counter[j] = 1;
      }
      if (x.yal) {
        units[j].counter[i] = 1;
      }
    }
  }
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << ms_double.count() << "ms" << endl;
  int counter = 0;
  for (int i = 0; i < UNITS_COUNT; i++) {
    for (int j = 0; j < UNITS_COUNT; j++) {
      counter += units[i].counter[j];
    }
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
  context.visibleDistance = VISIBLE_DISTANCE;
  context.visibleSectorAngle = VISIBLE_ANGLE;

  mouseButtonCallbackVec(units[0].pos);
  for (size_t i = 0; should_close(); i++) {
    if (context.currentCameraPos != context.targetCameraPos) {
      vec2 diff = (context.targetCameraPos - context.currentCameraPos) / 10.0f;
      context.currentCameraPos += diff;
    }
    render(&unitsRenderer);
  }

  glfwTerminate();
}
