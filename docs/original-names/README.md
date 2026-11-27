# Original names from the prerelease debuggers

Two prerelease builds of the game contain a developer debugger with a struct browser
(`rom_debug_struct.c`, per its own on-screen text): the October 25, 2000 debug build and the August 26,
2000 ECTS build. The browser prints the fields of the game's main structs by their original names. The
US release does not contain it. These files record those names and where they fit in the US structs.
They are the evidence for the "original names" step of [tools/RENAMING.md](../../tools/RENAMING.md).

The files contain names, offsets and addresses only. They contain no ROM data. You need the prerelease
ROMs to reproduce them, and they are not needed to build the US version.

## Files

| File | Content |
|---|---|
| `fields_debug.tsv` | Every row the October 2000 browser prints: struct group, label, offset, load type, display format, the code address of the viewer, and the value expression |
| `fields_ects.tsv` | The same for the August 2000 ECTS build |
| `mapping.tsv` | 480 browser fields mapped to US structs: our struct, our field name at the time of the mapping, the US offset, the original name, the confidence, and the evidence |
| `enums.txt` | Name tables found next to the viewers: zone types, object state, task flags, camera modes, task ids and level ids |

## How the names were recovered

The browser is code, not a table. Each struct has a hand-written viewer that prints a label and then
the field's value, for example `printstr(row, "label"); printval(row + 16, mode, s->field);`. Running
each viewer symbolically gives every label together with the load that reads its field. The load gives
the offset and width in the prerelease layout.

The prerelease layouts differ from US in places (`Actor` is 0x328 bytes in October 2000 and 0x32C in
US). To find the US offset, US functions were paired with identical prerelease functions (immediates
masked): 1,946 game and 85 init functions in the debug build. Each load or store in a paired function
links a prerelease offset to a US offset.

## Confidence

- **high** (439 rows): direct code evidence for the offset, or both neighbouring fields agree. The US
  field starts exactly there with the same width, and it is not padding.
- **medium** (41 rows): the US offset is padding (US no longer uses the field), the width differs, or
  only one side gives evidence.

## How to use them

- Match by struct and US offset, not by our field name: our names changed after the mapping was made.
- Check each offset against the US code before you rename. The names describe the October 2000 build.
- The enum values are the prerelease orderings. Check each value against the US code before you use it.
- The task and level ids are the developers' identifiers (for example `W1_BULL_PADDOCK`), not text that
  the game displays. Never copy the game's own text (dialogue, menus, titles) into the repository.
