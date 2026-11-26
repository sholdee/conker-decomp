#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/**
 * Display-list checker state:
 *   D_800C6860[16]  segment base table, mirrored from G_MW_SEGMENT moveword commands (-1 = unset)
 *   D_800C68A0      set once something is drawn, cleared by G_RDPPIPESYNC
 *   D_800C68A1      current G_DL nesting depth
 *   D_800848B0      last G_SETTIMG address
 *   D_800844B0[256] per-opcode handlers used when dumping commands
 */

/** Checks the display list at dl (up to end, or up to its G_ENDDL when end is 0). */
void func_15040350(void *dl, u32 end) {
    s32 i;

    D_800C68A0 = D_800848B0 = 0;
    D_800C68A1 = 0;

    for (i = 0; i < 16; i++) {
        D_800C6860[i] = -1;
    }
    D_800C6860[0] = 0;
    func_150403C8(dl, end, 0);
}

/** Posts error code 0xC00005B to D_8003C8E0 and calls func_150AD770. */
#define DL_ERROR()                          \
    D_8003C8E0 = 0xC00005B;                 \
    func_150AD770();

/** DL_ERROR(), then calls func_10007DA0 with D_800BE9C0 toggled for the call. */
#define DL_ERROR_FLIP()                   \
    DL_ERROR();                             \
    D_800BE9C0 = 1 - D_800BE9C0;            \
    func_10007DA0();                        \
    D_800BE9C0 = 1 - D_800BE9C0;

/**
 * Walks a display list (up to end, or up to its G_ENDDL when end is 0), following
 * G_DL branches, and reports malformed commands: bad branch targets, nesting
 * deeper than 20, corrupt G_VTX/G_TRI1 commands, and RDP state changes made after
 * drawing without a G_RDPPIPESYNC. Each report dumps the commands around the
 * culprit with func_15040CC8. parent (the calling G_DL command) is unused.
 */
void func_150403C8(void *dl, u32 end, s32 parent) {
    Gfx *gfx;
    s32 pad[2]; // matching: reserves the stack slots the original frame has
    s32 isBad = 0;
    s32 needsPipeSync;
    u32 addr;
    u32 segment;

    gfx = dl;
    while ((end == 0 && gfx->dma.cmd != (s8)G_ENDDL) || (end != 0 && (u32)gfx < end)) {
        needsPipeSync = 0;
        switch (gfx->dma.cmd) {
            case (s8)G_DL:
                addr = gfx->words.w1;
                isBad = func_150408CC(gfx);
                segment = (addr >> 24) & 0xF;
                addr &= 0xFFFFFF;
                addr += D_800C6860[segment];
                addr |= 0x80000000;
                D_800C68A1++;
                if (D_800C68A1 == 20) {
                    func_15040CC8(gfx);
                    DL_ERROR();
                }
                if (addr > 0x80000000 && addr < 0xA0000000 && !(addr & 7)) {
                    func_150403C8((void *)addr, 0, (s32)gfx);
                } else {
                    func_15040CC8(gfx);
                    DL_ERROR_FLIP();
                }
                if (D_800C68A1 > 0) {
                    D_800C68A1--;
                } else {
                    DL_ERROR_FLIP();
                }
                break;
            case (s8)G_RDPPIPESYNC:
                D_800C68A0 = 0;
                break;
            case (s8)G_TEXRECT: case (s8)G_TEXRECTFLIP: case (s8)G_FILLRECT: case G_TRI2: case G_QUAD:
            // raw RDP triangle commands
            case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
            case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F:
                D_800C68A0 = 1;
                break;
            case G_TRI1:
                isBad = func_1504082C(gfx);
                D_800C68A0 = 1;
                break;
            case G_VTX:
                isBad = func_15040754(gfx);
                break;
            case (s8)G_SETTIMG:
                D_800848B0 = gfx->words.w1;
                break;
            // matching: these three groups are separate case bodies in the original;
            // merging them changes the jump table and code size
            case (s8)G_SETCIMG:
                needsPipeSync = 1;
                break;
            case (s8)G_SETOTHERMODE_L: case (s8)G_SETOTHERMODE_H: case (s8)G_SETCONVERT:
            case (s8)G_SETSCISSOR: case (s8)G_SETPRIMDEPTH: case (s8)G_SETFILLCOLOR:
            case (s8)G_SETFOGCOLOR: case (s8)G_SETBLENDCOLOR: case (s8)G_SETENVCOLOR:
            case (s8)G_SETCOMBINE: case (s8)G_SETZIMG:
                needsPipeSync = 1;
                break;
            case (s8)G_SETKEYGB: case (s8)G_SETKEYR:
                needsPipeSync = 1;
                break;
            case (s8)G_MOVEWORD:
                {
                    u32 index;
                    s32 offset;
                    s32 data;

                    index = gfx->words.w0 >> 16;
                    offset = gfx->words.w0 & 0xFFFF;
                    data = gfx->words.w1;
                    if ((index & 0xFF) == G_MW_SEGMENT) {
                        D_800C6860[offset / 4] = data;
                    }
                }
                break;
            case (s8)G_SPECIAL_3: case (s8)G_SPECIAL_2: case (s8)G_SPECIAL_1: case (s8)G_DMA_IO:
            case (s8)G_TEXTURE: case (s8)G_POPMTX: case (s8)G_GEOMETRYMODE: case (s8)G_MTX:
            case (s8)G_MOVEMEM: case (s8)G_LOAD_UCODE: case (s8)G_ENDDL: case (s8)G_SPNOOP:
            case (s8)G_RDPHALF_1: case (s8)G_RDPLOADSYNC: case (s8)G_RDPTILESYNC: case (s8)G_RDPFULLSYNC:
            case (s8)G_RDPSETOTHERMODE: case (s8)G_LOADTLUT: case (s8)G_RDPHALF_2: case (s8)G_SETTILESIZE:
            case (s8)G_LOADBLOCK: case (s8)G_LOADTILE: case (s8)G_SETTILE:
            case (s8)G_SETPRIMCOLOR: case G_NOOP: case G_MODIFYVTX: case G_CULLDL: case G_BRANCH_Z: case G_LINE3D:
                break;
            default:
                DL_ERROR();
                break;
        }
        if (needsPipeSync && D_800C68A0) {
            func_15040CC8(gfx);
            D_800C68A0 = 0;
            DL_ERROR_FLIP();
        }
        if (isBad) {
            isBad = 0;
            func_15040CC8(gfx);
            DL_ERROR_FLIP();
        }
        gfx++;
    }
}

/** Returns whether a display list address is not 8-byte aligned. */
s32 func_1504072C(s32 addr) {
    if (addr & 7) {
        return 1;
    }
    return 0;
}

/** Command handler for every opcode without its own entry (empty). */
void func_15040748(Gfx *gfx) {
}

/**
 * G_VTX check: returns nonzero when the first vertex index (v0 + n minus n)
 * is negative or the vertex source address is misaligned.
 */
s32 func_15040754(Gfx *gfx) {
    s32 first = (gfx->words.w0 >> 1) & 0x7F; // v0 + n
    s32 count = (gfx->words.w0 >> 12) & 0xFF;

    first -= count;
    if (first < 0) {
        return 1;
    }
    return func_1504072C(gfx->words.w1);
}

/** G_VTX command handler (empty). */
void func_150407A0(void) {
}

/** G_MODIFYVTX command handler (empty). */
void func_150407A8(Gfx *gfx) {
}

/** G_CULLDL command handler (empty). */
void func_150407B4(Gfx *gfx) {
}

/** G_BRANCH_Z command handler (empty). */
void func_150407C0(Gfx *gfx) {
}

/** G_LINE3D command handler (empty). */
void func_150407CC(Gfx *gfx) {
}

/** G_RDPHALF_1 command handler (empty). */
void func_150407D8(Gfx *gfx) {
}

/** G_RDPHALF_2 command handler (empty). */
void func_150407E4(Gfx *gfx) {
}

/** G_SETKEYGB command handler (empty). */
void func_150407F0(Gfx *gfx) {
}

/** G_SETKEYR command handler (empty). */
void func_150407FC(Gfx *gfx) {
}

/** G_QUAD command handler (empty). */
void func_15040808(Gfx *gfx) {
}

/** G_TRI2 command handler (empty). */
void func_15040814(Gfx *gfx) {
}

/** Command handler for opcodes 0x10-0x1F (empty). */
void func_15040820(Gfx *gfx) {
}

/**
 * G_TRI1 check: the three vertex indices are stored doubled, so an odd byte
 * means a corrupt command. Returns nonzero in that case.
 */
s32 func_1504082C(Gfx *tri) {
    s32 v0 = (tri->words.w0 >> 16) & 0xFF;
    s32 v1 = (tri->words.w0 >> 8) & 0xFF;
    s32 v2 = tri->words.w0 & 0xFF;

    if ((v0 & 1) || (v1 & 1) || (v2 & 1)) {
        return 1;
    }
    return 0;
}

/** G_TRI1 command handler (empty). */
void func_1504086C(Gfx *gfx) {
}

/** G_LOADTLUT command handler (empty). */
void func_15040878(Gfx *gfx) {
}

/** G_LOADTILE command handler (empty). */
void func_15040884(Gfx *gfx) {
}

/** G_RDPTILESYNC command handler (empty). */
void func_15040890(Gfx *gfx) {
}

/** G_RDPFULLSYNC command handler (empty). */
void func_1504089C(Gfx *gfx) {
}

/** G_RDPPIPESYNC command handler (empty). */
void func_150408A8(Gfx *gfx) {
}

/** G_RDPLOADSYNC command handler (empty). */
void func_150408B4(Gfx *gfx) {
}

/** G_SETCOMBINE command handler (empty). */
void func_150408C0(Gfx *gfx) {
}

/** G_DL check: returns nonzero when the branch target is misaligned. */
s32 func_150408CC(Gfx *gfx) {
    return func_1504072C(gfx->words.w1);
}

/** G_DL command handler (empty). */
void func_150408F0(Gfx *gfx) {
}

/** G_SETOTHERMODE_H command handler (empty). */
void func_150408FC(Gfx *gfx) {
}

/** G_SETOTHERMODE_L command handler (empty). */
void func_15040908(Gfx *gfx) {
}

/** G_SETCONVERT command handler (empty). */
void func_15040914(Gfx *gfx) {
}

/** G_SETSCISSOR command handler (empty). */
void func_15040920(Gfx *gfx) {
}

/** G_NOOP command handler (empty). */
void func_1504092C(Gfx *gfx) {
}

/** G_SPNOOP command handler (empty). */
void func_15040938(Gfx *gfx) {
}

/** G_SETCIMG command handler (empty). */
void func_15040944(Gfx *gfx) {
}

/** G_SETZIMG command handler (empty). */
void func_15040950(Gfx *gfx) {
}

/** G_SETTIMG command handler (empty). */
void func_1504095C(Gfx *gfx) {
}

/** G_SETENVCOLOR command handler (empty). */
void func_15040968(Gfx *gfx) {
}

/** G_SETPRIMCOLOR command handler (empty). */
void func_15040974(Gfx *gfx) {
}

/** G_SETBLENDCOLOR command handler (empty). */
void func_15040980(Gfx *gfx) {
}

/** G_SETFOGCOLOR command handler (empty). */
void func_1504098C(Gfx *gfx) {
}

/** G_SPECIAL_3 command handler (empty). */
void func_15040998(Gfx *gfx) {
}

/** G_SPECIAL_2 command handler (empty). */
void func_150409A4(Gfx *gfx) {
}

/** G_SPECIAL_1 command handler (empty). */
void func_150409B0(Gfx *gfx) {
}

/** G_DMA_IO command handler (empty). */
void func_150409BC(Gfx *gfx) {
}

/** G_SETFILLCOLOR command handler (empty). */
void func_150409C8(Gfx *gfx) {
}

/** G_FILLRECT command handler (empty). */
void func_150409D4(Gfx *gfx) {
}

/** G_TEXRECT command handler (empty). */
void func_150409E0(Gfx *gfx) {
}

/** G_TEXRECTFLIP command handler (empty). */
void func_150409EC(Gfx *gfx) {
}

/** G_LOADBLOCK command handler (empty). */
void func_150409F8(Gfx *gfx) {
}

/** G_SETTILE command handler (empty). */
void func_15040A04(Gfx *gfx) {
}

/** G_SETTILESIZE command handler (empty). */
void func_15040A10(Gfx *gfx) {
}

/** G_ENDDL command handler (empty). */
void func_15040A1C(Gfx *gfx) {
}

/** G_POPMTX command handler (empty). */
void func_15040A28(Gfx *gfx) {
}

/** G_MTX command handler (empty). */
void func_15040A34(Gfx *gfx) {
}

/** G_GEOMETRYMODE command handler (empty). */
void func_15040A40(Gfx *gfx) {
}

void func_15040A4C(void) {
}

/** G_LOAD_UCODE command handler (empty). */
void func_15040A54(Gfx *gfx) {
}

/** G_MOVEMEM command handler (empty). */
void func_15040A60(Gfx *gfx) {
}

/** G_MOVEWORD command handler (empty). */
void func_15040A6C(Gfx *gfx) {
}

/**
 * Walks the display list at start, following G_DL branches, and runs the
 * D_800844B0 handler of every command from `from` through `to` (inclusive).
 * Also tracks G_MW_SEGMENT movewords in D_800C6860 to resolve branch targets.
 */
void func_15040A78(Gfx *start, Gfx *from, Gfx *to) {
    s32 i;
    Gfx *gfx;
    u32 w0;
    u32 w1;
    s32 count;
    Gfx *stack[16];
    u8 cmd;
    u8 inRange;
    u32 segment;
    s32 pad1[2]; // matching: stack slots of unused locals in the original
    u32 data;
    u32 mtx;
    s32 depth;
    s32 pad2[4]; // matching: as pad1

    depth = 0;
    i = 0;
    do { // matching: a for loop here is scheduled differently
        D_800C6860[i] = -1;
        i++;
    } while (i < 16);
    D_800C6860[0] = 0;
    inRange = 0;
    gfx = start;
    while (gfx <= to || depth > 0) {
        if (gfx == from) {
            inRange = 1;
        }
        cmd = gfx->dma.cmd;
        if (inRange) {
            D_800844B0[cmd]((s32)gfx);
        }
        if (gfx == to) {
            inRange = 0;
        }
        switch (cmd) {
            case G_ENDDL:
                depth--;
                gfx = stack[depth];
                break;
            case G_DL:
                stack[depth] = gfx + 1;
                w1 = gfx->words.w1;
                // matching: the empty ifs in this function look like compiled-out debug
                // prints; removing them moves code
                if (inRange) {
                }
                depth++;
                if (inRange) {
                }
                segment = (w1 >> 24) & 0xF;
                if (D_800C6860[segment] != -1) {
                    w1 &= 0xFFFFFF;
                    w1 += D_800C6860[segment];
                    gfx = (Gfx *)(w1 | 0x80000000);
                } else {
                    gfx++;
                    depth--;
                }
                break;
            case G_MOVEWORD:
                w0 = gfx->words.w0;
                data = gfx->words.w1;
                if ((w0 & 0xFFFF) == (w0 >> 16)) {
                }
                if (((w0 >> 16) & 0xFF) == G_MW_SEGMENT) {
                    D_800C6860[(s32)(w0 & 0xFFFF) >> 2] = data;
                }
                gfx++;
                break;
            case G_VTX:
                if (inRange) {
                    count = (gfx->words.w0 >> 12) & 0xFF;
                    for (i = 0; i < count; i++) {
                        // empty, like the ifs above
                    }
                }
                gfx++;
                break;
            case G_MTX:
                mtx = (D_800C6860[(gfx->words.w1 >> 24) & 0xF] + (gfx->words.w1 & 0xFFFFFF)) | 0x80000000;
                if (mtx == 0) {
                }
                gfx++;
                break;
            case G_MOVEMEM:
                gfx++;
                break;
            default:
                gfx++;
                break;
        }
    }
}

/**
 * Dumps the commands from 20 before gfx to 9 after it through the D_800844B0
 * handlers, then passes the last G_SETTIMG address to func_1500390C.
 */
void func_15040CC8(Gfx *gfx) {
    s32 i;
    s32 addr; // matching: a Gfx * local here changes the generated code
    s32 lastTexture;
    void (**handlers)(s32);

    for (i = 0; i < 16; i++) {
        // empty: the loop itself is emitted
    }

    handlers = D_800844B0;
    // matching: "| 0" keeps IDO from rewriting the exit test as a bne against 10 held in a register
    for (i = -20; i < 10; i = (i + 1) | 0) {
        addr = (s32)&gfx[i];
        handlers[*(u8 *)addr](addr);
    }

    lastTexture = D_800848B0;
    if (lastTexture != 0) {
        func_1500390C(lastTexture);
    }
}

/** G_SETPRIMDEPTH command handler (empty). */
void func_15040D60(Gfx *gfx) {
}

/** G_TEXTURE command handler (empty). */
void func_15040D6C(Gfx *gfx) {
}

/** G_RDPSETOTHERMODE command handler (empty). */
void func_15040D78(Gfx *gfx) {
}
