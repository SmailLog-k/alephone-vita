# PS Vita Port Status

## Summary

The PS Vita port is currently a playable real-hardware prototype.

The earlier toolchain blockers have been worked around. The current focus is polish: HUD completeness, performance stability, input tuning, packaging, and release hygiene.

## Confirmed working

- Builds with VitaSDK in WSL/Linux.
- Produces a VPK and `eboot.bin`.
- Installs and launches on PS Vita as title ID `ALEPH0001`.
- Loads Marathon data from `ux0:data/AlephOne/`.
- Runs Marathon 1 gameplay on real hardware.
- Fullscreen 960x544 output.
- Software renderer path.
- Vita clock/performance setup.
- Vita controller mapping.
- Lightweight in-game Vita HUD.
- FPS overlay for profiling.

## Current performance

Real-device testing has shown roughly 23-30 FPS depending on scene and HUD state.

The largest early performance issue was the Lua/Enhanced HUD path. On Vita, Lua HUD rendering is currently disabled and replaced with a lightweight native HUD.

## Current limitations

- Original full Aleph One HUD is not fully restored yet.
- Lightweight Vita HUD currently replaces the original HUD in gameplay.
- OpenGL rendering is disabled for the Vita target.
- Lua HUD is disabled for the Vita target.
- Network play is not implemented/tested for Vita.
- Packaging metadata/icons are still minimal.
- The current repository contains active development/profiling code and should be cleaned before a tagged release.

## Data layout

Game data is loaded from:

```text
ux0:data/AlephOne/
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

1. Finish HUD:
   - either adapt the original Aleph One HUD panel to fullscreen Vita output;
   - or expand the lightweight Vita HUD into a complete functional HUD.
2. Remove or gate profiling logs/FPS overlay for release builds.
3. Add final app icon/livearea assets.
4. Add reproducible clean-clone build instructions.
5. Test Marathon 1, Marathon 2, and Marathon Infinity data layouts separately.
6. Prepare a separate repository for game/scenario packaging instructions.
