# PS Vita Port Status

## Summary

The PS Vita port is currently a playable real-hardware prototype.

The earlier toolchain blockers have been worked around. The current focus is polish: HUD completeness, performance stability, input tuning, per-game application profiles, packaging, and release hygiene.

## Confirmed working

- Builds with VitaSDK in WSL/Linux.
- Produces profile-specific VPKs and `eboot.bin`.
- Produces three profile-specific game VPKs from one shared engine codebase.
- Installs and launches on PS Vita with separate title IDs per game profile.
- Loads game data from profile-specific directories under `ux0:data/AlephOne/`.
- Runs Marathon 1 gameplay on real hardware.
- Runs Marathon 2 gameplay on real hardware.
- Runs Marathon Infinity gameplay on real hardware.
- Fullscreen 960x544 output.
- Software renderer path.
- Vita clock/performance setup.
- Vita controller mapping.
- Classic HUD/cockpit rendering path under active Vita adaptation.
- Marathon 1 classic HUD is restored enough for gameplay testing.
- Marathon 2 and Marathon Infinity gameplay HUD and automap HUD paths are ready for longer gameplay testing.
- FPS overlay for profiling.

## Current performance

Real-device testing has shown roughly 23-30 FPS depending on scene and HUD state.

The largest early performance issue was the Lua/Enhanced HUD path. On Vita, Lua HUD rendering is currently disabled while classic/native rendering paths are adapted.

The original persistent liquid fade path caused major frame-rate drops when fully underwater. Vita now skips that palette/fade path and draws liquid tint as a cheap renderer overlay during final presentation.

## Current limitations

- Original full Aleph One HUD behavior is not fully verified across all Marathon Trilogy scenarios yet.
- Marathon 2 main menu uses a temporary Vita-side selection overlay because the original pressed-button redraw path is unreliable on Vita.
- OpenGL rendering is disabled for the Vita target.
- Lua HUD is disabled for the Vita target.
- Network play is not implemented/tested for Vita.
- Packaging metadata/icons are still minimal.
- Game bubbles are installed directly as VPKs through VitaShell.
- The current repository contains active development/profiling code and should be cleaned before a tagged release.

## Data layout

Game data is loaded from profile-specific directories:

```text
ux0:data/AlephOne/ # Marathon 1 legacy layout
ux0:data/AlephOne/Marathon2/
ux0:data/AlephOne/MarathonInfinity/
```

Required scenario files include:

```text
Map
Shapes
Images
Sounds
```

Game data is intentionally not included in the engine repository.

## Next engineering tasks

1. Continue long-session testing across Marathon 1, Marathon 2, and Infinity.
2. Finish and verify classic HUD behavior across the Marathon Trilogy.
3. Remove or gate profiling logs/FPS overlay for release builds.
4. Add final app icon/livearea assets.
5. Prepare release packaging instructions for legally obtained game/scenario data.
6. Add final LiveArea/icon assets for the three game VPKs.
