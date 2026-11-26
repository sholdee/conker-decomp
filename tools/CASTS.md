# Cast audit: correct types, no warnings, no unnecessary casts

**Goal.** The C code builds with no IDO type warnings, and it has no cast that exists only because a
type is wrong. The ROM must stay byte-identical. The whole-ROM sha1 decides.

**Why.** A cast that hides a wrong type makes the code hard to read. Readers call it a "cast fest". Each
compiler warning shows a place where two parts of the code disagree about a type. At the start, the
build gave 1,917 warnings in 298 files (`~/conker-ws/hdr/warnings_baseline.txt`):
- 709 "Incompatible pointer type assignment" (1,458): one object has two struct types, or a field has
  the wrong pointer type.
- 712 "illegal combination of pointer and integer" (340): an `s32` holds a pointer.
- 642 "prototype declaration and non-prototype definition" (116): a K&R definition with a header
  prototype.

## The owner's rule
The code must be clean, and faithful to the original source. If the original form is out of date (K&R
definitions, declarations without a prototype), the original form wins. Read the section "Callers that
disagree with the definition" in `tools/HEADERS.md`.

## Loop
```
python3 ~/conker/tools/cw.py new --tree cast.<name>          # a tree workspace
#   edit types and code; then show the warnings of a file:
python3 -c "import sys; sys.path.insert(0,'/home/ethan/conker/tools'); import mw; \
print(mw.run('make -B build/src/game_X.c.o 2>&1 | grep Warning', '/home/ethan/conker-ws/clean.cast.<name>/conker').stdout)"
python3 ~/conker/tools/cw.py check clean.cast.<name>          # must print RESULT: MATCH
python3 ~/conker/tools/cw.py land  clean.cast.<name> -m "types: <what>"
```
Land small and often. Other agents change the same headers at the same time.

## Rules
- **Correct the type. Do not hide the warning.** Do not add a cast to stop a warning. Find the correct
  type from the definition and from how the code uses the value. Then correct the struct field, the
  parameter, the return type or the local variable.
- **One object, one type.** Two struct types often describe one object (for example `ModelInstance`,
  `HudModelAnim` and `AnimInstance`). Merge them into one definition, as in phase 1 of
  `tools/HEADERS.md`. Every layout must agree: the same size and the same offsets. Apply the original
  field names (`~/conker-ws/origfields/mapping.tsv`, rules in `tools/SHARED.md`, section "Wave 3a").
- **Pointers are pointers.** A field, a variable or a parameter that holds an address gets a pointer
  type. If the code stores different types there, use `void *`.
- **Remove an unnecessary cast.** Some casts do nothing: the value already has the type, or the
  implicit conversion is the same. If the bytes stay the same without such a cast, remove it.
- **Keep a necessary cast.** A cast that changes the code stays. Most of these are normal C (`(f32)i`,
  `(u8)x`) and need no comment. If the cast looks wrong to a reader, add a `// matching:` comment.
- **Warning 642 can stay.** A K&R definition whose callers used a prototype gives this warning. That is
  the original form. Do not change it to remove the warning.
- **Do not rename `func_` or `D_` symbols.** Do not change the yaml, the Makefile or `tools/`.
- **The .bss order.** IDO lays out the `.bss` of a file in an order that depends on the number of types
  and variables declared before it. A new type in a header can move the `.bss` of another file. Read
  the comment at the top of `src/init_50A0.c`. Use the pattern that other files use: they define their
  `.bss` before `#include "functions.h"`.
- If something cannot be done cleanly, stop and write the reason in the `notes.md` of the workspace.
