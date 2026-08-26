# Aleph One Vita release notes

This release updates the PlayStation Vita Marathon Trilogy port with final
release documentation, LiveArea manuals for all three games, optional Russian
terminal-text translation layers, and additional Vita-specific rendering/input
polish.

The release still contains only the engine and Vita-specific assets. Original
Marathon game data is not included.

## Downloads

```text
alephone_vita_legacy.vpk     -> Marathon
alephone_vita_marathon2.vpk  -> Marathon 2: Durandal
alephone_vita_infinity.vpk   -> Marathon Infinity
```

Optional Russian terminal-text translation layers:

```text
Russian_marathon1.rar
Russian_marathon2.rar
Russian_infinity.rar
```

## Game status

- Marathon is playable and has been completed 100% on real PS Vita hardware.
- Marathon 2: Durandal is playable on real PS Vita hardware. Levels load and
  play correctly through the built-in level selection screen; full linear
  playthrough testing is still in progress.
- Marathon Infinity is playable on real PS Vita hardware. Levels load and play
  correctly through the built-in level selection screen; full linear playthrough
  testing is still in progress.

## Changes since v0.1.0

- Added bilingual English/Russian LiveArea manuals for all three game VPKs.
- Added optional Russian terminal-text translation support for all three games.
- Added Cyrillic text rendering support in the Vita engine.
- Fixed Marathon Infinity intro/main-menu music fallback for Steam data layouts
  using `Music.ogg`.
- Added Vita LiveArea `content-rev` metadata so updated manuals/assets are
  refreshed more reliably after installation.
- Improved Vita automap controls: D-pad up/down now zoom the map without also
  changing the selected weapon while the map is open.
- Kept Marathon on the stable legacy data path:
  `ux0:data/AlephOne/`.
- Preserved separate game bubbles, Title IDs, preferences, saves, logs, and data
  directories for Marathon, Marathon 2, and Marathon Infinity.
- Improved build reliability when switching Vita profiles by forcing
  profile-dependent objects to rebuild.
- Updated English and Russian documentation for installation, controls,
  profiles, build instructions, status, and release validation.

## Installation summary

Install the desired VPK with VitaShell, then copy legally obtained game data to
the matching Vita directory:

```text
Marathon:          ux0:data/AlephOne/
Marathon 2:        ux0:data/AlephOne/Marathon2/
Marathon Infinity: ux0:data/AlephOne/MarathonInfinity/
```

If using the optional Russian translation layer, extract the matching archive
and copy the included `Russian/` folder to the same game data directory.

See `README.md` and `README.ru.md` for the tested Steam Classic Marathon Trilogy
file layouts.

## Notes

- Software rendering is used on Vita.
- OpenGL and Lua HUD are disabled for the Vita target.
- FPS overlay and performance logging are disabled in normal release builds.
- Built-in level selection is available from the main menu with `L + R + Cross`.
- Networking is not implemented/tested for Vita.
- Scene-dependent performance variation is still expected.
