#include <ultra64.h>

#include "conker/boot.h"
#include "conker/collision.h"
#include "conker/debug.h"
#include "conker/game.h"
#include "conker/level.h"

/* Display-list checker state. */
static s32 gDlCheckSegments[16]; // segment base table, mirrored from G_MW_SEGMENT moveword commands (-1 = unset)
static u8 gDlCheckDrawn;      // set once something is drawn, cleared by G_RDPPIPESYNC
static u8 gDlCheckDepth;      // current G_DL nesting depth

#define DL_NONE debug_cmdDefault  /* opcodes without their own handler */
#define DL_RSP1X debug_cmdTri4 /* opcodes 0x10-0x1F */

/* Per-opcode handlers used when dumping commands (debug_dumpDlRange, debug_dumpDlAround), indexed by the command byte. */
static void (*gDlCmdHandlers[256])() = {
    /* 0x00 */ debug_cmdNoop, debug_cmdVtx, debug_cmdModifyVtx, debug_cmdCullDl, debug_cmdBranchZ, debug_cmdTri1, debug_cmdTri2, debug_cmdQuad,
    /* 0x08 */ debug_cmdLine3D, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x10 */ DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X,
    /* 0x18 */ DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X, DL_RSP1X,
    /* 0x20 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x28 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x30 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x38 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x40 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x48 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x50 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x58 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x60 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x68 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x70 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x78 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x80 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x88 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x90 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0x98 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0xA0 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0xA8 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0xB0 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0xB8 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0xC0 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0xC8 */ DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE, DL_NONE,
    /* 0xD0 */ DL_NONE, DL_NONE, DL_NONE, debug_cmdSpecial3, debug_cmdSpecial2, debug_cmdSpecial1, debug_cmdDmaIo, debug_cmdTexture,
    /* 0xD8 */ debug_cmdPopMtx, debug_cmdGeometryMode, debug_cmdMtx, debug_cmdMoveWord, debug_cmdMoveMem, debug_cmdLoadUcode, debug_cmdDl, debug_cmdEndDl,
    /* 0xE0 */ debug_cmdSpNoop, debug_cmdRdpHalf1, debug_cmdSetOtherModeL, debug_cmdSetOtherModeH, debug_cmdTexRect, debug_cmdTexRectFlip, debug_cmdRdpLoadSync, debug_cmdRdpPipeSync,
    /* 0xE8 */ debug_cmdRdpTileSync, debug_cmdRdpFullSync, debug_cmdSetKeyGB, debug_cmdSetKeyR, debug_cmdSetConvert, debug_cmdSetScissor, debug_cmdSetPrimDepth, debug_cmdRdpSetOtherMode,
    /* 0xF0 */ debug_cmdLoadTlut, debug_cmdRdpHalf2, debug_cmdSetTileSize, debug_cmdLoadBlock, debug_cmdLoadTile, debug_cmdSetTile, debug_cmdFillRect, debug_cmdSetFillColor,
    /* 0xF8 */ debug_cmdSetFogColor, debug_cmdSetBlendColor, debug_cmdSetPrimColor, debug_cmdSetEnvColor, debug_cmdSetCombine, debug_cmdSetTImg, debug_cmdSetZImg, debug_cmdSetCImg
};
static s32 gDlCheckLastTImg = 0; // last G_SETTIMG address


/** Checks the display list at dl (up to end, or up to its G_ENDDL when end is 0). */
void debug_checkDl(void *dl, u32 end) {
    s32 i;

    gDlCheckDrawn = gDlCheckLastTImg = 0;
    gDlCheckDepth = 0;

    for (i = 0; i < 16; i++) {
        gDlCheckSegments[i] = -1;
    }
    gDlCheckSegments[0] = 0;
    debug_walkCheckDl(dl, end, NULL);
}

/** Posts error code 0xC00005B to gDebugStatusCode and calls debug_fatalTrap. */
#define DL_ERROR()                          \
    gDebugStatusCode = 0xC00005B;                 \
    debug_fatalTrap();

/** DL_ERROR(), then calls boot_debugTrap with gDrawBufferIndex toggled for the call. */
#define DL_ERROR_FLIP()                   \
    DL_ERROR();                             \
    gDrawBufferIndex = 1 - gDrawBufferIndex;            \
    boot_debugTrap();                        \
    gDrawBufferIndex = 1 - gDrawBufferIndex;

/**
 * Walks a display list (up to end, or up to its G_ENDDL when end is 0), following
 * G_DL branches, and reports malformed commands: bad branch targets, nesting
 * deeper than 20, corrupt G_VTX/G_TRI1 commands, and RDP state changes made after
 * drawing without a G_RDPPIPESYNC. Each report dumps the commands around the
 * culprit with debug_dumpDlAround. parent (the calling G_DL command) is unused.
 */
void debug_walkCheckDl(void *dl, u32 end, Gfx *parent) {
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
                isBad = debug_isBadDlCmd(gfx);
                segment = (addr >> 24) & 0xF;
                addr &= 0xFFFFFF;
                addr += gDlCheckSegments[segment];
                addr |= 0x80000000;
                gDlCheckDepth++;
                if (gDlCheckDepth == 20) {
                    debug_dumpDlAround(gfx);
                    DL_ERROR();
                }
                if (addr > 0x80000000 && addr < 0xA0000000 && !(addr & 7)) {
                    debug_walkCheckDl((void *)addr, 0, gfx);
                } else {
                    debug_dumpDlAround(gfx);
                    DL_ERROR_FLIP();
                }
                if (gDlCheckDepth > 0) {
                    gDlCheckDepth--;
                } else {
                    DL_ERROR_FLIP();
                }
                break;
            case (s8)G_RDPPIPESYNC:
                gDlCheckDrawn = 0;
                break;
            case (s8)G_TEXRECT: case (s8)G_TEXRECTFLIP: case (s8)G_FILLRECT: case G_TRI2: case G_QUAD:
            // raw RDP triangle commands
            case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
            case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F:
                gDlCheckDrawn = 1;
                break;
            case G_TRI1:
                isBad = debug_isBadTri1Cmd(gfx);
                gDlCheckDrawn = 1;
                break;
            case G_VTX:
                isBad = debug_isBadVtxCmd(gfx);
                break;
            case (s8)G_SETTIMG:
                gDlCheckLastTImg = gfx->words.w1;
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
                        gDlCheckSegments[offset / 4] = data;
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
        if (needsPipeSync && gDlCheckDrawn) {
            debug_dumpDlAround(gfx);
            gDlCheckDrawn = 0;
            DL_ERROR_FLIP();
        }
        if (isBad) {
            isBad = 0;
            debug_dumpDlAround(gfx);
            DL_ERROR_FLIP();
        }
        gfx++;
    }
}

/** Returns whether a display list address is not 8-byte aligned. */
s32 debug_isMisalignedDl(s32 addr) {
    if (addr & 7) {
        return 1;
    }
    return 0;
}

/** Command handler for every opcode without its own entry (empty). */
void debug_cmdDefault(Gfx *gfx) {
}

/**
 * G_VTX check: returns nonzero when the first vertex index (v0 + n minus n)
 * is negative or the vertex source address is misaligned.
 */
s32 debug_isBadVtxCmd(Gfx *gfx) {
    s32 first = (gfx->words.w0 >> 1) & 0x7F; // v0 + n
    s32 count = (gfx->words.w0 >> 12) & 0xFF;

    first -= count;
    if (first < 0) {
        return 1;
    }
    return debug_isMisalignedDl(gfx->words.w1);
}

/** G_VTX command handler (empty). */
void debug_cmdVtx(void) {
}

/** G_MODIFYVTX command handler (empty). */
void debug_cmdModifyVtx(Gfx *gfx) {
}

/** G_CULLDL command handler (empty). */
void debug_cmdCullDl(Gfx *gfx) {
}

/** G_BRANCH_Z command handler (empty). */
void debug_cmdBranchZ(Gfx *gfx) {
}

/** G_LINE3D command handler (empty). */
void debug_cmdLine3D(Gfx *gfx) {
}

/** G_RDPHALF_1 command handler (empty). */
void debug_cmdRdpHalf1(Gfx *gfx) {
}

/** G_RDPHALF_2 command handler (empty). */
void debug_cmdRdpHalf2(Gfx *gfx) {
}

/** G_SETKEYGB command handler (empty). */
void debug_cmdSetKeyGB(Gfx *gfx) {
}

/** G_SETKEYR command handler (empty). */
void debug_cmdSetKeyR(Gfx *gfx) {
}

/** G_QUAD command handler (empty). */
void debug_cmdQuad(Gfx *gfx) {
}

/** G_TRI2 command handler (empty). */
void debug_cmdTri2(Gfx *gfx) {
}

/** Command handler for opcodes 0x10-0x1F (empty). */
void debug_cmdTri4(Gfx *gfx) {
}

/**
 * G_TRI1 check: the three vertex indices are stored doubled, so an odd byte
 * means a corrupt command. Returns nonzero in that case.
 */
s32 debug_isBadTri1Cmd(Gfx *tri) {
    s32 v0 = (tri->words.w0 >> 16) & 0xFF;
    s32 v1 = (tri->words.w0 >> 8) & 0xFF;
    s32 v2 = tri->words.w0 & 0xFF;

    if ((v0 & 1) || (v1 & 1) || (v2 & 1)) {
        return 1;
    }
    return 0;
}

/** G_TRI1 command handler (empty). */
void debug_cmdTri1(Gfx *gfx) {
}

/** G_LOADTLUT command handler (empty). */
void debug_cmdLoadTlut(Gfx *gfx) {
}

/** G_LOADTILE command handler (empty). */
void debug_cmdLoadTile(Gfx *gfx) {
}

/** G_RDPTILESYNC command handler (empty). */
void debug_cmdRdpTileSync(Gfx *gfx) {
}

/** G_RDPFULLSYNC command handler (empty). */
void debug_cmdRdpFullSync(Gfx *gfx) {
}

/** G_RDPPIPESYNC command handler (empty). */
void debug_cmdRdpPipeSync(Gfx *gfx) {
}

/** G_RDPLOADSYNC command handler (empty). */
void debug_cmdRdpLoadSync(Gfx *gfx) {
}

/** G_SETCOMBINE command handler (empty). */
void debug_cmdSetCombine(Gfx *gfx) {
}

/** G_DL check: returns nonzero when the branch target is misaligned. */
s32 debug_isBadDlCmd(Gfx *gfx) {
    return debug_isMisalignedDl(gfx->words.w1);
}

/** G_DL command handler (empty). */
void debug_cmdDl(Gfx *gfx) {
}

/** G_SETOTHERMODE_H command handler (empty). */
void debug_cmdSetOtherModeH(Gfx *gfx) {
}

/** G_SETOTHERMODE_L command handler (empty). */
void debug_cmdSetOtherModeL(Gfx *gfx) {
}

/** G_SETCONVERT command handler (empty). */
void debug_cmdSetConvert(Gfx *gfx) {
}

/** G_SETSCISSOR command handler (empty). */
void debug_cmdSetScissor(Gfx *gfx) {
}

/** G_NOOP command handler (empty). */
void debug_cmdNoop(Gfx *gfx) {
}

/** G_SPNOOP command handler (empty). */
void debug_cmdSpNoop(Gfx *gfx) {
}

/** G_SETCIMG command handler (empty). */
void debug_cmdSetCImg(Gfx *gfx) {
}

/** G_SETZIMG command handler (empty). */
void debug_cmdSetZImg(Gfx *gfx) {
}

/** G_SETTIMG command handler (empty). */
void debug_cmdSetTImg(Gfx *gfx) {
}

/** G_SETENVCOLOR command handler (empty). */
void debug_cmdSetEnvColor(Gfx *gfx) {
}

/** G_SETPRIMCOLOR command handler (empty). */
void debug_cmdSetPrimColor(Gfx *gfx) {
}

/** G_SETBLENDCOLOR command handler (empty). */
void debug_cmdSetBlendColor(Gfx *gfx) {
}

/** G_SETFOGCOLOR command handler (empty). */
void debug_cmdSetFogColor(Gfx *gfx) {
}

/** G_SPECIAL_3 command handler (empty). */
void debug_cmdSpecial3(Gfx *gfx) {
}

/** G_SPECIAL_2 command handler (empty). */
void debug_cmdSpecial2(Gfx *gfx) {
}

/** G_SPECIAL_1 command handler (empty). */
void debug_cmdSpecial1(Gfx *gfx) {
}

/** G_DMA_IO command handler (empty). */
void debug_cmdDmaIo(Gfx *gfx) {
}

/** G_SETFILLCOLOR command handler (empty). */
void debug_cmdSetFillColor(Gfx *gfx) {
}

/** G_FILLRECT command handler (empty). */
void debug_cmdFillRect(Gfx *gfx) {
}

/** G_TEXRECT command handler (empty). */
void debug_cmdTexRect(Gfx *gfx) {
}

/** G_TEXRECTFLIP command handler (empty). */
void debug_cmdTexRectFlip(Gfx *gfx) {
}

/** G_LOADBLOCK command handler (empty). */
void debug_cmdLoadBlock(Gfx *gfx) {
}

/** G_SETTILE command handler (empty). */
void debug_cmdSetTile(Gfx *gfx) {
}

/** G_SETTILESIZE command handler (empty). */
void debug_cmdSetTileSize(Gfx *gfx) {
}

/** G_ENDDL command handler (empty). */
void debug_cmdEndDl(Gfx *gfx) {
}

/** G_POPMTX command handler (empty). */
void debug_cmdPopMtx(Gfx *gfx) {
}

/** G_MTX command handler (empty). */
void debug_cmdMtx(Gfx *gfx) {
}

/** G_GEOMETRYMODE command handler (empty). */
void debug_cmdGeometryMode(Gfx *gfx) {
}

void func_15040A4C(void) {
}

/** G_LOAD_UCODE command handler (empty). */
void debug_cmdLoadUcode(Gfx *gfx) {
}

/** G_MOVEMEM command handler (empty). */
void debug_cmdMoveMem(Gfx *gfx) {
}

/** G_MOVEWORD command handler (empty). */
void debug_cmdMoveWord(Gfx *gfx) {
}

/**
 * Walks the display list at start, following G_DL branches, and runs the
 * gDlCmdHandlers handler of every command from `from` through `to` (inclusive).
 * Also tracks G_MW_SEGMENT movewords in gDlCheckSegments to resolve branch targets.
 */
void debug_dumpDlRange(Gfx *start, Gfx *from, Gfx *to) {
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
        gDlCheckSegments[i] = -1;
        i++;
    } while (i < 16);
    gDlCheckSegments[0] = 0;
    inRange = 0;
    gfx = start;
    while (gfx <= to || depth > 0) {
        if (gfx == from) {
            inRange = 1;
        }
        cmd = gfx->dma.cmd;
        if (inRange) {
            gDlCmdHandlers[cmd](gfx);
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
                if (gDlCheckSegments[segment] != -1) {
                    w1 &= 0xFFFFFF;
                    w1 += gDlCheckSegments[segment];
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
                    gDlCheckSegments[(s32)(w0 & 0xFFFF) >> 2] = data;
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
                mtx = (gDlCheckSegments[(gfx->words.w1 >> 24) & 0xF] + (gfx->words.w1 & 0xFFFFFF)) | 0x80000000;
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
 * Dumps the commands from 20 before gfx to 9 after it through the gDlCmdHandlers
 * handlers, then passes the last G_SETTIMG address to level_findTextureByAddr.
 */
void debug_dumpDlAround(Gfx *gfx) {
    s32 i;
    s32 addr; // matching: a Gfx * local here changes the generated code
    s32 lastTexture;
    void (**handlers)();

    for (i = 0; i < 16; i++) {
        // empty: the loop itself is emitted
    }

    handlers = gDlCmdHandlers;
    // matching: "| 0" keeps IDO from rewriting the exit test as a bne against 10 held in a register
    for (i = -20; i < 10; i = (i + 1) | 0) {
        addr = (s32)&gfx[i];
        handlers[*(u8 *)addr](addr);
    }

    lastTexture = gDlCheckLastTImg;
    if (lastTexture != 0) {
        level_findTextureByAddr(lastTexture);
    }
}

/** G_SETPRIMDEPTH command handler (empty). */
void debug_cmdSetPrimDepth(Gfx *gfx) {
}

/** G_TEXTURE command handler (empty). */
void debug_cmdTexture(Gfx *gfx) {
}

/** G_RDPSETOTHERMODE command handler (empty). */
void debug_cmdRdpSetOtherMode(Gfx *gfx) {
}
