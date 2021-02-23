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

const int BUFSIZE = 128;
const int zero = 0;

typedef struct led {
  uint x;
  uint y;
} led;

uint8_t led_states[ROWS][COLS] = { { 0, 8 }, { 8, 0 } };

void reboot(void) {
  reset_usb_boot(25, 0);
}

void handle_quit_input(char c) {
  if (c == 'r') reboot();
}

void io_debug(uint g, bool state) {
  printf("%d %d\n", g, state);
  gpio_put(g, state);
}

void print_state(void) {
  printf("leds on:");
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (led_states[i][j]) printf(" %d", i * COLS + j);
    }
  }
  printf("\n");
}

void display_leds(void) {
  uint8_t tick = 0;
  while (true) {
    for (uint i = 0; i < ROWS; i++) {
      gpio_put(i, 1);
      for (uint j = 0; j < COLS; j++) {
        gpio_put(COLS + j, !is_led_on(led_states[i][j], tick));
      }
      sleep_us(500);
      gpio_put(i, 0);
    }
    tick = (tick + 1) % MAX_BRIGHTNESS;
  }
}

// so we have time to connect to serial
void countdown(void) {
  for (int i = 30; i >= 0; i--) {
    printf("countdown %d\n", i);
    sleep_ms(100);
  }
}

int get_int_stdin(void) {
  while (true) {
    int res = getchar();
    handle_quit_input(res);
    return res - '0';
  }
}

int get_led_ind_stdin(void) {
  while (true) {
    int res = get_int_stdin();
    if (res >= 0 && res < ROWS * COLS) return res;
  }
}

void toggle_leds_from_input(void) {
  while (true) {
    printf("> ");
    fflush(stdout);
    int led_ind = get_led_ind_stdin();
    printf("got input: %d\n", led_ind);
    led_states[led_ind / COLS][led_ind % COLS]
      = !led_states[led_ind / COLS][led_ind % COLS];
    print_state();
  }
}

void set_leds_to(int to, int delay) {
  for (int i = 0; i < ROWS * COLS; i++) {
    int j = (i + 1) % (ROWS * COLS);
    uint8_t prev = led_states[j / COLS][j % COLS];
    int add = to > prev ? 1 : -1;
    for (int k = 0; k < abs(to - prev); k++) {
      led_states[j / COLS][j % COLS] += add;
      sleep_ms(delay / 100);
    }
  }
}

void led_pattern(void) {
  while (true) {
    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        led_states[i][j] += 1;
        led_states[i][j] %= MAX_BRIGHTNESS;
        printf(" %d", led_states[i][j]);
      }
    }
    putchar('\n');
    handle_quit_input(getchar_timeout_us(100));
    sleep_ms(500);
  }
}

void spin_for_reset(void) {
  while (true) {
    handle_quit_input(getchar_timeout_us(1000));
    sleep_ms(100);
  }
}

void init_pins(void) {
  for (int i = 0; i < 4; i++) {
    gpio_init(i);
    gpio_set_dir(i, GPIO_OUT);
  }
  stdio_init_all();
}

int main(void) {
  init_pins();
  countdown();
  multicore_launch_core1(display_leds);
  led_pattern();
  // spin_for_reset();
  return 0;
}
