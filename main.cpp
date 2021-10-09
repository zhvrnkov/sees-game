#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <chrono>

using namespace glm;
using namespace std;

typedef struct {
  float pos_x;
  float pos_y;
  float dir_x;
  float dir_y;
  int counter;
} Unit;

float d = 20.0f;

Unit units[] = {
  { .pos_x = 1, .pos_y = 1, .dir_x = 0 * d, .dir_y = 1 * d },
  { .pos_x = 1, .pos_y = 2, .dir_x = 1  * d, .dir_y = 0 * d },
  { .pos_x = -5, .pos_y = -1, .dir_x = 0.7071067f * d, .dir_y = 0.7071067f * d },
};

bool sees(const Unit *src, const Unit *dest, const float c, const float d) {
  float dp_x = dest->pos_x - src->pos_x;
  float dp_y = dest->pos_y - src->pos_y;
  float dott = (src->dir_x * dp_x + src->dir_y * dp_y);
  return dott < d && dott >= c;
}


int main() {
  float a = cos(135 / 2 * (M_PI / 180));
  float c = d * a;
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

  float ds = d * d;
  auto t1 = high_resolution_clock::now();
  #pragma omp parallel for
  for (int i = 0; i < us; i++) {
    for (int j = 0; j < us; j++) {
      if (sees(&uns[i], &uns[j], c, ds)) {
        uns[i].counter += 1;
      }
    }
  }
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << ms_double.count() << "ms" << endl;
  int counter = 0;
  for (int i = 0; i < us; i++) {
    counter += uns[i].counter;
  }
  std::cout << counter << endl;
}
