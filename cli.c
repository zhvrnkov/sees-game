#include <iostream>
#include <chrono>
#include "unit.h"
#include "parser.h"

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main() {
  Unit *units = (Unit *)malloc(sizeof(Unit) * UNITS_COUNT);
  parse_units(units, VISIBLE_DISTANCE, "units.csv", UNITS_COUNT);

  auto t1 = high_resolution_clock::now();
  #pragma omp parallel for
  for (int i = 0; i < UNITS_COUNT; i++) {
    for (int j = 0; j < UNITS_COUNT; j++) {
      if (fastSees(&units[i], &units[j], VISIBLE_COEF, VISIBLE_DISTANCE)) {
        units[i].seesCount += 1;
      }
    }
  }
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "Computing unit's sees status took: " <<  ms_double.count() << "ms" << endl;
  int counter = 0;
  for (int i = 0; i < UNITS_COUNT; i++) {
      counter += units[i].seesCount;
  }
  std::cout << counter << endl;
  return 0;
}
