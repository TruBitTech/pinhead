#include "joystick.h"
#include <math.h>

/* Original Synthi joystick was roughly ±1.5 V.
 * We treat 1.0 as “full original range”. */

#ifndef JOYSTICK_DEFAULT_RANGE
#define JOYSTICK_DEFAULT_RANGE  0.70f   /* moderate default, feels like the original */
#endif

#ifndef JOYSTICK_DEADZONE
#define JOYSTICK_DEADZONE       0.04f
#endif

void joystick_init(JoystickState *j)
{
    j->x        = 0.0f;
    j->y        = 0.0f;
    j->range_x  = JOYSTICK_DEFAULT_RANGE;
    j->range_y  = JOYSTICK_DEFAULT_RANGE;
    j->deadzone = JOYSTICK_DEADZONE;
}

static float apply_deadzone(float v, float dz)
{
    if (v > -dz && v < dz)
        return 0.0f;
    /* Re-scale so the remaining range still reaches ±1 */
    float sign = (v >= 0.0f) ? 1.0f : -1.0f;
    float mag  = (fabsf(v) - dz) / (1.0f - dz);
    if (mag > 1.0f) mag = 1.0f;
    return sign * mag;
}

void joystick_set_from_knobs(JoystickState *j, float raw_x, float raw_y)
{
    /* Expect raw values already in –1 … +1.
     * If your platform gives 0…1, map them first. */
    j->x = apply_deadzone(raw_x, j->deadzone);
    j->y = apply_deadzone(raw_y, j->deadzone);
}

float joystick_mod_x(const JoystickState *j)
{
    return j->x * j->range_x;
}

float joystick_mod_y(const JoystickState *j)
{
    return j->y * j->range_y;
}

void joystick_set_range_x(JoystickState *j, float r)
{
    if (r < 0.0f) r = 0.0f;
    if (r > 1.0f) r = 1.0f;
    j->range_x = r;
}

void joystick_set_range_y(JoystickState *j, float r)
{
    if (r < 0.0f) r = 0.0f;
    if (r > 1.0f) r = 1.0f;
    j->range_y = r;
}
