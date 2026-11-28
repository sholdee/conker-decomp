# Code accounting (US)

Every byte of the CPU code in Conker's Bad Fur Day (US) is accounted for. It is either C that compiles to
byte-identical code, or hand-written assembly. No undecompiled asm remains: there are no `GLOBAL_ASM`
stubs, no inline asm, and no `asm` code segments. Two `GLOBAL_ASM` blocks remain in C files
(`hudoverlay.c`, `n_resample.c`). Each is a run of `nop`s that pads between objects, not code.

The hand-written assembly is 63,664 bytes (2.8% of the code), counted in 289 labelled pieces. A piece is
a `glabel`. 126 pieces are `jal` targets, and 139 are entry points: a `jal` target, an address stored in
data, or the target of a jump from another object. The other labels are inside routines. The rules are
computed on runs that start at an object start or an entry point, so a label alone cannot create a proof.
281 pieces have a proof by the rules below. The other 8 (592 bytes) are in a search tier: no
rule covers them, and they are accepted as hand-written because no compiler search reproduced them. That
is evidence, not a proof (see "Search tier").

To check it, build and run the checker. It exits non-zero if anything is left over, and writes the
per-function manifest to `tools/accounting.tsv`.

```
python3 tools/accounting.py
```

| Segment  | Code bytes | Matched C            | Hand-written asm, rule-proven | Hand-written asm, search tier | Nop padding |
|----------|-----------:|---------------------:|------------------------------:|------------------------------:|------------:|
| init     |    164,048 | 151,764 (92.51%)     | 12,208 (66 functions)         | 0                             | 76          |
| game     |  2,072,880 | 2,021,520 (97.52%)   | 50,704 (214 functions)         | 592 (8 functions)             | 64          |
| debugger |     14,064 | 13,904 (98.86%)      | 160 (1 function)              | 0                             | 0           |
| **all**  |  2,250,992 | 2,187,188 (97.17%)   | 63,072 (281 functions)        | 592 (8 functions)             | 140         |

Byte counts are each object's `.text` from the link map. A function is every `glabel` in a hand-written `.s` file (`tools/filter_auto_syms.py` keeps
splat's auto-symbol files from shadowing them) and every C function. Functions are cut at global symbols only, so a
jump-table label inside a function stays part of it. Bytes before an object's first symbol count as a
piece of their own.

## Hand-written asm: proof rules

Hand-written functions are committed as `conker/src/*.s` (yaml `hasm` segments). A function is listed as
hand-written only with a proof or with the search-tier evidence, which `tools/accounting.tsv` records in
its `proof` column.

Rules marked * are computed from the instructions by `tools/accounting.py`, on runs: a run starts at an
entry point, where the code changes kind, or at a 16-aligned object start after a finished routine. The
counts below are the pieces whose run meets each rule. Each one was checked against
the 5,761 functions IDO 5.3 compiles in this game, and none of them shows the pattern. The other rules come
from `tools/hasm_proofs.tsv`.

| Rule | Meaning | Functions |
|------|---------|----------:|
| a    | Source-identical: the SDK's own `.s` (ultralib), assembled by IDO 5.3 (`cc`, with as0 and as1), gives the same bytes | 16 |
| b*   | An instruction IDO 5.3 `-mips2` cannot emit: cop0, TLB, cache, sync, syscall, 64-bit `d*`, `ld`/`sd`, trapping `add`/`addi`/`sub`, or the kernel registers `$k0`/`$k1` | 174 |
| c    | Dataflow: reads a register that is not an ABI input and was not written first | 45 |
| d*   | An opcode-2 `j`, or a branch that leaves the function (IDO uses `b` and never tail-calls) | 195 |
| e*   | `cvt.w.s`/`cvt.w.d` with no `ctc1` in the function (IDO always sets the rounding mode) | 75 |
| f*   | Frame layout: a store below `$sp`, or `ra` saved in the argument area or without allocating a frame | 31 |
| g*   | No return: execution falls into the next function | 4 |
| h*   | An odd FPR as a single-precision arithmetic operand, or `jal` without saving `ra` | 154 |
| i*   | 6 or more `mul.s`, every one followed by a `nop` (compiled C reaches at most 67%) | 2 |
| k*   | Same object as a proven function, which it meets at a non-16-aligned address (IDO pads every C object's `.text` to 16 bytes at both ends: all 758 C objects with code) | 7 |
| s    | Search tier (below) | 8 |

**Rule b and `-mips3`.** The 64-bit part of rule b is proof only for `-mips2` objects. With `-mips3`,
IDO 5.3 emits `ld`, `sd` and `d*` instructions for `long long` code (`libultra/ll.c` is built that way).
Nine functions have no evidence other than 64-bit instructions: `math_mtxIdentity`, `math_mtxScale`,
`math_mtxTranslate`, `anim_copyRootMtx`, `math_random`, `math_randomFloat`, `math_seedRandom`,
`func_150AE1C0` and `func_150AE1DC`. Each one was compiled from several C variants with `-mips3` at
`-O1`, `-O2`, `-O3` and with `-g3`, and none of them matched. Seven of the nine show code that IDO does
not produce:

- Dead stores (`math_mtxScale`, `math_mtxTranslate`, `math_seedRandom`).
- A `dmult` and a variable shift where IDO uses `dmultu` and a constant shift (`func_150AE1C0`).
- A 64-bit return in `$v0` alone, and `ddiv` with no divide checks (`func_150AE1C0`, `func_150AE1DC`).
- An empty loop delay slot together with no stack frame (`anim_copyRootMtx`).

`math_random` and `math_randomFloat` share one object with `math_seedRandom` (they meet at addresses
that are not 16-aligned, as rule k describes). The weakest case is `math_mtxIdentity`. IDO 5.3
`-mips3 -O2` compiles a C version (eight `u64` stores of 0, four `s16` stores of 1, one statement on each
line) to the same 14 instructions in the same order. The only difference is the register that holds the
constant 1: IDO uses `$v0`, the ROM has `$t0`. (IDO's assembler orders stores that come from one source
line in its own way. That is why an earlier variant, with the four `s16` stores on one line, also had them
in reverse order.) `-O1` with a `register` local gives `$t0`, but then the function has a stack frame.
1,760 more source variants and a 690,000-step permuter run did not give `$t0` without a frame. The
function shares its register use and its first 13 instructions with `math_mtxScale` and
`math_mtxTranslate`.

A function can meet several rules. GCC is ruled out as the source of the rule-proven code: GCC's `move`
assembles to `addu rd,rs,zero`, which appears 9 times in all compiled C and never in these functions.

### Search tier (8 functions, 592 bytes)

These meet none of the rules above. The project owner accepted them as hand-written on this evidence:

1. No compiler reproduces them from natural C. Sweeps of IDO 5.3, IDO 7.1 and KMC GCC 2.7.2 (the N64 SDK
   GCC) covered about 10,000 source and flag combinations, with 0 natural matches.
2. Their assembly, written as an `.s` file, assembles byte-for-byte with IDO's assembler.
3. They sit inside hand-written asm blocks.

Two exact C matches exist. Both are forced, and neither is evidence that the code was written in C:

- guMtxIdentF and the scale/translate pair match KMC GCC only with pinned FP registers and a contrived
  statement order.
- math_mtxScaleF matches IDO 5.3 with the game's own flags (`-O2 -g3`), but only from a source with three
  devices. It declares the three float arguments as `s32`, although every caller passes floats under the
  `f32` prototype in its header. It puts the first 15 stores in a block that has a dummy local. It
  returns from inside `while (1)` after the last store. The function stays asm: C with the real argument
  types does not match.

**Why this tier is evidence and not proof.** A review on 2026-11-26 tried to find a rule for these
functions, in the way the computed rules above were found. Every candidate rule failed on IDO's own output.
The test set was every C file of the game compiled with 21 flag sets (120,533 compiled functions), and more
than a million generated functions.

- *Instruction order.* IDO's assembler (`as1`) reorders the instructions of each block. With `-g3` it
  handles every lexical block that has a declaration as a block of its own (72,000 of 72,000 generated
  single-block functions agree with this model), and source line numbers decide the order of instructions
  that are otherwise free to move. So C with one statement in each block keeps its statement order. "The
  assembler cannot produce this order" is true only for code that is one block. With blocks, the assembler
  reproduces the bytes of math_dot3 and math_length3 from their own instructions, also with
  `-mips2 -r4300_mul`.
- *Registers.* IDO does write `$t0`-`$t5` without `$t6`-`$t9`. Two of the game's own functions do it with
  other flags: `cutscene_setAnchor` at `-O2` without `-g3` writes only `$t2` (the assembler deletes its sign
  extensions, and their temporaries with them), and `math_closestPointOnLine` at `-O3` keeps its fifth
  argument in `$t0`. IDO does move a float argument from `$a1`-`$a3` into `$f8` or `$f10`. It does keep a
  value in `$f4`-`$f10` from one statement to the next.
- *Delay slots.* With `-g3`, IDO fills the `jr ra` delay slot when the `return` is inside `while (1)`.

The table gives what remains specific to each function.

| Function | Bytes | What it is, and why it is asm |
|----------|------:|-------------------------------|
| guMtxIdentF        | 80 | A second, custom copy of guMtxIdentF; the SDK's C one is separately at 0x151EFE00. It sits in hasm block D2450–D7980, one function per 16-byte unit, in SDK `mgu` order. One `$f4` holds 1.0 for all four stores. IDO's closest code (`-O2 -g3`) holds it in `$f0` and has a `nop` after the `mtc1`; 3,510 signature and constant variants never held it in `$f4` |
| math_mtxScaleF     | 80 | guScaleF-like. Its hasm neighbours use `sd $zero`. IDO reproduces it only from the forced source described above |
| math_mtxTranslateF | 80 | guTranslateF-like. It follows the hasm math_mtxTranslate. The same `$f4` and `mtc1` evidence as guMtxIdentF |
| math_transformPoint | 160 | guMtxXFMF-like. Proven-asm math_transformPointW calls it with a hand-set `ra`, and math_storeTransformW then reads the `$f0/$f2/$f4` values it leaves behind. Its first two rows are, instruction for instruction, the 11-instruction row of math_storeTransformW with other offsets and another result register |
| math_cross3        | 80 | Cross product. Every `mul.s` alternates with another instruction. As one block, IDO's assembler changes the ROM's instruction order at all 54 settings, and no order in a sample of 74,683 (of 18,655,520) gives it |
| math_dot3          | 48 | Dot product accumulating in place in `$f0`. As one block, none of the 19,200 orders of its instructions gives this schedule in IDO's assembler (54 settings) |
| math_length3       | 48 | Vector length. As one block, IDO's assembler gives this schedule only with `-mips1 -r4300_mul`, and with `-mips1` the compiler does not emit `sqrt.s` (0 in 40,178 compiled functions; it calls `sqrtf`) |
| func_150AE1B0 | 16 | IDO `-O1` gives these 4 instructions but in `$t6`. This one uses `$t2`, like its asm neighbours in game_DAFA0 |

The sweep scripts and raw results are not part of the repository; the text above describes what was tried.
"54 settings" means `-mips1`, `-mips2` or `-mips3`, with `-O1`, `-O2` or `-O3`, with and without `-r4300_mul`,
for three source styles: a plain `.s` file, and the compiler's own output style with one source line for each
instruction or one line for all.

## File boundaries

A byte match does not show that our source files have Rare's boundaries. `tools/boundaries.py` grades
the evidence for each boundary between two objects in the code segments. It only reports. It does not
change the layout. It writes two tables, which hold addresses, names and grades only:

- `docs/file-boundaries.tsv`: one row for each boundary. The columns are the US address where the right
  object starts, the left and right objects, the grade, the evidence kinds and a note.
- `docs/file-boundary-candidates.tsv`: places where the evidence says that one of our files was two
  files (split), or that two of our files were one file (merge), with the conditions that hold.

The tool uses these facts:

- IDO pads the code of each C object to 16 bytes. It puts no padding between the functions of one
  object. So zero words after a return, between two C functions, show a file boundary. A function that
  starts on an address that is not 16-aligned, directly after the previous function, is in the same
  object. A function on a 16-aligned address with no zero words before it shows nothing.
- These rules are for compiled C only. A hand-written `.s` file can hold `.align` or `.balign`, data words
  and nops. So zero words next to hand-written asm are not a proof.
- The `.data` and `.rodata` of one C object are each one block, in link order. Each block starts on a
  16-aligned address. This is not true for hand-written asm, whose data is 4-aligned, or for `.bss`.
  Also, the linker script places the code and data sections with `SUBALIGN(4)` (8 in the debugger), so our layout decides where a block
  starts. The tool uses only the `.data` and `.rodata` of two C objects.
- One object is compiled once, with one set of flags. C and hand-written asm are never one object.
- Two older builds have almost the same link order: the ECTS build (August 2000) and the debug build
  (October 2000). The padding in them is evidence too. But a file can change between builds.

| Evidence | Meaning |
|----------|---------|
| `padding-us` | Zero words after the return of the last function of the left object, before the first function of the right object, in US |
| `padding-ects`, `padding-debug` | In that build, the partners of the functions next to the boundary are in two objects: there is padding between them. The build has them in the US order, with no other paired function between them |
| `data-edge` | Two C objects: the block of the right object directly follows the block of the left object, in `.data` or `.rodata`. It starts on a 16-aligned address |
| `flags-change` | The two objects have different `-O` or `-mips` flags in `conker/Makefile`, or one is C and the other hand-written asm. If the asm function next to the boundary has only search evidence (ACCOUNTING.md, "Search tier"), this is not a proof |
| `rare-name` | Functions of the two objects use two different source file paths (the path strings of the debug tunables) |

The grades:

- **proven**: between two C objects, `padding-us`, or `padding-ects` or `padding-debug` that is pinned
  and that the other old build does not contradict. Also `flags-change`, and `rare-name` that is pinned.
  Pinned means that the paired functions are the two functions next to the boundary. US joins (starts
  that are not 16-aligned) can also tie them to those functions. If the evidence is not pinned, the
  boundary can be one function away.
- **likely**: other evidence. Padding between two hand-written asm objects is only likely. `data-edge`
  alone is weak, because a block can start on a 16-aligned address by chance.
- **unproven**: no evidence. Sometimes the US code shows that the two sides are one object: the right
  object starts on an address that is not 16-aligned. Such a boundary is also unproven. The note gives
  the evidence against the boundary.

In the candidates table, `order_agrees` says that the old build has the two functions in the US order,
with no other paired function between them. `us_16_aligned` says whether the US address is 16-aligned.
A split needs this, because IDO starts each object on a 16-aligned address. `other_old_build` says what
the other old build shows: it agrees, it contradicts, or it is silent. `us_code` says what the US code
shows at the address: padding, a join or neither (flush). Splits come only from C objects. A C data block
that starts on an address that is not 16-aligned gives a merge candidate. But if the code proves two
objects there, the note says "data split suspect" instead: then our data split is the likely error. A
candidate is not a decision. Read the code before you change the layout.

To make the tables again, you need `conker/conker.us.bin` (from `make -C conker extract`). For the
evidence of the old builds, you also need the two prerelease ROMs and the files that pair their functions
with the US functions. They stay outside the repository.

```
python3 tools/boundaries.py --ects <ECTS ROM> --ects-pairs <ects_pairs.json> \
    --debug <debug ROM> --debug-pairs <function_pairs.tsv> --debug-functions <debug_functions.tsv>
```

If a build is missing, the tool says which evidence it cannot check, and the header of each table names
the missing build. The same inputs always give the same tables.

## Other ROM contents (not CPU code)

| ROM range | Contents | Status |
|-----------|----------|--------|
| 0x0–0x40 | Cartridge header | Data |
| 0x40–0x1000 | IPL3 boot code (Nintendo, CIC-specific; run by the PIF, not part of the game) | Binary, byte-identical |
| 0x2D4B0–0x42450 | RSP microcode: Nintendo F3DEX 2.08 family (F3DEX, F3DEXBS, F3DEXBG, F3DEXBGBS, each with a .NoN variant; "Yoshitaka Yasumoto 1999 Nintendo") and their data | Binary. The RSP vector-unit ISA has no C compiler |
| init/game/debugger `.data`, `.rodata`, `.bss` | Game data | Defined in C, or in the hand-written `.s` file that owns it. Assets and a few pieces whose owner is not known stay extracted from the ROM (`conker/asm/data`) |
| 0x1A37E0 onward | Compressed assets | Binary |

This accounting covers the US ROM only (sha1 `4cbadd3c…`). Other regions are not decompiled.
