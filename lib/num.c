#include <math.h>

#include "num.h"

float square(float a) {
  return a * a;
}

float distance(point a, point b) {
  return sqrt(square(a.x - b.x) + square(a.y - b.y));
}
