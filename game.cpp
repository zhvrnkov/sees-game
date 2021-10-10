#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include "unit.h"
#include "parser.h"
#include "renderer/renderer.h"

using namespace glm;
using namespace std;

Unit *units;
RenderingContext context;
Renderer unitsRenderer;

void scrollCallback(double xoffset, double yoffset) {
  unitsRenderer.zoomScale += (float)yoffset/10.0f;
  if (unitsRenderer.zoomScale < 0.1)
    unitsRenderer.zoomScale = 0.1;
}

static int countVisibleUnits(short *counter) {
  int count = 0;
  for (int i = 0; i < UNITS_COUNT; i++)
    count += (int)counter[i];
  return count;
}

static void mouseButtonCallbackVec(vec2 click) {
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
      cout << "It sees " << countVisibleUnits(units[i].isSeeUnitIndices) << " units." << endl;
    }
  }
}

void mouseButtonCallback(double x, double y) {
  vec2 click = vec2(x, y) / unitsRenderer.zoomScale + context.currentCameraPos;
  mouseButtonCallbackVec(click);
}

static int initialUnitIndex = 0;
void keyCallback(int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    cout << "click" << endl;
    mouseButtonCallbackVec(units[++initialUnitIndex].pos);
  }
}

int main() {
  units = (Unit *)malloc(sizeof(Unit) * UNITS_COUNT);
  parse_units(units, VISIBLE_DISTANCE, "units.csv", UNITS_COUNT);

#pragma omp parallel for
  for (int i = 0; i < UNITS_COUNT; i++) {
    for (int j = i; j < UNITS_COUNT; j++) {
      SeesResult result = sees(&units[i], &units[j], VISIBLE_COEF, VISIBLE_DISTANCE);
      if (result.isSrcSeesDest) {
        units[i].isSeeUnitIndices[j] = 1;
      }
      if (result.isDestSeesSrc) {
        units[j].isSeeUnitIndices[i] = 1;
      }
    }
  }

  setupWindow();
  unitsRenderer = makeRenderer();
  unitsRenderer.context = &context;
  windowPresenter.mouseButtonCallback = mouseButtonCallback;
  windowPresenter.scrollCallback = scrollCallback;
  windowPresenter.keyCallback = keyCallback;

  context.units = units;
  context.units_count = UNITS_COUNT;
  context.currentCameraPos = vec2(0.0);
  context.targetCameraPos = context.currentCameraPos;
  context.visibleDistance = VISIBLE_DISTANCE;
  context.visibleSectorAngle = VISIBLE_ANGLE;

  mouseButtonCallbackVec(units[initialUnitIndex].pos);
  for (size_t i = 0; should_close(); i++) {
    if (context.currentCameraPos != context.targetCameraPos) {
      vec2 diff = (context.targetCameraPos - context.currentCameraPos) / 10.0f;
      context.currentCameraPos += diff;
    }
    render(&unitsRenderer);
  }

  glfwTerminate();
  return 0;
}
