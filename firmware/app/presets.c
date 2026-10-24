#include "presets.h"
#include <string.h>

/* Helper – clear then place a pin */
static void p(MatrixState *m, uint8_t row, uint8_t col, PinType t)
{
    matrix_set(m, row, col, t);
}

/* -------------------------------------------------------------------------
 * 1. On the Run  (Pink Floyd)
 * ------------------------------------------------------------------------- */
void preset_on_the_run(MatrixState *m)
{
    matrix_clear(m);

    /* Sequencer → Osc 2 Pitch (precision)          row 6, col 1 (B) */
    p(m, 6, 1, PIN_RED);

    /* Osc 2 Pulse → Filter Input                   row 2, col 3 (D) */
    p(m, 2, 3, PIN_WHITE);

    /* Noise → Ring Mod A                           row 5, col 6 (G) */
    p(m, 5, 6, PIN_WHITE);

    /* Trapezoid → Ring Mod B                       row 8, col 7 (H) */
    p(m, 8, 7, PIN_WHITE);

    /* Ring Mod → Output                            row 9, col 9 (J) */
    p(m, 9, 9, PIN_WHITE);

    /* Trapezoid → Filter Cutoff (subtle)           row 8, col 4 (E) */
    p(m, 8, 4, PIN_GREEN);

    /* Optional light reverb on the clicks          row 9, col 8 (I) */
    p(m, 9, 8, PIN_GREEN);
}

/* -------------------------------------------------------------------------
 * 2. Any Colour Lead
 * ------------------------------------------------------------------------- */
void preset_any_colour_lead(MatrixState *m)
{
    matrix_clear(m);

    p(m, 7, 0, PIN_RED);    /* Keyboard → Osc 1 Pitch */
    p(m, 7, 1, PIN_RED);    /* Keyboard → Osc 2 Pitch */
    p(m, 0, 3, PIN_WHITE);  /* Osc 1 Sine → Filter In */
    p(m, 2, 3, PIN_WHITE);  /* Osc 2 Pulse → Filter In */
    p(m, 9, 9, PIN_WHITE);  /* (via filter path) → Output */
    p(m, 8, 4, PIN_WHITE);  /* Trapezoid → Filter Cut */
    p(m, 0, 6, PIN_GREEN);  /* Osc 1 → Ring A (light edge) */
    p(m, 2, 7, PIN_GREEN);  /* Osc 2 → Ring B */
}

/* -------------------------------------------------------------------------
 * 3. Irregular Clicks
 * ------------------------------------------------------------------------- */
void preset_irregular_clicks(MatrixState *m)
{
    matrix_clear(m);

    p(m, 1, 0, PIN_WHITE);  /* Osc 1 Saw → Osc 1 Pitch (cross-mod) */
    p(m, 4, 1, PIN_WHITE);  /* Osc 3 → Osc 2 Pitch */
    p(m, 5, 3, PIN_WHITE);  /* Noise → Filter In */
    p(m, 1, 4, PIN_GREEN);  /* Osc 1 → Filter Cut */
    p(m, 9, 9, PIN_WHITE);  /* Filter → Output */
    /* Self-running envelope can be added later via Env Ctrl */
}

/* -------------------------------------------------------------------------
 * 4. Noise Sweep
 * ------------------------------------------------------------------------- */
void preset_noise_sweep(MatrixState *m)
{
    matrix_clear(m);

    p(m, 5, 3, PIN_WHITE);  /* Noise → Filter In */
    p(m, 8, 4, PIN_WHITE);  /* Trapezoid → Filter Cut */
    p(m, 8, 5, PIN_GREEN);  /* Trapezoid → Filter Res */
    p(m, 9, 9, PIN_WHITE);  /* Filter → Output */
    p(m, 9, 8, PIN_GREEN);  /* Filter → Reverb (space) */
}

/* -------------------------------------------------------------------------
 * 5. Spring Drone
 * ------------------------------------------------------------------------- */
void preset_spring_drone(MatrixState *m)
{
    matrix_clear(m);

    p(m, 0, 3, PIN_WHITE);  /* Osc 1 → Filter */
    p(m, 2, 3, PIN_GREEN);  /* Osc 2 → Filter (quieter) */
    p(m, 8, 4, PIN_GREEN);  /* Envelope → Filter Cut */
    p(m, 9, 8, PIN_WHITE);  /* Filter → Reverb */
    p(m, 15, 3, PIN_GREEN); /* Reverb → Filter In (feedback) */
    p(m, 9, 9, PIN_WHITE);  /* → Output */
}

/* -------------------------------------------------------------------------
 * 6. Metallic Perc
 * ------------------------------------------------------------------------- */
void preset_metallic_perc(MatrixState *m)
{
    matrix_clear(m);

    p(m, 5, 6, PIN_WHITE);  /* Noise → Ring A */
    p(m, 8, 7, PIN_WHITE);  /* Trapezoid → Ring B */
    p(m, 9, 9, PIN_WHITE);  /* Ring → Output */
    /* Keyboard → Env trigger can be added via Env Ctrl column */
}

/* -------------------------------------------------------------------------
 * 7. Empty Matrix (Init)
 * ------------------------------------------------------------------------- */
void preset_empty(MatrixState *m)
{
    matrix_clear(m);
}

/* -------------------------------------------------------------------------
 * Table
 * ------------------------------------------------------------------------- */

const Preset factory_presets[PRESET_COUNT] = {
    { "On the Run",       preset_on_the_run },
    { "Any Colour Lead",  preset_any_colour_lead },
    { "Irregular Clicks", preset_irregular_clicks },
    { "Noise Sweep",      preset_noise_sweep },
    { "Spring Drone",     preset_spring_drone },
    { "Metallic Perc",    preset_metallic_perc },
    { "Empty Matrix",     preset_empty },
};

void preset_load(MatrixState *m, int index)
{
    if (index < 0 || index >= PRESET_COUNT) index = 0;
    factory_presets[index].load(m);
}
