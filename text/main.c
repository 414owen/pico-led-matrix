#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "bootsel.h"
#include "char.h"
#include "matrix.h"

void led_pattern(void) {
  char *string = "Hello World!   ";
  while (true) {
    for (char *c = string; *c != 0; c++) {
      display_char(*c);
      sleep_ms(300);
      handle_quit_input(getchar_timeout_us(1));
    }
  }
}

int main(void) {
  stdio_init_all();
  init_matrix();
  led_pattern();
  return 0;
}
