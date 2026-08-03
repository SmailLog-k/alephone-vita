#!/usr/bin/env bash
set -euo pipefail

VITASDK="${VITASDK:-/usr/local/vitasdk}"
export VITASDK
export PATH="$VITASDK/bin:$PATH"

project_dir="$(cd "$(dirname "$0")" && pwd)"
output_dir="${1:-$project_dir/pkg-release}"
game_output_dir="$output_dir/games"

mkdir -p "$game_output_dir"

"$project_dir/build-vita-vpk.sh" "$game_output_dir" legacy
"$project_dir/build-vita-vpk.sh" "$game_output_dir" marathon2
"$project_dir/build-vita-vpk.sh" "$game_output_dir" infinity

echo
echo "Release artifacts:"
echo "  $game_output_dir/alephone_vita_legacy.vpk"
echo "  $game_output_dir/alephone_vita_marathon2.vpk"
echo "  $game_output_dir/alephone_vita_infinity.vpk"
echo
echo "Original game data is not included. Install VPKs with VitaShell and place game data under ux0:data/AlephOne/."
