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
pkg/alephone_vita_marathon1.vpk
pkg/alephone_vita_marathon2.vpk
pkg/alephone_vita_infinity.vpk
```

The build script supports separate Vita application profiles so one engine
codebase can produce multiple LiveArea bubbles with separate TitleIDs,
preferences, saves, quick-saves, logs, and data directories.

## Rendering/performance

For the Vita target:

- Uses the software renderer.
- Disables OpenGL for the current build.
- Uses 960x544 fullscreen output.
- Disables Lua HUD rendering because it caused severe frame-time spikes.
- Adapts the classic HUD/cockpit rendering path for Vita software rendering.
- Replaces the original persistent liquid fade path with a Vita renderer overlay to keep underwater scenes playable.
- Keeps temporary lightweight/debug HUD paths only where needed during active testing.
- Uses Vita-specific performance defaults.
- Includes profiling/FPS code used during device testing.

## Input

Vita buttons and sticks are mapped into Aleph One input actions.

Analog axis thresholding was adjusted for Vita so forward/back movement works reliably on physical hardware.

## Gameplay/HUD

The current HUD state is pragmatic:

- original Lua/Enhanced HUD is disabled on Vita;
- classic HUD/cockpit rendering is being restored in the Vita software renderer;
- Marathon 1 HUD is usable in gameplay testing;
- Marathon 2 and Marathon Infinity HUD and automap paths are under active compatibility testing.
- HUD redraw requests are suppressed while the non-translucent automap is active to avoid one-frame HUD sprite flashes during map controls.

## Compatibility policy

Fixes should be made in the Vita engine first whenever possible. Scenario-specific
handling belongs in the engine/profile layer only when the behavior is genuinely
tied to a specific scenario. Game data/resource changes should be a last resort.

## Repository policy

This repository is for the engine port only.

Do not commit:

- Marathon scenario data;
- installed Vita app data;
- VPK/SELF/ELF/BIN outputs;
- local logs or crash dumps.
