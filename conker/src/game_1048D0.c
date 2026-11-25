#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Object that receives message 0x36; the three bytes at 0x28 are set to pure red or green, so they look like an RGB tint. */
typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 pad2B[0x2D];
    /* 0x58 */ s32 flags; /* bit 1: tint enabled */
} TintedObject;

#define TINT_ENABLED 2
#define MSG_SET_TINT 0x36

/* Message handler: state 1 or 2 tints the object red, 3 tints it green, anything else clears the tint. */
void func_150D7420(TintedObject *obj, u8 *state, u8 msg) {
    if (msg == MSG_SET_TINT) {
        switch (*state) {
            case 0:
                break;
            case 1:
                obj->flags |= TINT_ENABLED;
                obj->r = 0xFF;
                obj->g = 0;
                obj->b = 0;
                return;
            case 2:
                obj->flags |= TINT_ENABLED;
                obj->r = 0xFF;
                obj->g = 0;
                obj->b = 0;
                return;
            case 3:
                obj->r = 0;
                obj->flags |= TINT_ENABLED;
                obj->g = 0xFF;
                obj->b = 0;
                return;
        }
        obj->flags &= ~TINT_ENABLED;
    }
}

/* Message handler: state 2 tints the object red, 3 tints it green, anything else clears the tint. */
void func_150D74DC(TintedObject *obj, u8 *state, u8 msg) {
    if (msg == MSG_SET_TINT) {
        switch (*state) {
            case 0:
                break;
            case 1:
                obj->flags &= ~TINT_ENABLED;
                return;
            case 2:
                obj->flags |= TINT_ENABLED;
                obj->r = 0xFF;
                obj->g = 0;
                obj->b = 0;
                return;
            case 3:
                obj->r = 0;
                obj->flags |= TINT_ENABLED;
                obj->g = 0xFF;
                obj->b = 0;
                return;
        }
        obj->flags &= ~TINT_ENABLED;
    }
}

/* Message handler: state 3 tints the object green, anything else clears the tint. */
void func_150D758C(TintedObject *obj, u8 *state, u8 msg) {
    if (msg == MSG_SET_TINT) {
        switch (*state) {
            case 0:
                break;
            case 1:
                obj->flags &= ~TINT_ENABLED;
                return;
            case 2:
                obj->flags &= ~TINT_ENABLED;
                return;
            case 3:
                obj->r = 0;
                obj->flags |= TINT_ENABLED;
                obj->g = 0xFF;
                obj->b = 0;
                return;
        }
        obj->flags &= ~TINT_ENABLED;
    }
}

extern u16 D_800D9910; /* texture animation clock; the high byte is the frame */

/* Advances the texture animation clock by this frame's tick count (unless arg0 is set). */
void func_150D7630(s32 arg0) {
    if (arg0 == 0) {
        D_800D9910 += D_800BE9E4 << 6;
    }
}

/* Two texture-id tables of five animation frames each. */
typedef struct {
    s32 *frames[2];
} TextureFrameTables;
extern TextureFrameTables D_80088900; /* = { D_80090230, D_80090244 } */
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

/* Binds six animated textures to segments 2-7, texture i showing frame (clock + 13 * i) % 5;
 * the last two come from the second frame table. */
Gfx *func_150D765C(Gfx *gdl) {
    s32 i;
    s32 segment;
    s32 frame;
    s32 size;
    s32 table;
    s32 frameIndex;
    s32 addr;
    TextureFrameTables tables = D_80088900;

    table = 0;
    frame = D_800D9910 >> 8;
    for (i = 0; i < 6; i++) {
        if (i >= 4) {
            table = 1;
        }
        frameIndex = (frame + i * 13) % 5;
        addr = func_1510D0EC(tables.frames[table][frameIndex], &size, 3, 0);
        segment = i + 2;
        gSPSegment(gdl++, segment, addr);
    }
    return gdl;
}

/* The object at struct127.unk31C; struct126 doesn't reach 0x1AE, so this file uses its own view. */
typedef struct {
    /* 0x000 */ u8 pad0[0x4E];
    /* 0x04E */ u8 unk4E;
    /* 0x04F */ u8 unk4F;
    /* 0x050 */ u8 pad50[0x28];
    /* 0x078 */ u8 unk78;
    /* 0x079 */ u8 pad79[0xA1];
    /* 0x11A */ u8 unk11A;
    /* 0x11B */ u8 pad11B[0x11];
    /* 0x12C */ s16 unk12C;
    /* 0x12E */ u8 pad12E[0x80];
    /* 0x1AE */ s8 unk1AE; /* uses left: each attack below spends one */
} Obj31C;

#define O31C(obj) ((Obj31C *)(obj)->unk31C)

s32 func_1508855C(struct127 *obj);
void func_150885EC(s32 src, s32 dst);
s32 func_1508868C(struct127 *obj);

/* Performs action `action` for obj: 0x30/0x31 play sound 0x628 and set unk12C to 120; 0x32/0x33 fire a
 * kind-0xD projectile (0x33 with unk83 60 instead of 30, and the projectile takes over a copy of obj's racer
 * slot). Each spends one use of unk1AE, and nothing happens once those run out. */
void func_150D7790(struct127 *obj, s32 action) {
    s32 newRacer;
    s32 oldRacer;
    s32 variant;
    struct127 *projectile;

    variant = 0;
    if (obj->unk31C != NULL && O31C(obj)->unk1AE <= 0) {
        return;
    }
    switch (action) {
    case 0x30:
    case 0x31:
        if (obj->unk31C != NULL) {
            O31C(obj)->unk12C = 120;
            O31C(obj)->unk1AE -= 1;
            func_10010154(0x628, obj, 0x7FFF, 500, 1000);
        }
        return;
    case 0x33:
        variant = 1;
    case 0x32:
        break;
    default:
        return;
    }
    projectile = (struct127 *)func_1506C460(obj->unk40, 310.0f, 0.0f, 0.0f, variant * 30 + 30, 0xD, 0.0f, 1.0f,
                                            0, 1, variant == 0);
    if (projectile != NULL) {
        projectile->unk124 = D_800C3E78 + 1;
    }
    if (projectile != NULL && variant != 0) {
        projectile->unk221 = -1;
        oldRacer = func_1508855C(obj);
        if (oldRacer != -1) {
            newRacer = func_1508868C(projectile);
            if (newRacer != -1) {
                func_150885EC(oldRacer, newRacer);
            }
        }
    }
    if (obj->unk31C != NULL) {
        O31C(obj)->unk1AE -= 1;
    }
}

extern f32 D_800A0AF0;
extern f32 D_800A0AF4;
extern f32 D_800A0AF8;
extern f32 D_800A0AFC;
extern f32 D_800A0B00;
extern f32 D_800A0B04;
extern f32 D_800A0B08;
extern s16 D_800CC39C;
extern s8 D_800CC4B4;
extern u8 *func_15033E84(struct127 *arg0);
extern s32 func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
extern void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
extern void func_1505A3A8(f32 arg0, struct127 *arg1, f32 arg2, f32 arg3, u8 arg4);
extern void func_1505C7D8(struct127 *arg0, u8 arg1);
extern s32 func_150535F4(struct127 *arg0);
extern void func_1001091C(struct127 *arg0, s32 arg1);
extern void func_10010FFC(s32 a, s32 b, s32 c, s32 d, s32 e, struct127 *f);
#define BY(p, o) (*(u8 *)((u8 *)(p) + (o)))

void func_150D7928(struct127 *arg0) {
    f32 sp84;
    u8 sp83;
    f32 sp7C;
    struct127 *temp_a2;
    s32 sp74;
    s16 *var_v0;
    s16 *var_v1;
    s32 var_a0;
    s32 var_v0_3;
    f32 pad2;
    u8 temp_v0;
    u16 temp_v0_5;
    f32 sp58;
    s32 var_t0;
    f32 var_f0;

    sp83 = arg0->unk124;
    sp7C = arg0->xz_velocity;
    temp_v0 = arg0->id;
    sp74 = -1;
    if (temp_v0 == 0x48) {
        temp_a2 = (struct127 *)func_15033E84(arg0);
        if (temp_a2 != NULL) {
            sp74 = temp_a2->unk6;
        }
    }
    arg0->unkB0 = 0x19;
    if (arg0->unkE4 != 0) {
        arg0->unk90 = arg0->unkE4;
    }
    if (arg0->unk13C != 0) {
        temp_a2 = &D_800CC2D0[sp83];
        if (BY(temp_a2->unk31C, 0x4E) != 0) {
            var_t0 = temp_a2->unk7A;
            arg0->unkE4 = 0;
            arg0->unk180 = temp_a2->unk180;
                    arg0->unk184 = temp_a2->unk184;
            var_v0 = (s16 *)&arg0->unk18C;
            var_v1 = (s16 *)&temp_a2->unk18C;
            for (var_a0 = 0; var_a0 < 9; var_a0++) {
                var_v0[var_a0] = var_v1[var_a0];
            }
            arg0->gravity = 6.0f;
            sp84 = *(f32 *)&temp_a2->pad54 * D_800A0AF0;
            arg0->unk44 = temp_a2->unkC4;
            if (temp_v0 == 0x7F) {
                temp_a2->immune = 0x14;
                temp_a2->unk184 &= ~0x1F;
                temp_a2->unk184 |= 4;
                arg0->y_position = temp_a2->y_position;
                arg0->x_position = arg0->x_position + ((temp_a2->x_position - arg0->x_position) * 0.5f);
                arg0->z_position = arg0->z_position + ((temp_a2->z_position - arg0->z_position) * 0.5f);
                temp_a2->disable_jump = 0xA;
                if (BY(temp_a2->unk31C, 0x4F) == 2) {
                    if (sp7C < temp_a2->unk44 * D_800A0AF4) {
                        sp7C = temp_a2->unk44 * D_800A0AF4;
                    }
                    temp_a2->unkAA = 0x46;
                }
            } else {
                arg0->y_velocity = 0.0f;
            }
            temp_v0_5 = arg0->unk21C;
            arg0->unk21C = temp_v0_5 + ((s16)(var_t0 - temp_v0_5) / 3);
            arg0->unk7A = temp_v0_5;
            arg0->unk76 = temp_v0_5;
            arg0->xz_velocity = temp_a2->xz_velocity;
            if (arg0->unkD0 != 0) {
                func_1505C7D8(arg0, D_800C3E78);
            }
            goto block_36;
        }
    }
    arg0->unk21C = arg0->unk7A;
    if (arg0->unk13C != 0) {
        if (D_800CC3D4[sp83 * 0x32C] != 0) {
            arg0->target_speed = 15.0f;
            arg0->y_velocity = 80.0f;
        }
        arg0->unk13C = 0;
    }
    sp83 = D_800C3E78;
    arg0->unk65 = 0;
    arg0->unkE4 = *(s16 *)&arg0->unk90;
    func_1505A3A8(0.0f, arg0, 0.0f, 0.5f, 1);
    sp58 = arg0->xz_velocity;
    arg0->stunned = 0x1E;
    func_15059140(arg0);
    arg0->xz_velocity = sp58;
    sp84 = (arg0->unkC4 - arg0->unk44) * D_800A0AF8;
    if (arg0->unk232 == 4) {
        if (temp_v0 == 0x48) {
            D_800CC4B4 = 1;
        }
        if (func_15052F9C(arg0, 50.0f, 0x17, 4, 0, 5, 0, 0, 0, 0) != 0) {
            return;
        }
    } else {
        for (var_t0 = 0; var_t0 != 2; var_t0++) {
            if (D_800CC2D0[var_t0].interaction_state == 1) {
                if ((BY(D_800CC2D0[var_t0].unk31C, 0x78) == 0) && (BY(D_800CC2D0[var_t0].unk31C, 0x11A) == 0) && ((1 << var_t0) & D_800CC268) && (D_800CC2D0[var_t0].y_velocity < 0.0f)) {
                    arg0->unk124 = var_t0;
                    var_f0 = 39.0f;
                    if (temp_v0 == 0x48) {
                        D_800CC2D0[var_t0].stunned = 0;
                        var_f0 = 0.0f;
                    }
                    func_1505327C(arg0, var_f0, 3.0f, 4, -1);
                }
            }
        }
    }
block_36:
    arg0->immune = 0xFF;
    arg0->unk28 = arg0->y_position - arg0->unk180;
    if (temp_v0 == 0x48) {
        if (sp74 == 0x7A) {
            f32 temp_f0_3;
            temp_f0_3 = arg0->unk180 + 180.0f;
            arg0->unkF8 |= 0x10000;
            func_15058EA4(arg0, temp_f0_3 + 10.0f, 1.2f, temp_f0_3, D_800A0AFC, 4.0f, -4.0f);
            if (arg0->unk13C != 0) {
                *(s16 *)((u8 *)&D_800CC39C + arg0->unk124 * 0x32C) = 0x1E;
            }
            arg0->unkC4 = -arg0->y_velocity;
            arg0->unk40 = (f32)arg0->unk7A * 0.005493164f;
        }
    } else {
        s32 var_a2;
        s32 pad4;
        func_1506B100(0, 2.0f, 4.0f);
        if (arg0->unk28 > 75.0f) {
            var_v0_3 = arg0->unk102;
            if (var_v0_3 < 0x1E) {
                var_v0_3 = ++arg0->unk102;
            }
        } else {
            var_v0_3 = arg0->unk102;
            if (var_v0_3 < 0xA) {
                var_v0_3 = arg0->unk102 = 0;
            }
        }
        if ((var_v0_3 >= 0xB) && ((temp_a2 = &D_800CC2D0[sp83], (temp_a2->unk28 == 0.0f)) || (arg0->in_water != 0) || ((arg0->unk124 != 0) && (BY(temp_a2->unk31C, 0x4F) == 2)))) {
            if (arg0->unk13C != 0) {
                D_800CC2D0[arg0->unk124].unkCC = 0;
                BY(D_800CC2D0[arg0->unk124].unk31C, 0x4E) = 0;
                D_800CC2D0[arg0->unk124].in_water = 0;
                D_800CC2D0[arg0->unk124].gravity = 4.0f;
            }
            func_100109D0(arg0);
            func_10010FFC(0, 0x51, 0x6D60, 0, 0, arg0);
            arg0->health = 0;
        }
        if (arg0->y_scale < 1.0f) {
            arg0->y_scale = arg0->y_scale + D_800A0B00;
        }
        if (arg0->unk28 == 0.0f) {
            arg0->target_speed = 0.0f;
            *(f32 *)&arg0->padBC = *(f32 *)&arg0->padBC * 0.5f;
        }
        if (arg0->target_speed == 0.0f) {
            arg0->unkC4 = arg0->unk44 + sp84;
        } else {
            arg0->unkC4 += arg0->target_speed;
            if (arg0->unkC4 < 0.0f) {
                arg0->unkC4 = arg0->unkC4 + 360.0f;
            } else if (arg0->unkC4 > 360.0f) {
                arg0->unkC4 = arg0->unkC4 - 360.0f;
            }
        }
        if (arg0->unk13C != 0) {
            sp58 = (f32)(s16)(s32)(fabsf(arg0->unkC4 * D_800A0B04) + 44.0f);
            *(f32 *)&arg0->padC8 = sp58;
            D_800CC2D0[arg0->unk124].unkCC = (s16)(s32)((f32)(s16)(s32)(sp58 + 44) * arg0->y_scale);
        } else {
            *(f32 *)&arg0->padC8 = 44.0f;
        }
        if ((D_800A0B08 < sp7C) && (arg0->unk28 < 10.0f) && (arg0->health != 0) && (D_800C35EA != 1)) {
            var_a2 = (s32)(1000.0f * sp7C);
            if (var_a2 >= 0x4651) {
                var_a2 = 0x4650;
            }
            if (func_100107F8(arg0) == 0) {
                func_10010154(0x678, arg0, var_a2, 0x3E8, 0x7D0);
            } else {
                func_1001091C(arg0, var_a2);
            }
        } else {
            func_100109D0(arg0);
        }
        func_15052490(arg0, arg0->unk76, sp7C, 0.55f);
    }
    func_150535F4(arg0);
}

