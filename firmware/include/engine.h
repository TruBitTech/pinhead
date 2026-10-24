#ifndef PINHEAD_ENGINE_H
#define PINHEAD_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include "matrix.h"
#include "joystick.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Pin Head – Sound Engine
 * Three oscillators, noise, resonant low-pass filter, ring modulator,
 * trapezoid envelope, matrix-addressable spring reverb + two optional
 * global post-effects (lush reverb + ping-pong delay).
 * ------------------------------------------------------------------------- */

#define ENGINE_MAX_VOICES  6

/* ---- Global lush reverb (Schroeder-style) ---- */
#define LUSH_COMBS     4
#define LUSH_ALLPASSES 2
#define LUSH_COMB_LEN  1200
#define LUSH_AP_LEN    400

typedef struct {
    float comb_buf[LUSH_COMBS][LUSH_COMB_LEN];
    int   comb_idx[LUSH_COMBS];
    float comb_feedback;
    float ap_buf[LUSH_ALLPASSES][LUSH_AP_LEN];
    int   ap_idx[LUSH_ALLPASSES];
    float damp;
    float damp_state[LUSH_COMBS];
    float mix;
    bool  enabled;
} LushReverb;

/* ---- Global ping-pong delay ---- */
#define PINGPONG_MAX_SAMPLES  22050   /* 500 ms at 44.1 kHz */

typedef struct {
    float buf_l[PINGPONG_MAX_SAMPLES];
    float buf_r[PINGPONG_MAX_SAMPLES];
    int   write_idx;
    int   delay_samples;
    float feedback;
    float mix;
    bool  enabled;
} PingPongDelay;

/* ---- Matrix-addressable lo-fi tape delay ---- */
#define TAPE_MAX_SAMPLES  13230   /* 300 ms at 44.1 kHz */

typedef struct {
    float buf[TAPE_MAX_SAMPLES];
    int   write_idx;
    int   delay_samples;
    float feedback;          /* 0…0.9 */
    float tone;              /* 0 = dark, 1 = brighter */
    float wow_depth;         /* subtle speed modulation */
    float wow_phase;
    float lp_state;          /* simple darkening filter */
    float saturation;        /* soft clip amount */
    float input_level;       /* from matrix column P */
    float output_level;      /* available as matrix source row 15 */
} TapeDelay;

typedef struct {
    float phase1, phase2, phase3;
    float inc1,   inc2,   inc3;
    float osc1_level, osc2_level, osc3_level;
    float osc1_shape, osc2_shape, osc3_shape;

    float noise_level, noise_color, noise_state;

    float filter_cutoff, filter_res;
    float f_low, f_band, f_high;

    float ring_a, ring_b, ring_out;

    float env_attack, env_on, env_decay, env_off;
    float env_level, env_signal;
    int   env_stage;
    float env_timer;

    /* Matrix-addressable spring reverb */
    float reverb_mix, reverb_feedback;
    float reverb_buf[256];
    int   reverb_idx;

    int   note;
    float velocity;
    bool  active, gate;

    float bus_osc1, bus_osc2, bus_osc3, bus_noise;
    float bus_filter_in, bus_out;
} EngineVoice;

typedef struct {
    EngineVoice voices[ENGINE_MAX_VOICES];
    int active_voices;

    MatrixState   *matrix;
    JoystickState *joystick;

    LushReverb     lush;
    PingPongDelay  pingpong;
    TapeDelay      tape;          /* matrix-addressable lo-fi tape */

    float sample_rate;
    float inv_sample_rate;
    uint32_t rng_state;
} Engine;

void engine_init(Engine *e, float sample_rate);
void engine_set_matrix(Engine *e, MatrixState *m);
void engine_set_joystick(Engine *e, JoystickState *j);

void engine_process(Engine *e, float *left, float *right, int n_frames);

void engine_note_on(Engine *e, int note, float velocity);
void engine_note_off(Engine *e, int note);

/* Global lush reverb */
void engine_lush_enable(Engine *e, bool on);
void engine_lush_set_mix(Engine *e, float mix);
void engine_lush_set_decay(Engine *e, float decay);

/* Global ping-pong delay */
void engine_pingpong_enable(Engine *e, bool on);
void engine_pingpong_set_mix(Engine *e, float mix);
void engine_pingpong_set_time(Engine *e, float seconds);   /* 0.01 … 0.5 */
void engine_pingpong_set_feedback(Engine *e, float fb);    /* 0 … 0.95 */

#ifdef __cplusplus
}
#endif

#endif /* PINHEAD_ENGINE_H */
