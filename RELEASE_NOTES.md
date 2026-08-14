# Aleph One Vita first release notes

This release provides three standalone PlayStation Vita VPKs built from one shared Aleph One Vita engine codebase.

## Downloads

```text
alephone_vita_legacy.vpk     -> Marathon
alephone_vita_marathon2.vpk  -> Marathon 2
alephone_vita_infinity.vpk   -> Marathon Infinity
```

Original game data is not included.

## Game status

- Marathon: playable and 100% completed on real PS Vita hardware.
- Marathon 2: playable on real PS Vita hardware; full-playthrough testing is still in progress.
- Marathon Infinity: playable on real PS Vita hardware; full-playthrough testing is still in progress.

## Installation summary

Install the desired VPK with VitaShell, then copy legally obtained game data to the matching Vita directory:

```text
Marathon:          ux0:data/AlephOne/
Marathon 2:        ux0:data/AlephOne/Marathon2/
Marathon Infinity: ux0:data/AlephOne/MarathonInfinity/
```

See `README.md` for the tested Steam Classic Marathon Trilogy file layouts.

## Notes

- Software rendering is used on Vita.
- OpenGL and Lua HUD are disabled for the Vita target.
- FPS overlay and performance logging are disabled in normal release builds.
- Networking is not implemented/tested for Vita.
- Scene-dependent performance variation is still expected.
