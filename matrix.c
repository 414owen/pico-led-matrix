#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "brightness.h"

extern const int MAX_BRIGHTNESS;

static int positives[] = { 13, 14, 15, 16, 17 };
static int negatives[] = { 18, 19, 20, 21, 22, 26, 27, 28};

enum {
  positive_num = sizeof(positives) / sizeof(positives[0]),
  negative_num = sizeof(negatives) / sizeof(negatives[0])
};

typedef uint8_t *matrix[negative_num];

int matrix_width = positive_num;
int matrix_height = negative_num;

static const matrix buffer_a = {0};
static const matrix buffer_b = {0};
static const matrix *buffers[] = { &buffer_a, &buffer_b };

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

const matrix *get_inactive_buffer(void) {
  return buffers[next_buffer_ind()];
}

void write_pixel(int x, int y, uint8_t val) {
  const matrix *buffer = get_inactive_buffer();
  *buffer[x][y] = val;
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
  stdio_init_all();
}

void display_loop(void) {
  for (int tick = 0; tick < MAX_BRIGHTNESS; tick++) {
    const matrix *buffer = get_active_buffer();
    for (int i = 0; i < matrix_height; i++) {
      gpio_put(positives[i], 1);
      for (int j = 0; j < matrix_width; j++) {
        gpio_put(negatives[j], !is_led_on(*buffer[i][j], tick));
      }
      sleep_ms(1);
      gpio_put(positives[i], 0);
    }
  }
}



void init_matrix(void) {
  printf("here\n");
  init_all_pins();
  multicore_launch_core1(display_loop);
}
