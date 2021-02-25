#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "brightness.h"

extern const int MAX_BRIGHTNESS;

static int positives[] = { 14, 15, 13, 16, 17 };
static int negatives[] = { 18, 19, 20, 21, 22, 26, 27, 28};

enum {
  positive_num = sizeof(positives) / sizeof(positives[0]),
  negative_num = sizeof(negatives) / sizeof(negatives[0])
};

int matrix_width = positive_num;
int matrix_height = negative_num;

typedef uint8_t matrix[negative_num][positive_num];

static matrix buffer_a = {0};
static matrix buffer_b = {0};
static uint8_t (*buffers[])[][positive_num] = { &buffer_a, &buffer_b };

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

void init_pins(int *pins, int pin_num) {
  for (int i = 0; i < pin_num; i++) {
    gpio_init(pins[i]);
    gpio_set_dir(pins[i], GPIO_OUT);
  }
}

void init_all_pins(void) {
  init_pins(positives, positive_num);
  init_pins(negatives, negative_num);
}

void display_loop(void) {
  for (int tick = 0; true; tick++, tick %= MAX_BRIGHTNESS) {
    const matrix *buffer = get_active_buffer();
    for (int i = 0; i < matrix_width; i++) {
      gpio_put(positives[i], 1);
      for (int j = 0; j < matrix_height; j++) {
        gpio_put(negatives[j], !is_led_on((*buffer)[j][i], tick));
      }
      sleep_us(200);
      gpio_put(positives[i], 0);
    }
  }
}

void zero(void) {
  for (int k = 0; k < 2; k++) {
    matrix *buffer = buffers[k];
    printf("zeroing buffer %d\n", k);
    for (int i = 0; i < matrix_height; i++) {
      for (int j = 0; j < matrix_width; j++) {
        (*buffer)[j][i] = 0;
      }
    }
  }
}

void init_matrix(void) {
  printf("matrix size: %dx%d\n", positive_num, negative_num);
  printf("buffer dimensions: %d\n", sizeof(buffer_a[0]));
  zero();
  printf("initializing gpio\n");
  init_all_pins();
  printf("launching display thread\n");
  multicore_launch_core1(display_loop);
}
