# PS Vita Port Changes

This document summarizes the PlayStation Vita-specific engineering work in Aleph One Vita.

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
- Sound/platform glue required by the Vita build.
- Cyrillic-capable text handling used by the optional Russian localization.

## Build and packaging

Added:

```text
build-vita-vpk.sh
vita-crt/
```

The build flow compiles the engine, creates a Vita `eboot.bin`, generates `param.sfo`, and packages profile-specific VPKs.

Outputs:

```text
pkg/eboot.bin
pkg/alephone_vita_legacy.vpk
pkg/alephone_vita_marathon2.vpk
pkg/alephone_vita_infinity.vpk
```

One shared engine codebase produces multiple standalone LiveArea applications through Vita profile manifests. Each profile can define its own Title ID, application name, preferences, saves, quick-saves, logs, data directory, artwork, manual assets, and compatibility behavior.

Current release VPKs:

```text
alephone_vita_legacy.vpk     -> Marathon
alephone_vita_marathon2.vpk  -> Marathon 2
alephone_vita_infinity.vpk   -> Marathon Infinity
```

The current release model is direct installation of the three standalone game VPKs through VitaShell.

## Rendering and performance

The Vita renderer was adapted around the console's performance characteristics.

- Uses fullscreen 960x544 widescreen output.
- Uses the software renderer as the supported Vita rendering path.
- Does not use the desktop OpenGL path in the current Vita release.
- Replaces the desktop Lua/Enhanced HUD renderer with a Vita-optimized classic HUD/cockpit path because Lua HUD rendering caused severe frame-time spikes on Vita.
- Preserves the classic gameplay HUD while avoiding the persistent HUD performance drops seen in early builds.
- Replaces the original persistent liquid fade path with a lightweight Vita renderer overlay during final presentation.
- Removes the previously identified persistent underwater/liquid frame-rate drop in the current tested path.
- Uses dirty-rectangle updates for Marathon cockpit/HUD changes so the complete HUD does not need to be rescaled every frame.
- Adapts automap, terminal, menu, and full-screen fade rendering for Vita.
- Suppresses HUD redraw requests while the non-translucent automap is active to avoid one-frame HUD sprite flashes during map controls.
- Forces a full Marathon cockpit redraw after leaving map/terminal modes so dirty-rectangle updates cannot leave stale black screen regions.
- Falls back to common streamed music filenames such as `Music.ogg` for intro/menu music when the original `Music` filename is not present.
- Uses Vita-specific performance defaults.
- Gates FPS overlays and performance log output behind diagnostic build flags.
- Keeps profiling/FPS instrumentation available for diagnostic builds without enabling it in normal releases.

## Input

Vita buttons and sticks are mapped directly into Aleph One gameplay actions.

Current user-facing layout:

| Vita input | Action |
| --- | --- |
| Left stick | Movement |
| Right stick | Turn / look |
| R | Primary fire |
| L | Secondary fire |
| Cross | Action / Use |
| Circle | Run |
| Square | Automap / minimap |
| Triangle | Center camera |
| D-pad | Weapon / inventory / map controls |
| Start | Quit current game to the main menu (with confirmation) |

Additional Vita input fixes include:

- Analog axis thresholding adjusted so forward/back movement works reliably on physical hardware.
- When the automap is open, D-pad up/down is reserved for map zoom and no longer changes the selected weapon at the same time.

## Gameplay, HUD, and menus

The current Vita gameplay presentation is a dedicated console adaptation of Aleph One's classic rendering paths.

- Classic HUD/cockpit rendering is integrated into the Vita software renderer.
- Marathon HUD/cockpit has been used through a complete start-to-finish campaign test on real hardware.
- Marathon 2 and Marathon Infinity HUD, automap, terminal, and menu paths are playable and have been exercised through direct level loading.
- Marathon 2 and Marathon Infinity use a Vita-side menu selection overlay for reliable selection feedback.
- Built-in level selection can be opened from the main menu with `L + R + Cross`.
- The level selector is useful for replay and targeted regression testing, including additional maps present in the scenario data.

## LiveArea integration

The three standalone game profiles include dedicated Vita presentation assets.

- Separate LiveArea bubbles for Marathon, Marathon 2, and Marathon Infinity.
- Game-specific icons and LiveArea artwork.
- Built-in LiveArea manuals for all three games.
- English and Russian manual pages are provided for installation, controls, translation setup, troubleshooting, and credits/legal information.

## Cyrillic and Russian translation support

The Vita engine includes Cyrillic text rendering support.

Optional Russian translation layers are supported for all three Marathon Trilogy profiles. The provided translation covers all story terminal text.

The translation layer is separate from the engine VPKs and does not replace the original scenario data. Users still provide the original game data and copy the optional `Russian/` folder into the corresponding game data directory.

