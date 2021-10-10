#ifndef unit_h
#define unit_h

#include <glm/glm.hpp>

#define UNITS_COUNT 10000
#define VISIBLE_ANGLE 135.0f
#define VISIBLE_DISTANCE 1.0f

float VISIBLE_COEF = cos(VISIBLE_ANGLE / 2 * (M_PI / 180)) * VISIBLE_DISTANCE;
float VISIBLE_DISTANCE_SQUARE = VISIBLE_DISTANCE * VISIBLE_DISTANCE;

using namespace glm;
using namespace std;

typedef struct {
  vec2 pos;
  vec2 dir;
  short isSeeUnitIndices[UNITS_COUNT];
  bool isSelected;
  int seesCount;
} Unit;

void print_unit(Unit unit) {
  cout << unit.pos.x << " " << unit.pos.y << endl;
  cout << unit.dir.x << " " << unit.dir.y << endl << endl;
}

typedef struct {
  bool isSrcSeesDest;
  bool isDestSeesSrc;
} SeesResult;

SeesResult sees(const Unit *src, const Unit *dest, const float angleCoef, const float distance) {
  vec2 destPosFromSrcPos = dest->pos - src->pos;
  float srcToDestDistance = length(destPosFromSrcPos);
  vec2 normDestPosFromSrcPos = destPosFromSrcPos / srcToDestDistance;
  return SeesResult {
    .isSrcSeesDest = srcToDestDistance < distance && dot(src->dir, normDestPosFromSrcPos) > angleCoef,
    .isDestSeesSrc = srcToDestDistance < distance && dot(dest->dir, -normDestPosFromSrcPos) > angleCoef
  };
}

bool fastSees(const Unit *src, const Unit *dest, const float angleCoef, const float distance) {
  vec2 destPosFromSrcPos = dest->pos - src->pos;
  float srcToDestDistance = length(destPosFromSrcPos);
  return srcToDestDistance < distance && dot(src->dir, destPosFromSrcPos / srcToDestDistance) > angleCoef;
}

#endif
