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

float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

bool sees(const Unit *src, const Unit *dest, const float distance, const float c) {
  float dp_x = dest->pos_x - src->pos_x;
  float dp_y = dest->pos_y - src->pos_y;
  float dpl = 1/Q_rsqrt(dp_x * dp_x + dp_y * dp_y);
  return dpl <= distance && (src->dir_x * dp_x + src->dir_y * dp_y) >= c;
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
  #pragma omp parallel for
  for (int i = 0; i < us; i++) {
    for (int j = 0; j < us; j++) {
      if (sees(&uns[i], &uns[j], d, a)) {
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
