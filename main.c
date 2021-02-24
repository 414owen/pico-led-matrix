#include <stdio.h>
#include <stdlib.h>
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "brightness.h"

#define ROWS 2
#define COLS 2

extern const uint16_t brightnesses[];
extern const int MAX_BRIGHTNESS;
extern const int positives[];
extern const int negatives[];
extern const int positive_num;
extern const int negative_num;

const int BUFSIZE = 128;
const int zero = 0;

uint8_t led_states[ROWS][COLS] = { { 16, 16 }, { 16, 16 } };

void reboot(void) {
  reset_usb_boot(25, 0);
}

void handle_quit_input(char c) {
  if (c == 'r') reboot();
}

void led_pattern(void) {
  while (true) {
    for (int i = 0; i < positive_num; i++) {
      gpio_put(positives[i], 1);
      handle_quit_input(getchar_timeout_us(100));
      sleep_ms(500);
      gpio_put(positives[i], 0);
    }
  }
}

void init_pins(void) {
  for (int i = 0; i < positive_num; i++) {
    gpio_init(positives[i]);
    gpio_set_dir(positives[i], GPIO_OUT);
  }
  for (int i = 0; i < negative_num; i++) {
    gpio_init(negatives[i]);
    gpio_set_dir(negatives[i], GPIO_OUT);
  }
  stdio_init_all();
}

int main(void) {
  init_pins();
  led_pattern();
  return 0;
}
