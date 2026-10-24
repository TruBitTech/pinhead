# Integration with fm1-base

## Why fm1-base

- Clean platform layer (`plat.h`) for screen, keys, knobs, audio, MIDI
- Host simulator + browser emulator
- MPL-2.0 base; app can be GPL or other
- Used by CTL-1, Radio, and others

## Mapping

| Pin Head call | fm1-base / plat equivalent |
|---------------|----------------------------|
| `plat_draw_fill` / `plat_draw_rect` / `plat_draw_circle` / `plat_draw_text` | Drawing functions in `plat.h` |
| Audio process callback | App audio callback that calls `engine_process` |
| Knob deltas | Encoder / knob handlers |
| Button events | Panel button callbacks |

## Steps

1. `git clone --recursive https://github.com/charlesvestal/fm1-base`
2. Copy `firmware/include/*.h` and `firmware/app/*.c` into the template or a new app folder.
3. Implement or wrap the `plat_*` drawing stubs using the real platform API.
4. Call `ui_init`, `ui_process_audio`, and `ui_draw` from the app entry points.
5. Build with the fm1-base tools and test in the emulator.

## Current stubs

The drawing and some audio entry points are declared `extern` in `matrix.c` and `ui.c`. They must be satisfied by the platform or by thin wrappers.
