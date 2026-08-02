# Building Aleph One for PS Vita

These instructions describe the current WSL/Linux VitaSDK workflow used by this fork.

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

## Build VPK

```bash
./build-vita-vpk.sh pkg marathon1
./build-vita-vpk.sh pkg marathon2
./build-vita-vpk.sh pkg infinity
```

Outputs:

```text
pkg/alephone_vita_marathon1.vpk
pkg/alephone_vita_marathon2.vpk
pkg/alephone_vita_infinity.vpk
pkg/eboot.bin
```

## Rebuild after source changes

For normal development:

```bash
./build-vita-vpk.sh pkg marathon1
```

The script removes the previous final executable before linking so source
changes inside static libraries are picked up by `pkg/eboot.bin`.

Use the profile name that matches the installed Vita app.

## Deploy to an installed Vita app

If the VPK is already installed and VitaShell FTP is running, replace `VITA_IP` with your device address:

```bash
export VITA_IP=192.0.2.10

curl --ftp-method nocwd \
  -T pkg/eboot.bin \
  "ftp://$VITA_IP:1337/ux0:/app/ALEPH0001/eboot.bin"
```

`192.0.2.10` is only an example placeholder.

Profile TitleIDs:

```text
marathon1  -> ALEPH0001
marathon2  -> ALEPH0002
infinity   -> ALEPH0003
```

## Runtime data

Install game data separately to the matching profile directory:

```text
ux0:data/AlephOne/Marathon/
ux0:data/AlephOne/Marathon2/
ux0:data/AlephOne/MarathonInfinity/
```

At minimum, the selected scenario must provide:

```text
Map
Shapes
Images
Sounds
```

Do not commit these files to this engine repository. Use game data from a
legally obtained copy or from a legal Aleph One scenario distribution.

## Release notes

Before publishing a release build:

- disable or make optional the FPS overlay;
- disable or gate profiling log writes;
- verify that no scenario data or generated binaries are included;
- test launch from a fresh Vita install;
- test with each Marathon Trilogy scenario layout separately.
