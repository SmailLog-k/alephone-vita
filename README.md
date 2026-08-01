# Aleph One PS Vita

This repository contains a PlayStation Vita port of the Aleph One engine, the open source continuation of Bungie's Marathon engine.

The goal of this fork is engine-only Vita support. Marathon game data is not included and should live in a separate repository or be installed by the user from a legally obtained copy.

## Current status

Playable prototype.

- Marathon 1 starts on real PS Vita hardware.
- Software rendering is used; OpenGL/Lua HUD are disabled on Vita for performance.
- Fullscreen 960x544 output works.
- Performance target is 30 FPS. Current real-device testing is approximately 23-30 FPS depending on scene/HUD state.
- Vita controls are mapped and playable.
- A lightweight Vita HUD is implemented while the original Aleph One HUD is being adapted.
- Networking and some desktop-only integrations are not part of the Vita target yet.

This is not a polished release. Treat it as an active porting branch for developers and testers.

## Controls

Current Vita mapping:

| Vita input | Action |
| --- | --- |
| Left stick | Move forward/back and strafe |
| Right stick | Turn/look |
| R | Primary fire |
| L | Secondary fire |
| Cross | Action/use |
| Circle | Run |
| Left stick press | Run |
| Square | Automap |
| Triangle | Recenter/view action |
| D-pad | Weapon/inventory/map controls depending on game state |
| Select | Scores |

## Game data

This repository does not include Marathon data files.

For the current Vita build, place a scenario under:

```text
ux0:data/AlephOne/
```

The scenario directory must contain the required Aleph One files such as:

```text
Map
Shapes
Images
Sounds
```

Depending on the scenario, additional files may be needed.

## Vita build quick start

The build is intended to run from WSL/Linux with VitaSDK installed.

Expected default VitaSDK path:

```bash
/usr/local/vitasdk
```

Initial setup from a clean clone:

```bash
cd /path/to/alephone-vita
export VITASDK=/usr/local/vitasdk
export PATH="$VITASDK/bin:$PATH"

autoreconf -fi

PKG_CONFIG_LIBDIR="$VITASDK/arm-vita-eabi/lib/pkgconfig" \
./configure \
  --host=arm-vita-eabi \
  --with-boost="$VITASDK/arm-vita-eabi" \
  --disable-opengl \
  --without-curl \
  --without-zzip \
  --without-sdl_image \
  --without-png \
  --without-miniupnpc \
  --without-nfd \
  --without-vpx \
  --without-matroska \
  --without-ebml \
  --without-vorbis \
  --without-vorbisenc \
  --without-libyuv \
  --without-catch2

./build-vita-vpk.sh pkg
```

The build output is written to:

```text
pkg/alephone_vita.vpk
pkg/eboot.bin
```

For iterative testing on an already-installed app, upload only `pkg/eboot.bin` to:

```text
ux0:/app/ALEPH0001/eboot.bin
```

## Development notes

See:

- [docs/VITA_BUILD.md](docs/VITA_BUILD.md) for detailed build and deployment notes.
- [PORT_STATUS.md](PORT_STATUS.md) for the current port status.
- [PORT_CHANGES.md](PORT_CHANGES.md) for the high-level list of Vita-specific changes.

## Upstream

Aleph One upstream:

- Website: https://alephone.lhowon.org
- Source: https://github.com/Aleph-One-Marathon/alephone

This fork should keep upstream authorship and licensing intact.

## License

Aleph One is licensed under the GNU General Public License version 3. See [COPYING](COPYING).

Marathon names, trademarks, and game data belong to their respective owners and are not included here.
