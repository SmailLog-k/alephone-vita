# Building Aleph One for PS Vita

These instructions describe the WSL/Linux VitaSDK workflow used by Aleph One Vita.

## Requirements

- WSL or Linux
- VitaSDK installed
- `arm-vita-eabi` toolchain in `PATH`
- Vita packaging tools:
  - `vita-elf-create`
  - `vita-make-fself`
  - `vita-mksfoex`
  - `vita-pack-vpk`
- Autotools:
  - `autoconf`
  - `automake`
  - `libtool`

Default expected VitaSDK path:

```bash
/usr/local/vitasdk
```

## Configure

From a clean clone:

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
```

`--disable-opengl` is intentional. The current Vita release uses the software renderer as its supported rendering path.

## Build VPKs

Build individual profiles:

```bash
./build-vita-vpk.sh pkg legacy
./build-vita-vpk.sh pkg marathon2
./build-vita-vpk.sh pkg infinity
```

Outputs:

```text
pkg/alephone_vita_legacy.vpk
pkg/alephone_vita_marathon2.vpk
pkg/alephone_vita_infinity.vpk
pkg/eboot.bin
```

Build all three release VPKs:

```bash
./build-vita-release.sh pkg-release
```

Release helper outputs:

```text
pkg-release/games/alephone_vita_legacy.vpk
pkg-release/games/alephone_vita_marathon2.vpk
pkg-release/games/alephone_vita_infinity.vpk
```

## Profile model

All three game applications are built from one shared engine codebase.

Profiles select game-specific Vita configuration such as:

- Title ID;
- application name;
- data directory;
- preferences and saves;
- logs;
- artwork and LiveArea assets;
- Vita compatibility behavior.

Profile Title IDs:

```text
legacy     -> ALEPH0001
marathon2  -> ALEPH0002
infinity   -> ALEPH0003
```

## Rebuild after source changes

For normal development, build the profile that matches the installed Vita app.

For Marathon, use the stable legacy profile:

```bash
./build-vita-vpk.sh pkg legacy
```

The script removes the previous final executable before linking so source changes inside static libraries are picked up by `pkg/eboot.bin`.

For Marathon 2 and Marathon Infinity:

```bash
./build-vita-vpk.sh pkg marathon2
./build-vita-vpk.sh pkg infinity
```

## Diagnostic performance builds

Diagnostic profiling can be enabled explicitly:

```bash
VITA_PROFILE_PERF=1 ./build-vita-vpk.sh pkg legacy
```

Normal release builds should leave `VITA_PROFILE_PERF` unset so the FPS overlay and `vita_perf.log` writes remain disabled.

## Deploy to an installed Vita app

If the VPK is already installed and VitaShell FTP is running, replace `VITA_IP` with your device address:

```bash
export VITA_IP=192.0.2.10

curl --ftp-method nocwd \
  -T pkg/eboot.bin \
  "ftp://$VITA_IP:1337/ux0:/app/ALEPH0001/eboot.bin"
```

`192.0.2.10` is only an example placeholder.

For iterative testing, upload the matching `eboot.bin` to the corresponding installed Title ID.

## Runtime data

Install game data separately to the matching profile directory:

```text
ux0:data/AlephOne/
ux0:data/AlephOne/Marathon2/
ux0:data/AlephOne/MarathonInfinity/
```

Marathon intentionally uses the legacy root path `ux0:data/AlephOne/`.

Required files are scenario/profile-specific. See the top-level `README.md` for the currently tested Steam Classic Marathon Trilogy layouts.

Do not commit commercial or scenario game data to this engine repository. Use game data from a legally obtained copy or from a legal Aleph One scenario distribution.

## Optional Russian translation data

The Vita engine supports Cyrillic text rendering.

Optional Russian terminal-text translation data is loaded from a `Russian/` folder inside the matching game data directory:

```text
ux0:data/AlephOne/Russian/
ux0:data/AlephOne/Marathon2/Russian/
ux0:data/AlephOne/MarathonInfinity/Russian/
```

Original game data is still required.

## Release checklist

Before publishing a release build:

- leave `VITA_PROFILE_PERF` unset unless intentionally publishing a diagnostic build;
- verify that no original scenario data or generated development binaries are committed;
- test launch from a fresh Vita install;
- test each Marathon Trilogy profile with its own data layout;
- verify the correct LiveArea icon and artwork for all three game VPKs;
- open and verify the built-in LiveArea manual for all three games;
- verify English and Russian manual pages;
- verify optional Russian terminal text with the corresponding `Russian/` folder;
- verify built-in level selection with `L + R + Cross`;
- perform long-session regression testing on real PS Vita hardware.

