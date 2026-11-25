# Wave 3: naming exported symbols

Agents **propose** names. The coordinator applies them centrally with `tools/rename.py`, which rewrites the
splat symbol file, every source and header, re-runs splat and gates on the ROM sha1. Never rename symbols
yourself in the tree.

## Conventions (decided by the project owner)
- **Functions:** `prefix_camelCase`. The prefix is the subsystem of the file that defines the function,
  from `tools/subsystems.tsv`, e.g. `light_spawnPulse`, `camera_updateHeight`, `mp3_getBits`,
  `heap_alloc`, `math_distance3D`. Start the verb in lowercase: `get`, `set`, `update`, `draw`,
  `spawn`, `free`, `init`, `find`, `is`/`has` for predicates.
- **Globals and statics:** `g` + camelCase, e.g. `gActors`, `gFrameDelta`, `gCameras`. Arrays are plural.
  Use no subsystem prefix unless it avoids ambiguity (`gLightPool`).
- **Only clear meanings.** Rename a symbol only when the code (its body, callers, data use, strings) makes
  its purpose clear. If you're unsure, leave the address name; a wrong name is worse than `func_XXXXXXXX`.
  Don't rename libultra/SDK functions that already have their SDK names.
- Names must be unique across the whole tree and must not clash with existing identifiers (types,
  fields, macros, libultra).

## Evidence to use
- The cleaned code itself (waves 1–2), and the wave notes in `~/conker-ws/clean.*/notes.md`. Grep them
  for the symbol; agents left many naming suggestions there, e.g. "func_150AD960 is a 2D distance".
- `~/ext/conkers-bfd-decomp/config/symbols/*.txt` and its sources (read-only); adopt its names by address
  where they fit.
- Original source paths that survive in ROM strings: `../Effects/Blood/blood.c`,
  `../Effects/CharacterFlameThrower/characterflamethrower.c`,
  `../Effects/ColourFrameBuffer/colourframebuffer.c`, `../Effects/Light/light.c`,
  `../Effects/PipeExplode/pipeexplode.c`.

## Output (proposals)
One TSV line per proposed rename: `old<TAB>new<TAB>confidence(high|medium)<TAB>one-line evidence`.
Only `high` lines are applied directly. `medium` lines are reviewed.
