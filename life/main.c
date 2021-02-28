#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"

#include "bootsel.h"
#include "brightness.h"
#include "char.h"
#include "matrix.h"

extern enum matrix_dims dims;

bool second_board[MATRIX_HEIGHT][MATRIX_WIDTH];
bool board[MATRIX_HEIGHT][MATRIX_WIDTH] = {
  {0,1,0,0,0},
  {0,0,1,0,0},
  {1,1,1,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0}
};

int count_neighbours(int row, int col) {
  int res = 0 - board[row][col];
  for (int i = -1; i <= 1; i++) {
    bool *r = board[(row + i + MATRIX_HEIGHT) % MATRIX_HEIGHT];
    for (int j = -1; j <= 1; j++) {
      res += r[(col + j + MATRIX_WIDTH) % MATRIX_WIDTH];
    }
  }
  return res;
}

bool is_live(int row, int col) {
  const bool prev_live = board[row][col];
  const int neighbours = count_neighbours(row, col);
  if (prev_live) return neighbours == 2 || neighbours == 3;
  return neighbours == 3;
}

void update_place(int row, int col) {
  const bool live = is_live(row, col);
  write_pixel(col, row, live * MAX_BRIGHTNESS);
  second_board[row][col] = live;
}

void tick() {
  for (int i = 0; i < MATRIX_HEIGHT; i++) {
    for (int j = 0; j < MATRIX_WIDTH; j++) {
      update_place(i, j);
    }
  }
  swap_buffers();
  memcpy(board, second_board, sizeof(uint8_t) * MATRIX_HEIGHT * MATRIX_WIDTH);
}

int main(void) {
  stdio_init_all();
  init_matrix();
  sleep_ms(2000);
  swap_buffers();
  count_neighbours(1, 1);
  while (true) {
    tick();
    handle_quit_input(getchar_timeout_us(100));
    sleep_ms(100);
  }
  return 0;
}
