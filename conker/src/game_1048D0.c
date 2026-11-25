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

/* The fields of struct127.unk31C (a struct126) that struct126 doesn't name; it also stops short of 0x1AE. */
typedef struct {
    /* 0x000 */ u8 pad0[0x4F];
    /* 0x04F */ u8 unk4F;
    /* 0x050 */ u8 pad50[0xDC];
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

extern s8 D_800CC4B4;
extern u8 *func_15033E84(struct127 *arg0);
extern s32 func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
extern void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
extern void func_1505A3A8(f32 arg0, struct127 *arg1, f32 arg2, f32 arg3, u8 arg4);
extern void func_1505C7D8(struct127 *arg0, u8 arg1);
extern s32 func_150535F4(struct127 *arg0);
extern void func_1001091C(struct127 *arg0, s32 arg1);
extern void func_10010FFC(s32 a, s32 b, s32 c, s32 d, s32 e, struct127 *f);

/*
 * Per-frame update for an object that can be carried by a player (unk13C set, unk124 = player index).
 * While the player's unk31C->unk4E is set the object follows them: it copies their 0x180-0x19E state, eases
 * its angle towards theirs and (id 0x7F) moves halfway to them. Otherwise it is let go and, unless unk232 is 4,
 * calls func_1505327C for each falling, eligible player. Unless its id is 0x48, it then tracks its height (unk28); once it has
 * spent more than 10 frames over 75 units high, it dies when the player lands or it is in water. Finally it
 * plays its movement sound and runs func_150535F4.
 */
void func_150D7928(struct127 *obj) {
    f32 angleOffset;
    u8 playerIndex;
    f32 speed;
    struct127 *player;
    s32 linkType;
    s16 *dst;
    s16 *src;
    s32 i;
    s32 timer;
    f32 pad; // matching: unused stack slot keeps the frame size
    u8 id;
    u16 angle;
    f32 scratch; /* obj's speed across func_15059140, later the rounded size stored in padC8 */
    s32 n; /* the rider's target angle, later the player loop index */
    f32 offset;

    playerIndex = obj->unk124;
    speed = obj->xz_velocity;
    id = obj->id;
    linkType = -1;
    if (id == 0x48) {
        player = (struct127 *)func_15033E84(obj);
        if (player != NULL) {
            linkType = player->unk6;
        }
    }
    obj->unkB0 = 25;
    if (obj->unkE4 != 0) {
        obj->unk90 = obj->unkE4;
    }
    if (obj->unk13C != 0 && (player = &D_800CC2D0[playerIndex])->unk31C->unk4E != 0) {
        n = player->unk7A;
        obj->unkE4 = 0;
        obj->unk180 = player->unk180;
        obj->unk184 = player->unk184;
        /* copy the nine halfwords at 0x18C from the player */
        dst = (s16 *)&obj->unk18C;
        src = (s16 *)&player->unk18C;
        for (i = 0; i < 9; i++) {
            dst[i] = src[i];
        }
        obj->gravity = 6.0f;
        angleOffset = *(f32 *)&player->pad54 * 0.45f;
        obj->unk44 = player->unkC4;
        if (id == 0x7F) {
            player->immune = 20;
            player->unk184 &= ~0x1F;
            player->unk184 |= 4;
            obj->y_position = player->y_position;
            obj->x_position += (player->x_position - obj->x_position) * 0.5f;
            obj->z_position += (player->z_position - obj->z_position) * 0.5f;
            player->disable_jump = 10;
            if (O31C(player)->unk4F == 2) {
                if (speed < player->unk44 * 1.35f) {
                    speed = player->unk44 * 1.35f;
                }
                player->unkAA = 70;
            }
        } else {
            obj->y_velocity = 0.0f;
        }
        angle = obj->unk21C;
        obj->unk21C = angle + (s16)(n - angle) / 3;
        obj->unk7A = angle;
        obj->unk76 = angle;
        obj->xz_velocity = player->xz_velocity;
        if (obj->unkD0 != 0) {
            func_1505C7D8(obj, D_800C3E78);
        }
    } else {
        obj->unk21C = obj->unk7A;
        if (obj->unk13C != 0) {
            if (D_800CC2D0[playerIndex].stunned != 0) {
                obj->target_speed = 15.0f;
                obj->y_velocity = 80.0f;
            }
            obj->unk13C = 0;
        }
        playerIndex = D_800C3E78;
        obj->unk65 = 0;
        // matching: unk90 (u16) is read as s16 (lh); a value cast still loads with lhu
        obj->unkE4 = *(s16 *)&obj->unk90;
        func_1505A3A8(0.0f, obj, 0.0f, 0.5f, 1);
        scratch = obj->xz_velocity;
        obj->stunned = 30;
        func_15059140(obj);
        obj->xz_velocity = scratch;
        angleOffset = (obj->unkC4 - obj->unk44) * 0.8f;
        if (obj->unk232 == 4) {
            if (id == 0x48) {
                D_800CC4B4 = 1;
            }
            if (func_15052F9C(obj, 50.0f, 0x17, 4, 0, 5, 0, 0, 0, 0) != 0) {
                return;
            }
        } else {
            for (n = 0; n != 2; n++) {
                if (D_800CC2D0[n].interaction_state == 1) {
                    if (D_800CC2D0[n].unk31C->unk78 == 0 && D_800CC2D0[n].unk31C->unk11A == 0 && (D_800CC268 & (1 << n)) &&
                        D_800CC2D0[n].y_velocity < 0.0f) {
                        obj->unk124 = n;
                        offset = 39.0f;
                        if (id == 0x48) {
                            D_800CC2D0[n].stunned = 0;
                            offset = 0.0f;
                        }
                        func_1505327C(obj, offset, 3.0f, 4, -1);
                    }
                }
            }
        }
    }
    obj->immune = 0xFF;
    obj->unk28 = obj->y_position - obj->unk180;
    if (id == 0x48) {
        if (linkType == 0x7A) {
            f32 height = obj->unk180 + 180.0f;

            obj->unkF8 |= 0x10000;
            func_15058EA4(obj, height + 10.0f, 1.2f, height, -1.1f, 4.0f, -4.0f);
            if (obj->unk13C != 0) {
                D_800CC2D0[obj->unk124].unkCC = 30;
            }
            obj->unkC4 = -obj->y_velocity;
            obj->unk40 = obj->unk7A * (360.0f / 65536.0f);
        }
    } else {
        s32 volume;
        s32 pad; // matching: unused stack slot keeps the frame layout
        func_1506B100(0, 2.0f, 4.0f);
        if (obj->unk28 > 75.0f) {
            timer = obj->unk102;
            if (timer < 30) {
                timer = ++obj->unk102;
            }
        } else {
            timer = obj->unk102;
            if (timer < 10) {
                timer = obj->unk102 = 0;
            }
        }
        if (timer > 10 && ((player = &D_800CC2D0[playerIndex])->unk28 == 0.0f || obj->in_water != 0 ||
                            (obj->unk124 != 0 && O31C(player)->unk4F == 2))) {
            if (obj->unk13C != 0) {
                D_800CC2D0[obj->unk124].unkCC = 0;
                D_800CC2D0[obj->unk124].unk31C->unk4E = 0;
                D_800CC2D0[obj->unk124].in_water = 0;
                D_800CC2D0[obj->unk124].gravity = 4.0f;
            }
            func_100109D0(obj);
            func_10010FFC(0, 0x51, 0x6D60, 0, 0, obj);
            obj->health = 0;
        }
        if (obj->y_scale < 1.0f) {
            obj->y_scale += 0.05f;
        }
        if (obj->unk28 == 0.0f) {
            obj->target_speed = 0.0f;
            *(f32 *)&obj->padBC *= 0.5f;
        }
        if (obj->target_speed == 0.0f) {
            obj->unkC4 = obj->unk44 + angleOffset;
        } else {
            obj->unkC4 += obj->target_speed;
            if (obj->unkC4 < 0.0f) {
                obj->unkC4 += 360.0f;
            } else if (obj->unkC4 > 360.0f) {
                obj->unkC4 -= 360.0f;
            }
        }
        if (obj->unk13C != 0) {
            scratch = (s16)(fabsf(obj->unkC4 * 0.7f) + 44.0f);
            *(f32 *)&obj->padC8 = scratch;
            D_800CC2D0[obj->unk124].unkCC = (s16)(scratch + 44) * obj->y_scale;
        } else {
            *(f32 *)&obj->padC8 = 44.0f;
        }
        if (speed > 0.2f && obj->unk28 < 10.0f && obj->health != 0 && D_800C35EA != 1) {
            volume = (s32)(1000.0f * speed);
            if (volume > 18000) {
                volume = 18000;
            }
            if (func_100107F8(obj) == 0) {
                func_10010154(0x678, obj, volume, 1000, 2000);
            } else {
                func_1001091C(obj, volume);
            }
        } else {
            func_100109D0(obj);
        }
        func_15052490(obj, obj->unk76, speed, 0.55f);
    }
    func_150535F4(obj);
}

