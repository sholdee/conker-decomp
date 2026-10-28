#include <ultra64.h>
#define func_1000F85C func_1000F85C_hdr
#define func_1000EDA0 func_1000EDA0_hdr
#define func_1000EE70 func_1000EE70_hdr
#define func_10010BE8 func_10010BE8_hdr
#define func_10010344 func_10010344_hdr
#define func_10010154 func_10010154_hdr
#include "functions.h"
#undef func_1000F85C
#undef func_1000EDA0
#undef func_1000EE70
#undef func_10010BE8
#undef func_10010344
#undef func_10010154
s32 func_10010344(u16, void *, u32, s16, u16);
#include "variables.h"

u16 func_10010E78(u16 arg0, s32 arg1, u16 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9,
                   s32 argA);
void func_10011624(struct15 *arg0, s32 *arg1, s32 arg2, s32 arg3);
s32 func_1510F8CC(s32 arg0);
s32 func_1000F568(s32 arg0, s32 arg1);

typedef struct {
    u16 unk0;
    u16 unk2;
} struct_init_EB00_10011EB8_pair;

typedef struct {
    struct_init_EB00_10011EB8_pair unk0[5];
} struct_init_EB00_10011EB8_row;

typedef struct {
    u16 unk0;
    u8 pad2[6];
    u16 unk8;
    s16 unkA;
    s32 unkC;
    u8 pad10[8];
    union {
        s32 w;
        u16 h[2];
    } unk18;
    struct127 *unk1C;
} struct_init_EB00_1000ECCC;

s32 func_1000ECCC(struct_init_EB00_1000ECCC *arg0, s32 arg1, s32 arg2, s32 arg3,
                  s32 arg4, s32 arg5, u16 *arg6);

s32 func_1000EB00(struct04 *arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg6, s32 arg7, u16 *arg8) {
    if (arg0->unk24 != 0) {
        arg0->unk24 = 0;
    }
    *arg3 = 64;
    if (D_800CC37D || (*arg2 == 0)) {
        *arg2 = 0;
        *arg8 = 0;
        return 0;
    }
    arg0->unk18 -= D_800BE9E4;
    if (arg0->unk18 > 0) {
        *arg2 = 0;
        *arg8 = 0;
    } else {
        arg0->unk18 = (func_150ADA20() & 0x7F) + 0x80;
        arg0->unk0 = (func_150ADA20() % 3U) + 0x6C;
    }
    return 0;
}

s32 func_1000EBC4(struct00 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0 = arg0->unkC;
    s32 temp_v1 = arg0->unk18;
    temp_v1 -= D_800BE9E4;

    if (temp_v1 <= 0) {
        temp_v0 = temp_v0 - D_800BE9E4 * 1000;
        if (temp_v0 < 0) {
            return 1;
        }
        arg0->unkC = temp_v0;
    }
    arg0->unk18 = temp_v1;
    return 0;
}

s32 func_1000EC24(struct251 *arg0, s32 arg1, s32 *arg2, struct11 *arg3, struct04 *arg4, s32 *arg5, u16 *arg6) {
    s16 temp_v1 = arg0->unk18.h[1];

    if (*arg6 != 0) {
        arg0->unk1C = *arg6;
        arg0->unk0 = 0;
        *arg6 = 0;
    }

    temp_v1 -= D_800BE9E4;

    if (temp_v1 <= 0) {
        if (*arg2 != 0) {
            func_10010F30(arg0->unk1C, *arg2, arg3->unk3, arg4->unk2, *arg5);
        }
        return 1;
    }
    arg0->unk18.w = temp_v1;
    return 0;
}

s32 func_1000ECCC(struct_init_EB00_1000ECCC *arg0, s32 arg1, s32 arg2, s32 arg3,
                  s32 arg4, s32 arg5, u16 *arg6) {
    s16 temp_a1;
    s16 temp_t4;
    s32 temp_v1;

    temp_v1 = arg0->unk18.w;
    temp_a1 = temp_v1;
    if (*arg6 != 0) {
        arg0->unk18.w = (*arg6 << 16) | (temp_v1 & 0xFFFF);
        arg0->unk0 = 0;
        *arg6 = 0;
        temp_v1 = arg0->unk18.w;
    }

    temp_a1 -= D_800BE9E4;
    temp_t4 = temp_v1 >> 16;
    if (temp_a1 <= 0) {
        *arg6 = temp_t4;
        arg0->unk0 = temp_t4;
        if (func_10010894(arg0->unk1C) == 0) {
            func_10010344(*arg6, arg0->unk1C, arg0->unkC, arg0->unkA, arg0->unk8);
        }
        return 1;
    }

    arg0->unk18.w = (temp_v1 & 0xFFFF0000) | temp_a1;
    return 0;
}
// ? func_1000ECCC(void *arg0, ? arg1, ? arg2, ? arg3, void *arg6) {
//     s16 temp_a1;
//     s32 temp_t4;
//     s32 temp_v1;
//     u16 temp_v0;
//
//     temp_v1 = arg0->unk18;
//     temp_v0 = *arg6;
//     if (temp_v0 != 0) {
//         arg0->unk18 = (s32) ((temp_v0 << 0x10) | (temp_v1 & 0xFFFF));
//         arg0->unk0 = (u16)0;
//         *arg6 = (u16)0U;
//     }
//     temp_a1 = (s16) temp_v1 - *(void *)0x800BE9E4;
//     if ((s32) temp_a1 <= 0) {
//         temp_t4 = arg0->unk18 >> 0x10;
//         *arg6 = (u16) temp_t4;
//         arg0->unk0 = (s16) temp_t4;
//         if (func_10010894(arg0->unk1C, temp_a1, arg6) == 0) {
//             func_10010344(*arg6, arg0->unk1C, arg0->unkC, arg0->unkA, (?32) arg0->unk8);
//         }
//         return 1;
//     }
//     arg0->unk18 = (s32) ((arg0->unk18 & 0xFFFF0000) | temp_a1);
//     return 0;
// }

s32 func_1000EDA0(struct_init_EB00_1000ECCC *arg0, s32 arg1, s32 arg2, s32 arg3,
                  s32 arg4, s32 arg5, u16 *arg6) {
    s16 temp_a1;
    s32 temp_v1;

    temp_v1 = arg0->unk18.w;
    temp_a1 = temp_v1;
    if (*arg6 != 0) {
        arg0->unk18.w = (*arg6 << 16) | (temp_v1 & 0xFFFF);
        arg0->unk0 = 0;
        *arg6 = 0;
        temp_v1 = arg0->unk18.w;
    }
    temp_a1 -= D_800BE9E4;
    if (temp_a1 <= 0) {
        *arg6 = temp_v1 >> 16;
        arg0->unk0 = *arg6;
        func_10010630(*arg6, arg0->unk1C, arg0->unkC, arg0->unkA, arg0->unk8);
        return 1;
    }
    arg0->unk18.w = (temp_v1 & 0xFFFF0000) | temp_a1;
    return 0;
}


typedef struct {
    u8 pad0[2];
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 pad8[0x10];
    struct127 *unk18;
    s32 unk1C;
    u8 pad20[4];
    u16 unk24;
} struct_init_EB00_EE70;

s32 func_1000EE70(struct_init_EB00_EE70 *arg0, s32 arg1, s32 *arg2, s32 arg3, s32 arg4, s32 *arg5) {
    struct127 *obj;
    u8 uid;

    if ((obj = arg0->unk18) != 0) {
        if (*arg2 != 0) {
            uid = arg0->unk1C & 0xFF;
            if ((obj->interaction_state != 0) && (uid == obj->unique_id)) {
                *arg5 = (((u32) obj->unk184 >> 3) & 0x30) << 1;
                arg0->unk2 = (s16) obj->x_position;
                arg0->unk4 = (s16) obj->y_position;
                arg0->unk6 = (s16) obj->z_position;
                return 0;
            }
            if (func_1000F44C(arg0->unk24) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

s32 func_1000EF40(struct57 *arg0, struct57 *arg1, s32 *arg2, s32 arg3, s32 arg4, s32 arg5, u16 *arg6) {
    if (arg0->unk10 & 0x80) {
        arg0->unk10 = (s32) (arg0->unk10 & -0x81);
    }
    if (*arg2 == 0) {
        if (arg0->unk24 != 0) {
            func_100111C8(arg0->unk24);
            arg0->unk24 = 0;
        }
        *arg6 = 0;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/init_EB00/func_1000EFB4.s")
void func_1000F1A8(void) {
    s32 i;

    D_80042760 = 0;
    D_80041FD9 = 1;
    D_80041FD8 = 0;
    bzero(D_800425E0, 0x180);

    for (i = 0; i < 0x10; i++) {
        D_800425E0[i].unk2 = i + 0x10;
    }

    D_80041F50 = 0;
    func_100176EC();
    D_80041F60 = D_80041F61 = 0;
}

void func_10017780(u8 arg0, u16 arg1);
s32* allocate_memory(s32, s32, s32, s32);
void func_1000F248(s32 arg0) {
    u16 tmp;

    func_1000F1A8();
    if (arg0 == 4) {
        D_80041F54 = 0;
        D_80041F58 = D_8002C3F8; // 0.009999999776482582
    } else {
        D_80041F54 = 23000;
        D_80041F58 = D_8002C3FC; // 0.10000000149011612
    }

    if (arg0 == 0x35) {
        D_80041FD9 = 0;
    } else if (arg0 == 0x36) {
        D_80041FD9 = 0;
    } else if (arg0 == 0x3C) {
        D_80041FD9 = 0;
        D_80041FD8 = 0x3C;
    } else if (arg0 == 0x27) {
        D_80041FD9 = 0;
        D_80041FD8 = 0x28;
    } else if ((arg0 == 0x3A) || (arg0 == 0x40)) {
        D_80041FD9 = 0;
        D_80041FD8 = 0x3C;
    }

    D_80041F5C = allocate_memory(1762, 1, 0, 0);

    bzero(D_80041F5C, 1762);

    if (arg0 == 0x31) {
        D_80041FDC = 14000;
    } else {
        D_80041FDC = 23000;
    }
    func_10011E88(arg0);
    D_80041F60 = D_80041F61 = 0;
    func_10017780(0, D_80041F56);
    func_10017780(1, D_80041F56);
    func_10017780(2, 23000);
    // fakematch
    if (arg0) {}
}

s32 func_1000F3D0(u16 arg0) {
    struct120 *temp_v1;

    temp_v1 = &D_800425E0[arg0 & 0xF];
    if (temp_v1->unk8 != 0) {
        if ((temp_v1->unk0 == arg0) || (arg0 == (arg0 & 0xF))) {
            if (func_100173C4(&temp_v1->unk8) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_1000F44C(u16 arg0) {
    u32 mask;
    struct120 *temp_a1;
    struct31 *temp_a2;

    mask = __osDisableInt();
    temp_a1 = &D_800425E0[arg0 & 0xF];
    temp_a2 = temp_a1->unk8;
    if ((temp_a2 != 0) && (temp_a1->unk0 == arg0) && ((temp_a2->unk53 & 2) != 0)) {
        __osRestoreInt(mask);
        return 1;
    }
    __osRestoreInt(mask);
    return 0;
}

s32 func_100173C4(struct31 **arg0);
extern struct120 D_800425E0[];
extern struct120 D_800426A0[];
s32 func_1000F4D8(u16 arg0)
{
  struct120 *temp_s0;
  arg0 &= 0x7FFF;
 temp_s0 = D_800425E0; do { if (temp_s0->unk8 != 0) { if ((temp_s0->unk4 & 0x7FFF) == arg0) {
        if (func_100173C4(&temp_s0->unk8) != 0)
        {
          return 1;
        }
      }
    }
    temp_s0++;
  }
  while (temp_s0 != D_800426A0);
  return 0;
}

s32 func_1000F568(s32 arg0, s32 arg1) {
    s32 r;
    s32 i;
    u8 bits;
    u8 cur;
    s32 mask;

    r = func_150ADA20() % (u32)arg1;
    i = r;
    if (arg0 >= 0x6E2) {
        return 1;
    }
    if (arg1 < 2) {
        return arg0;
    }
    if (D_80041F5C != NULL) {
        bits = ((u8 *)D_80041F5C)[arg0];
        if (arg1 < 8) {
            mask = (1 << arg1) - 1;
            if (!(bits & 0x80) || !(bits & mask)) {
                bits = 0xFF;
            }
            while (!(bits & (1 << i))) {
                i = (i + 1) % arg1;
            }
            cur = bits ^ (1 << i);
            ((u8 *)D_80041F5C)[arg0] = cur;
            if (!(cur & mask)) {
                ((u8 *)D_80041F5C)[arg0] = bits ^ mask;
            }
        } else {
            ((u8 *)D_80041F5C)[arg0] = r + 1;
        }
    }
    return arg0 + i;
}
extern s32 func_1000A420(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6,
                         s32 arg7, s32 arg8, s32 *arg9, s32 *argA, s32 *argB);

s32 func_1000F6B8(s32 arg0, s16 arg1, s16 arg2, s16 arg3,
                   s32 *output, s32 nearDistance, s32 farDistance) {
    s32 result;
    struct00 *selected;
    s32 index;
    s32 dx;
    s32 dy;
    s32 bestX;
    s32 bestY;
    s32 bestZ;
    s32 dz;
    u32 distance;

    if (D_80082FA0 != 0) {
        u32 closest;

        closest = 0xFFFFFFFF;
        for (index = 0; index <= D_80082FA0; index++) {
            dx = arg1 - D_80041F68[index].unkC;
            dy = arg2 - D_80041F68[index].unk10;
            dz = arg3 - D_80041F68[index].unk14;
            distance = dx * dx + dy * dy + dz * dz;
            if (distance < closest) {
                closest = distance;
                selected = &D_80041F68[index];
                bestX = dx;
                bestY = dy;
                bestZ = dz;
            }
        }
    } else {
        selected = D_80041F68;
        bestX = arg1 - selected->unkC;
        bestY = arg2 - selected->unk10;
        bestZ = arg3 - selected->unk14;
    }
    func_1000A420(bestX, bestY, bestZ, selected->unk18,
                 arg1 - selected->unk0, arg2 - selected->unk4, arg3 - selected->unk8,
                 (s16)farDistance, (s16)nearDistance, output, &result, 0);
    return result;
}
typedef struct {
    struct31 *unk0;
    u8 pad4[8];
} struct_init_EB00_425E8;
extern struct_init_EB00_425E8 D_800425E8[];

void func_1000F85C(u16 arg0, s16 arg1, s32 arg2) {
    if (arg0 < 0x10) {
        return;
    }
    if (func_1000F3D0(arg0) != 0) {
        if (arg1 == 0x10) {
            f32 sp1C = alCents2Ratio(arg2);
            arg2 = *(s32 *)&sp1C;
        } else if (arg1 == 0x11) {
            arg1 = 0x10;
        }
        func_10017714((s32) D_800425E8[arg0 & 0xF].unk0, arg1, arg2);
    }
}

void func_1000F91C(u16 arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4,
                   s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9) {
    s32 sp2C;
    s32 tmp = func_1000F6B8(arg4, arg5, arg6, arg7, &sp2C, (s32) arg8, (s32) arg9);

    func_1000F85C(arg0, 8, (u32) (tmp * arg1) >> 0xF);
    func_1000F85C(arg0, 4, sp2C & 0x7F);
    func_1000F85C(arg0, 256, (sp2C & 0x80) | arg3);
    func_1000F85C(arg0, 16, arg2);
}

void func_1000F9D4(u16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    u32 tmp;

    func_1000F6B8(-1, arg1, arg2, arg3, &tmp, 32760, 32765);
    func_1000F85C(arg0, 4, tmp & 0x7F);
    func_1000F85C(arg0, 256, tmp & 0x80);
}

u16 func_1000FA64(u16 arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4,
                  u16 arg5, s16 arg6, s32 arg7, void *arg8, s32 arg9,
                  s32 argA, s32 argB) {
    s32 sp24;
    register s32 temp_a2;

    if (D_80042760 < 0x20) {
        temp_a2 = D_80042760;
        D_80042760++;
    } else {
        return 0;
    }

    if (arg7 != 0) {
        D_80041FE0[temp_a2].unk10 = argA | 0x12;
    } else {
        D_80041FE0[temp_a2].unk10 = (argA & 0x108) | 2;
    }

    if (argA & 0x40) {
        arg2 = func_15083E0C((u8)arg1);
        if (arg2 == -1) {
            return 0;
        }
    }

    D_80041FE0[temp_a2].unk24 = 0;
    *((u8 *)&D_80041FE0[temp_a2] + 0x23) = 0;
    *((u8 *)&D_80041FE0[temp_a2] + 0x22) = 0;
    *((u16 *)&D_80041FE0[temp_a2] + 0) = arg0;
    *((s16 *)&D_80041FE0[temp_a2] + 1) = arg1;
    *((s16 *)&D_80041FE0[temp_a2] + 2) = arg2;
    *((s16 *)&D_80041FE0[temp_a2] + 3) = arg3;
    *((u16 *)&D_80041FE0[temp_a2] + 4) = arg5;
    *((s16 *)&D_80041FE0[temp_a2] + 5) = arg6;
    D_80041FE0[temp_a2].unkC = arg4;
    D_80041FE0[temp_a2].unk14 = arg7;
    D_80041FE0[temp_a2].unk18 = (s32)arg8;
    D_80041FE0[temp_a2].unk1C = arg9;
    *(f32 *)&D_80041FE0[temp_a2].unk2C = alCents2Ratio(argB);
    D_80041FE0[temp_a2].unk26 = 0;
    *(u16 *)&D_80041FE0[temp_a2].unk28 = 0;
    *(s16 *)&D_80041FE0[temp_a2].unk20 = argB;
    sp24 = D_80042760;
    func_10011624(D_80041FE0, &D_80042760, temp_a2, temp_a2 + 1);
    if (sp24 == D_80042760) {
        D_80041FE0[temp_a2].unk10 |= 0x1000;
        return D_80041FE0[temp_a2].unk24;
    }
    return 0;
}
typedef struct {
    u16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u16 unk8;
    u8 padA[6];
    s32 unk10;
    u8 pad14[0x10];
    u16 unk24;
    u8 pad26[0xA];
} struct_init_EB00_FC18;

void func_1000FC18(u16 arg0, s16 arg1, s16 arg2, s16 arg3, u16 arg4) {
    s32 id;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    struct_init_EB00_FC18 *ptr;
    u16 handle;

    id = arg0;
    x = arg1;
    y = arg2;
    z = arg3;
    for (i = 0; i < D_80042760; i++) {
        ptr = (struct_init_EB00_FC18 *)&D_80041FE0[i];
        if ((id == ptr->unk0) && (x == ptr->unk2) && (y == ptr->unk4) &&
            (z == ptr->unk6) && ((ptr->unk8 & 0x7FFF) == arg4)) {
            handle = D_80041FE0[i].unk24;
            if (handle != 0) {
                func_100111C8(handle);
            }
            D_80041FE0[i].unk10 |= 0x80;
        }
    }
}
void func_1000FD38(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;

    for (i = 0; i < D_80042760; i++) {
        if ((arg0 == D_80041FE0[i].unk14) && (arg1 == D_80041FE0[i].unk18) &&
            (arg2 == D_80041FE0[i].unk1C)) {
            if (D_80041FE0[i].unk24 != 0) {
                func_100111C8(D_80041FE0[i].unk24);
            }
            D_80041FE0[i].unk10 |= 0x80;
        }
    }
}
// PERMUTER CANDIDATE / JUSTREG (best 40). The index form below is byte-perfect in
// structure, ordering, and every other register EXCEPT the loop count D_80042760:
// target keeps it in v0 (blez/slt/reload), IDO gives mine v1. Pure caller-saved
// coloring; resisted for/while/reversed-compare/explicit-guard/do-while forms (all 40)
// and a fn-ptr-cast (682, adds addr load). Needs the permuter to flip v0<->v1.
void func_1000FDF4(u16 arg0) {
    s32 i;
    struct15 *temp;
    for (i = 0; i < D_80042760; i++) {
        if (D_80041FE0[i].unk24 == arg0) {
            if ((temp = D_80041FE0)[i].unk24) {
                func_100111C8(D_80041FE0[i].unk24);
            }
            D_80041FE0[i].unk10 |= 0x80;
        }
    }
}
s32 func_1000FE88(struct15 *arg0, s32 arg1, s32 *arg2) {
    if (arg1 < *arg2) {
        if (((struct15 *)((u8 *)arg0 + (arg1 * 0x30)))->unk24 != 0) {
            func_100111C8(((struct15 *)((u8 *)arg0 + (arg1 * 0x30)))->unk24);
        }
        ((struct15 *)((u8 *)arg0 + (arg1 * 0x30)))->unk10 |= 0x80;
        return 0;
    }
    return 1;
}
// ? func_1000FE88(s32 arg0, s32 arg1, void *arg2) {
//     void *sp1C;
//     s32 temp_t7;
//     u16 temp_a0;
//     void *temp_v1;
//     ? phi_return;
//
//     phi_return = 1;
//     if (arg1 < *arg2) {
//         temp_t7 = arg1 * 0x30;
//         temp_v1 = arg0 + temp_t7;
//         temp_a0 = temp_v1->unk24;
//         if (temp_a0 != 0) {
//             sp1C = temp_v1;
//             func_100111C8(temp_a0, arg0);
//         }
//         (arg0 + temp_t7)->unk10 = (s32) ((arg0 + temp_t7)->unk10 | 0x80);
//         phi_return = 0;
//     }
//     return phi_return;
// }

s32 func_1000FEF0(u16 arg0, void *arg1, s32 arg2) {
    u16 id;
    s32 i;

    if (!arg0) {
        return -1;
    }
    for (i = 0; i < D_80042760; i++) {
        if ((arg0 == (*(&D_80041FE0[i])).unk24) && ((s32) arg1 == D_80041FE0[i].unk18) &&
            (arg2 == D_80041FE0[i].unk1C) && ((D_80041FE0[i].unk10 & 0x80) == 0)) {
            return i;
        }
    }
    return -1;
}

s32 func_1000FF90(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;

    for (i = 0; i < D_80042760; i++) {
        if ((arg0 == D_80041FE0[i].unk14) &&
            ((arg1 == D_80041FE0[i].unk18) || (arg1 == -1)) &&
            ((arg2 == D_80041FE0[i].unk1C) || ((u32) arg2 == 0xFFFFFFFFU)) &&
            ((D_80041FE0[i].unk10 & 0x80) == 0)) {
            return i;
        }
    }
    return -1;
}
void func_1001001C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 i;
    struct15 *tmp;

    tmp = D_80041FE0;
    i = 0;
    if (D_80042760 > 0) {
        do {
            if ((arg0 == tmp->unk14) && (arg1 == tmp->unk18) && (arg2 == tmp->unk1C)) {
                *(f32 *)&tmp->unk2C = alCents2Ratio(arg4);
                tmp->unkC = arg3;
            }
            i++;
            tmp++;
        } while (i < D_80042760);
    }
}
void func_100100E0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 count;
    struct15 *p;

    p = D_80041FE0;
    count = D_80042760;
    if (count > 0) {
        p = D_80041FE0;
        do {
            if ((arg0 == p->unk14) && (arg1 == p->unk18) && (arg2 == p->unk1C)) {
                p->unk14 = arg3;
                p->unk18 = arg4;
                p->unk1C = arg5;
            }
            p++;
        } while (p < &D_80041FE0[count]);
    }
}
typedef struct {
    u32 field0;
    u8 field4;
    u8 pad5[0xF];
    f32 field14;
    f32 field18;
    f32 field1C;
    u8 pad20[0x1B];
    u8 field3B;
    u8 pad3C[0x48];
    u16 field84;
    u8 pad86[6];
    u16 handle8C;
    u16 handle8E;
    u8 pad90[0xAC];
    u8 field13C;
    u8 pad13D[0xF];
    f32 field14C;
    u8 pad150[0x34];
    u32 field184;
    u8 pad188[0x190];
    u32 field318;
    u8 *field31C;
} SoundOwnerState_10344;

u16 func_10010BE8(u16, s32, u16, u8, s16, u8, u8);

s32 func_10010154(u16 sound, SoundOwnerState_10344 *owner, s32 volume,
                   s16 arg3, u16 farDistance) {
    s32 result = 0;
    s32 flags = 0;

    if ((owner->field0 == 0) || (owner->field0 == 5)) {
        return 0;
    }
    if (owner->field318 != 0) {
        u32 mixFlags = owner->field184;

        result = func_10010BE8(owner->handle8E, sound, (u16)volume,
                              0x40, 0, ((mixFlags >> 3) & 0x30) * 2,
                              D_80041FD9);
    } else {
        if (owner->field4 == 0x16) {
            arg3 *= 2;
            if (farDistance < arg3) {
                arg3 = farDistance - 0xC8;
            }
        } else if ((owner->field4 == 5U) ||
                   (owner->field4 == 0x4F) ||
                   (owner->field4 == 0x83)) {
            flags = 4;
        } else if ((owner->field13C != 0) &&
                   ((owner->field4 == 0x8A) || (owner->field4 == 0x23))) {
            flags = 0x100;
        }
        func_1000FD38((s32)func_1000EE70, (s32)owner, owner->field3B | 0x10000);
        if (owner->field0 != 0) {
            result = func_1000FA64(sound, (s16)(s32)owner->field14,
                                  (s16)(s32)owner->field18,
                                  (s16)(s32)owner->field1C, volume,
                                  farDistance, arg3,
                                  (s32)func_1000EE70, (void *)owner,
                                  owner->field3B | 0x10000, flags, 0);
        }
    }
    owner->handle8E = result;
}

s32 func_10010344(u16 sound, void *arg1, u32 volume,
                  s16 nearDistance, u16 farDistance) {
    SoundOwnerState_10344 *owner = arg1;
    s32 flags = 0;

    if ((owner->field0 == 0) || (owner->field0 == 5)) {
        return 0;
    }
    if ((s32)volume < 0) {
        volume = -(s32)volume;
        flags = 0x100;
    }
    if (owner->field318 != 0) {
        u8 enabled;
        s32 mix;

        enabled = D_80041FD9;
        mix = ((owner->field184 >> 3) & 0x30) * 2;
        if ((owner->field31C != 0) && (owner->field31C[0x94] == 1)) {
            enabled = 1;
            volume = 0x7FFF;
        }
        flags = func_10010BE8(owner->handle8C, (u16)sound, (u16)volume,
                              0x40, 0, mix, enabled);
    } else {
        if (owner->field4 == 0x16) {
            nearDistance *= 2;
            if (((u16 *)&farDistance)[0] < nearDistance) {
                nearDistance = ((u16 *)&farDistance)[0] - 0xC8;
            }
        } else if ((owner->field4 == 0x8A) && (owner->field13C != 0)) {
            flags |= 0x100;
        } else if ((owner->field4 == 0x4F) || (owner->field4 == 0x83)) {
            flags |= 4;
        }
        func_1000FD38((s32)func_1000EE70, (s32)owner, owner->field3B | 0x20000);
        flags = func_1000FA64((u16)sound, (s16)(s32)owner->field14,
                              (s16)(s32)owner->field18,
                              (s16)(s32)owner->field1C, volume,
                              ((u16 *)&farDistance)[0], nearDistance,
                              (s32)func_1000EE70, (void *)owner,
                              owner->field3B | 0x20000, flags, 0);
    }
    owner->handle8C = flags;
    return flags;
}

void func_10010558(u16 arg0, struct127 *arg1, s32 arg2, s16 arg3, u16 arg4, s32 arg5) {
    if (arg5 <= 0) {
        func_10010344(arg0, arg1, arg2, arg3, arg4);
    } else {
        func_1000FA64(arg0, arg1->x_position, arg1->y_position, arg1->z_position, arg2, arg4, arg3, func_1000ECCC, arg5, arg1, 0, 0);
    }
}

// PERMUTER CANDIDATE (best 2084). Arg mapping / all casts byte-correct. Blocked by a
// register-allocation divergence: target promotes arg2 to saved reg s1 (2 saved regs)
// and schedules the func_1000EE70 %hi/%lo address late (inside the else); IDO keeps arg2
// on the stack (1 saved reg, reloads it) and hoists the address into the camera branch,
// cascading every temp + float register. Local-copy of arg2 didn't force promotion.
// void func_10010630(u16 arg0, struct127 *arg1, s32 arg2, s16 arg3, u16 arg4) {
//     if (arg1->interaction_state != 0) {
//         if (arg1->camera != 0) {
//             func_10010F30(arg0, arg2 & 0xFFFF, 64, 0, (((u32) arg1->unk184 >> 3) & 0x30) * 2);
//         } else {
//             func_1000FA64(arg0, arg1->x_position, arg1->y_position, arg1->z_position, arg2,
//                           arg4, arg3, (void *) func_1000EE70, arg1, arg1->unique_id, 0, 0);
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/init_EB00/func_10010630.s")

void func_10010720(u16 arg0, struct127 *arg1, s32 arg2, s16 arg3, u16 arg4, s32 arg5) {
    if (arg5 <= 0) {
        func_10010630(arg0, arg1, arg2, arg3, arg4);
    } else {
        func_1000FA64(arg0, arg1->x_position, arg1->y_position, arg1->z_position, arg2, arg4, arg3, func_1000EDA0, arg5, arg1, 0, 0);
    }
}

s32 func_100107F8(struct127 *arg0) {
    if (arg0->interaction_state == 0) {
        return 0;
    } else {
        if (arg0->camera != 0) {
            if (arg0->unk8E != 0) {
                if (func_1000F3D0(arg0->unk8E) != 0) {
                    return 1;
                }
            }
        } else {
            if (func_1000FF90(func_1000EE70, arg0, arg0->unique_id | 0x10000) != -1) {
                return 1;
            }
        }
        arg0->unk8E = 0;
    }
    return 0;
}

s32 func_10010894(struct127 *arg0) {
    if (arg0->camera != 0) {
        if (arg0->unk8C != 0) {
            if (func_1000F3D0(arg0->unk8C) != 0) {
                return 1;
            }
        }
    } else {
        if (func_1000FF90(func_1000EE70, arg0, arg0->unique_id | 0x20000) != -1) {
            return 1;
        }
    }
    arg0->unk8C = 0;
    return 0;
}

void func_1001091C(struct127 *arg0, struct15 *arg1) {
    s32 temp_v0;

    if ((arg1 != 0) && (arg0->interaction_state != 0)) {
        if (arg0->camera != 0) {
            if (arg0->unk8E != 0) {
                func_1000F85C(arg0->unk8E, 8, *(s32 *) &arg1);
            }
        } else {
            temp_v0 = func_1000FF90((s32) func_1000EE70, (s32) arg0, arg0->unique_id | 0x10000);
            if (temp_v0 != -1) {
                D_80041FEC[temp_v0][0] = (s32) arg1;
            } else {
                arg0->unk8E = 0;
            }
        }
    }
}
// NON-MATCHING: something isnt right...
// void func_1001091C(struct127 *arg0, struct15 *arg1) {
//     s32 temp_v0;
//
//     if ((arg1 != NULL) && (arg0->unk0 != 0)) {
//         if (arg0->camera != 0) {
//             if (arg0->unk8E != 0) {
//                 func_1000F85C(arg0->unk8E, 8, arg1->unk1C); // help
//             }
//         } else {
//             temp_v0 = func_1000FF90(func_1000EE70, arg0, arg0->unique_id | 0x10000);
//             if (temp_v0 != -1) {
//                 D_80041FEC[temp_v0][0] = arg1;
//             } else {
//                 arg0->unk8E = 0;
//             }
//         }
//     }
// }

void func_100109D0(struct127 *arg0) {
    if (arg0->camera) {
        if (arg0->unk8E) {
            func_100111C8(arg0->unk8E);
        }
    } else {
        func_1000FD38(func_1000EE70, arg0, arg0->unique_id | 0x10000);
    }
    arg0->unk8E = 0;
}

void func_10010A3C(struct127 *arg0) {
    if (arg0->camera) {
        if (arg0->unk8C) {
            func_100111C8(arg0->unk8C);
        }
    } else {
        func_1000FD38(func_1000EE70, arg0, arg0->unique_id | 0x20000);
    }
    arg0->unk8C = 0;
}

void func_10010AA8(struct127 *arg0) {
    s32 sp24;
    struct15 *tmp;

    if (arg0->camera != 0) {
        if (arg0->unk8C && func_1000F44C(arg0->unk8C)) {
            func_100111C8(arg0->unk8C);
        }
        if (arg0->unk8E && func_1000F44C(arg0->unk8E)) {
            func_100111C8(arg0->unk8E);
        }
    } else {
        sp24 = func_1000FEF0(arg0->unk8C, arg0, arg0->unique_id);
        if (sp24 != -1) {
            if (func_1000F44C(arg0->unk8C)) {
                func_100111C8(arg0->unk8C);
            }
            tmp = &D_80041FE0[sp24];
            tmp->unk10 |= 0x80;
        }
        sp24 = func_1000FEF0(arg0->unk8E, arg0, arg0->unique_id);
        if (sp24 != -1) {
            if (func_1000F44C(arg0->unk8E)) {
                func_100111C8(arg0->unk8E);
            }
            tmp = &D_80041FE0[sp24];
            tmp->unk10 |= 0x80;
        }
    }
    arg0->unk8C = 0;
    arg0->unk8E = 0;
}

struct31 *func_10017438(s32, s16, u16, u8, f32, u8, u8, struct31 **);

u16 func_10010BE8(u16 arg0, s32 sound, u16 volume, u8 pan,
                  s16 cents, u8 arg5, u8 bus) {
    u16 result;
    u16 next;
    u32 index;

    index = arg0 & 0xF;
    if ((arg0 == D_800425E0[index].unk0) && (arg0 != 0)) {
        if ((D_800425E0[index].unk8 != 0) &&
            (func_100173C4(&D_800425E0[index].unk8) != 0)) {
            func_10017594(D_800425E0[index].unk8);
            D_800425E0[index].unk8 = 0;
        }
    } else if (((D_800425E0[index].unk8 != 0) &&
                (func_100173C4(&D_800425E0[index].unk8) != 0)) ||
               (D_800425E0[index].unk4 & 0x8000)) {
        index = 0;
        while ((index < 0x10) &&
               (((D_800425E0[index].unk8 != 0) &&
                 (func_100173C4(&D_800425E0[index].unk8) != 0)) ||
                (D_800425E0[index].unk4 & 0x8000))) {
            index++;
        }
    }
    if (volume < 0x64) {
        return 0;
    }
    if (index >= 0x10) {
        result = 0;
    } else {
        if (sound == 0) {
            return 0;
        }
        if ((sound & 0x7FFF) >= 0x6E3) {
            return 0;
        }
        result = D_800425E0[index].unk2;
        next = result + 0x10;
        D_800425E0[index].unk0 = result;
        if (next < 0x10) {
            next += 0x10;
        }
        D_800425E0[index].unk2 = next;
        D_800425E0[index].unk4 = sound;
        if (D_800425E0[index].unk8 != 0) {
            ((u8 *)D_800425E0[index].unk8)[0x54] = 5;
        }
        if ((arg5 & 0x7F) + (u8)D_80041FD8 < 0x80) {
            arg5 = (u8)(arg5 + (u8)D_80041FD8);
        } else {
            arg5 = (u8)(arg5 | 0x7F);
        }
        func_10017438(D_8003E368, (sound & 0x7FFF), volume, pan,
                     alCents2Ratio(cents), arg5, bus, &D_800425E0[index].unk8);
    }
    return result;
}

u16 func_10010E78(u16 arg0, s32 arg1, u16 arg2, s32 arg3, s32 arg4,
                  s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9,
                  s32 argA) {
    s32 sp2C;
    u32 temp_t1;

    temp_t1 = func_1000F6B8(arg5, (s16)arg6, (s16)arg7, (s16)arg8, &sp2C, (s16)arg9, (s16)argA);
    temp_t1 = ((u32)arg2 * temp_t1) >> 15;
    if (temp_t1 != 0) {
        return func_10010BE8(arg0, arg1, (u16)temp_t1, sp2C & 0x7F, ((s16 *)&arg3)[1], (sp2C & 0x80) | (u8)arg4, D_80041FD9);
    }
    return 0;
}

void func_10010F30(s32 arg0, u16 arg1, u8 arg2, s16 arg3, u8 arg4) {
    func_10010BE8(0, arg0, arg1, arg2, arg3, arg4, D_80041FD9);
}

void func_10010F88(s32 arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4,
                   s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9) {
    func_10010E78(0, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}
typedef struct {
    u8 pad0[0xE];
    u16 fieldE;
} SoundOwnerScale;

s32 func_10010FFC(u16 arg0, s32 arg1, u16 arg2, s16 arg3,
                  u8 arg4, SoundOwnerState_10344 *owner) {
    f32 scale;

    if ((owner == 0) || (owner->field0 == 0)) {
        return 0;
    }
    if (owner->field318 != 0) {
        arg0 = func_10010BE8(arg0, arg1, arg2, 0x40,
                              arg3, arg4, D_80041FD9);
    } else {
        arg2 = (u16)((arg2 + arg2 + arg2) >> 2);
        if (owner->field4 != 0xFF) {
            scale = (f32)(u32)((SoundOwnerScale *)D_800D1C90[owner->field4])->fieldE * owner->field14C;
        } else {
            scale = 0.0f;
        }
        if (scale > 256.0f) {
            scale = 1.0f;
        } else if (scale < 80.0f) {
            scale = 0.3125f;
        } else {
            scale *= 0.00390625f;
        }
        arg0 = func_10010E78(arg0, arg1, arg2, arg3,
                              arg4, 0, (s32)owner->field14,
                              (s32)owner->field18, (s32)owner->field1C,
                              0x1F4, (s32)(2000.0f * scale) + 0x1F5);
    }
    return arg0;
}

void func_100111C8(u16 arg0) {
    struct120 *tmp = &D_800425E0[arg0 & 0xF];

    if ((tmp->unk8 != 0) && (tmp->unk0 == arg0)) {
        tmp->unk0 = 0;
        tmp->unk4 = 0;
        func_10017594(tmp->unk8);
        tmp->unk8 = 0;
    }
}

void func_1001123C(u16 arg0) {
    struct120 *tmp = &D_800425E0[arg0 & 0xF];

    if ((tmp->unk8 != 0) && (tmp->unk0 == arg0)) {
        if (func_100112BC(arg0, 1) == 0) {
            func_10017594(tmp->unk8);
            tmp->unk8 = 0;
        }
    }
}

s32 func_100112BC(s32 arg0, s32 arg1) {
    struct49 *temp_a0;
    s32 temp_v1 = D_80041F50;

    if (temp_v1 < 16) {
        temp_a0 = &D_80041F10[temp_v1];
        temp_a0->unk0 = arg0;
        temp_a0->unk2 = arg1;
        temp_a0->unk3 = arg0 & 0xF;
        D_80041F50 = temp_v1 + 1;
        return 1;
    } else {
        return 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/init_EB00/func_10011310.s")

s32 func_1001147C(u16 arg0) {
    struct120 *tmp;

    if (arg0 != 0) {
        tmp = &D_800425E0[arg0 & 0xF];
        if (tmp->unk0 == arg0) {
            return tmp->unk4 & 0x7FFF;
        }
    }
    return -1;
}

extern s32 func_1000A420(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6,
                         s32 arg7, s32 arg8, s32 *arg9, s32 *argA, s32 *argB);

s32 func_100114D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 *arg6, s32 *arg7, s32 *arg8) {
    struct00 *best;
    u32 bestDist;
    u32 i;
    u32 dx;
    u32 dy;
    u32 dz;
    s32 vol;
    u32 d;
    s32 result;

    best = D_80041F68;
    if (D_80082FA0 != 0) {
        bestDist = 0xFFFFFFFF;
        for (i = 0; i <= D_80082FA0; i++) {
            dx = arg0 - D_80041F68[i].unkC;
            dy = arg1 - D_80041F68[i].unk10;
            dz = arg2 - D_80041F68[i].unk14;
            d = dx * dx + dy * dy + dz * dz;
            if (d < bestDist) {
                bestDist = d;
                best = &D_80041F68[i];
            }
        }
    }
    result = func_1000A420(arg0 - best->unkC, arg1 - best->unk10, arg2 - best->unk14, best->unk18,
                  arg0 - best->unk0, arg1 - best->unk4, arg2 - best->unk8, arg4, arg5, arg6, &vol, arg8);
    *arg7 = ((u32)vol * arg3) >> 15;
    return result;
}
typedef struct {
    u16 field0;
    s16 positionX;
    s16 positionY;
    s16 positionZ;
    u16 field8;
    s16 fieldA;
    s32 valueC;
    u32 flags;
    void *callback;
    void *owner;
    s32 key;
    s16 cents;
    u8 field22;
    u8 field23;
    u16 handle;
    s16 field26;
    s16 field28;
    u8 pad2A[2];
    f32 pitch;
} SoundArrayRecord;

typedef struct {
    s32 x0;
    s32 y4;
    s32 z8;
    s32 xC;
    s32 y10;
    s32 z14;
    f32 field18;
} SoundSpatialRecord;

typedef s32 (*SoundRecordCallback)(void *, s32 *, u32 *, s32 *, s32 *,
                                    s32 *, u16 *);

s32 func_1000A750(s32, s32, s32, s32, f32, s32, s32, s32, s32, s32,
                  s32 *, s32 *, s32 *);
extern f32 D_8002C400;
extern f32 D_8002C404;

#define SPATIAL ((SoundSpatialRecord *)D_80041F68)

void func_10011624(struct15 *records, s32 *count, s32 start, s32 end) {
    SoundArrayRecord *record;
    u32 flags;
    s32 pan;
    s32 temporary;
    u32 volume;
    s32 active;
    s32 distance;
    s32 cents;
    u16 sound;
    u16 handle;
    s32 mix;
    union {
        f32 value;
        s32 bits;
    } pitch;
    s32 index;
    s32 offset;
    s32 *panOut;
    s32 fxmix;
    f32 ratio;
    f32 unclampedRatio;
    f32 pitchDelta;

    index = start;
    if ((index < *count) && (index < end)) {
        offset = index * sizeof(*records);
        do {
        record = (SoundArrayRecord *)((u8 *)records + offset);
        flags = record->flags;
        if (record->flags & 0x80) {
            goto next_record;
        }
        if (record->flags & 0x1000) {
            record->flags &= ~0x1000;
            goto next_record;
        }
        handle = record->handle;
        if ((flags & 1) && (handle != 0) && (func_1000F3D0(handle) == 0)) {
            handle = 0;
            flags &= ~1;
            if (!(flags & 8)) {
                flags |= 0x80;
                record->field0 = 0;
            }
            record->field26 = 0;
            record->field22 = 0;
            record->handle = 0;
            record->flags = flags;
            record->pitch = 1.0f;
        }
        if ((flags & 2) || (handle == 0)) {
            if (flags & 0x100) {
                pan = 0x40;
                panOut = 0;
            } else {
                panOut = &pan;
            }
            if (flags & 0x40) {
                active = func_1000A750(record->positionY,
                         SPATIAL->xC, SPATIAL->y10, SPATIAL->z14,
                         SPATIAL->field18, SPATIAL->x0,
                         SPATIAL->y4, SPATIAL->z8,
                         record->field8, record->fieldA, &pan,
                         (s32 *)&volume, &distance);
                volume = (u32)(record->valueC * volume) >> 15;
            } else {
                active = func_100114D0(record->positionX, record->positionY,
                         record->positionZ, record->valueC, record->field8,
                         record->fieldA, panOut, (s32 *)&volume, &distance);
            }
            if (D_800BE615 != 0) {
                volume = 0;
            }
            cents = record->cents;
            mix = record->field23;
            sound = record->field0;
            if ((flags & 0x10) && (record->callback != 0)) {
                record->flags = flags;
                if (((SoundRecordCallback)record->callback)((void *)(offset + (u32)records),
                        &active, &volume, &pan, &cents, &mix, &sound) != 0) {
                    func_1000FE88(records, index, count);
                    goto next_record;
                }
                handle = record->handle;
                flags = record->flags;
            }
            if (sound != 0) {
                if (volume != 0) {
                    f32 alCents2Ratio(s32);

                    fxmix = (pan & 0x80) | mix;
                    pan &= 0x7F;
                    if (flags & 0x200) {
                        pan = 0x80 - pan;
                        if (pan == 0x80) {
                            pan = 0x7F;
                        }
                        fxmix ^= 0x80;
                    }
                    pitch.value = alCents2Ratio(cents);
                    if (handle == 0) {
                        if (flags & 0xC00) {
                            if ((flags & 0xC00) == 0x400) {
                                temporary = 0;
                            } else {
                                temporary = 1;
                            }
                        } else {
                            temporary = D_80041FD9;
                        }
                        handle = func_10010BE8(0, sound, (u16)volume,
                                             (u8)pan, cents, fxmix, temporary);
                        if (handle != 0) {
                            flags |= 1;
                            record->field0 = sound;
                        }
                    } else {
                        if (volume != (u16)record->field26) {
                            func_1000F85C(handle, 8, volume);
                        }
                        if (pan != (record->field22 & 0x7F)) {
                            func_1000F85C(handle, 4, pan);
                        }
                        if (fxmix != (record->field23 | (record->field22 & 0x80))) {
                            func_1000F85C(handle, 0x100, fxmix);
                        }
                        if (flags & 4) {
                            temporary = ((u16)record->field28 - distance) * D_800BE9E4 / 3;
                            if (temporary >= 0x16F) {
                                ratio = 2.0f;
                            } else {
                                unclampedRatio = D_8002C400 / (f32)(0x16F - temporary);
                                ratio = unclampedRatio;
                                if (unclampedRatio > 2.0f) {
                                    ratio = 2.0f;
                                } else if (unclampedRatio < 0.5f) {
                                    ratio = 0.5f;
                                }
                            }
                            pitch.value *= ratio;
                            pitchDelta = pitch.value - record->pitch;
                            pitch.value = record->pitch + pitchDelta * D_8002C404;
                        }
                        start = pitch.bits;
                        if (pitch.value != record->pitch) {
                            func_1000F85C(handle, 0x11, start);
                        }
                    }
                    record->field28 = distance;
                    record->field26 = volume;
                    record->field22 = pan | fxmix;
                    record->field23 = mix;
                    record->pitch = pitch.value;
                } else if (flags & 8) {
                    if (handle != 0) {
                        func_100111C8(handle);
                    }
                    handle = 0;
                    flags &= ~1;
                } else {
                    func_1000FE88(records, index, count);
                    goto next_record;
                }
            }
        }
        record->handle = handle;
        record->flags = flags;
next_record:
        index++;
        offset += sizeof(*records);
        } while ((index < *count) && (index < end));
    }
}
typedef struct {
    u8 pad0[0x2C];
    u32 flags;
    u8 pad30[0x274];
    f32 field2A4;
    f32 field2A8;
    f32 field2AC;
    u8 pad2B0[0x48];
    f32 field2F8;
    f32 field2FC;
    f32 field300;
    u8 pad304[0x7C];
    f32 field380;
    u8 pad384[0x61C];
} SoundListenerState;

void func_10011310(void);

void func_10011BB8(void) {
    s32 index;
    s32 kept;
    s32 delta;

    if ((D_80041F60 == 0) && (D_80041F61 == 0)) {
        for (index = 0; index <= D_80082FA0; index++) {
            SoundListenerState *current = &((SoundListenerState *)D_800DBFF0)[index];

            if ((D_80082FA0 != 0) || (current->flags & 0x80000)) {
                SPATIAL[index].x0 = (s32)current->field2F8;
                SPATIAL[index].y4 = (s32)current->field2FC;
                SPATIAL[index].z8 = (s32)current->field300;
            } else {
                SPATIAL[index].x0 = (s32)current->field2A4;
                SPATIAL[index].y4 = (s32)current->field2A8;
                SPATIAL[index].z8 = (s32)current->field2AC;
            }
            SPATIAL[index].xC = (s32)current->field2F8;
            SPATIAL[index].y10 = (s32)current->field2FC;
            SPATIAL[index].z14 = (s32)current->field300;
            SPATIAL[index].field18 = current->field380;
        }
        func_10011310();
        func_10011624(D_80041FE0, &D_80042760, 0, D_80042760);
        for (index = 0, kept = 0; index < D_80042760; index++) {
            D_80041FE0[kept] = D_80041FE0[index];
            if (!(D_80041FE0[kept].unk10 & 0x80)) {
                kept++;
            }
        }
        D_80042760 = kept;
    }
    if ((D_80041F54 != D_80041FDC) || (D_80041F60 != D_80041F61)) {
        if (D_80041F61 == 1) {
            func_10017780(0, 0);
            func_10017780(1, 0);
        } else {
            func_10017780(0, D_80041F54);
            func_10017780(1, ((u16 *)&D_80041F54)[1]);
            delta = (s32)((f32)(D_80041FDC - D_80041F54) * D_80041F58);
            if (delta != 0) {
                D_80041F54 += delta;
            } else {
                D_80041F54 = D_80041FDC;
            }
        }
        D_80041F60 = D_80041F61;
    }
}

void func_10011E88(s32 arg0) {
}

void func_10011E94(s32 arg0) {
    if (arg0) {
        D_80041F61 = 1;
    } else {
        D_80041F61 = 0;
    }
}

u16 func_10011EB8(s32 arg0, s16 *arg1, s32 arg2) {
    register s32 temp_a3;
    struct_init_EB00_10011EB8_pair *entry;
    s32 temp_a0;

    temp_a3 = arg0;
    temp_a3 = func_1510F8CC(temp_a3 | 0);

    if (arg1 != NULL) {
        if (D_80082FA0 != 0) {
            *arg1 = 0x7FFF / (D_80082FA0 + 1);
        } else {
            *arg1 = 0x7FFF;
        }
    }

    entry = &((struct_init_EB00_10011EB8_row *) D_8002C240)[temp_a3].unk0[arg2];
    temp_a0 = entry->unk0;
    if (entry->unk2 >= 2) {
        temp_a0 = func_1000F568(temp_a0, ((struct_init_EB00_10011EB8_row *) D_8002C240)[temp_a3].unk0[arg2].unk2);
    }
    return temp_a0;
}
