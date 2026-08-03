# Aleph One PS Vita

This repository provides a native PlayStation Vita port of the Aleph One engine, targeting the Marathon Trilogy and compatible Aleph One scenarios.

## Current status

Playable real-hardware prototype.

- Marathon 1 is playable and currently undergoing active compatibility testing.
- Marathon 2 is playable and currently undergoing active compatibility testing.
- Marathon Infinity is playable and currently undergoing active compatibility testing.
- Software rendering is used; OpenGL and Lua HUD are disabled on Vita for performance.
- Fullscreen 960×544 output is supported.
- Target performance is 30 FPS. Current testing on real PS Vita hardware shows approximately 23–30 FPS depending on the scene and HUD state.
- PlayStation Vita controls are fully adapted and suitable for gameplay.
- Networking and some desktop-only integrations are not currently part of the Vita target.
- Underwater/liquid tint effects use a Vita-specific renderer overlay to avoid the original software-renderer frame-rate drop.

This is not yet a fully polished release. The project is currently under active development and testing.

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

The Vita port is built as a single engine codebase with separate application profiles for each game. Each profile provides its own LiveArea bubble, Title ID, data directory, preferences directory, save directory, quick-save directory, and log directory.

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

Depending on the scenario, additional files may be required.

Users must provide the original game data from a legally obtained copy or from a legal Aleph One scenario distribution.

This engine repository may include PlayStation Vita compatibility fixes required to make supported Aleph One scenarios run correctly on the console. Commercial game assets are not included in this repository.

The intended release model is a single engine codebase with separate VPK packages for each game. Each game's data should be stored in its own `ux0:data/AlephOne/<Game>/` directory.

Game-specific adaptation files are intended to be handled separately from the engine. A future installer/update bubble may check user-provided game data, create required directories, and apply only redistributable Vita compatibility files or legal patch instructions. The profile mapping in `build-vita-vpk.sh` is the current source of truth for Title IDs and data directories, so other developers can build compatible installer or updater applications without bundling original game assets.

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
