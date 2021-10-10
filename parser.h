#ifndef parser_h
#define parser_h

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "unit.h"

using namespace std;

void parse_units(Unit *units, float distance, string path, size_t units_count) {
  ifstream file;
  file.open(path, ios::in);
  string line;
  float floats[4];
  int units_index = 0;
  while (units_count > 0) {
    units_count -= 1;
    for (int i = 0; i < 4; i++) {
      getline(file, line);
      istringstream in(line);
      in >> floats[i];
    }
    getline(file, line); // skip counter value
    units[units_index].pos = vec2(floats[0], floats[1]);
    units[units_index].dir = vec2(floats[2], floats[3]) * distance;
    units[units_index].seesCount = 0;
    for (int i = 0; i < 10000; i++)
      units[units_index].isSeeUnitIndices[i] = 0;
    units_index += 1;
  }
  file.close();
}

void write_units(const Unit *units, string path, size_t units_count) {
  ofstream file;
  file.open(path, ios::out);
  for (size_t i = 0; i < units_count; i++) {
    file << units[i].pos.x << endl;
    file << units[i].pos.y << endl;
    file << units[i].dir.x << endl;
    file << units[i].dir.y << endl;
    file << units[i].seesCount << endl;
  }
  file.close();
}

void write_units_description(const Unit *units, string path, size_t units_count) {
  ofstream file;
  file.open(path, ios::out);
  for (size_t i = 0; i < units_count; i++) {
    file << "Unit " << i << " sees " << units[i].seesCount << " units." << endl;
  }
  file.close();
}

#endif
