# Aleph One PS Vita

This repository contains a PlayStation Vita port of the Aleph One engine, the open source continuation of Bungie's Marathon engine.

The goal of this fork is engine-only Vita support. Marathon game data is not included and should live in a separate repository or be installed by the user from a legally obtained copy.

## Current status

Playable real-hardware prototype.

- Marathon 1 is playable on real PS Vita hardware.
- Marathon 2 starts and is in active compatibility testing.
- Software rendering is used; OpenGL/Lua HUD are disabled on Vita for performance.
- Fullscreen 960x544 output works.
- Performance target is 30 FPS. Current real-device testing is approximately 23-30 FPS depending on scene/HUD state.
- Vita controls are mapped and playable.
- Classic scenario HUD rendering is being adapted for Vita. Marathon 1 HUD and Marathon 2 gameplay HUD are partially restored.
- Networking and some desktop-only integrations are not part of the Vita target yet.
- Known active issue: underwater/full-screen liquid effects can still reduce frame rate significantly on Vita.

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

## Vita application profiles and game data

This repository does not include Marathon data files. The Vita port is built as
one engine codebase with separate application profiles for each game. Each
profile produces a different LiveArea bubble, TitleID, data directory,
preferences directory, save directory, quick-save directory, and log directory.

Current profiles:

```text
marathon1  -> TitleID ALEPH0001 -> ux0:data/AlephOne/Marathon/
marathon2  -> TitleID ALEPH0002 -> ux0:data/AlephOne/Marathon2/
infinity   -> TitleID ALEPH0003 -> ux0:data/AlephOne/MarathonInfinity/
```

Each scenario directory must contain the required Aleph One files, for example:

```text
Map
Shapes
Images
Sounds
```

Depending on the scenario, additional files may be needed.

Users should provide game data from a legally obtained copy or from a legal
Aleph One scenario distribution. The engine repository may contain Vita-side
compatibility fixes required to make that original data playable on PS Vita, but
it should not contain commercial scenario resources.

The intended release model is not "replace files manually to switch games".
Instead, install separate VPKs and keep each game's data under its own
`ux0:data/AlephOne/<Game>/` directory.

Porting policy: fix problems in the Vita engine first whenever possible. Keep
scenario-specific handling in the engine/profile layer only when the behavior is
actually tied to a specific Marathon scenario. Do not patch game data unless the
problem is proven to be a resource/scenario issue.

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

./build-vita-vpk.sh pkg marathon1
./build-vita-vpk.sh pkg marathon2
./build-vita-vpk.sh pkg infinity
```

The build outputs are written to:

```text
pkg/alephone_vita_marathon1.vpk
pkg/alephone_vita_marathon2.vpk
pkg/alephone_vita_infinity.vpk
pkg/eboot.bin
```

For iterative testing on an already-installed app, upload only `pkg/eboot.bin`
to the matching TitleID:

```text
Marathon 1:        ux0:/app/ALEPH0001/eboot.bin
Marathon 2:        ux0:/app/ALEPH0002/eboot.bin
Marathon Infinity: ux0:/app/ALEPH0003/eboot.bin
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
