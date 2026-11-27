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
that are not 16-aligned, as rule k describes). The weakest case is `math_mtxIdentity`. Its best C
variant differs only in one register and the order of four stores, and a 185,000-step permuter run did
not close the gap. It shares its register use and its first 13 instructions with `math_mtxScale` and
`math_mtxTranslate`.

A function can meet several rules. GCC is ruled out as the source of the rule-proven code: GCC's `move`
assembles to `addu rd,rs,zero`, which appears 9 times in all compiled C and never in these functions.

### Search tier (8 functions, 592 bytes)

These meet none of the rules above. The project owner accepted them as hand-written on this evidence:

1. No compiler reproduces them from C. Sweeps of IDO 5.3, IDO 7.1 and KMC GCC 2.7.2 (the N64 SDK GCC)
   covered about 10,000 source and flag combinations, with 0 natural matches.
2. Their assembly, written as an `.s` file, assembles byte-for-byte with IDO's assembler.
3. They sit inside hand-written asm blocks.

The only exact C match found was a forced one: guMtxIdentF and the scale/translate pair match KMC GCC only
with pinned FP registers and a contrived statement order. That is not evidence that they were written in C.

| Function | Bytes | What it is, and why it is asm |
|----------|------:|-------------------------------|
| guMtxIdentF        | 80 | A second, custom copy of guMtxIdentF; the SDK's C one is separately at 0x151EFE00. It sits in hasm block D2450–D7980, one function per 16-byte unit, in SDK `mgu` order |
| math_mtxScaleF     | 80 | guScaleF-like. Its hasm neighbours use `sd $zero` |
| math_mtxTranslateF | 80 | guTranslateF-like. It follows the hasm math_mtxTranslate |
| math_transformPoint | 160 | guMtxXFMF-like. Proven-asm math_transformPointW calls it with a hand-set `ra`, and math_storeTransformW then reads the `$f0/$f2/$f4` values it leaves behind |
| math_cross3        | 80 | Cross product. Every `mul.s` alternates with another instruction, which IDO's reorder mode cannot produce |
| math_dot3          | 48 | Dot product accumulating in place in `$f0`. None of 19,200 instruction orders fed to IDO's assembler gives this schedule |
| math_length3       | 48 | Vector length. A plain reorder-mode `.s` assembled by IDO with `-r4300_mul` gives exactly these bytes |
| func_150AE1B0 | 16 | IDO `-O1` gives these 4 instructions but in `$t6`. This one uses `$t2`, like its asm neighbours in game_DAFA0 |

The sweep scripts and raw results are not part of the repository; the matrix above describes what was tried.

## Other ROM contents (not CPU code)

| ROM range | Contents | Status |
|-----------|----------|--------|
| 0x0–0x40 | Cartridge header | Data |
| 0x40–0x1000 | IPL3 boot code (Nintendo, CIC-specific; run by the PIF, not part of the game) | Binary, byte-identical |
| 0x2D4B0–0x42450 | RSP microcode: Nintendo F3DEX 2.08 family (F3DEX, F3DEXBS, F3DEXBG, F3DEXBGBS, each with a .NoN variant; "Yoshitaka Yasumoto 1999 Nintendo") and their data | Binary. The RSP vector-unit ISA has no C compiler |
| init/game/debugger `.data`, `.rodata`, `.bss` | Game data | Defined in C, or in the hand-written `.s` file that owns it. Assets and a few pieces whose owner is not known stay extracted from the ROM (`conker/asm/data`) |
| 0x1A37E0 onward | Compressed assets | Binary |

This accounting covers the US ROM only (sha1 `4cbadd3c…`). Other regions are not decompiled.
