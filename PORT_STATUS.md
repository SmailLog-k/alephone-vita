# PS Vita Port Status

## Summary

Aleph One Vita is playable across the complete Marathon Trilogy on real PlayStation Vita hardware. The current release provides three standalone game VPKs built from one shared Vita-optimized Aleph One engine codebase.

The core Vita port implementation for the current release scope is complete. Remaining work is focused on long-form playthrough validation and release QA.

- **Marathon** has been completed 100% from start to finish on real PS Vita hardware.
- **Marathon 2: Durandal** is playable on real hardware. Its levels load and play correctly through the built-in level selection screen; a full linear playthrough is still in progress.
- **Marathon Infinity** is playable on real hardware. Its levels load and play correctly through the built-in level selection screen; a full linear playthrough is still in progress.

## Confirmed working

- Builds with VitaSDK in WSL/Linux.
- Produces three profile-specific game VPKs and `eboot.bin` from one shared engine codebase.
- Installs and launches as three standalone LiveArea applications with separate Title IDs.
- Uses separate preferences, saves, logs, and data directories per game profile.
- Loads original game data from profile-specific directories under `ux0:data/AlephOne/`.
- Marathon full campaign completion confirmed on real PS Vita hardware.
- Marathon 2 levels load and play correctly on real hardware through the built-in level selection screen.
- Marathon Infinity levels load and play correctly on real hardware through the built-in level selection screen.
- Fullscreen 960x544 widescreen output.
- Vita-optimized software rendering path.
- Vita clock/performance setup.
- Vita controller mapping.
- Classic HUD/cockpit rendering adapted for the Vita software renderer.
- Automap, terminal, menu, and full-screen fade rendering adapted for Vita.
- Previously identified persistent HUD and underwater/liquid frame-time drops resolved in current tested paths.
- Built-in level selection from the main menu with `L + R + Cross`.
- Cyrillic text rendering support in the Vita engine.
- Optional Russian translation of all story terminal text for all three games.
- Bilingual English/Russian LiveArea manuals for all three game VPKs.

## Current performance

The current Vita release is tuned around a 30 FPS target on real hardware.

The largest early performance bottleneck was the desktop Lua/Enhanced HUD renderer. That renderer produced severe frame-time spikes on Vita, so the supported Vita configuration now uses the classic HUD/cockpit through a dedicated optimized software-rendering path.

The original persistent liquid fade path also caused major frame-rate drops when fully underwater. The Vita build replaces that path with a lightweight renderer overlay during final presentation.

Marathon's classic HUD/cockpit uses Vita-specific dirty-rectangle updates so the complete HUD does not need to be rescaled and blitted every frame.

The persistent Vita-specific FPS drops previously identified in the HUD and liquid rendering paths are no longer present in the current tested build.

## Vita design choices

These are intentional characteristics of the current Vita release:

- The supported Vita renderer is the software renderer; OpenGL is not used.
- The Vita build uses the optimized classic HUD/cockpit instead of the heavier Lua/Enhanced HUD renderer.
- Marathon 2 and Marathon Infinity use a Vita-side menu selection overlay for reliable selection feedback.
- The three games are installed directly as standalone VPKs through VitaShell.
- Normal release builds keep profiling overlays and performance log writes disabled.
- Network play is not part of the current Vita release.

## Release validation still in progress

- Complete start-to-finish playthrough validation of Marathon 2.
- Complete start-to-finish playthrough validation of Marathon Infinity.
- Continued long-session regression testing across all three VPKs.

The built-in level selection screen allows individual levels in Marathon 2 and Marathon Infinity to be loaded directly for targeted regression testing while the longer linear playthroughs continue.

## Data layout

Game data is loaded from profile-specific directories:

```text
ux0:data/AlephOne/                  # Marathon legacy layout
ux0:data/AlephOne/Marathon2/
ux0:data/AlephOne/MarathonInfinity/
```

Required files are profile-specific. See `README.md` for the tested Steam Classic Marathon Trilogy layouts.

Game data is intentionally not included in the engine repository or game VPKs.

## Future work

After current release validation:

1. Expand support for additional compatible Aleph One scenarios.
2. Continue long-session regression testing and maintenance.

