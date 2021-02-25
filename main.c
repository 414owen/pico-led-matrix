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
  printf("tick\n");
  swap_buffers();
  while (true) {
    sleep_ms(100);
    handle_quit_input(getchar_timeout_us(100));
  }
}

void countdown(void) {
  for (int i = 0; i < 30; i++) {
    sleep_ms(100);
    printf("%d\n", i);
  }
}

int main(void) {
  stdio_init_all();
  countdown();
  init_matrix();
  led_pattern();
  return 0;
}
