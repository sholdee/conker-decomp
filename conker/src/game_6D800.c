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
s32 func_15040754(struct148 *arg0);
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
                sp40 = func_15040754((struct148 *)gfx);
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

s32 func_1504072C(s32 arg0) {
    if ((arg0 & 7) != 0) {
        return 1;
    }
    return 0;
}

void func_15040748(s32 arg0) {
}

s32 func_15040754(struct148 *arg0) { // overly simple
    s32 temp_a2;
    s32 temp_a1;

    temp_a1 = (arg0->unk0 >> 1) & 0x7F;
    temp_a2 = (arg0->unk0 >> 12) & 0xFF;
    temp_a1 = temp_a1 - temp_a2;

    if (temp_a1 < 0) {
        return 1;
    }
    return func_1504072C(arg0->unk4);
}

void func_150407A0(void) {
}

void func_150407A8(s32 arg0) {
}

void func_150407B4(s32 arg0) {
}

void func_150407C0(s32 arg0) {
}

void func_150407CC(s32 arg0) {
}

void func_150407D8(s32 arg0) {
}

void func_150407E4(s32 arg0) {
}

void func_150407F0(s32 arg0) {
}

void func_150407FC(s32 arg0) {
}

void func_15040808(s32 arg0) {
}

void func_15040814(s32 arg0) {
}

void func_15040820(s32 arg0) {
}

s32 func_1504082C(u32 *arg0) {
    s32 tmp0 = (*arg0 >> 16) & 0xff;
    s32 tmp1 = (*arg0 >> 8) & 0xff;
    s32 tmp2 = (*arg0 >> 0) & 0xff;

    if ((tmp0 & 1) || (tmp1 & 1) || (tmp2 & 1)) {
        return 1;
    } else {
        return 0;
    }
}

void func_1504086C(s32 arg0) {
}

void func_15040878(s32 arg0) {
}

void func_15040884(s32 arg0) {
}

void func_15040890(s32 arg0) {
}

void func_1504089C(s32 arg0) {
}

void func_150408A8(s32 arg0) {
}

void func_150408B4(s32 arg0) {
}

void func_150408C0(s32 arg0) {
}

s32 func_150408CC(struct148 *arg0) {
    func_1504072C(arg0->unk4);
}

void func_150408F0(s32 arg0) {
}

void func_150408FC(s32 arg0) {
}

void func_15040908(s32 arg0) {
}

void func_15040914(s32 arg0) {
}

void func_15040920(s32 arg0) {
}

void func_1504092C(s32 arg0) {
}

void func_15040938(s32 arg0) {
}

void func_15040944(s32 arg0) {
}

void func_15040950(s32 arg0) {
}

void func_1504095C(s32 arg0) {
}

void func_15040968(s32 arg0) {
}

void func_15040974(s32 arg0) {
}

void func_15040980(s32 arg0) {
}

void func_1504098C(s32 arg0) {
}

void func_15040998(s32 arg0) {
}

void func_150409A4(s32 arg0) {
}

void func_150409B0(s32 arg0) {
}

void func_150409BC(s32 arg0) {
}

void func_150409C8(s32 arg0) {
}

void func_150409D4(s32 arg0) {
}

void func_150409E0(s32 arg0) {
}

void func_150409EC(s32 arg0) {
}

void func_150409F8(s32 arg0) {
}

void func_15040A04(s32 arg0) {
}

void func_15040A10(s32 arg0) {
}

void func_15040A1C(s32 arg0) {
}

void func_15040A28(s32 arg0) {
}

void func_15040A34(s32 arg0) {
}

void func_15040A40(s32 arg0) {
}

void func_15040A4C(void) {
}

void func_15040A54(s32 arg0) {
}

void func_15040A60(s32 arg0) {
}

void func_15040A6C(s32 arg0) {
}

// contains delay slot
#pragma GLOBAL_ASM("asm/nonmatchings/game_6D800/func_15040A78.s")

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

void func_15040D60(s32 arg0) {
}

void func_15040D6C(s32 arg0) {
}

void func_15040D78(s32 arg0) {
}
