#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <chrono>

using namespace glm;
using namespace std;

typedef struct {
  vec2 pos;
  vec2 dir;
  short counter[10000] = { 0 };
} Unit;

float d = 20.0f;

Unit units[] = {
  { .pos = vec2(1, 1), .dir = vec2(0 * d, 1 * d) },
  { .pos = vec2(1, 2), .dir = vec2(1  * d, 0 * d) },
  { .pos = vec2(-5, -1), .dir = vec2(0.7071067f * d, 0.7071067f * d) },
};

typedef struct {
  bool val;
  bool yal;
} S;

S sees(const Unit *src, const Unit *dest, const float c, const float d) {
  float dott = dot(src->dir, dest->pos - src->pos);
  float dotv = dot(dest->dir, src->pos - dest->pos);
  return {
    .val = dott < d && dott >= c,
    .yal = dotv < d && dotv >= c,
  };
}

int main() {
  float a = cos(135 / 2 * (M_PI / 180));
  float c = d * a;
  size_t s = sizeof(units) / sizeof(Unit);
  size_t us = 10000;
  Unit *uns = (Unit *)malloc(sizeof(Unit) * us);
  for (int i = 0; i < us; i++) {
    uns[i] = units[i % s];
  }

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  float ds = d * d;
  auto t1 = high_resolution_clock::now();
  #pragma omp parallel for
  for (int i = 0; i < us; i++) {
    for (int j = i; j < us; j++) {
      S x = sees(&uns[i], &uns[j], c, ds);
      if (x.val) {
        uns[i].counter[j] = 1;
      }
      if (x.yal) {
        uns[j].counter[i] = 1;
      }
    }
  }
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << ms_double.count() << "ms" << endl;
  int counter = 0;
  for (int i = 0; i < us; i++) {
    for (int j = 0; j < us; j++) {
      counter += uns[i].counter[j];
    }
  }
  std::cout << counter << endl;
}
