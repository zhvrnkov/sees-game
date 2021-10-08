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

Unit units[] = {
  { .pos_x = 1, .pos_y = 1, .dir_x = 0, .dir_y = 1 },
  { .pos_x = 1, .pos_y = 2, .dir_x = 1, .dir_y = 0 },
  { .pos_x = -5, .pos_y = -1, .dir_x = 0.7071067, .dir_y = 0.7071067 },
};

typedef struct {
  bool val;
  bool yal;
} S;

S sees(const Unit *src, const Unit *dest, const float distance, const float c) {
  float dp_x = dest->pos_x - src->pos_x;
  float dp_y = dest->pos_y - src->pos_y;
  float sp_x = src->pos_x - dest->pos_x;
  float sp_y = src->pos_y - dest->pos_y;;
  float dpl = sqrt(dp_x * dp_x + dp_y * dp_y);
  return { 
    .val = dpl <= distance && (src->dir_x * dp_x + src->dir_y * dp_y) >= c,
    .yal = dpl <= distance && (dest->dir_x * sp_x + dest->dir_y * sp_y) >= c,
  };
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
//  #pragma omp parallel for
  for (int i = 0; i < us; i++) {
    for (int j = i; j < us; j++) {
      S x = sees(&uns[i], &uns[j], d, a);
      if (x.val) {
        uns[i].counter += 1;
      }
      if (x.yal) {
        uns[j].counter += 1;
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
