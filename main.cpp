#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <chrono>

using namespace glm;
using namespace std;

typedef struct {
  vec2 position;
  vec2 direction;
} Unit;

Unit units[] = {
  { .position = vec2(1,1), .direction = vec2(0,1) },
  { .position = vec2(1,2), .direction = vec2(1,0) },
  { .position = vec2(-5,-1), .direction = vec2(0.7071067, 0.7071067) },
};

bool sees(Unit src, Unit dest, float distance, float c) {
  vec2 dp = dest.position - src.position;
  float dpl = length(dp);
  return dpl <= distance && dot(src.direction, dp / dpl) >= c;
}


int main() {
  float a = cos(135 / 2 * (M_PI / 180));
  float d = 20.0;
  size_t s = sizeof(units) / sizeof(Unit);
  size_t us = 10000;
  Unit uns[us];
  for (int i = 0; i < us; i++) {
    uns[i] = units[i % s];
  }

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  auto t1 = high_resolution_clock::now();
  for (int i = 0; i < us; i++) {
    for (int j = 0; j < us; j++) {
      if (i == j) {
        continue;
      }
      if (sees(uns[i], uns[j], d, a)) {
        // cout << i << " sees " << j << endl;
      }
    }
  }
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << ms_double.count() << "ms";
}
