#ifndef PINHEAD_MATRIX_H
#define PINHEAD_MATRIX_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Pin Head – Pin Matrix
 * 16×16 logical matrix modelled on the EMS Synthi AKS / VCS3
 * ------------------------------------------------------------------------- */

#define MATRIX_ROWS     16
#define MATRIX_COLS     16
#define CORE_SIZE       10          /* visible size of the Core view */

/* Pin types – coloured pins from the original Synthi */
typedef enum {
    PIN_NONE  = 0,      /* empty / black */
    PIN_WHITE = 1,      /* normal full-level connection */
    PIN_RED   = 2,      /* precision / cleaner full-level */
    PIN_GREEN = 3       /* attenuated ≈ –12 dB */
} PinType;

/* Gain applied by each pin type */
static inline float pin_gain(PinType t)
{
    switch (t) {
        case PIN_WHITE:
        case PIN_RED:   return 1.0f;
        case PIN_GREEN: return 0.25f;    /* ≈ –12 dB */
        default:        return 0.0f;
    }
}

typedef struct {
    PinType pins[MATRIX_ROWS][MATRIX_COLS];

    /* View */
    bool    full_page;          /* false = Core 10×10, true = Full 16×16 */
    uint8_t scroll_row;         /* top-most visible row   (Full Page only) */
    uint8_t scroll_col;         /* left-most visible col  (Full Page only) */

    /* Cursor (always in absolute 0..15 coordinates) */
    uint8_t cursor_row;
    uint8_t cursor_col;
} MatrixState;

/* Lifecycle */
void matrix_init(MatrixState *m);
void matrix_clear(MatrixState *m);
void matrix_set_view(MatrixState *m, bool full_page);

/* Navigation – call with delta from Knob 1 / Knob 2 */
void matrix_move_cursor(MatrixState *m, int d_row, int d_col);

/* Pin editing – cycle NONE → WHITE → RED → GREEN → NONE */
void matrix_cycle_pin(MatrixState *m);

/* Direct access */
PinType matrix_get(const MatrixState *m, uint8_t row, uint8_t col);
void    matrix_set(MatrixState *m, uint8_t row, uint8_t col, PinType p);

/* -------------------------------------------------------------------------
 * Drawing
 * The platform must supply a simple drawing API (see matrix_draw.c comments).
 * ------------------------------------------------------------------------- */

/* Visible window size in cells (depends on screen layout) */
#define VISIBLE_ROWS_CORE   10
#define VISIBLE_COLS_CORE   10
#define VISIBLE_ROWS_FULL    8   /* example – adjust to real TFT layout */
#define VISIBLE_COLS_FULL    8

void matrix_draw(const MatrixState *m);

/* Source / destination short labels for the sticky headers */
const char *matrix_row_label(uint8_t row);   /* "1" … "16" or short name */
const char *matrix_col_label(uint8_t col);   /* "A" … "P" */

/* Full descriptive names (used for the 45° slanted labels) */
const char *matrix_row_name(uint8_t row);
const char *matrix_col_name(uint8_t col);

#ifdef __cplusplus
}
#endif

#endif /* PINHEAD_MATRIX_H */
