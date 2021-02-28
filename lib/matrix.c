#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "brightness.h"
#include "matrix.h"

// static int positives[] = { 18, 21, 26, 27, 28 };
const  int positives[] = { 28, 27, 26, 21, 18 };
const  int negatives[] = { 2, 3, 11, 5, 7, 9, 13, 14 };

typedef uint8_t matrix[NEGATIVE_NUM][POSITIVE_NUM];

static matrix buffer_a = {0};
static matrix buffer_b = {0};
static uint8_t (*buffers[])[][POSITIVE_NUM] = { &buffer_a, &buffer_b };

static int buffer_ind = 0;

int next_buffer_ind(void) {
  return (buffer_ind + 1) % 2;
}

const matrix *get_active_buffer(void) {
  return buffers[buffer_ind];
}

void swap_buffers() {
  buffer_ind = next_buffer_ind();
}

matrix *get_inactive_buffer(void) {
  return buffers[next_buffer_ind()];
}

void write_pixel(int x, int y, uint8_t val) {
  matrix *buffer = get_inactive_buffer();
  (*buffer)[y][x] = val;
}

void init_pins(const int *pins, int pin_num) {
  for (int i = 0; i < pin_num; i++) {
    gpio_init(pins[i]);
    gpio_set_dir(pins[i], GPIO_OUT);
  }
}

void init_all_pins(void) {
  init_pins(positives, POSITIVE_NUM);
  init_pins(negatives, NEGATIVE_NUM);
}

void display_loop(void) {
  for (int tick = 0; true; tick++, tick %= MAX_BRIGHTNESS) {
    const matrix *buffer = get_active_buffer();
    for (int i = 0; i < MATRIX_WIDTH; i++) {
      gpio_put(positives[i], 1);
      for (int j = 0; j < MATRIX_HEIGHT; j++) {
        gpio_put(negatives[j], !is_led_on((*buffer)[j][i], tick));
      }
      sleep_us(1000);
      gpio_put(positives[i], 0);
    }
  }
}

void zero(void) {
  for (int k = 0; k < 2; k++) {
    matrix *buffer = buffers[k];
    printf("zeroing buffer %d\n", k);
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
      for (int j = 0; j < MATRIX_WIDTH; j++) {
        (*buffer)[i][j] = 0;
      }
    }
  }
}

void init_matrix(void) {
  printf("matrix size: %dx%d\n", POSITIVE_NUM, NEGATIVE_NUM);
  printf("buffer dimensions: %d\n", sizeof(buffer_a[0]));
  zero();
  printf("initializing gpio\n");
  init_all_pins();
  printf("launching display thread\n");
  multicore_launch_core1(display_loop);
}
