#ifndef PINHEAD_UI_H
#define PINHEAD_UI_H

#include "matrix.h"
#include "joystick.h"
#include "engine.h"
#include "presets.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Pin Head – UI / Mode management
 * ------------------------------------------------------------------------- */

typedef enum {
    MODE_PLAY = 0,
    MODE_MATRIX,
    MODE_PRESET,
    MODE_GLO
} UiMode;

/* GLO pages – each page groups related parameters on K1/K2/K3 */
typedef enum {
    GLO_LUSH = 0,        /* K1 = Mix,     K2 = Decay */
    GLO_PINGPONG,        /* K1 = Mix,     K2 = Time,   K3 = Feedback */
    GLO_TAPE,            /* K1 = Time,    K2 = Feedback */
    GLO_JOYSTICK,        /* K1 = Range X, K2 = Range Y */
    GLO_PAGE_COUNT
} GloPage;

typedef struct {
    UiMode mode;
    UiMode previous_mode;

    MatrixState   matrix;
    JoystickState joystick;
    Engine        engine;

    int  current_preset;
    bool dirty;

    GloPage glo_page;
} PinHeadApp;

void ui_init(PinHeadApp *app, float sample_rate);
void ui_set_mode(PinHeadApp *app, UiMode mode);

void ui_knob(PinHeadApp *app, int knob_index, float delta_or_value);
void ui_button(PinHeadApp *app, int button_id, bool pressed);

void ui_draw(PinHeadApp *app);
void ui_process_audio(PinHeadApp *app, float *left, float *right, int n_frames);

#ifdef __cplusplus
}
#endif

#endif /* PINHEAD_UI_H */
