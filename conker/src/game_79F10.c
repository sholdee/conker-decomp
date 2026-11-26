#include <ultra64.h>

#include "structs.h"

/* This file's .bss (0x800CC280-0x800CC2BF). IDO lays out a unit's variables in the order of their first
   declaration, so they are defined here, in address order, before variables.h declares them. The order
   also depends on how many types and variables are declared before them (see init_50A0.c), so they come
   before functions.h, whose subsystem headers change often. */
s32 D_800CC280;
OSContPad *D_800CC284; // the current player's controller input
s32 D_800CC288;
s32 D_800CC28C;
s32 D_800CC290;
s32 D_800CC294; // not referenced by any code
TexAnimState D_800CC298;
u8 D_800CC2B0;
u8 D_800CC2B1;
s16 D_800CC2B2;
f32 D_800CC2B4;
u8 D_800CC2B8;
u16 D_800CC2BA; // frame step of the actor being updated
s32 D_800CC2BC;

#include "functions.h"

#include "variables.h"


void func_1504CA60(Actor *actor) {
    f32 range;
    f32 height;

    height = actor->floorY + 20.0f;
    actor->unk1CC = actor->ypos;
    range = 18.0f;
    if (actor->shad > 30.0f) {
        range = 80.0f;
    }
    if (actor->count3 != 0) {
        range = 80.0f;
        height -= 30.0f;
        if ((actor->shad < 5.0f) && (actor->yspd < 0.0f)) {
            actor->count3 = 0;
            actor->yspd = 45.0f;
        }
    } else if (actor->shad < 20.0f) {
        if ((func_150ADA20_u8() & 0x3F) == 0) {
            actor->yspd = 25.0f;
        }
    }
    func_15058EA4(actor, height, 3.8f, height, -3.5f, 80.0f, -range);
    actor->count2 = 1;
}

/* The part of the rope (ActorCtl.rope) read here; game_1D9F10.c has the full Rope. */
typedef struct Sub9C {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ f32 unk2C;
} Sub9C;




extern f32 D_800CBD90;
extern f32 D_800CBD94;
extern f32 D_800CBD98;
extern f32 D_800CBDFC;
extern u16 D_800CC27E;

// matching: second names for D_800CC280/D_800CC2BC (aliases in undefined_syms.us.txt). As separate
// symbols IDO neither shares their %hi nor reuses loads across them; a few uses need that.
extern s32 D_800cc280;
extern s32 D_800cc2bc;
// matching: a declaration-only block scope (no storage). IDO opens a lexical block for it, which changes
// register allocation/scheduling of the surrounding statements; no storage-free idiomatic form matches.
#define BLK typedef s32 blk_t

/*
 * Per-frame update of a player-controlled actor: reads (or overrides) the controller, turns stick input
 * into a move angle/speed, then runs turning, swimming, climbing, pushing, floor collision and water entry.
 */
void func_1504CB98(Actor *actor) {
    u16 prevFacing;
    u16 moveAngle;
    u8 pad1; // matching: unused; keeps the original stack layout
    u8 turn;
    u8 turnRate;
    f32 moveSpeed;
    s16 angleDelta;
    u16 heading;
    s32 diff;
    f32 surfaceDelta;
    s8 turnStep;
    s8 pad2; // matching: unused; keeps the original stack layout
    u8 pad11; // matching: unused; keeps the original stack layout
    u8 pad3; // matching: unused; keeps the original stack layout
    u8 pad4; // matching: unused; keeps the original stack layout
    s32 variant;
    s32 pushAnim;
    s8 lean;
    f32 startX;
    f32 startZ;
    s32 buttons;
    OSContPad savedPad;
    s32 padOverridden;
    f32 probeTop;
    s32 mode;
    TriggerZone *climb;
    TriggerZone *climbSpot;
    f32 climbTop;
    u8 pad5; // matching: unused; keeps the original stack layout
    f32 animSpeed;
    f32 stickY;
    u32 chance;
    f32 steer;
    f32 climbRate;
    f32 swimSpeed;
    s32 turnSize;
    u16 lockAngle;
    u16 minOffset;
    u16 pad6; // matching: unused; keeps the original stack layout
    s32 anim;
    u8 turnLeft;
    f32 phase;
    u16 deflect;
    u16 quadrant;
    f32 pitchInput;
    f32 friction;
    Actor *target;
    f32 tilt;
    f32 steerDiv;
    f32 offsetX;
    f32 offsetZ;
    f32 offsetY;
    f32 pushX;
    f32 pushZ;
    f32 pushY;
    s32 climbAnim;
    f32 climbAnimSpeed;
    u16 moveDelta;
    u16 pad7; // matching: unused; keeps the original stack layout
    s16 stickX;
    u16 pad8; // matching: unused; keeps the original stack layout
    u16 pad9; // matching: unused; keeps the original stack layout
    u16 offset;
    u16 maxOffset;
    u16 centerAngle;
    s16 pad10; // matching: unused; keeps the original stack layout
    s32 climbIndex;
    s32 physics;
    f32 scratch;
    f32 nudgeX;
    f32 nudgeZ;
    f32 nudgeY;
    f32 follow;
    u8 carry;
    f32 pushScale;
    f32 pushAccel;
    f32 floorY;
    f32 snapRange;
    s32 dirMask;
    s32 lockDiff;
    s32 wantAngle;
    f32 climbInput;
    struct Sub9C *path;
    u16 dirAngle;
    u16 otherFacing;
    f32 tiltDeg;
    f32 turnLean;
    f32 prevSteer;
    s16 ledgeAngle;
    f32 height;
    f32 ledgeY;
    f32 splashSpeed;
    s32 sound;
    f32 climbSpeed;
    f32 heightDiff;

    moveSpeed = 0.0f;
    pushAnim = 0;
    lean = 0;
    D_800CC28C = D_800BE710[dc_glbl_CharNum];
    D_800CC290 = D_800CC284->button;
    D_800CC2B1 = 0;
    D_800CC2B2 = 0;
    D_800CC2B4 = 4.0f;
    prevFacing = actor->angle;
    padOverridden = 0;
    savedPad = *D_800CC284;
    D_800CC2BC = 1;
    actor->mv->unk1B3 = 0;
    if (actor->mv->bot != 0) {
        if (actor->stunned == 0) {
            func_15056B08(actor);
        }
        savedPad = *D_800CC284;
        D_800CC284->stick_x = actor->mv->stick_x;
        D_800CC284->stick_y = actor->mv->stick_y;
        D_800CC284->button = actor->mv->buttons;
        D_800CC288 = actor->mv->pressedButtons;
        buttons = actor->mv->buttons;
        if (actor->CameraPtr == NULL) {
            func_151279A0(actor, D_800CC284);
            func_15121490(actor, D_800CC284);
        }
        func_1504C854(actor);
    } else {
        actor->mv->buttons = D_800CC284->button;
        D_800CC288 = D_800BE710[dc_glbl_CharNum];
        buttons = actor->mv->buttons;
    }
    if ((D_80082FA0 >= dc_glbl_CharNum) && (D_800DDE3C[dc_glbl_CharNum] != 0) && (D_800DDDC8[dc_glbl_CharNum] > 0.75f) && (D_800E0B94 != 2)) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->button = 0;
        D_800CC288 = 0;
        padOverridden = 1;
        D_800CC2BC |= 2;
    }
    if ((actor->stunned != 0) && (actor->energy != 0) && ((((actor->mv->break_stun & 1) != 0) && (D_800CC288 & 0x8000)) || ((actor->mv->break_stun & 2) && (func_1505A5CC(D_800CC284) > 15.0f)))) {
        actor->unk10C = 0;
        actor->stunnednum = 3;
        func_150585F0(actor, 0.05f);
    }
    if (D_800BE616 != 0) {
        if (D_800BE9A0 < actor->mv->unk1B1) {
            actor->mv->unk1B1 -= D_800BE9A0;
            if (actor->fogMode != 4) {
                func_1502EA0C(actor, 0xFF, 0xFF, 0xFF, 0xBF, 4);
            }
        } else if (actor->fogMode == 4) {
            actor->mv->unk1B1 = 0;
            func_1502EA50(actor);
        }
        if (((level == 0x2D) || (level == 0x30) || (level == 0x34) || (level == 0x1A) || (level == 0x3F)) && (D_800CC288 & 4) && !(D_800CC288 & 0xC000) && ((s32) actor->shad == 0) && (actor->yspd < 0.0f) && (actor->norun == 0) && (actor->lifting == 0) && (actor->mv->GrenadeThrowState == 0) && (actor->mv->rt_context_b == 0) && ((actor->mv->context_b & ~0x80) != 3) && (actor->mv->numGrenades != 0)) {
            if ((actor->character_type == 0x3B) || (level == 0x1A) || (level == 0x3F)) {
                actor->mv->GrenadeThrowState = 5;
            } else {
                actor->mv->GrenadeThrowState = 1;
            }
        }
    }
    actor->mv->old_keys = D_800CC284->button;
    if (D_800C3638 != 0) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->button = 0;
        D_800CC288 = 0;
        D_800CC2BC |= 4;
    }
    func_1507FEA0(actor);
    func_1504BB88(actor);
    func_1504BA38(actor);
    if ((actor->unk228 != 0) || (actor->unk2F8 & 0x10)) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->button = 0;
        D_800CC288 = 0;
        padOverridden = 1;
        D_800cc2bc |= 8;
    }

    if (actor->energy == 0) {
        actor->norun = 0x14;
        if ((actor->stunned == 0) && (actor->mv->death_State == 0)) {
            if ((actor->floorMaterial & 0x1F) == 0xE) {
                func_1507CD64(actor, 0xD);
            } else if ((actor->floorMaterial & 0x1F) == 0x13) {
                func_1507CD64(actor, 0xD);
            } else {
                func_1507CD64(actor, 0);
            }

        }
    }
    if (actor->mv->rt_context_b != 0x1B) {
        if (((actor->mv->AimState == 0) || (actor->mv->rt_context_b == 3)) && (actor->mv->rt_context_b != 0x12) && (actor->mv->rt_context_b != 2) && (actor->mv->rt_context_b != 0x13) && (actor->mv->rt_context_b != 0xA) && (actor->mv->rt_context_b != 0x34)) {
            mode = actor->mv->rt_context_b;
            D_800CC2BC |= 0x10;
            moveAngle = func_1505A630(D_800CC284->stick_x, D_800CC284->stick_y, 0);
            moveSpeed = func_1505A5CC(D_800CC284);
            physics = actor->mv->transport;
            if (physics == 2) {
                angleDelta = D_800CC284->stick_x * D_800CC264;
                pitchInput = D_800CC284->stick_y;
                pitchInput *= 0.6f;
                if (pitchInput > 45.0f) {
                    pitchInput = 45.0f;
                }
                if (pitchInput < -35.0f) {
                    pitchInput = -35.0f;
                }
                if (actor->mv->unk50 > 0) {
                    actor->mv->unk50 -= D_800BE9A0;
                    angleDelta = (angleDelta / 3);
                    actor->nojump = 6;
                    pitchInput -= 60.0f;
                } else if ((actor->shad > 100.0f) || (actor->count5 != 0)) {
                    angleDelta = (angleDelta / 2);
                }
                moveSpeed = 135.0f + pitchInput;
                actor->intelhitroute = 0x29;
                if (actor->mv->bot == 0) {
                    if (actor->norun == 0) {
                        actor->angle -= angleDelta / 9;
                    } else {
                        moveSpeed = 0.0f;
                    }
                    func_15087E54(dc_glbl_CharNum, actor);
                    moveAngle = actor->angle - D_800CC280;
                }
                physics = actor->mv->transport;
            }
            if (physics == 0x81) {
                centerAngle = 0x6000;
                angleDelta = D_800CC284->stick_x * D_800CC264;
                actor->angle -= angleDelta / 8;
                if (level == 6) {
                    moveSpeed = 42.0f;
                    centerAngle = func_1505A630(2540.0f - actor->xpos, actor->zpos - -160.0f, 0);
                    maxOffset = 0xF000;
                    minOffset = 0x8000;
                } else {
                    maxOffset = 0xB000;
                    minOffset = 0x2000;
                    moveSpeed = 65.0f;
                }
                offset = (actor->angle - centerAngle);
                if (maxOffset < offset) {
                    actor->angle = centerAngle + maxOffset;
                }
                if (offset < minOffset) {
                    actor->angle = centerAngle + minOffset;
                }
                moveAngle = actor->angle - D_800CC280;
                actor->fangle = actor->angle;
                { BLK; actor->langle = actor->angle; }
            }
            if (actor->mv->SniperState != 0) {
                moveSpeed = 0.1f;
                actor->langle = (s32) (actor->mv->aimYaw * 182.04445f);
                moveAngle = actor->langle - D_800CC280;
                if (actor->mv->bot != 0) {
                    actor->angle = actor->langle;
                }
                scratch = actor->mv->aimPitch;
                if (scratch > 180.0f) {
                    scratch -= 360.0f;
                }
                scratch *= 0.91022223f;
                lean = scratch;
                if ((D_800BE616 != 0) && ((actor->character_type == 0x3B) || (actor->character_type == 0x80)) && ((mode == 0x18) || (mode == 0x41) || (mode == 0x23) || (mode == 0x24)) && (D_800CC284->button & 0x2000) && ((actor->mv->rayHit.hitType == 3) || (actor->mv->rayHit.hitType == 2))) {
                    chance = 0;
                    if (actor->mv->unkAF < 0x3C) {
                        chance = 0x30;
                    }
                    if (func_10010894(actor) == 0) {
                            if (chance >= (func_150ADA20_u8() & 0x5F)) {
                            if (actor->character_type == 0x80) {
                                if ((mode == 0x23) && !(func_150ADA20_u8() & 7)) {
                                    func_15060A30(0x586, actor);
                                } else {
                                    variant = (mode == 0x23) ? 4 : 5;
                                    func_15060A30(func_1000F568(0x574, variant), actor);
                                }
                            } else if ((D_800CC284->button & 8) && !(func_150ADA20_u8() & 1)) {
                                func_15060A30(0x532, actor);
                            } else {
                                if ((actor->mv->rayHit.hitType == 3) && (actor->mv->rayHit.part == 0xA) && !(func_150ADA20_u8() & 3)) {
                                    func_15060A30(0x533, actor);
                                } else {
                                    func_15060A30((func_150ADA20_u8() & 3) + 0x534, actor);
                                }
                            }
                        }
                    }
                    actor->mv->unkAF = 0x78;
                }
                if (actor->mv->unkAF != 0) {
                    if (D_800BE9E4 < actor->mv->unkAF) {
                        actor->mv->unkAF -= D_800BE9E4;
                    } else {
                        actor->mv->unkAF = 0;
                    }
                }
                { BLK; }
            } else {
                actor->mv->unkAF = 0;
            }
            if (actor->mv->lockout > 0) {
                actor->mv->lockout -= D_800CC264;
                moveAngle = actor->angle - D_800CC280;
            }
            if (actor->mv->bitten > 0) {
                actor->mv->bitten -= D_800CC264;
                if (D_800CC288 & 0xC000) {
                    if (actor->shad == 0.0f) {
                        D_800D1580 = 0xFF030170;
                        actor->speed *= 0.4f;
                        actor->yspd = 10.0f;
                        actor->ygrav = 5.0f;
                    }
                    actor->mv->bitten -= 0x190;
                }
                if (actor->norun == 0) {
                    phase = ((D_800CC250 * 0xC) & 0xFF) * 0.024543693f;
                    if (moveSpeed < 5.0f) {
                        moveAngle = actor->angle - D_800CC280;
                    }
                    moveSpeed = 28.0f;
                    actor->mv->turn = 4;
                    scratch = func_150AD78C(phase) * 10240.0f;
                    actor->mv->pissed_cycle += 0x10;
                    actor->mv->pissed_angle = scratch;
                }
            } else {} // matching: the empty else changes block layout
            if ((actor->animseq.uh == 0xBF) || (actor->animseq.uh == 0xC1)) {
                lean = (D_800CC284->stick_y / 2);
            }
            if (actor->mv->rt_context_b == 0x38) {
                lean -= actor->mv->gunRecoil >> 8;
            }
            { BLK; }
        } else {
            moveAngle = 0;
            moveSpeed = 0.0f;
            D_800cc2bc |= 0x20;
            if ((actor->mv->rt_context_b == 2) || (actor->mv->rt_context_b == 0x12) || (actor->mv->rt_context_b == 0x13) || (actor->mv->rt_context_b == 0x10) || (actor->mv->rt_context_b == 0xA) || (actor->mv->rt_context_b == 0x34) || ((D_800CC284->button & 0x10) && (actor->mv->rt_context_b == 0x16)) || (actor->mv->rt_context_b == 9) || (actor->mv->rt_context_b == 0x38) || (actor->mv->rt_context_b == 0x39) || (actor->mv->rt_context_b == 0x37) || (actor->mv->rt_context_b == 0x3B) || (actor->mv->rt_context_b == 0x15) || (actor->mv->rt_context_b == 0x26) || (actor->mv->rt_context_b == 0x3A)) {
                if ((actor->mv->rt_context_b == 0x10) || (actor->mv->rt_context_b == 0x14) || (actor->mv->rt_context_b == 0x3F)) {
                    lean = (D_800CC284->stick_y / 2);
                }
                if ((actor->mv->rt_context_b == 0xC) || (actor->mv->rt_context_b == 0x16) || (actor->mv->rt_context_b == 0x15) || (actor->mv->rt_context_b == 0x38) || (actor->mv->rt_context_b == 0x39) || (actor->mv->rt_context_b == 0x37) || (actor->mv->rt_context_b == 0x3B) || (actor->mv->rt_context_b == 0x12) || (actor->mv->rt_context_b == 9)) {
                    tilt = actor->mv->aimPitch;
                    if (tilt > 180.0f) {
                        tilt -= 360.0f;
                    }
                    tilt *= 0.91022223f;
                    lean = tilt;
                    if (actor->mv->rt_context_b == 0x38) {
                        lean -= actor->mv->gunRecoil >> 8;
                    }
                }
                if ((actor->mv->rt_context_b != 9) && (actor->mv->rt_context_b != 0x38) && (actor->mv->rt_context_b != 0x39) && (actor->mv->rt_context_b != 0x37) && (actor->mv->rt_context_b != 0x3B) && (actor->mv->rt_context_b != 0x15) && (actor->mv->rt_context_b != 0x26) && (actor->mv->rt_context_b != 0x3A)) {
                    actor->langle = (s32) ((actor->mv->aimYaw + 90.0f) * 182.04445f);
                    actor->angle = actor->langle;
                }
            } else if ((actor->mv->rt_context_b != 0x14) && (actor->mv->rt_context_b != 0x3F)) {
                D_800CC2BC |= 0x100;
                D_800CC284->button &= ~0x2000;
                D_800CC288 &= ~0x2000;
            }
            if (actor->mv->rt_context_b != 9) {
                D_800CC2BC |= 0x200;
                D_800CC284->button &= ~0x8000;
                D_800CC288 &= ~0x8000;
            }
        }
    }
    if (!(actor->mv->transport & 0xF) && (actor->energy != 0) && (actor->stunned == 0) && (actor->shad == 0.0f) && ((D_800BE616 != 0) || ((actor->mv->rt_context_b == 9) && (actor->mv->matrix_physics == 0)) || (actor->mv->rt_context_b == 0x16) || (actor->mv->rt_context_b == 0x3B)) && ((actor->mv->weapon_status == 2) || (actor->mv->weapon_status == 0))) { BLK;
        dirMask = ((actor->mv->rt_context_b) && ((actor->mv->SniperState == 0) || (actor->mv->rt_context_b == 0x16)) && (actor->mv->GrenadeThrowState == 0)) ? 0xF : 3;
        if ((D_800CC284->button & dirMask) != 0) {
            dirMask = (D_800CC284->button & dirMask);
            if ((actor->mv->SniperState == 0) && (actor->mv->AimState == 0)) {
                if (D_800CC284->stick_y >= 6) {
                    dirMask |= 8;
                } else if (D_800CC284->stick_y < -5) {
                    dirMask |= 4;
                }
            }
            dirAngle = D_8009917C[dirMask];
            actor->mv->use_cam_butt = 2;
            if (dirAngle != 0xFFFF) {
                if ((actor->unkAA == 0) || (actor->unkAA >= 0x42)) {
                    actor->unkAA = 0x41;
                }
                if (actor->speed > 12.0f) {
                    actor->mv->turn = 4;
                }
                if (actor->angle != actor->langle) {
                    actor->mv->turn = 0;
                }
                actor->langle -= ((D_800CC284->stick_x * D_800CC264) >> 4);
                moveAngle = ((actor->langle + dirAngle) - actor->mv->pssd_waist_tilt) - D_800CC280;
                moveSpeed = 34.0f;
                if (actor->mv->SniperState != 0) {
                    actor->angle = moveAngle + D_800CC280;
                }
                actor->mv->unk1B3 = 1;
            } else {
                actor->speed = 0.0f;
            }
        } else if (actor->mv->use_cam_butt != 0) {
            actor->angle = actor->langle;
            if (actor->mv->SniperState != 0) {
                actor->speed = 0.0f;
            }
        }
        
    }
    if (actor->mv->door_walkf_time > 0) {
        actor->norun = 0;
        actor->count5 = 0;
        moveSpeed = 18.0f;
        moveAngle = actor->angle - D_800CC280;
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->button = 0;
        D_800CC288 = 0;
        padOverridden = 1;
        actor->mv->door_walkf_time -= D_800BE9A0;
        D_800cc2bc |= 0x40;
    }
    if (actor->mv->unk6B != 0) {
        func_15052260(actor);
    }
    surfaceDelta = actor->unk118 - actor->unk11C;
    if (fabsf(surfaceDelta) > 30.0f) {
        surfaceDelta = 0.0f;
    }
    actor->unk11C = actor->unk118;
    if (actor->mv->bot == 0) {
        actor->mv->buttons = D_800CC284->button;
        actor->mv->pressedButtons = D_800CC288;
    }
    if ((actor->animseq.uh == 0) && (actor->animsp == 0.0f)) {
        func_1505E650(actor, 0x24, 1.0f, 1.0f, 0.0f, 0.0f, 0);
    }
    if (D_800CC284->stick_y == 0) {
        actor->mv->hang_ledge = 0;
    }
    if (actor->unk137 == 0xFF) {
        actor->unk137 = 0;
    }
    turnRate = 0xC;
    if ((actor->mv->pissed_up > 0) || (actor->mv->rt_context_b == 3)) {
        func_1504BE2C(actor, &moveAngle, &moveSpeed, &lean);
    }
    func_1504C9E4(actor, lean, actor->mv->rt_context_b);
    if (actor->stunned == 0) {
        if (actor->mv->bot != 0) {
            moveAngle = actor->fangle;
            moveSpeed = actor->stickspeed;
        }
        if (moveSpeed > 0.0f) {
            lockAngle = 0;
            lockDiff = 0;
            diff = 0;
            actor->mv->press_angle = moveAngle + D_800cc280;
            if (actor->mv->walk_attrib & 2) {
                lockAngle = actor->mv->unk34;
                wantAngle = actor->mv->press_angle;
                lockDiff = actor->mv->unk34 - wantAngle;
                if (!((lockDiff + 0x2000) & 0xC000)) {
                    actor->mv->press_angle = actor->mv->unk34;
                    lockAngle = actor->mv->unk34;
                    wantAngle = actor->mv->press_angle;
                    lockDiff = lockAngle - wantAngle;
                }
                if (!((lockDiff + 0xA000) & 0xC000)) {
                    actor->mv->press_angle = lockAngle + 0x8000;
                    wantAngle = actor->mv->press_angle;
                }
                moveAngle = wantAngle - D_800cc280;
            }
        } else {
            actor->mv->press_angle = actor->langle;
        }
        if (actor->inwater != 0) {
            swimSpeed = func_1504B0FC(actor, surfaceDelta);
            moveAngle = actor->mv->press_angle;
            if (actor->mv->unk22 >= 0) {
                actor->mv->unk22 -= D_800BE9A0;
                if ((actor->mv->unk22 >= 0x5B) && (actor->mv->unk22 < 0x64)) {
                    actor->mv->unk22 = 0;
                }
            }
            if (actor->mv->unk21 == 0) {
                moveSpeed = swimSpeed;
                moveAngle = actor->angle;
                if (!(D_800CC284->button & 0x4000)) {
                    if ((actor->mv->unk22 <= 0) && (((actor->unk118 - 60.0f) - 30.0f) < actor->ypos)) {
                        actor->mv->unk21 = 1;
                        actor->norun = 0xC;
                    }
                }
            } else {
                if ((D_800CC288 & 0x4000) && (D_800BE616 == 0) && (level != 0x29)) {
                    if ((actor->CameraPtr != NULL) && (actor->CameraPtr->Mode != 0x100) && !(actor->mv->water_attribs & 1)) {
                        actor->mv->unk21 = 0;
                        actor->mv->unk22 = 0x76;
                        if (((actor->unk118 - 60.0f) - 30.0f) < actor->ypos) {
                            actor->vtilt = 20.0f;
                        }
                        func_15060A30(0x66F, actor);
                    }
                }
                actor->mv->turn = 4;
                {
                BLK;
                actor->unkAA = 0x28;
                }
            }
        } else {
            moveAngle += D_800cc280;
        }
        D_800CC27E = moveAngle;
        actor->stickspeed = moveSpeed;
        if (actor->unkAA != 0) {
            moveSpeed *= actor->unkAA * 0.01f;
        }
        if (actor->lifting >= 0x64) {
            moveSpeed *= 0.8f;
        }
        if ((actor->norun != 0) || (actor->count3 >= 0x79)) {
            moveSpeed = 0.0f;
        }
        func_1504BC38(actor);
        steerDiv = 4.0f;
        steer = 0.0f;
        if ((actor->inwater != 0) && (actor->mv->unk21 == 0)) {
            if ((actor->stunned == 0) && (actor->animseq.uh != 0x262)) {
                steer = (D_800CC284->stick_x / 2);
            }
        } else {
            if ((actor->mv->rt_context_b == 2) || (actor->mv->rt_context_b == 0x13) || (actor->mv->rt_context_b == 0xA) || (actor->mv->rt_context_b == 0x34)) {
                if (actor->mv->AimState != 0) {
                    tiltDeg = -actor->mv->aimPitch;
                    if ((actor->mv->rt_context_b == 2) || (actor->mv->rt_context_b == 0x13)) {
                        tiltDeg += 30.0f;
                    } else {
                        tiltDeg = -tiltDeg;
                    }
                    { BLK; steer = (s8) tiltDeg; }
                } else {
                    steer = (D_800CC284->stick_y / 2);
                }
            } else {
                if (actor->norun != 0xFF) {
                    angleDelta = moveAngle - prevFacing;
                    if ((actor->shad < 20.0f) && (actor->speed < 1.5f)) {
                        steer = actor->stickspeed * 0.3f;
                    } else {
                        steer = actor->stickspeed * 1.75f;
                        turnLean = fabsf(angleDelta * 0.00390625f * 5.0f);
                        if (turnLean > 50.0f) {
                            turnLean = 50.0f;
                        }
                        if (turnLean < steer) {
                            steer = turnLean;
                        }
                        if (actor->shad > 30.0f) {
                            steer *= 0.6f;
                        }
                        if (angleDelta >= 0) {
                            steer = 0.0f - steer;
                        }
                    }
                }
                if (((actor->lifting != 0) && (movement_obj[D_800D154C->lifting - 100].character_type != 0x57)) || (actor->mv->AimState != 0) || (D_800CC284->button & 0x2000)) {
                    steer = 0.0f;
                }
            }
        }
        if (actor->mv->env_suit != 0) {
            if (actor->animseq.uh == 0x279) {
                if (actor->norun == 0) {
                    if (D_800CC284->button & 0x8000) {
                        actor->ygrav = 1.1f;
                    } else {
                        if (actor->yspd > 0.0f) {
                            actor->yspd *= 0.85f;
                        }
                        actor->ygrav = 2.5f;
                    }
                }
                if (actor->shad > 0.0f) {
                    actor->lean_angle += (((actor->unk54 * 4.0f) - actor->lean_angle) * 0.15f);
                    actor->vtilt = actor->speed * 1.5f;
                }
                actor->unkAA = 0x3C;
                actor->count2 = 1;
                actor->count5 = 0xFF;
            }
            steer *= 0.6f;
            steerDiv = 12.0f;
        }
        if ((actor->unk110 == 2) || (actor->animseq.uh == 0xCD)) {
            steer = -steer;
        }
        steer = (steer - actor->unk54) / (steerDiv / D_800D1550);
        if ((actor->shad < 30.0f) || (actor->mv->env_suit != 0)) {
            if (((steer > 0.0f) && (actor->mv->lean_last < 0.0f)) || ((prevSteer = actor->mv->lean_last, steer < 0.0f) && (prevSteer > 0.0f))) {
                steer = 0.0f;
            } else if (fabsf(steer - prevSteer) > 2.0f) {
                if (prevSteer < steer) {
                    steer = prevSteer + 2.0f;
                } else {
                    steer = prevSteer - 2.0f;
                }
            }
        }
        actor->mv->lean_last = steer;
        if ((D_800BEAC0 == 0) && (D_800BEA0C == 0)) {
            actor->unk54 += steer;
        }
        if (moveSpeed != 0.0f) {
            if (moveAngle != actor->angle) {
                if (actor->speed < 5.0f) {
                    turnStep = 0x19;
                } else {
                    turnStep = (((64.0f - actor->speed) * 0.25f));
                    if (actor->mv->prop >= 3) {
                        turnStep += 1;
                    } else if (actor->count3 > 0) {
                        turnStep = ((51.0f - actor->speed) / 13.0f);
                        if (actor->yspd < -10.0f) {
                            turnStep = 0;
                        }
                    }
                }
                turn = (moveAngle - actor->angle) >> 8;
                turnLeft = turn & 0x80;
                turnSize = turn;
                if (turn >= 0x80) {
                    turn = -turn;
                    turnSize = turn;
                }
                if (actor->unk102 != 0) {
                    turnStep = 5;
                } else if ((turnSize >= 0x51) && (actor->inwater == 0)) {
                    if ((actor->mv->bitten <= 0) && (actor->mv->pissed_up <= 0)) {
                        if ((actor->mv->transport & 0xF) == 1) {
                            if (actor->speed > 7.0f) {
                                turnStep = 0;
                                actor->speed -= 3.5f;
                            } else {
                                turnStep = 6;
                            }
                        } else if (actor->speed < 7.0f) {
                            if (actor->speed > 3.0f) {
                                turnStep = 0xC;
                                actor->speed = 0.0f;
                                if (actor->count5 >= 5) {
                                    if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3)) {
                                        actor->count5 = 4;
                                    }
                                }
                            }
                        } else {
                            moveSpeed = 0.0f;
                            turnStep = 0;
                            if (actor->unkAE == 0) {
                                actor->speed -= 2.0f;
                            }
                        }
                    }
                }
                turnStep = ((turnStep * D_800CC264) / 100);
                if (actor->lifting >= 0x64) {
                    turnStep = ((turnStep * 2) / 3);
                } else if (actor->unk110 != 0) {
                    turnStep = 2;
                }
                if (actor->inwater != 0) {
                    turnStep = (turnStep / 4);
                }
                if (actor->mv->turn != 0) {
                    turnStep = ((actor->mv->turn * D_800CC264) / 100);
                }
                if (turnSize < turnStep) {
                    turnStep = turnSize;
                }
                if (turnLeft) {
                    actor->angle -= (turnStep << 8);
                } else {
                    actor->angle += (turnStep << 8);
                }
            }
        }
        actor->mv->turn = 0;
        if (actor->unk110 != 0) {
            turnRate = 0x10;
            if (actor->unk110 & 1) {
                moveAngle = actor->mv->xslopeangle;
            } else {
                moveAngle = actor->mv->xslopeangle ^ 0x8000;
            }
        } else if ((moveSpeed == 0.0f) || (actor->inwater != 0) || (actor->mv->env_suit != 0) || (actor->shad < 50.0f)) {
            moveAngle = actor->angle;
        }
        turn = 0;
        if (actor->mv->matrix_physics != 0) {
            moveAngle = 0x4000;
            turnRate = 0x14;
        } else if ((actor->mv->env_suit != 0) && (actor->lifting == 0)) {
            target = func_15083E90(6);
            if (target != NULL) {
                if (func_1505A6F8(target, actor) < 600.0f) {
                    moveAngle = func_1505A630(target->xpos - actor->xpos, actor->zpos - target->zpos, 0);
                } else {
                    actor->mv->turn = 5;
                }
            }
        } else { } // matching: the empty else changes block layout
        if (actor->mv->use_cam_butt == 0) {
            turn = func_150599C8(actor, turnRate, moveAngle);
        }
        if ((turn >= 0x5B) && (actor->speed > 12.0f) && (actor->inwater == 0) && (actor->count3 == 0)) {
            moveSpeed = 0.0f;
        }
        if (actor->angle == actor->langle) {
            actor->count2 = 0;
        }
        friction = 0.0f;
        physics = actor->mv->transport;
        if ((physics & 0xF) == 1) {
            if (actor->mv->transport_surface != 0) {
                friction = 0.27f;
            } else {
                friction = 0.8f;
                if (physics == 0x81) {
                    friction = 0.2f;
                }
            }
            steer = 0.65f;
        } else {
            steer = 2.5f;
            if (actor->animseq.uh == 0x16) {
                steer = 3.2f;
                if (D_800CC284->button & 0x2000) {
                    moveSpeed = 0.0f;
                }
            } else if (actor->lifting != 0) {
                if (actor->character_type == 0x13) {
                    steer = 0.75f;
                }
                friction = 0.75f;
            } else if (actor->unk110 != 0) {
                moveSpeed = 0.0f;
                steer = 2.0f;
            } else {
                if (actor->mv->bitten > 0) {
                    friction = 4.0f;
                } else {
                    friction = 1.0f;
                }
            }
            if (actor->mv->pissed_up > 0) {
                friction = 1.0f;
                steer = 2.0f;
            }
        }
        if (actor->mv->matrix_physics != 0) {
            steer = 0.0f;
        }
        func_1505A3A8(moveSpeed, actor, friction, steer, turn);
        func_1505F298(actor, D_800CC284);
    } else {
        if (actor->inwater != 0) {
            func_1504B0FC(actor, surfaceDelta);
        }
        if (actor->stunned != 0xFF) {
            func_1506045C(actor, D_800CC284->button, D_800CC288, actor->shad, 0);
            actor->unk54 = 0.0f;
            actor->norun = 0;
            actor->count5 = 0;
            if (((actor->animseq.uh == 0xF1) || (actor->animseq.uh == 0xF6)) && (actor->shad == 0.0f)) {
                actor->yspd = 0.0f;
                actor->ygrav = 0.0f;
                actor->noshove |= 0x01008000;
            }
            if (!(actor->unk10B & 1) && (actor->speed < 2.0f) && (actor->stunned < 0xD2) && (actor->shad < 10.0f) && (actor->yspd < 0.5f)) {
                actor->stunned = 1;
                actor->unk10C = 1;
                actor->stunnednum = 3;
            }
            func_150585F0(actor, 0.1f);
            if (actor->unk10B & 0x40) {
                height = actor->ypos - D_800CC278;
                if (height < 400.0f) {
                    if (height < 10.0f) {
                        height = 10.0f;
                    }
                    actor->angle += (D_800CC264 * 0x1400) / 100;
                    actor->yspd = (height * 0.06f) + 6.0f;
                    actor->speed = 3.0f;
                    func_1505A184(actor->angle, height, 0.0f, &offsetX, &offsetZ, &offsetY);
                    actor->mv->swing_xvel = offsetX + D_800CC270;
                    actor->mv->swing_zvel = offsetZ + D_800CC274;
                    actor->mv->gpoint = 2;
                } else {
                    actor->unk10B &= ~0x40;
                    actor->speed = 30.0f;
                    actor->yspd = 90.0f;
                    actor->ygrav = 4.0f; { BLK; }
                }
            }
            if (actor->unk10B & 0x80) {
                actor->unk228 = 0;
                actor->imune = 0x5A;
                if ((actor->shad == 0.0f) && (actor->yspd < 24.0f)) {
                    actor->unk10C = 0;
                    if (actor->unk10B & 0x20) {
                        actor->mv->bitten = 0xFA0;
                    }
                }
                if (moveSpeed > 5.0f) {
                    moveAngle += D_800CC280;
                    actor->count2 = 1;
                    func_150599C8(actor, 6, moveAngle);
                    func_1505A184(moveAngle, moveSpeed, 0.0f, &pushX, &pushZ, &pushY);
                    actor->ob_overslopex += ((pushX - actor->ob_overslopex) * 0.1000000015f);
                    actor->ob_overslopey += ((pushZ - actor->ob_overslopey) * 0.1000000015f);
                    actor->angle = actor->langle;
                }
            }
        }
    }
    if ((actor->mv->rope != 0) && (actor->stunned == 0)) {
        stickX = D_800CC284->stick_x;
        stickY = D_800CC284->stick_y;
        actor->mv->prop = 0;
        actor->unk54 = 0.0f;
        actor->norun = 0xFF;
        actor->speed *= 0.95f;
        actor->yspd = 0.0f;
        actor->ygrav = 0.0f;
        actor->count3 = 1;
        if ((actor->CameraPtr != NULL) && (actor->CameraPtr->Mode == 0x100)) {
            stickY = 0.0f;
            stickX = 0;
        }
        climbAnimSpeed = stickY;
        if ((stickY == 0.0f) || (stickY < -40.0f)) {
            climbAnimSpeed = 40.0f;
            climbAnim = 0xA4;
            if (stickY < -40.0f) {
                stickY *= 2.0f;
            }
        } else {
            climbAnim = 0x16A;
            if (stickY > 0.0f) {
                climbAnim = 0x98;
            } else {
                climbAnimSpeed = -stickY;
            }
        }
        path = (Sub9C *)actor->mv->ropePtr;
        climbRate = path->unk2C;
        climbRate = 0.08f / climbRate;
        actor->mv->ropeProgress += stickY * climbRate * D_800D1550;
        if (actor->mv->ropeProgress > 1.0f) {
            actor->mv->ropeProgress = 1.0f;
        } else if (actor->mv->ropeProgress < 0.0f) {
            func_151AE264(actor);
            actor->ygrav = 5.0f;
        }
        actor->unk1CC = actor->ypos;
        actor->mv->side = 8;
        heading = actor->angle + ((stickX * D_800CC264) / 5);
        actor->fangle = heading;
        actor->langle = heading;
        actor->angle = heading;
        func_1505E650(actor, climbAnim, climbAnimSpeed * 0.025f, 6.0f, 0.0f, 0.0f, 0);
    }
    if ((actor->mv->ladder != 0) && (actor->stunned == 0)) {
        if ((actor->CameraPtr != NULL) && (actor->CameraPtr->Mode != 0x100)) {
            climbIndex = (actor->mv->ladder & ~0x80) - 1;
            climbSpeed = D_800CC284->stick_y;
            climbSpot = &zones[climbIndex];
            if ((actor->inwater != 0) && (actor->ypos < actor->unk118)) {
                actor->mv->unk96 = 8;
            }
            if (actor->mv->unk96 != 0) {
                actor->mv->unk96 -= 1;
                climbSpeed = 60.0f;
            }
            actor->mv->swing_xvel = climbSpot->x;
            actor->mv->swing_zvel = climbSpot->z;
            actor->mv->gpoint = 0xA;
            actor->mv->prop = 0;
            actor->speed = 0.0f;
            actor->unk54 = 0.0f;
            actor->norun = 0xFF;
            actor->ygrav = 0.0f;
            actor->count3 = 1;
            actor->count2 = 1;
            actor->angle = (s32) (climbSpot->yaw * 182.04445f) - 0x4000;
            actor->yspd = climbSpeed * 0.14f;
            if (actor->mv->ladderFromTop != 0) {
                actor->yspd = 0.0f;
            }
            actor->unk1CC = actor->ypos;
            actor->mv->side = 8;
        }
    }
    if (actor->mv->unk6B == 0) {
        func_15059C84(actor);
        if (actor->mv->gpoint > 0) {
            actor->mv->gpoint -= D_800BE9A0;
            actor->xpos += ((actor->mv->swing_xvel - actor->xpos) * 0.2f);
            actor->zpos += ((actor->mv->swing_zvel - actor->zpos) * 0.2f);
        }
        func_1502178C(actor, 0, -1);
        func_1505A770(actor);
        func_1505D6F0(actor, dc_glbl_CharNum);
        if ((D_800CC26C != 0) && (actor->mv->pissed_up <= 0) && (actor->lifting == 0)) {
            heading = func_1505A630(movement_obj[D_800CC26C].xpos - actor->xpos, actor->zpos - movement_obj[D_800CC26C].zpos, 0);
            
            if ((actor->shad == 0.0f) && (moveSpeed > 15.0f) && (((D_800CC27E - heading) - 0x4000) & 0x8000)) { BLK;
                follow = 0.1f;
                carry = 1;
                if (movement_obj[D_800CC26C].character_type == 0x61) {
                    if (actor->animseq.uh == 0x7A) {
                        otherFacing = movement_obj[D_800CC26C].langle;
                        diff = actor->angle - otherFacing;
                        quadrant = (diff + 0x2000) & 0xC000;
                        if (quadrant & 0x4000) {
                            movement_obj[D_800CC26C].scriptCmd = NULL;
                            movement_obj[D_800CC26C].intelwalk = 2;
                            movement_obj[D_800CC26C].angle = otherFacing + quadrant;
                            actor->angle = movement_obj[D_800CC26C].angle;
                            actor->mv->turn = 1;
                            { BLK; actor->unk54 *= 0.4f; }
                        }
                    }
                } else {
                    pushScale = 0.3f;
                    pushAccel = 0.5f;
                    heightDiff = fabsf(actor->ypos - movement_obj[D_800CC26C].ypos);
                    nudgeX = heightDiff;
                    if (heightDiff > 30.0f) {
                        nudgeX = 30.0f;
                    }
                    nudgeX = (2.0f * movement_obj[D_800CC26C].bodyRadius) - nudgeX;
                    actor->unk54 *= 0.6f;
                    actor->speed = movement_obj[D_800CC26C].stickspeed;
                    if (movement_obj[D_800CC26C].stickspeed < movement_obj[D_800CC26C].speed) {
                        nudgeZ = movement_obj[D_800CC26C].speed;
                    } else {
                        nudgeZ = movement_obj[D_800CC26C].stickspeed;
                    }
                    actor->mv->turn = ((32.0f - nudgeZ) * 0.125f);
                    if (movement_obj[D_800CC26C].character_type == 0x34) {
                        if (movement_obj[D_800CC26C].unk251 != 1) {
                            carry = 0;
                        }
                        pushAnim = 0x23;
                        actor->mv->turn = 2;
                        follow = 0.3f;
                        if (actor->speed > 5.0f) {
                            follow = 0.6f;
                        }
                    } else {
                        pushAnim = 0x18;
                    }
                    if ((pushAnim != actor->animseq.uh) && (movement_obj[D_800CC26C].character_type != 0x17)) {
                        D_800D1580 = 0xFF08012B;
                        pushAccel = 0.5f;
                        func_1506E8D8();
                    }
                    if (movement_obj[D_800CC26C].unk224 != 0) {
                        pushScale = movement_obj[D_800CC26C].unk224 * 0.01f;
                    }
                    if (carry != 0) {
                        pushAccel = 0.5f;
                        func_1505A184(actor->angle, nudgeX, 0.0f, &nudgeX, &nudgeZ, &nudgeY);
                        actor->xpos += ((((movement_obj[D_800CC26C].xpos - nudgeX) + movement_obj[D_800CC26C].unkBC) - actor->xpos) * follow);
                        actor->zpos += ((((movement_obj[D_800CC26C].zpos - nudgeZ) + movement_obj[D_800CC26C].unk148) - actor->zpos) * follow);
                        movement_obj[D_800CC26C].angle = actor->angle;
                    }
                    movement_obj[D_800CC26C].unk87 = 1;
                    nudgeX = actor->stickspeed * pushScale;
                    if (movement_obj[D_800CC26C].character_type == 0x25) {
                        nudgeX = actor->stickspeed * 0.6f;
                        actor->speed += 2.0f;
                        pushAccel = 1.0f;
                        actor->mv->turn += 3;
                    }
                    if (movement_obj[D_800CC26C].character_type != 0x34) {
                        if (movement_obj[D_800CC26C].speed < nudgeX) {
                            movement_obj[D_800CC26C].speed += (pushAccel * D_800BE9A4);
                        }
                    } else {
                        actor->speed = movement_obj[D_800CC26C].speed;
                    }
                }
            }
        }
    }
    if ((D_800BE616 != 0) && (D_800CC268 != 0) && (actor->mv->on_fire != 0)) {
        func_15063168(actor);
    }
    actor->theta = (actor->langle + 0x4000) * 0.005493164f;
    func_1502178C(actor, 0, -1);
    startX = actor->xpos;
    startZ = actor->zpos;
    if (!(actor->noshove & 0x8000)) {
        func_15055E50(actor, actor->family);
        if (actor->mvmt_control & 0x100000) {
            probeTop = 30000.0f;
        } else {
            probeTop = actor->old_ypos;
        }
        if ((actor->unk118 != -10000.0f) || ((actor->noshove & 2) == 0) || ((func_150A3FC4(actor->xpos, actor->zpos, actor->floorTriVerts, actor->floorTri.v, &floorY) == 0))) {
            func_1510E8BC(&actor->floorTriVerts, actor->floorTri.v, &floorY, &actor->unk118, &actor->floorMaterial, &actor->floorType, actor->xpos, actor->ypos, actor->zpos, probeTop, 0, actor, -10000.0f, actor->ypos, actor->noshove & 0x04000000);
            if (actor->unk118 < actor->unk120) {
                actor->unk118 = actor->unk120;
            }
        }
        if (actor->ypos < floorY) {
            snapRange = 30.0f;
        } else {
            snapRange = 15.0f;
        }
        if ((actor->noshove & 4) && (actor->mv->side <= 0) && (actor->mv->unk6B == 0) && (actor->unk137 == 0) && (actor->attach_char == 0) && ((actor->mv->bot == 0) || (actor->xpos != actor->old_xpos) || (actor->ypos != actor->old_ypos) || (actor->zpos != actor->old_zpos))) {
            if (actor->noshove & 0x08000000) {
                D_800CBDD3 = 1;
            }
            if (actor->mv->transport == 2) {
                D_800CBDD4 = 1;
            }
            func_15044380(actor->old_xpos, actor->old_ypos, actor->old_zpos, actor, 1, 1);
            D_800CBDD4 = 0;
            D_800CBDD3 = 0;
        } else {
            D_800CBDF4 = D_800CBDF8 = -32768.0f;
        }
        if ((startX != actor->xpos) || (startZ != actor->zpos)) {
            scratch = -10000.0f;
            actor->mvmt_control |= 0x10000;
            if ((scratch != actor->unk118) || (floorY != scratch) || !(actor->noshove & 2) || (func_150A3FC4(actor->xpos, actor->zpos, actor->floorTriVerts, actor->floorTri.v, &floorY) == 0)) {
                func_1510E8BC(&actor->floorTriVerts, actor->floorTri.v, &floorY, &actor->unk118, &actor->floorMaterial, &actor->floorType, actor->xpos, actor->ypos, actor->zpos, probeTop, 0, actor, scratch, actor->ypos, actor->noshove & 0x04000000);
                if (actor->unk118 < actor->unk120) {
                    actor->unk118 = actor->unk120;
                }
            }
            heading = func_1505A630(actor->xpos - startX, startZ - actor->zpos, 0);
            moveDelta = actor->angle - heading;
            if (actor->mv->transport == 2) {
                if ((moveDelta >= 0x1E01) && (moveDelta < 0xE200)) {
                    dirAngle = actor->angle - heading - 0x8000;
                    if (dirAngle & 0x8000) {
                        deflect = (dirAngle >> 2) + 0x9800;
                    } else {
                        deflect = (dirAngle >> 2) + 0x2800;
                    }
                    actor->angle = heading - deflect;
                    if (actor->mv->camjolt <= 0) {
                        if ((actor->energy == 1) || ((moveDelta >= 0x5E01) && (moveDelta < 0xA200))) {
                            actor->energy = 0;
                            func_150D5A6C(actor, 0xFF, 1);
                            func_1507CD64(actor, 8);
                        } else if (actor->energy != 0) {
                            if (D_800BE616 == 0) {
                                D_800D1580 = 0xFF0201D6;
                                func_1506E9D8();
                            }
                            actor->energy -= 1;
                        }
                        actor->mv->camjolt = 0x14;
                        if (moveDelta >= 0x5E01) {} // matching: dead test the original evaluates
                    }
                }
                if (actor->speed > 30.0f) {
                    actor->speed *= 0.7f;
                    actor->norun = 0xC;
                }
                if ((actor->energy != 0) && (func_100107F8(actor) == 0)) {
                    D_800D1580 = 0xFF08030A;
                    func_1506E5FC();
                }
            } else {
                turnSize = actor->inwater;
                if (turnSize == 0) {
                    if (actor->shad > 60.0f) {
                        if ((moveDelta >= 0x5E01) && (moveDelta < 0xA200) && (turnSize != 0x64) /* matching: always true here, still emitted */ && (actor->speed > 14.0f)) {
                            actor->speed = 14.0f;
                        }
                    } else if ((actor->mv->env_suit == 0) && (actor->mv->unk1B3 == 0) && (actor->lifting == 0) && (moveDelta >= 0x5E01) && (moveDelta < 0xA200) && (((scratch = actor->speed) > 10.0f) || ((actor->mv->duck_tilt == 0) && (scratch > 2.0f)))) {
                        if (scratch > 14.0f) {
                            actor->speed = 14.0f;
                        }
                        if ((actor->mv->transport == 0) && (actor->mv->weapon_status == 0)) {
                            pushAnim = 0x7A;
                            if ((actor->count3 == 0) && (actor->count5 == 0)) {
                                actor->theta = (heading - 0x4000) * 0.005493164f;
                            }
                        }
                    }
                }
            }
        }
        D_800CC27C = (actor->floorMaterial >> 28) & 7;
        if (D_800CC27C >= 7) {
            D_800CC27C = 0;
        }
        actor->floorY = floorY;
        actor->mv->transport_surface = 0;
        physics = actor->mv->transport;
        if ((physics & 0xF) == 1) {
            if (actor->ygrav < 1.0f) {
                actor->ygrav = 5.0f;
            }
            if ((floorY + 45.0f) < actor->unk118) {
                actor->mv->transport_surface = 1;
                snapRange = 0.0f;
            }
        } else if (physics == 2) {
            if (floorY == -10000.0f) {
                actor->energy = 0;
                func_150D5A6C(actor, 0xFF, 1);
                func_1507CD64(actor, 8);
            }
            if (actor->mv->unk50 <= 0) {
                func_1504CA60(actor);
            }
            actor->unkCC = 0x1E;
            actor->unkCE = actor->unkCC;
            if ((D_800BE9E4 < actor->mv->unk12C) && (actor->energy != 0)) {
                actor->mv->unk12C -= D_800BE9E4;
                actor->speed = 254.0f;
            } else {
                actor->mv->unk12C = 0;
            }
            if ((actor->mv->pressedButtons & 0x2000) && (actor->energy != 0)) {
                func_150D7790(actor, actor->mv->context_b & 0x7F);
            }
        }
        if ((actor->ypos < (floorY + snapRange)) && (actor->yspd < 0.0f) && ((snapRange > 20.0f) || (actor->shad == 0.0f))) {
            actor->ypos = floorY - 1.0f;
        }
        if ((actor->mv->transport == 2) || (actor->mv->matrix_physics != 0)) {
            func_1505B5F8(actor, actor->floorY);
        } else {
            func_1505A9AC(actor, actor->floorY);
        }
        if (actor->mv->ladder != 0) {
            actor->vtilt = 0.0f;
            if (actor->stunned == 0) {
                if ((actor->CameraPtr != NULL) && (actor->CameraPtr->Mode != 0x100)) {
                    climbInput = D_800CC284->stick_y;
                    climbIndex = (actor->mv->ladder & ~0x80) - 1;
                    climb = &zones[climbIndex];
                    if (actor->mv->unk96 != 0) {
                        climbInput = 60.0f;
                    }
                    climbTop = (climb->y + climb->ySize);
                    animSpeed = (climbInput * 0.025f);
                    actor->unk1CC = actor->ypos;
                    if (actor->mv->ladderFromTop != 0) {
                        actor->count5 = 0xFF;
                        actor->norun = 0xC;
                        animSpeed = 1.0f;
                        actor->unk100 |= 0x20;
                        anim = 0x31A;
                    } else {
                        if ((climbTop <= actor->ypos) && (climbInput >= 0.0f)) {
                            animSpeed = 1.0f;
                            anim = 0x206;
                        } else {
                            anim = 0x297;
                            if (climbInput >= 0.0f) {
                                anim = 0x193;
                            } else {
                                animSpeed = -(climbInput * 0.025f);
                            }
                        }
                        actor->floorMaterial &= ~0x1F;
                        if (level == 0x23) {
                            actor->floorMaterial |= 9;
                        } else {
                            actor->floorMaterial |= 3;
                        }
                        if ((climbTop - 80.0f) <= actor->ypos) {
                            if (!(actor->mv->ladder & 0x80)) {
                                actor->unk137 = 2;
                                animSpeed = 3.0f;
                                anim = 0x262;
                                actor->mv->ladder = 0;
                            } else if (climbTop <= actor->ypos) {
                                animSpeed = 1.0f;
                                anim = 0x206;
                                actor->yspd = 0.0f;
                                actor->ypos = climbTop;
                            }
                        } else if (actor->ypos < climb->y) {
                            actor->norun = 0;
                            actor->mv->ladder = 0;
                            actor->ygrav = 4.0f;
                        }
                        if ((actor->mv->ladder == 0) && (actor->inwater != 0)) {
                            actor->ygrav = 0.0f;
                        }
                    }
                    if (anim != -1) {
                        func_1505E650(actor, anim, animSpeed, 6.0f, 0.0f, 0.0f, 0);
                    }
                }
            }
            if (actor->shad == 0.0f) {
                if (actor->mv->ladderFromTop == 0) {
                    actor->mv->ladder = 0;
                    actor->norun = 6;
                    actor->ygrav = 4.0f;
                }
            }
        }
    }
    if ((actor->shad == 0.0f) && (actor->mv->transport != 2) && (actor->energy != 0) && ((actor->floorMaterial & 0x1F) == 0xE)) {
        func_1505D024(actor, 1, actor->langle, -1);
        actor->unk10B |= 0x80;
    }
    if ((actor->floorY < -9000.0f) && (actor->yspd < -120.0f)) {
        actor->mv->over_edge += D_800BE9A0;
        if (actor->mv->over_edge >= 0x24) {
            func_1506D538();
        }
    } else {
        actor->mv->over_edge = 0;
    }
    func_15051558(actor, startX, startZ, moveAngle);
    if (actor->mv->transport == 0) {
        if ((actor->shad > 5.0f) && (actor->unk137 == 0) && (actor->norun == 0) && (actor->mv->rt_context_b == 0) && (actor->mv->env_suit == 0) && (actor->stunned == 0) && (actor->mv->bitten == 0) && (actor->inwater == 0) && (actor->lifting == 0) && (actor->floorY != -10000.0f) && (actor->energy != 0) && (actor->mv->pissed_up == 0) && (actor->mv->transport == 0) && (actor->unk110 == 0) && ((actor->unk1CC - actor->ypos) < 450.0f)) {
            anim = 0;
            if (D_800CBDF4 != -32768.0f) {
                ledgeAngle = (((actor->theta + D_800CBD94) - 90.0f) / 1.40625f);
                ledgeY = D_800CBDF4;
                anim = 1;
            } else if ((D_800CBDF8 != -32768.0f) && (actor->speed < 10.0f)) {
                ledgeAngle = ((actor->theta + D_800CBD98 + 90.0f) / 1.40625f);
                ledgeY = D_800CBDF8;
                anim = 2;
            }
            if (anim != 0) {
                actor->angle = (ledgeAngle << 8);
                actor->langle = actor->angle;
                actor->ypos = ledgeY - 25.0f;
                actor->xpos = D_800CBDFC;
                actor->zpos = D_800CBD90;
                actor->mv->hang_ledge = 1;
                actor->mv->save_done = 0;
                actor->norun = 0xFF;
                actor->unk137 = 1;
                actor->count3 = 1;
                actor->speed = 0.0f;
                actor->yspd = 0.0f;
                actor->ygrav = 0.0f;
                actor->unk1CC = actor->ypos;
                func_1505E650(actor, 0x42, 1.0f, 1.0f, 0.0f, 0.0f, 0);
                actor->mv->camjolt = 0xC;
            }
        }
        if (actor->yspd < 5.0f) {
            if ((actor->unk118 != -10000.0f) && (actor->ypos < (actor->unk118 - 60.0f)) && (actor->inwater != 1)) {
                splashSpeed = 4.0f;
                actor->mv->save_done = 0;
                actor->mv->prop = 0;
                actor->inwater = 1;
                actor->ob_slopex = 0.0f;
                actor->ob_slopey = 0.0f;
                actor->unk162 = 0;
                actor->unk1CC = actor->ypos;
                actor->speed *= 0.5f;
                actor->angle = actor->langle;
                actor->mv->unk22 = 0;
                if (D_800BE9B4 == 0) {
                    actor->mv->unk21 = 1;
                    actor->unkB2 = 0;
                }
                if ((actor->mv->water_attribs & 1) && (D_800BE616 == 0)) {
                    func_15083568(actor, 0x38, 0.0f, 0);
                    splashSpeed = 4.0f;
                }
                if (actor->unk110 == 2) {
                    actor->angle ^= 0x8000;
                    actor->langle = actor->angle;
                    actor->speed = 20.0f;
                    actor->vtilt = -30.0f;
                }
                actor->unk110 = 0;
                if ((actor->animseq.uh == 0x3C) || (actor->animseq.uh == 0x3B)) {
                    splashSpeed = 0.0f;
                    actor->yspd *= 0.5f;
                    actor->ygrav = -2.4f;
                    actor->vtilt = 89.0f;
                } else {
                    actor->yspd *= 0.4f;
                    actor->ygrav = -3.0f;
                }
                actor->stunned = 0;
                actor->wantspeed = 0.0f;
                actor->count3 = 0;
                actor->unkA9 = 0;
                if ((actor->mv->ladder == 0) && (actor->energy != 0)) {
                    func_1506B100(0xD1, 2.0f, splashSpeed);
                    actor->norun = 0;
                }
                if (actor->energy == 0) {
                    actor->norun = 0xFF;
                    func_1507CD64(actor, 4);
                    actor->stunned = 0xFF;
                }
            }
        }
    }
    func_1505250C(actor, 1);
    if (actor->unkCC < -0x5D) {
        actor->energy = 0;
        func_1507CD64(actor, 3);
    }
    if (actor->doing_hit != 0) {
        func_1505C7D8(actor, dc_glbl_CharNum);
    }
    if (actor->count1 >= 9) {
        actor->count2 = 1;
    }
    if (actor->count5 != 0) {
        if (actor->count5 != 0xFF) {
            if (D_800BE9A0 >= actor->count5) {
                actor->count5 = 0;
            } else {
                actor->count5 -= D_800BE9A0;
            }
        }
        if ((actor->animseq.uh == 0x36) || (actor->animseq.uh == 0x19D)) {
            if ((actor->speed > 8.0f) && (actor->norun == 0)) {
                actor->count5 = 0;
                moveSpeed = actor->speed;
            }
        }
    } else {
        actor->unkAA = 0;
    }
    if (actor->unkA8 != 0) {
        actor->unkA8 -= 1;
    }
    if (actor->unk102 != 0) {
        actor->unk102--;
        actor->ygrav = 5.0f;
        if (actor->unk102 == 0) {
            func_1506AF74();
        }
    }
    if (actor->unk100 & 1) {
        if (actor->mv->rt_context_b == 0) {
            if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3) && (actor->inwater == 0) && (actor->stunned == 0) && (D_800CC2B1 == 0) && ((actor->norun == 0) || (actor->animseq.uh == 0x279)) && (actor->mv->pissed_up <= 0) && (actor->shad == 0.0f) && (D_800CC284->button & 0x2000)) {
                actor->count3 = 0;
                actor->count5 = 0;
                actor->unk100 &= 0xFE;
                actor->angle = actor->langle;
            }
        }
    }
    if (((actor->unk1CC - actor->ypos) > 600.0f) && (actor->mv->death_State == 0)) {
        if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3) && (actor->unk118 == -10000.0f)) {
            if ((actor->animseq.uh != 0x101) && (actor->inwater == 0)) {
                if (actor->animseq.uh != 0xCF) {
                    sound = 0x2007E5;
                    if (func_1506E46C(actor, &sound, 1) != 0) {
                        func_15060A30(sound, actor);
                    }
                    { BLK; }
                }
                actor->mv->prop = 0;
                func_1505E650(actor, 0xCF, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                actor->count5 = 0xC8;
            }
        }
    }
    actor->mv->duck_tilt = 0;
    func_1504C0E8(actor, pushAnim, moveSpeed);
    actor->mv->actual_speed = 2.0f * sqrtf(((actor->xpos - actor->old_xpos) * (actor->xpos - actor->old_xpos)) + ((actor->zpos - actor->old_zpos) * (actor->zpos - actor->old_zpos)));
    actor->mv->walk_attrib = 0;
    if ((actor->mv->bot != 0) || (padOverridden != 0)) {
        *D_800CC284 = savedPad;
    }
    actor->mv->prevButtons = buttons;
}

/* Tilts the actor toward the slope of the ground under it (pitch/roll and the unk164/unk168 lean). */
void func_150511E8(Actor *actor) {
    f32 slopeX;
    f32 slopeZ;
    f32 normalX;
    f32 normalZ;
    f32 leanRate;

    if (actor->shad <= 0.0f) {
        s32 clamped;
        f32 gain;
        f32 damping;
        f32 scale;
        f32 rise;
        f32 roll;

        gain = 0.6999999881f;
        damping = 0.3000000119f;
        scale = 35.0f;
        // matching: a literal spelled unlike gain's is not CSE'd with it by IDO
        rise = 0.69999998807907104f;
        if ((actor->character_type == 0x28) || (actor->character_type == 0x77)) {
            gain = 0.8999999762f;
            damping = 0.200000003f;
            scale = 7.0f;
            rise = 0.6000000238f;
        }
        func_1510F820(actor->floorTriVerts, actor->floorTri.v, &normalX, &normalZ, &leanRate, &slopeX, &slopeZ);
        if (actor->noshove & 0x100000) {
            f32 pitch;

            func_1510F820(actor->floorTriVerts, actor->floorTri.v, &normalX, &normalZ, &leanRate, &slopeX, &slopeZ);
            roll = func_1505210C(actor, slopeX, slopeZ, &pitch, 50.0f);
            pitch *= gain;
            actor->vtilt += (pitch - actor->vtilt) * damping;
            actor->lean_angle += (roll * gain - actor->lean_angle) * damping;
        }
        if (!(actor->noshove & 0x80000)) {
            return;
        }
        if (actor->unk226 != 0) {
            scale = actor->unk226;
        }
        if ((fabsf(slopeX) < 0.09000000358f) && (fabsf(slopeZ) < 0.09000000358f)) {
            slopeX = 0.0f;
            leanRate = 0.200000003f;
            slopeZ = 0.0f;
        } else {
            f32 steepness;

            clamped = 0;
            if (fabsf(slopeX) > 1.0f) {
                // matching: 1.00000000f (not 1.0f) keeps IDO from reusing the comparison's 1.0f register
                if (slopeX < 0.0f) {
                    slopeX = -1.0f;
                } else {
                    slopeX = 1.00000000f;
                }
                clamped = 1;
            }
            if (fabsf(slopeZ) > 1.0f) {
                if (slopeZ < 0.0f) {
                    slopeZ = -1.0f;
                } else {
                    slopeZ = 1.00000000f;
                }
                clamped = 1;
            }
            if (clamped && (actor->character_type == 0x77)) {
                if (actor->speed > 15.0f) {
                    actor->speed *= 0.8000000119f;
                }
                if (actor->norun < 8) {
                    actor->norun = 6;
                }
            }
            steepness = sqrtf(slopeX * slopeX + slopeZ * slopeZ);
            slopeX *= scale;
            slopeZ *= scale;
            leanRate = steepness * rise + 0.05000000075f;
        }
    } else {
        slopeZ = 0.0f;
        slopeX = 0.0f;
        leanRate = 0.1000000015f;
    }
    func_1505A250(-slopeX, -slopeZ, leanRate, &actor->ob_slopex, &actor->ob_slopey);
}


/* Ground/water tilt for a player-like actor: slides on steep slopes, then eases roll (unkC4) and pitch (unkB8). */
void func_15051558(Actor *actor, f32 x, f32 z, u16 facing) {
    f32 roll;
    f32 pitch;
    f32 blend;
    f32 height;
    u8 bumped;

    roll = 0.0f;
    bumped = 0;
    blend = 0.200000003f;
    height = actor->shad;
    pitch = 0.0f;
    if (actor->inwater == 0) {
        s32 wasSliding;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 moved;
        f32 normalX;
        f32 normalZ;
        f32 leanRate;
        u16 moveAngle;
        f32 animSpeed;
        u16 heading;
        f32 sway;
        s16 shake;
        s32 kind;
        u16 anim;

        wasSliding = actor->unk162;
        normalX = 0.0f;
        normalZ = 0.0f;
        leanRate = 3.0f;
        if (actor->mv->transport == 2) {
            func_1510F820(actor->floorTriVerts, actor->floorTri.v, &normalX, &normalZ, &leanRate, &roll, &pitch);
            if ((((roll * roll) + (pitch * pitch)) > 1.39999998f) && ((roll * roll) + (pitch * pitch) < 1000.0f)) {
                actor->nojump = 6;
                if ((height < 80.0f) && (actor->norun == 0)) {
                    func_15087EF0(dc_glbl_CharNum, actor);
                }
            }
            if (height >= 0.0f) {
                normalX = 0.0f;
                normalZ = 0.0f;
                roll = 0.0f;
                pitch = 0.0f;
                leanRate = 3.0f;
            }
        } else if (((height <= 0.0f) || (actor->unk110 != 0)) && (actor->mv->transport_surface == 0)) {
            func_1510F820(NULL, actor->floorTri.v, &normalX, &normalZ, &leanRate, &roll, &pitch);
        }
        if ((actor->mv->transport == 0) && (actor->mv->bot == 0) &&
            (((D_800CC27C == 0) && ((((roll * roll) + (pitch * pitch)) > 1.0f) || (((actor->floorMaterial >> 12) & 0xF) == 4))) ||
             ((D_800CC27C != 0) && (actor->mv->env_suit != 0))) &&
            (((actor->floorMaterial >> 12) & 0xF) != 3)) {
            D_800CC27C = 1;
            bumped = 1;
        }
        kind = (actor->stand_on_char != 0) ? movement_obj[actor->stand_on_char - 1].character_type : -1;
        if (D_800CC27C == 0) {
            normalX = 0.0f;
            normalZ = 0.0f;
            leanRate = 3.0f;
        }
        switch (kind) {
        case 8:
            bumped = 0;
            break;
        case 0x49:
            bumped = 0;
            break;
        }
        if ((D_800CC27C != 0) && (actor->shad < 5.0f) && (actor->unk110 != 0)) {
            actor->unk162 = 1;
            if (actor->norun != 0xFE) {
                actor->norun = D_800BE9A0 + 1;
            }
        } else {
            actor->unk162 = 0;
        }
        dx = x - actor->old_xpos;
        dy = actor->ypos - actor->old_ypos;
        dz = z - actor->old_zpos;
        moved = 2.0f * sqrtf((dx * dx) + (dy * dy) + (dz * dz)) * D_800BE9A8;
        if (actor->unk162 - wasSliding) { // matching: the original tests the difference (subu + bnez), not !=
            if (actor->unk162 != 0) {
                actor->ob_slopex = dx * D_800BE9A8;
                actor->ob_slopey = dz * D_800BE9A8;
                actor->speed = 0.0f;
            } else {
                actor->speed = moved;
                if (moved < 22.0f) {
                    actor->speed = 22.0f;
                }
                if (actor->unk110 == 3) {
                    actor->unk110 = 0;
                    if (actor->norun != 0xFE) {
                        actor->norun = 0;
                    }
                    actor->speed = 17.8999996f;
                }
                actor->angle = func_1505A630(x - actor->old_xpos, actor->old_zpos - z, 0);
                actor->ob_slopex = 0.0f;
                actor->ob_slopey = 0.0f;
                actor->ob_overslopex = 0.0f;
                actor->ob_overslopey = 0.0f;
            }
        }
        if ((D_800CC27C != 0) && (actor->shad < 5.0f)) {
            animSpeed = 1.0f;
            actor->nojump = D_800BE9A0 + 8;
            actor->unk112 = func_1505A630(-normalX, normalZ, 0);
            moveAngle = func_1505A630(actor->xpos - actor->old_xpos, actor->old_zpos - actor->zpos, 0);
            D_800CC27C = 1;
            if (actor->unk110 == 0) {
                leanRate = D_800990C0[D_800CC27C];
                if ((u8)(((actor->unk112 - moveAngle) >> 8) + 0x30) < 0x60) {
                    heading = (actor->stickspeed > 8.0f) ? facing : actor->langle;
                    if (bumped != 0) {
                        actor->unk110 = 3;
                        anim = 0x28E;
                        animSpeed = 2.0f;
                    } else {
                        anim = 0xD0;
                        if ((u8)(((actor->unk112 - heading) >> 8) + 0x58) < 0xB0) {
                            actor->unk110 = 1;
                            anim = 0x101;
                        } else {
                            actor->unk110 = 2;
                        }
                    }
                    if ((actor->stunned == 0) && (actor->lifting == 0) && (actor->norun != 0xFF) && (actor->count5 < 0xFA)) {
                        if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3) && (actor->mv->death_State == 0) && (actor->mv->GrenadeThrowState == 0)) {
                            func_1505E650(actor, anim, animSpeed, 6.0f, 0.0f, 0.0f, 0);
                        }
                    }
                }
            } else {
                leanRate = D_800990E0[D_800CC27C];
            }
        }
        func_1505A250(-normalX, -normalZ, leanRate, &actor->ob_slopex, &actor->ob_slopey);
        actor->ob_overslopex += (0.0f - actor->ob_overslopex) * 0.150000006f;
        actor->ob_overslopey += (0.0f - actor->ob_overslopey) * 0.150000006f;
        if (actor->unk110 != 0) {
            actor->mv->xslopeangle = func_1505A630(actor->xpos - actor->old_xpos, actor->old_zpos - actor->zpos, 0);
            if ((actor->animseq.uh == 0x28E) || (actor->animseq.uh == 0x101) || (actor->animseq.uh == 0xD0)) {
                if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3)) {
                    actor->count5 = D_800BE9A0 + 1;
                }
            }
            if (D_800CC27C == 0) {
                if (actor->unk110 == 2) {
                    actor->mv->xslopeangle = actor->angle = actor->unk112;
                    if ((actor->speed < 2.0f) && (actor->shad == 0.0f)) {
                        if (actor->stunned == 0) {
                            func_1505E650(actor, 0xA5, 1.39999998f, 5.0f, 0.0f, 0.0f, 0);
                            actor->count5 = 0x3C;
                            actor->norun = 0x3C;
                            actor->nojump = 0x1E;
                            actor->angle ^= 0x8000;
                            actor->langle = actor->angle;
                            actor->unk100 |= 0x20;
                        }
                        actor->unk110 = 0;
                    }
                } else if (actor->speed < 12.0f) {
                    actor->unk110 = 0;
                }
            }
        }
        if (height <= 0.0f) {
            roll = func_1505210C(actor, roll, pitch, &pitch, 50.0f);
            if (actor->mv->transport != 2) {
                if ((actor->mv->duck_tilt == 0) && (actor->unk110 == 0) && ((actor->mv->transport & 0xF) != 1)) {
                    roll *= (actor->mv->actual_speed * 0.00899999961f) + 0.200000003f;
                    pitch *= (actor->mv->actual_speed * 0.0140000004f) + 0.200000003f;
                } else {
                    roll *= 0.800000012f;
                    pitch *= 0.800000012f;
                    if ((actor->mv->transport & 0xF) == 1) {
                        roll *= 0.5f;
                        pitch = 0.0f;
                    }
                }
            }
        }
        if (actor->mv->transport == 2) {
            {
                f32 yVelocity = actor->yspd;
                sway = actor->unk54 * 6.0f;
                if (yVelocity < -30.0f) {
                    pitch -= -30.0f;
                } else {
                    pitch -= yVelocity;
                }
            }
            blend = 0.400000006f;
            if (actor->mv->camjolt > 0) {
                shake = (D_800CC250 & 4);
                shake -= 2;
                shake *= (u32)(actor->mv->camjolt * 2); // matching: unsigned multiply (multu)
                sway += shake;
                blend = 0.25f;
            }
            if ((fabsf(actor->lean_angle) > 70.0f) && (D_800BE616 == 0) && (actor->energy != 0) && (actor->mv->gun_before_reload == 0)) {
                D_800D1580 = 0xFF020146;
                func_1506E8D8();
                actor->mv->gun_before_reload = 1;
            }
            if (fabsf(actor->lean_angle) < 20.0f) {
                actor->mv->gun_before_reload = 0;
            }
            roll += sway;
        }
        actor->vtilt += (pitch - actor->vtilt) * blend;
    } else {
        f32 depth;

        depth = (actor->unk118 - 30.0f) - actor->ypos;
        depth += actor->speed * 6.0f;
        if (depth > 250.0f) {
            depth = 2.5f;
        } else {
            depth *= 0.00999999978f;
        }
        if ((fabsf(actor->vtilt) < 35.0f) && (depth > 0.0f)) {
            roll += actor->unk54 * depth;
        }
        func_1505A250(0.0f, 0.0f, 0.300000012f, &actor->ob_slopex, &actor->ob_slopey);
    }
    actor->lean_angle += (roll - actor->lean_angle) * blend;
}
/* Turns a surface slope into roll/pitch angles (degrees) in the actor's frame, clamped to +-limit. */
f32 func_1505210C(Actor *actor, f32 slopeX, f32 slopeZ, f32 *pitch, f32 limit) {
    f32 cosYaw;
    s32 pad; // matching: keeps the stack frame at the original size
    f32 yaw;
    f32 sinYaw;
    f32 localX;
    f32 minLimit;

    slopeZ = -slopeZ;
    yaw = actor->theta * 0.01745329238f;
    cosYaw = func_150AD780(yaw);
    sinYaw = func_150AD78C(yaw);
    localX = (slopeX * cosYaw) + (slopeZ * sinYaw);
    slopeZ = (-slopeX * sinYaw) + (slopeZ * cosYaw);
    slopeX = func_150484A0(localX, 1.0f) * 57.29577637f;
    slopeZ = func_150484A0(slopeZ, 1.0f) * 57.29577637f;
    if (slopeX >= 180.0f) {
        slopeX -= 360.0f;
    }
    if (slopeZ >= 180.0f) {
        slopeZ -= 360.0f;
    }
    minLimit = -limit;
    if (slopeX > limit) {
        slopeX = limit;
    }
    if (slopeX < minLimit) {
        slopeX = minLimit;
    }
    if (slopeZ > limit) {
        slopeZ = limit;
    }
    if (slopeZ < minLimit) {
        slopeZ = minLimit;
    }
    *pitch = slopeZ;
    return slopeX;
}
