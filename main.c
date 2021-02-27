#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "bootsel.h"
#include "char.h"
#include "countdown.h"
#include "matrix.h"

extern const int matrix_height;
extern const int matrix_width;

void led_pattern(void) {
  char *string = "SEND.NUDES...";
  while (true) {
    for (char *c = string; *c != 0; c++) {
      display_char(*c);
      sleep_ms(300);
      handle_quit_input(getchar_timeout_us(100));
    }
  }
}

int main(void) {
  stdio_init_all();
  countdown();
  init_matrix();
  led_pattern();
  return 0;
}
