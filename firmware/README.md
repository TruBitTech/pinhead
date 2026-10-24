# Pin Head Firmware

EMS Synthi AKS-inspired pin-matrix synthesizer for the M-VAVE FM-1.

## Status

These are **working sketches**, not a bootable firmware yet.

They implement:
- 16×16 pin matrix with white / red / green pin types and correct navigation (10×10 wrapping, 16×16 hard stops)
- Oscillators (polyBLEP), noise, Chamberlin SVF filter, ring modulator, trapezoid envelope
- Matrix-addressable spring reverb + lo-fi tape delay
- Global lush reverb and ping-pong delay
- Joystick X/Y with ±1.5 V style range controls
- GLO mode parameter pages
- Factory presets including "On the Run"

## Next step: integrate with a platform

The code calls platform functions (`plat_draw_*`, audio process, etc.) that must come from a base.

**Recommended:** [fm1-base](https://github.com/charlesvestal/fm1-base) (on felucca-bsp).

1. Clone fm1-base recursively.
2. Drop these files into the template app or a new app directory.
3. Wire `plat_draw_*` and the audio callback to the real platform APIs.
4. Build and package as a `.fwsc`.
5. Test in the host simulator and [fm1-emu](https://joakimeriksson.github.io/fm1-emu/).

See `INTEGRATION.md` for the mapping notes.

## Layout

```
firmware/
  include/     headers
  app/         implementation
```
