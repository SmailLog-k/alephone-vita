#!/usr/bin/env bash
set -euo pipefail

VITASDK="${VITASDK:-/usr/local/vitasdk}"
export VITASDK
export PATH="$VITASDK/bin:$PATH"

project_dir="$(cd "$(dirname "$0")" && pwd)"
output_dir="${1:-$project_dir/pkg}"
title_id="ALEPH0001"
title="Aleph One (Marathon Trilogy)"

mkdir -p "$output_dir"
vita_perf_cflags="${VITA_PERF_CFLAGS:--O3 -fomit-frame-pointer -DNDEBUG}"
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

vita-pack-vpk \
    -s "$output_dir/param.sfo" \
    -b "$output_dir/eboot.bin" \
    "$output_dir/alephone_vita.vpk"

unzip -t "$output_dir/alephone_vita.vpk"
sha256sum "$output_dir/alephone_vita.vpk"
