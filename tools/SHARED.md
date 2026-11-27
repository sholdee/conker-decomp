# Wave 2: shared headers (cw tree workspaces)

Wave 1 cleaned each file on its own, so shared declarations were out of scope. As a result, many files
carry file-local partial views of shared structs (for example `Camera` over struct108, or `Actor` and
`CamPlayer` over struct127), offset macros into header padding, `#define X X_hdr` tricks around a wrong
prototype in functions.h, and casts caused by a wrong global type in variables.h. Wave 2 fixes these
centrally. Each agent owns **one** shared area across the whole tree. The ROM stays byte-identical, exactly
as before.

## Loop
```
python3 tools/cw.py new --tree ITEM      # workspace $CONKER_WS/clean.ITEM (whole tree editable)
#   edit conker/include/*.h and any conker/src/*.c in the workspace
python3 tools/cw.py check clean.ITEM     # MATCH | MISMATCH (+ every function in the ROM whose bytes moved)
python3 tools/cw.py land  clean.ITEM -m "what changed"
```
- Change the header, then convert a few files at a time, check, and **land after every few files**. Other
  agents own other shared areas and edit some of the same files in parallel, so small lands keep merges
  trivial.
- On a merge conflict: copy main's current version of each conflicting file
  (`conker/<path>` in the main tree) over both your workspace copy and `base/<path>`, re-apply your edit to it,
  then check and land.
- Work only inside your workspace (scratch files in `my/`). Never edit the main tree or run git there.

## Evidence
- Comments in the code: grep `src/` and `include/` for your struct or header. Earlier waves recorded
  field meanings, offsets, wrong prototypes and wrong global types in comments and in the git log.
- File-local views and offset macros in `src/*.c`: grep for your struct name, its offsets and the
  `_hdr` / `#define` tricks.
- `$CONKER_REF` (your optional clone of DevOldSchool/conkers-bfd-decomp, read-only) names many struct fields at the same offsets. Adopt its names
  where the code supports them.

## Rules
- **Never change a shared struct's size or field offsets.**
  - Name fields, split padding into typed fields, and give fields real types (`f32` for a float field,
    pointer types for pointers).
  - Keep an offset comment on every field: `/* 0x2F4 */`.
  - `unkXX` stays where the meaning is unknown.
- **Type renames are allowed** (e.g. struct127 → `Actor`), across the whole tree in one land. Keep the
  old name only if removing it is impractical; never leave two different definitions.
- **Then convert the users.** Replace each file-local partial view, offset macro and cast with the shared
  type and field.
  - Delete duplicate local typedefs.
  - If a local view's layout disagrees with the shared struct, check which is right against the code;
    `check` will tell you if a change moves bytes.
- **Prototypes and global types (functions.h / variables.h items).**
  - Make each declaration match the definition and its real use, then remove the `#define X X_hdr` tricks
    and casts that worked around it.
  - Changing a prototype can change caller code (argument promotion, return-value handling), so check
    after every few changes. If the correct type mismatches a caller, keep the caller's workaround with a
    `// matching:` comment.
- **Out of scope:** renaming exported symbols (`func_XXXXXXXX`, `D_XXXXXXXX`). That is wave 3, a central
  rename. rodata_extern derives addresses from `D_<hex>` names.
- If you run low on context: land what matches and write `$CONKER_WS/clean.ITEM/notes.md` with what's
  done and what's next.

## Landed shared types (use these)
- `Vec3f` {x, y, z} and `MtxF` (f32[4][4]) in include/structs.h. `struct17` and `vertex` are gone: write
  `Vec3f` and `.x/.y/.z` (not `.unk0/.unk4/.unk8`). If your workspace predates this, rebase the conflicting files.
- Header dependencies are tracked now (the Makefile generates `.d` files), and lands do a full rebuild,
  so you no longer need to touch every `.c` file after a header edit.

## Wave 3a: names for types and fields (cw tree workspaces)

Same loop, rules and gate as wave 2. Now the goal is names: give struct fields, struct types and enums
their real names, tree-wide.

**Original names come first.** Rare's struct browser in the debug prototype builds names about 480
fields of our structs. An example is the October 2000 debug ROM (`conker.debug.yaml`): the browser is
in its debugger segment, which starts at ROM 0x189840 (source name `rom_debug_struct.c`). The names
are recorded in [docs/original-names](../docs/original-names/README.md), with each field mapped to its
US struct and offset. Each browser entry gives a field name and its offset in the Oct 2000 layout. Check
every offset against US code: some layouts shifted between Oct 2000 and US, and the viewer has bugs.
Many original names are already in the headers.
- **Keep original names exactly as Rare wrote them** (`stand_on_char`, `AimTargetOn`, `FCameraIndex`,
  `xpos`): same spelling and case, no prefix, no camelCase conversion. This is the owner's decision.
- Apply an original name when the code agrees with it (offset, type and use).
- An original name that is not a valid C identifier (`touch.i.a`, `dst_fade_alpha[1]`, `MCache[NORMAL]`)
  describes a nested struct, union or array: model that layout (keeping size and offsets) and use the
  original names for its parts.
- An original name beats our earlier guess, even when ours reads well. Keep our guess only where the
  original row is wrong for US (a layout shift); say so in a short comment.
- The same debugger also names original enums (camera modes, zone types, task flags and ids, level
  ids; `docs/original-names/enums.txt`). They are Oct 2000 orderings: check each value against US code
  before you use it. Name the enumerators in UPPER_CASE, keeping Rare's words (`CAMERA_MODE_NORMAL`).
- Struct type names stay ours (`Actor`, `Camera`, ...). Note the original viewer title in the struct's
  comment, for example `/* original: movement_move (rom_debug_struct.c) */`.

**Other fields and types:** name an `unkXX` field or a `structNNN` type only when the code makes its
meaning clear (same rule as RENAMING.md). Use camelCase for these names, since there is no original to
keep. An honest `unk` beats a wrong guess.

**Exported symbols are still central.** Don't rename `func_`/`D_` symbols yourself. Write proposals to
`$CONKER_WS/clean.ITEM/renames.tsv` (a file you create) as `old<TAB>new<TAB>confidence<TAB>evidence`, and the coordinator
applies them with `tools/rename.py`. A new name that clashes with an existing local or field (for example
`level` or `zones`) is refused: rename the clashing locals or fields in your workspace first, land that,
then propose the global.
