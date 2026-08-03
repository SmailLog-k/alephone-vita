# Aleph One PS Vita

This repository provides a native PlayStation Vita port of the Aleph One engine, targeting the Marathon Trilogy and compatible Aleph One scenarios.

## Current status

Playable real-hardware prototype.

- Marathon 1 is playable and currently undergoing longer compatibility testing.
- Marathon 2 is playable and currently undergoing longer compatibility testing.
- Marathon Infinity is playable and currently undergoing longer compatibility testing.
- The release model is three standalone game VPKs built from one shared Aleph One Vita engine codebase.
- Software rendering is used; OpenGL and Lua HUD are disabled on Vita for performance.
- Fullscreen 960x544 output is supported.
- Target performance is 30 FPS. Current testing on real PS Vita hardware shows playable performance across the Marathon Trilogy, with scene-dependent variation.
- PlayStation Vita controls are adapted and suitable for gameplay.
- Networking and some desktop-only integrations are not currently part of the Vita target.
- Underwater/liquid tint effects use a Vita-specific renderer overlay to avoid the original software-renderer frame-rate drop.

This is not yet a fully polished release. The project is currently under active development and longer gameplay testing.

## Photos

<p align="center">
 <img src="https://github.com/user-attachments/assets/9772bfdd-9632-428a-9b4e-769711993edb" width="48%" alt="Marathon 1 on PS Vita">
 <img src="https://github.com/user-attachments/assets/11df0556-6d9f-490e-8062-7aac3798cf4d" width="48%" alt="Marathon 2 on PS Vita">
</p>

## Controls

Current Vita mapping:

| Vita input | Action |
| --- | --- |
| Left stick | Move forward/back and strafe |
| Right stick | Turn/look |
| R | Primary fire |
| L | Secondary fire |
| Cross | Action / Use |
| Circle | Run |
| Left stick press | Run |
| Square | Automap |
| Triangle | Recenter camera / View action |
| D-pad | Weapon / Inventory / Map controls depending on game state |
| Select | Scores |

## Vita application profiles and game data

The Vita port is one engine codebase built into separate game VPKs. Each VPK contains the native Aleph One Vita engine plus a small profile selecting the Title ID, application name, data directory, preferences, saves, logs, and Vita compatibility behavior for that game.

Current release VPKs:

```text
alephone_vita_legacy.vpk     -> Marathon          -> ALEPH0001
alephone_vita_marathon2.vpk  -> Marathon 2        -> ALEPH0002
alephone_vita_infinity.vpk   -> Marathon Infinity -> ALEPH0003
```

Current game data layout on the Vita:

```text
ux0:data/AlephOne/                  -> Marathon 1 data, legacy layout
ux0:data/AlephOne/Marathon2/        -> Marathon 2 data
ux0:data/AlephOne/MarathonInfinity/ -> Marathon Infinity data
```

Marathon 1 uses the legacy root directory `ux0:data/AlephOne/`. This is intentional for the current stable M1 profile.

If your game data comes from the Steam **Classic Marathon** release, copy the following files:

```text
E:\SteamLibrary\steamapps\common\Classic Marathon
```

to:

```text
ux0:data/AlephOne/
```

Required Marathon 1 files:

```text
Map.scen
Shapes.shps
Sounds.sndz
Physics.phys
Marathon.appl
Music/
Plugins/
Scripts/
```

Do not copy Steam executables or Steam runtime files such as `.exe` files or `steam_api64.dll`.

For Marathon 2, copy the original data to `ux0:data/AlephOne/Marathon2/`. If using Steam's **Classic Marathon 2** release, copy these files and folders as-is:

```text
Map.sceA
Shapes.shpA
Images.imgA
Sounds.sndA
Music.ogg
Scripts/
Plugins/
Physics Models/
Demos/
```

For Marathon Infinity, copy the original data to `ux0:data/AlephOne/MarathonInfinity/`. If using Steam's **Classic Marathon Infinity** release, copy these files and folders as-is:

```text
Map.sceA
Shapes.shpA
Images.imgA
Sounds.sndA
Music.ogg
Scripts/
Plugins/
Physics Models/
Demos/
```

Users must provide original game data from a legally obtained copy or from a legal Aleph One scenario distribution. Commercial game assets are not included in this repository or in the engine VPKs.

## Installation on PS Vita

1. Install the desired game VPK with VitaShell.
2. Copy the original game data to the matching directory under `ux0:data/AlephOne/`.
3. Launch the corresponding LiveArea bubble.

Game VPKs are installed directly through VitaShell.

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

./build-vita-vpk.sh pkg legacy
./build-vita-vpk.sh pkg marathon2
./build-vita-vpk.sh pkg infinity
```

To build all three game VPKs:

```bash
./build-vita-release.sh pkg-release
```

The build outputs are written to:

```text
pkg/alephone_vita_legacy.vpk
pkg/alephone_vita_marathon2.vpk
pkg/alephone_vita_infinity.vpk
pkg/eboot.bin
```

Release helper outputs are written to:

```text
pkg-release/games/alephone_vita_legacy.vpk
pkg-release/games/alephone_vita_marathon2.vpk
pkg-release/games/alephone_vita_infinity.vpk
```

For iterative testing on an already-installed application, upload only `pkg/eboot.bin` to the corresponding Title ID:

```text
Marathon 1:        ux0:/app/ALEPH0001/eboot.bin
Marathon 2:        ux0:/app/ALEPH0002/eboot.bin
Marathon Infinity: ux0:/app/ALEPH0003/eboot.bin
```

## Development notes

See:

- [docs/VITA_BUILD.md](docs/VITA_BUILD.md) for detailed build and deployment instructions.
- [PORT_STATUS.md](PORT_STATUS.md) for the current status of the port.
- [PORT_CHANGES.md](PORT_CHANGES.md) for a high-level overview of Vita-specific engine changes.

## Upstream

Aleph One upstream:

- Website: https://alephone.lhowon.org
- Source: https://github.com/Aleph-One-Marathon/alephone

This fork preserves upstream authorship and licensing.

## License

Aleph One is licensed under the GNU General Public License version 3. See [COPYING](COPYING).

Game names, trademarks, and game data remain the property of their respective owners and are not included in this repository.
