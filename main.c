#include <stdio.h>
#include <stdlib.h>
#include "pico/bootrom.h"
#include "pico/stdlib.h"

#include "matrix.h"

extern const int matrix_height;
extern const int matrix_width;

void reboot(void) {
  reset_usb_boot(25, 0);
}

void handle_quit_input(char c) {
  if (c == 'r') reboot();
}

void led_pattern(void) {
  while (true) {
    for (int i = 0; i < matrix_width; i++) {
      // write_pixel(i, i, 1);
    }
    sleep_ms(1000);
    swap_buffers();
  }
}

void countdown(void) {
  for (int i = 0; i < 10; i++) {
    sleep_ms(1000);
    printf("%d\n", i);
  }
}

int main(void) {
  stdio_init_all();
  printf("main\n");
  countdown();
  init_matrix();
  led_pattern();
  return 0;
}
