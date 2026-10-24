#ifndef PINHEAD_PRESETS_H
#define PINHEAD_PRESETS_H

#include "matrix.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Factory presets for Pin Head
 * Coordinates are absolute (row 0..15, col 0..15)
 * Colour: PIN_WHITE / PIN_RED / PIN_GREEN
 * ------------------------------------------------------------------------- */

typedef struct {
    const char *name;
    void (*load)(MatrixState *m);
} Preset;

/* Individual loaders */
void preset_on_the_run(MatrixState *m);
void preset_any_colour_lead(MatrixState *m);
void preset_irregular_clicks(MatrixState *m);
void preset_noise_sweep(MatrixState *m);
void preset_spring_drone(MatrixState *m);
void preset_metallic_perc(MatrixState *m);
void preset_empty(MatrixState *m);

/* Table of all factory presets */
#define PRESET_COUNT 7
extern const Preset factory_presets[PRESET_COUNT];

/* Convenience */
void preset_load(MatrixState *m, int index);

#ifdef __cplusplus
}
#endif

#endif /* PINHEAD_PRESETS_H */
