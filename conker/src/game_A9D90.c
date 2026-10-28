#include <ultra64.h>

#define func_1506160C func_1506160C_hdr
#include "functions.h"
#undef func_1506160C
#include "variables.h"

extern s32 func_15084D70(s32, s32, s32, s16*, f32*, u8*, u8*, s32*, s32*, s32, s32*);
extern void func_15022190(s16, s16, s16, f32);
extern void func_1501D348(s32, s32, s32, s32, s32);
extern void func_1501C730(s32, s32, s32, s32, s32);
extern void func_1507D1D8(struct127 *);
extern void func_1509C3A0(void);


void func_1507C8E0(struct127 *arg0, s32 arg1) {
    arg0->unk31C->unk120 = (u8)2;
    arg0->unk31C->unk124 = arg1;
}

// "goto" hell
typedef struct C8FCStatus {
    u8 pad0[0x84];
    u8 team;
    u8 pad85[0x9B];
    u8 state;
    u8 pad121;
    u8 finished;
    u8 pad123;
    s16 timer;
    s16 elapsed;
} C8FCStatus;

typedef struct C8FCPlayer {
    u8 pad0[7];
    u8 controller;
    u8 pad8[0x314];
    C8FCStatus *status;
    u8 pad320[0xC];
} C8FCPlayer;

void func_1507CD0C(struct127 *);
void func_1506160C(struct127 *, u8, u8, u8, u8);
void func_1507D4F8(s32);
void func_1507E1D0(void *, f32 *, f32 *, f32 *);
void func_1517EE40(s32, s32, s32, s32, s8, s32);
void func_1517F814(s32, f32, f32, f32, s32);

void func_1507C8FC(void) {
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    C8FCPlayer *player;

    if (D_800BEAC0 == 0) {
        player = (C8FCPlayer *)&D_800CC2D0;
        for (i = 0; i < D_8008FD8C; i++, player++) {
            if (player->status == NULL) {
                continue;
            }
            if (player->status->state == 0) {
                player->status->elapsed = 0;
                continue;
            }
            if (player->status->state == 1) {
                continue;
            }
            if (i <= D_80082FA0) {
                if (D_800BE616 != 0 && (D_800D18A2 & (1 << i))) {
                    player->status->state = 2;
                    player->status->timer = 60;
                }
                if (player->status->state == 11) {
                    func_1517EE40(0, 0, 0, 15, 1, i);
                    player->status->state = 12;
                    continue;
                }
                if (player->status->state == 12) {
                    if (func_1517EFAC(i) != 0) {
                        func_1507D4F8(i);
                    }
                    continue;
                }
                if (player->status->state == 2) {
                    if (player->status->timer > 0) {
                        player->status->timer -= D_800BE9E4;
                    } else {
                        func_1507CD0C((struct127 *)player);
                    }
                } else if (player->status->state == 7) {
                    if (player->status->timer > 0) {
                        player->status->timer -= D_800BE9E4;
                    } else {
                        player->status->state = 8;
                        func_1506160C((struct127 *)player, 2, 0, 60, 0);
                    }
                }
                player->status->elapsed += D_800BE9E4;
                if ((D_800BE710[i] & 0x8000) || (D_800BE616 != 0 && player->status->team == 0) ||
                    func_150859AC((s16)i, 3) < 2 || (player->status->team == 1 && player->status->elapsed >= 181)) {
                    player->status->finished = 1;
                }
                if (D_8008FDBC & 0x80) {
                    player->status->finished = 0;
                }
                func_1507E1D0(player, &x, &y, &z);
                func_1517F814(0, x, y, z, i);
                if (player->status->finished != 0 && player->status->state == 5) {
                    player->status->state = 6;
                }
                if (player->status->state == 9) {
                    func_1507D4F8(i);
                }
            } else if (player->status->state == 11) {
                func_1507D4F8(i);
                continue;
            } else if (player->status->state == 2) {
                if (player->status->timer > 0) {
                    player->status->timer -= D_800BE9E4;
                } else {
                    player->status->state = 7;
                }
            } else {
                player->status->elapsed += D_800BE9E4;
                if (player->controller == 0xFF && player->status->elapsed >= 161) {
                    func_1506160C((struct127 *)player, 2, 0, 60, 0);
                }
                if (player->status->elapsed >= 181 && player->status->state != 10) {
                    func_1507D4F8(i);
                }
            }
        }
        D_8008FDBC &= ~0x80;
    }
}

void func_1507CD0C(struct127 *arg0) {
    s32 temp_lo = ((s32)arg0 - (s32)&D_800CC2D0) / (s32)sizeof(struct127);

    arg0->unk31C->unk120 = 3;

    if (temp_lo <= D_80082FA0) {
        func_15181D70(temp_lo);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507CD64.s")
void func_1509BFB0(s32 num, s32, s32, ...);
extern struct127 D_800CC40F[];

void func_1507D158(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp = *(u8*)&D_800CC40F[arg0];
    func_1509BFB0(3, temp | 0x2000, arg1, arg2, arg3, arg4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507D1D8.s")

void func_1507D4F8(s32 arg0) {
    s32 sp24;
    struct127 *temp_a0;

    if ((D_8008FDBC & 1) == 0) {
        func_15085710((s16)arg0, 4, 1);
    }

    sp24 = (s16)arg0;
    if (func_150859AC(sp24, 3) != 0) {
        func_15085710(sp24, 5, D_8008726C);
        temp_a0 = &D_800CC2D0[arg0];
        temp_a0->unkB2 = 0;

        if (D_800BE616 != 0) {
            if (*(s8 *)&D_800E0C20 == 0) {
                func_1507D1D8(temp_a0);
                return;
            }

            temp_a0->unk31C->unk120 = 0xA;
            return;
        }

        D_800D18A8 = 1;
        if (((D_800D2E4C->unk19 & 4) == 0) && (D_8008FDA8 >= 0)) {
            func_1501C730(1, 0x22, 0, 0, 0);
            return;
        }

        func_1501C730(2, *(s8 *)&D_800BE3DF, D_800BE3E0, 0, 0);
        return;
    }

    if (D_800BE616 == 0) {
        D_800D2E43 = 1;
        func_1509C3A0();
        D_800D18A8 = 1;
        func_15085710(sp24, 5, D_8008726C);
        func_15085710(sp24, 2, D_80087260);
        func_1501C730(1, 0x18, 0, 0, 0);
    } else {
        D_800D18A0 = D_800D18A0 | (1 << arg0);
    }

    temp_a0 = &D_800CC2D0[arg0];
    sp24 = (s32)temp_a0->unk31C;
    if (((struct126 *)sp24)->unk84 == 0) {
        *(volatile s8 *)&D_8008FD94 = *(s8 *)&D_8008FD94 - 1;
        sp24 = (s32)*(struct126 * volatile *)&temp_a0->unk31C;
    }
    ((struct126 *)sp24)->unk120 = 0xA;
    D_800BE618--;
}
// NON-MATCHING: pretty far away!
// void func_1507D4F8(s16 arg0) { // struct126 *
//     // ?32 sp24;
//     struct127 *temp_a0;
//
//     if ((D_8008FDBC & 1) == 0) {
//         func_15085710(arg0, 4, 1);
//     }
//     // sp24 = (?32) arg0;
//     if (func_150859AC(arg0, 3) != 0) {
//         func_15085710(arg0, 5, D_8008726C); //temp_ret =
//         temp_a0 = &D_800CC2D0[arg0];
//         temp_a0->unkB2 = (u16)0;
//         if (D_800BE616 == 0) {
//             D_800D18A8 = (u8)1;
//             if (((D_800D2E4C->unk19 & 4) != 0) || (D_8008FDA8 < 0)) {
//                 func_1501C730(2, D_800BE3DF, D_800BE3E0, 0, 0);
//                 return;
//             }
//             func_1501C730(1, 0x22, 0, 0, 0);
//             return;
//         }
//         if (D_800E0C20 != 0) {
//             temp_a0->unk31C->unk120 = (u8)0xA;
//             return; // temp_ret;
//         }
//         func_1507D1D8(temp_a0);
//         return;
//     }
//     if (D_800BE616 == 0) {
//         D_800D2E43 = (u8)1;
//         func_1509C3A0();
//         D_800D18A8 = (u8)1;
//         func_15085710(arg0, 5, D_8008726C);
//         func_15085710(arg0, 2, D_80087260);
//         func_1501C730(1, 0x18, 0, 0, 0);
//     } else {
//         D_800D18A0 = (u16) (D_800D18A0 | (1 << (s32) arg0));
//     }
//     // temp_a0 = &D_800CC2D0[arg0];
//     if (temp_a0->unk31C->unk84 == 0) {
//         D_8008FD94 -= 1; //(s8) (D_8008FD94 - 1);
//     }
//     temp_a0->unk31C->unk120 = (u8)0xA;
//     D_800BE618 -= 1; //(s8) (D_800BE618 - 1);
//     //return temp_a0_2->unk31C;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507D754.s")

void func_1507DB44(s32 arg0, s32 arg1) {
    func_1503DE70(arg0, arg1, -1);
}

void func_1507DB64(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507DB6C.s")

void func_1507DE4C(struct127 *arg0) {

    if (arg0->interaction_state == 1) {
        func_150836CC(arg0, 0x44);
        func_150836CC(arg0, 0x23);
        arg0->unk9C |= 0xF000;
        func_150836CC(arg0, 0x44);
        func_150836CC(arg0, 0x23);
        return;
    }

    switch(arg0->id) {
        case 0x9F:
        case 0xa0:
            arg0->unk9C |= 0xF000;
            break;
        case 0x5A:
        case 0x74:
        case 0x7A:
            arg0->unk9C |= 0xFF8;
            break;
    }
}


void func_1507DF10(void *arg0, s32 arg1) {
    switch (arg1) {
    case 9:
        *(s32 *)((u8 *)arg0 + 0x94) = (s32) (*(s32 *)((u8 *)arg0 + 0x94) | 0x20);
        *(s32 *)((u8 *)arg0 + 0x9C) = (s32) (*(s32 *)((u8 *)arg0 + 0x9C) | 0x78);
        *(s32 *)((u8 *)arg0 + 0x2E4) = 1;
        return;
    case 8:
        *(s32 *)((u8 *)arg0 + 0x94) |= 0x40;
        *(s32 *)((u8 *)arg0 + 0x94) &= ~0x200;
        *(s32 *)((u8 *)arg0 + 0x9C) = (s32) (*(s32 *)((u8 *)arg0 + 0x9C) | 0xF00);
        *(s32 *)((u8 *)arg0 + 0x2E4) = 2;
        return;
    case 6:
    case 7:
        *(s32 *)((u8 *)arg0 + 0x94) |= 0xE;
        *(s32 *)((u8 *)arg0 + 0x94) &= ~0x410;
        *(s32 *)((u8 *)arg0 + 0x9C) = (s32) (*(s32 *)((u8 *)arg0 + 0x9C) | 0xEE0000);
        *(s32 *)((u8 *)arg0 + 0x2E4) = 4;
        return;
    case 4:
    case 5:
        *(s32 *)((u8 *)arg0 + 0x94) |= 0x80;
        *(s32 *)((u8 *)arg0 + 0x94) &= ~0x500;
        *(s32 *)((u8 *)arg0 + 0x2E4) = 8;
        /* fallthrough */
    default:
        return;
    }
}
void func_1507DFE4(s32 arg0, s32 arg1) {
    s32 temp;
    s16 sp5C[3];
    f32 sp50[3];
    u8 sp4F;
    u8 sp4E;
    s32 sp48;
    s32 sp44;
    f32 temp_f0;
    s32 sp3C;
    s32 sp38;

    if (D_800C35EA != 1) {
        func_15084D70(0, arg0, 1, sp5C, sp50, &sp4F, &sp4E, &sp44, &sp48, 1, &sp38);
        temp_f0 = ((f32)(sp4E - 0x40) * 1.40625f) + 180.0f;
        if (sp38 == 0) {
            sp3C = 1;
        } else if (sp38 == 1) {
            sp3C = 3;
        } else {
            return;
        }
        func_15022190(sp5C[0], sp5C[1], sp5C[2], temp_f0);
        temp = D_800BE9F0;
        D_800BE9F0 = 0x25;
        D_800C3671 = 1;
        func_1501D348(0x25, sp3C, 0, 0, 0);
        D_800C3670 = 1;
        D_800BE9F0 = temp;
    }
}
extern s32 func_15084D70(s32, s32, s32, s16*, f32*, u8*, u8*, s32*, s32*, s32, s32*);


s32 func_1507E114(s32 arg0) {
    s32 sp5C;
    s16 sp54[3];
    f32 sp48[3];
    s32 sp44;
    u8 sp43;
    u8 sp42;
    s32 sp3C;
    s32 temp;

    if (D_800D18A8 == 0) {
        return 0;
    }
    temp = (s32)D_800BE9F4;
    if (temp == 0x22 || temp == 0x18) {
        return 0;
    }
    *(s32*)&D_800BE9F4 = (s8)D_800BE3DF;
    func_15084D70(0, (u8)D_800BE3E0, 1, sp54, sp48, &sp43, &sp42, &sp44, &sp3C, 1, &sp5C);
    return sp5C + 1;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507E1D0.s")
