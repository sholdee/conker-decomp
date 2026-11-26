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
1. **Original names.** The field names of Rare's debug struct viewer (`~/conker-ws/origfields/mapping.tsv`,
   with the rules in `tools/SHARED.md`), the source paths in ROM strings, and the debugger's labels.
2. **SDK names.** A function that is Rare's copy of a libultra function gets the SDK name.
3. **Descriptive names.** Use these where the code makes the meaning clear.
4. **The address name.** Keep it in all other cases.

Other sources of facts:
- The cleaned code and the notes of the earlier waves in `~/conker-ws/hdr/notes/`. Agents left many
  naming hints there, for example "func_150AD960 is a 2D distance".
- `~/ext/conkers-bfd-decomp` is a read-only reference. Use its facts (addresses, sizes). Never copy its
  code.

## File names and layout (the owner's decision)
- Files follow Rare's layout, as the five proven paths show: `src/game/<Area>/<Name>/<name>.c`. The
  file name is lowercase. `<Area>` and `<Name>` are CamelCase words, for example
  `game/Effects/PipeExplode/pipeexplode`.
- The five proven files are already in place under `src/game/Effects/`. Use the same pattern for the
  other files. Choose `<Area>` from the subsystem (for example `Effects`, `Camera`, `Collision`,
  `Audio`). Choose `<Name>` from what the file does.
- The root is the file's ROM segment: `game/` for the game code, `init/` for the init segment
  (`init/<Area>/<Name>/<name>.c`), and `debugger/` for the debugger segment.
- Each file name must be unique in `src/`. Leave the libultra files where they are.
- Rare's copies of SDK functions keep a separate step: their SDK names need matching prototypes in the
  SDK headers. Put such proposals in `symbols.tsv` as usual. The coordinator applies them after the
  prototypes are fixed.

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
