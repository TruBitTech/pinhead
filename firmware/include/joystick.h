#ifndef PINHEAD_JOYSTICK_H
#define PINHEAD_JOYSTICK_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Pin Head – Joystick
 * Models the EMS Synthi AKS / VCS3 joystick as closely as practical.
 *
 * Original characteristics:
 *   - Output range approximately ±1.5 V
 *   - Separate range (sensitivity) controls for X and Y
 *   - Limited depth was part of the instrument’s character
 * ------------------------------------------------------------------------- */

typedef struct {
    /* Current stick position, normalised –1.0 … +1.0 */
    float x;
    float y;

    /* Range / sensitivity (0.0 = no effect, 1.0 = full ±1.5 V equivalent) */
    float range_x;
    float range_y;

    /* Optional soft centre dead-zone (0.0 = none, 0.05–0.1 typical) */
    float deadzone;
} JoystickState;

void joystick_init(JoystickState *j);

/* Call from Play-mode knob handler.
 * raw_x / raw_y are the normalised knob values (–1 … +1 or 0 … 1 mapped). */
void joystick_set_from_knobs(JoystickState *j, float raw_x, float raw_y);

/* Final modulation signal that is fed into the matrix sources
 * “Joy X” and “Joy Y”.  Already scaled by the range controls. */
float joystick_mod_x(const JoystickState *j);
float joystick_mod_y(const JoystickState *j);

/* Direct access for UI / settings pages */
void joystick_set_range_x(JoystickState *j, float r);
void joystick_set_range_y(JoystickState *j, float r);

#ifdef __cplusplus
}
#endif

#endif /* PINHEAD_JOYSTICK_H */
