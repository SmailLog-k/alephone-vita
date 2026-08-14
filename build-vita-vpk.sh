#!/usr/bin/env bash
set -euo pipefail

VITASDK="${VITASDK:-/usr/local/vitasdk}"
export VITASDK
export PATH="$VITASDK/bin:$PATH"

project_dir="$(cd "$(dirname "$0")" && pwd)"
output_dir="${1:-$project_dir/pkg}"
profile="${A1_VITA_PROFILE:-${2:-legacy}}"

case "$profile" in
    marathon1|m1)
        profile="marathon1"
        title_id="ALEPH0001"
        title="Marathon"
        data_dir="ux0:data/AlephOne/Marathon"
        profile_id="marathon1"
        asset_profile="marathon1"
        m1_cockpit=1
        ;;
    marathon2|m2)
        profile="marathon2"
        title_id="ALEPH0002"
        title="Marathon 2"
        data_dir="ux0:data/AlephOne/Marathon2"
        profile_id="marathon2"
        asset_profile="marathon2"
        m1_cockpit=0
        ;;
    infinity|marathon-infinity|mi)
        profile="infinity"
        title_id="ALEPH0003"
        title="Marathon Infinity"
        data_dir="ux0:data/AlephOne/MarathonInfinity"
        profile_id="marathon-infinity"
        asset_profile="infinity"
        m1_cockpit=0
        ;;
    legacy|trilogy)
        profile="legacy"
        title_id="ALEPH0001"
        title="Marathon"
        data_dir="ux0:data/AlephOne"
        profile_id="legacy"
        asset_profile="marathon1"
        m1_cockpit=1
        ;;
    *)
        echo "Unknown Vita profile: $profile" >&2
        echo "Valid profiles: marathon1, marathon2, infinity, legacy" >&2
        exit 2
        ;;
esac

mkdir -p "$output_dir"
profile_header="$project_dir/VitaPlatform/vita_build_profile.h"
profile_header_new="$profile_header.tmp"
cat > "$profile_header_new" <<EOF
#ifndef VITA_BUILD_PROFILE_H
#define VITA_BUILD_PROFILE_H

#define A1_VITA_PROFILE_ID "$profile_id"
#define A1_VITA_TITLE_ID "$title_id"
#define A1_VITA_TITLE "$title"
#define A1_VITA_DATA_DIR "$data_dir"
#define A1_VITA_LOG_DIR "$data_dir/logs"
#define A1_VITA_M1_COCKPIT $m1_cockpit

#endif
EOF
if ! cmp -s "$profile_header_new" "$profile_header"; then
    mv "$profile_header_new" "$profile_header"
    rm -f \
        "$project_dir/VitaPlatform/"*.o \
        "$project_dir/Source_Files/CSeries/cspaths_sdl.o" \
        "$project_dir/Source_Files/Files/FileHandler.o" \
        "$project_dir/Source_Files/Misc/interface.o" \
        "$project_dir/Source_Files/RenderOther/screen.o" \
        "$project_dir/Source_Files/RenderOther/game_window.o" \
        "$project_dir/Source_Files/RenderOther/screen_drawing.o" \
        "$project_dir/Source_Files/CSeries/libcseries.a" \
        "$project_dir/Source_Files/Files/libfiles.a" \
        "$project_dir/Source_Files/Misc/libmisc.a" \
        "$project_dir/Source_Files/RenderOther/librenderother.a" \
        "$project_dir/Source_Files/alephone"
else
    rm -f "$profile_header_new"
fi

vita_perf_cflags="${VITA_PERF_CFLAGS:--O3 -fomit-frame-pointer -DNDEBUG}"
if [ "${VITA_TEST_INVINCIBLE:-0}" = "1" ]; then
  vita_perf_cflags="$vita_perf_cflags -DA1_VITA_TEST_INVINCIBLE=1"
fi
if [ "${VITA_PROFILE_PERF:-0}" = "1" ]; then
  vita_perf_cflags="$vita_perf_cflags -DA1_VITA_PROFILE_PERF=1"
fi
rm -f "$project_dir/Source_Files/alephone"
rm -f "$project_dir/Source_Files/GameWorld/player.o"
make -C "$project_dir" -j"$(nproc)" CFLAGS="$vita_perf_cflags" CXXFLAGS="$vita_perf_cflags"

vita-elf-create \
    "$project_dir/Source_Files/alephone" \
    "$output_dir/alephone.velf"

vita-make-fself -c \
    "$output_dir/alephone.velf" \
    "$output_dir/eboot.bin"

vita-mksfoex -s "TITLE_ID=$title_id" \
    "$title" \
    "$output_dir/param.sfo"

vpk_args=(
    -s "$output_dir/param.sfo"
    -b "$output_dir/eboot.bin"
)

asset_dir="$project_dir/VitaAssets/$asset_profile"
if [ -d "$asset_dir/sce_sys" ]; then
    vpk_args+=(-a "$asset_dir/sce_sys=sce_sys")
fi

vita-pack-vpk \
    "${vpk_args[@]}" \
    "$output_dir/alephone_vita_${profile}.vpk"

unzip -t "$output_dir/alephone_vita_${profile}.vpk"
sha256sum "$output_dir/alephone_vita_${profile}.vpk"
