#include "pico/bootrom.h"

void reboot(void) {
  reset_usb_boot(25, 0);
}

void handle_quit_input(char c) {
  if (c == 'r') reboot();
}
