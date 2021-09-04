#include <stddef.h>

static const int row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const int col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

void boardInit(void) {
    for (size_t i = 0; i < MATRIX_ROWS; ++i)
        gpio_init(row_pins[i]);
    for (size_t i = 0; i < MATRIX_COLS; ++i)
        gpio_init(col_pins[i]);
}
