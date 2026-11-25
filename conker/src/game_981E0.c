#include <ultra64.h>

#define func_150ADA20 func_150ADA20_u8_proto
#define func_10010154 func_10010154_hdr
#define func_10010630 func_10010630_hdr
#include "functions.h"
#undef func_150ADA20
#undef func_10010154
#undef func_10010630
/* volume (arg2) is unsigned at the call sites. */
u16 func_10010154(u16 arg0, void *arg1, u32 arg2, s16 arg3, u16 arg4);
void func_10010630(u16 arg0, struct127 *arg1, u32 arg2, s16 arg3, u16 arg4);
#define D_80099A3C D_80099A3C_s16_hdr
#define D_80099A3E D_80099A3E_s16_hdr
#include "variables.h"
#undef D_80099A3C
#undef D_80099A3E
/* variables.h types these as s16; they are byte tables (lbu). */
extern u8 D_80099A3C[];
extern u8 D_80099A3E;

/*
 * struct127 fields that include/structs.h only covers with padding, as a same-layout view.
 * (Fold into struct127 in the central pass.)
 */
typedef struct struct127Hidden {
    /* 0x000 */ u8 pad0[0x70];
    /* 0x070 */ u8 unk70;
    /* 0x071 */ u8 pad71[0x136 - 0x71];
    /* 0x136 */ u8 unk136;
    /* 0x137 */ u8 unk137;
    /* 0x138 */ u8 pad138[0x160 - 0x138];
    /* 0x160 */ s16 unk160;
    /* 0x162 */ u8 pad162[0x288 - 0x162];
    /* 0x288 */ u8 unk288;
    /* 0x289 */ u8 pad289[0x2D8 - 0x289];
    /* 0x2D8 */ f32 unk2D8;
    /* 0x2DC */ f32 unk2DC;
    /* 0x2E0 */ f32 unk2E0;
    /* 0x2E4 */ u8 pad2E4[0x304 - 0x2E4];
    /* 0x304 */ s32 unk304[4];
    /* 0x314 */ s32 unk314;
    /* 0x318 */ u8 pad318[0x32C - 0x318];
} struct127Hidden;
#define HIDDEN(obj) ((struct127Hidden *)(obj))

/* Same for struct126 (struct127.unk31C). */
typedef struct struct126Hidden {
    /* 0x000 */ f32 unk0;
    /* 0x004 */ u8 pad4[0x24 - 0x4];
    /* 0x024 */ s16 unk24;
    /* 0x026 */ u8 pad26[0xDC - 0x26];
    /* 0x0DC */ f32 unkDC[3];
    /* 0x0E8 */ u8 unkE8[0x12C - 0xE8];
    /* 0x12C */ u16 unk12C;
    /* 0x12E */ u8 pad12E[0x197 - 0x12E];
    /* 0x197 */ u8 unk197;
} struct126Hidden;
#define HIDDEN126(ext) ((struct126Hidden *)(ext))

void func_15131D4C(s32 arg0, s32 arg1);
void func_1506BF5C(void);
s32 func_1000F568(s32 arg0, s32 arg1);
extern s32 func_151EF610(void);
extern s32 D_800D187C;
extern void *func_150335C8(struct127 *arg0, struct127 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void func_15166118(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
extern void func_151942E8();
extern f32 D_8009A104;
void func_151C0698(struct127 *arg0, struct127 *arg1, void *arg2, u8 arg3, s32 arg4, s32 arg5);

struct func_1506F1A8_stack {
    s32 unk00;
    struct127 *unk04;
    u8 unk08;
    s8 unk09;
    u8 pad0A[2];
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s8 unk38;
    s8 unk39;
    s8 unk3A;
    s8 unk3B;
    s8 unk3C;
    s8 unk3D;
    u8 unk3E;
    u8 pad3F[1];
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s8 unk4C;
    u8 pad4D[3];
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    s8 unk54;
    u8 pad55[3];
    f32 unk58;
    f32 unk5C;
    s16 unk60;
    s16 unk62;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    s16 unk8C;
    s16 unk8E;
    f32 unk90;
    u8 unk94;
    u8 unk95;
    u8 pad96[2];
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    f32 unkAC;
    s32 unkB0;
    s32 unkB4;
    s16 unkB8;
    s16 unkBA;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    s16 unkD8;
    s16 unkDA;
    s16 unkDC;
    s16 unkDE;
    f32 unkE0;
    f32 unkE4;
    f32 unkE8;
    f32 unkEC;
    f32 unkF0;
    s16 unkF4;
    s16 unkF6;
    s16 unkF8;
    s16 unkFA;
    f32 unkFC;
    f32 unk100;
    f32 unk104;
    f32 unk108;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    s16 unk118;
    s16 unk11A;
};

void func_151994B8(s32 arg0, struct func_1506F1A8_stack *arg1, s32 arg2, s32 arg3);
extern f32 D_80099DAC;
extern f32 D_80099DB0;
extern f32 D_80099DB4;
extern f32 D_80099DB8;
extern f32 D_80099DBC;
extern f32 D_80099DC0;
extern f32 D_80099DC4;
extern f32 D_80099DC8;
extern f32 D_80099DCC;
extern f32 D_80099DD0;
extern f32 D_80099DD4;
extern f32 D_80099DD8;
extern f32 D_80099DDC;
extern f32 D_80099DE0;
extern f32 D_80099DE4;
extern f32 D_80099DE8;
extern f32 D_80099DEC;
extern f32 D_80099DF0;
extern f32 D_80099DF4;
extern f32 D_80099DF8;
extern f32 D_80099DFC;
extern f32 D_80099E00;
extern f32 D_80099E04;
extern f32 D_80099E08;
extern f32 D_80099E0C;
extern f32 D_80099E10;
extern f32 D_80099E14;
extern f32 D_80099E18;
extern f32 D_80099E1C;
extern f32 D_80099E20;

/* matching: four scalars, not s32[4] (an array gets an 8-aligned, larger frame) */
struct func_1506C32C_stack {
    s32 sound0;
    s32 sound1;
    s32 sound2;
    s32 sound3;
};

typedef struct Vec3w_15070D24 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Vec3w_15070D24;

typedef struct Stack_15070D24 {
    Header2 header2;
    Header header;
    u8 pad16[2];
    f32 sp64[3];
    f32 sp70[2];
    Vec3w_15070D24 sp78;
    Vec3w_15070D24 sp84;
} Stack_15070D24;

extern Vec3w_15070D24 D_80099B88;
extern Vec3w_15070D24 D_80099B94;
extern Vec3w_15070D24 D_80099BBC;
extern f32 D_8009A094;
extern f32 D_80099F2C;
extern s32 func_150ADA20(void);
extern void func_15143134(void *, void *, s32);
extern void func_151DC484(f32 *, struct199 *, s32, u8, s32);
extern void func_15102B38(s32, u8, s32, s32, f32 *, s32, s32, f32, s32, s32, s32, s32, u8, s32);
extern void func_150E1570(struct127 *, f32, f32, f32, f32, f32, f32, s32, s32);


typedef struct {
    f32 time;
    u8 pad4[3];
    u8 id;
    s32 parameter;
} Game6AD30Event;

typedef struct {
    u8 pad0[8];
    f32 time;
    u8 padC[0x1C];
    s32 token;
} Game6AD30State;

typedef struct {
    u8 pad0[5];
    u8 kind;
    u8 pad6[0xAE];
    f32 previousTime;
    u8 padB8[0x80];
    u8 index;
    u8 pad139[0x8B];
    Game6AD30Event *events;
    u8 pad1C8[0x108];
    Game6AD30State *state;
} Game6AD30Actor;

typedef void Game6AD30Callback(void);
extern Game6AD30Callback *D_800863FC[];
extern u8 D_80099A30[];
extern u8 D_800C365E;

/*
 * Runs every event of the actor's event list whose time is <= `time`. Each event stores its
 * parameter in D_800D1580 (and as a float in D_800D1874) and calls handler D_800863FC[id].
 * With `filtered` set, only ids in the zero-terminated list D_80099A30 run. A handler that
 * restarts the actor's state (new token) rewinds `time` to the state's time. Returns
 * D_800D1878, which handlers may set.
 */
f32 func_1506AD30(Game6AD30Actor *actor, f32 time, u8 filtered) {
    s32 token;
    u8 id;

    if (actor->kind == 4) {
        return 0.0f;
    }
    D_800D1878 = 0.0f;
    if (time < actor->previousTime) {
        actor->index = 0;
    }
    D_800D1880 = 1;
    while (actor->events != NULL && actor->events[actor->index].time <= time && D_800D1880 != 0) {
        if (actor->events[actor->index].time < 900.0f) {
            D_800D1580 = actor->events[actor->index].parameter;
            D_800D1874 = *(f32 *)&D_800D1580;
            token = actor->state->token;
            if (D_800C35EA == 0 || D_800C365E != 0) {
                id = actor->events[actor->index].id;
                if (filtered) {
                    s32 i;
                    for (i = 0; filtered; i++) {
                        if (D_80099A30[i] == 0) {
                            id = 0;
                        }
                        if (id == D_80099A30[i]) {
                            break;
                        }
                    }
                }
                if (id != 0) {
                    D_800863FC[id]();
                }
            }
            if (token != actor->state->token) {
                time = actor->state->time;
            } else {
                actor->index++;
            }
        }
    }
    return D_800D1878;
}

void func_1506AF74(void) {
    if ((D_800D154C->unk100 & 8) != 0) {
        D_800D154C->unk100 |= 4;
    }
    D_800D154C->unk100 &= 0xF7;
    func_1505E650(D_800D154C, 0x38, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506AFE0(void) {
    func_1505E650(D_800D154C, 0x48, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506B020(void) {
    D_800D154C->unk1CB = 1;
    func_1505E650(D_800D154C, 0x39, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506B070(void) {
}

void func_1506B078(void) {
    f32 speed = (D_800D154C->y_position - (D_800D154C->unk118 - 150.0f)) * 0.005f;

    if (speed < 0.0f) {
        speed = 1.0f;
    } else {
        speed += 1.0f;
    }
    func_1506B100(209, speed, 4.0f);
}

/* func_1505E650 on the current object with the given id and two float arguments. */
void func_1506B100(s32 id, f32 arg1, f32 arg2) {
    func_1505E650(D_800D154C, id, arg1, arg2, 0.0f, 0.0f, 0);
}

void func_1506B14C(void) {
    func_1505E650(D_800D154C, D_800D154C->unk84.uh + 1, D_800D154C->animation_speed, 3.0f, 0.0f, 0.0f, 0);
}

void func_1506B198(void) {
    func_1505E650(D_800D154C, D_800D154C->unk84.uh + 1, D_800D154C->animation_speed, 3.0f, 0.0f, 0.0f, 1);
}

void func_1506B1E8(void) {
    func_1505E650(D_800D154C, 0x3C, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_1506B228(void) {
    func_1505E650(D_800D154C, 0x54, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_1506B268(void) {
    D_800D154C->unk83 = 0;
    D_800D154C->disable_run = 0;
    func_1505E650(D_800D154C, 0xF, 1.0f, 6.0f, 0.0f, 0.0f, 0);
}

void func_1506B2BC(void) {
    struct127 *obj = func_150721E8(D_800D154C);

    if (obj != NULL && (obj - D_800CC2D0) + 1 == D_800CC2D0[0].unk274) {
        D_800D154C->unk218 = NULL;
        D_800D154C->unk232 = 4;
    }
}

void func_1506B328(void) {
    func_1505E650(D_800D154C, 0x3E, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506B368(void) {
}

void func_1506B370(void) {
    func_1507D4F8(D_800D154C - D_800CC2D0);
}

void func_1506B3B0(void) {
}

/* Picks unk244 from the D_80099A3C table by the angle between the object's facing and the player. */
void func_1506B3B8(void) {
    s32 absDiff32;
    s16 angleDiff;
    u8 sel = 0;
    u8 absDiff;

    angleDiff = (D_800D154C->unk7A + (D_800D154C->unk1FD << 8)) -
                func_1505A630(D_800CC2D0[0].x_position - D_800D154C->x_position,
                              D_800D154C->z_position - D_800CC2D0[0].z_position, 0);
    absDiff = angleDiff >> 8;
    if (absDiff & 0x80) {
        absDiff = -absDiff;
    }
    absDiff32 = absDiff;
    if (absDiff < D_80099A3C[sel]) { // matching: indexed by sel (still 0); [0] schedules differently
        sel = 1;
    } else {
        if (D_80099A43 < absDiff32) {
            sel = 5;
        } else if (D_80099A3E < absDiff32) {
            sel = 2;
        }
        if (angleDiff < 0) {
            sel += 4;
        } else {
            sel += 3;
        }
    }
    D_800D154C->unk138 = 0;
    D_800D154C->unk244 = D_80099A3C[sel];
    D_800D154C->unk21C = 20000;
}

void func_1506B4EC(void) {
    func_1506B3B8();
}

void func_1506B50C(void) {
    D_800D154C->unk21C = 0;
}

void func_1506B520(void) {
    s32 id;

    if (D_800D154C->health != 0) {
        if (D_800D154C->unk31C != NULL && D_800D154C->unk31C->unk78 == 0x25) {
            id = 427;
        } else {
            id = 240;
        }
        func_1505E650(D_800D154C, id, 1.5f, 3.0f, 0.0f, 0.0f, 0);
    } else {
        func_1507CD64(D_800D154C, 1);
    }
}

void func_1506B5A4(void) {
}

void func_1506B5AC(void) {
}

void func_1506B5B4(void) {
    D_800D154C->unk1CB = 1;
}

void func_1506B5CC(void) {
    D_800D154C->unk1CB = 1;
}

void func_1506B5E4(void) {
    func_1506160C(D_800D154C, 2, 7, 8, 0);
    D_800D154C->animation_speed = 0.0f;
    D_800D154C->unk2D0->unk10 = 0.0f;
}

/* Picks a random entry (0..count-1) of D_80099ABC / D_80099AB4 for the object's unk31C->unkE. */
void func_1506B634(s32 count) {
    u32 rand;
    u32 pick;
    u8 index;
    struct126 *ext;

    rand = func_150ADA20();
    pick = rand % (u32)(u8)count;
    index = pick;
    ext = D_800D154C->unk31C;
    if (ext->unk16 != 0) {
        if (D_800BE9F0 == 41) {
            if ((u8)pick == 7) {
                index = 0;
            }
        }
        ext->unkE = ((u8 *)D_80099ABC)[index];
    } else {
        ext->unkE = ((u8 *)D_80099AB4)[(u8)pick];
    }

    if (D_800D154C->unk31C->unkE == 0xA7) {
        D_800D1580 = 0xFF020144;
        D_800D154C->unk31C->unkC = (func_150ADA20() % 3U) + 2;
        func_1506E8D8();
        D_800D154C->xz_velocity = D_800D154C->xz_velocity * 0.2f;
    }
}

void func_1506B740(void) {
    u8 roll = func_150ADA20() & 0xFF;

    if (D_800D154C->unk118 != -10000.0f && D_800D154C->y_position < D_800D154C->unk118) {
        D_800D154C->unk31C->unkE = 720;
    } else {
        if (D_800D154C->unk84.uh == 0xA7) {
            if (D_800D154C->unk31C->unkC == 0) {
                roll = 0;
            } else {
                D_800D154C->unk31C->unkC--;
                roll = 0xFF;
            }
        }
        if (roll < 0x80) {
            func_1506B634(8);
        }
    }
}

void func_1506B7F4(void) {
    D_800D154C->disable_run = 0;
    D_800D154C->unk83 = 0;
    func_1506B634(2);
}

void func_1506B82C(void) {
    D_800D154C->disable_run = 0;
    D_800D154C->unk83 = 0;
    func_1507F640();
}

void func_1506B860(void) {
    func_1506B100(181, 0.6299999952316284f, 5.0f);
}

void func_1506B88C(void) {
    func_1506B100(182, 1.0f, 4.0f);
}

void func_1506B8B4(void) {
    D_800D154C->unk31C->unk78 = 0;
    D_800D154C->disable_run = 0;
    D_800D154C->unk83 = 0;
    func_1507F640();
}

void func_1506B8F4(void) {
    func_1506B100(222, 0.75f, 4.0f);
}

void func_1506B91C(void) {
    func_1506B100(191, 1.0f, 4.0f);
}

void func_1506B944(void) {
    D_800D154C->disable_run = 0;
    D_800D154C->unk83 = 0;
    func_1506B100(197, 1.0f, 4.0f);
}

void func_1506B984(void) {
    func_1506B100(175, 1.0f, 4.0f);
}

void func_1506B9AC(void) {
    D_800D154C->unk244 = 0;
}

void func_1506B9C0(void) {
    struct126 *ext;

    D_800BE720[D_800C3E78] |= 0x4000;
    ext = D_800D154C->unk31C;
    if (ext != NULL) {
        ext->unk78 = 0;
        D_800D2E60[D_800D1940 >> 3] |= 1 << (D_800D1940 & 7);
    }
    func_1507F640();
}

/* Sound handlers: play sound D_800D1580 / D_800D1582 at the current object (0 stops it). */
void func_1506BA4C(s32 minDistance, s32 maxDistance) {
    if (D_800D1580 == 0) {
        func_100109D0(D_800D154C);
    } else {
        if (D_800D154C->camera == 0) {
            func_10010154(D_800D1580, D_800D154C, 28000, minDistance, maxDistance);
        } else {
            func_10010154(D_800D1580, D_800D154C, 24000, 500, 2500);
        }
    }
}

void func_1506BAD8(s32 minDistance, s32 maxDistance) {
    if (D_800D1580 == 0) {
        func_10010A3C(D_800D154C);
    } else {
        if (D_800D154C->camera == 0) {
            func_10010344(D_800D1580, D_800D154C, 28000, minDistance, maxDistance);
        } else {
            func_10010344(D_800D1580, D_800D154C, 24000, 500, 2500);
        }
    }
}

void func_1506BB64(s32 minDistance, s32 maxDistance) {
    func_10012718(D_800D1582, D_800D154C, 28000, minDistance, maxDistance);
}

void func_1506BBA8(s32 minDistance, s32 maxDistance) {
    if (D_800D154C->camera == 0) {
        func_10010154(D_800D1582, D_800D154C, 14000, minDistance, maxDistance);
    } else {
        func_10010154(D_800D1582, D_800D154C, 12000, 500, 2500);
    }
}

void func_1506BC24(s32 minDistance, s32 maxDistance) {
    if (D_800D154C->camera == 0) {
        func_10010344(D_800D1582, D_800D154C, 14000, minDistance, maxDistance);
    } else {
        func_10010344(D_800D1582, D_800D154C, 12000, 500, 2500);
    }
}

void func_1506BCA0(void) {
    D_800D154C->y_velocity = D_800D1580;
}

/* Jump: sets y_velocity from D_800D1580 when the object is (nearly) at height unk180. */
void func_1506BCC8(void) {
    struct127 *obj;
    f32 height;

    obj = D_800D154C;
    if (obj->interaction_state == 0x16) {
        if (obj->y_position == obj->unk180) {
            D_800D154C->y_velocity = (func_151EF610() % 8) + D_800D1580;
            obj = D_800D154C;
            if (obj->xz_velocity != 0.0f) {
                obj->y_velocity = obj->y_velocity * (obj->xz_velocity / 40.0f);
            } else {
                obj->y_velocity = 0.0f;
            }
        }
    } else {
        height = obj->y_position - obj->unk180;
        if (-5.0f < height && height < 5.0f) {
            obj->y_velocity = D_800D1580;
        }
    }
}

void func_1506BDE8(void) {
    if (D_800D154C->y_position < -1750.0f) {
        D_800D154C->y_velocity = D_800D1580;
    }
}

void func_1506BE2C(void) {
    D_800D154C->gravity = D_800D1580;
}

void func_1506BE54(void) {
    D_800D154C->gravity = D_800D1580 * 0.1f;
}

void func_1506BE84(void) {
    D_800D154C->disable_run = 0;
}

void func_1506BE98(void) {
    D_800D154C->target_speed = D_800D1580;
}

void func_1506BEC0(void) {
    D_800D154C->disable_run = D_800D1580;
}

void func_1506BEDC(void) {
    D_800D154C->unkD0 = D_800D1580;
    D_800D154C->unk114 = D_800D1580 >> 8;
}

void func_1506BF1C(void) {
    if (D_800D154C->unk118 <= D_800D154C->y_position) {
        func_1506BF5C();
    }
}

extern u8 *D_80086CAC[];
u16 func_10011EB8(s32 arg0, u16 *arg1, s32 arg2);
s32 func_1510F8CC(s32);

void func_1506BF5C(void) {
    u32 sound;
    u32 owner;
    u16 volume;
    f32 level;
    f32 max;
    u32 top;
    s32 type;
    u8 id;

    sound = D_800D1580 & 0x7FF;
    id = D_800D154C->id;
    top = (u32)D_800D1580 >> 24;
    if (D_800D1C90[id]->unk4 != 0) {
        owner = *D_80086CAC[(u8)D_800D1C90[id]->unk5];
    } else {
        owner = id;
    }
    D_800D187C = 0;
    switch (D_800D1580 & 0xC000) {
    case 0x4000:
        if (owner != top) {
            return;
        }
        break;
    case 0x8000:
        if (owner == top) {
            return;
        }
        break;
    }
    if (D_800D1580 & 0x10000) {
        D_800D187C = D_800D1580 + 1;
        return;
    }
    volume = 32000;
    switch (D_800D1580 & 0x3800) {
    case 0x2000:
        if (D_80082FA0 >= 2) {
            break;
        }
        type = func_1510F8CC(D_800D154C->unk184);
        if (type == 0xC && D_800D154C->unkCC > -17.0f) {
            sound = func_1000F568(0x9C, 2);
        } else if (D_800D154C->unk118 != -10000.0f && D_800D154C->y_position <= D_800D154C->unk118) {
            sound = func_10011EB8(8, NULL, sound);
            type = 8;
        } else {
            sound = func_10011EB8(type, &volume, sound);
        }
        if (sound == 0) {
            break;
        }
        if (D_800D154C->unk31C != NULL) {
            level = *(f32 *)D_800D154C->unk31C;
        } else {
            level = D_800D154C->y_velocity;
        }
        if (type == 0xC) {
            max = 10.0f;
        } else {
            max = 30.0f;
        }
        if (max < level) {
            level = max;
        }
        if (level < 0.0f) {
            level = 0.0f;
        }
        level *= (f32)0.5 / max; // matching: 0.5f compiles differently
        level += 0.5f;
        volume = volume * level;
        if (type == 8) {
            func_10010630(sound, D_800D154C, volume, 500, 2500);
            break;
        }
        /* fallthrough */
    case 0:
        func_10010154(sound, D_800D154C, volume, 500, 2500);
        break;
    case 0x1000:
        func_10010344(sound, D_800D154C, 32000, 500, 2500);
        break;
    case 0x3800:
        if (func_1001147C(D_800D154C->unk8E) != sound) {
            func_10010154(sound, D_800D154C, volume, 500, 2500);
        }
        break;
    case 0x3000:
        func_10010630(sound, D_800D154C, 32000, 500, 2500);
        break;
    }
}

/*
 * Follow-up of func_1506BF5C's deferred sound (D_800D187C = parameter + 1): picks one of up to
 * four 11-bit sound ids (the deferred one plus three packed in D_800D1580) and plays it.
 */
void func_1506C32C(void) {
    s32 cmd;
    struct func_1506C32C_stack sounds;
    s32 index;
    s32 sound;

    if (D_800D187C != 0) {
        D_800D187C--;
        cmd = D_800D1580;
        sounds.sound0 = D_800D187C & 0x7FF;
        sounds.sound1 = cmd & 0x7FF;
        sounds.sound2 = (cmd >> 11) & 0x7FF;
        sounds.sound3 = (cmd >> 22) & 0x7FF;

        if (sounds.sound3 != 0) {
            index = 4;
        } else if (sounds.sound2 != 0) {
            index = 3;
        } else if (sounds.sound1 != 0) {
            index = 2;
        } else {
            index = 0;
        }

        if (index != 0) {
            index = func_1000F568(sounds.sound0, index) - sounds.sound0;
        }

        sound = (&sounds.sound0)[index];
        if (sound != 0) {
            D_800D187C &= 0xFFFEF800;
            D_800D1580 = sound | D_800D187C;
            func_1506BF5C();
        }
    }
}

void func_1506C418(void) {
    func_10010A3C(D_800D154C);
}

void func_1506C43C(void) {
    func_100109D0(D_800D154C);
}

// requires jump table
extern struct127 *func_1505EEB0(s32 state, s32 *arg1);
extern u8 func_1505DADC(struct127 *arg0, s16 *arg1, s32 arg2, u8 arg3, s32 arg4);
extern void *allocate_memory(s32, s32, s32, s32);
extern s16 D_80099AD8[][4];
extern void func_1506AC8C(struct127 *, s32, s32);
extern struct127 *func_1505ED34(void);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern s32 func_1515D480(s32);
extern s32 *func_1515D440(void);
extern s32 func_15158BD0(struct127 *, s32, s32);
extern void func_1514EC1C(s32, struct127 *, s32);
extern u8 D_800C3E90;
extern u8 D_80099AC4[];


/*
 * Spawns a projectile object of the given kind (D_80099AD8 / D_80099AC4 rows) from the current
 * object, at `pos` (an f32[3]), at the bone from D_80099AD8, or just above the object. `flags`
 * aims it: 1 = no auto-aim, 2 = face the player, 4 = arc onto the player, 8 = random yaw spread,
 * 0x10 = lead the target unk222, 0x20 = remember unk222. Returns the new object.
 */
u8 *func_1506C460(f32 yaw, f32 speed, f32 yVelocity, f32 gravity, s32 unk83, s32 kind, f32 minHeight,
                  f32 scale, s32 pos, s32 flags, s32 arg10) {
    struct127 *obj;
    s16 aimAngle;
    s16 angle;
    s32 slot;
    s32 anim;
    s32 count;
    s32 i;
    f32 *mtx;
    f32 x;
    f32 y;
    f32 z;
    f32 mf[4][4];

    count = 0;
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state == 4) {
            count++;
        }
    }
    if (count >= 5) {
        obj = func_1505EEB0(4, &slot);
        func_15060F28(obj, 0);
    } else {
        obj = func_1505ED34();
        if ((u32)(obj - D_800CC2D0) >= 25 && count > 0) {
            obj = func_1505EEB0(4, &slot);
            func_15060F28(obj, 0);
        }
    }
    HIDDEN(obj)->unk136 = 11;
    obj->interaction_state = 4;
    obj->health = 1;
    obj->unk7A = D_800D154C->unk7A;
    if (pos != 0) {
        obj->x_position = ((f32 *)pos)[0];
        obj->y_position = ((f32 *)pos)[1];
        obj->z_position = ((f32 *)pos)[2];
    } else if (D_800D154C->unk1D4 == NULL) {
        obj->x_position = D_800D154C->x_position;
        obj->y_position = D_800D154C->y_position + 50.0f;
        obj->z_position = D_800D154C->z_position;
    } else {
        mtx = (f32 *)D_800D154C->unk1D4;
        if (D_800C3E90) {
            guMtxL2F(mf, (Mtx *)((u8 *)mtx + (D_80099AD8[kind][3] << 6)));
            mtx = (f32 *)mf;
        } else {
            mtx = (f32 *)((u8 *)mtx + (D_80099AD8[kind][3] << 6));
        }
        func_150A7960(mtx, D_80099AD8[kind][0], D_80099AD8[kind][1], D_80099AD8[kind][2], &x, &y, &z);
        obj->x_position = x;
        obj->y_position = y;
        obj->z_position = z;
    }
    if (minHeight != 0.0f) {
        if (obj->y_position < D_800D154C->y_position + minHeight) {
            obj->y_position = D_800D154C->y_position + minHeight;
        }
    }
    HIDDEN(obj)->unk160 = 110;
    obj->unique_id = 0xFF - (obj - D_800CC2D0);
    if (kind == 0 || kind == 1) {
        obj->unk5 = 2;
        obj->unk103 = 1;
    } else {
        obj->unk5 = 5;
        for (i = 0; i <= D_80082FA0; i++) {
            HIDDEN(obj)->unk304[i] = func_1515D480(1);
        }
        if (HIDDEN(obj)->unk304[i - 1] != 0) {
            obj->unk301[0] = 1;
        }
        HIDDEN(obj)->unk314 = func_1515D440();
        switch (kind) {
        case 2:
            obj->unkB8 = 260.0f;
            anim = 0x2A;
            obj->unk103 = 1;
            break;
        case 18:
            obj->unkB8 = 260.0f;
            anim = 0x22;
            obj->unk103 = 1;
            break;
        case 11:
            anim = 0x6D;
            break;
        case 12:
            anim = 0x6B;
            break;
        case 3:
            anim = 0x39;
            break;
        case 6:
            anim = 0x51;
            break;
        case 15:
            anim = 0x93;
            break;
        case 17:
            anim = 0x39;
            break;
        default:
            anim = 0x50;
            break;
        }
        func_15083568(obj, anim, 1.0f, 0);
    }
    angle = (s16)(yaw * 182.04445f) - 0x4000;
    if (D_800BE616 && !(flags & 1) && kind != 5 && kind != 9 && kind != 10 && kind != 4) {
        u8 ret;
        f32 dist;
        u16 tmp;
        f32 targetY;

        aimAngle = tmp = func_1505A630(speed, yVelocity, 0);
        ret = func_1505DADC(obj, &aimAngle, (tmp >> 8) & 0xFF, 0xFF, 20);
        if (ret != 0xFF) {
            targetY = D_800CC2D0[ret].y_position;
            dist = func_1505A6F8(D_800D154C, &D_800CC2D0[ret]);
            angle = aimAngle;
            if (kind == 2 || kind == 18) {
                targetY -= 30.0f;
            }
            yVelocity = (targetY - D_800D154C->y_position) / (dist / speed);
            gravity = 0.0f;
        }
    }
    if (flags & 2) {
        angle = func_1505A630(D_800CC2D0[0].x_position - obj->x_position, obj->z_position - D_800CC2D0[0].z_position, 0);
    }
    if (flags & 4) {
        yVelocity = ((D_800CC2D0[0].y_position + 60.0f) - obj->y_position) / (func_1505A6F8(obj, D_800CC2D0) / speed);
    }
    if (flags & 8) {
        angle += (u16)(func_150ADA20() % 0x1200U) - 0x900;
    }
    if (flags & 0x10) {
        f32 pad1; // matching: three unused stack slots
        f32 pad2;
        f32 pad3;
        f32 tx;
        f32 ty;
        f32 tz;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 offx;
        f32 offz;
        f32 offy;
        struct127 *target;
        f32 hdist;
        f32 pitch;
        f32 dummy;

        target = &D_800CC2D0[D_800D154C->unk222];
        func_1505A184(target->unk76, target->xz_velocity * 4.0f, 0.0f, &offx, &offz, &offy);
        tx = target->x_position + offx;
        tz = target->z_position + offz;
        ty = (target->y_position + 30.0f) + target->y_velocity * 4.0f;
        dx = tx - obj->x_position;
        dy = ty - obj->y_position;
        dz = tz - obj->z_position;
        hdist = sqrtf(dx * dx + dz * dz);
        pitch = func_150484A0(dy, hdist) * 10430.378f; /* radians to s16 angle units */
        angle = func_1505A630(dx, -dz, 0);
        func_1505A184((s16)pitch, speed, 0.0f, &speed, &yVelocity, &dummy);
        yVelocity = -yVelocity;
    }
    obj->unk7A = angle;
    obj->unk76 = angle;
    obj->unk40 = (s16)((u16)angle + 0x4000) * 0.0054931640625f;
    obj->gravity = gravity;
    obj->y_velocity = yVelocity;
    obj->xz_velocity = speed;
    obj->unk83 = *((u8 *)&unk83 + 3); // matching: lbu from the arg slot (a cast emits andi)
    obj->unk81 = 0;
    obj->unk7F = 0;
    obj->unk80 = 0xFF;
    if (flags & 0x20) {
        obj->pad126 = obj->unk80 = D_800D154C->unk222;
    }
    obj->unk82 = 0x26;
    if (arg10 == 0) {
        obj->unkD0 = D_80099AC4[kind] + 1;
    }
    obj->unk28 = 100.0f;
    obj->unk114 = 255.0f;
    obj->unkE4 = 5;
    obj->unkE6 = 5;
    obj->unkEC = 1.0f;
    obj->unkF0 = 1.0f;
    obj->unkDC = 1.0f;
    obj->unkE0 = 1.0f;
    obj->unk124 = D_800C3E78 + 1;
    obj->unk122 = 0x11;
    obj->unk232 = 0;
    obj->immune = obj->unk232 * 0 + 0xFF; // matching: IDO reuses the zero of the unk232 store
    obj->unkF8 = 0x100E;
    if (arg10) {
        obj->unkF8 |= 0x8000;
        bzero(obj->unk31C = allocate_memory(0x1C0, 1, 0, 0), 0x1C0);
        obj->unk103 = 1;
    }
    obj->xz_scale = scale;
    obj->y_scale = scale;
    obj->pad66 = D_800D154C->pad66 & 0xC;
    obj->unk278 = kind;
    obj->unk284 = arg10;
    if (kind == 2 || kind == 18) {
        obj->unk282 = 60;
    }
    switch (kind) {
    case 1:
        func_1506AC8C(obj, 1, 0);
        func_1514EC1C(func_15158BD0(obj, 1, 0), obj, 0x13);
        break;
    case 15:
        func_1506AC8C(obj, 13, 0);
        break;
    case 13:
        func_10010154(567, obj, 30000, 500, 2000);
        /* fallthrough */
    case 5:
    case 9:
    case 10:
        func_1506AC8C(obj, 12, 0);
        break;
    case 0:
        func_1506AC8C(obj, 8, 0);
        func_1514EC1C(func_15158BD0(obj, 1, 0), obj, 0x13);
        break;
    }
    return (u8 *)obj;
}
// requires jump table
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern void func_15145974(void *, f32 *, f32 *);
extern void *D_800CC5EC;

/* Fires projectile kind D_800D1580 from the current object, aimed by its facing and camera pitch. */
void func_1506CE6C(void) {
    s32 bone;
    s32 kind;
    f32 elevation;
    f32 heading;
    f32 dist;
    f32 gravity;
    f32 speed;
    f32 yVelocity;
    f32 scale;
    f32 unused;
    f32 (*mtxs)[16];
    f32 x;
    f32 y;
    f32 z;
    f32 a;
    f32 b;
    f32 pad; // matching: unused stack slot
    s32 allocExt;
    s32 flags;
    f32 *pos;
    f32 vec[3];

    flags = 0;
    gravity = 0.0f;
    elevation = 45.0f;
    dist = 323.0f;
    scale = 0.5f;
    heading = (D_800D154C->unk7A + 0x4000) * 0.0054931640625f;
    bone = -1;
    allocExt = 0;
    pos = NULL;
    kind = D_800D1580;
    if (kind == 5 || kind == 9 || kind == 10) {
        bone = 3;
    }
    if (bone != -1) {
        if (D_800D154C->unk1D4 != NULL) {
            mtxs = (f32 (*)[16])D_800D154C->unk1D4;
            func_150A7960(mtxs[bone], 0.0f, 0.0f, 1.0f, &x, &y, &z);
            x -= mtxs[bone][12];
            y -= mtxs[bone][13];
            z -= mtxs[bone][14];
            a = func_150484A0(x, z);
            b = func_150484A0(y, sqrtf(x * x + z * z));
            a *= 57.295776f;
            b *= 57.295776f;
            if (a > 180.0f) {
                a -= 360.0f;
            }
            if (b > 180.0f) {
                b -= 360.0f;
            }
            heading = a;
            elevation = b;
        }
    } else if (D_800D154C->camera != NULL && D_800D154C->camera->unk2C == 0x100) {
        elevation = D_800D154C->camera->unk780;
        if (elevation < 0.0f) {
            elevation = elevation * -0.5f + elevation * -0.2333f + 7.0f;
        } else {
            elevation = elevation * -0.5f + (60.0f - elevation) * 0.133f;
        }
        elevation *= 1.40625f;
    } else {
        elevation = -D_800D154C->unk1D1;
    }

    switch (kind) {
    case 2:
    case 11:
    case 12:
    case 18:
        allocExt = 1;
        break;
    case 15:
        elevation *= 2.2f;
        elevation += 7.0f;
        gravity = 3.3f;
        break;
    case 1:
        gravity = 2.4f;
        break;
    case 3:
        dist = 500.0f;
        func_15145974(HIDDEN126(D_800CC2D0[0].unk31C)->unkE8, &heading, &elevation);
        vec[0] = HIDDEN126(D_800CC2D0[0].unk31C)->unkDC[0];
        vec[1] = HIDDEN126(D_800CC2D0[0].unk31C)->unkDC[1];
        vec[2] = HIDDEN126(D_800CC2D0[0].unk31C)->unkDC[2];
        allocExt = 1;
        pos = vec;
        elevation = -elevation;
        break;
    case 6:
        allocExt = 1;
        elevation = 0.0f;
        dist = 500.0f;
        break;
    case 17:
        flags = 0x30;
        elevation = 0.0f;
        dist = 150.0f;
        break;
    case 4:
        dist = 250.0f;
        break;
    case 5:
    case 9:
    case 10:
        allocExt = 1;
        gravity = 5.0f;
        dist = 593.0f;
        scale = 1.5f;
        break;
    }
    func_1505A184((s16)(elevation * 182.04445f), dist, 0.0f, &speed, &yVelocity, &unused);
    yVelocity = -yVelocity;
    func_1506C460(heading, speed, yVelocity, gravity, 100, kind, 0.0f, scale, (s32)pos, flags, allocExt);
}

extern struct127 *func_1505EEF4(s32 arg0);
extern void func_151A3390(void *arg0, u8 arg1);

typedef struct MarkerPoint {
    u8 pad0[8];
    s16 x;
    s16 y;
    s16 z;
} MarkerPoint;

void func_1506D2E8(void) {
    struct127 *obj;
    f32 xOff;
    f32 zOff;
    f32 yOff;
    s32 cmd;
    s32 angle;
    f32 height;

    cmd = D_800D1580 & 0xFF;
    obj = func_1505EEF4(func_15083E0C(cmd));
    if (obj == 0) {
        return;
    }

    height = 0.0f;
    func_151A3390(obj, 0xFF);
    if ((D_800D1580 & 0xFF00) != 0) {
        height = D_800D154C->camera->unk780 - 15.0f;
    }

    angle = D_800D154C->unk7A;
    obj->unk78 = angle;
    obj->unk7A = angle;
    obj->unk76 = angle;
    func_1505A184(angle, 5000.0f, height, &xOff, &zOff, &yOff);

    ((MarkerPoint **)D_800D2104)[obj->unk13F]->x = D_800D154C->x_position + xOff;
    ((MarkerPoint **)D_800D2104)[obj->unk13F]->z = D_800D154C->z_position + zOff;
    ((MarkerPoint **)D_800D2104)[obj->unk13F]->y = D_800D154C->y_position + yOff;
    obj->unk65 = 0;
    obj->unk218 = 0;

    if ((D_800D1580 & 0xFF00) != 0) {
        obj->y_position = D_800D154C->y_position + 70.0f;
        obj->unk232 = 4;
        obj->xz_scale = 0.25f;
        obj->y_scale = 0.25f;
    } else {
        obj->unk232 = 2;
        obj->xz_velocity = -15.0f;
        obj->y_velocity = 34.0f;
        obj->gravity = D_80099D44;
        obj->xz_scale = 0.5f;
        obj->y_scale = 0.5f;
    }
}

void func_1506D4EC(void) {
}

void func_1506D4F4(void) {
    func_1505E650(D_800D154C, D_800D154C->unk84.uh, 0.009999999776482582f, 0.0f, 0.0f, 0.0f, 0);
}

void func_1506D538(void) {
    if (D_800C35EA != 1) {
        func_1507D4F8(D_800C3E78);
    }
}

void func_1506D570(void) {
    D_800D154C->unk6E = 0;
}

extern f32 D_80099D48;
extern void func_1507E7E4(u8 *, u8, u8, u16, s32);
void func_1506D584(void) {
    struct127 *obj = D_800D154C;
    f32 divisor; u16 val; u8 a; u8 b;
    if (((u8 *)obj)[0x70] != 0x2A) {
        a = D_800D1580 >> 24;
        b = (D_800D1580 >> 16) & 0xFF;
        val = D_800D1580 & 0xFFFF;
        divisor = obj->unk2D0->unk10;
        if (divisor == 0.0f) divisor = D_80099D48;
        if (val != 0xFFFF) val = (u32)((f32)val / divisor);
        func_1507E7E4((u8 *)obj, a, b, val, 0);
    }
}
// ???
void func_1506D6B4(void) {
    struct127 *p = D_800D154C;
    s32 sel;
    if (D_80099D4C == p->unk118) { return; }
    if (p->unk118 < (f32) p->unk1A6) { return; }
    if (p->health >= 2) { sel = 0x2C; } else { sel = 0x29; }
    D_800D1580 = (D_800D1580 & 0xFFFF) | (sel = (sel << 10) << 14);
    func_1506D584();
}
void func_1506D74C(void) {
    f32 sp34;
    f32 sp30;
    f32 sp2C;

    if (D_800D154C->in_water == 0) {
        D_800D154C->gravity = 5.0f;
        D_800D154C->y_velocity = 0.0f;
    }
    D_800D154C->y_position += 70.0f;
    ((u8 *)D_800D154C)[0x137] = 0;
    D_800D154C->disable_run = 0;
    func_1505A184(D_800D154C->unk76, (f32)D_800D1580, 0, &sp34, &sp30, &sp2C);
    func_1505E650(D_800D154C, 0xF, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    D_800D154C->x_position += sp34;
    D_800D154C->z_position += sp30;
    D_800D154C->unk100 |= 2;
    D_800D154C->unk28 = 0.0f;
    D_800D154C->unk83 = 0x14;
    D_800D154C->unk31C->unk44 = 0xC;
    D_800D154C->unk31C->unk54 = 0xA;
}

void func_1506D898(void) {
    D_800D154C->y_position -= 80.0f;
    func_1505E650(D_800D154C, 663, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    D_800D154C->unk100 &= 0xFFDF;
    D_800D154C->unk83 = 0;
    D_800D154C->disable_run = 0;
    D_800D154C->unk31C->unk97 = 0;
    D_800D154C->unk31C->unk44 = 12;
}

void func_1506D934(void) {
    D_800D154C->unk103 = D_800D1580;
}

void func_1506D950(void) {
}

void func_1506D958(void) {
    s32 i;
    s32 id = D_800D1580 & 0xFF;
    s32 flag = (D_800D1580 >> 8) & 0xFF;
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state != 0 && id == D_800CC2D0[i].unkAB &&
            D_800C3E78 + 1 == D_800CC2D0[i].unk65) {
            D_800CC2D0[i].unk101 &= ~1;
            D_800CC2D0[i].unk101 |= flag;
            return;
        }
    }
}

void func_1506DA78(void) {
    D_800D154C->immune = D_800D1580;
}

void func_1506DA94(void) {
    if (D_800D1580 != 0) {
        if ((D_800D154C->unk44 > 20.0f) && (D_800D154C->xz_velocity > 20.0f)) {
            D_800D154C->unk83 = (u8)0;
        }
    }
    func_15174690(D_800C3E78, 0, 24, 0, 409, 4, 170, 0xFF, 0);
}

void func_1506DB30(void) {
    D_800D1890 = (s8) D_800D1580;
    func_15076760();
}

void func_1506DB5C(void) {
    D_800D154C->xz_velocity = (f32) D_800D1580;
}

void func_1506DB84(void) {
    D_800D154C->unk239 = (s8) D_800D1580;
}

void func_1506DBA0(void) {
    D_800D154C->unk1E5 = D_800D1580 & 0xff;
    D_800D154C->unk1E6 = (D_800D1580 >> 8) & 0xff;
}

void func_1506DBD4(void) {
    struct127 *temp_v0 = func_15072208(D_800D154C, 0);
    if (temp_v0 != NULL) {
        func_15054A5C(temp_v0, D_800D154C);
    }
}

extern f32 D_80099D50;

void func_1506DC10(void) {
    s32 a0;
    s32 v1;
    if (((D_800D154C->unk118 - 60.0f) < D_800D154C->y_position) || (D_80099D50 == D_800D154C->unk118)) {
        v1 = func_150ADA20() & 3;
        if (v1 >= 2) {
            a0 = v1 + 0x612;
        } else {
            a0 = v1 + 0x8F;
        }
    } else {
        a0 = 9;
    }
    func_15060A9C(a0, D_800D154C);
}

void func_1506DCA4(void) {
    D_800D154C->unk2E8 = D_800D1580;
}

void func_1506DCC0(void) {
    D_800D154C->unk2EC = (s32) D_800D1580;
}

void func_1506DCDC(void) {
    D_800D154C->health = (s8) D_800D1580;
}

void func_1506DCF8(void) {
}

void func_1506DD00(void) {
    struct127 *temp_v1;

    if (D_800D1580 == 0) {
        temp_v1 = D_800D154C;
        temp_v1->unkF8 = (s32) temp_v1->unk144->unk18;
    } else {
        temp_v1 = D_800D154C;
        temp_v1->unkF8 |= D_800D1580;
    }
}

void func_1506DD44(void) {
    D_800D154C->unkF8 &= ~D_800D1580;
}

void func_1506DD6C(void) {
    if (D_800D154C->unk31C != 0) {
        D_800D154C->unk31C->matrix_physics = D_800D1580 & 0x7F;
    }
    if (D_800D1580 == 0) {
        D_800D154C->unk76 = D_800D154C->unk7A;
    }
}

void func_1506DDB8(void) {
}

void func_1506DDC0(void) {
    if (func_15178E50(D_800BE9F0) != 0) {
        func_1516EED4(D_800C3E78, D_800D1580, 0xFF, 0);
    }
}

void func_1506DE04(void) {
    s32 i;
    for(i = 0; i < 2; i++) {
        func_15174690(D_800C3E78, 10, 18, 12, 0x199, 4, 0xAA, 0xFF, 0);
    }
}

// jump table
void func_1516F1C0(s32, s32, u8, s32);
void func_1516F2F8(u8, s32, s32, s32, u8, s32);
void func_151875E0(f32, f32, f32, s32, s32, s32, f32, f32);
void func_150AFDB0(void);
void func_150AFE64(s32);
void func_1506E0EC(void);
void func_1506E2CC(void);

void func_1506DE84(void) {
    switch (D_800D1580) {
    case 0:
        if (*(f32 *)((u8 *)D_800D154C + 0x28) == 0.0f) {
            func_151875E0(*(f32 *)((u8 *)D_800D154C + 0x14),
                          *(f32 *)((u8 *)D_800D154C + 0x18),
                          *(f32 *)((u8 *)D_800D154C + 0x1C),
                          30, 30, 7, 0.1f, 0.1f);
        }
        return;
    case 1:
        if ((u32)(func_150ADA20() & 0xFF) < 0x40U) {
            func_151875E0(*(f32 *)((u8 *)D_800D154C + 0x14),
                          *(f32 *)((u8 *)D_800D154C + 0x18),
                          *(f32 *)((u8 *)D_800D154C + 0x1C),
                          10, 20, 7, 0.03f, 0.02f);
            D_800D1580 = 0x106;
            func_1506BA4C(0x12C, 0x708);
        }
        return;
    case 2:
        func_1516F1C0(D_800C3E78, 0x10, 0xFF, 0);
        return;
    case 3:
        func_1516F1C0(D_800C3E78, 0x28, 0xFF, 0);
        return;
    case 4:
        func_1516F1C0(D_800C3E78, 0x28, 0xFF, 0);
        return;
    case 5:
        func_1516F2F8(D_800C3E78, 30, 20, 2, 0xFF, 0);
        return;
    case 6:
        func_1516F2F8(D_800C3E78, 18, 12, 2, 0xFF, 0);
        return;
    case 7:
        func_1516F2F8(D_800C3E78, 60, 120, 2, 0xFF, 0);
        return;
    case 8:
        func_1506E0EC();
        return;
    case 9:
        func_150AFE64(0);
        return;
    case 10:
        func_150AFE64(1);
        return;
    case 11:
        func_1506E2CC();
        return;
    case 12:
        func_1516F2F8(D_800C3E78, 18, 12, 0, 0xFF, 0);
        return;
    case 13:
        func_150AFDB0();
        break;
    }
}

void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
void func_151700D8(f32 *, f32, f32, s32, f32, s32, s32, s32, s32, s32);
void func_151875E0(f32, f32, f32, s32, s32, s32, f32, f32);

void func_1506E0EC(void) {
    f32 x;
    f32 y;
    f32 z;
    f32 height;
    f32 width;
    f32 depth;
    s32 i;
    f32 vertices[4][3];
    f32 pos_x;
    f32 pos_z;

    if (D_800D154C->unk1D4 != NULL) {
        width = 270.0f;
        depth = width;
        x = width;
        z = depth;
        height = D_800D154C->unk180;
        for (i = 0; i < 4; i++) {
            y = 0.0f;
            func_150A7960((f32 *)D_800D154C->unk1D4, x, 0.0f, z, &x, &y, &z);
            vertices[i][0] = x;
            vertices[i][1] = height;
            vertices[i][2] = z;
            func_151875E0(x, y, z, 0x1E, 0x14, 7, 0.13f, 0.1f);
            if (i == 0) {
                z = depth;
                x = -width;
            } else if (i == 1) {
                x = -width;
                z = -depth;
            } else if (i == 2) {
                x = width;
                z = -depth;
            }
        }
        pos_x = D_800D154C->x_position;
        pos_z = D_800D154C->z_position;
        func_151700D8(vertices[0], pos_x, pos_z, 4, D_800D154C->xz_scale, 0, 4, 4, 0xFF, 0);
    }
}
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
void func_151700D8(f32 *, f32, f32, s32, f32, s32, s32, s32, s32, s32);

void func_1506E2CC(void) {
    f32 x;
    f32 y;
    f32 z;
    f32 s;
    f32 t;
    f32 h;
    s32 i;
    f32 verts[4][3];
    f32 px;
    f32 pz;

    if (D_800D154C->unk1D4 != NULL) {
        s = 100.0f;
        t = 100.0f;
        h = D_800D154C->unk180;
        x = s;
        z = s;
        for (i = 0; i < 4; i++) {
            y = 0.0f;
            func_150A7960((f32 *)D_800D154C->unk1D4, x, y, z, &x, &y, &z);
            verts[i][0] = x;
            verts[i][1] = h;
            verts[i][2] = z;
            if (i == 0) {
                z = s;
                x = -s;
            } else if (i == 1) {
                x = -s;
                z = -t;
            } else if (i == 2) {
                x = s;
                z = -s;
            }
        }
        px = D_800D154C->x_position;
        pz = D_800D154C->z_position;
        func_151700D8((f32 *)verts, px, pz, 5, D_800D154C->xz_scale * 0.5f, 0, 4, 4, 0xFF, 0);
    }
}
s32 func_151F2CDC(void);
extern s32 *D_800BE5A8[];
s32 func_1506E46C(struct127 *arg0, s32 *arg1, s32 arg2) {
    volatile s32 pad0;
    u32 cmd;
    s32 pad[3];
    u32 v1;
    u32 v0;
    u32 hi;

    cmd = *arg1;
    v1 = *arg1 & 0x7FFF;
    if (cmd & 0x200000) {
        switch (arg2) {
        case 0:
            if (func_100107F8(arg0) != 0) {
                return 0;
            }
            break;
        case 1:
            if (func_10010894(arg0) != 0) {
                return 0;
            }
            break;
        case 2:
            if (func_151F2CDC() == 1) {
                return 0;
            }
            break;
        }
    }
    if (v1 >= 2000) {
        v0 = D_800BE5A8[((u8 *)arg0)[0x288]][v1 - 2000];
        if (v0 == 0) {
            return 0;
        }
        cmd &= 0xFF200000;
        if (cmd != 0) {
            cmd |= v0 & 0xFFFFFF;
        } else {
            cmd |= v0;
        }
    }
    v1 = cmd >> 16;
    if (v1 != 0) {
    lab:
        if ((cmd >> 24) >= (func_150ADA20() & 0xFF)) {
            v1 &= 0x1F;
            cmd &= 0x7FFF;
            if (v1) {
                s32 pad;
                cmd = func_1000F568(cmd, v1 | 0);
            }
        } else {
            return 0;
        }
    }
    *arg1 = cmd;
    return 1;
}

void func_1506E5FC(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(300, 1800);
    }
}

void func_1506E63C(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(100, 800);
    }
}

void func_1506E67C(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(190, 1300);
    }
}

void func_1506E6BC(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(300, 3000);
    }
}

void func_1506E6FC(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(700, 4000);
    }
}

void func_1506E73C(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(3000, 8000);
    }
}

void func_1506E77C(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BBA8(300, 1800);
    }
}

void func_1506E7BC(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BBA8(300, 3000);
    }
}

void func_1506E7FC(void) {
    if (D_800D154C->unk28 == 0.0f) {
        if (D_800D154C->unk107 == 0) {
            func_1506E5FC();
        }
    }
}

void func_1506E848(void) {
    if ((func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) && (func_1000F4D8((u16)D_800D1582) == 0)) {
        func_1506BA4C(0x50, 800);
    }
}

void func_1506E898(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 2) != 0) {
        func_1506BB64(300, 1800);
    }
}

void func_1506E8D8(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(300, 1800);
    }
}

void func_1506E918(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(100, 800);
    }
}

void func_1506E958(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(190, 1300);
    }
}

void func_1506E998(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(300, 3000);
    }
}

void func_1506E9D8(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(700, 4000);
    }
}

void func_1506EA18(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(3000, 8000);
    }
}

void func_1506EA58(void) {
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BC24(300, 3000);
    }
}

void func_1506EA98(void) {
    u8 temp_t6;
    struct127 *temp_a1;

    temp_t6 = (D_800D154C->unk13C - 100);
    if (D_800D154C->unk13C != 0) {
        temp_a1 = &D_800CC2D0[temp_t6];
        if (D_800CC2D0[temp_t6].unk13D >= 100) {
            D_800CC2D0[temp_t6].unk13D = (u8)0U;
            D_800CC2D0[temp_t6].xz_velocity = 18.0f;
            D_800CC2D0[temp_t6].y_velocity = 40.0f;
            D_800CC2D0[temp_t6].gravity = 6.0f;
            D_800CC2D0[temp_t6].unk76 = (u16) D_800D154C->unk7A;
            D_800CC2D0[temp_t6].unk65 = (u8)0;
            if (D_800CC2D0[temp_t6].id  == 0x20) {
                D_800CC2D0[temp_t6].unkF8 |= 0x200;
                D_800CC2D0[temp_t6].y_velocity = 0.0f;
                D_800CC2D0[temp_t6].gravity = 0.0f;
                D_800CC2D0[temp_t6].xz_velocity = 0.0f;
            }
            D_800CC2D0[temp_t6].stunned = (u8)0xFE;
            D_800CC2D0[temp_t6].unk105 = (u8)0;
            D_800CC2D0[temp_t6].unk106 = func_1505E7CC(0xC, temp_a1); //, temp_t6);
            D_800CC2D0[temp_t6].unk84.uh = (u16)0xFFFF;
            D_800CC2D0[temp_t6].unk1CC = (f32) D_80099DA0;
            func_1505E874(temp_t6, temp_a1);
            D_800CC2D0[temp_t6].unk25C |= 0x40;
        }
        D_800D154C->unk13C = (u8)0;
    }
}

void func_1506EBC0(void) {
    u8 temp_v0 = (u8)(D_800D154C->unk13C - 100);

    if (D_800D154C->unk13C) {
        if ((s32) D_800CC2D0[temp_v0].unk13D >= 0x64) {
            D_800CC2D0[temp_v0].unk65 = 0;
            D_800CC2D0[temp_v0].unk13D = (u8)0U;
            D_800CC2D0[temp_v0].xz_velocity = 0.0f;
            D_800CC2D0[temp_v0].y_velocity = 0.0f;
            D_800CC2D0[temp_v0].gravity = 6.0f;
            D_800CC2D0[temp_v0].unk1CC = D_80099DA4; // 1000000.0
        }
        D_800D154C->unk13C = 0U;
    }
}

void func_1506EC50(void) {
    if (D_80099DA8 < D_800D154C->unk28) {
        if (((D_800D1580 & 0x8000) == 0) || (D_800CC2E8[0] < D_800D154C->y_position)) {
            D_800D1878 = D_800D1580 & 0xFF;
            D_800D1880 = 0;
            D_800D154C->unk138 -= 1;
        }
    }
}

void func_1506ECD0(void) {
    D_800D154C->unk2D0->unk4 |= 0x8000;
}

void func_1506ECF0(void) {
    D_800D154C->unk3A = (s8) D_800D1580;
}

void func_1506ED0C(void) {
    D_800D154C->unk83 = (s8) D_800D1580;
    D_800D154C->unk100 |= 0x10;
    D_800D154C->unk31C->unk31 = (u8)1;
}

void func_1506ED4C(void) {
    D_800D154C->unk100 = (s8) D_800D1580;
}

void func_1506ED68(void) {
    D_800D154C->unkA9 = (u8)5;
    D_800D154C->unk31C->unk31 = (u8)0;
}

void func_1506ED90(void) {
    D_800D154C->unk21C = (s16) D_800D1580;
}

void func_1506EDAC(void) {
    D_800D154C->unkF4 = (s32) D_800D1580;
}

void func_1506EDC8(void) {
    D_800D154C->unkF4 &= ~D_800D1580;
}

void func_1506EDF0(void) {
    D_800D154C->unkF4 |= D_800D1580;
}

void func_1506EE14(void) {
    D_800D154C->unk25C |= D_800D1580;
}

void func_1506EE38(void) {
    D_800D154C->unk25C &= ~D_800D1580;
}

void func_1506EE60(void) {
    s32 temp_v0 = D_800D1580;
    s32 temp_a1 = D_800D1580 & 0xFFFF;

    if (temp_v0 != 0) {
        func_15188810(D_800D154C, temp_a1, D_800D1580 >> 0x10);
        return;
    }
    func_15188A9C(D_800D154C, temp_a1);
}

void func_1506EEAC(void) {
    func_151898C0(D_800D154C, D_800D1580);
}

void func_1506EED8(void) {
    D_800D154C->unk24E = (s8) D_800D1580;
}

void func_1506EEF4(void) {
    D_800D154C->unk276 = (D_800D1580 >> 16) & 0xFFFF;
    D_800D154C->unk278 = (D_800D1580 >> 24) & 0xFF;
    D_800D154C->unk282 = D_800D1580 & 0xFFFF;
    D_800D154C->unk284 = 0;
    D_800D154C->unk285 = 0;
    D_800D154C->unk286 = 0;
    D_800D154C->unk287 = 0;
}

void func_1506EF5C(void) {
    s32 index;
    s16 first;
    s16 second;

    index = (D_800D1580 >> 16) & 0xFF;
    first = (D_800D1580 >> 8) & 0xFF;
    second = D_800D1580 & 0xFF;
    *(u16 *)((u8 *)D_800D154C + 0x282) = 0xFFFF;
    ((u8 *)D_800D154C)[0x276] = 5;
    index *= 2;
    ((u8 *)D_800D154C)[index + 0x284] = first;
    ((u8 *)D_800D154C)[index + 0x285] = second;
}

void func_1506EFB4(void) {
    D_800D154C->unk282 = (u16)0;
}

void func_1506EFC8(s32 arg0) {
    if (D_800D154C->id != 0x8C) {
        func_150BB760(D_800D154C);
    }
}

void func_1506F004(s32 arg0) {
    func_150BCBBC(D_800D154C);
}

void func_1506F02C(s32 arg0) {
    func_150BA4C0(D_800D154C, 0xFF, 0);
}

void func_1506F05C(s32 arg0) {
    func_151925C4(D_800D154C, 0x32, 0xFF, 1);
}

void func_1506F090(s32 arg0) {
    func_151925C4(D_800D154C, -1, 0xFF, 1);
}

void func_1506F0C4(s32 arg0) {
    func_150C1260(D_800D154C, 0);
}

void func_1506F0F0(s32 arg0) {
    func_150C1260(D_800D154C, 1);
}

void func_1506F11C(s32 arg0) {
    func_150BABE0(D_800D154C, 0, 0xFF);
}

void func_1506F14C(s32 arg0) {
    func_150BABE0(D_800D154C, 1, 0xFF);
}

void func_1506F17C(s32 arg0) {
    func_150AEEB0(D_800D154C, 0xFF);
}

void func_1506F1A8(s32 arg0) {
    struct func_1506F1A8_stack sp1C;
    struct127 *obj;

    obj = D_800D154C;
    sp1C.unk04 = obj;
    sp1C.unk08 = obj->unique_id;
    sp1C.unk14 = 80.0f;
    sp1C.unk20 = 102.0f;
    sp1C.unk24 = D_80099DAC;
    sp1C.unk28 = D_80099DB0;
    sp1C.unk2C = 1200.0f;
    sp1C.unk40 = D_80099DB4;
    sp1C.unk30 = 26.0f;
    sp1C.unk34 = D_80099DB8;
    sp1C.unk44 = -187.0f;
    sp1C.unk48 = D_80099DBC;
    sp1C.unk58 = D_80099DC0;
    sp1C.unk5C = D_80099DC4;
    sp1C.unk64 = D_80099DC8;
    sp1C.unk6C = D_80099DCC;
    sp1C.unk68 = D_80099DD0;
    sp1C.unk70 = D_80099DD4;
    sp1C.unk74 = D_80099DD8;
    sp1C.unk78 = D_80099DDC;
    sp1C.unk7C = D_80099DE0;
    sp1C.unk80 = D_80099DE4;
    sp1C.unk84 = D_80099DE8;
    sp1C.unk88 = D_80099DEC;
    sp1C.unk90 = D_80099DF0;
    sp1C.unk98 = D_80099DF4;
    sp1C.unkA8 = D_80099DF8;
    sp1C.unkAC = D_80099DFC;
    sp1C.unkBC = 14.0f;
    sp1C.unkC0 = 9.0f;
    sp1C.unkCC = 25.0f;
    sp1C.unkD0 = D_80099E00;
    sp1C.unkD4 = D_80099E04;
    sp1C.unkEC = D_80099E08;
    sp1C.unk09 = 0xC;
    sp1C.unk3E = 0xC8;
    sp1C.unkF0 = D_80099E0C;
    sp1C.unk38 = -1;
    sp1C.unk39 = 2;
    sp1C.unk3A = -1;
    sp1C.unk4C = -1;
    sp1C.unk00 = 0xF;
    sp1C.unk54 = 1;
    sp1C.unk50 = 0x3C;
    sp1C.unk51 = 0xFF;
    sp1C.unk52 = 0xEB;
    sp1C.unk53 = 0x52;
    sp1C.unkFC = 402.0f;
    sp1C.unk60 = 0x1E;
    sp1C.unk62 = 0xF;
    sp1C.unk8C = 0xC;
    sp1C.unk8E = 0x15;
    sp1C.unk94 = 0x50;
    sp1C.unk95 = 0x78;
    sp1C.unkB0 = 4;
    sp1C.unkB4 = 2;
    sp1C.unkB8 = 0x1E;
    sp1C.unkBA = 0x11;
    sp1C.unk0C = 0.0f;
    sp1C.unk10 = 0.0f;
    sp1C.unk18 = 0.0f;
    sp1C.unk1C = 0.0f;
    sp1C.unk3B = 0;
    sp1C.unk3C = 0;
    sp1C.unk3D = 0;
    sp1C.unk9C = 0.0f;
    sp1C.unkA0 = 5.0f;
    sp1C.unkA4 = 100.0f;
    sp1C.unkC4 = 0.5f;
    sp1C.unkC8 = 0.5f;
    sp1C.unkD8 = 0x50;
    sp1C.unkDA = 0x78;
    sp1C.unkDC = 0x10;
    sp1C.unkDE = 0xF;
    sp1C.unkE0 = 0.0f;
    sp1C.unkE4 = 5.0f;
    sp1C.unkE8 = 100.0f;
    sp1C.unkF4 = 0x28;
    sp1C.unkF6 = 0xF;
    sp1C.unkF8 = 0x50;
    sp1C.unkFA = 0x50;
    sp1C.unk118 = 0x14;
    sp1C.unk11A = 0xC;
    sp1C.unk100 = D_80099E10;
    sp1C.unk104 = D_80099E14;
    sp1C.unk108 = D_80099E18;
    sp1C.unk10C = 45.0f;
    sp1C.unk110 = D_80099E1C;
    sp1C.unk114 = D_80099E20;
    func_151994B8(0, &sp1C, 0xFF, 0);
}

void func_1506F524(s32 arg0) {
    func_15197A7C(D_800D154C);
}

void func_1506F54C(s32 arg0) {
    struct func_1506F1A8_stack descriptor;

    descriptor.unk04 = D_800D154C;
    descriptor.unk08 = D_800D154C->unique_id;
    descriptor.unk09 = 13;
    descriptor.unk0C = 22.0f;
    descriptor.unk10 = 22.0f;
    descriptor.unk14 = 165.0f;
    descriptor.unk18 = 22.0f;
    descriptor.unk1C = 22.0f;
    descriptor.unk20 = 193.0f;
    descriptor.unk24 = 1.13800001f;
    descriptor.unk28 = 0.795000017f;
    descriptor.unk2C = 370.0f;
    descriptor.unk40 = 0.61500001f;
    descriptor.unk30 = 17.8000011f;
    descriptor.unk34 = 4.9000001f;
    descriptor.unk3E = 159;
    descriptor.unk38 = 0;
    descriptor.unk39 = -1;
    descriptor.unk3A = 0;
    descriptor.unk3B = 0;
    descriptor.unk3C = 0;
    descriptor.unk3D = 0;
    descriptor.unk44 = -191.0f;
    descriptor.unk48 = 7.8499999f;
    descriptor.unk4C = -1;
    descriptor.unk00 = 15;
    descriptor.unk54 = 1;
    descriptor.unk50 = 60;
    descriptor.unk51 = 255;
    descriptor.unk52 = 235;
    descriptor.unk53 = 82;
    descriptor.unk58 = 0.00289999996f;
    descriptor.unk5C = 0.00234999997f;
    descriptor.unk60 = 35;
    descriptor.unk62 = 10;
    descriptor.unk64 = 0.802000046f;
    descriptor.unk6C = 0.367000014f;
    descriptor.unk68 = 0.589000046f;
    descriptor.unk70 = 0.307000011f;
    descriptor.unk74 = 0.614000022f;
    descriptor.unk78 = 0.299000025f;
    descriptor.unk7C = 0.0f;
    descriptor.unk80 = 0.738000035f;
    descriptor.unk84 = 0.962646008f;
    descriptor.unk88 = 0.961867988f;
    descriptor.unk8C = 15;
    descriptor.unk8E = 17;
    descriptor.unk90 = 0.369000018f;
    descriptor.unk94 = 80;
    descriptor.unk95 = 80;
    descriptor.unk98 = 0.0240000002f;
    descriptor.unk9C = 22.0f;
    descriptor.unkA0 = 58.0f;
    descriptor.unkA4 = 212.0f;
    descriptor.unkA8 = 0.200000003f;
    descriptor.unkAC = 0.105000004f;
    descriptor.unkB0 = 3;
    descriptor.unkB4 = 2;
    descriptor.unkB8 = 25;
    descriptor.unkBA = 10;
    descriptor.unkBC = 6.0f;
    descriptor.unkC0 = 10.0f;
    descriptor.unkC4 = 0.300000012f;
    descriptor.unkC8 = 0.200000003f;
    descriptor.unkCC = 37.0f;
    descriptor.unkD0 = 0.751000047f;
    descriptor.unkD4 = 0.322000027f;
    descriptor.unkD8 = 80;
    descriptor.unkDA = 80;
    descriptor.unkDC = 15;
    descriptor.unkDE = 17;
    descriptor.unkE0 = 22.0f;
    descriptor.unkE4 = 130.0f;
    descriptor.unkE8 = 306.0f;
    descriptor.unkEC = 0.208000004f;
    descriptor.unkF0 = 0.193000004f;
    descriptor.unkF4 = 35;
    descriptor.unkF6 = 15;
    descriptor.unkF8 = 80;
    descriptor.unkFA = 100;
    descriptor.unkFC = 198.0f;
    descriptor.unk100 = 102.0f;
    descriptor.unk104 = 0.0400000028f;
    descriptor.unk108 = 0.0400000028f;
    descriptor.unk10C = 89.0f;
    descriptor.unk110 = -0.495000035f;
    descriptor.unk114 = 0.443000019f;
    descriptor.unk118 = 15;
    descriptor.unk11A = 17;
    func_151994B8(0, &descriptor, 0xFF, 0);
}

void func_1506F8C0(s32 arg0) {
    func_1519EF70(D_800D154C, 0, 0);
}

void func_1506F8F0(s32 arg0) {
    struct127* a0;
    func_150E2EA4(a0 = D_800D154C, a0->unique_id, 0x14, -1, 482.0f, -127.0f, -45.0f, D_80099E98, -211.0f, -114.0f, 3, 3, 5, 20.0f, (func_150ADA68() * 10.0f) + 40.0f, 0, 0.0f);
}

void func_1506F9C0(s32 arg0) {
    struct127* a0;
    func_150E2EA4(a0 = D_800D154C, a0->unique_id, 0x14, -1, 241.0f, -127.0f, -45.0f, 418.0f, -211.0f, -114.0f, 3, 3, 5, 20.0f, (func_150ADA68() * 10.0f) + 40.0f, 0, 0.0f);
}

void func_1506FA90(s32 arg0) {
    struct127* a0;
    func_150E2EA4(a0 = D_800D154C, a0->unique_id, 0x1A, -1, -181.0f, -218.0f, -1.0f, D_80099E9C, -584.0f, -2.0f, 3, 3, 5, 20.0f, (func_150ADA68() * 10.0f) + 40.0f, 0, 0.0f);
}

void func_1506FB60(s32 arg0) {
    if (func_151044F4() != 0) {
        func_151C6A28(D_800D154C, (arg0 - 0x13) & 0xFF, 0xFF, 0);
    } else {
        func_151C62D0(D_800D154C, (arg0 - 0x13) & 0xFF, 0, 0, -1, 0xFF, 0);
    }
}

void func_1506FBE8(s32 arg0) {
    func_151A0A10(D_800D154C, 0x46, 0xFF, 0);
}

void func_1506FC1C(s32 arg0) {
    func_151A0A10(D_800D154C, 0x28, 0xFF, 0);
}

void func_1506FC50(s32 arg0) {
    func_1519E688();
}

void func_1506FC74(s32 arg0) {
    func_1519E6BC(D_800D154C);
}

void func_1506FC9C(s32 arg0) {
    func_150B3AB0(D_800D154C, 0xFF);
}

void func_1506FCC8(s32 arg0) {
    func_15196438(D_800D154C, 6, 0xFF, 0);
}

void func_1506FCFC(s32 arg0) {
    func_15196438(D_800D154C, 5, 0xFF, 0);
}

extern f32 D_80099EA0;

void func_1506FD30(s32 arg0) {
    struct127* a0;
    func_150E2EA4(a0 = D_800D154C, a0->unique_id, 1, -1, 0.0f, 0, D_80099EA0, 0.0f, 0, 414.0f, 3, 3, 5, 20.0f, (func_150ADA68() * 10.0f) + 40.0f, 1, 50.0f);
}

void func_1506FDF0(s32 arg0) {
    func_151AABC4(D_800D154C, 0);
}

void func_1506FE1C(s32 arg0) {
    func_151AABC4(D_800D154C, 1);
}

void func_1506FE48(s32 arg0) {
    func_151AABC4(D_800D154C, 0);
}

void func_1506FE74(s32 arg0) {
    func_151AABC4(D_800D154C, 1);
}

void func_1506FEA0(s32 arg0) {
    func_151AABC4(D_800D154C, 2);
}

void func_1506FECC(s32 arg0) {
    func_151AABC4(D_800D154C, 3);
}

void func_1506FEF8(s32 arg0) {
    func_151AB920(D_800D154C, 0);
}

void func_1506FF24(s32 arg0) {
    func_151AB920(D_800D154C, 1);
}

void func_1506FF50(s32 arg0) {
    func_151AB930(D_800D154C);
}

void func_1506FF78(s32 arg0) {
    func_150CBF80(D_800D154C, 0, 1, 0xFF);
}

void func_1506FFAC(s32 arg0) {
    func_150CBF80(D_800D154C, 1, 1, 0xFF);
}

void func_1506FFE0(s32 arg0) {
    func_150CBF80(D_800D154C, 2, 1, 0xFF);
}

void func_15070014(s32 arg0) {
    func_150CA150(D_800D154C);
}

void func_1507003C(s32 arg0) {
    func_151B01B8(D_800D154C, 0);
    func_151B09BC(D_800D154C, 0, 0x3E8, 0xFF, 0);
}

void func_15070084(s32 arg0) {
    func_151AECA0(D_800D154C, 0xFF, 1);
}

void func_150700B4(s32 arg0) {
    func_151B03B8(D_800D154C, 0xFF, 1);
}

void func_150700E4(s32 arg0) {
    func_15193660(D_800D154C, 0xFF, 1);
}

void func_15070114(s32 arg0) {
    func_151937F4(D_800D154C, 0xFF, 1);
}

void func_15070144(s32 arg0) {
    struct198 tmp;
    s32 tmp0;

    tmp.unk0 = 1;
    tmp.unk8 = 0;
    tmp.unkC = 0;
    tmp.unk4 = D_800D154C;

    if (D_800BE9F0 == 0x2B) {
        tmp0 = 3;
    } else {
        tmp0 = 0;
    }
    func_151C0698(D_800D154C, 0, &tmp, tmp0, 0xFF, 1);
    func_151C1FB8(D_800D154C);
}

void func_150701C4(s32 arg0) {
    func_151C5280(D_800D154C, 0xFF, 1);
}

void func_150701F4(s32 arg0) {
    func_151C9740(D_800D154C, 0xFF, 1);
}

void func_15070224(s32 arg0) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 5U) + 4;
    header.unk4 = 0;

    header2.unk0 = (s32) D_800D154C->x_position;
    header2.unk4 = (s32) D_800D154C->y_position;
    header2.unk8 = (s32) D_800D154C->z_position;

    func_151602C0(&header, &header2, (func_150ADA20() % 3U) + 4, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 1);
}

extern s8 D_800E0BCE[];
extern Vec3w_15070D24 D_80099B70;
extern Vec3w_15070D24 D_80099B7C;
extern s32 func_15145EA4(void *arg0, void *arg1, s32 arg2, s32 arg3);
extern struct127 *func_1507515C(struct127 *);
extern void func_151C1860(void *, s32, s32);

typedef struct {
    s32 unk0;
    struct127 *unk4;
    struct127 *unk8;
    struct127 *unkC;
} Info_15070300;

void func_15070300(s32 arg0) {
    u8 flag;
    struct127 *obj;
    u8 mode;
    struct127 *other;
    u8 idx;
    Info_15070300 info;
    Vec3w_15070D24 sp80;
    Vec3w_15070D24 sp74;
    f32 sp6C[2];
    f32 sp60[3];
    s32 *srcs[1];
    f32 *dsts[1];

    other = NULL;
    if (D_800D154C->id == 0x77) {
        mode = 1;
        obj = D_800D154C;
        idx = D_800D154C - D_800CC2D0;
        if ((idx < 2) && (D_800E0BCE[idx] != 0)) {
            flag = 1;
            other = &D_800CC2D0[D_800E0BCE[idx]];
            goto done;
        }
        flag = 0;
        if (((u8 *)obj->unk31C)[0x197] != 0) {
            flag = 1;
        } else {
            flag = 0;
        }
    done:;
    } else if (D_800D154C->id == 0x28) {
        obj = func_1507515C(D_800D154C);
        mode = 0;
        flag = 0;
        if (((u8 *)obj->unk31C)[0x197] != 0) {
            flag = 1;
        } else {
            flag = 0;
        }
    } else {
        return;
    }
    func_151C1FB8(obj);
    if (other != NULL) {
        func_151C1FB8(other);
    }
    if (D_800BE616 != 0) {
        if (D_800D154C->unk31C->unk128 & 0x10) {
            D_800D1580 = 10;
            mode = 2;
            (*(u16 *)((u8 *)D_800D154C->unk31C + 0x12C))--;
        } else {
            D_800D1580 = 9;
        }
    } else {
        D_800D1580 = 5;
    }
    if (flag) {
        info.unk0 = 3;
        info.unk4 = obj;
        info.unk8 = other;
        info.unkC = D_800D154C;
        func_151C0698(obj, other, &info, mode, 0xFF, 1);
    } else {
        func_1506CE6C();
    }
    if (D_800D154C->unk1D4 == NULL) {
        return;
    }
    if ((D_800D154C->unk74 & 0xF) == 0xF) {
        return;
    }
    sp80 = D_80099B70;
    sp74 = D_80099B7C;
    sp6C[0] = (func_150ADA68() * 30.0f) + 40.0f;
    sp6C[1] = (func_150ADA68() * 2.512000084f) + 3.590000153f;
    srcs[0] = (s32 *)&sp80;
    dsts[0] = sp60;
    func_15145EA4(srcs, dsts, (s32)D_800D154C->unk1D4 + 0x100, 1);
    func_15102B38((s32)D_800D154C, 4, (s32)&sp80, (s32)&sp74, sp6C,
                  ((func_150ADA20() + 0U) % 5U) + 5, ((func_150ADA20() + 0U) % 0x38U) + 0xC8,
                  (func_150ADA68() * 510.0f) + 1296.0f,
                  (s32)sp60, 0xFF, 0, -1, 0xFF, 1);
    func_151C1860(sp60, 0xFF, 1);
}

void func_15070690(s32 arg0) {
    func_150EEE00(D_800D154C, (arg0 - 0x3E) & 0xFF, arg0);
}

void func_150706C4(s32 arg0) {
    func_150EEF40(D_800D154C, (arg0 - 0x3E) & 0xFF, arg0);
}

void func_150706F8(s32 arg0) {
    if ((D_800D154C->unk94 & 0x10) == 0) {
        func_150F03F8(D_800D154C, 0, 0xFF, 1);
    }
    if ((D_800D154C->unk94 & 8) == 0) {
        func_150F03F8(D_800D154C, 1, 0xFF, 1);
    }
}

void func_15070760(s32 arg0) {
    func_150EBEC0(D_800D154C, 0, 0xFF, 1);
}

void func_15070794(s32 arg0) {
    func_150EBEC0(D_800D154C, 1, 0xFF, 1);
}

void func_150707C8(s32 arg0) {
    func_150FDDA0(D_800D154C, 0xFF, 1);
}

void func_150707F8(s32 arg0) {
    func_150FDF38(D_800D154C, 0xFF, 1, 0, 0);
}

void func_15070830(s32 arg0) {
    func_150FE320(D_800D154C, 0xFF, 1);
}

void func_15070860(s32 arg0) {
    func_150FE49C(D_800D154C, 0xFF, 1, 0, 0);
}

void func_15070898(s32 arg0) {
    struct func_1506F1A8_stack descriptor;

    descriptor.unk04 = D_800D154C;
    descriptor.unk08 = D_800D154C->unique_id;
    descriptor.unk09 = 9;
    descriptor.unk0C = -4.73400116f;
    descriptor.unk10 = -111.957596f;
    descriptor.unk14 = 33.4733963f;
    descriptor.unk18 = -8.83799744f;
    descriptor.unk1C = -159.75f;
    descriptor.unk20 = 33.4733963f;
    descriptor.unk24 = 0.277999997f;
    descriptor.unk28 = 0.828000069f;
    descriptor.unk2C = 312.0f;
    descriptor.unk40 = 0.324999988f;
    descriptor.unk30 = 7.0999999f;
    descriptor.unk34 = 5.0f;
    descriptor.unk3E = 0xB4;
    descriptor.unk38 = -1;
    descriptor.unk39 = 1;
    descriptor.unk3A = -1;
    descriptor.unk3B = -1;
    descriptor.unk3C = -1;
    descriptor.unk3D = -1;
    descriptor.unk44 = -232.0f;
    descriptor.unk48 = 9.06999969f;
    descriptor.unk4C = -1;
    descriptor.unk00 = 0xF;
    descriptor.unk54 = 0x11;
    descriptor.unk50 = 0x3C;
    descriptor.unk51 = 0xFF;
    descriptor.unk52 = 0xEB;
    descriptor.unk53 = 0x52;
    descriptor.unk58 = 0.00688999984f;
    descriptor.unk5C = 0.0103799999f;
    descriptor.unk60 = 0x19;
    descriptor.unk62 = 0xA;
    descriptor.unk64 = 0.367000014f;
    descriptor.unk6C = 0.256000012f;
    descriptor.unk68 = 0.350000024f;
    descriptor.unk70 = 0.205000013f;
    descriptor.unk74 = 0.776000023f;
    descriptor.unk78 = 0.811000049f;
    descriptor.unk7C = 0.125f;
    descriptor.unk80 = 1.31100011f;
    descriptor.unk84 = 0.935118973f;
    descriptor.unk88 = 0.943452001f;
    descriptor.unk8C = 0xC;
    descriptor.unk8E = 0x15;
    descriptor.unk90 = 0.307000011f;
    descriptor.unk94 = 0x50;
    descriptor.unk95 = 0x50;
    descriptor.unk98 = 0.00200000009f;
    descriptor.unk9C = -8.83799744f;
    descriptor.unkA0 = -159.75f;
    descriptor.unkA4 = 33.4733963f;
    descriptor.unkA8 = 0.300000012f;
    descriptor.unkAC = 0.401000023f;
    descriptor.unkB0 = 4;
    descriptor.unkB4 = 2;
    descriptor.unkB8 = 0x1A;
    descriptor.unkBA = 0x10;
    descriptor.unkBC = 6.0f;
    descriptor.unkC0 = 5;
    descriptor.unkC4 = 0.300000012f;
    descriptor.unkC8 = 0.300000012f;
    descriptor.unkCC = 50.0f;
    descriptor.unkD0 = 0.0f;
    descriptor.unkD4 = 0.712000012f;
    descriptor.unkD8 = 0x50;
    descriptor.unkDA = 0x78;
    descriptor.unkDC = 0xF;
    descriptor.unkDE = 0x11;
    descriptor.unkE0 = -8.83799744f;
    descriptor.unkE4 = -159.75f;
    descriptor.unkE8 = 33.4733963f;
    descriptor.unkEC = 0.204000011f;
    descriptor.unkF0 = 0.504000008f;
    descriptor.unkF4 = 0x1E;
    descriptor.unkF6 = 0xF;
    descriptor.unkF8 = 0x50;
    descriptor.unkFA = 0x50;
    descriptor.unkFC = 140.0f;
    descriptor.unk100 = 182.0f;
    descriptor.unk104 = 0.160000011f;
    descriptor.unk108 = 0.113000005f;
    descriptor.unk10C = 42.0f;
    descriptor.unk110 = -0.518000007f;
    descriptor.unk114 = 0.376000017f;
    descriptor.unk118 = 0xA;
    descriptor.unk11A = 0x19;
    func_151994B8(0, &descriptor, 0xFF, 0);
}

void func_15070C18(s32 arg0) {
    func_15199834(D_800D154C);
}

void func_15103E40(struct127 *, struct127 *, struct17 *, u8, u8, u8, u8);
void func_15070C40(u8 arg0) {
    struct17 tmp;
    struct127 *temp_a1;
    struct127 *owner;

    temp_a1 = D_800D154C;
    tmp.unk0 = temp_a1->x_position;
    tmp.unk4 = temp_a1->y_position;
    tmp.unk8 = temp_a1->z_position;

    owner = temp_a1->unk124 ? &D_800CC2D0[temp_a1->unk124 - 1] : temp_a1;
    func_15103E40(owner, temp_a1, &tmp, arg0, 0, 0xFF, 0);
}

void func_15070CDC(s32 arg0) {
    func_15070C40(1);
}

void func_15070D00(s32 arg0) {
    func_15070C40(0);
}

void func_15070D24(s32 arg0) {
    Stack_15070D24 sp4C;

    sp4C.sp84 = D_80099B88;
    sp4C.sp78 = D_80099B94;

    if (D_800D154C->unk1D4 == 0) {
        return;
    }

    if ((D_800D154C->unk74 & 0xF) == 0xF) {
        return;
    }

    sp4C.sp70[0] = (func_150ADA68() * 101.0f) + 151.0f;
    sp4C.sp70[1] = (func_150ADA68() * 4.0f) + 4.0f;

    func_15143134(&sp4C.sp84, sp4C.sp64, (s32)D_800D154C->unk1D4 + 0x40);

    func_15102B38((s32)D_800D154C, 1, (s32)&sp4C.sp84, (s32)&sp4C.sp78, sp4C.sp70,
                  ((func_150ADA20() + 0U) % 6U) + 5, ((func_150ADA20() + 0U) % 0x9CU) + 0x64,
                  (func_150ADA68() * 2496.0f) + D_80099F2C,
                  (s32)sp4C.sp64, 0xFF, 0, -1, 0xFF, 0);

    sp4C.header.unk0 = 3;
    sp4C.header.unk1 = -1;
    sp4C.header.unk2 = (func_150ADA20() % 6U) + 0xA;
    sp4C.header.unk4 = 0;

    sp4C.header2.unk0 = (s32)sp4C.sp64[0];
    sp4C.header2.unk4 = (s32)sp4C.sp64[1];
    sp4C.header2.unk8 = (s32)sp4C.sp64[2];

    func_151602C0(&sp4C.header, &sp4C.header2, (func_150ADA20() % 0xC9U) + 0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0);
}
extern Vec3w_15070D24 D_80099BA0;
extern Vec3w_15070D24 D_80099BAC;
extern void func_151D5334(struct17*, f32, f32, f32, s32, s32, s32);
void func_15070F60(s32 arg0) {
    Stack_15070D24 sp58;

    sp58.sp84 = D_80099BA0;
    sp58.sp78 = D_80099BAC;

    if (D_800D154C->unk1D4 == 0) {
        return;
    }

    if ((D_800D154C->unk74 & 0xF) == 0xF) {
        return;
    }

    sp58.sp70[0] = (func_150ADA68() * 180.0f) + 150.0f;
    sp58.sp70[1] = func_150ADA68() * 2.0f + 2.5f;

    func_15143134(&sp58.sp84, sp58.sp64, ((u8)arg0 ? 1 : 2) * 0x40 + (s32)D_800D154C->unk1D4);

    func_15102B38((s32)D_800D154C, (*((u8 *)&arg0 + 3)) ? 1 : 2, (s32)&sp58.sp84, (s32)&sp58.sp78, sp58.sp70,
                  ((func_150ADA20() + 0U) % 5U) + 5, ((func_150ADA20() + 0U) % 0x9CU) + 0x64,
                  (func_150ADA68() * 4000.0f) + 4000.0f,
                  (s32)sp58.sp64, 0xFF, 0, -1, 0xFF, 0);

    sp58.header.unk0 = 3;
    sp58.header.unk1 = -1;
    sp58.header.unk2 = (func_150ADA20() % 5U) + 3;
    sp58.header.unk4 = 0;

    sp58.header2.unk0 = (s32)sp58.sp64[0];
    sp58.header2.unk4 = (s32)sp58.sp64[1];
    sp58.header2.unk8 = (s32)sp58.sp64[2];

    func_151602C0(&sp58.header, &sp58.header2, (func_150ADA20() % 0xC9U) + 0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0);
    func_151D5404((struct17 *)sp58.sp64, 506.0f, 1013.0f, 0.0009871668880805373f, 0xF, 0x14, 0xFF, 0);
    func_151D5334((struct17 *)sp58.sp64, 506.0f, 1013.0f, 0.0009871668880805373f, 5, 0xFF, 0);
}
void func_15071230(s32 arg0) {
    func_15070F60(0);
}

void func_15071254(s32 arg0) {
    func_15070F60(1);
}

void func_15071278(s32 arg0) {
    func_150FC438(D_800D154C, 0, 1, D_800D154C->unk84.ub[1]);
}

extern void func_150FE860(struct127 *arg0, s32 arg1, s32 arg2);
extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);

struct sp18_150712AC {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 unk7;
};

void func_150712AC(s32 arg0) {
    struct sp18_150712AC sp18;

    func_150FE860(D_800D154C, 0xFF, 1);
    if (D_800D154C->camera != 0) {
        sp18.unk0 = 1;
        sp18.unk2 = (func_150ADA20() % 7U) + 0xA;
        sp18.unk5 = 1 << D_800D154C->camera->unk23D;
        sp18.unk4 = (func_150ADA20() % 7U) + 2;
        sp18.unk6 = -1;
        func_151D8868(&sp18, 0, 0xFF, 1);
    }
}

extern void func_150FF084(struct127 *arg0, s32 arg1, s32 arg2);
extern void func_150FED30(struct127 *arg0, s32 arg1, s32 arg2);

void func_15071360(s32 arg0) {
    struct sp18_150712AC sp18;

    if (func_151044F4() != 0) {
        func_150FF084(D_800D154C, 0xFF, 1);
    } else {
        func_150FED30(D_800D154C, 0xFF, 1);
    }
    if (D_800D154C->camera != 0) {
        sp18.unk0 = 1;
        sp18.unk2 = (func_150ADA20() % 5U) + 0xF;
        sp18.unk5 = 1 << D_800D154C->camera->unk23D;
        sp18.unk4 = (func_150ADA20() % 4U) + 3;
        sp18.unk6 = -1;
        func_151D8868(&sp18, 0, 0xFF, 1);
    }
}

void func_15071434(s32 arg0) {
    func_150FF840(D_800D154C, 1, 0, 0xFF, 1);
}

void func_15071470(s32 arg0) {
    func_150FF840(D_800D154C, 0, 0, 0xFF, 1);
}

void func_150714AC(s32 arg0) {
    func_150FF840(D_800D154C, 0, 1, 0xFF, 1);
}

void func_150714E8(s32 arg0) {
    func_151D5714(D_800D154C, &D_800A2148, &D_800A2154, D_80088BB0, 1.0f, 0xFF, 1);
}

void func_15071544(s32 arg0) {
    func_151D4668(D_800D154C);
    func_151D469C(D_800D154C, 4, 0x78, 0xFF, 1);
}

void func_1507158C(s32 arg0) {
    func_151D4668(D_800D154C);
    func_151D469C(D_800D154C, 2, 0x78, 0xFF, 1);
}

void func_150715D4(s32 arg0) {
    func_151D4668(D_800D154C);
    func_151D469C(D_800D154C, 3, 0x78, 0xFF, 1);
}

void func_1507161C(s32 arg0) {

}

void func_15071628(s32 arg0) {
    func_151D0058(D_800D154C, (arg0 - 0x55) & 0xFF, 0xFF, 1);
}

void func_15071668(s32 arg0) {
    func_151D0024(D_800D154C);
}

void func_15071690(s32 arg0) {
    struct127 *temp_a0;
    s32 pad0;
    s32 sp1C;

    temp_a0 = D_800D154C;
    if ((temp_a0->unk1D4 != 0) && ((temp_a0->unk74 & 0xF) != 0xF)) {
        func_150B60E0(temp_a0, &sp1C);
        func_150B5C38(&sp1C, 0xFF, 1);
    }
}

void func_150716EC(s32 arg0) {
    struct17 tmp;
    struct127 *temp_v0;

    temp_v0 = D_800D154C;
    tmp.unk0 = temp_v0->x_position;
    tmp.unk4 = temp_v0->y_position;
    tmp.unk8 = temp_v0->z_position;
    func_151D5404(&tmp, 1502.0f, 3000.0f, 0.00033333332976326346f, 0xC, 0xF, 0xFF, 0);
}

void func_15071764(s32 arg0) {
    struct127 *sp34 = func_15083E90(18);
    if (sp34 != NULL) {
        func_150F0BEC(sp34);
        func_150F10D4(sp34);
        func_15161E24(sp34, 1, 2, 300, 70, 255, 130, 0, 255, 1);
    }
}

void func_150717E0(s32 arg0) {
    struct {
        struct127 *unk0;
        u8 unk4;
    } sp20;
    struct127 *temp_v0;

    temp_v0 = func_15083E90(0x12);
    if (temp_v0 != 0) {
        sp20.unk0 = temp_v0;
        sp20.unk4 = temp_v0->unique_id;
        func_15131D4C((s32)&sp20, 0x43);
        func_151494E0((s32)&sp20, 0x43);
    }
}

void func_15071830(s32 arg0) {
    func_150F9BB0(D_800D154C, 0xFF, 1);
}

void func_15071860(s32 arg0) {
    func_1519072C(D_800D154C);
}

void func_15071888(s32 arg0) {
    func_151D5714(D_800D154C, &D_800A1FB0, &D_800A1FBC, D_80088B90, D_80099F30, 0xFF, 1);
}

extern u32 D_80099BB8;
void func_15143134(void *, void *, s32);
void func_151C329C(s32 arg0, s32 arg1, s32 arg2);

void func_150718E4(s32 arg0) {
    f32 *p;
    f32 sp24[2];
    u32 sp20;

    sp20 = D_80099BB8;
    if ((D_800D154C->unk1D4 != 0) || ((D_800D154C->unk74 & 0xF) == 0xF)) {
        p = sp24;
        func_15143134(&D_800A5480, p,
                      (((s32)((u8 *)&sp20)[func_150ADA20() & 3]) << 6) + (s32)D_800D154C->unk1D4);
        sp24[1] = D_800D154C->unk180 + 20.0f;
        func_151C329C((s32)p, 0xFF, 1);
    }
}

void func_15071998(s32 arg0) {
    func_150FA520(D_800D154C, 0, 0xFF, 1);
}

void func_150719CC(s32 arg0) {
    func_150FA520(D_800D154C, 1, 0xFF, 1);
}

void func_15071A00(s32 arg0) {
    func_150FA520(D_800D154C, 2, 0xFF, 1);
}

void func_15071A34(s32 arg0) {
    func_151D09A8(D_800D154C, 0xFF, 1);
}

void func_15071A64(s32 arg0) {
    struct199 tmp;

    if (!(func_150ADA20() & 1) && (D_800D154C->unk1D4 != 0) && ((D_800D154C->unk74 & 0xF) != 0xF) &&
        (D_800CC2D0->stunned != 0) && ((s32)D_800CC2D0->health > 0)) {
        func_1504715C(&tmp, D_800D154C);
        func_15143134(&D_80099BBC, &tmp.unk24, (s32)D_800D154C->unk1D4 + 0x3C0);
        func_151DC484(&tmp.unk24, &tmp, 0, 0xFF, 1);
    }
}
typedef struct Game981E0EffectPoint {
    f32 values[3];
} Game981E0EffectPoint;

typedef struct Game981E0SmokeConfig {
    s32 kind;
    s32 count;
    Game981E0EffectPoint position;
    s16 zero14;
    s16 alpha;
    s16 minimum;
    s16 maximum;
    f32 size0;
    f32 size1;
    f32 range0;
    f32 range1;
    s16 lifetime;
    s16 rate;
    f32 value0;
    f32 value1;
    f32 value2;
} Game981E0SmokeConfig;

void func_151DC260(f32 *, void *, s32, s32, s32);
void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
extern f32 D_80099BC8[3][3];
extern u8 D_80099BEC[];
extern u8 D_80099C04[];
extern f32 D_80099F34;
extern f32 D_80099F38;
extern f32 D_80099F3C;
extern f32 D_80099F40;
extern f32 D_80099F44;

void func_15071B18(s32 arg0) {
    Game981E0EffectPoint point;
    u8 collision[0x24];

    if ((*(s32 *)((u8 *)D_800D154C + 0x1D4) != 0) &&
        ((((u8 *)D_800D154C)[0x74] & 0xF) != 0xF)) {
        func_15143134(D_80099BC8[func_150ADA20() % 3U], point.values,
                      *(s32 *)((u8 *)D_800D154C + 0x1D4) + 0x1C0);
        func_1504715C(collision, D_800D154C);
        if (func_150ADA20() & 1) {
            if (func_150ADA20() & 1) {
                func_151DC260(point.values, collision, 0, 0xFF, 1);
            } else {
                func_151DC484(point.values, (struct199 *)collision, 0, 0xFF, 1);
            }
        }
        if (func_150ADA20() & 1) {
            Game981E0SmokeConfig config;

            config.kind = 8;
            config.count = 5;
            config.position = point;
            config.zero14 = 0;
            config.alpha = 0xFF;
            config.minimum = -0x40;
            config.maximum = 0x3C;
            config.size0 = 10.0f;
            config.size1 = 10.0f;
            config.lifetime = 0x19;
            config.rate = 0x14;
            config.range0 = D_80099F34;
            config.range1 = D_80099F38;
            config.value0 = D_80099F3C;
            config.value1 = D_80099F40;
            config.value2 = D_80099F44;
            func_15152190(&config, D_80099BEC, D_80099C04,
                          6, 25.0f, 0, 0xFF, 1);
        }
    }
}

void func_15071D08(s32 arg0) {
    func_150F2230(D_800D154C, 0xFF, 1);
}

void func_15071D38(void) {
    void (*func)(s32);

    func = D_80086150[D_800D1580];
    if (func != 0) {
        func(D_800D1580);
    }
}

void func_15071D78(void) {
    struct127 *tmp;
    struct127 *orig;

    tmp = func_150721E8(D_800D154C);
    if (tmp != NULL) {
        orig = D_800D154C;
        D_800D154C = tmp;
        func_15071D38();
        D_800D154C = orig;
    }
}

void func_15071DC8(void) {
    func_15141A7C(D_800D154C, D_800D1580);
}

void func_15071DF4(void) {
    func_15192800(D_800D154C, D_800D1580);
}

void func_15071E20(void) {
    D_800D154C->unk247 = (s8) D_800D1580;
}

void func_15071E3C(void) {
    D_800D154C->unk248 = (s8) D_800D1580;
}

void func_15071E58(void) {
    D_800D1878 = D_800D1580 & 0xFF;
    D_800D154C->unk244 = (D_800D1580 >> 8) & 0xFF;
    func_1505E650(D_800D154C, D_800D154C->unk244, 1.2000000476837158f, 3.0f, D_800D1878, 0.0f, 0);
}

void func_15071ED4(void) {
    func_1505E650(D_800D154C, 0x59, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_15071F14(void) {
    func_1505E650(D_800D154C, 0x24, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_15071F54(void) {
    func_1507C8E0(D_800D154C, D_800D1580);
}

void func_15071F80(void) {
    if (D_800C35EA == 1) {
        D_800D154C->unk138++;
    }
}

void func_15071FB0(void) {
    if (D_800D154C->unk28 == 0.0f) {
        D_800D154C->unk10C = (u16)0;
    }
}

extern f32 D_80099F48;
extern void *D_800CC5EC;

void func_15071FDC(void) {
    u8 *base;
    u8 *entry;
    s32 found;
    s32 mode;
    s32 visible;
    s32 i;

    visible = 1;
    mode = 0x194;
    base = (u8 *)&D_800CC2D0;
    if (D_800D1580 == 0) {
        found = -1;
        entry = base;
        for (i = 0; i < 0x19; i++) {
            if (*(u8 *)(entry + 0x65) == (D_800C3E78 + 1)) {
                found = i;
                break;
            }
            entry += 0x32C;
        }
        if ((found + 1) != *(u8 *)(base + 0x274)) {
            return;
        }
        *(s16 *)(base + 0x76) = *(u16 *)((u8 *)D_800D154C + 0x76) + 0x8000;
        *(f32 *)(base + 0x1CC) = D_80099F48;
    } else {
        if (*(s32 *)((u8 *)D_800D154C + 0x2E8) >= 0x71) {
            mode = 0x2E5;
            *(f32 *)((u8 *)D_800D154C + 0x2E0) = 0.0f;
        } else {
            mode = 0x32;
            *(s16 *)(base + 0x76) = *(u16 *)((u8 *)D_800D154C + 0x76);
            if (*(u8 *)((u8 *)D_800D154C + 0x251) != 0) {
                *(f32 *)(base + 0x1CC) = D_800CC2E8[0] + 800.0f;
            }
            visible = 0;
        }
        *(s32 *)((u8 *)D_800D154C + 0x2E4) = 0x32;
    }
    *(f32 *)((u8 *)base + 0x20) = *(f32 *)((u8 *)D_800D154C + 0x2D8);
    *(f32 *)((u8 *)base + 0x24) = *(f32 *)((u8 *)D_800D154C + 0x2DC);
    *(f32 *)((u8 *)base + 0x3C) = *(f32 *)((u8 *)D_800D154C + 0x2E0);
    *((u8 *)base + 0x81) = 1;
    func_1505E650((struct127 *)base, (u16)mode, 1.0f, 2.0f, 6.0f, 0.0f, 0);
    *(s8 *)((u8 *)D_800CC5EC + 0x31) = visible;
    if (*(u8 **)((u8 *)base + 0x318) != 0) {
        *(s32 *)(*(u8 **)((u8 *)base + 0x318) + 0x5F0) |= 0x400;
    }
}

void func_150721A4(void) {
    u16 lo = D_800D1580;
    u16 mid = D_800D1580 >> 8;
    u16 hi = D_800D1580 >> 16;

    func_1506160C(D_800D154C, (u8)hi, lo, mid, 0);
}

struct127 *func_150721E8(struct127 *arg0) {
    return func_15072208(arg0, 0);
}

struct127 *func_15072208(struct127 *arg0, s32 arg1) {
    f32 t;
    s32 idx;
    s32 count;
    s32 i;

    idx = (arg0 - D_800CC2D0) + 1;
    count = 0;
    for (i = 0; i < 25; i++) {
        if (idx == D_800CC2D0[i].unk65) {
            if (count == arg1) {
                return &D_800CC2D0[i];
            }
            count++;
        }
    }
    return NULL;
}


void func_150722F0(void) {
    struct127 *temp_v0 = func_150721E8(D_800D154C);
    if (temp_v0 != 0) {
        s32 tmp0 = D_800D1580 >> 8;
        u16 tmp1 = D_800D1580;
        func_1506160C(temp_v0, 2, tmp0, tmp1, 0);
    }
}

void func_1507233C(void) {
    D_800D154C->unk94 = (s32) ~(D_800D1580 | 1);
}

void func_15072360(void) {
    D_800D154C->unk94 &= ~D_800D1580;
}

void func_15072388(void) {
    D_800D154C->unk94 |= D_800D1580;
}

void func_150723AC(void) {
    struct127 *temp_v0 = func_15083E90(D_800D1583);
    if (temp_v0 != NULL) {
        func_15060F28(temp_v0, 0);
    }
}

void func_150723E0(void) {
    struct127 *temp_v0 = func_15083E90(D_800D1583);
    if ((temp_v0 != 0) && (temp_v0->unk65 != 0)) {
        func_15060F28(temp_v0, 0);
    }
}

extern u8 *D_800D210C;
extern void func_1505F188(struct127 *arg0);
extern s32 func_15082A44(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

typedef struct SpawnSlot {
    u8  pad0[2];
    u8  unk2;
    u8  pad3[0x2D];
} SpawnSlot;

void func_15072420(void) {
    struct127 *obj;
    s32 spawnFlags;
    s32 angle;
    s32 slot;
    s32 id;
    s32 noKill;

    angle = (D_800D1580 >> 8) & 0x7F;
    noKill = (D_800D1580 >> 15) & 1;
    spawnFlags = (D_800D1580 >> 16) & 0xFF;
    id = func_15083E0C(D_800D1580);
    if (id == -1) {
        return;
    }
    D_800D210C[id] = 0;
    if (!(spawnFlags & 0x80)) {
        obj = func_1505EEF4(id);
        if (obj != 0) {
            func_15060F28(obj, 0);
            func_1505F188(obj);
        }
    }
    ((SpawnSlot *)D_800D20FC)[id].unk2 = 0;
    slot = func_15082A44((SpawnSlot *)((id * 0x30) + (s32)D_800D20FC), id, 0, 0, 0);
    if (slot == 0) {
        return;
    }
    obj = &D_800CC2D0[slot - 1];
    if (obj == NULL) {
        return;
    }
    obj->unk65 = (D_800D154C - D_800CC2D0) + 1;
    obj->unk5C = angle;
    if (noKill) {
        obj->unk101 = 0;
    } else {
        obj->unk101 = 4;
    }
    obj->unk232 = spawnFlags & 0x7F;
    obj->unk124 = D_800C3E78 + 1;
    obj->x_position = D_800D154C->x_position;
    obj->y_position = D_800D154C->y_position;
    obj->z_position = D_800D154C->z_position;
    if (D_800BE616 != 0) {
        if (D_800D154C->id == 0x53) {
            D_800D154C->unk222 = obj - D_800CC2D0;
        }
    }
    if ((D_800BE9F0 == 0x3F) && (obj->id == 0x16)) {
        obj->unk94 |= 2;
    }
}

void func_1507266C(void) {
    u16 packed;
    s32 angle;
    struct127 *obj;

    if (D_800BE616 == 0) {
        if (D_800D154C->unk222 == 0) {
            return;
        }
    }
    packed = D_800D1580;
    angle = D_800D1580 >> 16;
    obj = &D_800CC2D0[D_800D154C->unk222];
    obj->unk65 = D_800C3E78 + 1;
    obj->unk5C = angle;
    obj->unk101 = 4;
    obj->stunned = 0;
    obj->immune = 0xFF;
    obj->unk218 = 0;
    obj->unkE4 = 0;
    obj->unkE6 = 0;
    obj->unk232 = packed;
    obj->unkEC = 1.0f;
    obj->unkF0 = 1.0f;
    obj->unkDC = 1.0f;
    obj->unkE0 = 1.0f;
    func_151B5BA0(&D_800CC2D0[D_800C3E78], obj, angle);
}

void func_15072740(void) {
    s32 temp_v0;
    struct127 *temp_v1;

    temp_v0 = D_800D1580 >> 16;
    temp_v1 = &D_800CC2D0[D_800D154C->unk222];
    temp_v1->unk65 = D_800C3E78 + 1;
    temp_v1->unk5C = temp_v0;
    temp_v1->unk101 = 4;
}

void func_150727AC(void) {
    D_800D154C->animation_speed = D_800D1580 * D_80099F4C;
    D_800D154C->unk2D0->unk10 = D_800D154C->animation_speed;
}

void func_150727F0(void) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state != 0 && D_800C3E78 + 1 == D_800CC2D0[i].unk65) {
            D_800CC2D0[i].unk218 = 0;
            D_800CC2D0[i].unk232 = D_800D1580;
        }
    }
}

void func_15072918(void) {
    func_15060F28(D_800D154C, 0);
}

void func_15072940(void) {
    func_15060F28(D_800D154C, 1);
}

void func_15072968(void) {
    func_1505E650(D_800D154C, 44, 1.5f, 12.0f, 0.0f, 0.0f, 0);
    D_800D154C->disable_run = 25;
}

void func_150729B4(void) {
    D_800D154C->unk2E4 = D_800D1580;
}

void func_150729D0(void) {
    D_800D154C->unk31C->unk8 = (u16)1;
    func_1507EB4C(D_800D154C, 21);
    func_1506B82C();
}

void func_15072A14(void) {
    D_800D154C->unk31C->unk66 = (u16)0;
    func_1507F640();
}

void func_15072A40(void) {
    D_800D154C->unk31C->unk19B = (u8)0;
    D_800D154C->unk31C->unk78 = (u8)0;
    func_1507F640();
}

void func_15072A7C(void) {
    if (D_800D154C->unk31C != 0) {
        D_800D154C->unk31C->unk1A9++;
        if ((func_150ADA20() & 7) < (u32) D_800D154C->unk31C->unk1A9) {
            func_1505E650(D_800D154C, 394, 1.0f, 4.0f, 0.0f, 0.0f, 0);
        }
    }
}

void func_15072AF8(void) {
    func_1505E650(D_800D154C, D_800D154C->unk84.uh + 1, 1.0f, 6.0f, 0.0f, 0.0f, 0);
}

void func_15072B44(void) {
    s32 state;
    s32 anim = -1;

    state = D_800D154C->unk84.uh;
    switch (state) {
    case 0x26F:
    case 0x270:
    case 0x271:
    case 0x272:
    case 0x273:
        anim = state + 1;
        break;
    case 0x274:
        anim = 0x26F;
        break;
    case 0x1D6:
        anim = 0x1D7;
        break;
    case 0x1D7:
        anim = 0x1D8;
        break;
    case 0x175:
        anim = 0x177;
        break;
    case 0x176:
        anim = 0x177;
        break;
    case 0x35:
        anim = 0x3A;
        break;
    case 0x3F:
        anim = 0x3A;
        break;
    case 0x1CD:
        anim = 0x1CE;
        break;
    case 0x1CF:
        anim = 0x1CE;
        break;
    case 0xFD:
        anim = 0xFE;
        break;
    case 0x10A:
        anim = 0x10A;
        break;
    case 0x12C:
        anim = 0x12D;
        break;
    case 0x142:
        anim = 0x12D;
        break;
    case 0x138:
        anim = 0x3E;
        break;
    case 0x10C:
        if (*((u8 *)D_800D154C->unk31C + 0x197)) {
            anim = 0x10A;
        } else {
            func_1507F640();
        }
        break;
    case 0x156:
        if (*((u8 *)D_800D154C->unk31C + 0x197)) {
            anim = 0x157;
        } else {
            func_1507F640();
        }
        break;
    case 0x167:
        if (*((u8 *)D_800D154C->unk31C + 0x197)) {
            anim = 0x165;
        } else {
            func_1507F640();
        }
        break;
    default:
        if ((state == 0xBE) ||
            ((state == 0xC1) && !(*(s32 *)((u8 *)D_800D154C->unk31C + 0x38) & 0x2000))) {
            anim = 0xBF;
        }
        break;
    }
    if (anim != -1) {
        func_1505E650(D_800D154C, anim, 1.0f, 4.0f, 0.0f, 0.0f, 0);
    }
}

/* The original TU defined the state-handler table D_8008630C (now asm/data/22AC10.rodata.s)
 * holding this function's address. That data reference makes as1 treat the entry as an
 * indirect-jump target, which changes its delay-slot filling (the jump-table index
 * subtractions are hoisted into earlier delay slots). Game .c.o(.rodata) is discarded at
 * link, so this only restores the assembler's view. */
static void (*const D_8008630C_entry[])(void) = { func_15072B44 };
void func_15072DA0(void) {
    D_800D154C->unk2F8 &= 0xFFF8;
    D_800D154C->unk2F8 |= D_800D1580;
}

void func_15072DD8(void) {
    func_15083568(D_800D154C, D_800D1580, 1.0f, 0);

    if ((D_800D154C->unk31C != 0) && (D_800D154C->unk31C->unk11A == 1)) {
        D_800D154C->unk31C->unk11A = (u8)2U;
    }
}

void func_15072E38(void) {
    func_150836CC(D_800D154C, D_800D1580);

    if (D_800D154C->unk31C != 0) {
        D_800D154C->unk31C->unk11A = (u8)0;
    }
}

void func_15072E7C(void) {
    D_800D154C->unk10C = (s16) D_800D1580;
}

void func_15072E98(void) {
    D_800D154C->unk2D0->unk10 *= D_800D1874;
}

void func_15072EC0(void) {
    struct197 *tmp;

    tmp = D_800D154C->unk2D0;
    if (tmp->unk10 < D_800D1874) {
        D_800D154C->unk10C = 0;
    }
}

void func_15072EF4(void) {
    D_800D154C->stunned = (s8) D_800D1580;
}

void func_15072F10(void) {
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    s16 temp;

    temp = (s16)(D_800D154C->unk1D1 * 200);
    *(s16 *)((u8 *)D_800D154C->unk31C + 0x24) -= 1;
    func_1505A184((u16)(D_800D154C->unk7A - D_800D154C->unk31C->unk12), 2000.0f, (f32)temp * 0.0054931640625f, &sp4C, &sp48, &sp44);
    sp4C = sp4C * D_8009A094 + D_800D154C->x_position;
    sp48 = sp48 * D_8009A094 + D_800D154C->z_position;
    sp44 = sp44 * D_8009A094 + D_800D154C->y_position;
    func_150E1570(D_800D154C, D_800D154C->x_position, D_800D154C->y_position + (f32)D_800D154C->unkD6, D_800D154C->z_position, sp4C, sp44, sp48, D_800C3E78, 0x12);
}

void func_1507304C(void) {
}

void func_15073054(void) {
    D_800D154C->unk22E = (s16) D_800D1580;
}

void func_15073070(void) {

}

void func_15073078(void) {
    func_1512D748(0, D_800D1580, 1);
}

void func_150730A4(void) {
    D_800D154C->unk1FF = (u8)3;
    D_800D154C->unk200 = (s8) D_800D1580;
}

void func_150730D0(void) {
    struct127 *temp_v0;

    if (D_800D1580 == 1) {
        temp_v0 = D_800D154C;
        temp_v0->unk2F8 |= 0x100;
    } else {
        temp_v0 = D_800D154C;
        temp_v0->unk2F8 &= 0xFEFF;
      }
}


typedef struct {
    u8 pad_0[0x300];
    s32 field_0x300;
    s32 field_0x304[4];
    s32 *field_0x314;
} Game981E0ActorSlots;

u8 *func_15073118(u8 *arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, s32 argA) {
    Mtx *mtx;
    f32 mf[4][4];
    struct127 *obj;
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    obj = func_1505ED34();
    obj->gravity = arg9;
    obj->y_velocity = arg8;
    obj->xz_velocity = arg7;
    obj->unk83 = argA;
    if ((((struct127 *)arg0)->unk1D4 != 0) && (arg1 != -1)) {
        mtx = (Mtx *)((struct127 *)arg0)->unk1D4 + arg1;
        if (D_800C3E90 != 0) {
            guMtxL2F(mf, mtx);
            mtx = (Mtx *)mf;
        }
        func_150A7960((f32 *)mtx, arg3, arg4, arg5, &x, &y, &z);
    } else {
        x = ((struct127 *)arg0)->x_position;
        y = ((struct127 *)arg0)->y_position;
        z = ((struct127 *)arg0)->z_position;
    }
    obj->x_position = x;
    obj->y_position = y;
    obj->z_position = z;
    obj->unk76 = obj->unk7A = (f32)((struct127 *)arg0)->unk7A + arg6 * 182.0444489f;
    obj->interaction_state = 0x21;
    obj->immune = 0xFF;
    obj->unkF8 = 2;
    if (arg2 != 0x42) {
        obj->unkF8 = 0x100E;
    }
    obj->pad66 = ((struct127 *)arg0)->pad66 & 0xC;
    obj->unkE4 = 10;
    obj->unkE6 = 10;
    obj->unkEC = 1.0f;
    obj->unkF0 = 1.0f;
    obj->unkDC = 1.0f;
    obj->unkE0 = 1.0f;
    obj->xz_scale = 0.5f;
    obj->y_scale = 0.5f;
    if (arg2 == 0x1F) {
        *(f32 *)&obj->padC8 = 10.0f;
    }
    obj->unk278 = arg2;
    obj->unk5 = 5;
    obj->unique_id = 255U - (obj - D_800CC2D0);
    func_15083568(obj, arg2, 1.0f, 0);
    for (i = 0; i <= D_80082FA0; i++) {
        ((Game981E0ActorSlots *)obj)->field_0x304[i] = func_1515D480(1);
    }
    if ((&((Game981E0ActorSlots *)obj)->field_0x300)[i] != 0) {
        obj->unk301[0] = 1;
    }
    ((Game981E0ActorSlots *)obj)->field_0x314 = func_1515D440();
    func_1514EC1C(func_15158BD0(obj, 1, 0), obj, 0x13);
    return (u8 *)obj;
}
s32 func_15037698(s32, s32, s32, f32, f32, f32, f32 *, f32, s32, s32);
u8 *func_15073118(u8 *, s32, s32, f32, f32, f32, f32, f32, f32, f32, s32);
extern f32 D_8009A09C;
extern f32 D_8009A0A0;
extern f32 D_8009A0A4;
extern f32 D_8009A0A8;
extern f32 D_8009A0AC;
extern f32 D_8009A0B0;
extern f32 D_8009A0B4;
extern f32 D_8009A0B8;
extern f32 D_8009A0BC;
extern f32 D_8009A0C0;
extern f32 D_8009A0C4;
extern f32 D_8009A0C8;
extern f32 D_8009A0CC;
extern f32 D_8009A0D0;
extern f32 D_8009A0D4;

void func_1507342C(void) {
    u8 *obj;
    s32 rot;
    f32 y;
    f32 x;
    f32 unk84[7];

    if (D_800D1580 == 0) {
        f32 dist;
        u8 *target;
        s32 hit;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 angle;
        f32 unk48;

        hit = 0;
        target = *(u8 **)((u8 *)D_800D154C + 0x318);
        if (func_15037698(((u8 *)D_800D154C - (u8 *)&D_800CC2D0) / 0x32C, 0x17, 0x17,
                          *(f32 *)(target + 0x2F8), *(f32 *)(target + 0x2FC), *(f32 *)(target + 0x300),
                          &dist, 30.0f, 0, 0) != 0 &&
            (u32)func_150ADA20() % 1000 < 200) {
            hit = 1;
        }
        obj = func_15073118((u8 *)D_800D154C, 0x17, 0x1F, 0.0f, 0.0f, 16.0f, 0.0f, D_8009A09C, D_8009A0A0, D_8009A0A4, 100);
        *(s32 *)(obj + 0xF8) |= 0x01000000;
        if (hit != 0) {
            dx = *(f32 *)(target + 0x2F8) - *(f32 *)(obj + 0x14);
            dy = *(f32 *)(target + 0x2FC) - *(f32 *)(obj + 0x18);
            dz = *(f32 *)(target + 0x300) - *(f32 *)(obj + 0x1C);
            angle = func_150484A0(dx, dz) * D_8009A0A8;
            unk48 = angle;
            if (angle > 180.0f) {
                unk48 = angle - 360.0f;
            }
            angle = *(f32 *)(obj + 0x3C) * dy / sqrtf(dx * dx + dz * dz);
            *(f32 *)(obj + 0x20) = angle;
            *(f32 *)(obj + 0x24) = 0.0f;
            *(s32 *)(obj + 0x2E4) = 1;
            rot = (s16)(unk48 * D_8009A0AC) - 0x4000;
            *(s16 *)(obj + 0x7A) = rot;
            *(s16 *)(obj + 0x76) = rot;
        } else {
            *(s32 *)(obj + 0x2E4) = 0;
        }
    } else if (D_800D1580 == 1) {
        f32 t;
        f32 spread;

        spread = *(s32 *)((u8 *)D_800D154C + 0x2E4) * D_8009A0B0;
        t = (s32)((u32)func_150ADA20() % 40 - 20) * spread;
        y = D_8009A0B4 + t;
        t = (s32)((u32)func_150ADA20() % 40 - 20) * spread;
        x = D_8009A0B8 + t;
        func_15073118((u8 *)D_800D154C, 0xE, 0x42, 0.0f, 0.0f, 16.0f, (s32)((u32)func_150ADA20() % 40 - 20) * spread, x, y, D_8009A0BC, 100);
    } else if (D_800D1580 == 2) {
        obj = func_15073118((u8 *)D_800D154C, 9, 0x35, 0.0f, D_8009A0C0, 0.0f, 180.0f, D_8009A0C4, D_8009A0C8, 3.5f, 100);
        *(f32 *)(obj + 0xB8) = 180.0f;
        *(s32 *)(obj + 0xF8) |= 0x01000000;
    } else if (D_800D1580 == 3) {
        f32 t;

        t = (s32)((u32)func_150ADA20() % 40 - 20) * 10.0f;
        y = D_8009A0CC + t;
        t = (s32)((u32)func_150ADA20() % 40 - 20) * 10.0f;
        x = D_8009A0D0 + t;
        func_15073118((u8 *)D_800D154C, 0xE, 0x88, 0.0f, 0.0f, 16.0f, (s32)((u32)func_150ADA20() % 40 - 20) * 10.0f, x, y, D_8009A0D4, 100);
    }
}

void func_150738E8(void) {
    u8 temp_v0 = (u8)(D_800D154C->unk13C - 100);

    if (D_800D154C->unk13C != 0) {
        D_800D154C->unk13C = 0U;
        D_800CC2D0[temp_v0].unkF8 = 0x8200;
        D_800CC2D0[temp_v0].unk13D = 0;
        D_800CC2D0[temp_v0].unk76 = D_800D154C->unk7A;
        D_800CC2D0[temp_v0].unk65 = 0;
        D_800CC2D0[temp_v0].unk232 = 6;
        D_800CC2D0[temp_v0].unk218 = 0;
        D_800CC2D0[temp_v0].stunned = 0;
        D_800CC2D0[temp_v0].unk7A = 0xE000;
        func_1505E650(D_800D154C, 0x97, 1.0f, 5.0f, 0.0f, 0.0f, 0);
    }
}

void func_150739A4(void) {
    D_800D154C->unk64 = (s8) D_800D1580;
}

void func_150739C0(void) {
    D_800D154C->unk64 -= D_800BE9A0;

    if (D_800D154C->unk64 >= 0) {
        D_800D1880 = 0;
        D_800D154C->unk138--;
        D_800D1878 = D_800D1580;
    }
}

void func_15073A28(void) {
    D_800D154C->unk44 = (f32) D_800D1580;
}

typedef struct Game981E0ActorRecord {
    u8 pad0[0x65];
    u8 field_65;
    u8 pad66[0x10];
    s16 field_76;
    u8 pad78[0xAD];
    u8 field_125;
    u8 pad126[0xA6];
    f32 field_1CC;
    u8 pad1D0[0x48];
    s32 field_218;
    u8 pad21C[0x16];
    u8 field_232;
    u8 pad233[0x41];
    u8 field_274;
    u8 pad275[0xB7];
} Game981E0ActorRecord;
s32 func_1505D024(void *, s32, u16, s32);
extern f32 D_8009A0D8;

void func_15073A50(void) {
    u8 *parent;
    s32 flags;
    s32 index;

    parent = (u8 *)D_800D154C;
    index = parent[0x124];
    if (((Game981E0ActorRecord *)&D_800CC2D0 + index)->field_65 != 0) {
        flags = D_800D1580;
        parent[0x13C] = 0;
        ((Game981E0ActorRecord *)&D_800CC2D0 + index)->field_125 = 0;
        func_1505D024((Game981E0ActorRecord *)&D_800CC2D0 + index, D_800D1580 & 0xFF00FF, 0, D_800C3E78);
        ((Game981E0ActorRecord *)&D_800CC2D0 + index)->field_1CC = D_8009A0D8;
        if (flags & 0x40000000) {
            ((Game981E0ActorRecord *)&D_800CC2D0 + index)->field_1CC = *(f32 *)((u8 *)D_800D154C + 0x18);
        }
        ((Game981E0ActorRecord *)&D_800CC2D0 + index)->field_125 = 0x14;
        ((Game981E0ActorRecord *)&D_800CC2D0 + index)->field_76 = *(u16 *)((u8 *)D_800D154C + 0x7A) + flags;
    }
}

void func_15073B38(void) {
    struct func_15073B38_inner {
        u8 padA8[0xA8];
        f32 unkA8;
        u8 unkAC;
        s8 unkAD;
    };
    s32 *temp_v0;
    struct127 **temp_v1;
    f32 temp_f0;
    f32 temp_f2;

    temp_v0 = &D_800D1580;
    temp_v1 = &D_800D154C;
    temp_f0 = (f32)(*temp_v0 & 0xFFFF);
    ((struct func_15073B38_inner *)(*temp_v1)->unk31C)->unkA8 = temp_f0;
    temp_f2 = (f32)((*temp_v0 >> 16) & 0xFF);
    ((struct func_15073B38_inner *)(*temp_v1)->unk31C)->unkAC = (u32)temp_f2;
    ((struct func_15073B38_inner *)(*temp_v1)->unk31C)->unkAD = (*temp_v0 >> 24) & 0xFF;
}

void func_15073C28(void) {
    func_1507F640();
}

void func_15073C48(void) {

}

void func_15073C50(void) {
    D_800D154C->unk224 = (s32)(*(f32 *)((u8 *)D_800CC2E8 + (D_800D154C->unk222 * 0x32C)) + (f32)D_800D1580);
}

void func_15073CB8(void) {
    struct127 *tmp = func_1505F0AC(0x53);
    if ((tmp != 0) && ((u16)tmp->unk244 == 0x1F)) {
        tmp->unk21C = (u16)0;
    }
}

void func_15073CF4(void) {
    func_15062B1C(D_800D154C, (f32) D_800D1580);
}


void func_15073D34(void) {
    func_15062B50(D_800D154C, (f32) D_800D1580);
}

void func_15073D74(void) {
    struct127 *temp_v0;

    temp_v0 = func_15083E90(D_800D1583);
    if (temp_v0 != 0) {
        temp_v0->unk65 = (u8)0;
    }
}

void func_15073DA4(void) {
    s32 tmp;

    if (D_800D1580 != 0) {
        tmp = 18;
    } else {
        tmp = 2;
    }
    func_1506C460(D_800D154C->unk40, 150.0f, 0, 0, 100, tmp, 60.0f, 0.5f, 0, 0, 1);
}

void func_15073E2C(void) {
    func_1506C460(D_800D154C->unk40, 80.0f, 0, 0, 100, 11, 40.0f, 0.5f, 0, 14, 1);
}

void func_15073EA4(void) {
    func_1506C460(D_800D154C->unk40, 60.0f, 0, 0, 100, 12, 40.0f, 0.5f, 0, 14, 1);
}

void func_15073F1C(void) {
    struct126 *temp_v0 = func_1503195C(D_800D154C, 0x3C, 0);
    if (temp_v0 != NULL) {
        temp_v0->unk38 = 0x960;
    }
}

void func_15073F54(void) {
}

void func_15073F5C(void) {
    D_800D154C->unk2CB = (s8) D_800D1580;
}

void func_15073F78(void) {
    D_800D154C->unk10B &= ~D_800D1580;
}

extern u8 *D_800D210C;
extern f32 D_800CC2E4;
extern f32 D_800CC2EC;
extern s32 func_15082A44(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 func_15033BDC();
extern void func_100100E0(void *, void *, void *, void *, void *, s32);
extern void func_1506AC8C(struct127 *, s32, s32);
extern void func_150F16DC(struct127 *, u8, u8, s32, s32);
typedef struct MP { u8 pad0[8]; s16 unk8; s16 unkA; s16 unkC; } MP;
#define B(p, o, t) (*(t *)((u8 *)(p) + (o)))
void func_15073FA0(void) {
    s32 state;
    s32 idx;
    s32 useDefault;
    s32 arg;
    f32 speed;
    struct127 *obj;
    s32 bone;
    s32 slot;
    s32 cmd;
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 y2;
    s32 rnd;
    s16 angle;
    struct127 *player;

    arg = D_800D1580 & 0xFFFF;
    slot = func_15083E0C(arg);
    if (slot != -1) {
        state = 1;
        D_800D210C[slot] = 0;
        idx = func_15082A44((void *)((slot * 0x30) + (s32)D_800D20FC), slot, 0, 0, 0);
        if (idx != 0) {
            idx--;
            obj = &D_800CC2D0[idx];
            obj->unk124 = D_800C3E78 + 1;
            obj->unique_id = D_800C3E78 + 0x96;
            player = D_800D154C;
            speed = 40.0f;
            useDefault = 1;
            if (player->unk1D4 != NULL) {
                useDefault = 0;
                switch (player->id) {
                case 0x75:
                case 0x80:
                    bone = 4;
                    break;
                case 0x5A:
                    bone = 0x13;
                    break;
                case 0x7B:
                    if (arg == 0x10) {
                        bone = (D_800D1580 >> 0x10) & 0xFF;
                    } else {
                        useDefault = 1;
                    }
                    break;
                default:
                    useDefault = 1;
                    break;
                }
                if (!useDefault) {
                    slot = (s32)player->unk1D4 + (bone << 6);
                    func_15143134(NULL, &obj->x_position, slot);
                    {
                        typedef s32 padA;
                        player = D_800D154C;
                    }
                }
            }
            if (useDefault) {
                obj->x_position = player->x_position;
                obj->y_position = player->y_position + 100.0f;
                obj->z_position = player->z_position;
            }
            if (D_800BE616 != 0) {
                obj->unk2E4 = B(player->unk31C, 0x19C, u16);
                state = 5;
                player->unk31C->unk19B = 0;
                D_800D154C->unk31C->unk78 = 0;
                cmd = (D_800D1580 >> 0x10) & 1;
                obj->gravity = 4.0;
                player = D_800D154C;
                speed = ((f32)(0xF0 - B(player->unk31C, 0x19C, u16)) * 60.0f) / 240.0f;
                speed += 40.0f;
                if (cmd) {
                    speed = 0.0f;
                }
                if (player->unk31C->unk198 != 0) {
                    {
                        typedef s32 padT;
                        func_1505A184((u16)(s32)((45.0f - B(player->unk31C, 0x170, f32)) * 182.0444489f) & 0xFFFF, speed * 4.0f, 0.0f, &x1, &y1, &z1);
                        obj->y_velocity = -y1;
                    }
                    player = D_800D154C;
                } else {
                    obj->y_velocity = 50.0f;
                }
                if (player->id == 0x75) {
                    slot = (s32)func_1503195C(player, 0x64, 0);
                } else {
                    slot = (s32)func_1503195C(player, 0x2C, 0);
                }
                if (slot != 0) {
                    obj->unk8E = B(slot, 0x3C, s32);
                    func_100100E0(func_15033BDC, (void *)slot, D_800D154C, func_1000EE70, obj, obj->unique_id);
                }
                player = D_800D154C;
            }
            angle = (s16)player->unk7A;
            if (player->unk31C != NULL) {
                angle -= player->unk31C->unk12;
            }
            obj->unk7A = angle;
            obj->unk76 = angle;
            obj->xz_velocity = speed;
            obj->unk1CC = obj->y_position;
            if (player->id == 0x4A) {
                state = 5;
                obj->xz_velocity = 0.0f;
                func_151A3390(obj, 0xFF);
                ((MP **)D_800D2104)[obj->unk13F]->unk8 = D_800CC2E4;
                ((MP **)D_800D2104)[obj->unk13F]->unkC = D_800CC2EC;
            } else if (player->id == 0x7B) {
                if (arg == 0x10) {
                    state = 7;
                    ((MP **)D_800D2104)[obj->unk13F]->unk8 = D_800CC2E4;
                    ((MP **)D_800D2104)[obj->unk13F]->unkA = 0x3D4;
                    ((MP **)D_800D2104)[obj->unk13F]->unkC = D_800CC2EC;
                    obj->unique_id = 0xFF - idx;
                    func_1506AC8C(obj, 0xC, 0);
                    func_150F16DC(D_800D154C, func_150ADA20() & 1, func_150ADA20() & 3, 0xFF, 1);
                } else if (arg == 0xA) {
                    state = 0xE;
                    obj->y_position = player->y_position;
                } else {
                    state = 4;
                    obj->unique_id = 0x12;
                    obj->y_position = 1040.0f;
                    obj->xz_velocity = 0.0f;
                }
            } else if (player->id == 0x9A) {
                rnd = (func_150ADA20() & 0xFFF) - 0x800;
                speed = (f32)(s16)(func_150ADA20() % 800U) * 0.009999999776f;
                state = 7;
                func_1505A184(D_800D154C->unk76 + rnd, 30.0f - speed, 0.0f, &x2, &z2, &y2);
                obj->x_position -= x2;
                obj->z_position -= z2;
                player = D_800D154C;
                obj->xz_velocity = (player->xz_velocity * 1.350000024f) + 6.0f;
                obj->y_velocity = player->y_velocity;
                obj->immune = 0xFF;
                obj->y_position = player->y_position - 40.0f;
                obj->unk222 = player->unk222;
            }
            obj->unk232 = state;
            obj->health |= 0x80;
        }
    }
}


void func_15074644(void) {
    D_800D154C->unk31C->unk11A = (s8) D_800D1580;
}

void func_15074664(void) {
    if (D_800D154C->unk31C != NULL) {
        if (D_800D1580 == 1) {
            func_10011FDC(5);
        } else if ((u8) D_800D154C->unk31C->unk94 == 1) {
            func_10011FDC(0);
        }
        D_800D154C->unk31C->unk94 = D_800D1580;
    }
}

void func_150746F0(void) {
    struct127 *target;
    s32 damage;
    struct126 *player;
    struct108 *camera;

    target = NULL;
    if (D_800BE616 != 0) {
        target = D_800D154C;
    } else if (D_800D154C->unk13C != 0) {
        target = D_800CC2D0;
    }
    if (target != NULL) {
        damage = D_800D1580;
        if (damage >= target->health) {
            target->health = 0;
            D_800D154C->health = 0;
            if (target->interaction_state == 1) {
                target->immune = 0xFF;
                func_1507CD64(target, 6);
            }
        } else {
            player = target->unk31C;
            target->health = target->health - damage;
            if (player != NULL) {
                camera = target->camera;
                if (camera != NULL) {
                    if (((u8 *)player)[0x197] != 0) {
                        func_1517F488(0xFF, 0, 0, 0xB4, 0x14, ((u8 *)camera)[0x23D]);
                    }
                }
            }
        }
    }
}
// ?
void func_150747E4(void) {
    s32 i;
    s32 t;

    t = D_800D154C->unk65;
    i = t;
    if (i != 0) {
        i = i - 1;
        D_800CC2D0[i].unk232 = D_800D1580;
        D_800CC2D0[i].unk218 = 0;
    }
}

void func_15074840(void) {
    if (D_800D154C->unk31C != 0) {
        D_800D154C->unk31C->unk1AA += D_800D1580;
    }
}

void func_15074870(void) {
    D_800D154C->unk24F = (s8) D_800D1580;
}

void func_1507488C(void) {
    u8 index;
    u8 mask;
    u8 enabled;

    index = D_800D1580 >> 0x10;
    mask = D_800D1580 >> 8;
    enabled = D_800D1580 & 1;
    if (((s32 *)((u8 *)D_800D154C + 0x2E4))[index] & mask) {
        enabled ^= 1;
    }
    if (enabled != 0) {
        *(u8 *)((u8 *)D_800D154C + 0x138) += (u8)(D_800D1580 >> 0x18);
    }
}

void func_150748F4(void) {
    D_800CC3D7 = (s8) D_800D1580;
}

void func_1507490C(void) {
    struct127 *tmp;
    u8 temp_v0 = (u8)(D_800D154C->unk13C - 100);

    if (D_800D154C->unk13C != 0) {
        tmp = &D_800CC2D0[temp_v0 & 0xFF];
        if (D_800CC2D0[temp_v0].unk13D >= 0x64) {
            D_800CC2D0[temp_v0].unk232 = D_800D1580;
            D_800CC2D0[temp_v0].unk218 = 0;
            D_800CC2D0[temp_v0].stunned = 0;
        }
    }
}

void func_15074980(void) {
    s32 i;
    struct126 *temp;
    f32 dist = (f32)(D_800D1580 * 8);

    for (i = 0; i < D_8008FD8C; i++) {
        if (func_1505A72C(D_800D154C, &D_800CC2D0[i]) < dist) {
            temp = D_800CC2D0[i].unk31C;
            if (temp != 0) {
                temp->chasing = 0x14;
            }
        }
    }
}

void func_15074A44(void) {
    if (D_800D154C->unk31C != 0) {
        D_800D154C->unk31C->unk26 = (s8) D_800D1580;
    }
}

void func_15074A6C(void) {
    if (D_800D154C->unk13C == 0) {
        D_800D154C->unk138++;
    }
}

void func_15074A94(void) {
    f32 temp_f0;
    f32 temp_f14;
    f32 var_f2;
    f32 *new_var = &D_8009A0F4;

    temp_f0 = func_1505A72C(D_800CC2D0, D_800D154C);
    if (D_800D154C->unk148 < D_8009A0E8) {
        D_800D154C->unk148 = D_800D154C->unk154;
    }
    if (temp_f0 < 200.0f) {
        var_f2 = D_8009A0EC;
    } else {
        temp_f14 = D_800D154C->unk148;
        if (D_8009A0F0 < temp_f0) {
            var_f2 = temp_f14;
        } else {
            f32 temp_f12;
            temp_f12 = temp_f14 - *new_var;
            var_f2 = *new_var;
            var_f2 = var_f2 + (((temp_f0 - 200.0f) / D_8009A0F8) * temp_f12);
            temp_f14 = D_800D154C->unk148;
        }
    }
    D_800D154C->unk158 = var_f2;
    D_800D154C->unk154 = D_800D154C->unk158;
    D_800D154C->unk15C = D_8009A0FC;
}

void func_15074B7C(void) {
    D_800D1880 = 0;
    D_800D1878 = D_800D1580;

    if (D_800D1580 == 0) {
        D_800D1878 = D_8009A100;
        D_800D154C->unk1FC |= 4;
        D_800D154C->unk138 = 0;
    }
}

void func_15074BD8(s32 arg0, s32 arg1, s32 arg2) {
}

void func_15074BEC(s32 arg0, s32 arg1, s32 arg2) {
}

void func_15074C00(s32 arg0, struct127 *arg1, s32 arg2) {
    struct199 tmp; // is this actually 2 structs?

    if (((u8)arg1->unk239 & 0x7F) == 5) {
        func_1504715C(&tmp);
        tmp.unk24 = D_800CC2C0;
        tmp.unk28 = D_800CC2C4;
        tmp.unk2C = D_800CC2C8;
        func_150C04C0(&tmp.unk24, &tmp, 0x16, 1, 0xFF, 0);
    }
}

void func_15074C80(struct127 *arg0, struct127 *arg1) {
    s32 temp_a2;

    if ((u16)arg0->unk278 == 0x11) {
        func_151942E8(arg0, arg1);
    }

    if (((u16)arg0->unk278 == 2) || ((u16)arg0->unk278 == 0x12) || ((u16)arg0->unk278 == 3) || ((u16)arg0->unk278 == 6)) {
        if (arg1->health == 0) {
            if ((u16)arg0->unk278 == 2) {
                temp_a2 = 0x2A;
            } else if ((u16)arg0->unk278 == 0x12) {
                temp_a2 = 0x2F;
            } else {
                if ((u16)arg0->unk278 == 3) {
                    temp_a2 = 0x39;
                } else {
                    temp_a2 = 0x51;
                }
            }
            func_150335C8(arg0, arg1, temp_a2, 0xC, 1, 0);
        }
        arg0->health = 0;
    }

    if (arg1->interaction_state == 0x19) {
        func_15166118((s32)(256.0f - (func_150484A0(arg0->y_velocity, arg0->xz_velocity) * D_8009A104)),
                      0xC0 - ((s32)arg0->unk76 >> 8),
                      (s32)arg0->x_position,
                      (s32)arg0->y_position,
                      (s32)arg0->z_position,
                      0x10,
                      0x14,
                      0,
                      0xFF,
                      0);
    }
}

void func_15074DEC(struct127 *arg0, s32 arg1, s32 arg2) {
    arg0->unk2E8 = 1;
}

void func_15074E04(s32 arg0, s32 arg1, s32 arg2) {
    func_1516FE1C((s32) (arg1 - (s32)&D_800CC2D0) / (s32) sizeof(struct127), 0xB4, 0xFF, 0);
    func_1518D1C0(arg1, 0xB, 0, 1, 0xFF, 0, &D_80099C1C);
}

void func_15074E80(struct127 *arg0, struct127 *arg1, s32 arg2) {
    s8 sp1F = 0;
    func_15194794(arg0, arg1, &sp1F);
    if (arg1->interaction_state == 1) {
        arg0->immune = (u8)0xFF;
        arg1->immune = (u8)0xC8;
        D_800D1580 = 0x60000; // 393216
        func_15072740();
    }
}

void func_15074EE8(struct127 *arg0, struct127 *arg1, s32 arg2) {
    if (arg1->interaction_state == 1) {
        arg0->immune = (u8)0xFF;
        arg1->immune = (u8)0xC8;
        D_800D1580 = 0x70000;
        func_15072740();
    }
}

void func_15074F30(struct127 *arg0, struct127 *arg1, s32 arg2) {
    arg0->unk232 = arg1->unk109;
    arg0->unk218 = 0;
}

extern f32 D_8009A108;
extern f32 D_8009A10C;
extern f32 D_8009A110;
void func_15194408(struct127 *arg0, struct127 *arg1);

void func_15074F48(struct127 *arg0, struct127 *arg1, s32 arg2) {
    f32 dx = D_8009A108 - arg0->x_position;
    f32 dz = arg0->z_position - D_8009A10C;

    arg1->unk76 = func_1505A630(dx, dz, 0);
    arg1->xz_velocity = sqrtf(dx * dx + dz * dz) * D_8009A110;
    func_15194408(arg0, arg1);
}

void func_15074FD4(struct127 *arg0, struct127 *arg1, s32 arg2) {
    if (arg1->interaction_state == 1) {
        arg0->immune = (u8)0xFF;
        arg1->immune = (u8)0xC8;
        D_800D1580 = 0x60000;
        if (arg0->id == 0x8E) {
            D_800D1580 = 0x140000;
        }
        func_15072740();
    }
    func_151942B0(arg0, arg1, arg2);
}

void func_15075050(struct127 *arg0, s32 arg1, s32 arg2) {
    arg0->unkB8 = 5.0f;
    if (arg0->xz_velocity > 0.0f) {
        arg0->unkB8 = -5.0f;
    }
    arg0->gravity = 5.0f;
    arg0->y_velocity = 12.0f;
}

void func_150750A4(struct127 *arg0, s32 arg1, s32 arg2) {
    arg0->gravity = 5.0f;
}

void func_150750C4(struct127 *arg0, struct127 *arg1, u8 *arg2) {

    if ((arg1->id  == 0x88) && ((s32) arg0->unk107 < 0x37)) {
        arg0->unk138 += 3;
    }
    func_15194794(arg0, arg1, arg2);
    if (arg1->id != 0x53) {
        func_15145A50(arg1);
        arg1->health = (u8)0;
        func_1507CD64(arg1, 8);
        *arg2 = (u8)39;
    }
}

struct127 *func_1507515C(struct127 *arg0) {
    struct127 *obj;
    s32 i;

    obj = D_800CC2D0;
    for (i = 0; i < 26; i++) {
        if ((obj->interaction_state != 0) && (obj->id != 0xFF) &&
            (obj->unk65 == ((arg0 - D_800CC2D0) + 1))) {
            return obj;
        }
        obj++;
    }
    return NULL;
}

