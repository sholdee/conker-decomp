#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15040350(void *arg0, u32 arg1) {
    s32 i;

    D_800C68A0 = D_800848B0 = 0;
    D_800C68A1 = 0;

    for (i = 0; i < 16; i++) {
        D_800C6860[i] = -1;
    }
    D_800C6860[0] = 0;
    func_150403C8(arg0, arg1, 0);
}

// contains jump relative
void func_10007DA0(void);
s32 func_15040754(Gfx *gfx);
void func_15040CC8(u8 *arg0);

#define FLUSH()                              \
    D_8003C8E0 = 0xC00005B;                  \
    func_150AD770();                         \
    D_800BE9C0 = 1 - D_800BE9C0;             \
    func_10007DA0();                         \
    D_800BE9C0 = 1 - D_800BE9C0;

void func_150403C8(void *arg0, u32 arg1, s32 arg2) {
    u32 *gfx;
    s32 pad[2];
    s32 sp40 = 0;
    s32 sync;
    u32 addr;
    u32 hi;

    gfx = arg0;
    while (((arg1 == 0) && (*(s8 *)gfx != -0x21)) || ((arg1 != 0) && ((u32)gfx < arg1))) {
        sync = 0;
        switch (*(s8 *)gfx) {
            case -0x22:
                addr = gfx[1];
                sp40 = func_150408CC((struct148 *)gfx);
                hi = (addr >> 24) & 0xF;
                addr &= 0xFFFFFF;
                addr += D_800C6860[hi];
                addr |= 0x80000000;
                D_800C68A1++;
                if (D_800C68A1 == 20) {
                    func_15040CC8((u8 *)gfx);
                    D_8003C8E0 = 0xC00005B;
                    func_150AD770();
                }
                if ((addr > 0x80000000) && (addr < 0xA0000000) && !(addr & 7)) {
                    func_150403C8((void *)addr, 0, (s32)gfx);
                } else {
                    func_15040CC8((u8 *)gfx);
                    FLUSH();
                }
                if (D_800C68A1 > 0) {
                    D_800C68A1--;
                } else {
                    FLUSH();
                }
                break;
            case -0x19:
                D_800C68A0 = 0;
                break;
            case -0x1C: case -0x1B: case -0x0A: case 0x06: case 0x07:
            case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
            case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F:
                D_800C68A0 = 1;
                break;
            case 0x05:
                sp40 = func_1504082C(gfx);
                D_800C68A0 = 1;
                break;
            case 0x01:
                sp40 = func_15040754((Gfx *)gfx);
                break;
            case -0x03:
                D_800848B0 = gfx[1];
                break;
            case -0x01:
                sync = 1;
                break;
            case -0x1E: case -0x1D: case -0x14: case -0x13: case -0x12:
            case -0x09: case -0x08: case -0x07: case -0x05: case -0x04: case -0x02:
                sync = 1;
                break;
            case -0x16: case -0x15:
                sync = 1;
                break;
            case -0x25:
                {
                    u32 type;
                    s32 off;
                    s32 w1;
                    type = gfx[0] >> 16;
                    off = gfx[0] & 0xFFFF;
                    w1 = gfx[1];
                    if ((type & 0xFF) == 6) {
                        D_800C6860[off / 4] = w1;
                    }
                }
                break;
            case -0x2D: case -0x2C: case -0x2B: case -0x2A: case -0x29: case -0x28: case -0x27: case -0x26:
            case -0x24: case -0x23: case -0x21: case -0x20: case -0x1F: case -0x1A: case -0x18: case -0x17:
            case -0x11: case -0x10: case -0x0F: case -0x0E: case -0x0D: case -0x0C: case -0x0B:
            case -0x06: case 0x00: case 0x02: case 0x03: case 0x04: case 0x08:
                break;
            default:
                D_8003C8E0 = 0xC00005B;
                func_150AD770();
                break;
        }
        if (sync && D_800C68A0) {
            func_15040CC8((u8 *)gfx);
            D_800C68A0 = 0;
            FLUSH();
        }
        if (sp40) {
            sp40 = 0;
            func_15040CC8((u8 *)gfx);
            FLUSH();
        }
        gfx += 2;
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
s32 func_1504082C(u32 *w0) {
    s32 v0 = (*w0 >> 16) & 0xFF;
    s32 v1 = (*w0 >> 8) & 0xFF;
    s32 v2 = *w0 & 0xFF;

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
s32 func_150408CC(struct148 *arg0) {
    return func_1504072C(arg0->unk4);
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

// contains delay slot
typedef struct { u32 pad:4; u32 seg:4; u32 off:24; } SegAddr;
void func_15040A78(u32 *arg0, u32 *arg1, u32 *arg2) {
    s32 i;
    u32 *dl;
    u32 w0;
    u32 w1;
    s32 n;
    u32 *stack[16];
    u8 cmd;
    u8 active;
    u32 seg;
    u32 idx;
    u32 off;
    u32 data;
    u32 mtx;
    s32 depth;
    s32 pada;
    s32 padb;
    s32 padc;
    s32 padd;

    depth = 0;
    i = 0;
    do {
        D_800C6860[i] = -1;
        i++;
    } while (i < 16);
    D_800C6860[0] = 0;
    active = 0;
    dl = arg0;
    while (dl <= arg2 || depth > 0) {
        if (dl == arg1) {
            active = 1;
        }
        cmd = *(u8 *)dl;
        if (active) {
        }
        if (active) {
            D_800844B0[cmd]((s32)dl);
        }
        if (dl == arg2) {
            active = 0;
        }
        switch (cmd) {
            case 0xDF:
                depth--;
                dl = stack[depth];
                break;
            case 0xDE:
                stack[depth] = dl + 2;
                w1 = dl[1];
                if (active) {
                }
                depth++;
                if (active) {
                }
                seg = (w1 >> 24) & 0xF;
                if (D_800C6860[seg] != -1) {
                    w1 &= 0xFFFFFF;
                    w1 += D_800C6860[seg];
                    dl = (u32 *)(w1 | 0x80000000);
                } else {
                    dl += 2;
                    depth--;
                }
                break;
            case 0xDB:
                w0 = dl[0];
                data = dl[1];
                if ((w0 & 0xFFFF) == (w0 >> 16)) {
                }
                if (((w0 >> 16) & 0xFF) == 6) {
                    D_800C6860[(s32)(w0 & 0xFFFF) >> 2] = data;
                }
                dl += 2;
                break;
            case 1:
                if (active) {
                    n = (dl[0] >> 12) & 0xFF;
                    for (i = 0; i < n; i++) {
                    }
                }
                dl += 2;
                break;
            case 0xDA:
                mtx = (D_800C6860[(dl[1] >> 24) & 0xF] + (dl[1] & 0xFFFFFF)) | 0x80000000;
                if (mtx == 0) {
                }
                dl += 2;
                break;
            case 0xDC:
                dl += 2;
                break;
            default:
                dl += 2;
                break;
        }
    }
}

void func_15040CC8(u8 *arg0) {
    s32 i;
    s32 offset;
    s32 addr;
    s32 temp;
    void (**table)(s32);

    for (i = 0; i < 16; i++) {
    }

    table = D_800844B0;
    for (i = -20; i < 10; i = (i + 1) | 0) {
        offset = i << 3;
        addr = (s32) (offset + arg0);
        table[*(u8 *) addr](addr);
    }

    temp = D_800848B0;
    if (temp != 0) {
        func_1500390C(temp);
    }
}
// NON-MATCHING: todo
// void func_15040CC8(s32 *arg0) {
//     s32 temp_a0_2;
//     s32 temp_s0;
//     s32 temp_s0_2;
//     s32 *temp_a0;
//     s32 phi_s0;
//     s32 phi_s0_2;
//
//     for (phi_s0 = 0; phi_s0 < 16; phi_s0++)
//     {
//         // just waste time?
//     }
//
//     phi_s0_2 = -0x14;
// loop_3:
//     temp_a0 = arg0[phi_s0_2]; // * 8) + arg0;
//     D_800844B0[*temp_a0](temp_a0);
//     temp_s0_2 = phi_s0_2 + 1;
//     phi_s0_2 = temp_s0_2;
//     if (temp_s0_2 < 0xA) {
//         goto loop_3;
//     }
//     temp_a0_2 = D_800848B0;
//     if (temp_a0_2 != 0) {
//         func_1500390C(temp_a0_2);
//     }
// }

/** G_SETPRIMDEPTH command handler (empty). */
void func_15040D60(Gfx *gfx) {
}

/** G_TEXTURE command handler (empty). */
void func_15040D6C(Gfx *gfx) {
}

/** G_RDPSETOTHERMODE command handler (empty). */
void func_15040D78(Gfx *gfx) {
}
