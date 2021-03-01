#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"

#include "bootsel.h"
#include "brightness.h"
#include "char.h"
#include "num.h"
#include "matrix.h"

int max_distance;

point start = { 0, 0 };
point max_point = { .x = MATRIX_WIDTH - 1, .y = MATRIX_HEIGHT - 1 };

void draw_gradient(void) {
  for (int i = 0; i < MATRIX_HEIGHT; i++) {
    for (int j = 0; j < MATRIX_WIDTH; j++) {
      point here = { .x = j, .y = i };
      float dist = distance(start, here);
      write_pixel(j, i, MAX_BRIGHTNESS * dist / max_distance);
    }
  }
  swap_buffers();
}

int main(void) {
  max_distance = distance(start, max_point);
  stdio_init_all();
  init_matrix();
  draw_gradient();
  while (true) {
    handle_quit_input(getchar_timeout_us(100000));
  }
  return 0;
}
