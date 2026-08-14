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
pkg/alephone_vita_legacy.vpk
pkg/alephone_vita_marathon2.vpk
pkg/alephone_vita_infinity.vpk
```

The build script supports separate Vita application profiles so one engine
codebase can produce multiple LiveArea bubbles with separate TitleIDs,
preferences, saves, quick-saves, logs, and data directories.

The repository contains JSON profile manifests for the supported games. The
current release path is direct installation of the three game VPKs through
VitaShell.

First release assets are three standalone VPKs:

```text
alephone_vita_legacy.vpk     -> Marathon
alephone_vita_marathon2.vpk  -> Marathon 2
alephone_vita_infinity.vpk   -> Marathon Infinity
```

## Rendering/performance

For the Vita target:

- Uses the software renderer.
- Disables OpenGL for the current build.
- Uses 960x544 fullscreen output.
- Disables Lua HUD rendering because it caused severe frame-time spikes.
- Adapts the classic HUD/cockpit rendering path for Vita software rendering.
- Replaces the original persistent liquid fade path with a Vita renderer overlay to keep underwater scenes playable.
- Uses dirty-rectangle updates for Marathon 1 cockpit/HUD changes so the full HUD does not need to be rescaled every frame.
- Gates FPS overlay and performance log output behind diagnostic build flags for release builds.
- Uses Vita-specific performance defaults.
- Keeps profiling/FPS code available for diagnostic builds.

## Input

Vita buttons and sticks are mapped into Aleph One input actions.

Analog axis thresholding was adjusted for Vita so forward/back movement works reliably on physical hardware.

## Gameplay/HUD

The current HUD state is pragmatic:

- original Lua/Enhanced HUD is disabled on Vita;
- classic HUD/cockpit rendering is adapted in the Vita software renderer;
- Marathon 1 HUD has been used through a full completion test on real hardware;
- Marathon 2 and Marathon Infinity HUD and automap paths are playable and ready for full-playthrough testing.
- HUD redraw requests are suppressed while the non-translucent automap is active to avoid one-frame HUD sprite flashes during map controls.
- Marathon 1 forces a full cockpit redraw after leaving map/terminal modes so dirty-rectangle updates do not leave stale black screen regions.
