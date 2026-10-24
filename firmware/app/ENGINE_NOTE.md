# engine.c

The full engine implementation (~560 lines) lives in the local workspace at:
`/workspace/artifacts/pinhead/app/engine.c`

It contains:
- polyBLEP oscillators
- Noise
- Chamberlin SVF filter
- Ring modulator
- Trapezoid envelope
- Matrix evaluation with pin gains
- Spring reverb
- Lo-fi tape delay (matrix addressable)
- Global lush reverb and ping-pong delay
- `engine_process` stereo callback

It will be pushed in the next commit once the platform integration stubs are confirmed.

For now the headers and the rest of the app (matrix, ui, presets, joystick) are in this repo.
