# Header consolidation: one declaration for each symbol, one definition for each type

**Goal.** Each function and each global has exactly one declaration, in a header. Each struct, union
and enum has exactly one definition. A C file keeps only its `static` declarations and the types that
only it uses. The ROM must stay byte-identical. The whole-ROM sha1 decides.

**Why.** The C files hold about 4,800 function prototypes and 3,200 `extern` lines that are in no
header. 132 type names have more than one definition (388 definitions in all, and `Particle` has 41).
Readers see this as repeated definitions.

## Layout
- One header for each subsystem: `include/conker/<prefix>.h`. All 58 exist. The comment at the top of
  each lists the files of that subsystem in address order. `tools/subsystems.tsv` maps each file to its
  subsystem.
- `include/subsystems.h` includes the 58 headers. `functions.h` and `variables.h` include it at their
  end. Thus each C file still sees every declaration. Do not add `#include "conker/..."` lines to C
  files.
- A subsystem header holds the prototypes of the non-static functions and the `extern` declarations of
  the globals that its files define. Group them by file, in address order, with a one-line comment that
  names the file.
- A type that only one subsystem uses goes into that subsystem's header. A type that several subsystems
  use goes into `include/structs.h`, or into `include/effects.h` for effect types. A type that
  `functions.h`, `variables.h` or `structs.h` needs must be in `structs.h`, because the subsystem
  headers come after them. A subsystem header includes the other headers that it needs.

## Phase 1: types
Each agent takes a family of type names. For each name:
1. Read every definition and the code that uses it.
2. Decide: if the definitions describe the same object, make one definition with all the known fields.
   Every layout must agree: the same size and the same offsets. If the definitions describe different
   objects that share a name, give each object its own name. Read the code to decide. Do not guess.
3. Put the definition in the correct header (see Layout). Remove the local copies from the C files.
4. **Original field names.** `~/conker-ws/origfields/mapping.tsv` has the field names of Rare's debug
   struct viewer. Wave 3a applied many of them. Keep those names. When you merge definitions, apply the
   rows of every merged copy, by the rules in `tools/SHARED.md` (section "Wave 3a"): `high` rows
   always, `medium` rows after you check the code. Keep each original name exactly as Rare wrote it.
   Our names for the other fields use camelCase.

## Phase 2: declarations
Each agent takes a list of subsystems. For each subsystem:
1. Write the prototypes and the `extern` declarations into `include/conker/<prefix>.h`, from the
   definitions in its files.
2. Move the declarations of those symbols out of `functions.h` and `variables.h`.
3. Remove the local copies from every C file, in all subsystems.

## Loop
```
python3 ~/conker/tools/cw.py new --tree hdr.<name>           # a tree workspace
#   edit the headers and the C files; run check after every few files
python3 ~/conker/tools/cw.py check clean.hdr.<name>
python3 ~/conker/tools/cw.py land  clean.hdr.<name> -m "headers: <name>"
```
- Land often: after each type or each subsystem that matches. A small land has fewer merge conflicts.
- Other agents edit the same headers at the same time. `land` merges by lines. If it reports a merge
  conflict, do what the message says: copy main's file into the workspace and into `base/`, apply your
  edit again, check, and land again.
- A land cannot create a file or a directory. Use the headers that exist.

## Rules
- **The definition decides the declaration.** Write each prototype from the definition of the function:
  its return type and its parameter types.
- **A prototype can change the code of a caller.** A different parameter type changes the argument
  promotion (char, short and float arguments) and the handling of the return value. If the correct
  prototype changes the bytes of a caller, check the callee first: maybe the definition has the wrong
  type. If the caller needs another type, keep a local declaration in that caller with a
  `// matching:` comment that tells why.
- **The .bss order depends on declarations.** IDO lays out a file's `.bss` in the order of the first
  declaration of each variable, and an `extern` in a header counts. Keep the externs of each file in
  address order. 16 files define their `.bss` before they include `functions.h` and `variables.h`
  (for example `game_20AE20.c`). Keep that order of lines.
- **K&R definitions** (`void f(a) u8 a; {...}`) stay as they are. Their prototype in the header is the
  one that the callers used.
- **Unused declarations:** remove them. An unused `extern` costs nothing in the ROM, but it is noise.
- **Statics:** a `static` function or variable stays in its C file. If a prototype is necessary, it
  stays at the top of that file.
- Do not rename `func_` or `D_` symbols. Do not change the yaml, the Makefile or `tools/`.
- If something cannot be done cleanly, stop and write the reason in the `notes.md` of the workspace.
