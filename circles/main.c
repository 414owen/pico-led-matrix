#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"

#include "bootsel.h"
#include "brightness.h"
#include "char.h"
#include "num.h"
#include "matrix.h"

#define MAX_FRAME 100

int max_distance;
float tau = 2 * (float) M_PI;

point start = { 0, 0 };
point max_point = { .x = MATRIX_WIDTH - 1, .y = MATRIX_HEIGHT - 1 };

void loop(void) {
  for (int frame = 0; true; frame = (frame + 1) % MAX_FRAME) {
    float frame_frac = (float) -frame / MAX_FRAME;
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
      for (int j = 0; j < MATRIX_WIDTH; j++) {
        point here = { j, i };
        float dist = 15 * distance(start, here);
        float brightness = sin(dist / max_distance + tau * frame_frac);
        write_pixel(j, i, MAX_BRIGHTNESS * brightness);
      }
    }
    swap_buffers();
    sleep_ms(5);
    handle_quit_input(getchar_timeout_us(100));
  }
}

int main(void) {
  max_distance = distance(start, max_point);
  stdio_init_all();
  init_matrix();
  loop();
  return 0;
}
