#ifndef math_h_INCLUDED
#define math_h_INCLUDED

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct point {
  int x;
  int y;
} point;

float square(float a);
float distance(point a, point b);

#endif // ~/src/pico/lib/math_h_INCLUDED
