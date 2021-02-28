#include <stdbool.h>
#include <stdio.h>

#include "brightness.h"
#include "matrix.h"

#define GLYPH_HEIGHT 8
#define GLYPH_WIDTH 5

extern uint8_t console_font_5x8[];

// Index into a bit-encoded bitmap font
bool is_glyph_pixel_set(uint8_t *mask, int i, int j) {
  return (mask[j] & (1 << (GLYPH_WIDTH - 1 - i))) > 0;
}

void display_char(char c) {
  for (int i = 0; i < GLYPH_HEIGHT; i++) {
    for (int j = 0; j < GLYPH_WIDTH; j++) {
      write_pixel(j, i, MAX_BRIGHTNESS * is_glyph_pixel_set(&console_font_5x8[c * GLYPH_HEIGHT], j, i));
    }
  }
  swap_buffers();
}
