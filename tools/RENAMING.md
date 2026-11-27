# Naming wave: names for functions, globals, fields and files

Agents **propose** names, and a reviewer checks them. The coordinator applies the accepted names with
`tools/rename.py` and moves files with `tools/mvfile.py`. Both tools rebuild the ROM. If its sha1
still matches, they commit. If not, they restore everything. Never rename exported symbols or move files yourself.

## Conventions (decided by the project owner)
- **Functions:** `prefix_camelCase`. The prefix is the subsystem of the file that defines the function,
  from `tools/subsystems.tsv`. Examples: `light_spawnPulse`, `camera_updateHeight`, `mp3_getBits`,
  `heap_alloc`, `math_distance3D`. Start the verb in lowercase: `get`, `set`, `update`, `draw`,
  `spawn`, `free`, `init`, `find`, and `is` or `has` for predicates.
- **Globals and statics:** `g` and camelCase. Examples: `gActors`, `gFrameDelta`, `gCameras`. Arrays
  are plural. Use a subsystem prefix only to prevent ambiguity, for example `gLightPool`.
- **Only clear meanings.** The code must make the purpose clear: its body, its callers, its data use
  or its strings. If the purpose is not clear, keep the address name. A wrong name is worse than
  `func_XXXXXXXX`. Do not rename libultra or SDK functions that already have their SDK names.
- **Unique names.** A name must be unique in the whole tree. It must not clash with a type, a field, a
  macro or a libultra name.

## Evidence order
Use the first kind of evidence that exists:
1. **Original names.** The field names of Rare's debug struct viewer (recorded in
   [docs/original-names](../docs/original-names/README.md); see also `tools/SHARED.md`, section
   "Wave 3a"), the source paths in ROM strings, and the debugger's labels.
2. **SDK names.** A function that is Rare's copy of a libultra function gets the SDK name.
3. **Descriptive names.** Use these where the code makes the meaning clear.
4. **The address name.** Keep it in all other cases.

Other sources of facts:
- The cleaned code, its comments and the git log. Earlier waves left many naming hints there, for
  example "func_150AD960 is a 2D distance".
- `$CONKER_REF` (your optional clone of DevOldSchool/conkers-bfd-decomp; see "Paths" in AGENTS.md) is a
  read-only reference. Use its facts (addresses, sizes). Never copy its
  code.

## File names and layout (the owner's decision)
- Files follow Rare's layout, as the proven paths show: `src/game/<Area>/<Name>/<name>.c`. The
  file name is lowercase. `<Area>` and `<Name>` are CamelCase words, for example
  `game/Effects/PipeExplode/pipeexplode`.
- Nine files have a proven path, from path strings in the US ROM and in the prerelease builds. Each is
  at Rare's path under `src/game/`:
  - US ROM: `Effects/PipeExplode/pipeexplode.c`, `Effects/Blood/blood.c`,
    `Effects/ColourFrameBuffer/colourframebuffer.c`, `Effects/Light/light.c`,
    `Effects/CharacterFlameThrower/characterflamethrower.c`.
  - ECTS build (August 2000): `Effects/HoltenRope/holtenrope.c`,
    `Effects/Sfx/effects_sfx_w1_bee_cavern.c`, `Effects/Sight/effects_sight.c`.
  - Debug build (October 2000): `Camera/camera_camera.c`.

  Some of Rare's file names start with the directory names (`camera_camera.c`, `effects_sight.c`),
  and `camera_camera.c` is directly in its area directory. A file with a proven path keeps exactly
  that path. Rare had `Sight/` and `Trail/` under `Effects/` (`Effects/Sight/effects_sight.c`,
  `Effects/Trail/trail.c`), so the sight and trail files are in `game/Effects/Sight/` and
  `game/Effects/Trail/`. Use the `<Area>/<Name>/<name>.c` pattern for the
  other files. Choose `<Area>` from the subsystem (for example `Effects`, `Camera`, `Collision`,
  `Audio`). Choose `<Name>` from what the file does.
- The root is the file's ROM segment: `game/` for the game code, `init/` for the init segment
  (`init/<Area>/<Name>/<name>.c`), and `debugger/` for the debugger segment.
- Each file name must be unique in `src/`. Leave the libultra files where they are.
- **The audio library.** A file that is Rare's version of an SDK audio file has the SDK's file name and
  is in `src/libultra/audio/` (for example `n_seqplayer.c`, `n_event.c`, `bnkf.c`). Rare's own audio
  files are in `init/Audio/<Name>/<name>.c` (or `game/Audio/`) with descriptive names. Rare's own
  functions in the audio library have the `audio_` prefix (`mp3_` for the streaming player), never
  `n_al*`: that form is only for the SDK's names. Do not use the placeholder names of other
  decompilations for files or functions.
- Rare's copies of SDK functions keep a separate step: their SDK names need matching prototypes in the
  SDK headers. Put such proposals in `symbols.tsv` as usual. The coordinator applies them after the
  prototypes are fixed.

## Second pass (the owner's decisions)
The first pass named about two thirds of the functions. The second pass covers the rest:
- **Symbols:** the remaining `func_` functions, the `D_` data symbols and the `unk` fields. Many neighbours
  now have names, so use them as context. The rules do not change: if the meaning is not clear, keep the
  address name.
- **Hand-written asm files** (`*.s` at the top of `src/`) join Rare's layout beside the C of their
  subsystem, for example `game/Collision/Probe/probe.s`. The file name is lowercase, and the `.s` stays.
  Their functions get names by the same rules.
- **`src/game/done/`** is an old workflow folder, and it is removed in this pass. Give each of its files a
  path in Rare's layout. If no name is clear, propose the address name at the top of `src/`, for example
  `game/done/game_33010` to `game_33010`.
- **The address-named C files** at the top of `src/`: propose a path where their purpose is now clear.
  Otherwise they keep the address name.
- **Corrections.** Sometimes the code clearly contradicts a first-pass name. Then propose a correction
  in `symbols.tsv`, with the evidence. The reviewer checks it like a new name.

## Output (proposals)
- **Symbols:** one TSV line for each rename:
  `old<TAB>new<TAB>confidence(high|medium)<TAB>one-line evidence`.
- **Files:** one TSV line for each move: `old<TAB>new<TAB>one-line evidence`. The paths are under
  `conker/src`, without the extension, for example `game_C4DC0<TAB>game/Script/Interp/interp`.
- **Fields:** rename `unkXX` fields in a cw tree workspace, and land them yourself, as in Wave 3a.
  Fields are not exported symbols.

## Review
An independent reviewer reads the code for each proposal. The reviewer rejects a name that the code
does not support, and a `medium` name that is only a guess. The coordinator applies only the names that
the reviewer accepts.
