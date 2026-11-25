#include <ultra64.h>

// matching: func_1506160C, D_800D20FC and D_800BE9F4 are declared below with types that differ from the shared headers
#define func_1506160C func_1506160C_hdr
#include "functions.h"
#undef func_1506160C
#define D_800D20FC D_800D20FC_hdr
#define D_800BE9F4 D_800BE9F4_hdr
#include "variables.h"
#undef D_800D20FC
#undef D_800BE9F4

extern s32 func_15084D70(s32, s32, s32, s16*, f32*, u8*, u8*, s32*, s32*, s32, s32*);
extern void func_15022190(s16, s16, s16, f32);
extern void func_1501D348(s32, s32, s32, s32, s32);
extern void func_1501C730(s32, s32, s32, s32, s32);
void func_1507D1D8(struct127 *obj);
extern void func_1509C3A0(void);


/* View of the per-object state block hung off struct127.unk31C (struct126 lacks these fields). */
typedef struct ObjState {
    /* 0x000 */ u8 pad0[0x46];
    /* 0x046 */ s16 unk46;
    /* 0x048 */ u8 pad48[0x2D];
    /* 0x075 */ u8 unk75;
    /* 0x076 */ u8 pad76[0xE];
    /* 0x084 */ u8 team;
    /* 0x085 */ u8 pad85[0x96];
    /* 0x11B */ u8 unk11B;
    /* 0x11C */ u8 pad11C[4];
    /* 0x120 */ u8 state;
    /* 0x121 */ u8 cause;
    /* 0x122 */ u8 finished;
    /* 0x123 */ u8 pad123;
    /* 0x124 */ s16 timer;
    /* 0x126 */ s16 elapsed;
    /* 0x128 */ u8 pad128[0x82];
    /* 0x1AA */ s16 unk1AA;
    /* 0x1AC */ u8 unk1AC;
    /* 0x1AD */ u8 pad1AD[2];
    /* 0x1AF */ s8 unk1AF;
    /* 0x1B0 */ s8 unk1B0;
    /* 0x1B1 */ u8 unk1B1;
} ObjState;

#define STATE(obj) ((ObjState *)(obj)->unk31C)

void func_1507CD0C(struct127 *obj);
void func_1506160C(struct127 *, u8, u8, u8, u8);
void func_1507D4F8(s32);
void func_1507E1D0(struct127 *, f32 *, f32 *, f32 *);
void func_1517EE40(s32, s32, s32, s32, s8, s32);
void func_1517F814(s32, f32, f32, f32, s32);

/* Puts obj into state 2 with the given timer. */
void func_1507C8E0(struct127 *obj, s32 timer) {
    STATE(obj)->state = 2;
    STATE(obj)->timer = timer;
}

/* Per-frame update of every active player's state machine. */
void func_1507C8FC(void) {
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    struct127 *player;

    if (D_800BEAC0 == 0) {
        player = D_800CC2D0;
        for (i = 0; i < D_8008FD8C; i++, player++) {
            if (STATE(player) == NULL) {
                continue;
            }
            if (STATE(player)->state == 0) {
                STATE(player)->elapsed = 0;
                continue;
            }
            if (STATE(player)->state == 1) {
                continue;
            }
            if (i <= D_80082FA0) {
                if (D_800BE616 != 0 && (D_800D18A2 & (1 << i))) {
                    STATE(player)->state = 2;
                    STATE(player)->timer = 60;
                }
                if (STATE(player)->state == 11) {
                    func_1517EE40(0, 0, 0, 15, 1, i);
                    STATE(player)->state = 12;
                    continue;
                }
                if (STATE(player)->state == 12) {
                    if (func_1517EFAC(i) != 0) {
                        func_1507D4F8(i);
                    }
                    continue;
                }
                if (STATE(player)->state == 2) {
                    if (STATE(player)->timer > 0) {
                        STATE(player)->timer -= D_800BE9E4;
                    } else {
                        func_1507CD0C(player);
                    }
                } else if (STATE(player)->state == 7) {
                    if (STATE(player)->timer > 0) {
                        STATE(player)->timer -= D_800BE9E4;
                    } else {
                        STATE(player)->state = 8;
                        func_1506160C(player, 2, 0, 60, 0);
                    }
                }
                STATE(player)->elapsed += D_800BE9E4;
                if ((D_800BE710[i] & 0x8000) || (D_800BE616 != 0 && STATE(player)->team == 0) ||
                    func_150859AC(i, 3) < 2 || (STATE(player)->team == 1 && STATE(player)->elapsed >= 181)) {
                    STATE(player)->finished = TRUE;
                }
                if (D_8008FDBC & 0x80) {
                    STATE(player)->finished = FALSE;
                }
                func_1507E1D0(player, &x, &y, &z);
                func_1517F814(0, x, y, z, i);
                if (STATE(player)->finished && STATE(player)->state == 5) {
                    STATE(player)->state = 6;
                }
                if (STATE(player)->state == 9) {
                    func_1507D4F8(i);
                }
            } else if (STATE(player)->state == 11) {
                func_1507D4F8(i);
                continue;
            } else if (STATE(player)->state == 2) {
                if (STATE(player)->timer > 0) {
                    STATE(player)->timer -= D_800BE9E4;
                } else {
                    STATE(player)->state = 7;
                }
            } else {
                STATE(player)->elapsed += D_800BE9E4;
                if (player->unk7 == 0xFF && STATE(player)->elapsed >= 161) {
                    func_1506160C(player, 2, 0, 60, 0);
                }
                if (STATE(player)->elapsed >= 181 && STATE(player)->state != 10) {
                    func_1507D4F8(i);
                }
            }
        }
        D_8008FDBC &= ~0x80;
    }
}

/* Puts obj into state 3 and, for a local player slot, calls func_15181D70 on it. */
void func_1507CD0C(struct127 *obj) {
    s32 slot = obj - D_800CC2D0;

    STATE(obj)->state = 3;

    if (slot <= D_80082FA0) {
        func_15181D70(slot);
    }
}

typedef struct CD64Msg {
    struct127 *obj;
    u8 id;
} CD64Msg;

extern u8 D_800D18A9;
extern s16 D_800E0AD0[];
void func_15169040(void *, s32);
void func_1504BAF0(struct127 *);
void func_150E7994(s16, f32, u8, s32);

/* Kills obj with the given cause: plays its sound, resets movement and moves it to a dead state. */
void func_1507CD64(struct127 *obj, s32 cause) {
    f32 f;
    s32 sound;
    u32 r;
    s32 mode;
    s32 slot;
    CD64Msg msg;
    struct127 *saved;
    s32 again;
    s32 flag;

    if (STATE(obj) == NULL) {
        return;
    }
    slot = obj - D_800CC2D0;
    if (slot >= D_8008FD8C) {
        return;
    }
    if (STATE(obj)->state != 0) {
        if (cause != 9 || cause == STATE(obj)->cause) {
            return;
        }
        again = TRUE;
    } else {
        again = FALSE;
    }
    STATE(obj)->cause = cause;
    if (D_800BE616 != 0 && !again) {
        if (slot < 16) {
            D_800E0AD0[slot]++;
        }
        if ((D_8008FDBC & 0x20) && *(u8 *)&obj->pad128 == 0) { // byte at 0x128 (struct127 has a u32 pad there)
            func_15085710(1, 10, 1);
        }
    }
    msg.obj = obj;
    msg.id = obj->unique_id;
    func_15169040(&msg, 3);
    D_800D18A9 = 0;
lbl2: // matching: unused label splits the basic block; as1 then hoists the reload of cause above this store
    sound = -1;
    flag = 0;
    mode = 0;
    if (STATE(obj)->unk1AC) {
        D_800D18A9 = 2;
    }
    switch (cause) {
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
        if (obj->interaction_state == 1 || obj->interaction_state == 0x15 || obj->unk5 == 3 ||
            obj->unk65 != 0) {
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
        func_1505E650(obj, sound, 1.0f, 3.0f, 0.0f, 0.0f, flag);
    }
    obj->health = 0;
    obj->immune = 0xFF;
    obj->stunned = 0xFF;
    obj->disable_run = 0xFF;
    obj->unkF8 |= 0x200;
    obj->unkE4 = 0;
    obj->unkE6 = 0;
    obj->xz_velocity = 0.0f;
    STATE(obj)->unk46 = 0;
    if (obj->unk13C != 0) {
        saved = D_800D154C;
        D_800D154C = obj;
        func_1504BAF0(obj);
        D_800D154C = saved;
    }
    if (mode == 0 || mode == 3) {
        STATE(obj)->state = 1;
    } else {
        func_1507C8E0(obj, 0);
    }
    if (mode == 2) {
        STATE(obj)->state = 11;
    } else if (mode == 3) {
        func_1507D4F8(slot);
    }
    if (slot == 0 && D_800BE9F0 == 4) {
        r = func_150ADA20();
        f = func_150ADA68();
        func_150E7994(r % 3 + 4, f * 0.1f + 0.3f, 0xFF, 0);
    }
}

/* Forwards four arguments to func_1509BFB0(3, ...) keyed by object slot's unk13F | 0x2000. */
void func_1507D158(s32 slot, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 value = D_800CC2D0[slot].unk13F;

    func_1509BFB0(3, value | 0x2000, arg1, arg2, arg3, arg4);
}

/* 0x30-byte entries of the D_800D20FC table, one per object slot. */
typedef struct D20FCEntry {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5[0x2B];
} D20FCEntry;

void func_150054C4(struct108 *, u32, s32, s32);
void func_15085430(struct127 *, s32, s32);
void func_15181DC8(s32);
void func_15082A44(D20FCEntry *, s32, s32, s32, s32);
void func_15088D58(struct127 *);
extern s16 D_800BE9F2;
extern s32 D_800BE9F4;
extern D20FCEntry *D_800D20FC;

/* Re-initialises obj through func_15060F28/func_15082A44 (health back to D_8008726C), keeping a few state bytes. */
void func_1507D1D8(struct127 *obj) {
    s32 value;
    s32 saved1AA;
    s8 player;
    s8 saved1B0;
    s8 saved328;
    s8 saved1AF;
    s32 choice;
    s32 slot;

    saved1AA = STATE(obj)->unk1AA;
    saved1B0 = STATE(obj)->unk1B0;
    saved328 = *(s8 *)&obj->pad320[8]; // signed byte at 0x328 (struct127 types it u8)
    saved1AF = STATE(obj)->unk1AF;
    if (D_800BE616 != 0) {
        player = STATE(obj)->unk11B;
    } else {
        player = -1;
    }
    if (D_800BE9F0 == 0x2B) {
        slot = obj - D_800CC2D0;
        value = func_150859AC(slot, 1);
        if (value >= 0x11 && value < 0x21) {
            choice = (func_150ADA20() & 0xF) + 0x11;
            func_15085710(slot, 0, choice);
            if (choice < 0x1D) {
                D_800D20FC[slot].unk4 = 0x9C;
            } else {
                D_800D20FC[slot].unk4 = 0x9D;
            }
        }
    }
    slot = obj - D_800CC2D0;
    func_15060F28(obj, 0);
    D_800D18A8 = 1;
    func_15082A44(&D_800D20FC[slot], slot, 0, 0, 0);
    STATE(obj)->unk1B0 = saved1B0;
    STATE(obj)->unk1AF = saved1AF;
    obj->pad320[8] = saved328;
    if (D_80082FA0 >= slot) {
        func_150054C4(&D_800DBFF0[slot], slot, D_800BE9F2, player);
        func_15181DC8(slot);
        func_1517EE40(0, 0, 0, 0, 1, slot);
        func_1517EE40(0, 0, 0, 30, 0, slot);
    } else {
        func_15085430(obj, player, 0);
    }
    D_800D18A8 = 0;
    STATE(obj)->state = 0;
    STATE(obj)->finished = FALSE;
    STATE(obj)->timer = 0;
    STATE(obj)->unk1AA = saved1AA;
    obj->health = D_8008726C;
    obj->unk22C |= 0x100;
    STATE(obj)->unk75 = 0;
    obj->unk20F = 0xFF;
    obj->pad210 = 0xFF;
    obj->unk211 = 0xFF;
    obj->unk232 = 1;
    obj->unk218 = NULL;
    STATE(obj)->unk11B = player;
    if (D_800BE616 != 0) {
        obj->immune = 150;
        STATE(obj)->unk1B1 = obj->immune;
        if (D_800BE9F0 == 0x33) {
            func_1507D158(slot, 120, 0, 0, 0);
            func_15088D58(obj);
        }
    }
}

/* Ends slot's current run: re-initialises it if func_150859AC(slot, 3) allows, else retires it (state 10). */
void func_1507D4F8(s32 slot) {
    struct127 *obj;
    ObjState *state;

    if (!(D_8008FDBC & 1)) {
        func_15085710(slot, 4, 1);
    }

    if (func_150859AC(slot, 3) != 0) {
        func_15085710(slot, 5, D_8008726C);
        obj = &D_800CC2D0[slot];
        obj->unkB2 = 0;

        if (D_800BE616 != 0) {
            if (*(s8 *)&D_800E0C20 == 0) {
                func_1507D1D8(obj);
                return;
            }
            STATE(obj)->state = 10;
            return;
        }

        D_800D18A8 = 1;
        if (!(D_800D2E4C->unk19 & 4) && D_8008FDA8 >= 0) {
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
        func_15085710(slot, 5, D_8008726C);
        func_15085710(slot, 2, D_80087260);
        func_1501C730(1, 0x18, 0, 0, 0);
    } else {
        D_800D18A0 |= 1 << slot;
    }

    obj = &D_800CC2D0[slot];
    state = STATE(obj);
    if (state->team == 0) {
        // matching: the volatile store and load make IDO reload unk31C after the decrement, as the original does
        *(volatile s8 *)&D_8008FD94 = *(s8 *)&D_8008FD94 - 1;
        state = *(ObjState *volatile *)&obj->unk31C;
    }
    state->state = 10;
    D_800BE618--;
}

/* Object returned by func_151149AC (struct131 in game_11FF10.c is a narrower view of the same 0xA0 bytes). */
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
void func_1503DE70(struct127 *, s32, s32);
void func_1507DB44(struct127 *obj, s32 arg1);

/* Death handler: dispatches on obj->id; returns 1 or 2 when the object was handled specially, else 0. */
/* Runs the per-id handler for obj (mostly id handler(obj, 0xFF, 1)); returns 1 or 2 for ids handled specially, else 0. */
s32 func_1507D754(struct127 *obj) {
    D754Obj *spawned;
    void *handle;

    switch (obj->id) {
    case 0x7F:
        func_150B0348(obj, 0xFF, 1);
        break;
    case 0x10:
    case 0x38:
    case 0x91:
        func_150BFFE0(obj);
        break;
    case 0x3A:
    case 0x3D:
        func_150D85AC(obj);
        break;
    case 0x4A:
        func_150F1D10(obj, 0xFF, 1);
        break;
    case 0x47:
        func_151C9740(obj, 0xFF, 1);
        break;
    case 0x25:
        if (obj->unk2EC != 0 || D_800BE9F0 == 6 || D_800BE9F0 == 0x3B) {
            func_151CFA4C(obj, 0xFF, 1);
        }
        return 0;
    case 0x79:
        func_150BE494(obj, 0xFF, 1);
        break;
    case 0x05:
    case 0xAD:
    case 0xAE:
    case 0xAF:
        obj->pad320[8] = 0;
        func_1507DB44(obj, 0);
        func_150AEB9C(obj);
        return 1;
    case 0x53:
        if (D_800BE616 != 0) {
            func_1507CD64(obj, 10);
            return 2;
        }
        break;
    case 0x90:
        if (D_800BE616 != 0 && D_800BE9F0 == 0x33) {
            func_1507CD64(obj, 8);
            return 2;
        }
        break;
    case 0x28:
    case 0x77:
        spawned = NULL;
        func_151BA518(obj, 0xFF, 1);
        func_15145A50(obj);
        if (D_800BE616 != 0) {
            func_1507CD64(obj, 6);
            spawned = func_151149AC((u8)(obj->unk127 + 11));
        } else {
            if (D_800BE9F0 == 0x23) {
                spawned = func_151149AC(1);
            }
            handle = func_1507515C(obj);
            if (handle != NULL && D_800C35EA != 1) {
                func_15136C3C(handle, 1, 1, 1, 1, 0, 0xFF, 1);
            }
        }
        if (spawned != NULL) {
            spawned->unk6E = 0;
            spawned->unk10 = obj->x_position;
            spawned->unk12 = obj->y_position;
            spawned->unk14 = obj->z_position;
            spawned->unk0 = obj->unkB8;
            spawned->unk4 = obj->unk40;
            spawned->unk8 = obj->unkC4;
            spawned->unk8A = 0xFF;
            spawned->unk7C = 0;
            spawned->unk80 = 0;
            spawned->unk84 = 0;
        }
        if (D_800BE616 != 0) {
            return 2;
        }
        break;
    case 0x0D:
        func_150B7B40(obj, 0xFF, 1);
        break;
    case 0x20:
        if (obj->y_velocity < -20.0f) {
            func_150BC488(obj);
        } else {
            func_150BBB5C(obj);
        }
        break;
    case 0x4F:
    case 0x83:
        func_151A6C90(obj, 0xFF, 1);
        break;
    case 0x34:
        func_150E0870(obj, 0xFF, 1);
        break;
    case 0x44:
        func_151D22B0(obj, 0xFF, 1);
        break;
    }
    return 0;
}

void func_1507DB34(void) {
}

void func_1507DB3C(void) {
}

void func_1507DB44(struct127 *obj, s32 arg1) {
    func_1503DE70(obj, arg1, -1);
}

void func_1507DB64(void) {
}

/* Layout of the 0x180-byte D_800BE628 records, as far as func_1507DB6C uses them. */
typedef struct ViewRecord {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ u8 pad34[0x14C];
} ViewRecord; // size 0x180

s32 func_1510D0EC(s32, s32 *, s32, s32);
extern Gfx D_80086B80[];
extern u8 D_D0F;
#define D_800D18A4_u8 ((u8 *)&D_800D18A4) /* one byte per player: half-size of the square, minus 4 */

/*
 * Draws a 32x32 RGBA32 textured square centred on player's view (coordinates in 10.2 fixed point). While the
 * player's D_800D18A2 bit is set the square shrinks each frame and, once gone, the bit is cleared; otherwise it
 * grows back to its full size.
 */
Gfx *func_1507DB6C(Gfx *gfx, s32 player) {
    s32 texture;
    s32 size;
    ViewRecord *view;
    s32 right, left, bottom, top;
    s32 scale;

    gSPDisplayList(gfx++, D_80086B80);
    texture = func_1510D0EC((s32)&D_D0F, NULL, 3, 0);
    gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, texture);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
    gDPLoadSync(gfx++);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 32 * 32 - 1, 0);
    gDPPipeSync(gfx++);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 31 << G_TEXTURE_IMAGE_FRAC, 31 << G_TEXTURE_IMAGE_FRAC);
    gDPSetOtherMode(gfx++,
                    0xF /* blend mask bits */ | G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE |
                        G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_OPA_SURF | G_RM_OPA_SURF2);
    if (D_800D18A2 & (1 << player)) {
        if (D_800D18A4_u8[player] >= 4) {
            D_800D18A4_u8[player] -= 4;
        } else {
            D_800D18A2 &= ~(1 << player);
            func_15181DC8(player);
            func_1517EE40(0, 0, 0, 120, 0, player);
            return gfx;
        }
    } else {
        D_800D18A4_u8[player] += 4;
        if (D_800D18A4_u8[player] > 60) {
            D_800D18A4_u8[player] = 60;
        }
    }
    size = D_800D18A4_u8[player] + 4;
    view = &((ViewRecord *)D_800BE628)[player];
    left = right = (s32)(view->unk30 + view->unk2C) * 2;
    top = bottom = (s32)(view->unk28 + view->unk24) * 2;
    left -= size;
    right += size;
    top -= size;
    bottom += size;
    scale = 0x20000 / (right - left);
    gSPTextureRectangle(gfx++, left, top, right, bottom, G_TX_RENDERTILE, 0, 0, scale, scale);
    return gfx;
}

void func_150836CC(struct127 *, s32);

/* Sets bits in obj->unk9C (after re-running func_150836CC 0x44/0x23 when interaction_state is 1). */
void func_1507DE4C(struct127 *obj) {
    if (obj->interaction_state == 1) {
        func_150836CC(obj, 0x44);
        func_150836CC(obj, 0x23);
        obj->unk9C |= 0xF000;
        func_150836CC(obj, 0x44);
        func_150836CC(obj, 0x23);
        return;
    }

    switch (obj->id) {
    case 0x9F:
    case 0xA0:
        obj->unk9C |= 0xF000;
        break;
    case 0x5A:
    case 0x74:
    case 0x7A:
        obj->unk9C |= 0xFF8;
        break;
    }
}

/* Sets obj's unk94/unk9C flags and unk2E4 (1, 2, 4 or 8) for part 4..9. */
void func_1507DF10(struct127 *obj, s32 part) {
    switch (part) {
    case 9:
        obj->unk94 |= 0x20;
        obj->unk9C |= 0x78;
        obj->unk2E4 = 1;
        return;
    case 8:
        obj->unk94 |= 0x40;
        obj->unk94 &= ~0x200;
        obj->unk9C |= 0xF00;
        obj->unk2E4 = 2;
        return;
    case 6:
    case 7:
        obj->unk94 |= 0xE;
        obj->unk94 &= ~0x410;
        obj->unk9C |= 0xEE0000;
        obj->unk2E4 = 4;
        return;
    case 4:
    case 5:
        obj->unk94 |= 0x80;
        obj->unk94 &= ~0x500;
        obj->unk2E4 = 8;
        return;
    }
}

/* Looks up resource's position and facing via func_15084D70 and calls func_1501D348(0x25, ...) from there. */
void func_1507DFE4(s32 resource, s32 arg1) {
    s32 savedMode;
    s16 pos[3];
    f32 vec[3];
    u8 unk4F;
    u8 angle;
    s32 unk48;
    s32 unk44;
    f32 yaw;
    s32 kind;
    s32 type;

    if (D_800C35EA != 1) {
        func_15084D70(0, resource, 1, pos, vec, &unk4F, &angle, &unk44, &unk48, 1, &type);
        yaw = (f32)(angle - 64) * (360.0f / 256.0f) + 180.0f;
        if (type == 0) {
            kind = 1;
        } else if (type == 1) {
            kind = 3;
        } else {
            return;
        }
        func_15022190(pos[0], pos[1], pos[2], yaw);
        savedMode = D_800BE9F0;
        D_800BE9F0 = 0x25;
        D_800C3671 = 1;
        func_1501D348(0x25, kind, 0, 0, 0);
        D_800C3670 = 1;
        D_800BE9F0 = savedMode;
    }
}

/* Returns 0, or 1 + the type func_15084D70 reports for D_800BE3E0 (after setting D_800BE9F4 from D_800BE3DF). */
s32 func_1507E114(s32 arg0) {
    s32 type;
    s16 pos[3];
    f32 vec[3];
    s32 unk44;
    u8 unk43;
    u8 unk42;
    s32 unk3C;
    s32 mode;

    if (D_800D18A8 == 0) {
        return 0;
    }
    mode = D_800BE9F4;
    if (mode == 0x22 || mode == 0x18) {
        return 0;
    }
    D_800BE9F4 = (s8)D_800BE3DF;
    func_15084D70(0, D_800BE3E0, 1, pos, vec, &unk43, &unk42, &unk44, &unk3C, 1, &type);
    return type + 1;
}
/* unk1D4 points at an array of 4x4 f32 bone matrices (0x40 bytes each). */
typedef f32 BoneMtx[4][4];

void func_15143134(f32 *in, f32 *out, BoneMtx *mtx);

/* Gets obj's tracking point: 30 * y_scale up through bone 0 (12 or 3 in some states), or its position. */
void func_1507E1D0(struct127 *obj, f32 *x, f32 *y, f32 *z) {
    if (obj->unk1D4 != NULL) {
        BoneMtx *bone;
        f32 in[3];
        f32 out[3];

        in[0] = 0.0f;
        in[1] = obj->y_scale * 30.0f;
        in[2] = 0.0f;
        bone = (BoneMtx *)obj->unk1D4;
        if (obj->interaction_state == 1) {
            bone += 12;
        } else if (obj->interaction_state == 0x1E) {
            bone += 3;
        }
        func_15143134(in, out, bone);
        *x = out[0];
        *y = out[1];
        *z = out[2];
    } else {
        *x = obj->x_position;
        *y = obj->y_position;
        *z = obj->z_position;
    }
}
