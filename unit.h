#ifndef unit_h
#define unit_h

#include <glm/glm.hpp>

#define UNITS_COUNT 10000

using namespace glm;
using namespace std;

typedef struct {
  vec2 pos;
  vec2 dir;
  short counter[UNITS_COUNT];
  bool isSelected;
} Unit;

void print_unit(Unit unit) {
  cout << unit.pos.x << " " << unit.pos.y << endl;
  cout << unit.dir.x << " " << unit.dir.y << endl << endl;
}

#endif
