#include "ui.h"
#include <stdio.h>
#include <math.h>

extern void plat_draw_text(int x, int y, const char *txt, uint16_t color);

static const char *glo_page_names[GLO_PAGE_COUNT] = {
    "Lush Reverb", "Ping-Pong Delay", "Tape Delay", "Joystick"
};

static float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static float apply_delta(float current, float delta, float speed) { return clampf(current + delta * speed, 0.0f, 1.0f); }

void ui_init(PinHeadApp *app, float sample_rate)
{
    matrix_init(&app->matrix);
    joystick_init(&app->joystick);
    engine_init(&app->engine, sample_rate);
    engine_set_matrix(&app->engine, &app->matrix);
    engine_set_joystick(&app->engine, &app->joystick);
    app->mode = MODE_PLAY;
    app->previous_mode = MODE_PLAY;
    app->current_preset = 0;
    app->dirty = false;
    app->glo_page = GLO_LUSH;
    preset_load(&app->matrix, 0);
}

void ui_set_mode(PinHeadApp *app, UiMode mode)
{
    if (mode == MODE_GLO && app->mode != MODE_GLO) app->previous_mode = app->mode;
    app->mode = mode;
    if (mode == MODE_MATRIX) matrix_set_view(&app->matrix, false);
}

void ui_knob(PinHeadApp *app, int knob_index, float delta)
{
    switch (app->mode) {
    case MODE_PLAY:
        if (knob_index == 0 || knob_index == 1) {
            float x = (knob_index == 0) ? delta : app->joystick.x;
            float y = (knob_index == 1) ? delta : app->joystick.y;
            joystick_set_from_knobs(&app->joystick, x, y);
        }
        break;
    case MODE_MATRIX:
        if (knob_index == 0) {
            int dir = (delta > 0.01f) ? 1 : (delta < -0.01f) ? -1 : 0;
            if (dir) matrix_move_cursor(&app->matrix, 0, dir);
        } else if (knob_index == 1) {
            int dir = (delta > 0.01f) ? 1 : (delta < -0.01f) ? -1 : 0;
            if (dir) matrix_move_cursor(&app->matrix, dir, 0);
        } else if (knob_index == 2) {
            matrix_cycle_pin(&app->matrix);
            app->dirty = true;
        }
        break;
    case MODE_PRESET:
        if (knob_index == 0) {
            int idx = app->current_preset + ((delta > 0) ? 1 : -1);
            if (idx < 0) idx = PRESET_COUNT - 1;
            if (idx >= PRESET_COUNT) idx = 0;
            app->current_preset = idx;
            preset_load(&app->matrix, idx);
            app->dirty = false;
        }
        break;
    case MODE_GLO:
        if (knob_index == 0) {
            int dir = (delta > 0.02f) ? 1 : (delta < -0.02f) ? -1 : 0;
            if (dir) {
                int p = (int)app->glo_page + dir;
                if (p < 0) p = GLO_PAGE_COUNT - 1;
                if (p >= GLO_PAGE_COUNT) p = 0;
                app->glo_page = (GloPage)p;
            }
            break;
        }
        switch (app->glo_page) {
        case GLO_LUSH:
            if (knob_index == 1) engine_lush_set_mix(&app->engine, apply_delta(app->engine.lush.mix, delta, 0.5f));
            else if (knob_index == 2) engine_lush_set_decay(&app->engine, apply_delta((app->engine.lush.comb_feedback - 0.70f) / 0.25f, delta, 0.5f));
            break;
        case GLO_PINGPONG:
            if (knob_index == 1) engine_pingpong_set_mix(&app->engine, apply_delta(app->engine.pingpong.mix, delta, 0.5f));
            else if (knob_index == 2) {
                float sec = app->engine.pingpong.delay_samples * app->engine.inv_sample_rate;
                sec = apply_delta(sec / 0.5f, delta, 0.5f) * 0.5f;
                engine_pingpong_set_time(&app->engine, sec);
            } else if (knob_index == 3) engine_pingpong_set_feedback(&app->engine, apply_delta(app->engine.pingpong.feedback, delta, 0.5f));
            break;
        case GLO_TAPE:
            if (knob_index == 1) {
                float sec = app->engine.tape.delay_samples * app->engine.inv_sample_rate;
                sec = apply_delta(sec / 0.30f, delta, 0.5f) * 0.30f;
                int samples = (int)(sec * app->engine.sample_rate);
                if (samples < 64) samples = 64;
                if (samples >= TAPE_MAX_SAMPLES) samples = TAPE_MAX_SAMPLES - 1;
                app->engine.tape.delay_samples = samples;
            } else if (knob_index == 2) {
                app->engine.tape.feedback = apply_delta(app->engine.tape.feedback, delta, 0.5f);
                if (app->engine.tape.feedback > 0.9f) app->engine.tape.feedback = 0.9f;
            }
            break;
        case GLO_JOYSTICK:
            if (knob_index == 1) joystick_set_range_x(&app->joystick, apply_delta(app->joystick.range_x, delta, 0.5f));
            else if (knob_index == 2) joystick_set_range_y(&app->joystick, apply_delta(app->joystick.range_y, delta, 0.5f));
            break;
        default: break;
        }
        break;
    default: break;
    }
}

void ui_button(PinHeadApp *app, int button_id, bool pressed)
{
    if (!pressed) return;
    switch (button_id) {
    case 0: ui_set_mode(app, (app->mode == MODE_MATRIX) ? MODE_PLAY : MODE_MATRIX); break;
    case 1: ui_set_mode(app, MODE_PRESET); break;
    case 2: if (app->mode == MODE_MATRIX) matrix_set_view(&app->matrix, !app->matrix.full_page); break;
    case 3: engine_lush_enable(&app->engine, !app->engine.lush.enabled); break;
    case 4: engine_pingpong_enable(&app->engine, !app->engine.pingpong.enabled); break;
    case 5:
        if (app->mode == MODE_GLO) ui_set_mode(app, app->previous_mode);
        else ui_set_mode(app, MODE_GLO);
        break;
    default: break;
    }
}

void ui_draw(PinHeadApp *app)
{
    char buf[64];
    switch (app->mode) {
    case MODE_MATRIX:
        matrix_draw(&app->matrix);
        plat_draw_text(4, 220, app->matrix.full_page ? "FULL 16x16" : "CORE 10x10", 0xFFFF);
        break;
    case MODE_PLAY:
        plat_draw_text(10, 20, "PIN HEAD – PLAY", 0xFFFF);
        plat_draw_text(10, 50, "K1/K2 = Joystick", 0xC618);
        plat_draw_text(10, 80, app->engine.lush.enabled ? "LUSH REVERB: ON" : "LUSH REVERB: OFF", app->engine.lush.enabled ? 0x07E0 : 0x8410);
        plat_draw_text(10, 100, app->engine.pingpong.enabled ? "PING-PONG: ON" : "PING-PONG: OFF", app->engine.pingpong.enabled ? 0x07E0 : 0x8410);
        break;
    case MODE_PRESET:
        plat_draw_text(10, 20, "PRESETS", 0xFFFF);
        for (int i = 0; i < PRESET_COUNT; i++) {
            snprintf(buf, sizeof(buf), "%c %s", (i == app->current_preset) ? '>' : ' ', factory_presets[i].name);
            plat_draw_text(10, 50 + i * 18, buf, 0xC618);
        }
        break;
    case MODE_GLO:
        plat_draw_text(10, 8, "GLOBAL SETTINGS", 0xFFFF);
        snprintf(buf, sizeof(buf), "%d/%d  %s", (int)app->glo_page + 1, GLO_PAGE_COUNT, glo_page_names[app->glo_page]);
        plat_draw_text(10, 32, buf, 0xFFE0);
        /* page-specific drawing omitted for brevity – see full local file */
        plat_draw_text(10, 220, "SELECT=page  GLO=exit", 0x8410);
        break;
    default: break;
    }
}

void ui_process_audio(PinHeadApp *app, float *left, float *right, int n_frames)
{
    engine_process(&app->engine, left, right, n_frames);
}
