# Aleph One Vita — Native PS Vita Marathon Trilogy Port

Aleph One Vita is a native PlayStation Vita port of the open-source Aleph One engine for **Marathon**, **Marathon 2: Durandal**, and **Marathon Infinity**.

The current release uses one shared Vita-optimized engine codebase to provide three standalone LiveArea game applications, each with its own Title ID, preferences, saves, logs, data directory, artwork, and built-in manual.

**Русский язык:** [README на русском](README.ru.md). Для всех трёх игр доступен необязательный русский перевод всех сюжетных текстов терминалов.

## Release status

The core Vita port implementation for the current release scope is complete. Remaining work is focused on long-form playthrough validation and release QA.

- **Marathon** is playable and has been completed 100% on real PS Vita hardware.
- **Marathon 2: Durandal** is playable on real PS Vita hardware. Its levels load and play correctly through the built-in level selection screen; a full linear playthrough is still in progress.
- **Marathon Infinity** is playable on real PS Vita hardware. Its levels load and play correctly through the built-in level selection screen; a full linear playthrough is still in progress.
- Three standalone game VPKs are built from one shared Aleph One Vita engine codebase.
- Fullscreen **960x544 widescreen** output is supported.
- The Vita release uses a performance-optimized software rendering path with the classic HUD/cockpit adapted for the console.
- Previously identified persistent HUD and underwater/liquid frame-time drops have been removed in the current tested Vita paths.
- Classic HUD/cockpit, automap, terminal, menu, and full-screen fade rendering paths are adapted for Vita.
- Built-in level selection is available from the main menu with `L + R + Cross` for testing and replay.
- Cyrillic text rendering support is included in the Vita engine.
- An optional Russian translation is available for all three games and covers all story terminal text.
- Bilingual English/Russian LiveArea manuals are included for all three game VPKs.
- PlayStation Vita controls are fully mapped for single-player gameplay.

This release contains the engine and Vita-specific assets only. **Original game data is not included.**

## Rendering and HUD

The supported Vita configuration intentionally uses Aleph One's software renderer and a Vita-optimized classic HUD/cockpit path.

The desktop Lua/Enhanced HUD renderer is not used on Vita because it produced severe frame-time spikes on the console. Instead, the classic gameplay HUD is preserved through a dedicated Vita rendering path, including optimized cockpit redraws and Vita-specific presentation fixes.

The original persistent liquid fade path also caused major performance drops when fully underwater. The Vita build replaces that path with a lightweight renderer overlay during final presentation.

OpenGL is not used by the current Vita release; the software renderer is the supported Vita rendering path.

## Photos

<p align="center">
 <img src="https://github.com/user-attachments/assets/9772bfdd-9632-428a-9b4e-769711993edb" width="48%" alt="Marathon on PS Vita">
 <img src="https://github.com/user-attachments/assets/11df0556-6d9f-490e-8062-7aac3798cf4d" width="48%" alt="Marathon 2 on PS Vita">
</p>

## Controls

Current Vita mapping:

| Vita input | Action |
| --- | --- |
| Left stick | Move forward/back and strafe |
| Right stick | Turn / look |
| R | Primary fire |
| L | Secondary fire |
| Cross | Action / Use |
| Circle | Run |
| Square | Automap / minimap |
| Triangle | Center camera |
| D-pad | Weapon / inventory / map controls depending on game state |
| Start | Quit current game to the main menu (with confirmation) |

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
ux0:data/AlephOne/                  -> Marathon data, legacy layout
ux0:data/AlephOne/Marathon2/        -> Marathon 2 data
ux0:data/AlephOne/MarathonInfinity/ -> Marathon Infinity data
```

Marathon uses the legacy root directory `ux0:data/AlephOne/`. Do not create an additional `/Marathon/` subdirectory.

### Marathon

If your game data comes from the Steam **Classic Marathon** release, copy the following files from:

```text
<SteamLibrary>/steamapps/common/Classic Marathon
```

to:

```text
ux0:data/AlephOne/
```

Required Marathon files:

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

### Marathon 2: Durandal

Copy the original data to:

```text
ux0:data/AlephOne/Marathon2/
```

If using Steam's **Classic Marathon 2** release, copy these files and folders as-is:

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

### Marathon Infinity

Copy the original data to:

```text
ux0:data/AlephOne/MarathonInfinity/
```

If using Steam's **Classic Marathon Infinity** release, copy these files and folders as-is:

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

## Optional Russian translation

Aleph One Vita includes Cyrillic text rendering support and optional Russian translation layers for the complete Marathon Trilogy.

The provided Russian translation covers **all story terminal text**. It is separate from the engine VPKs and does not replace the original game data.

Copy the provided `Russian/` folder to the matching game data directory:

```text
Marathon:          ux0:data/AlephOne/Russian/
Marathon 2:        ux0:data/AlephOne/Marathon2/Russian/
Marathon Infinity: ux0:data/AlephOne/MarathonInfinity/Russian/
```

Original game data is still required.

## Installation on PS Vita

1. Install the desired game VPK with VitaShell.
2. Copy the original game data to the matching directory under `ux0:data/AlephOne/`.
3. Optional: copy the provided `Russian/` translation folder to the same game data directory.
4. Launch the corresponding LiveArea bubble.
5. Open the game's LiveArea manual for installation notes, controls, troubleshooting, and Russian-language instructions.

The three games are installed directly as standalone VPKs.

## Additional Aleph One scenarios

The shared profile architecture is designed so additional compatible Aleph One scenarios can be added as dedicated Vita profiles in future releases.

The current release packages and validates the official Marathon Trilogy profiles.

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
pkg/alephone_vita_<profile>.vpk
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
Marathon:          ux0:/app/ALEPH0001/eboot.bin
Marathon 2:        ux0:/app/ALEPH0002/eboot.bin
Marathon Infinity: ux0:/app/ALEPH0003/eboot.bin
```

## Development notes

See:

- [docs/VITA_BUILD.md](docs/VITA_BUILD.md) for detailed build and deployment instructions.
- [PORT_STATUS.md](PORT_STATUS.md) for current validation status.
- [PORT_CHANGES.md](PORT_CHANGES.md) for a high-level overview of Vita-specific engine changes.
- [README.ru.md](README.ru.md) for the Russian README.
- [PORT_STATUS.ru.md](PORT_STATUS.ru.md) for the Russian status document.
- [PORT_CHANGES.ru.md](PORT_CHANGES.ru.md) for the Russian port-changes document.
- [docs/VITA_BUILD.ru.md](docs/VITA_BUILD.ru.md) for the Russian build guide.

## Upstream

Aleph One upstream:

- Website: https://alephone.lhowon.org
- Source: https://github.com/Aleph-One-Marathon/alephone

This fork preserves upstream authorship and licensing.

## License

Aleph One is licensed under the GNU General Public License version 3. See [COPYING](COPYING).

Game names, trademarks, and game data remain the property of their respective owners and are not included in this repository.

