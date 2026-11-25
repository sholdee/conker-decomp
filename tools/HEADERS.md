# Header consolidation: one declaration for each symbol

**Goal.** Each function and each global has exactly one declaration, in a header, and each struct has
exactly one definition. A C file keeps only its `static` declarations and the types that only it uses.
The ROM must stay byte-identical. The whole-ROM sha1 decides.

**Why.** Today the C files hold about 5,200 function prototypes and 1,400 `extern` declarations that
are in no header. Also, 22 struct names have more than one definition (`Particle` has ten). Readers
see this as repeated definitions.

## Layout
- One header for each subsystem: `include/<prefix>.h`, with the prefixes of
  `~/conker-ws/rename/subsystems.tsv` (for example `include/camera.h`, `include/particle.h`).
- A subsystem header holds the prototypes of the non-static functions and the `extern` declarations of
  the globals that the files of that subsystem define. Group them by file, in address order, with a
  one-line comment that names the file.
- `include/functions.h` and `include/variables.h` include every subsystem header. Thus each C file still
  sees every declaration.
- Shared structs, unions and enums go to `include/structs.h` or `include/effects.h`. A type that only
  one subsystem uses goes to the header of that subsystem.

## Loop
```
python3 ~/conker/tools/cw.py new --tree hdr.<prefix>          # a tree workspace for your subsystems
#   write include/<prefix>.h; remove the local copies from the C files; check after every few files
python3 ~/conker/tools/cw.py check hdr.<prefix>
python3 ~/conker/tools/cw.py land  hdr.<prefix> -m "headers: <prefix>"
```

## Rules
- **The definition decides the declaration.** Write each prototype from the definition of the function:
  its return type and its parameter types.
- **A prototype can change the code of a caller.** A different parameter type changes the argument
  promotion (char, short and float arguments) and the handling of the return value. If the correct
  prototype changes the bytes of a caller, check the callee first: maybe the definition has the wrong
  type. If the caller needs another type, keep a cast or a local declaration in that caller, with a
  `// matching:` comment.
- **K&R definitions** (`void f(a) u8 a; {...}`) stay as they are. Their prototype in the header is the
  one that the callers used.
- **Unused declarations:** remove them. IDO writes every declared `extern` into the symbol table of the
  object. An unused `extern` costs nothing in the ROM, but it is noise.
- **Duplicate structs:** keep one definition in a header. Use the most complete field names. Every
  layout must agree: same size, same offsets. If two definitions do not agree, read the code to find
  the correct one.
- **Statics:** a `static` function or variable stays in its C file. If a prototype is necessary, it
  stays at the top of that file.
- Do not rename symbols. Do not change the yaml, the Makefile or `tools/`.
- After a subsystem matches, land it. Other agents land other subsystems in parallel.
