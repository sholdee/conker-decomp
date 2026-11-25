#include <ultra64.h>

// matching: func_150ADA20 returns a u32 random number, but this file was built against a u8-returning
// prototype (callers use the low byte without masking it), so it keeps that view.
#define func_150ADA20 func_150ADA20_functions_h
#include "functions.h"
#undef func_150ADA20
u8 func_150ADA20(void);
#include "variables.h"

void func_1502178C(Actor *arg0, s32 arg1, s32 arg2);
void func_15056B08(Actor *arg0);
void func_150585F0(Actor *arg0, f32 arg1);

void func_1510F820(void *, void *, f32 *, f32 *, f32 *, f32 *, f32 *);
f32 func_1505210C(Actor *arg0, f32 arg1, f32 arg2, f32 *arg3, f32 arg4);
void func_1505A250(f32, f32, f32, f32 *, f32 *);
void func_15060A30(s32, void *);
void func_1507CD64(void *, s32);
f32 func_1504B0FC(Actor *arg0, f32 arg1);
void func_1504BA38(Actor *arg0);
void func_1504BB88(Actor *arg0);
void func_1504BC38(Actor *arg0);
void func_1504BE2C(Actor *arg0, u16 *arg1, f32 *arg2, s8 *arg3);

extern u8 D_800CC2B1;
extern u16 D_800CC2B2;
extern f32 D_800CC2B4;
void func_1504C0E8(Actor *arg0, s32 arg1, f32 arg2);
void func_1504C9E4(Actor *arg0, s8 arg1, u8 arg2);

void func_1504CA60(Actor *actor) {
    f32 range;
    f32 height;

    height = actor->floorY + 20.0f;
    actor->unk1CC = actor->y_position;
    range = 18.0f;
    if (actor->unk28 > 30.0f) {
        range = 80.0f;
    }
    if (actor->unk81 != 0) {
        range = 80.0f;
        height -= 30.0f;
        if ((actor->unk28 < 5.0f) && (actor->y_velocity < 0.0f)) {
            actor->unk81 = 0;
            actor->y_velocity = 45.0f;
        }
    } else if (actor->unk28 < 20.0f) {
        if ((func_150ADA20() & 0x3F) == 0) {
            actor->y_velocity = 25.0f;
        }
    }
    func_15058EA4(actor, height, 3.8f, height, -3.5f, 80.0f, -range);
    actor->unk80 = 1;
}

/* The part of the rope (ActorCtl.rope) read here; game_1D9F10.c has the full Rope. */
typedef struct Sub9C {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ f32 unk2C;
} Sub9C;




typedef struct {
    /* 0x0 */ u16 buttons;
    /* 0x2 */ s8 stick_x;
    /* 0x3 */ s8 stick_y;
    /* 0x4 */ u16 unk4;
} ContPad;

// variables.h declares D_800CC284 as s32; it points at the current player's controller input.
#define D_800CC284 (*(ContPad **)&D_800CC284)
extern u16 D_8009917C[];
extern u8 D_800BEA0C;
extern f32 D_800CBD90;
extern f32 D_800CBD94;
extern f32 D_800CBD98;
extern s8 D_800CBDD3;
extern s8 D_800CBDD4;
extern f32 D_800CBDF4;
extern f32 D_800CBDF8;
extern f32 D_800CBDFC;
extern u8 D_800CC26C;
extern f32 D_800CC270;
extern f32 D_800CC274;
extern f32 D_800CC278;
extern u16 D_800CC27E;
extern s32 D_800CC28C;
extern s32 D_800CC290;
s32 func_1000F568(s32, s32);
void func_1502EA0C(Actor *, s32, s32, s32, s32, s32);
void func_1502EA50(Actor *);
void func_15044380(f32, f32, f32, Actor *, s32, s32);
void func_15055E50(Actor *, u8);
u8 func_150599C8(Actor *, u8, u16);
f32 func_1505A3A8(f32, Actor *, f32, f32, u8);
void func_1505A9AC(Actor *, f32);
void func_1505D024(Actor *, s32, u16, s32);
void func_1505D6F0(Actor *, u8);
void func_1505F298(Actor *, ContPad *);
void func_1506045C(Actor *, u16, s32, f32, s32);
void func_15063168(Actor *);
s32 func_1506E46C(Actor *, s32 *, s32);
void func_1507FEA0(Actor *);
void func_15087E54(u8, Actor *);
s32 func_150A3FC4(f32, f32, void *, s16 (*)[3], f32 *);
void func_150D5A6C(Actor *, s32, s32);
void func_150D7790(Actor *, s32);
void func_1510E8BC(void **, s16 (*)[3], f32 *, f32 *, u32 *, s16 *, f32, f32, f32, f32, s32, Actor *, f32, f32, s32);
void func_15121490(Actor *, ContPad *);
void func_151279A0(Actor *, ContPad *);
void func_151AE264(Actor *);

// matching: same addresses as D_800CC280/D_800CC2BC (rodata_extern places a D_<addr> static there), but as
// separate symbols IDO neither shares their %hi nor reuses loads across them; a few uses need that.
static s32 D_800cc280;
static s32 D_800cc2bc;
// matching: a declaration-only block scope (no storage). IDO opens a lexical block for it, which changes
// register allocation/scheduling of the surrounding statements; no storage-free idiomatic form matches.
#define BLK typedef s32 blk_t
void func_15051558(Actor *actor, f32 x, f32 z, u16 facing);
void func_1505A770(Actor *arg0);
void func_15052260(Actor *arg0);
void func_1505250C(Actor *arg0, s32 arg1);
s32 func_1505C7D8(Actor *, u8);
void func_15059C84(Actor *);

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
    ContPad savedPad;
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
    D_800CC28C = D_800BE710[D_800C3E78];
    D_800CC290 = D_800CC284->buttons;
    D_800CC2B1 = 0;
    D_800CC2B2 = 0;
    D_800CC2B4 = 4.0f;
    prevFacing = actor->unk76;
    padOverridden = 0;
    savedPad = *D_800CC284;
    D_800CC2BC = 1;
    actor->unk31C->unk1B3 = 0;
    if (actor->unk31C->unk84 != 0) {
        if (actor->stunned == 0) {
            func_15056B08((Actor *) actor);
        }
        savedPad = *D_800CC284;
        D_800CC284->stick_x = actor->unk31C->stick_x;
        D_800CC284->stick_y = actor->unk31C->stick_y;
        D_800CC284->buttons = actor->unk31C->buttons;
        D_800CC288 = actor->unk31C->unk8C;
        buttons = actor->unk31C->buttons;
        if (actor->camera == NULL) {
            func_151279A0(actor, D_800CC284);
            func_15121490(actor, D_800CC284);
        }
        func_1504C854((Actor *) actor);
    } else {
        actor->unk31C->buttons = D_800CC284->buttons;
        D_800CC288 = D_800BE710[D_800C3E78];
        buttons = actor->unk31C->buttons;
    }
    if ((D_80082FA0 >= D_800C3E78) && (D_800DDE3C[D_800C3E78] != 0) && (D_800DDDC8[D_800C3E78] > 0.75f) && (D_800E0B94 != 2)) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->buttons = 0;
        D_800CC288 = 0;
        padOverridden = 1;
        D_800CC2BC |= 2;
    }
    if ((actor->stunned != 0) && (actor->health != 0) && ((((actor->unk31C->unk26 & 1) != 0) && (D_800CC288 & 0x8000)) || ((actor->unk31C->unk26 & 2) && (func_1505A5CC((struct49 *) D_800CC284) > 15.0f)))) {
        actor->unk10C = 0;
        actor->unk105 = 3;
        func_150585F0((Actor *) actor, 0.05f);
    }
    if (D_800BE616 != 0) {
        if (D_800BE9A0 < actor->unk31C->unk1B1) {
            actor->unk31C->unk1B1 -= D_800BE9A0;
            if (actor->fogMode != 4) {
                func_1502EA0C(actor, 0xFF, 0xFF, 0xFF, 0xBF, 4);
            }
        } else if (actor->fogMode == 4) {
            actor->unk31C->unk1B1 = 0;
            func_1502EA50(actor);
        }
        if (((D_800BE9F0 == 0x2D) || (D_800BE9F0 == 0x30) || (D_800BE9F0 == 0x34) || (D_800BE9F0 == 0x1A) || (D_800BE9F0 == 0x3F)) && (D_800CC288 & 4) && !(D_800CC288 & 0xC000) && ((s32) actor->unk28 == 0) && (actor->y_velocity < 0.0f) && (actor->disable_run == 0) && (actor->unk13C == 0) && (actor->unk31C->unk19B == 0) && (actor->unk31C->unk78 == 0) && ((actor->unk31C->unk75 & ~0x80) != 3) && (actor->unk31C->grenade_count != 0)) {
            if ((actor->id == 0x3B) || (D_800BE9F0 == 0x1A) || (D_800BE9F0 == 0x3F)) {
                actor->unk31C->unk19B = 5;
            } else {
                actor->unk31C->unk19B = 1;
            }
        }
    }
    actor->unk31C->unk38 = D_800CC284->buttons;
    if (D_800C3638 != 0) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->buttons = 0;
        D_800CC288 = 0;
        D_800CC2BC |= 4;
    }
    func_1507FEA0(actor);
    func_1504BB88((Actor *) actor);
    func_1504BA38((Actor *) actor);
    if ((actor->unk228 != 0) || (actor->unk2F8 & 0x10)) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->buttons = 0;
        D_800CC288 = 0;
        padOverridden = 1;
        D_800cc2bc |= 8;
    }

    if (actor->health == 0) {
        actor->disable_run = 0x14;
        if ((actor->stunned == 0) && (actor->unk31C->unk120 == 0)) {
            if ((actor->unk184 & 0x1F) == 0xE) {
                func_1507CD64(actor, 0xD);
            } else if ((actor->unk184 & 0x1F) == 0x13) {
                func_1507CD64(actor, 0xD);
            } else {
                func_1507CD64(actor, 0);
            }

        }
    }
    if (actor->unk31C->unk78 != 0x1B) {
        if (((actor->unk31C->unk198 == 0) || (actor->unk31C->unk78 == 3)) && (actor->unk31C->unk78 != 0x12) && (actor->unk31C->unk78 != 2) && (actor->unk31C->unk78 != 0x13) && (actor->unk31C->unk78 != 0xA) && (actor->unk31C->unk78 != 0x34)) {
            mode = actor->unk31C->unk78;
            D_800CC2BC |= 0x10;
            moveAngle = func_1505A630(D_800CC284->stick_x, D_800CC284->stick_y, 0);
            moveSpeed = func_1505A5CC((struct49 *) D_800CC284);
            physics = actor->unk31C->unk4E;
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
                if (actor->unk31C->unk50 > 0) {
                    actor->unk31C->unk50 -= D_800BE9A0;
                    angleDelta = (angleDelta / 3);
                    actor->disable_jump = 6;
                    pitchInput -= 60.0f;
                } else if ((actor->unk28 > 100.0f) || (actor->unk83 != 0)) {
                    angleDelta = (angleDelta / 2);
                }
                moveSpeed = 135.0f + pitchInput;
                actor->unk239 = 0x29;
                if (actor->unk31C->unk84 == 0) {
                    if (actor->disable_run == 0) {
                        actor->unk76 -= angleDelta / 9;
                    } else {
                        moveSpeed = 0.0f;
                    }
                    func_15087E54(D_800C3E78, actor);
                    moveAngle = actor->unk76 - D_800CC280;
                }
                physics = actor->unk31C->unk4E;
            }
            if (physics == 0x81) {
                centerAngle = 0x6000;
                angleDelta = D_800CC284->stick_x * D_800CC264;
                actor->unk76 -= angleDelta / 8;
                if (D_800BE9F0 == 6) {
                    moveSpeed = 42.0f;
                    centerAngle = func_1505A630(2540.0f - actor->x_position, actor->z_position - -160.0f, 0);
                    maxOffset = 0xF000;
                    minOffset = 0x8000;
                } else {
                    maxOffset = 0xB000;
                    minOffset = 0x2000;
                    moveSpeed = 65.0f;
                }
                offset = (actor->unk76 - centerAngle);
                if (maxOffset < offset) {
                    actor->unk76 = centerAngle + maxOffset;
                }
                if (offset < minOffset) {
                    actor->unk76 = centerAngle + minOffset;
                }
                moveAngle = actor->unk76 - D_800CC280;
                actor->unk78 = actor->unk76;
                { BLK; actor->unk7A = actor->unk76; }
            }
            if (actor->unk31C->aimState != 0) {
                moveSpeed = 0.1f;
                actor->unk7A = (s32) (actor->unk31C->aimYaw * 182.04445f);
                moveAngle = actor->unk7A - D_800CC280;
                if (actor->unk31C->unk84 != 0) {
                    actor->unk76 = actor->unk7A;
                }
                scratch = actor->unk31C->aimPitch;
                if (scratch > 180.0f) {
                    scratch -= 360.0f;
                }
                scratch *= 0.91022223f;
                lean = scratch;
                if ((D_800BE616 != 0) && ((actor->id == 0x3B) || (actor->id == 0x80)) && ((mode == 0x18) || (mode == 0x41) || (mode == 0x23) || (mode == 0x24)) && (D_800CC284->buttons & 0x2000) && ((actor->unk31C->unk109 == 3) || (actor->unk31C->unk109 == 2))) {
                    chance = 0;
                    if (actor->unk31C->unkAF < 0x3C) {
                        chance = 0x30;
                    }
                    if (func_10010894((Actor *) actor) == 0) {
                            if (chance >= (func_150ADA20() & 0x5F)) {
                            if (actor->id == 0x80) {
                                if ((mode == 0x23) && !(func_150ADA20() & 7)) {
                                    func_15060A30(0x586, actor);
                                } else {
                                    variant = (mode == 0x23) ? 4 : 5;
                                    func_15060A30(func_1000F568(0x574, variant), actor);
                                }
                            } else if ((D_800CC284->buttons & 8) && !(func_150ADA20() & 1)) {
                                func_15060A30(0x532, actor);
                            } else {
                                if ((actor->unk31C->unk109 == 3) && (actor->unk31C->unk108 == 0xA) && !(func_150ADA20() & 3)) {
                                    func_15060A30(0x533, actor);
                                } else {
                                    func_15060A30((func_150ADA20() & 3) + 0x534, actor);
                                }
                            }
                        }
                    }
                    actor->unk31C->unkAF = 0x78;
                }
                if (actor->unk31C->unkAF != 0) {
                    if (D_800BE9E4 < actor->unk31C->unkAF) {
                        actor->unk31C->unkAF -= D_800BE9E4;
                    } else {
                        actor->unk31C->unkAF = 0;
                    }
                }
                { BLK; }
            } else {
                actor->unk31C->unkAF = 0;
            }
            if (actor->unk31C->unk48 > 0) {
                actor->unk31C->unk48 -= D_800CC264;
                moveAngle = actor->unk76 - D_800CC280;
            }
            if (actor->unk31C->unk46 > 0) {
                actor->unk31C->unk46 -= D_800CC264;
                if (D_800CC288 & 0xC000) {
                    if (actor->unk28 == 0.0f) {
                        D_800D1580 = 0xFF030170;
                        actor->xz_velocity *= 0.4f;
                        actor->y_velocity = 10.0f;
                        actor->gravity = 5.0f;
                    }
                    actor->unk31C->unk46 -= 0x190;
                }
                if (actor->disable_run == 0) {
                    phase = ((D_800CC250 * 0xC) & 0xFF) * 0.024543693f;
                    if (moveSpeed < 5.0f) {
                        moveAngle = actor->unk76 - D_800CC280;
                    }
                    moveSpeed = 28.0f;
                    actor->unk31C->unk3C = 4;
                    scratch = func_150AD78C(phase) * 10240.0f;
                    actor->unk31C->unkB += 0x10;
                    actor->unk31C->unk10 = scratch;
                }
            } else {} // matching: the empty else changes block layout
            if ((actor->unk84.uh == 0xBF) || (actor->unk84.uh == 0xC1)) {
                lean = (D_800CC284->stick_y / 2);
            }
            if (actor->unk31C->unk78 == 0x38) {
                lean -= actor->unk31C->unk1A2 >> 8;
            }
            { BLK; }
        } else {
            moveAngle = 0;
            moveSpeed = 0.0f;
            D_800cc2bc |= 0x20;
            if ((actor->unk31C->unk78 == 2) || (actor->unk31C->unk78 == 0x12) || (actor->unk31C->unk78 == 0x13) || (actor->unk31C->unk78 == 0x10) || (actor->unk31C->unk78 == 0xA) || (actor->unk31C->unk78 == 0x34) || ((D_800CC284->buttons & 0x10) && (actor->unk31C->unk78 == 0x16)) || (actor->unk31C->unk78 == 9) || (actor->unk31C->unk78 == 0x38) || (actor->unk31C->unk78 == 0x39) || (actor->unk31C->unk78 == 0x37) || (actor->unk31C->unk78 == 0x3B) || (actor->unk31C->unk78 == 0x15) || (actor->unk31C->unk78 == 0x26) || (actor->unk31C->unk78 == 0x3A)) {
                if ((actor->unk31C->unk78 == 0x10) || (actor->unk31C->unk78 == 0x14) || (actor->unk31C->unk78 == 0x3F)) {
                    lean = (D_800CC284->stick_y / 2);
                }
                if ((actor->unk31C->unk78 == 0xC) || (actor->unk31C->unk78 == 0x16) || (actor->unk31C->unk78 == 0x15) || (actor->unk31C->unk78 == 0x38) || (actor->unk31C->unk78 == 0x39) || (actor->unk31C->unk78 == 0x37) || (actor->unk31C->unk78 == 0x3B) || (actor->unk31C->unk78 == 0x12) || (actor->unk31C->unk78 == 9)) {
                    tilt = actor->unk31C->aimPitch;
                    if (tilt > 180.0f) {
                        tilt -= 360.0f;
                    }
                    tilt *= 0.91022223f;
                    lean = tilt;
                    if (actor->unk31C->unk78 == 0x38) {
                        lean -= actor->unk31C->unk1A2 >> 8;
                    }
                }
                if ((actor->unk31C->unk78 != 9) && (actor->unk31C->unk78 != 0x38) && (actor->unk31C->unk78 != 0x39) && (actor->unk31C->unk78 != 0x37) && (actor->unk31C->unk78 != 0x3B) && (actor->unk31C->unk78 != 0x15) && (actor->unk31C->unk78 != 0x26) && (actor->unk31C->unk78 != 0x3A)) {
                    actor->unk7A = (s32) ((actor->unk31C->aimYaw + 90.0f) * 182.04445f);
                    actor->unk76 = actor->unk7A;
                }
            } else if ((actor->unk31C->unk78 != 0x14) && (actor->unk31C->unk78 != 0x3F)) {
                D_800CC2BC |= 0x100;
                D_800CC284->buttons &= ~0x2000;
                D_800CC288 &= ~0x2000;
            }
            if (actor->unk31C->unk78 != 9) {
                D_800CC2BC |= 0x200;
                D_800CC284->buttons &= ~0x8000;
                D_800CC288 &= ~0x8000;
            }
        }
    }
    if (!(actor->unk31C->unk4E & 0xF) && (actor->health != 0) && (actor->stunned == 0) && (actor->unk28 == 0.0f) && ((D_800BE616 != 0) || ((actor->unk31C->unk78 == 9) && (actor->unk31C->matrix_physics == 0)) || (actor->unk31C->unk78 == 0x16) || (actor->unk31C->unk78 == 0x3B)) && ((actor->unk31C->unk11A == 2) || (actor->unk31C->unk11A == 0))) { BLK;
        dirMask = ((actor->unk31C->unk78) && ((actor->unk31C->aimState == 0) || (actor->unk31C->unk78 == 0x16)) && (actor->unk31C->unk19B == 0)) ? 0xF : 3;
        if ((D_800CC284->buttons & dirMask) != 0) {
            dirMask = (D_800CC284->buttons & dirMask);
            if ((actor->unk31C->aimState == 0) && (actor->unk31C->unk198 == 0)) {
                if (D_800CC284->stick_y >= 6) {
                    dirMask |= 8;
                } else if (D_800CC284->stick_y < -5) {
                    dirMask |= 4;
                }
            }
            dirAngle = D_8009917C[dirMask];
            actor->unk31C->unk52 = 2;
            if (dirAngle != 0xFFFF) {
                if ((actor->unkAA == 0) || (actor->unkAA >= 0x42)) {
                    actor->unkAA = 0x41;
                }
                if (actor->xz_velocity > 12.0f) {
                    actor->unk31C->unk3C = 4;
                }
                if (actor->unk76 != actor->unk7A) {
                    actor->unk31C->unk3C = 0;
                }
                actor->unk7A -= ((D_800CC284->stick_x * D_800CC264) >> 4);
                moveAngle = ((actor->unk7A + dirAngle) - actor->unk31C->unk12) - D_800CC280;
                moveSpeed = 34.0f;
                if (actor->unk31C->aimState != 0) {
                    actor->unk76 = moveAngle + D_800CC280;
                }
                actor->unk31C->unk1B3 = 1;
            } else {
                actor->xz_velocity = 0.0f;
            }
        } else if (actor->unk31C->unk52 != 0) {
            actor->unk76 = actor->unk7A;
            if (actor->unk31C->aimState != 0) {
                actor->xz_velocity = 0.0f;
            }
        }
        
    }
    if (actor->unk31C->unk23 > 0) {
        actor->disable_run = 0;
        actor->unk83 = 0;
        moveSpeed = 18.0f;
        moveAngle = actor->unk76 - D_800CC280;
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->buttons = 0;
        D_800CC288 = 0;
        padOverridden = 1;
        actor->unk31C->unk23 -= D_800BE9A0;
        D_800cc2bc |= 0x40;
    }
    if (actor->unk31C->unk6B != 0) {
        func_15052260((Actor *) actor);
    }
    surfaceDelta = actor->unk118 - actor->unk11C;
    if (fabsf(surfaceDelta) > 30.0f) {
        surfaceDelta = 0.0f;
    }
    actor->unk11C = actor->unk118;
    if (actor->unk31C->unk84 == 0) {
        actor->unk31C->buttons = D_800CC284->buttons;
        actor->unk31C->unk8C = D_800CC288;
    }
    if ((actor->unk84.uh == 0) && (actor->animation_speed == 0.0f)) {
        func_1505E650((Actor *) actor, 0x24, 1.0f, 1.0f, 0.0f, 0.0f, 0);
    }
    if (D_800CC284->stick_y == 0) {
        actor->unk31C->unk53 = 0;
    }
    if (actor->unk137 == 0xFF) {
        actor->unk137 = 0;
    }
    turnRate = 0xC;
    if ((actor->unk31C->unk8 > 0) || (actor->unk31C->unk78 == 3)) {
        func_1504BE2C((Actor *) actor, &moveAngle, &moveSpeed, &lean);
    }
    func_1504C9E4((Actor *) actor, lean, actor->unk31C->unk78);
    if (actor->stunned == 0) {
        if (actor->unk31C->unk84 != 0) {
            moveAngle = actor->unk78;
            moveSpeed = actor->unk44;
        }
        if (moveSpeed > 0.0f) {
            lockAngle = 0;
            lockDiff = 0;
            diff = 0;
            actor->unk31C->unk4C = moveAngle + D_800cc280;
            if (actor->unk31C->unk32 & 2) {
                lockAngle = actor->unk31C->unk34;
                wantAngle = actor->unk31C->unk4C;
                lockDiff = actor->unk31C->unk34 - wantAngle;
                if (!((lockDiff + 0x2000) & 0xC000)) {
                    actor->unk31C->unk4C = actor->unk31C->unk34;
                    lockAngle = actor->unk31C->unk34;
                    wantAngle = actor->unk31C->unk4C;
                    lockDiff = lockAngle - wantAngle;
                }
                if (!((lockDiff + 0xA000) & 0xC000)) {
                    actor->unk31C->unk4C = lockAngle + 0x8000;
                    wantAngle = actor->unk31C->unk4C;
                }
                moveAngle = wantAngle - D_800cc280;
            }
        } else {
            actor->unk31C->unk4C = actor->unk7A;
        }
        if (actor->in_water != 0) {
            swimSpeed = func_1504B0FC((Actor *) actor, surfaceDelta);
            moveAngle = actor->unk31C->unk4C;
            if (actor->unk31C->unk22 >= 0) {
                actor->unk31C->unk22 -= D_800BE9A0;
                if ((actor->unk31C->unk22 >= 0x5B) && (actor->unk31C->unk22 < 0x64)) {
                    actor->unk31C->unk22 = 0;
                }
            }
            if (actor->unk31C->unk21 == 0) {
                moveSpeed = swimSpeed;
                moveAngle = actor->unk76;
                if (!(D_800CC284->buttons & 0x4000)) {
                    if ((actor->unk31C->unk22 <= 0) && (((actor->unk118 - 60.0f) - 30.0f) < actor->y_position)) {
                        actor->unk31C->unk21 = 1;
                        actor->disable_run = 0xC;
                    }
                }
            } else {
                if ((D_800CC288 & 0x4000) && (D_800BE616 == 0) && (D_800BE9F0 != 0x29)) {
                    if ((actor->camera != NULL) && (actor->camera->mode != 0x100) && !(actor->unk31C->unk20 & 1)) {
                        actor->unk31C->unk21 = 0;
                        actor->unk31C->unk22 = 0x76;
                        if (((actor->unk118 - 60.0f) - 30.0f) < actor->y_position) {
                            actor->unkB8 = 20.0f;
                        }
                        func_15060A30(0x66F, actor);
                    }
                }
                actor->unk31C->unk3C = 4;
                {
                BLK;
                actor->unkAA = 0x28;
                }
            }
        } else {
            moveAngle += D_800cc280;
        }
        D_800CC27E = moveAngle;
        actor->unk44 = moveSpeed;
        if (actor->unkAA != 0) {
            moveSpeed *= actor->unkAA * 0.01f;
        }
        if (actor->unk13C >= 0x64) {
            moveSpeed *= 0.8f;
        }
        if ((actor->disable_run != 0) || (actor->unk81 >= 0x79)) {
            moveSpeed = 0.0f;
        }
        func_1504BC38((Actor *) actor);
        steerDiv = 4.0f;
        steer = 0.0f;
        if ((actor->in_water != 0) && (actor->unk31C->unk21 == 0)) {
            if ((actor->stunned == 0) && (actor->unk84.uh != 0x262)) {
                steer = (D_800CC284->stick_x / 2);
            }
        } else {
            if ((actor->unk31C->unk78 == 2) || (actor->unk31C->unk78 == 0x13) || (actor->unk31C->unk78 == 0xA) || (actor->unk31C->unk78 == 0x34)) {
                if (actor->unk31C->unk198 != 0) {
                    tiltDeg = -actor->unk31C->aimPitch;
                    if ((actor->unk31C->unk78 == 2) || (actor->unk31C->unk78 == 0x13)) {
                        tiltDeg += 30.0f;
                    } else {
                        tiltDeg = -tiltDeg;
                    }
                    { BLK; steer = (s8) tiltDeg; }
                } else {
                    steer = (D_800CC284->stick_y / 2);
                }
            } else {
                if (actor->disable_run != 0xFF) {
                    angleDelta = moveAngle - prevFacing;
                    if ((actor->unk28 < 20.0f) && (actor->xz_velocity < 1.5f)) {
                        steer = actor->unk44 * 0.3f;
                    } else {
                        steer = actor->unk44 * 1.75f;
                        turnLean = fabsf(angleDelta * 0.00390625f * 5.0f);
                        if (turnLean > 50.0f) {
                            turnLean = 50.0f;
                        }
                        if (turnLean < steer) {
                            steer = turnLean;
                        }
                        if (actor->unk28 > 30.0f) {
                            steer *= 0.6f;
                        }
                        if (angleDelta >= 0) {
                            steer = 0.0f - steer;
                        }
                    }
                }
                if (((actor->unk13C != 0) && (D_800CC2D0[D_800D154C->unk13C - 100].id != 0x57)) || (actor->unk31C->unk198 != 0) || (D_800CC284->buttons & 0x2000)) {
                    steer = 0.0f;
                }
            }
        }
        if (actor->unk31C->unk17 != 0) {
            if (actor->unk84.uh == 0x279) {
                if (actor->disable_run == 0) {
                    if (D_800CC284->buttons & 0x8000) {
                        actor->gravity = 1.1f;
                    } else {
                        if (actor->y_velocity > 0.0f) {
                            actor->y_velocity *= 0.85f;
                        }
                        actor->gravity = 2.5f;
                    }
                }
                if (actor->unk28 > 0.0f) {
                    actor->unkC4 += (((actor->unk54 * 4.0f) - actor->unkC4) * 0.15f);
                    actor->unkB8 = actor->xz_velocity * 1.5f;
                }
                actor->unkAA = 0x3C;
                actor->unk80 = 1;
                actor->unk83 = 0xFF;
            }
            steer *= 0.6f;
            steerDiv = 12.0f;
        }
        if ((actor->unk110 == 2) || (actor->unk84.uh == 0xCD)) {
            steer = -steer;
        }
        steer = (steer - actor->unk54) / (steerDiv / *D_800D1550);
        if ((actor->unk28 < 30.0f) || (actor->unk31C->unk17 != 0)) {
            if (((steer > 0.0f) && (actor->unk31C->unk40 < 0.0f)) || ((prevSteer = actor->unk31C->unk40, steer < 0.0f) && (prevSteer > 0.0f))) {
                steer = 0.0f;
            } else if (fabsf(steer - prevSteer) > 2.0f) {
                if (prevSteer < steer) {
                    steer = prevSteer + 2.0f;
                } else {
                    steer = prevSteer - 2.0f;
                }
            }
        }
        actor->unk31C->unk40 = steer;
        if ((D_800BEAC0 == 0) && (D_800BEA0C == 0)) {
            actor->unk54 += steer;
        }
        if (moveSpeed != 0.0f) {
            if (moveAngle != actor->unk76) {
                if (actor->xz_velocity < 5.0f) {
                    turnStep = 0x19;
                } else {
                    turnStep = (((64.0f - actor->xz_velocity) * 0.25f));
                    if (actor->unk31C->unk31 >= 3) {
                        turnStep += 1;
                    } else if (actor->unk81 > 0) {
                        turnStep = ((51.0f - actor->xz_velocity) / 13.0f);
                        if (actor->y_velocity < -10.0f) {
                            turnStep = 0;
                        }
                    }
                }
                turn = (moveAngle - actor->unk76) >> 8;
                turnLeft = turn & 0x80;
                turnSize = turn;
                if (turn >= 0x80) {
                    turn = -turn;
                    turnSize = turn;
                }
                if (actor->unk102 != 0) {
                    turnStep = 5;
                } else if ((turnSize >= 0x51) && (actor->in_water == 0)) {
                    if ((actor->unk31C->unk46 <= 0) && (actor->unk31C->unk8 <= 0)) {
                        if ((actor->unk31C->unk4E & 0xF) == 1) {
                            if (actor->xz_velocity > 7.0f) {
                                turnStep = 0;
                                actor->xz_velocity -= 3.5f;
                            } else {
                                turnStep = 6;
                            }
                        } else if (actor->xz_velocity < 7.0f) {
                            if (actor->xz_velocity > 3.0f) {
                                turnStep = 0xC;
                                actor->xz_velocity = 0.0f;
                                if (actor->unk83 >= 5) {
                                    if ((actor->unk31C->unk11A != 1) && (actor->unk31C->unk11A != 3)) {
                                        actor->unk83 = 4;
                                    }
                                }
                            }
                        } else {
                            moveSpeed = 0.0f;
                            turnStep = 0;
                            if (actor->unkAE == 0) {
                                actor->xz_velocity -= 2.0f;
                            }
                        }
                    }
                }
                turnStep = ((turnStep * D_800CC264) / 100);
                if (actor->unk13C >= 0x64) {
                    turnStep = ((turnStep * 2) / 3);
                } else if (actor->unk110 != 0) {
                    turnStep = 2;
                }
                if (actor->in_water != 0) {
                    turnStep = (turnStep / 4);
                }
                if (actor->unk31C->unk3C != 0) {
                    turnStep = ((actor->unk31C->unk3C * D_800CC264) / 100);
                }
                if (turnSize < turnStep) {
                    turnStep = turnSize;
                }
                if (turnLeft) {
                    actor->unk76 -= (turnStep << 8);
                } else {
                    actor->unk76 += (turnStep << 8);
                }
            }
        }
        actor->unk31C->unk3C = 0;
        if (actor->unk110 != 0) {
            turnRate = 0x10;
            if (actor->unk110 & 1) {
                moveAngle = actor->unk31C->unk3E;
            } else {
                moveAngle = actor->unk31C->unk3E ^ 0x8000;
            }
        } else if ((moveSpeed == 0.0f) || (actor->in_water != 0) || (actor->unk31C->unk17 != 0) || (actor->unk28 < 50.0f)) {
            moveAngle = actor->unk76;
        }
        turn = 0;
        if (actor->unk31C->matrix_physics != 0) {
            moveAngle = 0x4000;
            turnRate = 0x14;
        } else if ((actor->unk31C->unk17 != 0) && (actor->unk13C == 0)) {
            target = func_15083E90(6);
            if (target != NULL) {
                if (func_1505A6F8(target, (Actor *) actor) < 600.0f) {
                    moveAngle = func_1505A630(target->x_position - actor->x_position, actor->z_position - target->z_position, 0);
                } else {
                    actor->unk31C->unk3C = 5;
                }
            }
        } else { } // matching: the empty else changes block layout
        if (actor->unk31C->unk52 == 0) {
            turn = func_150599C8(actor, turnRate, moveAngle);
        }
        if ((turn >= 0x5B) && (actor->xz_velocity > 12.0f) && (actor->in_water == 0) && (actor->unk81 == 0)) {
            moveSpeed = 0.0f;
        }
        if (actor->unk76 == actor->unk7A) {
            actor->unk80 = 0;
        }
        friction = 0.0f;
        physics = actor->unk31C->unk4E;
        if ((physics & 0xF) == 1) {
            if (actor->unk31C->unk4F != 0) {
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
            if (actor->unk84.uh == 0x16) {
                steer = 3.2f;
                if (D_800CC284->buttons & 0x2000) {
                    moveSpeed = 0.0f;
                }
            } else if (actor->unk13C != 0) {
                if (actor->id == 0x13) {
                    steer = 0.75f;
                }
                friction = 0.75f;
            } else if (actor->unk110 != 0) {
                moveSpeed = 0.0f;
                steer = 2.0f;
            } else {
                if (actor->unk31C->unk46 > 0) {
                    friction = 4.0f;
                } else {
                    friction = 1.0f;
                }
            }
            if (actor->unk31C->unk8 > 0) {
                friction = 1.0f;
                steer = 2.0f;
            }
        }
        if (actor->unk31C->matrix_physics != 0) {
            steer = 0.0f;
        }
        func_1505A3A8(moveSpeed, actor, friction, steer, turn);
        func_1505F298(actor, D_800CC284);
    } else {
        if (actor->in_water != 0) {
            func_1504B0FC((Actor *) actor, surfaceDelta);
        }
        if (actor->stunned != 0xFF) {
            func_1506045C(actor, D_800CC284->buttons, D_800CC288, actor->unk28, 0);
            actor->unk54 = 0.0f;
            actor->disable_run = 0;
            actor->unk83 = 0;
            if (((actor->unk84.uh == 0xF1) || (actor->unk84.uh == 0xF6)) && (actor->unk28 == 0.0f)) {
                actor->y_velocity = 0.0f;
                actor->gravity = 0.0f;
                actor->unkF8 |= 0x01008000;
            }
            if (!(actor->unk10B & 1) && (actor->xz_velocity < 2.0f) && (actor->stunned < 0xD2) && (actor->unk28 < 10.0f) && (actor->y_velocity < 0.5f)) {
                actor->stunned = 1;
                actor->unk10C = 1;
                actor->unk105 = 3;
            }
            func_150585F0((Actor *) actor, 0.1f);
            if (actor->unk10B & 0x40) {
                height = actor->y_position - D_800CC278;
                if (height < 400.0f) {
                    if (height < 10.0f) {
                        height = 10.0f;
                    }
                    actor->unk76 += (D_800CC264 * 0x1400) / 100;
                    actor->y_velocity = (height * 0.06f) + 6.0f;
                    actor->xz_velocity = 3.0f;
                    func_1505A184(actor->unk76, height, 0.0f, &offsetX, &offsetZ, &offsetY);
                    actor->unk31C->unk28 = offsetX + D_800CC270;
                    actor->unk31C->unk2C = offsetZ + D_800CC274;
                    actor->unk31C->unk30 = 2;
                } else {
                    actor->unk10B &= ~0x40;
                    actor->xz_velocity = 30.0f;
                    actor->y_velocity = 90.0f;
                    actor->gravity = 4.0f; { BLK; }
                }
            }
            if (actor->unk10B & 0x80) {
                actor->unk228 = 0;
                actor->immune = 0x5A;
                if ((actor->unk28 == 0.0f) && (actor->y_velocity < 24.0f)) {
                    actor->unk10C = 0;
                    if (actor->unk10B & 0x20) {
                        actor->unk31C->unk46 = 0xFA0;
                    }
                }
                if (moveSpeed > 5.0f) {
                    moveAngle += D_800CC280;
                    actor->unk80 = 1;
                    func_150599C8(actor, 6, moveAngle);
                    func_1505A184(moveAngle, moveSpeed, 0.0f, &pushX, &pushZ, &pushY);
                    actor->unk16C += ((pushX - actor->unk16C) * 0.1000000015f);
                    actor->unk170 += ((pushZ - actor->unk170) * 0.1000000015f);
                    actor->unk76 = actor->unk7A;
                }
            }
        }
    }
    if ((actor->unk31C->ropeId != 0) && (actor->stunned == 0)) {
        stickX = D_800CC284->stick_x;
        stickY = D_800CC284->stick_y;
        actor->unk31C->unk31 = 0;
        actor->unk54 = 0.0f;
        actor->disable_run = 0xFF;
        actor->xz_velocity *= 0.95f;
        actor->y_velocity = 0.0f;
        actor->gravity = 0.0f;
        actor->unk81 = 1;
        if ((actor->camera != NULL) && (actor->camera->mode == 0x100)) {
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
        path = (Sub9C *)actor->unk31C->rope;
        climbRate = path->unk2C;
        climbRate = 0.08f / climbRate;
        actor->unk31C->ropeProgress += stickY * climbRate * *D_800D1550;
        if (actor->unk31C->ropeProgress > 1.0f) {
            actor->unk31C->ropeProgress = 1.0f;
        } else if (actor->unk31C->ropeProgress < 0.0f) {
            func_151AE264(actor);
            actor->gravity = 5.0f;
        }
        actor->unk1CC = actor->y_position;
        actor->unk31C->unk54 = 8;
        heading = actor->unk76 + ((stickX * D_800CC264) / 5);
        actor->unk78 = heading;
        actor->unk7A = heading;
        actor->unk76 = heading;
        func_1505E650((Actor *) actor, climbAnim, climbAnimSpeed * 0.025f, 6.0f, 0.0f, 0.0f, 0);
    }
    if ((actor->unk31C->unk95 != 0) && (actor->stunned == 0)) {
        if ((actor->camera != NULL) && (actor->camera->mode != 0x100)) {
            climbIndex = (actor->unk31C->unk95 & ~0x80) - 1;
            climbSpeed = D_800CC284->stick_y;
            climbSpot = &D_800D3098[climbIndex];
            if ((actor->in_water != 0) && (actor->y_position < actor->unk118)) {
                actor->unk31C->unk96 = 8;
            }
            if (actor->unk31C->unk96 != 0) {
                actor->unk31C->unk96 -= 1;
                climbSpeed = 60.0f;
            }
            actor->unk31C->unk28 = climbSpot->x;
            actor->unk31C->unk2C = climbSpot->z;
            actor->unk31C->unk30 = 0xA;
            actor->unk31C->unk31 = 0;
            actor->xz_velocity = 0.0f;
            actor->unk54 = 0.0f;
            actor->disable_run = 0xFF;
            actor->gravity = 0.0f;
            actor->unk81 = 1;
            actor->unk80 = 1;
            actor->unk76 = (s32) (climbSpot->yaw * 182.04445f) - 0x4000;
            actor->y_velocity = climbSpeed * 0.14f;
            if (actor->unk31C->unk97 != 0) {
                actor->y_velocity = 0.0f;
            }
            actor->unk1CC = actor->y_position;
            actor->unk31C->unk54 = 8;
        }
    }
    if (actor->unk31C->unk6B == 0) {
        func_15059C84((Actor *) actor);
        if (actor->unk31C->unk30 > 0) {
            actor->unk31C->unk30 -= D_800BE9A0;
            actor->x_position += ((actor->unk31C->unk28 - actor->x_position) * 0.2f);
            actor->z_position += ((actor->unk31C->unk2C - actor->z_position) * 0.2f);
        }
        func_1502178C((Actor *) actor, 0, -1);
        func_1505A770((Actor *) actor);
        func_1505D6F0(actor, D_800C3E78);
        if ((D_800CC26C != 0) && (actor->unk31C->unk8 <= 0) && (actor->unk13C == 0)) {
            heading = func_1505A630(D_800CC2D0[D_800CC26C].x_position - actor->x_position, actor->z_position - D_800CC2D0[D_800CC26C].z_position, 0);
            
            if ((actor->unk28 == 0.0f) && (moveSpeed > 15.0f) && (((D_800CC27E - heading) - 0x4000) & 0x8000)) { BLK;
                follow = 0.1f;
                carry = 1;
                if (D_800CC2D0[D_800CC26C].id == 0x61) {
                    if (actor->unk84.uh == 0x7A) {
                        otherFacing = D_800CC2D0[D_800CC26C].unk7A;
                        diff = actor->unk76 - otherFacing;
                        quadrant = (diff + 0x2000) & 0xC000;
                        if (quadrant & 0x4000) {
                            D_800CC2D0[D_800CC26C].unk218 = NULL;
                            D_800CC2D0[D_800CC26C].unk232 = 2;
                            D_800CC2D0[D_800CC26C].unk76 = otherFacing + quadrant;
                            actor->unk76 = D_800CC2D0[D_800CC26C].unk76;
                            actor->unk31C->unk3C = 1;
                            { BLK; actor->unk54 *= 0.4f; }
                        }
                    }
                } else {
                    pushScale = 0.3f;
                    pushAccel = 0.5f;
                    heightDiff = fabsf(actor->y_position - D_800CC2D0[D_800CC26C].y_position);
                    nudgeX = heightDiff;
                    if (heightDiff > 30.0f) {
                        nudgeX = 30.0f;
                    }
                    nudgeX = (2.0f * D_800CC2D0[D_800CC26C].unkE4) - nudgeX;
                    actor->unk54 *= 0.6f;
                    actor->xz_velocity = D_800CC2D0[D_800CC26C].unk44;
                    if (D_800CC2D0[D_800CC26C].unk44 < D_800CC2D0[D_800CC26C].xz_velocity) {
                        nudgeZ = D_800CC2D0[D_800CC26C].xz_velocity;
                    } else {
                        nudgeZ = D_800CC2D0[D_800CC26C].unk44;
                    }
                    actor->unk31C->unk3C = ((32.0f - nudgeZ) * 0.125f);
                    if (D_800CC2D0[D_800CC26C].id == 0x34) {
                        if (D_800CC2D0[D_800CC26C].unk251 != 1) {
                            carry = 0;
                        }
                        pushAnim = 0x23;
                        actor->unk31C->unk3C = 2;
                        follow = 0.3f;
                        if (actor->xz_velocity > 5.0f) {
                            follow = 0.6f;
                        }
                    } else {
                        pushAnim = 0x18;
                    }
                    if ((pushAnim != actor->unk84.uh) && (D_800CC2D0[D_800CC26C].id != 0x17)) {
                        D_800D1580 = 0xFF08012B;
                        pushAccel = 0.5f;
                        func_1506E8D8();
                    }
                    if (D_800CC2D0[D_800CC26C].unk224 != 0) {
                        pushScale = D_800CC2D0[D_800CC26C].unk224 * 0.01f;
                    }
                    if (carry != 0) {
                        pushAccel = 0.5f;
                        func_1505A184(actor->unk76, nudgeX, 0.0f, &nudgeX, &nudgeZ, &nudgeY);
                        actor->x_position += ((((D_800CC2D0[D_800CC26C].x_position - nudgeX) + D_800CC2D0[D_800CC26C].unkBC) - actor->x_position) * follow);
                        actor->z_position += ((((D_800CC2D0[D_800CC26C].z_position - nudgeZ) + D_800CC2D0[D_800CC26C].unk148) - actor->z_position) * follow);
                        D_800CC2D0[D_800CC26C].unk76 = actor->unk76;
                    }
                    D_800CC2D0[D_800CC26C].unk87 = 1;
                    nudgeX = actor->unk44 * pushScale;
                    if (D_800CC2D0[D_800CC26C].id == 0x25) {
                        nudgeX = actor->unk44 * 0.6f;
                        actor->xz_velocity += 2.0f;
                        pushAccel = 1.0f;
                        actor->unk31C->unk3C += 3;
                    }
                    if (D_800CC2D0[D_800CC26C].id != 0x34) {
                        if (D_800CC2D0[D_800CC26C].xz_velocity < nudgeX) {
                            D_800CC2D0[D_800CC26C].xz_velocity += (pushAccel * D_800BE9A4);
                        }
                    } else {
                        actor->xz_velocity = D_800CC2D0[D_800CC26C].xz_velocity;
                    }
                }
            }
        }
    }
    if ((D_800BE616 != 0) && (D_800CC268 != 0) && (actor->unk31C->unk1AC != 0)) {
        func_15063168(actor);
    }
    actor->unk40 = (actor->unk7A + 0x4000) * 0.005493164f;
    func_1502178C((Actor *) actor, 0, -1);
    startX = actor->x_position;
    startZ = actor->z_position;
    if (!(actor->unkF8 & 0x8000)) {
        func_15055E50(actor, actor->unk1E4);
        if (actor->unkF4 & 0x100000) {
            probeTop = 30000.0f;
        } else {
            probeTop = actor->old_y_position;
        }
        if ((actor->unk118 != -10000.0f) || ((actor->unkF8 & 2) == 0) || ((func_150A3FC4(actor->x_position, actor->z_position, actor->unk188, actor->floorVerts, &floorY) == 0))) {
            func_1510E8BC(&actor->unk188, actor->floorVerts, &floorY, &actor->unk118, &actor->unk184, &actor->floorType, actor->x_position, actor->y_position, actor->z_position, probeTop, 0, actor, -10000.0f, actor->y_position, actor->unkF8 & 0x04000000);
            if (actor->unk118 < actor->unk120) {
                actor->unk118 = actor->unk120;
            }
        }
        if (actor->y_position < floorY) {
            snapRange = 30.0f;
        } else {
            snapRange = 15.0f;
        }
        if ((actor->unkF8 & 4) && (actor->unk31C->unk54 <= 0) && (actor->unk31C->unk6B == 0) && (actor->unk137 == 0) && (actor->unk65 == 0) && ((actor->unk31C->unk84 == 0) || (actor->x_position != actor->old_x_position) || (actor->y_position != actor->old_y_position) || (actor->z_position != actor->old_z_position))) {
            if (actor->unkF8 & 0x08000000) {
                D_800CBDD3 = 1;
            }
            if (actor->unk31C->unk4E == 2) {
                D_800CBDD4 = 1;
            }
            func_15044380(actor->old_x_position, actor->old_y_position, actor->old_z_position, actor, 1, 1);
            D_800CBDD4 = 0;
            D_800CBDD3 = 0;
        } else {
            D_800CBDF4 = D_800CBDF8 = -32768.0f;
        }
        if ((startX != actor->x_position) || (startZ != actor->z_position)) {
            scratch = -10000.0f;
            actor->unkF4 |= 0x10000;
            if ((scratch != actor->unk118) || (floorY != scratch) || !(actor->unkF8 & 2) || (func_150A3FC4(actor->x_position, actor->z_position, actor->unk188, actor->floorVerts, &floorY) == 0)) {
                func_1510E8BC(&actor->unk188, actor->floorVerts, &floorY, &actor->unk118, &actor->unk184, &actor->floorType, actor->x_position, actor->y_position, actor->z_position, probeTop, 0, actor, scratch, actor->y_position, actor->unkF8 & 0x04000000);
                if (actor->unk118 < actor->unk120) {
                    actor->unk118 = actor->unk120;
                }
            }
            heading = func_1505A630(actor->x_position - startX, startZ - actor->z_position, 0);
            moveDelta = actor->unk76 - heading;
            if (actor->unk31C->unk4E == 2) {
                if ((moveDelta >= 0x1E01) && (moveDelta < 0xE200)) {
                    dirAngle = actor->unk76 - heading - 0x8000;
                    if (dirAngle & 0x8000) {
                        deflect = (dirAngle >> 2) + 0x9800;
                    } else {
                        deflect = (dirAngle >> 2) + 0x2800;
                    }
                    actor->unk76 = heading - deflect;
                    if (actor->unk31C->unk44 <= 0) {
                        if ((actor->health == 1) || ((moveDelta >= 0x5E01) && (moveDelta < 0xA200))) {
                            actor->health = 0;
                            func_150D5A6C(actor, 0xFF, 1);
                            func_1507CD64(actor, 8);
                        } else if (actor->health != 0) {
                            if (D_800BE616 == 0) {
                                D_800D1580 = 0xFF0201D6;
                                func_1506E9D8();
                            }
                            actor->health -= 1;
                        }
                        actor->unk31C->unk44 = 0x14;
                        if (moveDelta >= 0x5E01) {} // matching: dead test the original evaluates
                    }
                }
                if (actor->xz_velocity > 30.0f) {
                    actor->xz_velocity *= 0.7f;
                    actor->disable_run = 0xC;
                }
                if ((actor->health != 0) && (func_100107F8((Actor *) actor) == 0)) {
                    D_800D1580 = 0xFF08030A;
                    func_1506E5FC();
                }
            } else {
                turnSize = actor->in_water;
                if (turnSize == 0) {
                    if (actor->unk28 > 60.0f) {
                        if ((moveDelta >= 0x5E01) && (moveDelta < 0xA200) && (turnSize != 0x64) /* matching: always true here, still emitted */ && (actor->xz_velocity > 14.0f)) {
                            actor->xz_velocity = 14.0f;
                        }
                    } else if ((actor->unk31C->unk17 == 0) && (actor->unk31C->unk1B3 == 0) && (actor->unk13C == 0) && (moveDelta >= 0x5E01) && (moveDelta < 0xA200) && (((scratch = actor->xz_velocity) > 10.0f) || ((actor->unk31C->unk4 == 0) && (scratch > 2.0f)))) {
                        if (scratch > 14.0f) {
                            actor->xz_velocity = 14.0f;
                        }
                        if ((actor->unk31C->unk4E == 0) && (actor->unk31C->unk11A == 0)) {
                            pushAnim = 0x7A;
                            if ((actor->unk81 == 0) && (actor->unk83 == 0)) {
                                actor->unk40 = (heading - 0x4000) * 0.005493164f;
                            }
                        }
                    }
                }
            }
        }
        D_800CC27C = (actor->unk184 >> 28) & 7;
        if (D_800CC27C >= 7) {
            D_800CC27C = 0;
        }
        actor->floorY = floorY;
        actor->unk31C->unk4F = 0;
        physics = actor->unk31C->unk4E;
        if ((physics & 0xF) == 1) {
            if (actor->gravity < 1.0f) {
                actor->gravity = 5.0f;
            }
            if ((floorY + 45.0f) < actor->unk118) {
                actor->unk31C->unk4F = 1;
                snapRange = 0.0f;
            }
        } else if (physics == 2) {
            if (floorY == -10000.0f) {
                actor->health = 0;
                func_150D5A6C(actor, 0xFF, 1);
                func_1507CD64(actor, 8);
            }
            if (actor->unk31C->unk50 <= 0) {
                func_1504CA60((Actor *) actor);
            }
            actor->unkCC = 0x1E;
            actor->unkCE = actor->unkCC;
            if ((D_800BE9E4 < actor->unk31C->unk12C) && (actor->health != 0)) {
                actor->unk31C->unk12C -= D_800BE9E4;
                actor->xz_velocity = 254.0f;
            } else {
                actor->unk31C->unk12C = 0;
            }
            if ((actor->unk31C->unk8C & 0x2000) && (actor->health != 0)) {
                func_150D7790(actor, actor->unk31C->unk75 & 0x7F);
            }
        }
        if ((actor->y_position < (floorY + snapRange)) && (actor->y_velocity < 0.0f) && ((snapRange > 20.0f) || (actor->unk28 == 0.0f))) {
            actor->y_position = floorY - 1.0f;
        }
        if ((actor->unk31C->unk4E == 2) || (actor->unk31C->matrix_physics != 0)) {
            func_1505B5F8((Actor *) actor, actor->floorY);
        } else {
            func_1505A9AC(actor, actor->floorY);
        }
        if (actor->unk31C->unk95 != 0) {
            actor->unkB8 = 0.0f;
            if (actor->stunned == 0) {
                if ((actor->camera != NULL) && (actor->camera->mode != 0x100)) {
                    climbInput = D_800CC284->stick_y;
                    climbIndex = (actor->unk31C->unk95 & ~0x80) - 1;
                    climb = &D_800D3098[climbIndex];
                    if (actor->unk31C->unk96 != 0) {
                        climbInput = 60.0f;
                    }
                    climbTop = (climb->y + climb->height);
                    animSpeed = (climbInput * 0.025f);
                    actor->unk1CC = actor->y_position;
                    if (actor->unk31C->unk97 != 0) {
                        actor->unk83 = 0xFF;
                        actor->disable_run = 0xC;
                        animSpeed = 1.0f;
                        actor->unk100 |= 0x20;
                        anim = 0x31A;
                    } else {
                        if ((climbTop <= actor->y_position) && (climbInput >= 0.0f)) {
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
                        actor->unk184 &= ~0x1F;
                        if (D_800BE9F0 == 0x23) {
                            actor->unk184 |= 9;
                        } else {
                            actor->unk184 |= 3;
                        }
                        if ((climbTop - 80.0f) <= actor->y_position) {
                            if (!(actor->unk31C->unk95 & 0x80)) {
                                actor->unk137 = 2;
                                animSpeed = 3.0f;
                                anim = 0x262;
                                actor->unk31C->unk95 = 0;
                            } else if (climbTop <= actor->y_position) {
                                animSpeed = 1.0f;
                                anim = 0x206;
                                actor->y_velocity = 0.0f;
                                actor->y_position = climbTop;
                            }
                        } else if (actor->y_position < climb->y) {
                            actor->disable_run = 0;
                            actor->unk31C->unk95 = 0;
                            actor->gravity = 4.0f;
                        }
                        if ((actor->unk31C->unk95 == 0) && (actor->in_water != 0)) {
                            actor->gravity = 0.0f;
                        }
                    }
                    if (anim != -1) {
                        func_1505E650((Actor *) actor, anim, animSpeed, 6.0f, 0.0f, 0.0f, 0);
                    }
                }
            }
            if (actor->unk28 == 0.0f) {
                if (actor->unk31C->unk97 == 0) {
                    actor->unk31C->unk95 = 0;
                    actor->disable_run = 6;
                    actor->gravity = 4.0f;
                }
            }
        }
    }
    if ((actor->unk28 == 0.0f) && (actor->unk31C->unk4E != 2) && (actor->health != 0) && ((actor->unk184 & 0x1F) == 0xE)) {
        func_1505D024(actor, 1, actor->unk7A, -1);
        actor->unk10B |= 0x80;
    }
    if ((actor->floorY < -9000.0f) && (actor->y_velocity < -120.0f)) {
        actor->unk31C->unk36 += D_800BE9A0;
        if (actor->unk31C->unk36 >= 0x24) {
            func_1506D538();
        }
    } else {
        actor->unk31C->unk36 = 0;
    }
    func_15051558(actor, startX, startZ, moveAngle);
    if (actor->unk31C->unk4E == 0) {
        if ((actor->unk28 > 5.0f) && (actor->unk137 == 0) && (actor->disable_run == 0) && (actor->unk31C->unk78 == 0) && (actor->unk31C->unk17 == 0) && (actor->stunned == 0) && (actor->unk31C->unk46 == 0) && (actor->in_water == 0) && (actor->unk13C == 0) && (actor->floorY != -10000.0f) && (actor->health != 0) && (actor->unk31C->unk8 == 0) && (actor->unk31C->unk4E == 0) && (actor->unk110 == 0) && ((actor->unk1CC - actor->y_position) < 450.0f)) {
            anim = 0;
            if (D_800CBDF4 != -32768.0f) {
                ledgeAngle = (((actor->unk40 + D_800CBD94) - 90.0f) / 1.40625f);
                ledgeY = D_800CBDF4;
                anim = 1;
            } else if ((D_800CBDF8 != -32768.0f) && (actor->xz_velocity < 10.0f)) {
                ledgeAngle = ((actor->unk40 + D_800CBD98 + 90.0f) / 1.40625f);
                ledgeY = D_800CBDF8;
                anim = 2;
            }
            if (anim != 0) {
                actor->unk76 = (ledgeAngle << 8);
                actor->unk7A = actor->unk76;
                actor->y_position = ledgeY - 25.0f;
                actor->x_position = D_800CBDFC;
                actor->z_position = D_800CBD90;
                actor->unk31C->unk53 = 1;
                actor->unk31C->unk55 = 0;
                actor->disable_run = 0xFF;
                actor->unk137 = 1;
                actor->unk81 = 1;
                actor->xz_velocity = 0.0f;
                actor->y_velocity = 0.0f;
                actor->gravity = 0.0f;
                actor->unk1CC = actor->y_position;
                func_1505E650((Actor *) actor, 0x42, 1.0f, 1.0f, 0.0f, 0.0f, 0);
                actor->unk31C->unk44 = 0xC;
            }
        }
        if (actor->y_velocity < 5.0f) {
            if ((actor->unk118 != -10000.0f) && (actor->y_position < (actor->unk118 - 60.0f)) && (actor->in_water != 1)) {
                splashSpeed = 4.0f;
                actor->unk31C->unk55 = 0;
                actor->unk31C->unk31 = 0;
                actor->in_water = 1;
                actor->unk164 = 0.0f;
                actor->unk168 = 0.0f;
                actor->unk162 = 0;
                actor->unk1CC = actor->y_position;
                actor->xz_velocity *= 0.5f;
                actor->unk76 = actor->unk7A;
                actor->unk31C->unk22 = 0;
                if (D_800BE9B4 == 0) {
                    actor->unk31C->unk21 = 1;
                    actor->unkB2 = 0;
                }
                if ((actor->unk31C->unk20 & 1) && (D_800BE616 == 0)) {
                    func_15083568((Actor *) actor, 0x38, 0.0f, 0);
                    splashSpeed = 4.0f;
                }
                if (actor->unk110 == 2) {
                    actor->unk76 ^= 0x8000;
                    actor->unk7A = actor->unk76;
                    actor->xz_velocity = 20.0f;
                    actor->unkB8 = -30.0f;
                }
                actor->unk110 = 0;
                if ((actor->unk84.uh == 0x3C) || (actor->unk84.uh == 0x3B)) {
                    splashSpeed = 0.0f;
                    actor->y_velocity *= 0.5f;
                    actor->gravity = -2.4f;
                    actor->unkB8 = 89.0f;
                } else {
                    actor->y_velocity *= 0.4f;
                    actor->gravity = -3.0f;
                }
                actor->stunned = 0;
                actor->target_speed = 0.0f;
                actor->unk81 = 0;
                actor->unkA9 = 0;
                if ((actor->unk31C->unk95 == 0) && (actor->health != 0)) {
                    func_1506B100(0xD1, 2.0f, splashSpeed);
                    actor->disable_run = 0;
                }
                if (actor->health == 0) {
                    actor->disable_run = 0xFF;
                    func_1507CD64(actor, 4);
                    actor->stunned = 0xFF;
                }
            }
        }
    }
    func_1505250C((Actor *) actor, 1);
    if (actor->unkCC < -0x5D) {
        actor->health = 0;
        func_1507CD64(actor, 3);
    }
    if (actor->unkD0 != 0) {
        func_1505C7D8((Actor *) actor, D_800C3E78);
    }
    if (actor->unk7F >= 9) {
        actor->unk80 = 1;
    }
    if (actor->unk83 != 0) {
        if (actor->unk83 != 0xFF) {
            if (D_800BE9A0 >= actor->unk83) {
                actor->unk83 = 0;
            } else {
                actor->unk83 -= D_800BE9A0;
            }
        }
        if ((actor->unk84.uh == 0x36) || (actor->unk84.uh == 0x19D)) {
            if ((actor->xz_velocity > 8.0f) && (actor->disable_run == 0)) {
                actor->unk83 = 0;
                moveSpeed = actor->xz_velocity;
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
        actor->gravity = 5.0f;
        if (actor->unk102 == 0) {
            func_1506AF74();
        }
    }
    if (actor->unk100 & 1) {
        if (actor->unk31C->unk78 == 0) {
            if ((actor->unk31C->unk11A != 1) && (actor->unk31C->unk11A != 3) && (actor->in_water == 0) && (actor->stunned == 0) && (D_800CC2B1 == 0) && ((actor->disable_run == 0) || (actor->unk84.uh == 0x279)) && (actor->unk31C->unk8 <= 0) && (actor->unk28 == 0.0f) && (D_800CC284->buttons & 0x2000)) {
                actor->unk81 = 0;
                actor->unk83 = 0;
                actor->unk100 &= 0xFE;
                actor->unk76 = actor->unk7A;
            }
        }
    }
    if (((actor->unk1CC - actor->y_position) > 600.0f) && (actor->unk31C->unk120 == 0)) {
        if ((actor->unk31C->unk11A != 1) && (actor->unk31C->unk11A != 3) && (actor->unk118 == -10000.0f)) {
            if ((actor->unk84.uh != 0x101) && (actor->in_water == 0)) {
                if (actor->unk84.uh != 0xCF) {
                    sound = 0x2007E5;
                    if (func_1506E46C(actor, &sound, 1) != 0) {
                        func_15060A30(sound, actor);
                    }
                    { BLK; }
                }
                actor->unk31C->unk31 = 0;
                func_1505E650((Actor *) actor, 0xCF, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                actor->unk83 = 0xC8;
            }
        }
    }
    actor->unk31C->unk4 = 0;
    func_1504C0E8((Actor *) actor, pushAnim, moveSpeed);
    actor->unk31C->unk0 = 2.0f * sqrtf(((actor->x_position - actor->old_x_position) * (actor->x_position - actor->old_x_position)) + ((actor->z_position - actor->old_z_position) * (actor->z_position - actor->old_z_position)));
    actor->unk31C->unk32 = 0;
    if ((actor->unk31C->unk84 != 0) || (padOverridden != 0)) {
        *D_800CC284 = savedPad;
    }
    actor->unk31C->prevButtons = buttons;
}

/* Tilts the actor toward the slope of the ground under it (pitch/roll and the unk164/unk168 lean). */
void func_150511E8(Actor *actor) {
    f32 slopeX;
    f32 slopeZ;
    f32 normalX;
    f32 normalZ;
    f32 leanRate;

    if (actor->unk28 <= 0.0f) {
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
        if ((actor->id == 0x28) || (actor->id == 0x77)) {
            gain = 0.8999999762f;
            damping = 0.200000003f;
            scale = 7.0f;
            rise = 0.6000000238f;
        }
        func_1510F820(actor->unk188, actor->floorVerts, &normalX, &normalZ, &leanRate, &slopeX, &slopeZ);
        if (actor->unkF8 & 0x100000) {
            f32 pitch;

            func_1510F820(actor->unk188, actor->floorVerts, &normalX, &normalZ, &leanRate, &slopeX, &slopeZ);
            roll = func_1505210C(actor, slopeX, slopeZ, &pitch, 50.0f);
            pitch *= gain;
            actor->unkB8 += (pitch - actor->unkB8) * damping;
            actor->unkC4 += (roll * gain - actor->unkC4) * damping;
        }
        if (!(actor->unkF8 & 0x80000)) {
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
            if (clamped && (actor->id == 0x77)) {
                if (actor->xz_velocity > 15.0f) {
                    actor->xz_velocity *= 0.8000000119f;
                }
                if (actor->disable_run < 8) {
                    actor->disable_run = 6;
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
    func_1505A250(-slopeX, -slopeZ, leanRate, &actor->unk164, &actor->unk168);
}

extern f32 D_800990C0[];
extern f32 D_800990E0[];
void func_15087EF0(s32, void *);

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
    height = actor->unk28;
    pitch = 0.0f;
    if (actor->in_water == 0) {
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
        if (actor->unk31C->unk4E == 2) {
            func_1510F820(actor->unk188, actor->floorVerts, &normalX, &normalZ, &leanRate, &roll, &pitch);
            if ((((roll * roll) + (pitch * pitch)) > 1.39999998f) && ((roll * roll) + (pitch * pitch) < 1000.0f)) {
                actor->disable_jump = 6;
                if ((height < 80.0f) && (actor->disable_run == 0)) {
                    func_15087EF0(D_800C3E78, actor);
                }
            }
            if (height >= 0.0f) {
                normalX = 0.0f;
                normalZ = 0.0f;
                roll = 0.0f;
                pitch = 0.0f;
                leanRate = 3.0f;
            }
        } else if (((height <= 0.0f) || (actor->unk110 != 0)) && (actor->unk31C->unk4F == 0)) {
            func_1510F820(NULL, actor->floorVerts, &normalX, &normalZ, &leanRate, &roll, &pitch);
        }
        if ((actor->unk31C->unk4E == 0) && (actor->unk31C->unk84 == 0) &&
            (((D_800CC27C == 0) && ((((roll * roll) + (pitch * pitch)) > 1.0f) || (((actor->unk184 >> 12) & 0xF) == 4))) ||
             ((D_800CC27C != 0) && (actor->unk31C->unk17 != 0))) &&
            (((actor->unk184 >> 12) & 0xF) != 3)) {
            D_800CC27C = 1;
            bumped = 1;
        }
        kind = (actor->objectNum != 0) ? D_800CC2D0[actor->objectNum - 1].id : -1;
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
        if ((D_800CC27C != 0) && (actor->unk28 < 5.0f) && (actor->unk110 != 0)) {
            actor->unk162 = 1;
            if (actor->disable_run != 0xFE) {
                actor->disable_run = D_800BE9A0 + 1;
            }
        } else {
            actor->unk162 = 0;
        }
        dx = x - actor->old_x_position;
        dy = actor->y_position - actor->old_y_position;
        dz = z - actor->old_z_position;
        moved = 2.0f * sqrtf((dx * dx) + (dy * dy) + (dz * dz)) * D_800BE9A8;
        if (actor->unk162 - wasSliding) { // matching: the original tests the difference (subu + bnez), not !=
            if (actor->unk162 != 0) {
                actor->unk164 = dx * D_800BE9A8;
                actor->unk168 = dz * D_800BE9A8;
                actor->xz_velocity = 0.0f;
            } else {
                actor->xz_velocity = moved;
                if (moved < 22.0f) {
                    actor->xz_velocity = 22.0f;
                }
                if (actor->unk110 == 3) {
                    actor->unk110 = 0;
                    if (actor->disable_run != 0xFE) {
                        actor->disable_run = 0;
                    }
                    actor->xz_velocity = 17.8999996f;
                }
                actor->unk76 = func_1505A630(x - actor->old_x_position, actor->old_z_position - z, 0);
                actor->unk164 = 0.0f;
                actor->unk168 = 0.0f;
                actor->unk16C = 0.0f;
                actor->unk170 = 0.0f;
            }
        }
        if ((D_800CC27C != 0) && (actor->unk28 < 5.0f)) {
            animSpeed = 1.0f;
            actor->disable_jump = D_800BE9A0 + 8;
            actor->unk112 = func_1505A630(-normalX, normalZ, 0);
            moveAngle = func_1505A630(actor->x_position - actor->old_x_position, actor->old_z_position - actor->z_position, 0);
            D_800CC27C = 1;
            if (actor->unk110 == 0) {
                leanRate = D_800990C0[D_800CC27C];
                if ((u8)(((actor->unk112 - moveAngle) >> 8) + 0x30) < 0x60) {
                    heading = (actor->unk44 > 8.0f) ? facing : actor->unk7A;
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
                    if ((actor->stunned == 0) && (actor->unk13C == 0) && (actor->disable_run != 0xFF) && (actor->unk83 < 0xFA)) {
                        if ((actor->unk31C->unk11A != 1) && (actor->unk31C->unk11A != 3) && (actor->unk31C->unk120 == 0) && (actor->unk31C->unk19B == 0)) {
                            func_1505E650((Actor *)actor, anim, animSpeed, 6.0f, 0.0f, 0.0f, 0);
                        }
                    }
                }
            } else {
                leanRate = D_800990E0[D_800CC27C];
            }
        }
        func_1505A250(-normalX, -normalZ, leanRate, &actor->unk164, &actor->unk168);
        actor->unk16C += (0.0f - actor->unk16C) * 0.150000006f;
        actor->unk170 += (0.0f - actor->unk170) * 0.150000006f;
        if (actor->unk110 != 0) {
            actor->unk31C->unk3E = func_1505A630(actor->x_position - actor->old_x_position, actor->old_z_position - actor->z_position, 0);
            if ((actor->unk84.uh == 0x28E) || (actor->unk84.uh == 0x101) || (actor->unk84.uh == 0xD0)) {
                if ((actor->unk31C->unk11A != 1) && (actor->unk31C->unk11A != 3)) {
                    actor->unk83 = D_800BE9A0 + 1;
                }
            }
            if (D_800CC27C == 0) {
                if (actor->unk110 == 2) {
                    actor->unk31C->unk3E = actor->unk76 = actor->unk112;
                    if ((actor->xz_velocity < 2.0f) && (actor->unk28 == 0.0f)) {
                        if (actor->stunned == 0) {
                            func_1505E650((Actor *)actor, 0xA5, 1.39999998f, 5.0f, 0.0f, 0.0f, 0);
                            actor->unk83 = 0x3C;
                            actor->disable_run = 0x3C;
                            actor->disable_jump = 0x1E;
                            actor->unk76 ^= 0x8000;
                            actor->unk7A = actor->unk76;
                            actor->unk100 |= 0x20;
                        }
                        actor->unk110 = 0;
                    }
                } else if (actor->xz_velocity < 12.0f) {
                    actor->unk110 = 0;
                }
            }
        }
        if (height <= 0.0f) {
            roll = func_1505210C((Actor *)actor, roll, pitch, &pitch, 50.0f);
            if (actor->unk31C->unk4E != 2) {
                if ((actor->unk31C->unk4 == 0) && (actor->unk110 == 0) && ((actor->unk31C->unk4E & 0xF) != 1)) {
                    roll *= (actor->unk31C->unk0 * 0.00899999961f) + 0.200000003f;
                    pitch *= (actor->unk31C->unk0 * 0.0140000004f) + 0.200000003f;
                } else {
                    roll *= 0.800000012f;
                    pitch *= 0.800000012f;
                    if ((actor->unk31C->unk4E & 0xF) == 1) {
                        roll *= 0.5f;
                        pitch = 0.0f;
                    }
                }
            }
        }
        if (actor->unk31C->unk4E == 2) {
            {
                f32 yVelocity = actor->y_velocity;
                sway = actor->unk54 * 6.0f;
                if (yVelocity < -30.0f) {
                    pitch -= -30.0f;
                } else {
                    pitch -= yVelocity;
                }
            }
            blend = 0.400000006f;
            if (actor->unk31C->unk44 > 0) {
                shake = (D_800CC250 & 4);
                shake -= 2;
                shake *= (u32)(actor->unk31C->unk44 * 2); // matching: unsigned multiply (multu)
                sway += shake;
                blend = 0.25f;
            }
            if ((fabsf(actor->unkC4) > 70.0f) && (D_800BE616 == 0) && (actor->health != 0) && (actor->unk31C->unk24 == 0)) {
                D_800D1580 = 0xFF020146;
                func_1506E8D8();
                actor->unk31C->unk24 = 1;
            }
            if (fabsf(actor->unkC4) < 20.0f) {
                actor->unk31C->unk24 = 0;
            }
            roll += sway;
        }
        actor->unkB8 += (pitch - actor->unkB8) * blend;
    } else {
        f32 depth;

        depth = (actor->unk118 - 30.0f) - actor->y_position;
        depth += actor->xz_velocity * 6.0f;
        if (depth > 250.0f) {
            depth = 2.5f;
        } else {
            depth *= 0.00999999978f;
        }
        if ((fabsf(actor->unkB8) < 35.0f) && (depth > 0.0f)) {
            roll += actor->unk54 * depth;
        }
        func_1505A250(0.0f, 0.0f, 0.300000012f, &actor->unk164, &actor->unk168);
    }
    actor->unkC4 += (roll - actor->unkC4) * blend;
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
    yaw = actor->unk40 * 0.01745329238f;
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
