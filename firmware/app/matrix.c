#include "matrix.h"
#include <string.h>

/* Platform drawing stubs – replace with real plat_draw_* from fm1-base */
extern void plat_draw_fill(int x, int y, int w, int h, uint16_t color);
extern void plat_draw_rect(int x, int y, int w, int h, uint16_t color);
extern void plat_draw_circle(int cx, int cy, int r, uint16_t color, bool fill);
extern void plat_draw_text(int x, int y, const char *txt, uint16_t color);
extern void plat_draw_text_rotated(int x, int y, const char *txt, uint16_t color, int angle_deg);

#define COL_BG          0x0000
#define COL_GRID        0x3186
#define COL_HEADER      0xC618
#define COL_CURSOR      0xFFFF
#define COL_PIN_WHITE   0xFFFF
#define COL_PIN_RED     0xF800
#define COL_PIN_GREEN   0x07E0
#define COL_EMPTY       0x2104

#define CELL_SIZE       18
#define HEADER_W        22
#define HEADER_H        18
#define MATRIX_ORIGIN_X (HEADER_W + 2)
#define MATRIX_ORIGIN_Y (HEADER_H + 2)

void matrix_init(MatrixState *m)
{
    memset(m, 0, sizeof(*m));
    m->full_page   = false;
    m->scroll_row  = 0;
    m->scroll_col  = 0;
    m->cursor_row  = 0;
    m->cursor_col  = 0;
}

void matrix_clear(MatrixState *m)
{
    memset(m->pins, 0, sizeof(m->pins));
}

void matrix_set_view(MatrixState *m, bool full_page)
{
    m->full_page = full_page;
    if (!full_page) {
        m->scroll_row = 0;
        m->scroll_col = 0;
        if (m->cursor_row >= CORE_SIZE) m->cursor_row = CORE_SIZE - 1;
        if (m->cursor_col >= CORE_SIZE) m->cursor_col = CORE_SIZE - 1;
    }
}

void matrix_move_cursor(MatrixState *m, int d_row, int d_col)
{
    if (!m->full_page) {
        int r = (int)m->cursor_row + d_row;
        int c = (int)m->cursor_col + d_col;
        if (r < 0) r = CORE_SIZE - 1;
        else if (r >= CORE_SIZE) r = 0;
        if (c < 0) c = CORE_SIZE - 1;
        else if (c >= CORE_SIZE) c = 0;
        m->cursor_row = (uint8_t)r;
        m->cursor_col = (uint8_t)c;
    } else {
        int new_row = (int)m->cursor_row + d_row;
        int new_col = (int)m->cursor_col + d_col;
        if (new_row < 0) new_row = 0;
        if (new_row > 15) new_row = 15;
        if (new_col < 0) new_col = 0;
        if (new_col > 15) new_col = 15;
        m->cursor_row = (uint8_t)new_row;
        m->cursor_col = (uint8_t)new_col;
        const int vis_r = VISIBLE_ROWS_FULL;
        const int vis_c = VISIBLE_COLS_FULL;
        if (m->cursor_row < m->scroll_row) m->scroll_row = m->cursor_row;
        else if (m->cursor_row >= m->scroll_row + vis_r) m->scroll_row = m->cursor_row - vis_r + 1;
        if (m->cursor_col < m->scroll_col) m->scroll_col = m->cursor_col;
        else if (m->cursor_col >= m->scroll_col + vis_c) m->scroll_col = m->cursor_col - vis_c + 1;
        if (m->scroll_row > MATRIX_ROWS - vis_r) m->scroll_row = MATRIX_ROWS - vis_r;
        if (m->scroll_col > MATRIX_COLS - vis_c) m->scroll_col = MATRIX_COLS - vis_c;
    }
}

void matrix_cycle_pin(MatrixState *m)
{
    PinType *p = &m->pins[m->cursor_row][m->cursor_col];
    *p = (PinType)((*p + 1) % 4);
}

PinType matrix_get(const MatrixState *m, uint8_t row, uint8_t col)
{
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return PIN_NONE;
    return m->pins[row][col];
}

void matrix_set(MatrixState *m, uint8_t row, uint8_t col, PinType p)
{
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return;
    m->pins[row][col] = p;
}

static const char *row_short[MATRIX_ROWS] = {
    "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"
};
static const char *col_short[MATRIX_COLS] = {
    "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P"
};
static const char *row_names[MATRIX_ROWS] = {
    "Osc1 Sine","Osc1 Saw","Osc2 Pulse","Osc2 Tri","Osc3","Noise","Sequencer","Keyboard",
    "Trapezoid","Ring Mod","Env Signal","Joy X","Joy Y","LFO","Filter","Tape Out"
};
static const char *col_names[MATRIX_COLS] = {
    "Osc1 Pitch","Osc2 Pitch","Osc3 Pitch","Filter In","Filter Cut","Filter Res",
    "Ring A","Ring B","Reverb","Output","Osc1 Lvl","Osc2 Lvl","Osc3 Lvl","Env Ctrl","Out 2","Tape In"
};

const char *matrix_row_label(uint8_t row) { return (row < MATRIX_ROWS) ? row_short[row] : "?"; }
const char *matrix_col_label(uint8_t col) { return (col < MATRIX_COLS) ? col_short[col] : "?"; }
const char *matrix_row_name(uint8_t row)  { return (row < MATRIX_ROWS) ? row_names[row] : "?"; }
const char *matrix_col_name(uint8_t col)  { return (col < MATRIX_COLS) ? col_names[col] : "?"; }

static uint16_t pin_colour(PinType t)
{
    switch (t) {
        case PIN_WHITE: return COL_PIN_WHITE;
        case PIN_RED:   return COL_PIN_RED;
        case PIN_GREEN: return COL_PIN_GREEN;
        default:        return COL_EMPTY;
    }
}

void matrix_draw(const MatrixState *m)
{
    const bool full = m->full_page;
    const int vis_rows = full ? VISIBLE_ROWS_FULL : VISIBLE_ROWS_CORE;
    const int vis_cols = full ? VISIBLE_COLS_FULL : VISIBLE_COLS_CORE;
    const int start_r  = full ? m->scroll_row : 0;
    const int start_c  = full ? m->scroll_col : 0;

    plat_draw_fill(0, 0, 240, 240, COL_BG);

    for (int c = 0; c < vis_cols; c++) {
        int abs_c = start_c + c;
        int x = MATRIX_ORIGIN_X + c * CELL_SIZE;
        plat_draw_text(x + 4, 2, matrix_col_label(abs_c), COL_HEADER);
    }
    for (int r = 0; r < vis_rows; r++) {
        int abs_r = start_r + r;
        int y = MATRIX_ORIGIN_Y + r * CELL_SIZE;
        plat_draw_text(2, y + 4, matrix_row_label(abs_r), COL_HEADER);
    }

    for (int r = 0; r < vis_rows; r++) {
        for (int c = 0; c < vis_cols; c++) {
            int abs_r = start_r + r;
            int abs_c = start_c + c;
            int x = MATRIX_ORIGIN_X + c * CELL_SIZE;
            int y = MATRIX_ORIGIN_Y + r * CELL_SIZE;
            plat_draw_rect(x, y, CELL_SIZE - 1, CELL_SIZE - 1, COL_GRID);
            PinType p = m->pins[abs_r][abs_c];
            int cx = x + CELL_SIZE / 2;
            int cy = y + CELL_SIZE / 2;
            if (p == PIN_NONE)
                plat_draw_circle(cx, cy, 2, COL_EMPTY, true);
            else
                plat_draw_circle(cx, cy, 5, pin_colour(p), true);
            if (abs_r == m->cursor_row && abs_c == m->cursor_col)
                plat_draw_circle(cx, cy, 8, COL_CURSOR, false);
        }
    }
    if (full) {
        plat_draw_text_rotated(4, MATRIX_ORIGIN_Y + 30, matrix_row_name(start_r), COL_HEADER, 90);
    }
}
