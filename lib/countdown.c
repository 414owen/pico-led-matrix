#include <stdio.h>
#include "pico/stdlib.h"

#include "bootsel.h"

void countdown(void) {
  for (int i = 0; i < 30; i++) {
    sleep_ms(100);
    handle_quit_input(getchar_timeout_us(100));
    printf("%d\n", i);
  }
}
