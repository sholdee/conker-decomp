#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Aim camera: the camera mode that swings behind the player (func_1511FC60 enters it, func_15120158
 * runs it each frame, func_151218C4 leaves it). Camera.aimState is the state machine:
 *   1 = swinging round to the aim view, 2 = aiming with the stick, 3 = swinging back, 0 = off.
 */

#define DEG2RAD 0.01745329238f
#define BAM2DEG 0.005493164f /* 360 / 65536: 16-bit binary angle to degrees */

/* Zones whose aim yaw follows the player's facing angle; func_151C9BA0 runs while in one of them. */
#define IS_FACING_ZONE(z)                                                                                  \
    (((z) == 9) || ((z) == 0x38) || ((z) == 0x39) || ((z) == 0x37) || ((z) == 0x3B) || ((z) == 0x15) || \
     ((z) == 0x26) || ((z) == 0x3A))

/* Player extension block (struct127.unk31C, cached in Camera.playerExt); layout as in game_14FF90.c. */
typedef struct CamPlayerExt {
    /* 0x000 */ u8 pad0[0x12];
    /* 0x012 */ s16 unk12;
    /* 0x014 */ u8 pad14[0x78 - 0x14];
    /* 0x078 */ u8 cameraZone;
    /* 0x079 */ u8 pad79[0x114 - 0x79];
    /* 0x114 */ s16 unk114;
    /* 0x116 */ u8 pad116[0x130 - 0x116];
    /* 0x130 */ vertex aimDir;
    /* 0x13C */ vertex aimEye;
    /* 0x148 */ vertex aimTarget;
    /* 0x154 */ u8 pad154[0x16C - 0x154];
    /* 0x16C */ f32 aimYaw;
    /* 0x170 */ f32 aimPitch;
    /* 0x174 */ u8 pad174[0x198 - 0x174];
    /* 0x198 */ u8 aimActive;
    /* 0x199 */ u8 pad199[0x1A2 - 0x199];
    /* 0x1A2 */ u16 unk1A2;
} CamPlayerExt;

/* Aim-mode state, embedded in Camera at 0x740. */
typedef struct CamAim {
    /* 0x00 */ vertex orbitPos;      /* eye position on the orbit around the target */
    /* 0x0C */ vertex offset;        /* sideways shift of the aim ray (x and z only) */
    /* 0x18 */ f32 yawVel;
    /* 0x1C */ f32 pitchVel;
    /* 0x20 */ f32 savedYaw;         /* view to return to on exit */
    /* 0x24 */ f32 savedTargetDistance;
    /* 0x28 */ f32 savedHeight;
    /* 0x2C */ f32 savedUnk134;
    /* 0x30 */ f32 savedUnk354;
    /* 0x34 */ f32 targetYaw;
    /* 0x38 */ f32 targetPitch;
    /* 0x3C */ f32 pitch;
    /* 0x40 */ f32 signedPitch;      /* pitch in -180..180 */
    /* 0x44 */ f32 distance;
    /* 0x48 */ f32 turnDistance;     /* swing round once Camera.distance passes this */
    /* 0x4C */ u8 useOverride;       /* take pitch, yaw offset and distance from the fields below */
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ s8 approachDir;       /* -1: distance grows towards turnDistance, 1: it shrinks */
    /* 0x4F */ u8 pad4F;
    /* 0x50 */ f32 overridePitch;
    /* 0x54 */ f32 overrideYawOffset;
    /* 0x58 */ f32 overrideDistance;
    /* 0x5C */ f32 offsetXVel;
    /* 0x60 */ f32 offsetZVel;
    /* 0x64 */ s32 stickLock;        /* bit 0: stick_x held at <= 0, bit 1: at >= 0 */
} CamAim; // size 0x68

/* File-local view of struct108 (the camera record) with the fields this file uses; names follow
   game_14FF90.c's Camera. include/structs.h has most of these offsets as padding. */
typedef struct Camera {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 mode;
    /* 0x030 */ s32 savedMode[21];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0x134 - 0x88];
    /* 0x134 */ s32 unk134;
    /* 0x138 */ s32 savedUnk134[21];
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ f32 unk190;
    /* 0x194 */ f32 unk194;
    /* 0x198 */ f32 unk198;
    /* 0x19C */ u8 pad19C[0x23C - 0x19C];
    /* 0x23C */ u8 snap;             /* nonzero: jump to targets instead of easing */
    /* 0x23D */ u8 playerIndex;
    /* 0x23E */ u8 zone;             /* copy of playerExt->cameraZone */
    /* 0x23F */ u8 pad23F[0x2BC - 0x23F];
    /* 0x2BC */ vertex target;       /* look-at point */
    /* 0x2C8 */ u8 pad2C8[0x2F8 - 0x2C8];
    /* 0x2F8 */ vertex eye;          /* camera position */
    /* 0x304 */ vertex prevEye;
    /* 0x310 */ u8 pad310[0x344 - 0x310];
    /* 0x344 */ f32 unk344;
    /* 0x348 */ f32 height;
    /* 0x34C */ u8 pad34C[0x354 - 0x34C];
    /* 0x354 */ f32 unk354;
    /* 0x358 */ f32 unk358;
    /* 0x35C */ f32 unk35C;
    /* 0x360 */ f32 unk360;
    /* 0x364 */ u8 pad364[0x36C - 0x364];
    /* 0x36C */ OSContPad *contPad;
    /* 0x370 */ f32 distance;
    /* 0x374 */ f32 targetDistance;
    /* 0x378 */ u8 pad378[0x37C - 0x378];
    /* 0x37C */ f32 yaw;             /* degrees */
    /* 0x380 */ u8 pad380[0x39C - 0x380];
    /* 0x39C */ f32 yawRad;
    /* 0x3A0 */ u8 pad3A0[0x3D0 - 0x3A0];
    /* 0x3D0 */ struct127 *player;
    /* 0x3D4 */ CamPlayerExt *playerExt;
    /* 0x3D8 */ u8 pad3D8[0x3E8 - 0x3D8];
    /* 0x3E8 */ u8 unk3E8;
    /* 0x3E9 */ u8 pad3E9[0x5F0 - 0x3E9];
    /* 0x5F0 */ s32 status;
    /* 0x5F4 */ u8 pad5F4[0x600 - 0x5F4];
    /* 0x600 */ u8 blocked;          /* wall between focus and eye this frame */
    /* 0x601 */ u8 pad601[0x3];
    /* 0x604 */ f32 blockedDistance;
    /* 0x608 */ u8 pad608[0x6FC - 0x608];
    /* 0x6FC */ s32 unk6FC;
    /* 0x700 */ u8 pad700[0x73C - 0x700];
    /* 0x73C */ s16 aimState;
    /* 0x73E */ u8 pad73E[0x2];
    /* 0x740 */ CamAim aim;
    /* 0x7A8 */ u8 pad7A8[0x7B4 - 0x7A8];
    /* 0x7B4 */ f32 dt;              /* frame time scale */
    /* 0x7B8 */ u8 pad7B8[0x7D0 - 0x7B8];
    /* 0x7D0 */ f32 unk194Vel;
    /* 0x7D4 */ f32 unk18CVel;
    /* 0x7D8 */ u8 pad7D8[0x84A - 0x7D8];
    /* 0x84A */ s16 aimTimer;        /* frames left before the swing gives up */
    /* 0x84C */ u8 zoneHookActive;   /* func_151C9BA0 ran; func_151C9ED4 undoes it */
    /* 0x84D */ u8 pad84D[0x898 - 0x84D];
    /* 0x898 */ f32 unk898;
    /* 0x89C */ f32 unk89C;
    /* 0x8A0 */ u8 pad8A0[0x8B4 - 0x8A0];
    /* 0x8B4 */ f32 unk8B4;
    /* 0x8B8 */ u8 pad8B8[0x92C - 0x8B8];
    /* 0x92C */ u8 unk92C;
    /* 0x92D */ u8 pad92D[0x99C - 0x92D];
    /* 0x99C */ f32 sideOffsetTarget;
} Camera; // size 0x9A0

#define CAMERA_CHECK(name, cond) typedef char name[(cond) ? 1 : -1]
CAMERA_CHECK(Camera_size, sizeof(Camera) == 0x9A0);
CAMERA_CHECK(CamAim_size, sizeof(CamAim) == 0x68);
CAMERA_CHECK(off_aim, (u32)&((Camera *)0)->aim == 0x740);
CAMERA_CHECK(off_aimTimer, (u32)&((Camera *)0)->aimTimer == 0x84A);
CAMERA_CHECK(off_aimPitch, (u32)&((CamPlayerExt *)0)->aimPitch == 0x170);

void func_151C9BA0(Camera *camera, s32 zone);
void func_151C9ED4(Camera *camera);
void func_151216F8(Camera *camera);
void func_151218C4(Camera *camera);
void func_1510E7A4(s32, s32, f32 *, void *, s32, s32, f32, f32, f32, f32, s32, s32, f32, f32);
void func_150495B0(f32 *, f32, f32 *, f32, f32, f32);
void func_15123A54(Camera *camera);
void func_1512E140(Camera *camera);

/* Enters aim mode: saves the current view and picks the aim yaw, pitch and distance for the zone. */
void func_1511FC60(Camera *camera) {
    CamAim *aim;
    s32 zone;

    aim = &camera->aim;
    zone = camera->zone;
    camera->aimTimer = 60;
    if ((camera->status & 2) || (camera->zone == 9) || (camera->zone == 0x38) || (camera->zone == 0x39) || (camera->zone == 0x37) || (camera->zone == 0x3B)) {
        // matching: the original stores 0 then 1; IDO drops the dead store unless it is volatile
        *(volatile u8 *)&aim->unk4D = 0;
        aim->unk4D = 1;
    } else {
        aim->unk4D = 1;
    }
    camera->zoneHookActive = FALSE;
    if (camera->savedMode[1] == 0x40) {
        aim->targetYaw = camera->yaw;
    } else if (aim->unk4D) {
        if (IS_FACING_ZONE(camera->zone)) {
            aim->targetYaw = (f32)((camera->player->unk7A - camera->playerExt->unk12) + 0xC000) * BAM2DEG;
        } else if ((D_800D1940 == 0x42) && (zone == 0x1A)) {
            aim->targetYaw = camera->player->unk40;
        } else {
            aim->targetYaw = camera->player->unk40 - 180.0f;
        }
    } else {
        aim->targetYaw = camera->yaw;
        camera->unk84 &= ~0x280;
    }
    if (IS_FACING_ZONE(zone)) {
        aim->targetPitch = 0.0f;
        aim->pitch = 0.0f;
        func_151C9BA0(camera, zone);
        camera->zoneHookActive = TRUE;
    } else if ((zone == 2) || (zone == 0x13)) {
        aim->targetPitch = 0.0f;
        aim->pitch = 0.0f;
    } else if (zone == 0x1C) {
    } else if (zone == 0x34) {
        aim->targetPitch = 0.0f;
        aim->pitch = 0.0f;
    } else if (zone == 0xF) {
        aim->targetPitch = 27.0f;
        aim->pitch = 27.0f;
        aim->targetYaw = camera->player->unk40 + 225.0f;
    } else {
        aim->targetPitch = 0.0f;
        aim->pitch = 0.0f;
    }
    func_151216F8(camera);
    func_15048758(&aim->targetYaw);
    if (aim->useOverride) {
        aim->targetPitch = aim->overridePitch;
        aim->pitch = aim->overridePitch;
        aim->distance = aim->overrideDistance;
        aim->targetYaw += aim->overrideYawOffset;
    }
    aim->savedYaw = camera->yaw;
    aim->savedUnk134 = camera->unk134;
    if (camera->unk92C) {
        aim->savedTargetDistance = sqrtf((camera->target.x - camera->eye.x) * (camera->target.x - camera->eye.x) + (camera->target.z - camera->eye.z) * (camera->target.z - camera->eye.z));
        aim->savedHeight = camera->unk344;
        aim->savedUnk354 = camera->unk354;
    } else {
        aim->savedTargetDistance = camera->targetDistance;
        aim->savedHeight = camera->height;
        aim->savedUnk354 = camera->unk354;
    }
    camera->unk35C = camera->target.y;
    camera->unk354 = camera->target.y;
    aim->distance = 110.0f;
    camera->height = 30.0f;
    camera->playerExt->aimYaw = aim->targetYaw;
    if ((zone == 0) || (zone == 0xC) || IS_FACING_ZONE(zone)) {
        camera->height += 30.0f;
    }
    aim->stickLock = 0;
    camera->aimState = 1;
    if (camera->snap) {
        aim->targetYaw = camera->player->unk40 - 180.0f;
        camera->yaw = aim->targetYaw;
        camera->yawRad = camera->yaw * DEG2RAD;
        camera->unk194 = camera->unk198;
        camera->unk18C = camera->unk190;
    }
    if (aim->distance < camera->distance) {
        aim->turnDistance = aim->distance * 1.2f;
        aim->approachDir = -1;
    } else {
        aim->turnDistance = aim->distance * 0.6666659713f;
        aim->approachDir = 1;
    }
    if ((zone == 0xA) || (zone == 2) || (zone == 0x12) || (zone == 3) || (zone == 0x34)) {
        aim->turnDistance = camera->targetDistance;
        aim->approachDir = -1;
    }
    camera->unk84 &= ~0x280;
    camera->targetDistance = aim->distance;
}

/* Per-frame update of the aim camera (see the state machine at the top of the file). */
void func_15120158(Camera *camera) {
    CamAim *aim;
    f32 probeY;
    f32 dist;
    s32 zone;
    s32 inMap42Zone1A;
    f32 yawDelta;

    aim = &camera->aim;
    zone = camera->zone;
    inMap42Zone1A = (D_800D1940 == 0x42) && (zone == 0x1A);
    func_1510E7A4(0, 0, &probeY, &camera->unk360, 0, 0, camera->eye.x, camera->eye.y, camera->eye.z, camera->eye.y, 0, 0, -10000.0f, camera->eye.y);
    camera->playerExt->aimActive = camera->aimState;
    camera->distance = sqrtf((camera->eye.x - camera->target.x) * (camera->eye.x - camera->target.x) + (camera->eye.z - camera->target.z) * (camera->eye.z - camera->target.z));
    if (camera->snap) {
        aim->savedUnk354 = probeY;
    }
    if ((zone != 0x2A) && (camera->aimState != 3)) {
        if (zone == 0xD) {
            camera->unk190 = 100.0f;
        } else if (zone == 3) {
            aim->targetPitch = 15.0f;
            camera->unk198 = 0.0f;
            aim->distance = 350.0f;
            if (camera->blocked) {
                aim->pitch *= camera->blockedDistance * 0.002857142827f;
                aim->pitch = (aim->pitch < 0.0f) ? 0.0f : aim->pitch;
            }
        } else if (inMap42Zone1A) {
            camera->unk190 = 18.0f;
            camera->unk198 = 60.0f;
            aim->distance = 250.0f;
            aim->targetYaw = camera->player->unk40;
        } else if (zone == 0x1A) {
            camera->unk190 = 35.0f;
            camera->unk198 = 100.0f;
            aim->distance = 250.0f;
        } else {
            camera->unk190 = 20.0f;
            camera->unk198 = 0.0f;
            func_151216F8(camera);
        }
    }
    camera->unk35C = camera->unk354 = camera->target.y;
    func_1506160C(camera->player, 2, 0xFF, 0x20, camera->playerIndex);
    dist = aim->distance;
    if (camera->aimState == 1) {
        s32 horizontal;
        f32 sinPitch;
        s32 passed;

        if (camera->aimTimer != 0) {
            camera->aimTimer -= D_800BE9E4;
            if (camera->aimTimer <= 0) {
                camera->playerExt->aimActive = 1;
                camera->aimState = 2;
                aim->targetYaw = camera->yaw;
                return;
            }
        }
        camera->unk134 = 1;
        horizontal = cosf(aim->pitch * DEG2RAD) * dist;
        sinPitch = sinf(aim->pitch * DEG2RAD);
        camera->targetDistance = horizontal;
        camera->height = (s32)(sinPitch * dist);
        if (camera->snap) {
            if (inMap42Zone1A) {
                aim->targetYaw = camera->player->unk40;
            } else {
                aim->targetYaw = camera->player->unk40 - 180.0f;
            }
            camera->yaw = aim->targetYaw;
            camera->yawRad = camera->yaw * DEG2RAD;
            camera->unk194 = camera->unk198;
            camera->unk18C = camera->unk190;
        }
        // matching: a goto to the end of this branch makes IDO duplicate the yawRad store; break does not
        do {
            if (aim->unk4D) {
                // matching: the original compares the yaw error and does nothing with the result
                if (fabsf(func_15048A70(camera->yaw, aim->targetYaw)) < 0.009999999776f) {
                }
            } else {
                camera->playerExt->aimActive = 1;
                camera->aimState = 2;
                break;
            }
            if (aim->approachDir == -1) {
                passed = camera->distance < aim->turnDistance;
            } else {
                passed = aim->turnDistance < camera->distance;
            }
            // matching: the redundant & 0xFF uses up one ugen temp, which fixes the t-register rotation
            if (passed || (aim->useOverride & 0xFF)) {
                if (camera->blocked) {
                    func_15049688(&camera->yaw, aim->targetYaw, &aim->yawVel, 10.0f, 18.0f, camera->dt);
                } else {
                    func_15049688(&camera->yaw, aim->targetYaw, &aim->yawVel, 5.0f, 9.0f, camera->dt);
                }
                func_150495B0(&camera->unk194, camera->unk198, &camera->unk194Vel, 5.0f, 9.0f, camera->dt);
                func_150495B0(&camera->unk18C, camera->unk190, &camera->unk18CVel, 5.0f, 9.0f, camera->dt);
                camera->yawRad = camera->yaw * DEG2RAD;
            }
        } while (0);
    } else if (camera->aimState == 2) {
        f32 horizontal;
        f32 sinPitch;
        f32 rate;
        s32 notUnk3E8;
        s32 stickX;
        OSContPad *pad;

        if (zone) {
            rate = 0.01279999968f;
        } else {
            rate = 0.02500000037f;
        }
        if (camera->snap) {
            if (inMap42Zone1A) {
                aim->targetYaw = camera->player->unk40;
            } else {
                aim->targetYaw = camera->player->unk40 - 180.0f;
            }
            camera->yaw = aim->targetYaw;
            camera->yawRad = camera->yaw * DEG2RAD;
            camera->unk194 = camera->unk198;
            camera->unk18C = camera->unk190;
        }
        if (IS_FACING_ZONE(zone)) {
            if (!camera->zoneHookActive) {
                func_151C9BA0(camera, zone);
                camera->zoneHookActive = TRUE;
            }
        } else if (camera->zoneHookActive) {
            func_151C9ED4(camera);
            camera->zoneHookActive = FALSE;
        }
        if (zone != 0xD) {
            // matching: the negated copy in a local fixes the register allocation of this block
            notUnk3E8 = !camera->unk3E8;
            if (!aim->unk4D) {
                if (!notUnk3E8) {
                    aim->stickLock = 0;
                } else {
                    if (aim->stickLock == 0) {
                        stickX = camera->contPad->stick_x;
                        aim->stickLock |= (stickX > 0) ? 1 : ((stickX < 0) ? 2 : 0);
                    }
                    camera->eye = camera->prevEye;
                    if (aim->stickLock & 1) {
                        pad = camera->contPad;
                        stickX = pad->stick_x;
                        if (stickX > 0) {
                            pad->stick_x = 0;
                        } else {
                            pad->stick_x = stickX;
                        }
                    } else if (aim->stickLock & 2) {
                        pad = camera->contPad;
                        stickX = pad->stick_x;
                        if (stickX < 0) {
                            pad->stick_x = 0;
                        } else {
                            pad->stick_x = stickX;
                        }
                    }
                }
            }
            if (IS_FACING_ZONE(zone)) {
                aim->targetYaw = (f32)((camera->player->unk7A - camera->playerExt->unk12) + 0xC000) * BAM2DEG;
            } else if (inMap42Zone1A) {
                aim->targetYaw = camera->player->unk40;
            } else if (zone != 0xF) {
                if (camera->status & 2) {
                    aim->targetYaw = (((f32)(u32)camera->player->unk7A - (f32)camera->playerExt->unk12 * 0.349999994f) + 49152.0f) * BAM2DEG;
                } else {
                    if (zone == 0x34) {
                        aim->targetYaw = aim->targetYaw - (f32)camera->contPad->stick_x * rate * (f32)D_800BE9E4;
                    }
                    aim->targetYaw = aim->targetYaw - (f32)camera->contPad->stick_x * rate * (f32)D_800BE9E4;
                }
            }
            if ((zone != 3) && (zone != 0x1A) && !inMap42Zone1A && (zone != 0xF)) {
                aim->targetPitch += (f32)camera->contPad->stick_y * rate * (f32)D_800BE9E4;
            }
            if (zone == 0x38) {
                if (camera->contPad->button & Z_TRIG) {
                    sinPitch = camera->unk8B4 - (f32)(((CamPlayerExt *)camera->player->unk31C)->unk1A2 >> 8) * 1.098632812f;
                } else {
                    sinPitch = 0.0f;
                }
                aim->targetPitch += sinPitch;
                camera->unk8B4 = (f32)(((CamPlayerExt *)camera->player->unk31C)->unk1A2 >> 8) * 1.098632812f;
            }
            func_15048758(&aim->targetYaw);
            if (zone == 0x34) {
                aim->targetPitch = (aim->targetPitch < -5.0f) ? -5.0f : ((aim->targetPitch > 15.0f) ? 15.0f : aim->targetPitch);
            } else if (zone == 0x12) {
                aim->targetPitch = (aim->targetPitch < -40.0f) ? -40.0f : ((aim->targetPitch > 15.0f) ? 15.0f : aim->targetPitch);
            } else if ((zone == 2) || (zone == 0xA) || (zone == 0x13)) {
                aim->targetPitch = (aim->targetPitch < -37.0f) ? -37.0f : ((aim->targetPitch > 30.0f) ? 30.0f : aim->targetPitch);
            } else {
                aim->targetPitch = (aim->targetPitch < -45.0f) ? -45.0f : ((aim->targetPitch > 80.0f) ? 80.0f : aim->targetPitch);
            }
            if ((D_800BE616 == 0) && IS_FACING_ZONE(zone)) {
                aim->targetPitch = (aim->targetPitch < -20.0f) ? -20.0f : aim->targetPitch;
            }
            if (camera->snap) {
                camera->yaw = aim->targetYaw;
                aim->pitch = aim->targetPitch;
                camera->unk194 = camera->unk198;
                camera->unk18C = camera->unk190;
            } else if (zone || inMap42Zone1A) {
                func_15049688(&camera->yaw, aim->targetYaw, &aim->yawVel, 3.91f, 8.210000038f, camera->dt);
                func_15049688(&aim->pitch, aim->targetPitch, &aim->pitchVel, 3.91f, 8.210000038f, camera->dt);
                func_150495B0(&camera->unk194, camera->unk198, &camera->unk194Vel, 3.91f, 8.210000038f, camera->dt);
                func_150495B0(&camera->unk18C, camera->unk190, &camera->unk18CVel, 3.91f, 8.210000038f, camera->dt);
            } else {
                func_15049688(&camera->yaw, aim->targetYaw, &aim->yawVel, 4.3f, 7.300000191f, camera->dt);
                func_15049688(&aim->pitch, aim->targetPitch, &aim->pitchVel, 4.3f, 7.300000191f, camera->dt);
            }
        }
        camera->playerExt->aimYaw = camera->yaw;
        camera->playerExt->aimPitch = aim->pitch;
        camera->playerExt->aimTarget = camera->target;
        camera->playerExt->aimEye = camera->eye;
        while (camera->playerExt->aimPitch > 180.0f) {
            camera->playerExt->aimPitch -= 360.0f;
        }
        while (camera->playerExt->aimPitch < -180.0f) {
            camera->playerExt->aimPitch += 360.0f;
        }
        horizontal = cosf(aim->pitch * DEG2RAD) * dist;
        sinPitch = sinf(aim->pitch * DEG2RAD);
        camera->targetDistance = horizontal;
        camera->height = sinPitch * dist;
        aim->orbitPos.x = sinf(camera->yaw * DEG2RAD + 3.141592741f) * dist + camera->target.x;
        aim->orbitPos.y = sinf(aim->pitch * DEG2RAD + 3.141592741f) * dist + camera->target.y;
        aim->orbitPos.z = cosf(camera->yaw * DEG2RAD + 3.141592741f) * dist + camera->target.z;
        if (aim->pitch >= 180.0f) {
            aim->signedPitch = aim->pitch - 360.0f;
        } else {
            aim->signedPitch = aim->pitch;
        }
        func_151218C4(camera);
    } else if (camera->aimState == 3) {
        yawDelta = fabsf(func_15048A70(camera->yaw, aim->savedYaw));
        camera->unk134 = camera->savedUnk134[1];
        if (camera->aimTimer != 0) {
            camera->aimTimer -= D_800BE9E4;
            if (camera->aimTimer <= 0) {
                camera->playerExt->aimActive = 0;
                camera->aimState = 0;
                return;
            }
        }
        if ((camera->unk84 & 0x200000) || IS_FACING_ZONE(camera->zone)) {
            aim->unk4D = 0;
        }
        if (!aim->unk4D || (yawDelta < 45.0f)) {
            camera->targetDistance = aim->savedTargetDistance;
            camera->height = aim->savedHeight;
            if ((camera->status & 0x40) || (camera->unk360 != 0.0f)) {
                camera->unk35C = aim->savedUnk354;
                camera->unk354 = aim->savedUnk354;
            } else {
                camera->unk35C = camera->player->unk180;
                camera->unk354 = camera->player->unk180;
            }
        }
        if (!aim->unk4D || (yawDelta < 0.05000000075f)) {
            camera->playerExt->aimActive = 0;
            camera->aimState = 0;
        } else {
            func_15049688(&camera->yaw, aim->savedYaw, &aim->yawVel, 5.0f, 9.0f, camera->dt);
        }
    }
    camera->yawRad = camera->yaw * DEG2RAD;
    if (((zone == 2) || (zone == 0x13) || (zone == 0x12) || (zone == 0xA) || (zone == 0x34)) && (zone != 0xD)) {
        f32 offsetZ;
        f32 pad4; // matching: unused stack slot
        f32 offsetX;
        f32 shift;

        if (camera->status & 2) {
            shift = aim->targetYaw * -3.0f;
        } else {
            shift = -35.0f;
        }
        offsetX = sinf(camera->yawRad - 1.570796371f) * shift;
        offsetZ = cosf(camera->yawRad - 1.570796371f) * shift;
        if (camera->snap) {
            aim->offset.x = offsetX;
            aim->offset.z = offsetZ;
            aim->offsetXVel = 0.0f;
        } else {
            func_150495B0(&aim->offset.x, offsetX, &aim->offsetXVel, 4.0f, 9.0f, camera->dt);
            func_150495B0(&aim->offset.z, offsetZ, &aim->offsetZVel, 4.0f, 9.0f, camera->dt);
        }
    } else {
        func_150495B0(&aim->offset.x, 0.0f, &aim->offsetXVel, 4.0f, 9.0f, camera->dt);
        func_150495B0(&aim->offset.z, 0.0f, &aim->offsetZVel, 4.0f, 9.0f, camera->dt);
    }
    camera->playerExt->aimTarget.x += aim->offset.x;
    camera->playerExt->aimTarget.z += aim->offset.z;
    camera->playerExt->aimEye.x += aim->offset.x;
    camera->playerExt->aimEye.z += aim->offset.z;
    func_150491EC((struct17 *)&camera->playerExt->aimEye, (struct17 *)&camera->playerExt->aimTarget, (struct17 *)&camera->playerExt->aimDir);
    func_15123A54(camera);
    func_1512E140(camera);
    func_15125594((struct108 *)camera);
}

/* While R is held in a facing zone, publishes the player's aim ray (eye, target, direction) to the
   player extension block. */
extern f32 D_800A3408;
extern f32 D_800A340C;
extern f32 D_800A3410;
extern f32 D_800A3414;
extern f32 D_800A3418;

void func_15121490(struct127 *player, OSContPad *pad) {
    CamPlayerExt *ext;
    s32 zone;
    vertex target;
    vertex eye;
    f32 angle;
    vertex dir;
    f32 temp;

    player->unk31C->unk198 = 0;
    if (pad->button & R_TRIG) {
        zone = player->unk31C->unk78;
        if ((zone == 9) || (zone == 0x38) || (zone == 0x39) || (zone == 0x37) ||
            (zone == 0x3B) || (zone == 0x15) || (zone == 0x26) || (zone == 0x3A)) {
            ext = (CamPlayerExt *)player->unk31C;
            angle = (player->unk40 - 90.0f) * D_800A3408;
            target.x = player->x_position;
            target.y = player->y_position + ((f32)ext->unk114 * 0.75f);
            target.z = player->z_position;
            eye = target;
            target.x = (sinf(angle) * 100.0f) + eye.x;
            target.y = eye.y;
            target.z = (cosf(angle) * 100.0f) + eye.z;
            eye.y += player->y_position - player->old_y_position;
            ext->aimEye = eye;
            ext->aimTarget = target;
            ext->aimActive = 1;
            dir.x = 1.0f;
            temp = dir.x * cosf(ext->aimPitch * D_800A340C);
            dir.y = dir.x * sinf(ext->aimPitch * D_800A3410);
            dir.x = temp;
            temp = dir.x * cosf(ext->aimYaw * D_800A3414);
            dir.z = dir.x * sinf(ext->aimYaw * D_800A3418);
            dir.x = temp;
            ext->aimDir = dir;
        }
    }
}

/* Sets the aim distance, unk190 and sideOffsetTarget for the camera's zone. */
void func_151216F8(Camera *camera) {
    s32 zone;

    zone = camera->zone;
    if (zone) {
        if (zone == 0x34) {
            camera->unk190 = 80.0f;
            camera->aim.distance = 350.0f;
            return;
        }
        if ((zone == 0x3B) && ((D_800BE9F0 == 0x41) || (D_800BE9F0 == 0x3C))) {
            camera->unk190 = -12.0f;
            camera->sideOffsetTarget = -28.0f;
            camera->aim.distance = 106.0f;
            return;
        }
        if ((zone == 2) || (zone == 0xA) || (zone == 0x13)) {
            camera->unk190 = 10.0f;
            camera->unk84 |= 0x80;
            camera->aim.distance = 135.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        if ((zone == 0x12) && (D_800BE9F0 == 0x3C)) {
            camera->unk190 = 10.0f;
            camera->aim.distance = 180.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        if (zone == 0xF) {
            camera->unk190 = 10.0f;
            camera->aim.distance = 480.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        if (zone == 0x38) {
            camera->aim.distance = 110.0f;
            camera->unk190 = 5.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        if (zone == 0x15) {
            camera->aim.distance = 110.0f;
            camera->unk190 = 25.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        camera->unk190 = 5.0f;
        camera->aim.distance = 75.0f;
        camera->sideOffsetTarget = 0.0f;
        return;
    }
    camera->aim.distance = 90.0f;
    camera->unk190 = 25.0f;
    camera->sideOffsetTarget = 0.0f;
}

/* Leaves aim mode once R is released: state 3 swings back to the saved view (or off at once when the
   previous mode was 0x40). */
void func_151218C4(Camera *camera) {
    CamAim *aim = &camera->aim;

    if (camera->contPad->button & R_TRIG) {
        return;
    }
    if (camera->savedMode[1] == 0x40) {
        if (camera->unk6FC == 0xF) {
            camera->unk898 = 0.0f;
            camera->unk89C = 0.0f;
        }
        camera->playerExt->aimActive = 0;
        camera->aimState = 0;
        camera->aimTimer = 60;
    } else {
        camera->aimTimer = 60;
        camera->aimState = 3;
    }
    camera->targetDistance = 200.0f;
    camera->height = 100.0f;
    camera->unk134 = aim->savedUnk134;
    aim->useOverride = FALSE;
    camera->unk190 = 0.0f;
    camera->sideOffsetTarget = 0.0f;
    camera->unk198 = 0.0f;
    func_1506160C(camera->player, 2, 0xFF, 0x20, camera->playerIndex);
    if (camera->zoneHookActive) {
        func_151C9ED4(camera);
        camera->zoneHookActive = FALSE;
    }
}
