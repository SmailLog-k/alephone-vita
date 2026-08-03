# Vita profiles

Profile manifests describe Aleph One scenarios that can be packaged as standalone PS Vita VPKs.

Each profile defines:

- `profile` - stable profile identifier.
- `display_order` - optional integer used to keep profiles in a consistent order.
- `title_id` - Vita Title ID used by the game VPK.
- `title` - LiveArea/application title.
- `data_dir` - expected user-provided game data directory on the Vita.
- `data_dir_is_base_dir` - true only for the current Marathon 1 legacy layout.
- `data_dir_fallbacks` - optional legacy paths accepted by tools.
- `vpk_profile` - profile argument passed to `build-vita-vpk.sh`.
- `required_file_groups` - required user-provided files. Each group may list alternative accepted filenames.
- `compat_layer_dir` - optional directory for redistributable Vita compatibility files or patch metadata.

Current data layout:

```text
Marathon:          ux0:data/AlephOne/
Marathon 2:        ux0:data/AlephOne/Marathon2/
Marathon Infinity: ux0:data/AlephOne/MarathonInfinity/
```

Marathon 1 currently uses `ux0:data/AlephOne/` and the `legacy` VPK profile because this is the current stable Vita layout. The Marathon 1 manifest accepts both plain Aleph One names such as `Map` and original/typed names such as `Map.scen`.
