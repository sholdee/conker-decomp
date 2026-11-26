#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Defined in game_803A0.c as void func_15052F9C(...): it returns nothing, but this file was compiled
   with an s32 return and tests the result. */
s32 func_15052F9C(Actor *arg0, f32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 immune, s32 arg7,
                  s32 force, s32 arg9);

/* Object that receives message 0x36; the three bytes at 0x28 are set to pure red or green, so they look like an RGB tint. */
struct TintedObject {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 pad2B[0x2D];
    /* 0x58 */ s32 flags; /* bit 1: tint enabled */
};

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

extern TextureFrameTables D_80088900; /* = { D_80090230, D_80090244 } */

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


/* Performs action `action` for obj: 0x30/0x31 play sound 0x628 and set unk12C to 120; 0x32/0x33 fire a
 * kind-0xD projectile (0x33 with unk83 60 instead of 30, and the projectile takes over a copy of obj's racer
 * slot). Each spends one use of unk1AE, and nothing happens once those run out. */
void func_150D7790(Actor *obj, s32 action) {
    s32 newRacer;
    s32 oldRacer;
    s32 variant;
    Actor *projectile;

    variant = 0;
    if (obj->mv != NULL && obj->mv->ammo_count <= 0) {
        return;
    }
    switch (action) {
    case 0x30:
    case 0x31:
        if (obj->mv != NULL) {
            obj->mv->unk12C = 120;
            obj->mv->ammo_count -= 1;
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
    projectile = (Actor *)func_1506C460(obj->theta, 310.0f, 0.0f, 0.0f, variant * 30 + 30, 0xD, 0.0f, 1.0f,
                                            NULL, 1, variant == 0);
    if (projectile != NULL) {
        projectile->owner = dc_glbl_CharNum + 1;
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
    if (obj->mv != NULL) {
        obj->mv->ammo_count -= 1;
    }
}

extern s8 D_800CC4B4;

/*
 * Per-frame update for an object that can be carried by a player (unk13C set, unk124 = player index).
 * While the player's mv->transport is set the object follows them: it copies their 0x180-0x19E state, eases
 * its angle towards theirs and (id 0x7F) moves halfway to them. Otherwise it is let go and, unless unk232 is 4,
 * calls func_1505327C for each falling, eligible player. Unless its id is 0x48, it then tracks its height (unk28); once it has
 * spent more than 10 frames over 75 units high, it dies when the player lands or it is in water. Finally it
 * plays its movement sound and runs func_150535F4.
 */
void func_150D7928(Actor *obj) {
    f32 angleOffset;
    u8 playerIndex;
    f32 speed;
    Actor *player;
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

    playerIndex = obj->owner;
    speed = obj->speed;
    id = obj->character_type;
    linkType = -1;
    if (id == 0x48) {
        player = (Actor *)func_15033E84(obj);
        if (player != NULL) {
            linkType = player->unk6;
        }
    }
    obj->unkB0 = 25;
    if (obj->bodyRadius != 0) {
        obj->unk90 = obj->bodyRadius;
    }
    if (obj->lifting != 0 && (player = &movement_obj[playerIndex])->mv->transport != 0) {
        n = player->langle;
        obj->bodyRadius = 0;
        obj->floorY = player->floorY;
        obj->floorMaterial = player->floorMaterial;
        /* copy the player's floor triangle */
        dst = &obj->floorTri.v[0].x;
        src = &player->floorTri.v[0].x;
        for (i = 0; i < 9; i++) {
            dst[i] = src[i];
        }
        obj->ygrav = 6.0f;
        angleOffset = player->unk54 * 0.45f;
        obj->stickspeed = player->lean_angle;
        if (id == 0x7F) {
            player->imune = 20;
            player->floorMaterial &= ~0x1F;
            player->floorMaterial |= 4;
            obj->ypos = player->ypos;
            obj->xpos += (player->xpos - obj->xpos) * 0.5f;
            obj->zpos += (player->zpos - obj->zpos) * 0.5f;
            player->nojump = 10;
            if (player->mv->transport_surface == 2) {
                if (speed < player->stickspeed * 1.35f) {
                    speed = player->stickspeed * 1.35f;
                }
                player->unkAA = 70;
            }
        } else {
            obj->yspd = 0.0f;
        }
        angle = obj->unk21C;
        obj->unk21C = angle + (s16)(n - angle) / 3;
        obj->langle = angle;
        obj->angle = angle;
        obj->speed = player->speed;
        if (obj->doing_hit != 0) {
            func_1505C7D8(obj, dc_glbl_CharNum);
        }
    } else {
        obj->unk21C = obj->langle;
        if (obj->lifting != 0) {
            if (movement_obj[playerIndex].stunned != 0) {
                obj->wantspeed = 15.0f;
                obj->yspd = 80.0f;
            }
            obj->lifting = 0;
        }
        playerIndex = dc_glbl_CharNum;
        obj->attach_char = 0;
        // matching: unk90 (u16) is read as s16 (lh); a value cast still loads with lhu
        obj->bodyRadius = obj->unk90;
        func_1505A3A8(0.0f, obj, 0.0f, 0.5f, 1);
        scratch = obj->speed;
        obj->stunned = 30;
        func_15059140(obj);
        obj->speed = scratch;
        angleOffset = (obj->lean_angle - obj->stickspeed) * 0.8f;
        if (obj->intelwalk == 4) {
            if (id == 0x48) {
                D_800CC4B4 = 1;
            }
            if (func_15052F9C(obj, 50.0f, 0x17, 4, 0, 5, 0, 0, 0, 0) != 0) {
                return;
            }
        } else {
            for (n = 0; n != 2; n++) {
                if (movement_obj[n].type == 1) {
                    if (movement_obj[n].mv->rt_context_b == 0 && movement_obj[n].mv->weapon_status == 0 && (D_800CC268 & (1 << n)) &&
                        movement_obj[n].yspd < 0.0f) {
                        obj->owner = n;
                        offset = 39.0f;
                        if (id == 0x48) {
                            movement_obj[n].stunned = 0;
                            offset = 0.0f;
                        }
                        func_1505327C(obj, offset, 3.0f, 4, -1);
                    }
                }
            }
        }
    }
    obj->imune = 0xFF;
    obj->shad = obj->ypos - obj->floorY;
    if (id == 0x48) {
        if (linkType == 0x7A) {
            f32 height = obj->floorY + 180.0f;

            obj->noshove |= 0x10000;
            func_15058EA4(obj, height + 10.0f, 1.2f, height, -1.1f, 4.0f, -4.0f);
            if (obj->lifting != 0) {
                movement_obj[obj->owner].unkCC = 30;
            }
            obj->lean_angle = -obj->yspd;
            obj->theta = obj->langle * (360.0f / 65536.0f);
        }
    } else {
        s32 volume;
        s32 pad; // matching: unused stack slot keeps the frame layout
        func_1506B100(0, 2.0f, 4.0f);
        if (obj->shad > 75.0f) {
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
        if (timer > 10 && ((player = &movement_obj[playerIndex])->shad == 0.0f || obj->inwater != 0 ||
                            (obj->owner != 0 && player->mv->transport_surface == 2))) {
            if (obj->lifting != 0) {
                movement_obj[obj->owner].unkCC = 0;
                movement_obj[obj->owner].mv->transport = 0;
                movement_obj[obj->owner].inwater = 0;
                movement_obj[obj->owner].ygrav = 4.0f;
            }
            func_100109D0(obj);
            func_10010FFC(0, 0x51, 0x6D60, 0, 0, obj);
            obj->energy = 0;
        }
        if (obj->scale_y < 1.0f) {
            obj->scale_y += 0.05f;
        }
        if (obj->shad == 0.0f) {
            obj->wantspeed = 0.0f;
            obj->unkBC *= 0.5f;
        }
        if (obj->wantspeed == 0.0f) {
            obj->lean_angle = obj->stickspeed + angleOffset;
        } else {
            obj->lean_angle += obj->wantspeed;
            if (obj->lean_angle < 0.0f) {
                obj->lean_angle += 360.0f;
            } else if (obj->lean_angle > 360.0f) {
                obj->lean_angle -= 360.0f;
            }
        }
        if (obj->lifting != 0) {
            scratch = (s16)(fabsf(obj->lean_angle * 0.7f) + 44.0f);
            obj->unkC8 = scratch;
            movement_obj[obj->owner].unkCC = (s16)(scratch + 44) * obj->scale_y;
        } else {
            obj->unkC8 = 44.0f;
        }
        if (speed > 0.2f && obj->shad < 10.0f && obj->energy != 0 && D_800C35EA[0] != 1) {
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
        func_15052490(obj, obj->angle, speed, 0.55f);
    }
    func_150535F4(obj);
}

