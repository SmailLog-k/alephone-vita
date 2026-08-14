# PS Vita Port Status

## Summary

The PS Vita port is currently at its first public release-candidate stage.

Marathon 1 has been completed 100% on real PS Vita hardware. Marathon 2 and Marathon Infinity are playable on real hardware, but full-playthrough testing is still in progress.

## Confirmed working

- Builds with VitaSDK in WSL/Linux.
- Produces profile-specific VPKs and `eboot.bin`.
- Produces three profile-specific game VPKs from one shared engine codebase.
- Installs and launches on PS Vita with separate title IDs per game profile.
- Loads game data from profile-specific directories under `ux0:data/AlephOne/`.
- Runs Marathon 1 gameplay on real hardware; full completion confirmed.
- Runs Marathon 2 gameplay on real hardware; full completion testing pending.
- Runs Marathon Infinity gameplay on real hardware; full completion testing pending.
- Fullscreen 960x544 output.
- Software renderer path.
- Vita clock/performance setup.
- Vita controller mapping.
- Classic HUD/cockpit rendering path adapted for Vita software rendering.
- Marathon 1 classic HUD is restored and has been used through a full completion test.
- Marathon 2 and Marathon Infinity gameplay HUD and automap HUD paths are ready for longer gameplay testing.
- FPS overlay and performance logging are optional diagnostic build features.

## Current performance

Real-device testing has shown playable performance across the Trilogy, with scene-dependent variation.

The largest early performance issue was the Lua/Enhanced HUD path. On Vita, Lua HUD rendering is currently disabled while classic/native rendering paths are adapted.

The original persistent liquid fade path caused major frame-rate drops when fully underwater. Vita now skips that palette/fade path and draws liquid tint as a cheap renderer overlay during final presentation.

Marathon 1's classic HUD/cockpit path uses Vita-specific dirty-rectangle updates to avoid rescaling and blitting the entire HUD every frame.

## Current limitations

- Original full Aleph One HUD behavior is not fully verified across every Marathon 2 and Marathon Infinity level yet.
- Marathon 2 main menu uses a temporary Vita-side selection overlay because the original pressed-button redraw path is unreliable on Vita.
- OpenGL rendering is disabled for the Vita target.
- Lua HUD is disabled for the Vita target.
- Network play is not implemented/tested for Vita.
- Game bubbles are installed directly as VPKs through VitaShell.
- Profiling output is disabled by default in release builds and can be enabled for diagnostic builds.

## Data layout

Game data is loaded from profile-specific directories:

```text
ux0:data/AlephOne/ # Marathon 1 legacy layout
ux0:data/AlephOne/Marathon2/
ux0:data/AlephOne/MarathonInfinity/
```

Required files are profile-specific. See `README.md` for the Steam Classic Marathon Trilogy layouts currently tested with this port.

Game data is intentionally not included in the engine repository.

## Next engineering tasks

1. Continue full-playthrough testing for Marathon 2 and Marathon Infinity.
2. Continue long-session regression testing across all three game VPKs.
3. Replace temporary Vita-side menu selection handling where the original menu redraw path can be made reliable.
4. Continue performance tuning for heavier scenes.
5. Expand profile documentation for future third-party Aleph One scenarios.
