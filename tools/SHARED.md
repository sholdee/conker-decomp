# Wave 2: shared headers (cw tree workspaces)

Wave 1 cleaned each file on its own, so shared declarations were out of scope. As a result, many files
carry file-local partial views of shared structs (for example `Camera` over struct108, or `Actor` and
`CamPlayer` over struct127), offset macros into header padding, `#define X X_hdr` tricks around a wrong
prototype in functions.h, and casts caused by a wrong global type in variables.h. Wave 2 fixes these
centrally. Each agent owns **one** shared area across the whole tree. The ROM stays byte-identical, exactly
as before.

## Loop
```
python3 ~/conker/tools/cw.py new --tree ITEM      # workspace ~/conker-ws/clean.ITEM (whole tree editable)
#   edit conker/include/*.h and any conker/src/*.c in the workspace
python3 ~/conker/tools/cw.py check clean.ITEM     # MATCH | MISMATCH (+ every function in the ROM whose bytes moved)
python3 ~/conker/tools/cw.py land  clean.ITEM -m "what changed"
```
- Change the header, then convert a few files at a time, check, and **land after every few files**. Other
  agents own other shared areas and edit some of the same files in parallel, so small lands keep merges
  trivial.
- On a merge conflict: copy main's current version of each conflicting file
  (`~/conker/conker/<path>`) over both your workspace copy and `base/<path>`, re-apply your edit to it,
  then check and land.
- Work only inside your workspace (scratch files in `my/`). Never edit ~/conker or run git there.

## Evidence
- Wave-1 notes: `~/conker-ws/clean.*/notes.md`. Grep them for your struct or header. Agents recorded
  field meanings, offsets, wrong prototypes and wrong global types there.
- File-local views and offset macros in `src/*.c`: grep for your struct name, its offsets and the
  `_hdr` / `#define` tricks.
- `~/ext/conkers-bfd-decomp` (read-only) names many struct fields at the same offsets. Adopt its names
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
- If you run low on context: land what matches and write `~/conker-ws/clean.ITEM/notes.md` with what's
  done and what's next.

## Landed shared types (use these)
- `Vec3f` {x, y, z} and `MtxF` (f32[4][4]) in include/structs.h. `struct17` and `vertex` are gone: write
  `Vec3f` and `.x/.y/.z` (not `.unk0/.unk4/.unk8`). If your workspace predates this, rebase the conflicting files.
- Header dependencies are tracked now (the Makefile generates `.d` files), and lands do a full rebuild,
  so you no longer need to touch every `.c` file after a header edit.
