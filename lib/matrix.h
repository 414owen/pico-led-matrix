#ifndef matrix_h_INCLUDED
#define matrix_h_INCLUDED

#include <stdint.h>

#define POSITIVE_NUM 5
#define NEGATIVE_NUM 8
#define MATRIX_WIDTH POSITIVE_NUM
#define MATRIX_HEIGHT NEGATIVE_NUM

void init_matrix(void);
void write_pixel(int x, int y, uint8_t val);
void swap_buffers(void);

#endif // matrix_h_INCLUDED
