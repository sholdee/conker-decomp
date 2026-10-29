#include <ultra64.h>

#define func_1506160C func_1506160C_hdr
#include "functions.h"
#undef func_1506160C
#define D_800D20FC D_800D20FC_hdr
#include "variables.h"
#undef D_800D20FC

extern s32 func_15084D70(s32, s32, s32, s16*, f32*, u8*, u8*, s32*, s32*, s32, s32*);
extern void func_15022190(s16, s16, s16, f32);
extern void func_1501D348(s32, s32, s32, s32, s32);
extern void func_1501C730(s32, s32, s32, s32, s32);
extern void func_1507D1D8();
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
void func_1507E1D0(struct127 *, f32 *, f32 *, f32 *);
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
                func_1507E1D0((struct127 *)player, &x, &y, &z);
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

typedef struct CD64Inner {
    u8 pad0[0x46];
    s16 unk46;
    u8 pad48[0xD8];
    u8 unk120;
    u8 unk121;
    u8 pad122[0x8A];
    u8 unk1AC;
} CD64Inner;

typedef struct CD64Msg {
    struct127 *obj;
    u8 id;
} CD64Msg;

extern u8 D_800D18A9;
extern s16 D_800E0AD0[];
void func_15169040(void *, s32);
void func_1504BAF0(struct127 *);
void func_150E7994(s16, f32, u8, s32);

void func_1507CD64(struct127 *arg0, s32 arg1) {
    f32 f;
    s32 sound;
    u32 r;
    s32 mode;
    s32 index;
    CD64Msg msg;
    struct127 *saved;
    s32 changed;
    s32 flag;

    if (arg0->unk31C == NULL) {
        return;
    }
    index = ((s32)arg0 - (s32)&D_800CC2D0) / (s32)sizeof(struct127);
    if (index >= D_8008FD8C) {
        return;
    }
    if (((CD64Inner *)arg0->unk31C)->unk120 != 0) {
        if (arg1 != 9 || arg1 == ((CD64Inner *)arg0->unk31C)->unk121) {
            return;
        }
        changed = 1;
    } else {
        changed = 0;
    }
    ((CD64Inner *)arg0->unk31C)->unk121 = arg1;
    if (D_800BE616 != 0 && changed == 0) {
        if (index < 16) {
            D_800E0AD0[index]++;
        }
        if ((D_8008FDBC & 0x20) && *(u8 *)((u8 *)arg0 + 0x128) == 0) {
            func_15085710(1, 10, 1);
        }
    }
    msg.obj = arg0;
    msg.id = arg0->unique_id;
    func_15169040(&msg, 3);
    D_800D18A9 = 0;
lbl2:
    sound = -1;
    flag = 0;
    mode = 0;
    if (((CD64Inner *)arg0->unk31C)->unk1AC) {
        D_800D18A9 = 2;
    }
    switch (arg1) {
    case 11:
        sound = 0x2D4;
        break;
    case 12:
        sound = 0x2D6;
        break;
    case 0:
        sound = 0x58;
        break;
    case 1:
        sound = 0x13;
        flag = 1;
        break;
    case 2:
        sound = 7;
        flag = 1;
        break;
    case 3:
        sound = 0x28;
        D_800D18A9 = 1;
        break;
    case 4:
        sound = 0x24;
        mode = 1;
        D_800D18A9 = 1;
        break;
    case 5:
        sound = 0x24;
        mode = 1;
        D_800D18A9 = 3;
        break;
    case 6:
        if (arg0->interaction_state == 1 || arg0->interaction_state == 0x15 || arg0->unk5 == 3 ||
            arg0->unk65 != 0) {
            sound = 0x170;
        }
        mode = 1;
        break;
    case 7:
        sound = 0x16F;
        break;
    case 8:
        sound = 0x170;
        mode = 1;
        D_800D18A9 = 3;
        break;
    case 15:
        sound = 0x170;
        mode = 3;
        D_800D18A9 = 3;
        break;
    case 9:
        mode = 2;
        break;
    case 10:
        sound = 0x2F;
        D_800D18A9 = 3;
        break;
    case 14:
        sound = 0x1E;
        mode = 1;
        break;
    case 13:
        sound = 0x2D8;
        mode = 1;
        break;
    case 16:
        sound = 0xF6;
        mode = 1;
        break;
    }
    if (sound != -1) {
        func_1505E650(arg0, sound, 1.0f, 3.0f, 0.0f, 0.0f, flag);
    }
    arg0->health = 0;
    arg0->immune = 0xFF;
    arg0->stunned = 0xFF;
    arg0->disable_run = 0xFF;
    arg0->unkF8 |= 0x200;
    arg0->unkE4 = 0;
    arg0->unkE6 = 0;
    arg0->xz_velocity = 0.0f;
    ((CD64Inner *)arg0->unk31C)->unk46 = 0;
    if (arg0->unk13C != 0) {
        saved = D_800D154C;
        D_800D154C = arg0;
        func_1504BAF0(arg0);
        D_800D154C = saved;
    }
    if (mode == 0 || mode == 3) {
        ((CD64Inner *)arg0->unk31C)->unk120 = 1;
    } else {
        func_1507C8E0(arg0, 0);
    }
    if (mode == 2) {
        ((CD64Inner *)arg0->unk31C)->unk120 = 0xB;
    } else if (mode == 3) {
        func_1507D4F8(index);
    }
    if (index == 0 && D_800BE9F0 == 4) {
        r = func_150ADA20();
        f = func_150ADA68();
        func_150E7994((s16)(r % 3 + 4), f * 0.1f + 0.3f, 0xFF, 0);
    }
}
void func_1509BFB0(s32 num, s32, s32, ...);
extern struct127 D_800CC40F[];

void func_1507D158(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp = *(u8*)&D_800CC40F[arg0];
    func_1509BFB0(3, temp | 0x2000, arg1, arg2, arg3, arg4);
}

typedef struct GameA9D90Inner {
    u8 pad0[0x120];
    u8 field_120;
    u8 pad121[3];
    s16 field_124;
} GameA9D90Inner;

typedef struct GameA9D90Object {
    u8 pad0[0x31C];
    GameA9D90Inner *field_31C;
} GameA9D90Object;

void func_150054C4(u8 *, u32, s32, s32);
void func_15085430(void *, s32, s32);
void func_15181DC8(s32);
void func_15082A44(s32, s32, s32, s32, s32);
void func_15088D58(GameA9D90Object *);
extern s16 D_800BE9F2;
extern s32 D_800D20FC;

void func_1507D1D8(GameA9D90Object *arg0) {
    s32 value;
    s32 saved;
    s8 player;
    s8 state;
    s8 actorState;
    s8 mode;
    s32 choice;
    GameA9D90Inner *inner;
    s32 slot;
    union { s32 word; s16 halves[2]; } index;

    inner = arg0->field_31C;
    saved = *(s16 *)((u8 *)inner + 0x1AA);
    state = *(s8 *)((u8 *)inner + 0x1B0);
    actorState = *(s8 *)((u8 *)arg0 + 0x328);
    mode = *(s8 *)((u8 *)inner + 0x1AF);
    if (D_800BE616 != 0) {
        player = inner->pad0[0x11B];
    } else {
        player = -1;
    }
    if (D_800BE9F0 == 0x2B) {
        slot = ((u8 *)arg0 - (u8 *)D_800CC2D0) / 812;
        index.word = (s16)slot;
        value = func_150859AC((s16)slot, 1);
        if (value >= 0x11 && value < 0x21) {
            choice = (func_150ADA20() & 0xF) + 0x11;
            func_15085710(index.halves[1], 0, choice);
            if (choice < 0x1D) {
                *(u8 *)(D_800D20FC + slot * 0x30 + 4) = 0x9C;
            } else {
                *(u8 *)(D_800D20FC + slot * 0x30 + 4) = 0x9D;
            }
        }
    }
    slot = ((u8 *)arg0 - (u8 *)D_800CC2D0) / 812;
    index.word = slot * 0x30;
    func_15060F28((struct127 *)arg0, 0);
    D_800D18A8 = 1;
    func_15082A44(index.word + D_800D20FC, slot, 0, 0, 0);
    *(s8 *)((u8 *)arg0->field_31C + 0x1B0) = state;
    *(s8 *)((u8 *)arg0->field_31C + 0x1AF) = mode;
    *(s8 *)((u8 *)arg0 + 0x328) = actorState;
    if (D_80082FA0 >= slot) {
        func_150054C4(slot * 0x9A0 + (u8 *)D_800DBFF0, slot, D_800BE9F2, (s8)player);
        func_15181DC8(slot);
        func_1517EE40(0, 0, 0, 0, 1, slot);
        func_1517EE40(0, 0, 0, 0x1E, 0, slot);
    } else {
        func_15085430(arg0, (s8)player, 0);
    }
    D_800D18A8 = 0;
    arg0->field_31C->field_120 = 0;
    arg0->field_31C->pad121[1] = 0;
    arg0->field_31C->field_124 = 0;
    *(s16 *)((u8 *)arg0->field_31C + 0x1AA) = saved;
    arg0->pad0[0x1CA] = D_8008726C;
    *(u16 *)((u8 *)arg0 + 0x22C) = *(u16 *)((u8 *)arg0 + 0x22C) | 0x100;
    arg0->field_31C->pad0[0x75] = 0;
    arg0->pad0[0x20F] = 0xFF;
    arg0->pad0[0x210] = 0xFF;
    arg0->pad0[0x211] = 0xFF;
    arg0->pad0[0x232] = 1;
    *(s32 *)((u8 *)arg0 + 0x218) = 0;
    arg0->field_31C->pad0[0x11B] = (s8)player;
    if (D_800BE616 != 0) {
        arg0->pad0[0x125] = 0x96;
        *(u8 *)((u8 *)arg0->field_31C + 0x1B1) = arg0->pad0[0x125];
        if (D_800BE9F0 == 0x33) {
            func_1507D158(slot, 0x78, 0, 0, 0);
            func_15088D58(arg0);
        }
    }
}

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

typedef struct D754Obj {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x58];
    /* 0x6E */ u8 unk6E;
    /* 0x6F */ u8 pad6F[0xD];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ u8 pad88[2];
    /* 0x8A */ u8 unk8A;
} D754Obj;

extern void func_150B0348(struct127 *, s32, s32);
extern void func_150BFFE0(struct127 *);
extern void func_150D85AC(struct127 *);
extern void func_150F1D10(struct127 *, s32, s32);
extern void func_151C9740(struct127 *, s32, s32);
extern void func_151CFA4C(struct127 *, s32, s32);
extern void func_150BE494(struct127 *, s32, s32);
extern void func_150AEB9C(struct127 *);
extern void func_151BA518(struct127 *, s32, s32);
extern D754Obj *func_151149AC(s32);
extern void *func_1507515C(struct127 *);
extern void func_15136C3C(void *, s32, s32, s32, s32, s32, s32, s32);
extern void func_150B7B40(struct127 *, s32, s32);
extern void func_150BC488(struct127 *);
extern void func_150BBB5C(struct127 *);
extern void func_151A6C90(struct127 *, s32, s32);
extern void func_150E0870(struct127 *, s32, s32);
extern void func_151D22B0(struct127 *, s32, s32);
void func_1507DB44(s32 arg0, s32 arg1);

s32 func_1507D754(struct127 *arg0) {
    D754Obj *obj;
    void *temp;

    switch (arg0->id) {
    case 0x7F:
        func_150B0348(arg0, 0xFF, 1);
        break;
    case 0x10:
    case 0x38:
    case 0x91:
        func_150BFFE0(arg0);
        break;
    case 0x3A:
    case 0x3D:
        func_150D85AC(arg0);
        break;
    case 0x4A:
        func_150F1D10(arg0, 0xFF, 1);
        break;
    case 0x47:
        func_151C9740(arg0, 0xFF, 1);
        break;
    case 0x25:
        if (arg0->unk2EC != 0 || D_800BE9F0 == 6 || D_800BE9F0 == 0x3B) {
            func_151CFA4C(arg0, 0xFF, 1);
        }
        return 0;
    case 0x79:
        func_150BE494(arg0, 0xFF, 1);
        break;
    case 0x5:
    case 0xAD:
    case 0xAE:
    case 0xAF:
        arg0->pad320[8] = 0;
        func_1507DB44((s32)arg0, 0);
        func_150AEB9C(arg0);
        return 1;
    case 0x53:
        if (D_800BE616 != 0) {
            func_1507CD64(arg0, 0xA);
            return 2;
        }
        break;
    case 0x90:
        if (D_800BE616 != 0 && D_800BE9F0 == 0x33) {
            func_1507CD64(arg0, 8);
            return 2;
        }
        break;
    case 0x28:
    case 0x77:
        obj = NULL;
        func_151BA518(arg0, 0xFF, 1);
        func_15145A50(arg0);
        if (D_800BE616 != 0) {
            func_1507CD64(arg0, 6);
            obj = func_151149AC((u8)(arg0->unk127 + 0xB));
        } else {
            if (D_800BE9F0 == 0x23) {
                obj = func_151149AC(1);
            }
            temp = func_1507515C(arg0);
            if (temp != NULL && D_800C35EA != 1) {
                func_15136C3C(temp, 1, 1, 1, 1, 0, 0xFF, 1);
            }
        }
        if (obj != NULL) {
            obj->unk6E = 0;
            obj->unk10 = arg0->x_position;
            obj->unk12 = arg0->y_position;
            obj->unk14 = arg0->z_position;
            obj->unk0 = arg0->unkB8;
            obj->unk4 = arg0->unk40;
            obj->unk8 = arg0->unkC4;
            obj->unk8A = 0xFF;
            obj->unk7C = 0;
            obj->unk80 = 0;
            obj->unk84 = 0;
        }
        if (D_800BE616 != 0) {
            return 2;
        }
        break;
    case 0xD:
        func_150B7B40(arg0, 0xFF, 1);
        break;
    case 0x20:
        if (arg0->y_velocity < -20.0f) {
            func_150BC488(arg0);
        } else {
            func_150BBB5C(arg0);
        }
        break;
    case 0x4F:
    case 0x83:
        func_151A6C90(arg0, 0xFF, 1);
        break;
    case 0x34:
        func_150E0870(arg0, 0xFF, 1);
        break;
    case 0x44:
        func_151D22B0(arg0, 0xFF, 1);
        break;
    }
    return 0;
}

void func_1507DB34(void) {
}

void func_1507DB3C(void) {
}

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
void func_15143134(void *, f32 *, s32);

void func_1507E1D0(struct127 *arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    if (arg0->unk1D4 != NULL) {
        s32 base;
        f32 in[3];
        f32 out[3];

        in[0] = 0.0f;
        in[1] = arg0->y_scale * 30.0f;
        in[2] = 0.0f;
        base = (s32)arg0->unk1D4;
        if (arg0->interaction_state == 1) {
            base += 0x300;
        } else if (arg0->interaction_state == 0x1E) {
            base += 0xC0;
        }
        func_15143134(in, out, base);
        *arg1 = out[0];
        *arg2 = out[1];
        *arg3 = out[2];
    } else {
        *arg1 = arg0->x_position;
        *arg2 = arg0->y_position;
        *arg3 = arg0->z_position;
    }
}
