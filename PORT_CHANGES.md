# PS Vita Port Changes

This document summarizes the Vita-specific changes in this fork.

## Platform layer

New Vita-specific files live in:

```text
VitaPlatform/
```

They provide:

- Vita startup/shutdown integration.
- Vita path handling.
- Vita input mapping.
- Vita performance preferences.
- Vita-specific SDL/environment setup.
- Sound/platform glue needed by the current build.

## Build and packaging

Added:

```text
build-vita-vpk.sh
vita-crt/
```

The script builds the engine, creates a Vita `eboot.bin`, creates `param.sfo`, and packages a VPK.

Output:

```text
pkg/eboot.bin
pkg/alephone_vita.vpk
```

## Rendering/performance

For the Vita target:

- Uses the software renderer.
- Disables OpenGL for the current build.
- Uses 960x544 fullscreen output.
- Disables Lua HUD rendering because it caused severe frame-time spikes.
- Adds a lightweight native HUD path.
- Uses Vita-specific performance defaults.
- Includes profiling/FPS code used during device testing.

## Input

Vita buttons and sticks are mapped into Aleph One input actions.

Analog axis thresholding was adjusted for Vita so forward/back movement works reliably on physical hardware.

## Gameplay/HUD

The current HUD state is pragmatic:

- original Lua/Enhanced HUD is disabled on Vita;
- the old classic HUD panel is not fully restored in fullscreen mode yet;
- a lightweight native HUD displays health/shield, oxygen, weapon, ammo, and related gameplay information.

## Repository policy

This repository is for the engine port only.

Do not commit:

- Marathon scenario data;
- installed Vita app data;
- VPK/SELF/ELF/BIN outputs;
- local logs or crash dumps.
