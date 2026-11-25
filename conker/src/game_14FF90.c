#include <ultra64.h>

#include "functions.h"
#include "variables.h"

extern f32 D_800A34B0[4][2];
extern f32 D_800A34D0;
extern f32 D_800A34D4;
extern f32 D_800A34D8;
extern f32 D_800A34DC;
extern f32 D_800A34E0;
extern f32 D_800A34E4;
extern f32 D_800A34E8;
extern f32 D_800A34EC;
extern f32 D_800A34F0;
extern f32 D_800A34F4;
extern f32 D_800A34F8;
extern f32 D_800A34FC;
extern f32 D_800A3500;
extern f32 D_800A3504;
extern f32 D_800A3508;
extern f32 D_800A350C;
extern f32 D_800A3510;
extern f32 D_800A3514;
extern f32 D_800A3518;
extern f32 D_800A351C;
extern f32 D_800A3520;
extern f32 D_800A3524;
extern f32 D_800A3528;
extern f32 D_800A352C;
extern f32 D_800A3530;
extern f32 D_800A3534;
extern f32 D_800A3538;
extern f32 D_800A353C;
extern f32 D_800A3540;
extern f32 D_800A3544;
extern f32 D_800A3548;
extern f32 D_800A354C;
extern f32 D_800A3550;
extern f32 D_800A3554;
extern f32 D_800A3558;
extern f32 D_800A355C;
extern f32 D_800A3560;
extern f32 D_800A3564;
extern f32 D_800A3568;
extern f32 D_800A356C;
extern f32 D_800A3570;
extern f32 D_800A3574;
extern f32 D_800A3578;
extern f32 D_800A357C;
extern f32 D_800A3580;
extern f32 D_800A3584;
extern f32 D_800A3588;
extern f32 D_800A358C;
extern f32 D_800A3590;
extern f32 D_800A3594;
extern f32 D_800A3598;
extern f32 D_800A359C;
extern f32 D_800A35A0;
extern f32 D_800A35A4;
extern f32 D_800A35A8;
extern f32 D_800A35AC;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} CamVec3f;

/* Player extension block (struct127.unk31C, cached in Camera.playerExt). File-local view: struct108
   declares this pointer as struct127 *, and struct126 in include/ lacks these fields. */
typedef struct CamPlayerExt {
    /* 0x000 */ u8 pad0[0x8];
    /* 0x008 */ s16 unk8;
    /* 0x00A */ u8 padA[0xC];
    /* 0x016 */ u8 unk16;
    /* 0x017 */ u8 pad17[0x10];
    /* 0x027 */ u8 unk27;
    /* 0x028 */ u8 pad28[0x26];
    /* 0x04E */ u8 unk4E;
    /* 0x04F */ u8 pad4F[0x29];
    /* 0x078 */ u8 cameraZone;
    /* 0x079 */ u8 pad79[0x4];
    /* 0x07D */ u8 unk7D;
    /* 0x07E */ u8 pad7E[0x17];
    /* 0x095 */ u8 unk95;
    /* 0x096 */ u8 pad96[0x6];
    /* 0x09C */ s32 unk9C;
    /* 0x0A0 */ u8 padA0[0x74];
    /* 0x114 */ s16 unk114;
    /* 0x116 */ u8 pad116[0x4];
    /* 0x11A */ u8 unk11A;
    /* 0x11B */ u8 pad11B[0x5];
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 pad121[0xF];
    /* 0x130 */ CamVec3f aimDir;
    /* 0x13C */ CamVec3f aimEye;
    /* 0x148 */ CamVec3f aimTarget;
    /* 0x154 */ u8 pad154[0xC];
    /* 0x160 */ CamVec3f unk160;
    /* 0x16C */ f32 aimYaw;
    /* 0x170 */ f32 aimPitch;
    /* 0x174 */ f32 aimYawTarget;
    /* 0x178 */ f32 aimPitchTarget;
    /* 0x17C */ f32 aimYawVel;
    /* 0x180 */ f32 aimPitchVel;
    /* 0x184 */ f32 aimZoom;
    /* 0x188 */ f32 aimZoomY;
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ s32 aimZone;
    /* 0x194 */ u8 aimReset;
    /* 0x195 */ u8 pad195[0x2];
    /* 0x197 */ u8 aimState;
    /* 0x198 */ u8 unk198;
    /* 0x199 */ u8 pad199[0x13];
    /* 0x1AC */ u8 unk1AC;
    /* 0x1AD */ u8 pad1AD[0x6];
    /* 0x1B3 */ u8 unk1B3;
} CamPlayerExt;

/* Smooth transition of the camera to a new target/yaw (func_15128030); lives at Camera+0x7F4. */
typedef struct CamTransition {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 pad2;
    /* 0x04 */ CamVec3f startTarget;
    /* 0x10 */ CamVec3f endTarget;
    /* 0x1C */ f32 startDistance;
    /* 0x20 */ f32 startYaw;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 endDistance;
    /* 0x2C */ f32 endYaw;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 progress;
    /* 0x40 */ u16 duration;
    /* 0x42 */ u16 pad42;
    /* 0x44 */ f32 endHeight;
    /* 0x48 */ f32 startHeight;
    /* 0x4C */ s16 timer;
    /* 0x4E */ u16 pad4E;
} CamTransition;

/* Full file-local view of struct108 (one per player camera, 0x9A0 bytes). include/structs.h declares
   only part of it and mistypes several members (0x2AC, 0x5FC, 0x7B8, 0x3D4); a central pass should fold
   this into struct108.  Mode state 0x0..0x236 is a stack of 21 save slots (func_15123934 / func_151239CC). */
typedef struct Camera {
    /* 0x000 */ u16 modeIndex;              /* bit index of mode */
    /* 0x002 */ u16 savedModeIndex[21];
    /* 0x02C */ s32 mode;                   /* one-hot mode bit */
    /* 0x030 */ s32 savedMode[21];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ s32 savedUnk84[21];
    /* 0x0DC */ s32 unkDC;
    /* 0x0E0 */ s32 savedUnkDC[21];
    /* 0x134 */ s32 unk134;
    /* 0x138 */ s32 savedUnk134[21];
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ f32 unk190;
    /* 0x194 */ f32 unk194;
    /* 0x198 */ f32 unk198;
    /* 0x19C */ f32 unk19C;
    /* 0x1A0 */ f32 unk1A0;
    /* 0x1A4 */ f32 unk1A4;
    /* 0x1A8 */ f32 unk1A8;
    /* 0x1AC */ u8 pad1AC[0x8];
    /* 0x1B4 */ s16 preset;                 /* row of D_800A34B0 (distance, height) */
    /* 0x1B6 */ s16 savedPreset[21];
    /* 0x1E0 */ s16 presetMask;             /* presets reachable with the C buttons */
    /* 0x1E2 */ s16 savedPresetMask[21];
    /* 0x20C */ s16 slotUsed[21];
    /* 0x236 */ u8 pad236[0x2];
    /* 0x238 */ s32 unk238;
    /* 0x23C */ u8 snap;                    /* nonzero: jump to targets instead of easing (counts down) */
    /* 0x23D */ u8 playerIndex;
    /* 0x23E */ u8 zone;                    /* copy of playerExt->cameraZone */
    /* 0x23F */ u8 unk23F;
    /* 0x240 */ s32 unk240;
    /* 0x244 */ s16 pairIndex;              /* second object kept in frame (0 = none) */
    /* 0x246 */ s16 pad246;
    /* 0x248 */ struct127 *pairObj;
    /* 0x24C */ f32 unk24C;
    /* 0x250 */ f32 unk250;
    /* 0x254 */ u8 pad254[0x4];
    /* 0x258 */ f32 distanceVel;
    /* 0x25C */ u8 pad25C[0x10];
    /* 0x26C */ f32 unk26C;
    /* 0x270 */ s16 pad270;
    /* 0x272 */ s16 unk272;
    /* 0x274 */ u8 pad274[0x24];
    /* 0x298 */ s16 playerMoved;
    /* 0x29A */ u8 pad29A[0x2];
    /* 0x29C */ f32 swayPhase;
    /* 0x2A0 */ f32 targetYVel;
    /* 0x2A4 */ CamVec3f focus;             /* point followed on the player */
    /* 0x2B0 */ CamVec3f lastPlayerPos;
    /* 0x2BC */ CamVec3f target;            /* look-at point */
    /* 0x2C8 */ CamVec3f prevTarget;
    /* 0x2D4 */ u8 pad2D4[0xC];
    /* 0x2E0 */ CamVec3f savedTarget;
    /* 0x2EC */ CamVec3f savedEye;
    /* 0x2F8 */ CamVec3f eye;               /* camera position */
    /* 0x304 */ CamVec3f prevEye;
    /* 0x310 */ u8 pad310[0x34];
    /* 0x344 */ f32 unk344;
    /* 0x348 */ f32 height;
    /* 0x34C */ f32 unk34C;
    /* 0x350 */ u8 pad350[0x4];
    /* 0x354 */ f32 unk354;
    /* 0x358 */ f32 unk358;
    /* 0x35C */ f32 unk35C;
    /* 0x360 */ f32 unk360;
    /* 0x364 */ f32 unk364;
    /* 0x368 */ s16 controller;
    /* 0x36A */ u16 buttonsPressed;
    /* 0x36C */ OSContPad *contPad;
    /* 0x370 */ f32 distance;
    /* 0x374 */ f32 targetDistance;
    /* 0x378 */ u8 pad378[0x4];
    /* 0x37C */ f32 yaw;                    /* degrees */
    /* 0x380 */ f32 unk380;
    /* 0x384 */ f32 unk384;
    /* 0x388 */ f32 pitch;                  /* degrees */
    /* 0x38C */ f32 unk38C;
    /* 0x390 */ f32 unk390;
    /* 0x394 */ u8 pad394[0x4];
    /* 0x398 */ f32 pitchRad;
    /* 0x39C */ f32 yawRad;
    /* 0x3A0 */ f32 unk3A0;                 /* unk380 in radians */
    /* 0x3A4 */ u8 pad3A4[0x4];
    /* 0x3A8 */ f32 unk3A8;
    /* 0x3AC */ u8 pad3AC[0x8];
    /* 0x3B4 */ f32 distanceAccel;
    /* 0x3B8 */ f32 distanceMaxSpeed;
    /* 0x3BC */ u8 pad3BC[0x10];
    /* 0x3CC */ s16 unk3CC;
    /* 0x3CE */ u16 pad3CE;
    /* 0x3D0 */ struct127 *player;
    /* 0x3D4 */ CamPlayerExt *playerExt;
    /* 0x3D8 */ u8 pad3D8[0x8];
    /* 0x3E0 */ f32 unk3E0;
    /* 0x3E4 */ u8 pad3E4[0x4];
    /* 0x3E8 */ u8 unk3E8;
    /* 0x3E9 */ u8 pad3E9[0xB];
    /* 0x3F4 */ f32 unk3F4;
    /* 0x3F8 */ f32 unk3F8;
    /* 0x3FC */ u8 pad3FC[0x1D0];
    /* 0x5CC */ f32 unk5CC;
    /* 0x5D0 */ u8 pad5D0[0x4];
    /* 0x5D4 */ s32 unk5D4;
    /* 0x5D8 */ f32 unk5D8;
    /* 0x5DC */ u8 pad5DC[0xC];
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ f32 swayAmount;
    /* 0x5F0 */ s32 status;
    /* 0x5F4 */ u8 pad5F4[0x4];
    /* 0x5F8 */ s16 unk5F8;
    /* 0x5FA */ s16 unk5FA;
    /* 0x5FC */ s16 unk5FC;
    /* 0x5FE */ s16 unk5FE;
    /* 0x600 */ u8 blocked;                 /* wall between focus and eye this frame */
    /* 0x601 */ u8 pad601[0x3];
    /* 0x604 */ f32 blockedDistance;
    /* 0x608 */ u8 pad608[0x10];
    /* 0x618 */ CamVec3f unk618;
    /* 0x624 */ u8 pad624[0x38];
    /* 0x65C */ f32 unk65C;
    /* 0x660 */ f32 unk660;
    /* 0x664 */ CamVec3f forward;
    /* 0x670 */ f32 followRate;
    /* 0x674 */ f32 followRateTarget;
    /* 0x678 */ f32 followRateVel;
    /* 0x67C */ f32 unk67C;
    /* 0x680 */ u8 pad680[0x18];
    /* 0x698 */ s32 unk698;
    /* 0x69C */ u8 pad69C[0x14];
    /* 0x6B0 */ s32 unk6B0;
    /* 0x6B4 */ u8 pad6B4[0x14];
    /* 0x6C8 */ s32 unk6C8;
    /* 0x6CC */ u8 pad6CC[0x2C];
    /* 0x6F8 */ f32 maxDistance;
    /* 0x6FC */ u32 unk6FC;
    /* 0x700 */ u8 pad700[0xC];
    /* 0x70C */ s16 unk70C;
    /* 0x70E */ s16 unk70E;
    /* 0x710 */ s16 unk710;
    /* 0x712 */ s16 unk712;
    /* 0x714 */ u8 pad714[0xC];
    /* 0x720 */ f32 unk720;
    /* 0x724 */ f32 unk724;
    /* 0x728 */ f32 unk728;
    /* 0x72C */ f32 unk72C;
    /* 0x730 */ u8 pad730[0xC];
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0xE];
    /* 0x74C */ f32 unk74C;
    /* 0x750 */ u8 pad750[0x4];
    /* 0x754 */ f32 unk754;
    /* 0x758 */ u8 pad758[0x24];
    /* 0x77C */ f32 unk77C;
    /* 0x780 */ f32 unk780;
    /* 0x784 */ u8 pad784[0x18];
    /* 0x79C */ f32 unk79C;
    /* 0x7A0 */ f32 unk7A0;
    /* 0x7A4 */ u8 pad7A4[0xC];
    /* 0x7B0 */ f32 unk7B0;
    /* 0x7B4 */ f32 dt;                     /* frame time scale */
    /* 0x7B8 */ s32 presetCooldown;
    /* 0x7BC */ f32 unk7BC;
    /* 0x7C0 */ f32 unk7C0;
    /* 0x7C4 */ u8 pad7C4[0x4];
    /* 0x7C8 */ s32 unk7C8;
    /* 0x7CC */ s32 shakeTimer;
    /* 0x7D0 */ u8 pad7D0[0x10];
    /* 0x7E0 */ f32 spinDir;
    /* 0x7E4 */ u8 unk7E4;
    /* 0x7E5 */ u8 pad7E5;
    /* 0x7E6 */ u16 unk7E6;
    /* 0x7E8 */ f32 unk7E8;
    /* 0x7EC */ u8 pad7EC[0x8];
    /* 0x7F4 */ CamTransition transition;
    /* 0x844 */ f32 yawVel;
    /* 0x848 */ u8 pad848[0x2];
    /* 0x84A */ s16 unk84A;
    /* 0x84C */ u8 pad84C[0x20];
    /* 0x86C */ f32 unk86C;
    /* 0x870 */ s32 unk870;
    /* 0x874 */ f32 unk874;
    /* 0x878 */ f32 unk878;
    /* 0x87C */ f32 unk87C;
    /* 0x880 */ u8 pad880[0x3C];
    /* 0x8BC */ s32 unk8BC;
    /* 0x8C0 */ u8 pad8C0[0x20];
    /* 0x8E0 */ f32 lookBehindYawVel;
    /* 0x8E4 */ u8 lookBehindTimer;
    /* 0x8E5 */ u8 pad8E5[0x3];
    /* 0x8E8 */ s32 lookBehindHold;
    /* 0x8EC */ u8 lookingBehind;
    /* 0x8ED */ u8 unk8ED;
    /* 0x8EE */ u8 pad8EE[0x32];
    /* 0x920 */ u16 unk920;
    /* 0x922 */ u8 pad922[0xA];
    /* 0x92C */ u8 unk92C;
    /* 0x92D */ u8 pad92D[0x37];
    /* 0x964 */ f32 unk964;
    /* 0x968 */ f32 unk968;
    /* 0x96C */ u8 pad96C[0x8];
    /* 0x974 */ f32 unk974;
    /* 0x978 */ f32 unk978;
    /* 0x97C */ u8 pad97C[0x8];
    /* 0x984 */ f32 unk984;
    /* 0x988 */ f32 unk988;
    /* 0x98C */ u8 pad98C[0x8];
    /* 0x994 */ f32 sideOffset;
    /* 0x998 */ f32 sideOffsetVel;
    /* 0x99C */ f32 sideOffsetTarget;
} Camera; // size 0x9A0

#define CAM ((Camera *)camera)
#define DEG2RAD 0.01745329238474369f

#define CAMERA_CHECK(name, cond) typedef char name[(cond) ? 1 : -1]
CAMERA_CHECK(Camera_size, sizeof(Camera) == 0x9A0);
CAMERA_CHECK(CamTransition_size, sizeof(CamTransition) == 0x50);
CAMERA_CHECK(off_focus, (u32)&((Camera *)0)->focus == 0x2A4);
CAMERA_CHECK(off_target, (u32)&((Camera *)0)->target == 0x2BC);
CAMERA_CHECK(off_eye, (u32)&((Camera *)0)->eye == 0x2F8);
CAMERA_CHECK(off_contPad, (u32)&((Camera *)0)->contPad == 0x36C);
CAMERA_CHECK(off_yaw, (u32)&((Camera *)0)->yaw == 0x37C);
CAMERA_CHECK(off_player, (u32)&((Camera *)0)->player == 0x3D0);
CAMERA_CHECK(off_status, (u32)&((Camera *)0)->status == 0x5F0);
CAMERA_CHECK(off_blocked, (u32)&((Camera *)0)->blocked == 0x600);
CAMERA_CHECK(off_unk618, (u32)&((Camera *)0)->unk618 == 0x618);
CAMERA_CHECK(off_forward, (u32)&((Camera *)0)->forward == 0x664);
CAMERA_CHECK(off_maxDistance, (u32)&((Camera *)0)->maxDistance == 0x6F8);
CAMERA_CHECK(off_unk73C, (u32)&((Camera *)0)->unk73C == 0x73C);
CAMERA_CHECK(off_dt, (u32)&((Camera *)0)->dt == 0x7B4);
CAMERA_CHECK(off_transition, (u32)&((Camera *)0)->transition == 0x7F4);
CAMERA_CHECK(off_yawVel, (u32)&((Camera *)0)->yawVel == 0x844);
CAMERA_CHECK(off_unk84A, (u32)&((Camera *)0)->unk84A == 0x84A);
CAMERA_CHECK(off_lookBehindYawVel, (u32)&((Camera *)0)->lookBehindYawVel == 0x8E0);
CAMERA_CHECK(off_unk92C, (u32)&((Camera *)0)->unk92C == 0x92C);
CAMERA_CHECK(off_unk964, (u32)&((Camera *)0)->unk964 == 0x964);
CAMERA_CHECK(off_sideOffset, (u32)&((Camera *)0)->sideOffset == 0x994);
CAMERA_CHECK(off_slotUsed, (u32)&((Camera *)0)->slotUsed == 0x20C);
CAMERA_CHECK(off_snap, (u32)&((Camera *)0)->snap == 0x23C);
CAMERA_CHECK(off_distanceAccel, (u32)&((Camera *)0)->distanceAccel == 0x3B4);
CAMERA_CHECK(off_unk5FC, (u32)&((Camera *)0)->unk5FC == 0x5FC);
CAMERA_CHECK(ext_cameraZone, (u32)&((CamPlayerExt *)0)->cameraZone == 0x78);
CAMERA_CHECK(ext_unk95, (u32)&((CamPlayerExt *)0)->unk95 == 0x95);
CAMERA_CHECK(ext_unk9C, (u32)&((CamPlayerExt *)0)->unk9C == 0x9C);
CAMERA_CHECK(ext_unk114, (u32)&((CamPlayerExt *)0)->unk114 == 0x114);
CAMERA_CHECK(ext_unk120, (u32)&((CamPlayerExt *)0)->unk120 == 0x120);
CAMERA_CHECK(ext_aimDir, (u32)&((CamPlayerExt *)0)->aimDir == 0x130);
CAMERA_CHECK(ext_aimYaw, (u32)&((CamPlayerExt *)0)->aimYaw == 0x16C);
CAMERA_CHECK(ext_aimZone, (u32)&((CamPlayerExt *)0)->aimZone == 0x190);
CAMERA_CHECK(ext_aimState, (u32)&((CamPlayerExt *)0)->aimState == 0x197);
CAMERA_CHECK(ext_unk1AC, (u32)&((CamPlayerExt *)0)->unk1AC == 0x1AC);
CAMERA_CHECK(ext_unk1B3, (u32)&((CamPlayerExt *)0)->unk1B3 == 0x1B3);
CAMERA_CHECK(tr_timer, (u32)&((CamTransition *)0)->timer == 0x4C);

void func_15120158(struct108 *arg0);
void func_15122C5C(struct108 *arg0);
void func_15121C80(struct108 *arg0, f32 arg1);
void func_151220D0(struct108 *arg0);
void func_15122170(struct108 *arg0);
void func_15122440(struct108 *arg0);
void func_15122980(struct108 *arg0);
void func_151219D0(struct108 *arg0);
void func_151236D0(struct108 *arg0);
void func_151256BC(struct108 *arg0);
s32 func_15126378(struct108 *arg0);
s32 func_15128030(struct108 *arg0);
void func_15129934(struct108 *arg0);
void func_1512A360(u8 *arg0);
void func_1512E4B0(struct108 *arg0);
void func_1512DEA4(struct108 *arg0);
void func_15143134(void *arg0, void *arg1, s32 arg2);
void func_1508EF80(struct17 *arg0, struct17 *arg1, f32 arg2, struct17 *arg3);
void func_151CC290(s32 arg0);
void func_1515BA10(s32 arg0);
void func_1515BA1C(s16 arg0);
void func_1515BA48(s32 arg0);
void func_1515BA54(s16 arg0);
void func_1515BA80(s16 arg0);
void func_1515BAAC(s16 arg0);


#if 0
f32 D_800A34B0[4][2] = {
    { 267.0f, 100.0f },
    { 247.0f, 100.0f },
    { 370.0f, 185.0f },
    { 530.0f, 400.0f }
};

f32 D_800A34D0 = 0.01666666753590107f;
f32 D_800A34D4 = 0.10000000149011612f;
f32 D_800A34D8 = 999.0f;
f32 D_800A34DC = 0.01745329238474369f;
f32 D_800A34E0 = 0.01745329238474369f;
f32 D_800A34E4 = 0.01745329238474369f;
f32 D_800A34E8 = 0.01745329238474369f;
f32 D_800A34EC = 530.0f;
f32 D_800A34F0 = 3.1415927410125732f;
f32 D_800A34F4 = 3.1415927410125732f;
f32 D_800A34F8 = 0.3253220021724701f;
f32 D_800A34FC = 1.5707963705062866f;
f32 D_800A3500 = 1.5707963705062866f;
f32 D_800A3504 = 0.6499999761581421f;
f32 D_800A3508 = 0.550000011920929f;
f32 D_800A350C = 0.8999999761581421f;
f32 D_800A3510 = 0.8500000238418579f;
f32 D_800A3514 = -10000.0f;
f32 D_800A3518 = 0.0033333334140479565f;
f32 D_800A351C = 57.2957763671875f;
f32 D_800A3520 = 0.01745329238474369f;
f32 D_800A3524 = 0.01745329238474369f;
f32 D_800A3528 = 0.01745329238474369f;
f32 D_800A352C = 0.01745329238474369f;
f32 D_800A3530 = 0.01745329238474369f;
f32 D_800A3534 = 6.2831854820251465f;
f32 D_800A3538 = 0.01745329238474369f;
f32 D_800A353C = 0.03999999910593033f;
f32 D_800A3540 = 0.05000000074505806f;
f32 D_800A3544 = 0.07999999821186066f;
f32 D_800A3548 = 0.03999999910593033f;
f32 D_800A354C = 0.05000000074505806f;
f32 D_800A3550 = 0.01745329238474369f;
f32 D_800A3554 = 0.01745329238474369f;
f32 D_800A3558 = 0.019999999552965164f;
f32 D_800A355C = 0.01745329238474369f;
f32 D_800A3560 = 0.6000000238418579f;
f32 D_800A3564 = 10000.0f;
f32 D_800A3568 = 10000.0f;
f32 D_800A356C = -0.01745329238474369f;
f32 D_800A3570 = -0.01745329238474369f;
f32 D_800A3574 = -0.01745329238474369f;
f32 D_800A3578 = -0.01745329238474369f;
f32 D_800A357C = -0.9399999976158142f;
f32 D_800A3580 = -10000.0f;
f32 D_800A3584 = 0.01666666753590107f;
f32 D_800A3588 = 0.01745329238474369f;
f32 D_800A358C = 0.02500000037252903f;
f32 D_800A3590 = -0.01745329238474369f;
f32 D_800A3594 = -0.01745329238474369f;
f32 D_800A3598 = -0.01745329238474369f;
f32 D_800A359C = -0.01745329238474369f;
f32 D_800A35A0 = 999.0f;
f32 D_800A35A4 = 0.01745329238474369f;
f32 D_800A35A8 = 999.0f;
f32 D_800A35AC = 999.0f;
#endif

/* Per-frame update of every active player camera. */
void func_15122AE0(void) {
    struct108 *camera;
    s16 i;
    f32 dt;

    dt = D_800BEA08 * (1.0f / 60.0f);

    for (i = 0; i <= D_80082FA0; i++) {
        camera = &D_800DBFF0[i];
        if (func_150859AC(i, 0) || (i == 0)) {
            CAM->dt = dt;
            if ((D_800BEAC0 == 0) || (D_800C35EA != 0) || (D_800D2DB4 != 0)) {
                func_151239CC(camera, 5);
                func_15122C5C(camera);
            } else {
                func_15123934(camera, 0x2000, 0, CAM->unk134, 5);
                func_1512C490(camera);
            }
        }
    }
    D_800894B0 += 1;
}

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} V3_15122C5C;

void func_1510B128(s32, f32, f32, f32, f32);
void func_15097798(u8);
void func_15130230(struct108 *arg0, s32 arg1);
void func_15125A6C(struct108 *arg0);
void func_15128CB0(struct108 *arg0);
void func_15125DB4(struct108 *arg0);
void func_15127EB8(struct108 *arg0);
void func_1512E8E0(struct108 *arg0);
void func_15126138(struct108 *arg0);
void func_15125C40(struct108 *arg0);
void func_15128680(struct108 *arg0);
void func_1512317C(struct108 *arg0);
void func_15125394(struct108 *arg0);
void func_15123568(struct108 *arg0);
void func_1512C200(struct108 *arg0);
void func_15125924(struct108 *arg0);
void func_151283B8(struct108 *arg0);
void func_15125594(struct108 *arg0);
void func_15123070(struct108 *arg0);
void func_15123508(struct108 *arg0);
void func_1512D390(struct108 *arg0);
void func_1512D980(struct108 *arg0);
void func_1512C068(struct108 *arg0);
void func_1512B100(struct108 *arg0);
void func_1512C20C(struct108 *arg0);
void func_1512BB10(struct108 *arg0);
void func_1512C150(struct108 *arg0);
void func_151284C4(struct108 *arg0);
s32 func_15128540(struct108 *arg0);
void func_151287E0(struct108 *arg0, f32 *arg1, f32 *arg2);

void func_15122C5C(struct108 *arg0) {
    V3_15122C5C *temp_a0;
    V3_15122C5C *temp_a1;
    struct127 *temp_v0;
    s16 temp_v1;
    u8 idx;
    f32 var_f0;

    if (arg0->unk23C) {
        *(V3_15122C5C *)&arg0->unk2E0 = *(V3_15122C5C *)&arg0->unk2BC;
        *(V3_15122C5C *)&arg0->unk2EC = *(V3_15122C5C *)&arg0->unk2F8;
    }
    temp_v0 = arg0->unk3D4;
    temp_a0 = (V3_15122C5C *)&arg0->unk2BC;
    temp_a1 = (V3_15122C5C *)&arg0->unk2F8;
    if ((arg0->unk3D4 != NULL) && (arg0->unk73C != 3) && ((arg0->unk2C != 0x100) || (arg0->unk73C != 0))) {
            arg0->unk23E = *(u8 *)&arg0->unk3D4->unk78;
    }
    if (D_800DBFF4[arg0->unk23D] != 4) {
        arg0->unk5F0 &= ~4;
    }
    if (!(arg0->unk2C & 0x400000)) {
        *temp_a0 = *(V3_15122C5C *)&arg0->unk2E0;
        *temp_a1 = *(V3_15122C5C *)&arg0->unk2EC;
    }
    idx = arg0->unk23D;
    arg0->unk36C = (*(u16 ***)&D_800BE728)[idx];
    arg0->unk36A = D_800BE710[idx];
    *(V3_15122C5C *)&arg0->unk304 = *temp_a1;
    arg0->unk358 = arg0->unk35C;
    *(V3_15122C5C *)&arg0->unk2C8 = *temp_a0;
    if (arg0->unk23C != 0) {
        arg0->unk19C = arg0->unk1A4;
        arg0->unk1A0 = arg0->unk1A8;
    } else {
        arg0->unk19C += (arg0->unk1A4 - arg0->unk19C) * 0.10000000149011612f;
        arg0->unk1A0 += (arg0->unk1A8 - arg0->unk1A0) * 0.10000000149011612f;
    }
    if (arg0->unk2C & 0x100) {
        var_f0 = 16.0f;
    } else {
        var_f0 = 0.0f;
    }
    func_1510B128(idx = arg0->unk23D, arg0->unk19C, arg0->unk1A0, *(f32 *)((*(u8 **)&D_800BE628) + idx * 0x180 + 0x84), var_f0);
    func_15097798(arg0->unk23D);
    func_15125A6C(arg0);
    func_15128CB0(arg0);
    func_15125DB4(arg0);
    if ((arg0->unk3D4->unk120 != 0) && (arg0->unk3D4->unk197 != 0)) {
        func_15127EB8(arg0);
    }
    if (func_15128540(arg0) == 0) {
        if ((arg0->unk84 & 0x08000000) && !(arg0->unk84 & 0x400)) {
            func_1512E8E0(arg0);
        }
        func_15130230(arg0, D_800BE9F0);
        func_15126138(arg0);
        func_15125C40(arg0);
        func_15128680(arg0);
        func_1512317C(arg0);
        func_15125394(arg0);
        if (!(arg0->unk84 & 0x08000000) && !(arg0->unk84 & 0x400)) {
            func_1512E8E0(arg0);
        }
        func_15123568(arg0);
        func_1512C200(arg0);
        func_15125924(arg0);
        func_151283B8(arg0);
        if (arg0->unk23C != 0) {
            func_15125594(arg0);
            func_15123070(arg0);
            func_151283B8(arg0);
            func_151283B8(arg0);
        }
        func_15123508(arg0);
        if ((arg0->unk84 & 2) && (arg0->unk3D4->unk120 == 0)) {
            func_1512D390(arg0);
        }
        func_15125594(arg0);
        func_151256BC(arg0);
        func_1512D980(arg0);
        func_1512C068(arg0);
        func_1512B100(arg0);
        func_1512C20C(arg0);
        func_1512BB10(arg0);
        if (arg0->unk23C != 0) {
            func_1512C068(arg0);
            func_1512B100(arg0);
            func_1512C20C(arg0);
            func_1512BB10(arg0);
        }
        func_15125594(arg0);
        func_15123070(arg0);
        if (arg0->unk7F4 == 0) {
            func_151287E0(arg0, 0, 0);
        }
        func_1512C150(arg0);
        func_151284C4(arg0);
    }
}
// NON-MATCHING: 80% there
// void func_15122C5C(struct108 *arg0) {
//     f32 blend;
//     u8 temp_v0_2;
//     struct167 *temp_a1;
//     struct167 *temp_a0;
//
//     if (arg0->unk23C) {
//         arg0->unk2E0 = arg0->unk2BC;
//         arg0->unk2E4 = arg0->unk2C0;
//         arg0->unk2F0 = arg0->unk2FC;
//
//         arg0->unk2EC = arg0->unk2F8;
//         arg0->unk2E8 = arg0->unk2C4;
//         arg0->unk2F4 = arg0->unk300;
//     }
//
//     temp_a0 = &arg0->unk2BC;
//     temp_a1 = &arg0->unk2F8;
//
//     if ((arg0->unk3D4 != NULL) && (arg0->unk73C != 3) && ((arg0->unk2C != 0x100) || (arg0->unk73C != 0))) {
//         arg0->unk23E = arg0->unk3D4->unk78;
//     }
//
//     temp_v0_2 = arg0->unk23D;
//
//     if (D_800DBFF4[temp_v0_2] != 4) {
//         arg0->unk5F0 &= -5;
//     }
//
//     if ((arg0->unk2C << 9) >= 0) {
//         temp_a0->unk0 = arg0->unk2E0;
//         temp_a0->unk4 = arg0->unk2E4;
//         temp_a0->unk8 = arg0->unk2E8;
//         temp_a1->unk0 = arg0->unk2EC;
//         temp_a1->unk4 = arg0->unk2F0;
//         temp_a1->unk8 = arg0->unk2F4;
//     }
//     // temp_v0_2 = arg0->unk23D;
//     arg0->unk36C = D_800BE728[temp_v0_2];
//     arg0->unk36A = D_800BE710[temp_v0_2];
//     arg0->unk304 = temp_a1->unk0;
//     arg0->unk308 = temp_a1->unk4;
//     arg0->unk358 = arg0->unk35C;
//     arg0->unk30C = temp_a1->unk8;
//     arg0->unk2C8 = temp_a0->unk0;
//     arg0->unk2CC = temp_a0->unk4;
//     arg0->unk2D0 = temp_a0->unk8;
//     if (arg0->unk23C != 0) {
//         arg0->unk19C = arg0->unk1A4;
//         arg0->unk1A0 = arg0->unk1A8;
//         // blend = *(s32*)&arg0; // unk0?
//     } else {
//         arg0->unk19C += (arg0->unk1A4 - arg0->unk19C) * D_800A34D4;
//         arg0->unk1A0 += (arg0->unk1A8 - arg0->unk1A0) * D_800A34D4;
//         blend = D_800A34D4;
//     }
//
//     func_1510B128(blend, temp_v0_2, arg0->unk19C, arg0->unk1A0, D_800BE628[temp_v0_2].unk84, (arg0->unk2C & 0x100) ? 16.0f : 0.0f);
//     func_15097798(arg0->unk23D);
//     func_15125A6C(arg0);
//     func_15128CB0(arg0);
//     func_15125DB4(arg0);
//     if ((arg0->unk3D4->unk120 != 0) && (arg0->unk3D4->unk197 != 0)) {
//         func_15127EB8(arg0);
//     }
//     if (func_15128540(arg0) == 0) {
//         if (((arg0->unk84 * 0x10) < 0) && ((arg0->unk84 & 0x400) == 0)) {
//             func_1512E8E0(arg0);
//         }
//         func_15130230(arg0, D_800BE9F0);
//         func_15126138(arg0);
//         func_15125C40(arg0);
//         func_15128680(arg0);
//         func_1512317C(arg0);
//         func_15125394(arg0);
//         if (((arg0->unk84 * 16) >= 0) && ((arg0->unk84 & 0x400) == 0)) {
//             func_1512E8E0(arg0);
//         }
//         func_15123568(arg0);
//         func_1512C200(arg0);
//         func_15125924(arg0);
//         func_151283B8(arg0);
//         if (arg0->unk23C != 0) {
//             func_15125594(arg0);
//             func_15123070(arg0);
//             func_151283B8(arg0);
//             func_151283B8(arg0);
//         }
//         func_15123508(arg0);
//         if (((arg0->unk84 & 2) != 0) && (arg0->unk3D4->unk120 == 0)) {
//             func_1512D390(arg0);
//         }
//         func_15125594(arg0);
//         func_151256BC(arg0);
//         func_1512D980(arg0);
//         func_1512C068(arg0);
//         func_1512B100(arg0);
//         func_1512C20C(arg0);
//         func_1512BB10(arg0);
//         if (arg0->unk23C != 0) {
//             func_1512C068(arg0);
//             func_1512B100(arg0);
//             func_1512C20C(arg0);
//             func_1512BB10(arg0);
//         }
//         func_15125594(arg0);
//         func_15123070(arg0);
//         if (arg0->unk7F4 == 0) {
//             func_151287E0(arg0, 0, 0);
//         }
//         func_1512C150(arg0);
//         func_151284C4(arg0);
//     }
// }

/* Computes unk390, the angle (degrees, wrapped above -360) between the camera yaw and the player. */
void func_15123070(struct108 *camera) {
    f32 angle;

    if ((CAM->unk6C8 != 0) && ((CAM->unk6FC == 10) || (CAM->unk6FC == 14))) {
        struct17 delta;

        func_15048F90((struct17 *)&CAM->unk618, (struct17 *)&CAM->focus, &delta);
        CAM->unk390 = CAM->yaw - func_15048FC8(&delta);
    } else {
        angle = CAM->player->unk40 - CAM->yaw - 180.0f;

        while (angle < 0.0f) {
            angle += 360.0f;
        }

        CAM->unk390 = angle;
    }

    while (CAM->unk390 < -360.0f) {
        CAM->unk390 += 360.0f;
    }
}

void func_1511FC60(struct108 *arg0);
void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
s32 func_15125490(struct108 *arg0);

void func_1512317C(struct108 *arg0) {
    s32 cond1;
    s32 bit100;
    s32 bit200;
    s32 flag;

    bit100 = arg0->unk5F0 & 0x100;
    bit200 = arg0->unk5F0 & 0x200;
    cond1 = (arg0->unk3D0->stunned != 0) && (arg0->unk23E != 9) && (arg0->unk23E != 0x38) &&
            (arg0->unk23E != 0x39) && (arg0->unk23E != 0x3B) && (arg0->unk23E != 0x37) &&
            (arg0->unk23E != 0x15) && (arg0->unk23E != 0x26) && (arg0->unk23E != 0x3A);

    flag = ((arg0->unk2C & 0x80) != 0) ||
           ((arg0->unk84 & 0x1000000) != 0) ||
           ((arg0->unk3D0->unk28 != 0.0f) && (arg0->unk3D0->in_water == 0) &&
            (arg0->unk3D4->unk9C == 0) && (bit100 == 0) && (bit200 == 0) &&
            (arg0->unk23E != 3)) ||
           ((arg0->unk3D0->unk65 != 0) && (bit100 == 0) && (bit200 == 0)) ||
           (cond1 != 0) ||
           (func_15125490(arg0) != 0) ||
           (arg0->unk3D0->health == 0) ||
           (((*(u8 *)((u8 *)arg0->unk3D4 + 0x4E) & 0xF) == 1) &&
            (arg0->unk3D0->xz_velocity > 15.0f)) ||
           (*(u8 *)((u8 *)arg0->unk3D4 + 0x1AC) != 0);

    if (((*arg0->unk36C & 0x10) != 0) && (flag == 0)) {
        if ((arg0->unk36A & 0x10) == 0) {
            return;
        }
        if (func_15123934(arg0, 0x100, 4, arg0->unk134, 1) != 0) {
            func_1511FC60(arg0);
        }
        return;
    }

    if ((arg0->unk2C == 0x100) && (arg0->unk73C == 0)) {
        func_151239CC(arg0, 1);
        arg0->unk3D4->unk198 = 0;
        arg0->unk3D4->unk197 = 0;
        *(s16 *)((u8 *)arg0 + 0x5F8) = 1;
        arg0->unk5FC = 2;
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
    }

    if ((arg0->unk2C == 0x100) || (arg0->unk73C != 0)) {
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
    }
    arg0->unk3D4->unk198 = arg0->unk73C;

    if ((arg0->unk73C != 0) || (arg0->unk73C == 3)) {
        func_150495B0((f32 *)((u8 *)arg0 + 0x74C), 0.0f, (f32 *)((u8 *)arg0 + 0x79C), 4.0f, 9.0f,
                      arg0->unk7B4);
        func_150495B0((f32 *)((u8 *)arg0 + 0x754), 0.0f, (f32 *)((u8 *)arg0 + 0x7A0), 4.0f, 9.0f,
                      arg0->unk7B4);
    }

    if ((arg0->unk2C != 0x100) && (arg0->unk73C != 0)) {
        arg0->unk73C = 0;
    }
}

/* With the C-left/C-right buttons, sets unk6B0 to -1/+1 (when unk84 bit 2 allows it). */
void func_15123508(struct108 *camera) {
    if (CAM->unk84 & 2) {
        if ((CAM->buttonsPressed & L_CBUTTONS) && (CAM->unk698 == 0)) {
            CAM->unk6B0 = -1;
        }
        if ((CAM->buttonsPressed & R_CBUTTONS) && (CAM->unk698 == 0)) {
            CAM->unk6B0 = 1;
        }
    }
}

void func_15123568(struct108 *arg0) {
    s32 cycle;

    if ((*((u8 *)arg0->unk3D4 + 0x120) == 4) && (arg0->unk2C != 0x40) && (*((u8 *)arg0 + 0x92C) == 0)) {
        *(f32 *)((u8 *)arg0 + 0x7C0) = D_800A34D8;
        arg0->unk1B4 = 1;
        func_15124B18(arg0);
        arg0->unk198 = 0.0f;
        arg0->unk190 = 0.0f;
        return;
    }
    if (*(s32 *)((u8 *)arg0 + 0x7B8) > 0) {
        *(s32 *)((u8 *)arg0 + 0x7B8) -= D_800BE9E4;
        if (*(s32 *)((u8 *)arg0 + 0x7B8) < 0) {
            *(s32 *)((u8 *)arg0 + 0x7B8) = 0;
        }
    } else if ((arg0->unk84 & 4) && (arg0->unk36A & 8) && (arg0->unkDC != 4) &&
               ((arg0->unk6C8 == 0) || (arg0->unk6FC == 4U)) &&
               ((arg0->unk3D0->in_water != 1) || (arg0->unk1B4 != 1))) {
        cycle = arg0->unk1B4 != 1;
        while (cycle) {
            if (--arg0->unk1B4 <= 0) {
                arg0->unk1B4 = 3;
            }
            if ((1 << arg0->unk1B4) & arg0->unk1E0) {
                func_15124B18(arg0);
                func_15125608(arg0);
                *(s32 *)((u8 *)arg0 + 0x7B8) = 20;
                return;
            }
            cycle = TRUE;
        }
    }
}

void func_151236D0(struct108 *arg0) {
    s32 oldState;

    oldState = arg0->unk5F0 & 1;
    if (arg0->unk3D4 != NULL) {
        if ((arg0->unk3D4->unk9C != 0) ||
            ((*((u8 *)arg0->unk3D4 + 0x95) != 0) && (*((u8 *)arg0->unk3D0 + 0x137) == 0))) {
            arg0->unk5F0 |= 0x40;
        } else {
            arg0->unk5F0 &= ~0x40;
        }
        if (arg0->unk3D0->unk28 == 0.0f) {
            arg0->unk5F0 &= ~0x400;
        }
        if ((*(s16 *)((u8 *)arg0->unk3D4 + 0x8) != 0) && (*((u8 *)arg0->unk3D4 + 0x16) == 0)) {
            arg0->unk5F0 |= 8;
        } else {
            arg0->unk5F0 &= ~8;
        }
        if (*((u8 *)arg0->unk3D4 + 0x4E) == 2) {
            arg0->unk5F0 |= 0x80;
        } else {
            arg0->unk5F0 &= ~0x80;
        }
    }
    if ((arg0->unk2FC <= *(f32 *)((u8 *)arg0 + 0x360)) && (arg0->unk2C != 0x100)) {
        arg0->unk5F0 |= 1;
    } else {
        arg0->unk5F0 &= ~1;
    }
    if (arg0->unk23C != 0) {
        if ((arg0->unk5F0 & 1) != 0) {
            func_1515BA80(arg0->unk23D);
        } else {
            func_1515BA48(arg0->unk23D);
            *(f32 *)((u8 *)arg0 + 0x7B0) = 0.0f;
        }
    } else if ((oldState == 0) && ((arg0->unk5F0 & 1) != 0)) {
        if (((arg0->unk5F0 & 4) != 0) || ((arg0->unk2C & 0x40000) != 0)) {
            func_1515BA80(arg0->unk23D);
        } else {
            func_1515BAAC(arg0->unk23D);
        }
    } else if ((oldState != 0) && ((arg0->unk5F0 & 1) == 0)) {
        if (((arg0->unk5F0 & 4) != 0) || ((arg0->unk2C & 0x40000) != 0)) {
            func_1515BA48(arg0->unk23D);
            *(f32 *)((u8 *)arg0 + 0x7B0) = 0.0f;
        } else {
            func_1515BA54(arg0->unk23D);
            func_15124B18(arg0);
        }
    } else if ((arg0->unk5F0 & 1) != 0) {
        func_1515BA1C(arg0->unk23D);
    } else {
        func_1515BA10(arg0->unk23D);
    }
}
s32 func_15123934(struct108 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 v0;

    v0 = 0;
    if (((s16 *)((u8 *)arg0 + arg4 * 2))[0x106] == 0) {
        *(u16 *)((u8 *)arg0 + arg4 * 2 + 0x2) = arg0->unk0;
        *(s32 *)((u8 *)arg0 + arg4 * 4 + 0x30) = arg0->unk2C;
        *(s32 *)((u8 *)arg0 + arg4 * 4 + 0x88) = arg0->unk84;
        *(s32 *)((u8 *)arg0 + arg4 * 4 + 0xE0) = arg0->unkDC;
        *(s32 *)((u8 *)arg0 + arg4 * 4 + 0x138) = arg0->unk134;
        *(s16 *)((u8 *)arg0 + arg4 * 2 + 0x1B6) = arg0->unk1B4;
        *(s16 *)((u8 *)arg0 + arg4 * 2 + 0x1E2) = arg0->unk1E0;
        arg0->unk2C = arg1;
        arg0->unkDC = arg2;
        arg0->unk134 = arg3;
        *(s16 *)((u8 *)arg0 + arg4 * 2 + 0x20C) = 1;
        func_15125394(arg0);
        return 1;
    }
    return v0;
}

#if 0
// NON-MATCHING: not sure what is up with arg0
// s32 func_15123934(struct108 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
//     struct108 *temp_v0;
//     struct108 *temp_v1;
//
//     temp_v1 = &arg0[arg4]; // ???
//     if (temp_v1->unk20C == 0) {
//         temp_v1->unk2 = arg0->unk0;
//         temp_v0 = &arg0[arg4 * 4]; // ???
//         temp_v0->unk30 = arg0->unk2C;
//         temp_v0->unk88 = arg0->unk84;
//         temp_v0->unkE0 = arg0->unkDC;
//         temp_v0->unk138 = arg0->unk134;
//         temp_v1->unk1B6[0] = arg0->unk1B4;
//         temp_v1->unk1E2 = arg0->unk1E0;
//         arg0->unk2C = arg1;
//         arg0->unkDC = arg2;
//         arg0->unk134 = arg3;
//         temp_v1->unk20C = 1;
//         func_15125394();
//         return 1;
//     }
//     return 0;
// }
#endif

s32 func_151239CC(struct108 *arg0, s32 arg1) {
    if (arg0->unk20C[arg1] != 0) {
        arg0->unk0 = arg0->unk2[arg1];
        arg0->unk2C  = *(s32*)((u8*)arg0 + arg1 * 4 + 0x30);
        arg0->unkDC  = *(s32*)((u8*)arg0 + arg1 * 4 + 0xE0);
        arg0->unk84  = *(s32*)((u8*)arg0 + arg1 * 4 + 0x88);
        arg0->unk134 = *(s32*)((u8*)arg0 + arg1 * 4 + 0x138);
        arg0->unk1B4 = ((s16*)arg0)[arg1 + 0xDB];
        arg0->unk1E0 = ((s16*)arg0)[arg1 + 0xF1];
        func_15124B18(arg0);
        arg0->unk20C[arg1] = 0;
        return 1;
    }
    return 0;
}
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, s16 *, f32 *, f32 *, f32 *, f32 *, s32, s32, f32);
void func_150AD8B0(struct17 *, struct17 *, struct17 *);
void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
extern f32 D_800895B0;
extern u8 D_800BEA0C;
extern f32 D_800DC000;

#define FX(off) (*(f32 *)((u8 *)arg0 + (off)))
#define SX(off) (*(s32 *)((u8 *)arg0 + (off)))
#define BX(off) (*(u8 *)((u8 *)arg0 + (off)))
#define HX(off) (*(u16 *)((u8 *)arg0 + (off)))

void func_15123A54(struct108 *arg0) {
    f32 angle;
    f32 saved;

    saved = arg0->unk374;
    BX(0x600) = 0;
    FX(0x604) = arg0->unk374;
    if (D_800C3671 == 0) {
        if (FX(0x6F8) != 0.0f) {
            if (FX(0x6F8) < arg0->unk374) {
                arg0->unk374 = FX(0x6F8);
            } else {
                arg0->unk374 = arg0->unk374;
            }
        }
        if (arg0->unk23C == 0) {
            if ((arg0->unk5F0 & 0x10) && (*arg0->unk36C & 0x10)) {
                arg0->unk374 = 130.0f;
            }
            if ((arg0->unk73C == 0) && (arg0->unk5F0 & 0x40)) {
                arg0->unk374 = 420.0f;
            }
            if (arg0->unk2C & 0x403) {
                if (arg0->unk36A & 4) {
                    BX(0x8E4) = 1;
                    SX(0x8E8) = 180;
                    FX(0x8E0) = 0.0f;
                } else if ((*arg0->unk36C & 4) || BX(0x8E4)) {
                    if (BX(0x8E4) && (*arg0->unk36C & 4)) {
                        BX(0x8E4) += D_800BEA08;
                        BX(0x8E4) = (BX(0x8E4) >= 16) ? 15 : BX(0x8E4);
                    }
                    if (BX(0x8E4) == 15) {
                        BX(0x8EC) = 1;
                        SX(0x7B8) = 20;
                        arg0->unk374 = 300.0f;
                        arg0->unk348 = 150.0f;
                        func_15049688(&arg0->unk37C, arg0->unk3D0->unk40 - 180.0f, &FX(0x8E0), 1.5f, 2.0f, arg0->unk7B4);
                        if (!(*arg0->unk36C & 4)) {
                            BX(0x8E4) = 0;
                        }
                    } else {
                        func_15049688(&arg0->unk37C, arg0->unk3D0->unk40 - 180.0f, &FX(0x8E0), 2.0f, 4.0f, arg0->unk7B4);
                    }
                    arg0->unk39C = arg0->unk37C * 0.01745329238474369f;
                    if ((BX(0x8E4) < 15) && !(*arg0->unk36C & 4)) {
                        if (*(s16 *)&arg0->unk298 == 0) {
                            SX(0x8E8) -= D_800BEA08;
                            if (SX(0x8E8) <= 0) {
                                BX(0x8E4) = 0;
                            } else if (fabsf(arg0->unk37C - (arg0->unk3D0->unk40 - 180.0f)) <= 2.0f) {
                                BX(0x8E4) = 0;
                            }
                        } else {
                            BX(0x8E4) = 0;
                        }
                    }
                } else if (BX(0x8EC) != 0) {
                    func_15124B18(arg0);
                    BX(0x8EC) = 0;
                    BX(0x8E4) = 0;
                }
            }
        } else {
            arg0->unk39C = arg0->unk37C * 0.01745329238474369f;
        }
        angle = arg0->unk39C;
        if (D_800BEA0C != 0) {
            arg0->unk37C += (f32)1 * FX(0x7E0) * D_800BEA08;
            arg0->unk39C = arg0->unk37C * 0.01745329238474369f;
            angle = arg0->unk39C;
        } else if (func_150ADA20() & 1) {
            FX(0x7E0) = -1.0f;
        } else {
            FX(0x7E0) = 1.0f;
        }
        if ((*((u8 *)arg0->unk3D4 + 0x95) != 0) && (arg0->unk5F0 & 0x40)) {
            angle = arg0->unk3D0->unk40 + 180.0f;
            func_15049688(&arg0->unk37C, angle, &FX(0x844), 2.0f, 4.0f, arg0->unk7B4);
            arg0->unk39C = arg0->unk37C * 0.01745329238474369f;
            angle = arg0->unk39C;
            arg0->unk374 = 530.0f;
        }
        if (arg0->unk2C != 0x2000) {
            FX(0x370) = sqrtf((arg0->unk2F8 - arg0->unk2BC) * (arg0->unk2F8 - arg0->unk2BC) + (arg0->unk300 - arg0->unk2C4) * (arg0->unk300 - arg0->unk2C4));
            if ((arg0->unk73C != 0) || (arg0->unk23C != 0) || (*((u8 *)arg0->unk3D0 + 0x102) != 0) ||
                (!(arg0->unk84 & 0x1000) && (BX(0x92C) == 0) && !(arg0->unk84 & 0x40000) && !(arg0->unk5F0 & 0x80))) {
                {
                struct17 dir;
                s16 tri[10];
                f32 hit[3];
                f32 dist;
                f32 d;
                s16 tri2[10];
                f32 vz;
                f32 vy;
                f32 vx;
                struct17 e1;
                struct17 e2;
                struct17 n;
                f32 hit2[3];
                f32 dist2;
                f32 prev;
                struct17 p0;
                struct17 p1;
                struct17 p2;

                dir.unk0 = sinf(arg0->unk39C);
                dir.unk4 = 0.0f;
                dir.unk8 = cosf(arg0->unk39C);
                if ((func_150AC9C0(arg0->unk2A4, arg0->unk2A8 + 140.0f, FX(0x2AC), dir.unk0, dir.unk4, dir.unk8, 0, tri,
                                   &hit[0], &hit[1], &hit[2], &dist, 0, 0, arg0->unk374 + 30.0f) != 0) &&
                    (dist < arg0->unk374)) {

                    vx = sinf(arg0->unk39C + 3.1415927410125732f) * arg0->unk374;
                    vz = cosf(arg0->unk39C + 3.1415927410125732f) * arg0->unk374;
                    p0.unk0 = tri[0];
                    p0.unk4 = tri[1];
                    p0.unk8 = tri[2];
                    p1.unk0 = tri[3];
                    p1.unk4 = tri[4];
                    p1.unk8 = tri[5];
                    p2.unk0 = tri[6];
                    p2.unk4 = tri[7];
                    p2.unk8 = tri[8];
                    func_15048F90(&p0, &p1, &e1);
                    func_15048F90(&p0, &p2, &e2);
                    func_150AD8B0(&e2, &e1, &n);
                    func_1504917C(&n, &n);
                    if (0.3253220021724701f < func_150AD900(&dir.unk0, &n.unk0)) {
                        if (func_150AC9C0(arg0->unk2A4 - vx, arg0->unk2A8 + 140.0f, FX(0x2AC) - vz, vx, 0.0f, vz, 0,
                                       tri2, &hit2[0], &hit2[1], &hit2[2], &dist2, 0, 0, arg0->unk374 + 30.0f) != 0) {
                        {
                        f32 dx;
                        f32 dz;

                        dx = hit2[0] - hit[0];
                        dz = hit2[2] - hit[2];
                        if ((sqrtf(dx * dx + dz * dz) < 1.0f) ||
                         (arg0->unk5F0 & 0x10) || (arg0->unk23E == 0xC) || (arg0->unk23E == 9) ||
                         (arg0->unk23E == 0x38) || (arg0->unk23E == 0x39) || (arg0->unk23E == 0x37) ||
                         (arg0->unk23E == 0x3B) || (arg0->unk23E == 0x12) || (arg0->unk23E == 0x15) ||
                         (arg0->unk23E == 0x26) || (arg0->unk23E == 0x3A) || (arg0->unk23C != 0)) {
                        prev = arg0->unk374;
                        if (arg0->unk73C != 0) {
                            dist -= 16.0f;
                        } else {
                            dist -= 12.0f;
                        }
                        if (dist < 50.0f) {
                            dist = 50.0f;
                        } else {
                            dist = dist;
                        }
                        if (dist < arg0->unk374) {
                            arg0->unk374 = dist;
                        } else {
                            arg0->unk374 = arg0->unk374;
                        }
                        if (arg0->unk23E == 3) {
                            arg0->unk348 *= arg0->unk374 / prev;
                            if (arg0->unk5F0 & 0x10) {
                                if (arg0->unk348 < arg0->unk2C0 - arg0->unk354) {
                                    arg0->unk348 = arg0->unk2C0 - arg0->unk354;
                                } else {
                                    arg0->unk348 = arg0->unk348;
                                }
                            }
                        }
                        FX(0x604) = dist;
                        BX(0x600) = 1;
                    }
                    }
                    }
                    }
                }
                }
            }
            if (arg0->unk7F4 != 0) {
                arg0->unk374 = D_800DC000;
            }
            if (arg0->unk23C != 0) {
                FX(0x370) = arg0->unk374;
                FX(0x258) = 0.0f;
                if (arg0->unk84 & 0x200000) {
                    if (arg0->unk2C & 0x40) {
                        FX(0x974) = 1.0f;
                        FX(0x964) = 1.0f;
                        FX(0x978) = 2.0f;
                        FX(0x968) = 2.0f;
                    } else {
                        FX(0x978) = 8.0f;
                        FX(0x968) = 8.0f;
                        FX(0x974) = 6.0f;
                        FX(0x964) = 6.0f;
                    }
                    FX(0x984) = 0.0f;
                    FX(0x988) = 0.0f;
                }
            } else {
                if ((SX(0x7B8) != 0) || (arg0->unk2C & 0x100) || ((arg0->unk2C & 0x80) && (*arg0->unk36C & 0x10))) {
                    FX(0x3B4) = 6.0f;
                    FX(0x3B8) = 10.0f;
                } else if ((D_800BE616 != 0) || (arg0->unk84 & 0x200000)) {
                    if (arg0->unk2C & 0x40) {
                        FX(0x974) = 1.0f;
                        FX(0x978) = 2.0f;
                    } else {
                        FX(0x974) = 6.0f;
                        FX(0x978) = 8.0f;
                    }
                    func_150495B0(&FX(0x964), FX(0x974), &FX(0x984), 1.0f, 2.0f, arg0->unk7B4);
                    func_150495B0(&FX(0x968), FX(0x978), &FX(0x988), 1.0f, 2.0f, arg0->unk7B4);
                    FX(0x3B4) = FX(0x964);
                    FX(0x3B8) = FX(0x968);
                } else if ((*((u8 *)arg0->unk3D0 + 0x102) != 0) || (BX(0x8EC) != 0) ||
                           ((BX(0x92C) != 0) && (HX(0x920) & 0x40))) {
                    FX(0x3B4) = 6.0f;
                    FX(0x3B8) = 8.0f;
                } else if ((arg0->unk36A & 3) || (arg0->unk2C & 0x40)) {
                    FX(0x3B4) = 1.0f;
                    FX(0x3B8) = 2.0f;
                } else {
                    FX(0x3B4) = 2.0f;
                    FX(0x3B8) = 3.0f;
                }
                if (D_800BEA0C == 0) {
                    if (BX(0x600) != 0) {
                        if ((arg0->unk73C != 0) || (D_800BE616 != 0)) {
                            func_150495B0(&FX(0x370), arg0->unk374, &FX(0x258), 8.0f, 10.0f, arg0->unk7B4);
                        } else {
                            func_150495B0(&FX(0x370), arg0->unk374, &FX(0x258), 6.0f, 8.0f, arg0->unk7B4);
                        }
                    } else {
                        func_150495B0(&FX(0x370), arg0->unk374, &FX(0x258), FX(0x3B4), FX(0x3B8), arg0->unk7B4);
                    }
                } else if ((D_800895B0 == 0.0f) && ((arg0->unk3E8 != 0) || (*((u8 *)arg0->unk3D0 + 0x102) != 0))) {
                    func_150495B0(&FX(0x370), 200.0f, &FX(0x258), 4.0f, 8.0f, arg0->unk7B4);
                } else {
                    func_150495B0(&FX(0x370), 200.0f, &FX(0x258), 0.5f, 1.0f, arg0->unk7B4);
                }
            }
            arg0->unk2F8 = FX(0x370) * sinf(angle) + arg0->unk2BC;
            arg0->unk300 = FX(0x370) * cosf(angle) + arg0->unk2C4;
            FX(0x370) = sqrtf((arg0->unk2F8 - arg0->unk2BC) * (arg0->unk2F8 - arg0->unk2BC) + (arg0->unk300 - arg0->unk2C4) * (arg0->unk300 - arg0->unk2C4));
            if (arg0->unk374 + arg0->unk374 < FX(0x370)) {
                if ((*arg0->unk36C & 3) && (D_800BE616 == 0)) {
                    *arg0->unk36C &= 0xFFFC;
                    arg0->unk36A &= 0xFFFC;
                }
            }
            arg0->unk374 = saved;
        }
    }
}

#undef FX
#undef SX
#undef BX
#undef HX


/* Keeps object `index` of D_800CC2D0 in frame together with the player (0 clears it). */
void func_15124770(struct108 *camera, s32 index) {
    if (index != 0) {
        CAM->pairIndex = index;
        CAM->pairObj = &D_800CC2D0[index];
    } else {
        CAM->pairIndex = 0;
        CAM->pairObj = NULL;
    }
}

void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);

#define F(p, off) (*(f32 *)((u8 *)(p) + (off)))

void func_151247C0(struct108 *arg0) {
    f32 padA;
    f32 padB;
    f32 padC;
    f32 sp28;
    struct127 *obj;
    struct127 *other;
    u8 flag;
    f32 t;
    f32 t2;

    if (arg0->unk7F4 == 0) {
        obj = arg0->unk3D4;
        flag = (obj != NULL) ? (*((u8 *)obj + 0x95) | *(s32 *)((u8 *)obj + 0x9C)) : 0;
        if (flag) {
            arg0->unk674 = 0.25f;
        } else {
            arg0->unk674 = 1.0f;
        }
        if (arg0->unk23C) {
            arg0->unk670 = arg0->unk674;
            F(arg0, 0x678) = 0;
        } else {
            func_150495B0(&arg0->unk670, arg0->unk674, &F(arg0, 0x678), F(arg0, 0x3B4) * 0.25f, F(arg0, 0x3B8) * 0.25f, arg0->unk7B4);
        }
        if (arg0->unk244) {
            if (arg0->unk23C) {
                obj = arg0->unk3D0;
                other = (struct127 *)arg0->unk248;
                arg0->unk2A4 = (obj->x_position + other->x_position) * 0.5f;
                arg0->unk2A8 = (obj->y_position + other->y_position) * 0.5f;
                F(arg0, 0x2AC) = (obj->z_position + other->z_position) * 0.5f;
            } else {
                obj = arg0->unk3D0;
                other = (struct127 *)arg0->unk248;
                arg0->unk2A4 += (((obj->x_position + other->x_position) * 0.5f) - arg0->unk2A4) * arg0->unk670;
                arg0->unk2A8 = (obj->y_position + other->y_position) * 0.5f;
                F(arg0, 0x2AC) += (((obj->z_position + other->z_position) * 0.5f) - F(arg0, 0x2AC)) * arg0->unk670;
            }
        } else {
            if (arg0->unk23C) {
                obj = arg0->unk3D0;
                arg0->unk2A4 = obj->x_position;
                F(arg0, 0x2AC) = obj->z_position;
            } else {
                obj = arg0->unk3D0;
                arg0->unk2A4 += (obj->x_position - arg0->unk2A4) * arg0->unk670;
                F(arg0, 0x2AC) += (obj->z_position - F(arg0, 0x2AC)) * arg0->unk670;
            }
            arg0->unk2A8 = obj->y_position;
        }
        if (F(arg0, 0x994) != 0.0f || F(arg0, 0x99C) != 0.0f) {
            if (arg0->unk23C) {
                F(arg0, 0x994) = F(arg0, 0x99C);
                F(arg0, 0x998) = 0;
            } else {
                func_150495B0(&F(arg0, 0x994), F(arg0, 0x99C), &F(arg0, 0x998), 4.0f, 6.0f, arg0->unk7B4);
            }
            sp28 = F(arg0, 0x994) * sinf(arg0->unk39C - D_800A34FC);
            t = F(arg0, 0x994) * cosf(arg0->unk39C - D_800A3500);
            arg0->unk2A4 += sp28;
            F(arg0, 0x2AC) += t;
        }
        arg0->unk2BC = arg0->unk2A4;
        arg0->unk2C4 = F(arg0, 0x2AC);
        func_15125594(arg0);
    }
}

/* Unit view direction from the yaw and pitch angles. */
void func_15124AB4(struct108 *camera) {
    CAM->forward.y = -sinf(CAM->pitchRad);
    CAM->forward.z = cosf(CAM->pitchRad);
    CAM->forward.x = sinf(CAM->yawRad) * CAM->forward.z;
    CAM->forward.z = cosf(CAM->yawRad) * CAM->forward.z;
}

/* Loads the follow distance and height of the current preset (or of a mode/player state override). */
void func_15124B18(struct108 *camera) {
    s32 mode80;
    f32 *preset;

    if (CAM->preset == 0) {
        CAM->preset = 4;
    }
    mode80 = CAM->mode & 0x80;
    if (mode80 && (CAM->contPad->button & R_TRIG)) {
        CAM->height = 40.0f;
        CAM->unk34C = 40.0f;
        CAM->targetDistance = 150.0f;
    } else if (mode80) {
        CAM->height = CAM->eye.y - CAM->unk354;
    } else if (mode80 && (CAM->player->unk102 == 0) && (D_800BE9F0 != 0x17)) {
        CAM->height = 40.0f;
        CAM->unk34C = 40.0f;
        CAM->targetDistance = 150.0f;
    } else if (CAM->player->unk102 != 0) {
        CAM->height = 40.0f;
        CAM->unk34C = 40.0f;
        CAM->targetDistance = 194.0f;
    } else {
        preset = D_800A34B0[CAM->preset];
        CAM->targetDistance = preset[0];
        CAM->height = preset[1];
        CAM->unk34C = CAM->height;
    }
}

void func_1507C3E0(void *, s16 *, s16 *, s16 *);
void func_1507E1D0(void *, f32 *, f32 *, f32 *);

void func_15124C38(void *arg0, f32 *arg1) {
    f32 target;
    f32 factor;
    f32 height;

    if ((*(u16 *)((u8 *)arg0 + 0x7F4) == 0) || (arg1 != 0)) {
        if ((*(u8 **)((u8 *)arg0 + 0x3D4)) != NULL) {
            if (((*(u8 **)((u8 *)arg0 + 0x3D4))[0x4E] & 0xF) == 1) {
                if (*(s32 *)((u8 *)arg0 + 0x2C) & 0x100) {
                    height = *(s16 *)((*(u8 **)((u8 *)arg0 + 0x3D4)) + 0x114) * D_800A3504;
                } else {
                    height = *(s16 *)((*(u8 **)((u8 *)arg0 + 0x3D4)) + 0x114) * D_800A3508;
                }
            } else if (*(s32 *)((u8 *)arg0 + 0x2C) & 0x100) {
                if (*((u8 *)arg0 + 0x23E) == 0) {
                    if (*(f32 *)((u8 *)arg0 + 0x77C) < -15.0f) {
                        height = *(s16 *)((*(u8 **)((u8 *)arg0 + 0x3D4)) + 0x114) * D_800A350C + *(s16 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0xCC);
                    } else {
                        height = *(s16 *)((*(u8 **)((u8 *)arg0 + 0x3D4)) + 0x114) * 0.75f + *(s16 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0xCC);
                    }
                } else {
                    height = *(s16 *)((*(u8 **)((u8 *)arg0 + 0x3D4)) + 0x114) * D_800A3510 + *(s16 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0xCC);
                }
            } else if ((*((u8 *)arg0 + 0x23C) == 0) && (*((u8 *)arg0 + 0x600) != 0)) {
                height = (f32)*(s16 *)((*(u8 **)((u8 *)arg0 + 0x3D4)) + 0x114) + *(s16 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0xCC);
            } else {
                height = *(s16 *)((*(u8 **)((u8 *)arg0 + 0x3D4)) + 0x114) * 0.75f + *(s16 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0xCC);
            }
            if (*(s32 *)((u8 *)arg0 + 0x5F0) & 0x100) {
                height += 50.0f;
            } else if (*(s32 *)((u8 *)arg0 + 0x5F0) & 0x200) {
                height += 150.0f;
            } else if (*(s32 *)((u8 *)arg0 + 0x5F0) & 0x800) {
                height = 135.0f;
            }
        } else {
            s16 sp52;
            s16 sp50;
            s16 sp4E;

            func_1507C3E0(*(void **)((u8 *)arg0 + 0x3D0), &sp52, &sp50, &sp4E);
            height = sp52;
        }
        if ((*(u8 **)((u8 *)arg0 + 0x3D4))[0x120] != 0) {
            f32 sp48;
            f32 sp44;
            f32 sp40;

            func_1507E1D0(*(void **)((u8 *)arg0 + 0x3D0), &sp40, &sp44, &sp48);
            func_150495B0((f32 *)((u8 *)arg0 + 0x2C0), sp44, (f32 *)((u8 *)arg0 + 0x2A0), 4.0f, 8.0f,
                          *(f32 *)((u8 *)arg0 + 0x7B4));
            return;
        }
        {
            f32 speed;
            f32 limit;

            if (((*((u8 *)arg0 + 0x23E) == 0xC) || (*((u8 *)arg0 + 0x23E) == 9) || (*((u8 *)arg0 + 0x23E) == 0x38) ||
                 (*((u8 *)arg0 + 0x23E) == 0x39) || (*((u8 *)arg0 + 0x23E) == 0x37) || (*((u8 *)arg0 + 0x23E) == 0x3B) ||
                 (*((u8 *)arg0 + 0x23E) == 0x12) || (*((u8 *)arg0 + 0x23E) == 0x15) || (*((u8 *)arg0 + 0x23E) == 0x26) ||
                 (*((u8 *)arg0 + 0x23E) == 0x3A)) &&
                (*(s32 *)((u8 *)arg0 + 0x2C) & 0x100)) {
                height += 30.0f;
            }
            if ((*(s32 *)((u8 *)arg0 + 0x2C) & 0x80) || (func_15125490(arg0) != 0)) {
                target = *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x18) + height * 0.25f;
            } else if (*(s32 *)((u8 *)arg0 + 0x84) & 0x20000000) {
                target = *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x180) + height;
            } else if ((D_800C3671 != 0) || ((*(u8 **)((u8 *)arg0 + 0x3D0))[0x137] != 0) ||
                       (*(u16 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x84) == 0x194) ||
                       ((*(u8 **)((u8 *)arg0 + 0x3D0))[0xAD] == 1)) {
                target = *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x18) + height;
            } else if ((D_800A3514 == *(f32 *)((u8 *)arg0 + 0x360)) && !(*(s32 *)((u8 *)arg0 + 0x84) & 0x20000000) &&
                       !(*(s32 *)((u8 *)arg0 + 0x84) & 0x200000) &&
                       (*(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x28) < 170.0f) &&
                       (300.0f < *(f32 *)((u8 *)arg0 + 0x370))) {
                target = *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x180) + height;
            } else {
                factor = *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x28) * D_800A3518;
                factor = (factor < 0.0f) ? 0.0f : ((1.0f < factor) ? 1.0f : factor);
                target = *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x28) * factor +
                         *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x180) + height;
            }
            if ((*(u8 **)((u8 *)arg0 + 0x3D0))[0x102] != 0) {
                if (*(f32 *)((u8 *)arg0 + 0x370) < 100.0f) {
                    if (*(f32 *)((u8 *)arg0 + 0x2FC) < *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x18)) {
                        target = *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x18) - 10.0f;
                    } else {
                        target = *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x18) + 10.0f;
                    }
                } else {
                    target = *(f32 *)(*(u8 **)((u8 *)arg0 + 0x3D0) + 0x18);
                }
            }
            if (arg1 != 0) {
                *arg1 = target;
                return;
            }
            if (*((u8 *)arg0 + 0x23C) != 0) {
                *(f32 *)((u8 *)arg0 + 0x2C0) = target;
                *(f32 *)((u8 *)arg0 + 0x2A0) = 0.0f;
                return;
            }
            if (*(s32 *)((u8 *)arg0 + 0x5F0) & 0x80) {
                speed = 5.0f;
                limit = 8.0f;
            } else if (*(s32 *)((u8 *)arg0 + 0x2C) & 0x100) {
                speed = 5.0f;
                limit = 9.0f;
            } else if ((*(u8 **)((u8 *)arg0 + 0x3D0))[0x102] != 0) {
                speed = 7.0f;
                limit = 9.0f;
            } else {
                speed = 4.0f;
                limit = 8.0f;
            }
            if ((*(s32 *)((u8 *)arg0 + 0x84) & 0x200000) && (target < *(f32 *)((u8 *)arg0 + 0x2CC))) {
                speed += speed;
                limit += limit;
            }
            func_150495B0((f32 *)((u8 *)arg0 + 0x2C0), target, (f32 *)((u8 *)arg0 + 0x2A0), speed, limit,
                          *(f32 *)((u8 *)arg0 + 0x7B4));
        }
    }
}


/* Pitch of the eye->target line (degrees and radians), plus the pitch offsets unk38C / unk3A8. */
void func_1512523C(struct108 *camera) {
    f32 dz;
    f32 dx;
    f32 dist;

    dx = CAM->target.x - CAM->eye.x;
    dz = CAM->target.z - CAM->eye.z;
    dz = sqrtf((dx * dx) + (dz * dz));
    if (dz < 0.0f) {
        dist = -dz;
    } else {
        dist = dz;
    }
    dx = func_150484A0(CAM->eye.y - CAM->target.y, dist) * 57.2957763671875f;
    CAM->pitch = dx;
    if (dx > 180.0f) {
        CAM->pitch -= 360.0f;
    }
    CAM->pitch = -CAM->pitch;
    CAM->pitch -= CAM->unk3A8;
    CAM->pitch += CAM->unk38C;
    CAM->pitchRad = CAM->pitch * DEG2RAD;
}

/* Yaw (degrees and radians) of the eye->target line. */
void func_15125330(struct108 *camera) {
    struct17 delta;

    delta.unk0 = CAM->target.x - CAM->eye.x;
    delta.unk4 = 0.0f;
    delta.unk8 = CAM->target.z - CAM->eye.z;

    CAM->yaw = func_15048FC8(&delta);
    CAM->yawRad = CAM->yaw * DEG2RAD;
}

/* modeIndex = bit number of the (one-hot) mode. */
void func_15125394(struct108 *camera) {
    s32 i = 0;

    while (!(CAM->mode & (1 << i))) {
        i++;
    }
    CAM->modeIndex = i;
}

s32 func_151253CC(struct108 *arg0) {
    f32 temp_f0;
    f32 temp_f2;

    if (arg0->unk2C & 0x40000) {
        arg0->unk84 &= ~0x4F;
        arg0->unk84 |= 0x2680;
        arg0->unk3A8 = 0.0f;
        arg0->unk5E8 = 0.0f;
        arg0->unk38C = 0.0f;
        func_15125330(arg0);
        func_1512523C(arg0);
        temp_f0 = arg0->unk37C;
        temp_f2 = D_800A3528;
        arg0->unk380 = arg0->unk37C;
        arg0->unk3A0 = (f32) (temp_f0 * temp_f2);
        arg0->unk398 = (f32) (arg0->unk388 * temp_f2);
        func_15124AB4(arg0);
        if (arg0->unk3D4->unk197 != 0) {
            func_15127EB8(arg0);
        }
        func_1512C490(arg0);
        return 1;
    }
    return 0;
}

// no idea what going on here
s32 func_15125490(struct108 *arg0) {
    struct127 *temp = arg0->unk3D0;
    s32 v1;

    if (temp->in_water == 1) {
        v1 = (s32) fabsf(temp->y_position - temp->unk118);
        if (v1 < 0x64) {
            return 0;
        }
        if (v1 >= 0x12D) {
            return 1;
        }
    } else {
        return 0;
    }
}

// PERMUTER CANDIDATE (best 558): logic + operand/store order all match; only two IDO
// scheduling diffs remain (D_800A352C load placed after `sw ra`; a1-reload vs 0x670 store).
// void func_151254F4(struct108 *arg0, s32 arg1) {
//     f32 tmp = D_800A352C;
//     arg0->unk3A0 = arg0->unk380 * tmp;
//     arg0->unk398 = arg0->unk388 * tmp;
//     func_15124AB4(arg0);
//     func_151239CC(arg0, 1);
//     arg0->unk3D4->unk198 = 0;
//     arg0->unk73C = 0;
//     arg0->unk670 = 0.0f;
//     arg0->unk3D0 = &D_800CC2D0[arg1];
// }
void func_151254F4(void *arg0, s32 arg1) {
    f32 scale;

    scale = 0.01745329238474369f;
    *(f32 *)((u8 *)arg0 + 0x3A0) = *(f32 *)((u8 *)arg0 + 0x380) * scale;
    *(f32 *)((u8 *)arg0 + 0x398) = *(f32 *)((u8 *)arg0 + 0x388) * scale;
    func_15124AB4(arg0);
    func_151239CC(arg0, 1);
    *((u8 *)*(void **)((u8 *)arg0 + 0x3D4) + 0x198) = 0;
    *(s16 *)((u8 *)arg0 + 0x73C) = 0;
    *(void **)((u8 *)arg0 + 0x3D0) = (void *)((u32)D_800CC2D0 + (u32)arg1 * 0x32CU);
    *(f32 *)((u8 *)arg0 + 0x670) = 0.0f;
}
// NON-MATCHING: first statements in wrong order
// void func_151254F4(struct108 *arg0, s32 arg1) {
//     f32 tmp = D_800A352C;
//     arg0->unk3A0 = tmp * arg0->unk380;
//     arg0->unk398 = tmp * arg0->unk388;
//     func_15124AB4(arg0);
//     func_151239CC(arg0, 1);
//     arg0->unk3D4->unk198 = 0;
//     arg0->unk73C = 0;
//     arg0->unk3D0 = &D_800CC2D0[arg1];
//     arg0->unk670 = 0.0f;
// }

void func_15125594(struct108 *arg0) {
    f32 temp_f0;
    f32 temp_f2;

    func_1512523C(arg0);
    func_15125330(arg0);

    temp_f0 = arg0->unk37C;
    temp_f2 = arg0->unk5E8;
    arg0->unk380 = temp_f0 - temp_f2;
    if (temp_f0 < temp_f2) {
        arg0->unk380 += 360.0f;
    }
    arg0->unk3A0 = arg0->unk380 * D_800A3530;
    func_15124AB4(arg0);
}


void func_15125608(struct108 *arg0) {
    arg0->unk24C = 3.0f;
    arg0->unk250 = 2.5f;
}

// PERMUTER/CODEGEN CANDIDATE: logic correct, but IDO materializes each timer's address
// (lui+addiu, load/store via one reg) where target folds %lo separately per access.
// void func_15125628(void) {
//     if (D_800DBFF4[0] != 0) D_800DBFF4[0]--;
//     if (D_800DBFF5 != 0) D_800DBFF5--;
//     if (D_800DBFF6 != 0) D_800DBFF6--;
//     if ((&D_800DBFF6)[1] != 0) (&D_800DBFF6)[1]--;
// }
void func_15125628(void) {
    u8 v0;
    u8 *p4 = D_800DBFF4;
    u8 *p5 = &D_800DBFF5;
    u8 *p6 = &D_800DBFF6;
    u8 *p7 = &(&D_800DBFF6)[1];

    v0 = *p4;
    if (v0 != 0) {
        *p4 = v0 - 1;
    }
    v0 = *p5;
    if (v0 != 0) {
        *p5 = v0 - 1;
    }
    v0 = *p6;
    if (v0 != 0) {
        *p6 = v0 - 1;
    }
    v0 = *p7;
    if (v0 != 0) {
        *p7 = v0 - 1;
    }
}

void func_15125690(struct108 *arg0, s32 arg1) {
    u8 *temp_v0 = &D_800DBFF4[arg0->unk23D];

    if (*temp_v0 < arg1) {
        *temp_v0 = arg1;
    }
}

/* Camera "sway"/bob: +0x29C is the sway phase angle and +0x5EC the current sway
 * amount (structs.h mis-labels both offsets as padding). While swaying is armed
 * the phase advances by a random 0/2/4 degrees per frame and wraps at 2*PI;
 * otherwise the phase decays away. The resulting amount is fed to the camera
 * eye/target orbit routine. */
void func_151256BC(struct108 *arg0) {
    f32 twoPi;
    f32 speed;
    f32 scale;
    f32 mult;

    if (((arg0->unk2C & 0x80000) != 0) ||
        (((arg0->unk5F0 & 8) != 0) && ((arg0->unk2C & 0x40000) == 0))) {
        speed = (f32) ((u32) func_150ADA20() % 3);
        twoPi = D_800A3534;
        *(f32 *) ((u8 *) arg0 + 0x29C) += ((speed + speed) * D_800A3538) * D_800BE9A4;
        while (twoPi < *(f32 *) ((u8 *) arg0 + 0x29C)) {
            *(f32 *) ((u8 *) arg0 + 0x29C) -= twoPi;
        }
        *(f32 *) ((u8 *) arg0 + 0x5EC) += ((sinf(*(f32 *) ((u8 *) arg0 + 0x29C)) * 4.0f) -
                                           *(f32 *) ((u8 *) arg0 + 0x5EC)) * D_800A353C;
        speed = *(f32 *) ((u8 *) arg0 + 0x5EC) * D_800A3540;
        if ((arg0->unk2C & 0x80000) != 0) {
            scale = 4.0f;
            mult = 4.0f;
        } else {
            scale = 1.0f;
            mult = 20.0f;
        }
        func_1508EF80((struct17 *) &arg0->unk2F8, (struct17 *) &arg0->unk2BC, speed * scale,
                      (struct17 *) &arg0->unk2F8);
        func_1508EF80((struct17 *) &arg0->unk2BC, (struct17 *) &arg0->unk2F8, speed * mult,
                      (struct17 *) &arg0->unk2BC);
    } else if (*(f32 *) ((u8 *) arg0 + 0x29C) != 0.0f) {
        *(f32 *) ((u8 *) arg0 + 0x29C) -= *(f32 *) ((u8 *) arg0 + 0x29C) * D_800A3544;
        *(f32 *) ((u8 *) arg0 + 0x5EC) += ((sinf(*(f32 *) ((u8 *) arg0 + 0x29C)) * 4.0f) -
                                           *(f32 *) ((u8 *) arg0 + 0x5EC)) * D_800A3548;
        speed = *(f32 *) ((u8 *) arg0 + 0x5EC) * D_800A354C;
        func_1508EF80((struct17 *) &arg0->unk2F8, (struct17 *) &arg0->unk2BC, speed,
                      (struct17 *) &arg0->unk2F8);
        func_1508EF80((struct17 *) &arg0->unk2BC, (struct17 *) &arg0->unk2F8, speed * 20.0f,
                      (struct17 *) &arg0->unk2BC);
    }
}

void func_15125924(struct108 *arg0) {
    s32 temp_v1;
    s32 temp_a1;

    if (arg0->unk3D4 != NULL) {
        temp_a1 = (s32) arg0->unk3D4->z_position;
        temp_v1 = *(u8 *)((u8 *)arg0->unk3D4 + 0x4E);
    } else {
        temp_a1 = 0;
        temp_v1 = 0;
    }

    if ((arg0->unk2C & 0x40) == 0) {
        if (((arg0->unk84 & 0x4000) != 0) || (arg0->unk3D0->unk102 != 0)) {
            if (func_15125490(arg0) != 0) {
                if ((temp_v1 == 0) && (temp_a1 == 0)) {
                    if (func_15123934(arg0, 0x80, 1, 1, 0xD) != 0) {
                        if (arg0->unk6C8 == 0) {
                            func_15124B18(arg0);
                            arg0->unk5F0 |= 0x1000;
                        }
                    }
                    return;
                }
            }
            if ((arg0->unk2C & 0x80) != 0) {
                func_151239CC(arg0, 0xD);
                arg0->unk190 = 0.0f;
            }
            if ((arg0->unk5F0 & 0x1000) != 0) {
                arg0->unk5F0 &= ~0x1000;
                arg0->unk348 = (*(f32 *)((u8 *)arg0 + 0x344) = arg0->unk2FC - arg0->unk354);
            }
        }
    }
}
void func_1512D560(void *, s32, s32);
extern f32 D_800C3614;
extern f32 D_800C3618;
extern f32 D_800C361C;
extern f32 D_800C3624;
extern s8 D_800C365C;

void func_15125A6C(struct108 *arg0) {
    f32 offsetCos;
    f32 offsetSin;
    f32 radius;
    f32 yaw;
    f32 offsetRadius;
    f32 height;
    f32 trig;
    f32 angle;
    f32 offsetAngle;
    u8 mode;

    mode = arg0->unk23E;
    if (mode == 0x2A) {
        yaw = arg0->unk3D0->unk40;
        if (arg0->unk2C != 0x40000) {
            func_1512D560(arg0, 5, 0);
            arg0->unk7E4 = 1U;
        }
        *D_800DBFF4 = 3;
        ((struct168 *)&D_800C3600)->unk0 = 1;
        ((struct168 *)&D_800C3600)->unk4 = 0;
        radius = 138.0f;
        offsetRadius = 46.0f;
        height = 96.0f;
        angle = yaw * D_800A3550;
        trig = sinf(angle);
        offsetAngle = (yaw - 90.0f) * D_800A3554;
        offsetSin = sinf(offsetAngle);
        D_800C3614 = (offsetSin * offsetRadius) + (arg0->unk3D0->x_position + (radius * trig));
        D_800C3618 = arg0->unk3D0->y_position + height;
        trig = cosf(angle);
        offsetCos = cosf(offsetAngle);
        D_800C361C = (offsetCos * offsetRadius) + (arg0->unk3D0->z_position + (radius * trig));
        ((struct168 *)&D_800C3600)->unk20 = 0.0f;
        D_800C3624 = -180.0f;
        ((struct168 *)&D_800C3600)->unk8 = 0.0f;
        ((struct168 *)&D_800C3600)->unk28 = yaw - 180.0f;
        D_800C365C = 0;
        func_1512D560(arg0, 7, (s32)&D_800C3600);
        return;
    }
    if (arg0->unk7E4 != 0) {
        func_1512D560(arg0, 6, 0);
        arg0->unk7E4 = 0U;
    }
}
// NON-MATCHING: miles away
// void func_15125A6C(struct108 *arg0) {
//     f32 pad3C;
//     f32 sp38;
//     f32 pad34;
//     f32 pad30;
//     f32 sp2C;
//     f32 sp28;
//     f32 sp24;
//
//     f32 temp_f12;
//     f32 temp_f12_2;
//
//     if (arg0->unk23E == 0x2A) {
//         sp38 = arg0->unk3D0->unk40;
//         if (arg0->unk2C != 0x40000) {
//             func_1512D560(arg0, 5, 0);
//             arg0->unk7E4 = 1U;
//         }
//         D_800DBFF4[0] = 3;
//         D_800C3600->unk0 = 1;
//         D_800C3600->unk4 = 0;
//         temp_f12 = sp38 * D_800A3550;
//         sp28 = temp_f12;
//         sp2C = sinf(temp_f12);
//         temp_f12_2 = (sp38 - 90.0f) * D_800A3554;
//         sp24 = temp_f12_2;
//         D_800C3600->unk14 = (sinf(temp_f12_2) * 46.0f) + (arg0->unk3D0->unk14 + (138.0f * sp2C));
//         D_800C3600->unk18 = arg0->unk3D0->unk18 + 96.0f;
//         sp2C = cosf(temp_f12);
//         D_800C3600->unk1C = (cosf(temp_f12_2) * 46.0f) + (arg0->unk3D0->unk1C + (138.0f * sp2C));
//         D_800C3600->unk20 = 0.0f;
//         D_800C3600->unk24 = -180.0f;
//         D_800C3600->unk8 = 0.0f;
//         D_800C3600->unk28 = sp38 - 180.0f;
//         D_800C3600->unk5C = 0;
//         func_1512D560(arg0, 7, D_800C3600);
//         return;
//     }
//
//     if (arg0->unk7E4 != 0) {
//         func_1512D560(arg0, 6, 0);
//         arg0->unk7E4 = 0U;
//     }
// }

void func_1509BFB0(s32 num, s32, s32, ...);
extern u8 D_800D1940;
void func_15125C40(struct108 *arg0)
{
  s32 temp_v0;
  s32 temp_a0;
  s32 temp_a1;
  s32 temp_a2;
  u16 *temp_v0_2;
  temp_v0 = D_800D1940 == 0x42;
  temp_a1 = arg0->unk23E;
  temp_a2 = 0x1A;
  if (temp_v0 != 0)
  {
    temp_v0 = temp_a1 == temp_a2;
  }
  temp_a0 = temp_v0;
  temp_v0 = temp_a1 == 3;
  if (temp_v0 == 0)
  {
    temp_a2 = 0x1A;
    temp_v0 = temp_a1 == temp_a2;
    if (temp_v0 == 0)
    {
      temp_v0 = temp_a0 != 0;
    }
  }
  arg0->unk7CC = arg0->unk7CC - 1;
  if (temp_v0 != 0)
  {
    if (arg0->unk7CC == 0)
    {
      if (temp_a0 != 0)
      {
        func_1509BFB0(3, 0x9000, 0x18, (s32) arg0->unk3D0->unk40, 0, 0xFA);
      }
      else
        if (temp_a1 == temp_a2)
      {
        func_1509BFB0(3, 0x9000, 0x18, 0, 0, 0xFA);
      }
      temp_v0_2 = arg0->unk36C;
      arg0->unk5F0 = arg0->unk5F0 | 2;
      *temp_v0_2 |= 0x10;
      arg0->unk36A = arg0->unk36A | 0x10;
      arg0->unk7CC = 1;
    }
  }
  else
  {
    if ((arg0->unk5F0 & 2) && (temp_v0 == 0))
    {
      func_1509BFB0(1, 0x9000, 0x10, 0);
      func_1509BFB0(1, 0x9000, 0xF, 0);
      arg0->unk5F0 = arg0->unk5F0 & (~2);
    }
    arg0->unk7CC = 2;
  }
}

/* structs.h declares unk5FC as u16 (golden loads it signed) and has no member at
   0x84A at all -- it falls inside pad814[0x68].  Shadow both here rather than
   touching structs.h, which 106 other translation units include. */
typedef struct {
    /* 0x000 */ u8  pad0[0x5FC];
    /* 0x5FC */ s16 unk5FC;
    /* 0x5FE */ u8  pad5FE[0x24C];
    /* 0x84A */ s16 unk84A;
} struct108_shadow;

void func_15125DB4(struct108 *arg0) {
    s32 state;

    state = arg0->unk23E;
    if ((arg0->unk3D4->unk120 != 0) || (D_80084480 != 0)) {
        arg0->unk36A = 0;
        *arg0->unk36C &= ~0x1F;
        return;
    }

    if (state != 0) {
        if ((state == 2) || (state == 0x12) || (state == 0x13) || (state == 0xA) ||
            (state == 0x34) || (state == 0x10) || (state == 0xF)) {
            if (state == 0xF) {
                func_1509BFB0(3, 0x9000, 0x18, 0x25, 0x13, 0x1E0);
            }
            *arg0->unk36C |= 0x10;
            arg0->unk36A |= 0x10;
            *arg0->unk36C &= ~0xF;
        } else {
            arg0->unk36A = D_800BE710[arg0->unk368];
        }
        if ((state == 3) || (state == 0xD) || (state == 0x1A) || (state == 0x2A)) {
            arg0->unk36A &= ~0xF;
            *arg0->unk36C &= ~0xF;
        }
        if (state == 0x29) {
            *arg0->unk36C |= 0x10;
            arg0->unk36A |= 0x10;
        }
    } else {
        if (arg0->unk2C & 0x100) {
            arg0->unk36A &= ~0xF;
            *arg0->unk36C &= ~0xF;
        }
    }

    if ((arg0->unk3D0->in_water == 1) && (arg0->unk2C & 0x80)) {
        arg0->unk36A &= ~0x1C;
        *arg0->unk36C &= ~0xC;
    }

    if (arg0->unk84 & 0x200000) {
        arg0->unk36A &= ~0xF;
    }

    if ((D_800C3671 != 0) || (arg0->unk5F0 & 0x80)) {
        arg0->unk36A &= ~0x1F;
        *arg0->unk36C &= ~0x1F;
    }

    if ((((u8 *) arg0->unk3D4)[0x7D] != 0) || (((u8 *) arg0->unk3D4)[0x1AC] != 0) ||
        (((u8 *) arg0->unk3D4)[0x27] != 0)) {
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
    }

    if ((((u8 *) arg0->unk3D4)[0x95] != 0) ||
        ((((struct108_shadow *) arg0)->unk5FC != 0) && (((u8 *) arg0->unk3D4)[0x1B3] != 0))) {
        arg0->unk36A &= ~0x1F;
        *arg0->unk36C &= ~0x1F;
    }

    if (((arg0->unk2C & 0x100) && (arg0->unk73C == 0)) || (arg0->unk73C == 3)) {
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
    }

    if ((arg0->unk2C & 0x40) && (((struct108_shadow *) arg0)->unk84A != 0)) {
        ((struct108_shadow *) arg0)->unk84A -= D_800BE9E4;
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
        if (((struct108_shadow *) arg0)->unk84A <= 0) {
            ((struct108_shadow *) arg0)->unk84A = 0;
        }
    }
}

void func_15126138(struct108 *arg0) {
    struct127 *temp_v0_2;
    struct108 *unused;

    func_151247C0(arg0);

    temp_v0_2 = arg0->unk3D0;

    if (((temp_v0_2->x_position != arg0->unk2B0) ||
         (temp_v0_2->y_position != arg0->unk2B4) ||
         (temp_v0_2->z_position != arg0->unk2B8)) && ((arg0->unk2C & ~0x100) != 0)) {
        arg0->unk298 = 1;
        arg0->unk7E6 = 60;
        arg0->unk8ED = 0;
    } else {
        arg0->unk298 = 0;
    }

    if (arg0->unk84 & 0x200000) {
        temp_v0_2 = arg0->unk3D0; // again?!
        if (temp_v0_2->interaction_state != 0x1E) {
            if (temp_v0_2->unk102 == 0) {
                if ((*(arg0->unk36C) & 0xF) != 0) {
                    if ((arg0->unk2C & 0x40) == 0) {
                        func_151220D0(arg0);
                    }
                }
            }
        }
    }
    func_15124C38(arg0, 0);
}

typedef struct {
    u8 pad0[0x114];
    s16 unk114;
} struct_1512623C_arg1;

void func_1512623C(struct127 *arg0, struct_1512623C_arg1 *arg1, s32 arg2, f32 *arg3, f32 *arg4, f32 *arg5, s32 arg6) {
    s32 useObjectPos;
    f32 localPos[3];
    f32 worldPos[3];
    s32 boneIndex;
    s32 boneMtx;
    f32 offsetY;

    useObjectPos = 1;
    if (arg0->unk1D4 != NULL) {
        useObjectPos = 0;
        if (arg6 != 0) {
            boneIndex = 3;
            offsetY = 20.0f;
            localPos[0] = 0.0f;
            localPos[1] = offsetY;
            localPos[2] = 0.0f;
        } else if (arg2 == 0x1B) {
            boneIndex = 4;
            offsetY = 116.0f;
            localPos[1] = offsetY;
            localPos[0] = 0.0f;
            localPos[2] = 130.0f;
        } else {
            useObjectPos = 1;
        }

        if (useObjectPos == 0) {
            boneMtx = (s32)arg0->unk1D4;
            boneMtx += boneIndex << 6;
            func_15143134(localPos, worldPos, boneMtx);
            *arg3 = worldPos[0];
            *arg4 = worldPos[1];
            *arg5 = worldPos[2];
        }
    }

    if (useObjectPos != 0) {
        *arg3 = arg0->x_position;
        *arg4 = arg0->y_position + (arg1->unk114 * 0.75f);
        *arg5 = arg0->z_position;
    }
}
// ooh mama
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_15126378;

typedef struct {
    u8 pad0[0x114];
    s16 unk114;
    u8 pad116[0x11A - 0x116];
    u8 unk11A;
    u8 pad11B[0x130 - 0x11B];
    Vec3_15126378 unk130;
    Vec3_15126378 unk13C;
    Vec3_15126378 unk148;
    u8 pad154[0x160 - 0x154];
    Vec3_15126378 unk160;
    f32 unk16C;
    f32 unk170;
    f32 unk174;
    f32 unk178;
    f32 unk17C;
    f32 unk180;
    f32 unk184;
    f32 unk188;
    f32 unk18C;
    s32 unk190;
    u8 unk194;
    u8 pad195[0x197 - 0x195];
    u8 unk197;
    u8 unk198;
    u8 pad199[0x1AC - 0x199];
    u8 unk1AC;
} struct_15126378;

typedef struct {
    u8 pad0[0x6C];
    f32 unk6C;
    f32 unk70;
    u8 pad74[0x84 - 0x74];
    f32 unk84;
    u8 pad88[0xF8];
} struct_15126378_D_800BE628;

void func_151C9BA0(struct108 *arg0, s32 arg1);
s32 func_15127520();
s32 func_151277B0();

s32 func_15126378(struct108 *arg0) {
    struct_15126378 *s0;
    struct127 *target;
    f32 ratio;
    f32 aspect;
    f32 minDist;
    f32 step;
    s32 type;
    f32 scale;
    f32 angle;
    f32 f;
    f32 rad;
    f32 lo;
    f32 tmp;
    s32 buttons;
    Vec3_15126378 dir;
    f32 t;
    f32 hi;
    f32 dx;
    f32 dy;
    s32 valid;
    s32 ret;

    scale = 0.01999999955f;
    s0 = (struct_15126378 *)arg0->unk3D4;
    aspect = ((struct_15126378_D_800BE628 *)D_800BE628)[arg0->unk23D].unk70 / ((struct_15126378_D_800BE628 *)D_800BE628)[arg0->unk23D].unk6C;
    minDist = -(((struct_15126378_D_800BE628 *)D_800BE628)[arg0->unk23D].unk6C - 5.0f);
    step = ((struct_15126378_D_800BE628 *)D_800BE628)[arg0->unk23D].unk6C * scale;
    type = arg0->unk23E;
    if (((type == 0xC) && (arg0->unk3D0->unk84.uh != 0x76)) || (type == 0x1B)) {
        arg0->unk36A |= 0x10;
        ((OSContPad *)arg0->unk36C)->button |= 0x10;
    }
    if ((s0->unk197 == 0) && ((((OSContPad *)arg0->unk36C)->stick_x != 0) || (((OSContPad *)arg0->unk36C)->stick_y != 0))) {
        s0->unk194 = 1;
    }
    target = (arg0->unk3D0->unk65 != 0) ? &D_800CC2D0[arg0->unk3D0->unk65] - 1 : arg0->unk3D0;
    valid = (s0->unk11A == 2) || (type == 0x1B) || (type == 0x1F) || (type == 0x29);
    if (((type == 0x23) || (type == 0x24) || (type == 0x16) || (type == 0x18) || (type == 0x41) ||
         (type == 0xC) || (type == 0x14) || (type == 0x3F) || (type == 0x1B) || (type == 0x1F) ||
         (type == 0x29)) &&
        (((OSContPad *)arg0->unk36C)->button & 0x10) && valid &&
        (((s32)arg0->unk3D0->unk28 == 0) || ((s32)target->unk28 == 0)) &&
        (arg0->unk3D0->stunned == 0) && (s0->unk197 == 0)) {
        ((struct_15126378 *)arg0->unk3D4)->unk198 = 0;
        arg0->unk73C = 0;
        if (((type == 0x1F) || (type == 0x29)) && !(arg0->unk5F0 & 4)) {
            f = *(s16 *)&target->unk2E4 * scale;
            angle = (target->unk40 - 90.0f) - f;
            s0->unk194 = 1;
        } else {
            angle = arg0->unk3D0->unk40 - 90.0f;
        }
        func_15048758(&angle);
        rad = angle * 0.01745329238f;
        func_151239CC(arg0, 1);
        s0->unk197 = 2;
        func_151C9BA0(arg0, type);
        func_1512623C(target, (struct_1512623C_arg1 *)arg0->unk3D4, type, &((Vec3_15126378 *)&arg0->unk2F8)->x, &arg0->unk2FC, &arg0->unk300, arg0->unk5F0 & 0x10);
        arg0->unk2BC = (sinf(rad) * 100.0f) + arg0->unk2F8;
        arg0->unk2C0 = arg0->unk3D0->y_position + (((struct_15126378 *)arg0->unk3D4)->unk114 * 0.75f);
        arg0->unk2C4 = (cosf(rad) * 100.0f) + arg0->unk300;
        s0->unk13C = *((Vec3_15126378 *)&arg0->unk2F8);
        s0->unk18C = arg0->unk37C;
        s0->unk160 = *((Vec3_15126378 *)&arg0->unk2F8);
        if (type != s0->unk190) {
            if (type == 0x1B) {
                f = 1.0f;
            } else if (type == 0x16) {
                f = 0.75f;
            } else {
                f = 0.5f;
            }
            if ((type == 0x18) || (type == 0x41) || (type == 0x3F) || (type == 0x14) || (type == 0x12)) {
                f = (f < 0.0f) ? 0.0f : ((f > 0.6000000238f) ? 0.6000000238f : f);
            }
            s0->unk184 = ((0.0f - minDist) * f) + minDist;
            s0->unk194 = 1;
            s0->unk190 = type;
        }
        if (s0->unk194 != 0) {
            s0->unk174 = angle;
            s0->unk178 = 0.0f;
            s0->unk170 = 0.0f;
            s0->unk17C = 0.0f;
            s0->unk180 = 0.0f;
            s0->unk194 = 0;
            s0->unk16C = s0->unk174;
        }
        s0->unk188 = s0->unk184 * aspect;
        arg0->unk19C = s0->unk184;
        arg0->unk1A0 = s0->unk188;
        arg0->unk1A4 = s0->unk184;
        arg0->unk1A8 = s0->unk188;
        func_1510B32C(arg0->unk23D, arg0->unk19C, arg0->unk1A0, 1.0f);
        ((struct_15126378_D_800BE628 *)D_800BE628)[arg0->unk23D].unk84 = 1.0f;
        func_150627D4(arg0->unk3D0);
        D_800DBFF4[arg0->unk23D] = 2;
        if ((type == 0x24) || (type == 0x23)) {
            func_1000FA64(0x605, 0, 0, 0, 0x6590, 0x12C, 0x12B, (s32)func_15127520, arg0, (u32)(-arg0->unk1A4 * 10000.0f), 0, 0);
        } else if ((type == 0x1B) || (type == 0x1F) || (type == 0x29)) {
            tmp = s0->unk174 * s0->unk178;
            func_1000FA64(0x622, 0, 0, 0, 0x6590, 0x12C, 0x12B, (s32)func_15127520, arg0, (u32)(-arg0->unk1A4 * 10000.0f) | 0x80000000, 0x100, 0);
            func_1000FA64(0x622, 0, 0, 0, 0x3E80, 0x12C, 0x12B, (s32)func_151277B0, arg0, *(s32 *)&tmp, 0x100, 0);
        }
        target->pad2FC[0] |= 1 << arg0->unk23D;
        target->unk74 |= 1 << arg0->unk23D;
        { typedef s32 T_pad; }
    }
    ret = s0->unk197;
    if (ret == 0) {
        return 0;
    }
    if (ret == 2) {
        buttons = ((OSContPad *)arg0->unk36C)->button;
        func_1512623C(target, (struct_1512623C_arg1 *)arg0->unk3D4, type, &((Vec3_15126378 *)&arg0->unk2F8)->x, &arg0->unk2FC, &arg0->unk300, arg0->unk5F0 & 0x10);
        ratio = arg0->unk19C / minDist;
        ratio = (ratio < 0.0f) ? 0.0f : ((ratio > 1.0f) ? 1.0f : ratio);
        if (!(buttons & 0x10) || (type == 0) || (s0->unk1AC != 0) ||
            ((arg0->unk3D0->health == 0) && (type != 0x29)) ||
            ((arg0->unk3D0->stunned != 0) && (arg0->unk3D0->unk65 == 0) && ((s32)arg0->unk3D0->unk28 != 0) &&
             (type != 0x1F) && (type != 0x29))) {
            func_15127EB8(arg0);
            return 0;
        }
        if ((type == 0x1B)) {
            lo = 19.0f;
            hi = 31.0f;
        } else {
            lo = 25.0f;
            hi = 90.0f;
        }
        if (((OSContPad *)arg0->unk36C)->stick_x < 0) {
            valid = -1;
        } else {
            valid = 1;
        }
        f = 1.0f / (((hi - lo) * ratio) + lo);
        dx = valid * ((f32)((OSContPad *)arg0->unk36C)->stick_x * (f32)((OSContPad *)arg0->unk36C)->stick_x * (f * f));
        if (((OSContPad *)arg0->unk36C)->stick_y < 0) {
            valid = -1;
        } else {
            valid = 1;
        }
        dy = valid * ((f32)((OSContPad *)arg0->unk36C)->stick_y * (f32)((OSContPad *)arg0->unk36C)->stick_y * (f * f));
        if ((type == 0x1B)) {
            dx *= 0.25f;
            dy *= 0.5f;
            dx = (dx < -3.0f) ? -3.0f : ((dx > 3.0f) ? 3.0f : dx);
            dy = (dy < -3.0f) ? -3.0f : ((dy > 3.0f) ? 3.0f : dy);
        } else if (type == 0xC) {
            dx *= 1.5f;
            dy *= 1.5f;
        }
        s0->unk174 -= dx;
        s0->unk178 += dy;
        func_15048758(&s0->unk174);
        if ((type == 0x1F) || (type == 0x29)) {
            s0->unk178 = (s0->unk178 < -80.0f) ? -80.0f : ((s0->unk178 > 30.0f) ? 30.0f : s0->unk178);
        } else if ((type == 0x1B)) {
            s0->unk178 = (s0->unk178 < -60.0f) ? -60.0f : ((s0->unk178 > 50.0f) ? 50.0f : s0->unk178);
        } else {
            s0->unk178 = (s0->unk178 < -80.0f) ? -80.0f : ((s0->unk178 > 80.0f) ? 80.0f : s0->unk178);
        }
        func_15049688(&s0->unk16C, s0->unk174, &s0->unk17C, 6.0f, 8.0f, arg0->unk7B4);
        func_15049688(&s0->unk170, s0->unk178, &s0->unk180, 6.0f, 8.0f, arg0->unk7B4);
        if ((type != 0x16) && (type != 0xC) && ((type != 0x29) || (type != 0x1F) || (D_800BE616 != 0)) &&
            ((buttons & 0xC) != 0xC)) {
            if ((buttons & 8) && (minDist < arg0->unk1A4) && (type != 0xC)) {
                arg0->unk1A4 -= step;
                arg0->unk1A8 -= step * aspect;
            } else if ((buttons & 4) && (arg0->unk1A4 < 0.0f)) {
                arg0->unk1A4 += step;
                arg0->unk1A8 += step * aspect;
            }
        }
        if (type == 0x24) {
            arg0->unk1A4 = (arg0->unk1A4 < minDist) ? minDist : ((arg0->unk1A4 > 0.0f) ? 0.0f : arg0->unk1A4);
            arg0->unk1A8 = (arg0->unk1A8 < minDist * aspect) ? minDist * aspect : ((arg0->unk1A8 > 0.0f) ? 0.0f : arg0->unk1A8);
        } else if ((type == 0x1B)) {
            f = minDist * 0.5f;
            arg0->unk1A4 = (arg0->unk1A4 < f) ? f : ((arg0->unk1A4 > 0.0f) ? 0.0f : arg0->unk1A4);
            arg0->unk1A8 = (arg0->unk1A8 < f * aspect) ? f * aspect : ((arg0->unk1A8 > 0.0f) ? 0.0f : arg0->unk1A8);
        } else {
            f = minDist * 0.75f;
            arg0->unk1A4 = (arg0->unk1A4 < f) ? f : ((arg0->unk1A4 > 0.0f) ? 0.0f : arg0->unk1A4);
            arg0->unk1A8 = (arg0->unk1A8 < f * aspect) ? f * aspect : ((arg0->unk1A8 > 0.0f) ? 0.0f : arg0->unk1A8);
        }
        arg0->unk19C = arg0->unk1A4;
        arg0->unk1A0 = arg0->unk1A8;
        s0->unk184 = arg0->unk1A4;
        s0->unk188 = arg0->unk1A8;
        dir.x = 1.0f;
        t = cosf(s0->unk170 * -0.01745329238f) * dir.x;
        dir.y = sinf(s0->unk170 * -0.01745329238f) * dir.x;
        dir.x = t;
        t = cosf(s0->unk16C * -0.01745329238f) * dir.x;
        dir.z = sinf(s0->unk16C * -0.01745329238f) * dir.x;
        dir.x = t;
        arg0->unk2BC = arg0->unk2F8 + dir.x;
        arg0->unk2C0 = arg0->unk2FC + dir.y;
        arg0->unk2C4 = arg0->unk300 + dir.z;
        arg0->unk2A4 = arg0->unk3D0->x_position;
        arg0->unk2A8 = arg0->unk3D0->y_position;
        *(f32 *)&arg0->unk2AC = arg0->unk3D0->z_position;
        s0->unk148 = *(Vec3_15126378 *)&arg0->unk2BC;
        *(&s0->unk13C) = *((Vec3_15126378 *)&arg0->unk2F8);
        s0->unk130 = dir;
        func_15048F90((struct17 *)(&s0->unk13C), (struct17 *)&s0->unk148, (struct17 *)&s0->unk130);
        ((struct_15126378_D_800BE628 *)D_800BE628)[arg0->unk23D].unk84 = (-0.9399999976f * ratio) + 1.0f;
        func_1510B128(arg0->unk23D, arg0->unk19C, arg0->unk1A0, ((struct_15126378_D_800BE628 *)D_800BE628)[arg0->unk23D].unk84, (71.0f * ratio) + 54.0f);
        return 1;
    }
}
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8  pad8[0x8];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ struct108 *unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u16 unk24;
} struct_15127520_arg0;

extern u16 D_800894E0[2][3];

s32 func_15127520(struct_15127520_arg0 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *arg6) {
    struct108 *obj;
    u32 index;
    u32 oldTime;
    u32 newTime;
    f32 time;
    s32 changed;

    obj = arg0->unk18;
    index = arg0->unk1C >> 31;
    oldTime = arg0->unk1C & 0xFFFFFF;
    time = obj->unk1A4 * D_800A3580;
    changed = 0;

    if (*arg6 == D_800894E0[index][2]) {
        *arg6 = 0;
    }

    arg0->unk2 = obj->unk3D0->x_position;
    arg0->unk4 = obj->unk3D0->y_position;
    arg0->unk6 = obj->unk3D0->z_position;

    if (obj->unk3D4->unk197 == 2) {
        newTime = (u32)time;
        if (oldTime != newTime) {
            changed = 1;
            arg0->unk1C = (index << 31) | newTime;
        }

        if (changed) {
            if (*arg6 != D_800894E0[index][0]) {
                if (arg0->unk24 != 0) {
                    func_100111C8(arg0->unk24);
                }
                arg0->unk24 = 0;
                arg0->unk10 &= ~0x80;
                *arg6 = D_800894E0[index][0];
            }
        } else if (*arg6 == D_800894E0[index][0]) {
            if (arg0->unk24 != 0) {
                func_100111C8(arg0->unk24);
            }
            arg0->unk24 = 0;
            *arg6 = D_800894E0[index][1];
            func_10010F30(D_800894E0[index][1], 0x4E20, 0x40, 0, 0);
            arg0->unk10 &= ~0x80;
        } else if (arg0->unk10 & 0x80) {
            func_151CC290((s32)obj);
            func_10010F30(D_800894E0[index][2], 0x4E20, 0x40, 0, 0);
        }
    } else {
        return 1;
    }

    arg0->unk10 &= ~0x80;
    return 0;
}
s32 func_151277B0(struct_15127520_arg0 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *arg6) {
    struct108 *obj;
    u32 count;
    u32 oldTime;
    u32 newTime;
    s32 changed;
    f32 time;

    obj = arg0->unk18;
    oldTime = arg0->unk1C & 0xFFFFFF;
    count = arg0->unk1C >> 24;
    time = *(f32 *)((u8 *)obj->unk3D4 + 0x178) * *(f32 *)((u8 *)obj->unk3D4 + 0x174);
    newTime = *(u32 *)&time & 0xFFFFFF;
    changed = 0;

    if (*arg6 == 0x622) {
        *arg6 = 0;
    }

    arg0->unk2 = obj->unk3D0->x_position;
    arg0->unk4 = obj->unk3D0->y_position;
    arg0->unk6 = obj->unk3D0->z_position;

    if (obj->unk3D4->unk197 == 2) {
        if (oldTime != newTime) {
            if (count < 10) {
                if ((((oldTime - newTime) < 0) ? -(oldTime - newTime) : (oldTime - newTime)) > 100) {
                    count += D_800BE9E4;
                }
            } else {
                changed = 1;
            }
        } else {
            count = 0;
        }
        arg0->unk1C = (count << 24) | newTime;

        if (changed) {
            if (*arg6 != 0x61C) {
                if (arg0->unk24 != 0) {
                    func_100111C8(arg0->unk24);
                }
                arg0->unk24 = 0;
                arg0->unk10 &= ~0x80;
                *arg6 = 0x61C;
            }
        } else if (*arg6 == 0x61C) {
            if (arg0->unk24 != 0) {
                func_100111C8(arg0->unk24);
            }
            arg0->unk24 = 0;
            *arg6 = 0x61D;
            arg0->unk10 &= ~0x80;
        }
    } else {
        return 1;
    }

    if (arg0->unk24 == 0 && *arg6 == 0) {
        arg0->unk10 &= ~0x80;
    }
    return 0;
}
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_151279A0;

typedef struct {
    u8 pad0[0x78];
    u8 unk78;
    u8 pad79[0x114 - 0x79];
    s16 unk114;
    u8 pad116[0x130 - 0x116];
    Vec3_151279A0 unk130;
    Vec3_151279A0 unk13C;
    Vec3_151279A0 unk148;
    u8 pad154[0x16C - 0x154];
    f32 unk16C;
    f32 unk170;
    f32 unk174;
    f32 unk178;
    f32 unk17C;
    f32 unk180;
    u8 pad184[0x197 - 0x184];
    u8 unk197;
} struct_151279A0;

s32 func_151279A0(struct127 *arg0, OSContPad *arg1) {
    struct_151279A0 *s0;
    s32 type;
    Vec3_151279A0 pos;
    Vec3_151279A0 base;
    f32 speed;
    Vec3_151279A0 dir;
    f32 t;
    f32 t2;
    s32 buttons;
    f32 angle;

    s0 = (struct_151279A0 *) arg0->unk31C;
    type = s0->unk78;
    speed = D_800BE9E4 * 0.01666666753590107f;
    if ((type == 0xC) || (type == 0x1B)) {
        arg1->button |= 0x10;
    }
    if (((type == 0x23) || (type == 0x24) || (type == 0x16) || (type == 0x12) || (type == 0x18) ||
         (type == 0x41) || (type == 0xC) || (type == 0x14) || (type == 0x3F) || (type == 0x1B) ||
         (type == 0x1F) || (type == 0x29)) &&
        (arg1->button & 0x10) && (s0->unk197 == 0)) {
        s0->unk197 = 2;
        s0->unk13C = base;
        s0->unk174 = arg0->unk40 - 90.0f;
        s0->unk178 = 0.0f;
        s0->unk170 = 0.0f;
        s0->unk17C = 0.0f;
        s0->unk180 = 0.0f;
        s0->unk16C = s0->unk174;
        func_150627D4(arg0);
    }
    if (s0->unk197 == 0) {
        return 0;
    }
    if (s0->unk197 == 2) {
        buttons = arg1->button;
        angle = (arg0->unk40 - 90.0f) * 0.01745329238474369f;
        pos.x = arg0->x_position;
        pos.y = arg0->y_position + (s0->unk114 * 0.75f);
        pos.z = arg0->z_position;
        base = pos;
        pos.x = (sinf(angle) * 100.0f) + base.x;
        pos.z = (cosf(angle) * 100.0f) + base.z;
        base.y += arg0->y_position - arg0->old_y_position;
        if (!(buttons & 0x10) || (type == 0)) {
            s0->unk197 = 0;
            func_150627D4(arg0);
            return 0;
        }
        s0->unk174 -= arg1->stick_x * 0.02500000037252903f;
        s0->unk178 += arg1->stick_y * 0.02500000037252903f;
        func_15048758(&s0->unk174);
        if ((type == 0x1F) || (type == 0x29)) {
            s0->unk178 = (s0->unk178 < -20.0f) ? -20.0f : ((s0->unk178 > 80.0f) ? 80.0f : s0->unk178);
        } else {
            s0->unk178 = (s0->unk178 < -80.0f) ? -80.0f : ((s0->unk178 > 80.0f) ? 80.0f : s0->unk178);
        }
        func_15049688(&s0->unk16C, s0->unk174, &s0->unk17C, 6.0f, 8.0f, speed);
        func_15049688(&s0->unk170, s0->unk178, &s0->unk180, 6.0f, 8.0f, speed);
        dir.x = 1.0f;
        t = cosf(s0->unk170 * -0.01745329238474369f) * dir.x;
        dir.y = sinf(s0->unk170 * -0.01745329238474369f) * dir.x;
        dir.x = t;
        t = cosf(s0->unk16C * -0.01745329238474369f) * dir.x;
        dir.z = sinf(s0->unk16C * -0.01745329238474369f) * dir.x;
        dir.x = t;
        pos.x = base.x + dir.x;
        pos.y = base.y + dir.y;
        pos.z = base.z + dir.z;
        s0->unk148 = pos;
        s0->unk13C = base;
        s0->unk130 = dir;
        return 1;
    }

}

typedef struct {
    u8 pad0[0x84];
    f32 unk84;
    u8 pad88[0xF8];
} struct_15127EB8_D_800BE628;

void func_15127EB8(struct108 *arg0) {
    struct127 *temp_v1;
    struct127 *target;
    u8 temp_a0;

    func_151239CC(arg0, 1);
    arg0->unk3D4->unk197 = 0;
    func_151C9ED4(arg0);
    arg0->unk19C = 0.0f;
    arg0->unk1A0 = 0.0f;
    arg0->unk1A4 = 0.0f;
    arg0->unk1A8 = 0.0f;
    func_1510B32C(arg0->unk23D, 0.0f, 0.0f, 1.0f);
    ((struct_15127EB8_D_800BE628 *)D_800BE628)[arg0->unk23D].unk84 = 1.0f;
    func_150627D4(arg0->unk3D0);
    D_800DBFF4[arg0->unk23D] = 2;

    temp_v1 = arg0->unk3D0;
    temp_a0 = temp_v1->unk65;
    target = (temp_a0 != 0) ? &D_800CC2D0[temp_a0] - 1 : temp_v1;

    *(u8 *)((u8 *)target + 0x2FC) &= ~(1 << arg0->unk23D);
    target->unk74 &= ~(1 << arg0->unk23D);
    arg0->unk23C = 1;
}
// NON-MATCHING: not too far away
// void func_15127EB8(struct108 *arg0) {
//     struct127 *target;
//
//     func_151239CC(arg0, 1);
//     arg0->unk3D4->unk197 = (u8)0;
//     func_151C9ED4(arg0);
//     arg0->unk19C = 0.0f;
//     arg0->unk1A0 = 0.0f;
//     arg0->unk1A4 = 0.0f;
//     arg0->unk1A8 = 0.0f;
//     func_1510B32C(arg0->unk23D, 0.0f, 0.0f, 1.0f);
//     D_800BE628[arg0->unk23D].unk84 = 1.0f;
//     func_150627D4(arg0->unk3D0);
//     D_800DBFF4[arg0->unk23D] = (u8)2;
//
//     target = &arg0->unk3D0;
//     if (target->unk65 != 0) {
//         target = &D_800CC2D0[target->unk65];
//     }
//
//     target->unk2FC &= ~(1 << arg0->unk23D);
//     target->unk74 &= ~(1 << arg0->unk23D);
//     arg0->unk23C = (u8)1;
// }

void func_15127FEC(struct108 *arg0, s32 arg1, s32 arg2) {
    arg0->unk7F4 = 1;
    arg0->unk7F8 = arg0->unk2A4;
    arg0->unk7FC = arg0->unk2A8;
    arg0->unk800 = *(f32*)&arg0->unk2AC;
    func_1512A390();
}

extern f32 D_800DC004;

typedef struct {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 pad2;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u16 pad42;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
} struct108_7F4;

typedef struct {
    /* 0x000 */ u8  pad0[0x65C];
    /* 0x65C */ f32 unk65C;
    /* 0x660 */ f32 unk660;
    /* 0x664 */ u8  pad664[0x190];
    /* 0x7F4 */ struct108_7F4 c;
} struct108_ext;

#define EXT(a) (((struct108_ext *)(a))->c)

s32 func_15128030(struct108 *arg0) {
    struct108_7F4 *ctrl;

    ctrl = &EXT(arg0);
    if (arg0->unk23C != 0) {
        EXT(arg0).unk2C = D_800A35A0;
        EXT(arg0).unk40 = 0;
        EXT(arg0).state = 0;
        return 0;
    }
    switch (ctrl->state) {
    case 0:
        return 0;
    case 1:
        ctrl->unk1C = sqrtf((arg0->unk2F8 - arg0->unk3D0->x_position) * (arg0->unk2F8 - arg0->unk3D0->x_position) +
                            (arg0->unk300 - arg0->unk3D0->z_position) * (arg0->unk300 - arg0->unk3D0->z_position));
        arg0->unk5E8 = 0.0f;
        ((struct108_ext *)arg0)->unk660 = 0.0f;
        arg0->unk3A8 = 0.0f;
        ((struct108_ext *)arg0)->unk65C = 0.0f;
        EXT(arg0).unk48 = arg0->unk2FC - arg0->unk35C;
        func_15125594(arg0);
        ctrl->unk20 = arg0->unk37C;
        func_151287E0(arg0, &ctrl->unk34, &ctrl->unk38);
        EXT(arg0).unk4C = ctrl->unk40;
        ctrl->unk3C = 0.0f;
        ctrl->state = 2;
    case 2: {
        s32 done;
        f32 angle;
        done = EXT(arg0).unk4C == 0;
        if (arg0->unk23C != 0) {
            done = 1;
            ctrl->unk3C = 1.0f;
            EXT(arg0).unk4C = 0;
        } else {
            ctrl->unk3C = 1.0f - (f32)EXT(arg0).unk4C / (f32)ctrl->unk40;
        }
        func_15123568(arg0);
        ctrl->unk28 = arg0->unk374;
        EXT(arg0).unk44 = arg0->unk348;
        ctrl->unk10 = arg0->unk3D0->x_position;
        func_15124C38(arg0, &ctrl->unk14);
        angle = ctrl->unk2C;
        ctrl->unk18 = arg0->unk3D0->z_position;
        while (angle < 0.0f) {
            angle += 360.0f;
        }
        arg0->unk2A4 = arg0->unk3D0->x_position;
        arg0->unk2A8 = arg0->unk3D0->y_position;
        *(f32 *)&arg0->unk2AC = arg0->unk3D0->z_position;
        arg0->unk390 = angle;
        while (arg0->unk390 < -360.0f) {
            arg0->unk390 += 360.0f;
        }
        arg0->unk2BC = ctrl->unk4 + (ctrl->unk10 - ctrl->unk4) * ctrl->unk3C;
        arg0->unk2C0 = ctrl->unk8 + (ctrl->unk14 - ctrl->unk8) * ctrl->unk3C;
        arg0->unk2C4 = ctrl->unkC + (ctrl->unk18 - ctrl->unkC) * ctrl->unk3C;
        if (arg0->unk2C != 0x40) {
            arg0->unk37C = func_15048720(ctrl->unk3C, ctrl->unk20, ctrl->unk2C);
            arg0->unk39C = arg0->unk37C * D_800A35A4;
        }
        D_800DC000 = ctrl->unk1C + (ctrl->unk28 - ctrl->unk1C) * ctrl->unk3C;
        D_800DC004 = EXT(arg0).unk48 + (EXT(arg0).unk44 - EXT(arg0).unk48) * ctrl->unk3C;
        arg0->unk5E8 = func_15048720(ctrl->unk3C, 0.0f, ctrl->unk34);
        arg0->unk3A8 = func_15048720(ctrl->unk3C, 0.0f, ctrl->unk38);
        if (done) {
            EXT(arg0).unk2C = D_800A35A8;
            EXT(arg0).unk40 = 0;
            EXT(arg0).state = 0;
            arg0->unk84 |= 8;
            break;
        }
        EXT(arg0).unk4C -= D_800BE9E4;
        if (EXT(arg0).unk4C < 0) {
            EXT(arg0).unk4C = 0;
        }
        return 1;
    }
    }
}
void func_151283B8(struct108 *arg0) {
    switch (arg0->unk2C) {
    case 0x80:
        func_15122440(arg0);
        break;
    case 0x40:
        func_15129934(arg0);
        break;
    case 1:
        func_15122980(arg0);
        break;
    case 8:
        func_15121C80(arg0, 0.0f);
        break;
    case 2:
        func_151220D0(arg0);
        break;
    case 0x400:
        func_15122170(arg0);
        break;
    case 0x800:
        func_15122980(arg0);
        break;
    case 0x100:
        func_15120158(arg0);
        break;
    case 0x800000:
        func_1512DEA4(arg0);
        break;
    case 0x2000:
        break;
    }
}

void func_151284C4(struct108 *arg0) {
    struct127 *temp_v0;

    func_1512C490(arg0);
    temp_v0 = arg0->unk3D0;
    arg0->unk2B0 = temp_v0->x_position;
    arg0->unk2B4 = temp_v0->y_position;
    arg0->unk2B8 = temp_v0->z_position;
    if (arg0->unk23C != 0) {
        arg0->unk23C = arg0->unk23C - 1;
    }
    if ((arg0->unk84 & 8) != 0) {
        if ((s16)arg0->unk5FE <= 0) {
            arg0->unk5FE = 0x3C;
            func_15128774(arg0, arg0->unk3D0);
        }
    }
}
s32 func_15128540(struct108 *arg0) {
    s32 temp_v0;

    if (func_151253CC(arg0) != 0) {
        return 1;
    }

    if (func_15128030(arg0) != 0) {
        return 0;
    }

    temp_v0 = arg0->unk2C;
    if ((temp_v0 & 0x80000) != 0) {
        func_1512A360((u8 *)arg0);
        *(struct167 *)&arg0->unk2BC = *(struct167 *)&arg0->unk2A4;
        func_151256BC(arg0);
        func_151236D0(arg0);
        func_151284C4(arg0);
        return 1;
    }

    if ((temp_v0 & 0x100000) != 0) {
        func_1512E4B0(arg0);
        func_151236D0(arg0);
        func_151284C4(arg0);
        return 1;
    }

    if ((temp_v0 & 0x200000) != 0) {
        func_151219D0(arg0);
        func_151236D0(arg0);
        func_151284C4(arg0);
        return 1;
    }

    if ((temp_v0 & 0x400000) != 0) {
        func_1512D380((s32)arg0);
        func_151236D0(arg0);
        func_151284C4(arg0);
        return 1;
    }

    if (func_15126378(arg0) != 0) {
        func_151284C4(arg0);
        return 1;
    }
    return 0;
}

void func_15128680(struct108 *arg0) {
}

void func_1512868C(struct108 *arg0) {
    struct127 *temp_v1_2;
    u8 idx;
    s32 i;

    for (i = 0; i < 21; i++)
    {
        arg0->unk20C[i] = 0;
    }

    arg0->unk2C = 1;
    arg0->unkDC = 0;
    arg0->unk134 = 1;
    arg0->unk84 = 14;
    arg0->unk1B4 = 3;
    func_15124B18(arg0);
    idx = arg0->unk23D;
    arg0->unk3D0 = temp_v1_2 = &D_800CC2D0[idx];
    arg0->unk368 = arg0->unk3CC = idx;
    arg0->unk3D4 = temp_v1_2->unk31C;
    arg0->unk190 = 0.0f;
    arg0->unk198 = 0.0f;
    arg0->unk18C = 0.0f;
    arg0->unk194 = 0.0f;
    arg0->unk674 = 1.0f;
    arg0->unk3D4->unk198 = 0;
    arg0->unk73C = 0;
}

void func_15128774(struct108 *arg0, struct127 *arg1) {
    arg0->unk35C = arg1->unk180;
    arg0->unk304 = arg0->unk2F8 = arg1->x_position;
    arg0->unk308 = arg0->unk2FC = arg1->y_position;
    arg0->unk30C = arg0->unk300 = arg1->z_position;
    arg0->unk37C = arg0->unk3D0->unk40 - 180.0f;
    arg0->unk3D4->unk18C = D_800A35AC;
    arg0->unk23C = 1;
}
