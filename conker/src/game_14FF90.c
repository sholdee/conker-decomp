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
    /* 0x017 */ u8 pad17[0x5];
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ u8 pad20[0x7];
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
    /* 0x34 */ f32 endUnk5E8;
    /* 0x38 */ f32 endUnk3A8;
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
    /* 0x7CC */ s32 unk7CC;
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

/* Per-player view state, D_800BE628 points at an array of these (only the fields used here). */
typedef struct CamView {
    /* 0x000 */ u8 pad0[0x6C];
    /* 0x06C */ f32 unk6C;
    /* 0x070 */ f32 unk70;
    /* 0x074 */ u8 pad74[0x10];
    /* 0x084 */ f32 unk84;
    /* 0x088 */ u8 pad88[0xF8];
} CamView; // size 0x180

#define CAM_C_BUTTONS (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)

#define CAM_VIEWS ((CamView *)D_800BE628)

/* variables.h declares D_800C3600 as struct168 *; it is the struct itself (a scripted camera shot). */
#define SCRIPT_CAM (*(struct168 *)&D_800C3600)

/* variables.h declares D_800BE728 as s32[]; it holds a pointer to the per-controller OSContPad pointers. */
#define CONT_PADS (*(OSContPad ***)&D_800BE728)

/* struct127 has only pad134 over this byte. */
#define PLAYER_UNK137(obj) (((u8 *)(obj))[0x137])

/* The functions keep the struct108 * signature callers use; CAM views that `camera` parameter
   through the full layout (an inline cast: a Camera * local would cost IDO a stack slot). */
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
CAMERA_CHECK(ext_unk1C, (u32)&((CamPlayerExt *)0)->unk1C == 0x1C);
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
void func_1512A360(struct108 *camera);
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
void func_151287E0(struct108 *camera, f32 *outUnk5E8, f32 *outUnk3A8);

void func_15122C5C(struct108 *camera) {
    CamVec3f *target;
    CamVec3f *eye;
    u8 index;
    f32 fov;

    if (CAM->snap) {
        CAM->savedTarget = CAM->target;
        CAM->savedEye = CAM->eye;
    }
    target = &CAM->target;
    eye = &CAM->eye;
    if ((CAM->playerExt != NULL) && (CAM->unk73C != 3) && ((CAM->mode != 0x100) || (CAM->unk73C != 0))) {
        CAM->zone = CAM->playerExt->cameraZone;
    }
    if (D_800DBFF4[CAM->playerIndex] != 4) {
        CAM->status &= ~4;
    }
    if (!(CAM->mode & 0x400000)) {
        *target = CAM->savedTarget;
        *eye = CAM->savedEye;
    }
    index = CAM->playerIndex;
    CAM->contPad = CONT_PADS[index];
    CAM->buttonsPressed = D_800BE710[index];
    CAM->prevEye = *eye;
    CAM->unk358 = CAM->unk35C;
    CAM->prevTarget = *target;
    if (CAM->snap != 0) {
        CAM->unk19C = CAM->unk1A4;
        CAM->unk1A0 = CAM->unk1A8;
    } else {
        CAM->unk19C += (CAM->unk1A4 - CAM->unk19C) * 0.1f;
        CAM->unk1A0 += (CAM->unk1A8 - CAM->unk1A0) * 0.1f;
    }
    if (CAM->mode & 0x100) {
        fov = 16.0f;
    } else {
        fov = 0.0f;
    }
    // matching: the playerIndex reload is assigned inside the call (a separate statement reorders it)
    func_1510B128(index = CAM->playerIndex, CAM->unk19C, CAM->unk1A0, CAM_VIEWS[index].unk84, fov);
    func_15097798(CAM->playerIndex);
    func_15125A6C(camera);
    func_15128CB0(camera);
    func_15125DB4(camera);
    if ((CAM->playerExt->unk120 != 0) && (CAM->playerExt->aimState != 0)) {
        func_15127EB8(camera);
    }
    if (func_15128540(camera) == 0) {
        if ((CAM->unk84 & 0x08000000) && !(CAM->unk84 & 0x400)) {
            func_1512E8E0(camera);
        }
        func_15130230(camera, D_800BE9F0);
        func_15126138(camera);
        func_15125C40(camera);
        func_15128680(camera);
        func_1512317C(camera);
        func_15125394(camera);
        if (!(CAM->unk84 & 0x08000000) && !(CAM->unk84 & 0x400)) {
            func_1512E8E0(camera);
        }
        func_15123568(camera);
        func_1512C200(camera);
        func_15125924(camera);
        func_151283B8(camera);
        if (CAM->snap != 0) {
            func_15125594(camera);
            func_15123070(camera);
            func_151283B8(camera);
            func_151283B8(camera);
        }
        func_15123508(camera);
        if ((CAM->unk84 & 2) && (CAM->playerExt->unk120 == 0)) {
            func_1512D390(camera);
        }
        func_15125594(camera);
        func_151256BC(camera);
        func_1512D980(camera);
        func_1512C068(camera);
        func_1512B100(camera);
        func_1512C20C(camera);
        func_1512BB10(camera);
        if (CAM->snap != 0) {
            func_1512C068(camera);
            func_1512B100(camera);
            func_1512C20C(camera);
            func_1512BB10(camera);
        }
        func_15125594(camera);
        func_15123070(camera);
        if (CAM->transition.state == 0) {
            func_151287E0(camera, NULL, NULL);
        }
        func_1512C150(camera);
        func_151284C4(camera);
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

/* Whether the R-trigger camera is allowed this frame; pressing R then pushes mode 0x100 (slot 1). */
void func_1512317C(struct108 *camera) {
    s32 stunned;
    s32 status100;
    s32 status200;
    s32 blocked;

    status100 = CAM->status & 0x100;
    status200 = CAM->status & 0x200;
    stunned = (CAM->player->stunned != 0) && (CAM->zone != 9) && (CAM->zone != 0x38) &&
              (CAM->zone != 0x39) && (CAM->zone != 0x3B) && (CAM->zone != 0x37) &&
              (CAM->zone != 0x15) && (CAM->zone != 0x26) && (CAM->zone != 0x3A);

    blocked = ((CAM->mode & 0x80) != 0) ||
              ((CAM->unk84 & 0x1000000) != 0) ||
              ((CAM->player->unk28 != 0.0f) && (CAM->player->in_water == 0) &&
               (CAM->playerExt->unk9C == 0) && (status100 == 0) && (status200 == 0) &&
               (CAM->zone != 3)) ||
              ((CAM->player->unk65 != 0) && (status100 == 0) && (status200 == 0)) ||
              (stunned != 0) ||
              (func_15125490(camera) != 0) ||
              (CAM->player->health == 0) ||
              (((CAM->playerExt->unk4E & 0xF) == 1) && (CAM->player->xz_velocity > 15.0f)) ||
              (CAM->playerExt->unk1AC != 0);

    if ((CAM->contPad->button & R_TRIG) && !blocked) {
        if (!(CAM->buttonsPressed & R_TRIG)) {
            return;
        }
        if (func_15123934(camera, 0x100, 4, CAM->unk134, 1) != 0) {
            func_1511FC60(camera);
        }
        return;
    }

    if ((CAM->mode == 0x100) && (CAM->unk73C == 0)) {
        func_151239CC(camera, 1);
        CAM->playerExt->unk198 = 0;
        CAM->playerExt->aimState = 0;
        CAM->unk5F8 = 1;
        CAM->unk5FC = 2;
        CAM->buttonsPressed &= ~R_TRIG;
        CAM->contPad->button &= ~R_TRIG;
    }

    if ((CAM->mode == 0x100) || (CAM->unk73C != 0)) {
        CAM->buttonsPressed &= ~R_TRIG;
        CAM->contPad->button &= ~R_TRIG;
    }
    CAM->playerExt->unk198 = CAM->unk73C;

    if ((CAM->unk73C != 0) || (CAM->unk73C == 3)) {
        func_150495B0(&CAM->unk74C, 0.0f, &CAM->unk79C, 4.0f, 9.0f, CAM->dt);
        func_150495B0(&CAM->unk754, 0.0f, &CAM->unk7A0, 4.0f, 9.0f, CAM->dt);
    }

    if ((CAM->mode != 0x100) && (CAM->unk73C != 0)) {
        CAM->unk73C = 0;
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

/* C-up steps to the previous allowed distance preset (with a 20-frame cooldown). */
void func_15123568(struct108 *camera) {
    s32 cycle;

    if ((CAM->playerExt->unk120 == 4) && (CAM->mode != 0x40) && (CAM->unk92C == 0)) {
        CAM->unk7C0 = 999.0f;
        CAM->preset = 1;
        func_15124B18(camera);
        CAM->unk198 = 0.0f;
        CAM->unk190 = 0.0f;
        return;
    }
    if (CAM->presetCooldown > 0) {
        CAM->presetCooldown -= D_800BE9E4;
        if (CAM->presetCooldown < 0) {
            CAM->presetCooldown = 0;
        }
    } else if ((CAM->unk84 & 4) && (CAM->buttonsPressed & U_CBUTTONS) && (CAM->unkDC != 4) &&
               ((CAM->unk6C8 == 0) || (CAM->unk6FC == 4)) &&
               ((CAM->player->in_water != 1) || (CAM->preset != 1))) {
        // matching: the loop flag (not an if around for (;;)) keeps the constant 1 in a1
        cycle = CAM->preset != 1;
        while (cycle) {
            if (--CAM->preset <= 0) {
                CAM->preset = 3;
            }
            if ((1 << CAM->preset) & CAM->presetMask) {
                func_15124B18(camera);
                func_15125608(camera);
                CAM->presetCooldown = 20;
                return;
            }
            cycle = TRUE;
        }
    }
}

void func_151236D0(struct108 *camera) {
    s32 wasBelow;

    wasBelow = CAM->status & 1;
    if (CAM->playerExt != NULL) {
        if ((CAM->playerExt->unk9C != 0) ||
            ((CAM->playerExt->unk95 != 0) && (PLAYER_UNK137(CAM->player) == 0))) {
            CAM->status |= 0x40;
        } else {
            CAM->status &= ~0x40;
        }
        if (CAM->player->unk28 == 0.0f) {
            CAM->status &= ~0x400;
        }
        if ((CAM->playerExt->unk8 != 0) && (CAM->playerExt->unk16 == 0)) {
            CAM->status |= 8;
        } else {
            CAM->status &= ~8;
        }
        if (CAM->playerExt->unk4E == 2) {
            CAM->status |= 0x80;
        } else {
            CAM->status &= ~0x80;
        }
    }
    if ((CAM->eye.y <= CAM->unk360) && (CAM->mode != 0x100)) {
        CAM->status |= 1;
    } else {
        CAM->status &= ~1;
    }
    if (CAM->snap != 0) {
        if (CAM->status & 1) {
            func_1515BA80(CAM->playerIndex);
        } else {
            func_1515BA48(CAM->playerIndex);
            CAM->unk7B0 = 0.0f;
        }
    } else if (!wasBelow && (CAM->status & 1)) {
        if ((CAM->status & 4) || (CAM->mode & 0x40000)) {
            func_1515BA80(CAM->playerIndex);
        } else {
            func_1515BAAC(CAM->playerIndex);
        }
    } else if (wasBelow && !(CAM->status & 1)) {
        if ((CAM->status & 4) || (CAM->mode & 0x40000)) {
            func_1515BA48(CAM->playerIndex);
            CAM->unk7B0 = 0.0f;
        } else {
            func_1515BA54(CAM->playerIndex);
            func_15124B18(camera);
        }
    } else if (CAM->status & 1) {
        func_1515BA1C(CAM->playerIndex);
    } else {
        func_1515BA10(CAM->playerIndex);
    }
}
/* Saves the mode state in stack slot `slot` and switches to newMode. Returns 0 if the slot is taken. */
s32 func_15123934(struct108 *camera, s32 newMode, s32 newUnkDC, s32 newUnk134, s32 slot) {
    if (CAM->slotUsed[slot] == 0) {
        CAM->savedModeIndex[slot] = CAM->modeIndex;
        CAM->savedMode[slot] = CAM->mode;
        CAM->savedUnk84[slot] = CAM->unk84;
        CAM->savedUnkDC[slot] = CAM->unkDC;
        CAM->savedUnk134[slot] = CAM->unk134;
        CAM->savedPreset[slot] = CAM->preset;
        CAM->savedPresetMask[slot] = CAM->presetMask;
        CAM->mode = newMode;
        CAM->unkDC = newUnkDC;
        CAM->unk134 = newUnk134;
        CAM->slotUsed[slot] = 1;
        func_15125394(camera);
        return 1;
    }
    return 0;
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

/* Restores the mode state saved in `slot` by func_15123934. Returns 0 if the slot is empty. */
s32 func_151239CC(struct108 *camera, s32 slot) {
    if (CAM->slotUsed[slot] != 0) {
        CAM->modeIndex = CAM->savedModeIndex[slot];
        CAM->mode = CAM->savedMode[slot];
        CAM->unkDC = CAM->savedUnkDC[slot];
        CAM->unk84 = CAM->savedUnk84[slot];
        CAM->unk134 = CAM->savedUnk134[slot];
        CAM->preset = CAM->savedPreset[slot];
        CAM->presetMask = CAM->savedPresetMask[slot];
        func_15124B18(camera);
        CAM->slotUsed[slot] = 0;
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

void func_151247C0(struct108 *camera) {
    f32 pad[3]; // matching: three unused stack words in the frame
    f32 sideX;
    struct127 *obj;
    struct127 *pair;
    CamPlayerExt *ext;
    u8 slow;
    f32 sideZ;

    if (CAM->transition.state == 0) {
        ext = CAM->playerExt;
        slow = (ext != NULL) ? (ext->unk9C | ext->unk95) : 0;
        if (slow) {
            CAM->followRateTarget = 0.25f;
        } else {
            CAM->followRateTarget = 1.0f;
        }
        if (CAM->snap) {
            CAM->followRate = CAM->followRateTarget;
            CAM->followRateVel = 0;
        } else {
            func_150495B0(&CAM->followRate, CAM->followRateTarget, &CAM->followRateVel,
                          CAM->distanceAccel * 0.25f, CAM->distanceMaxSpeed * 0.25f, CAM->dt);
        }
        if (CAM->pairIndex) {
            if (CAM->snap) {
                obj = CAM->player;
                pair = CAM->pairObj;
                CAM->focus.x = (obj->x_position + pair->x_position) * 0.5f;
                CAM->focus.y = (obj->y_position + pair->y_position) * 0.5f;
                CAM->focus.z = (obj->z_position + pair->z_position) * 0.5f;
            } else {
                obj = CAM->player;
                pair = CAM->pairObj;
                CAM->focus.x += (((obj->x_position + pair->x_position) * 0.5f) - CAM->focus.x) * CAM->followRate;
                CAM->focus.y = (obj->y_position + pair->y_position) * 0.5f;
                CAM->focus.z += (((obj->z_position + pair->z_position) * 0.5f) - CAM->focus.z) * CAM->followRate;
            }
        } else {
            if (CAM->snap) {
                obj = CAM->player;
                CAM->focus.x = obj->x_position;
                CAM->focus.z = obj->z_position;
            } else {
                obj = CAM->player;
                CAM->focus.x += (obj->x_position - CAM->focus.x) * CAM->followRate;
                CAM->focus.z += (obj->z_position - CAM->focus.z) * CAM->followRate;
            }
            CAM->focus.y = obj->y_position;
        }
        if (CAM->sideOffset != 0.0f || CAM->sideOffsetTarget != 0.0f) {
            if (CAM->snap) {
                CAM->sideOffset = CAM->sideOffsetTarget;
                CAM->sideOffsetVel = 0;
            } else {
                func_150495B0(&CAM->sideOffset, CAM->sideOffsetTarget, &CAM->sideOffsetVel, 4.0f, 6.0f, CAM->dt);
            }
            sideX = CAM->sideOffset * sinf(CAM->yawRad - 1.5707963705062866f);
            sideZ = CAM->sideOffset * cosf(CAM->yawRad - 1.5707963705062866f);
            CAM->focus.x += sideX;
            CAM->focus.z += sideZ;
        }
        CAM->target.x = CAM->focus.x;
        CAM->target.z = CAM->focus.z;
        func_15125594(camera);
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

void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
void func_1507E1D0(struct127 *, f32 *, f32 *, f32 *);

void func_15124C38(struct108 *camera, f32 *outY) {
    f32 goalY;
    f32 factor;
    f32 height;

    if ((CAM->transition.state == 0) || (outY != 0)) {
        if (CAM->playerExt != NULL) {
            if ((CAM->playerExt->unk4E & 0xF) == 1) {
                if (CAM->mode & 0x100) {
                    height = CAM->playerExt->unk114 * 0.65f;
                } else {
                    height = CAM->playerExt->unk114 * 0.55f;
                }
            } else if (CAM->mode & 0x100) {
                if (CAM->zone == 0) {
                    if (CAM->unk77C < -15.0f) {
                        height = CAM->player->unkCC + CAM->playerExt->unk114 * 0.9f;
                    } else {
                        height = CAM->player->unkCC + CAM->playerExt->unk114 * 0.75f;
                    }
                } else {
                    height = CAM->player->unkCC + CAM->playerExt->unk114 * 0.85f;
                }
            } else if ((CAM->snap == 0) && (CAM->blocked != 0)) {
                height = CAM->player->unkCC + (f32)CAM->playerExt->unk114;
            } else {
                height = CAM->player->unkCC + CAM->playerExt->unk114 * 0.75f;
            }
            if (CAM->status & 0x100) {
                height += 50.0f;
            } else if (CAM->status & 0x200) {
                height += 150.0f;
            } else if (CAM->status & 0x800) {
                height = 135.0f;
            }
        } else {
            s16 top;
            s16 unused1;
            s16 unused2;

            func_1507C3E0(CAM->player, &top, &unused1, &unused2);
            height = top;
        }
        if (CAM->playerExt->unk120 != 0) {
            f32 z;
            f32 y;
            f32 x;

            func_1507E1D0(CAM->player, &x, &y, &z);
            func_150495B0(&CAM->target.y, y, &CAM->targetYVel, 4.0f, 8.0f, CAM->dt);
            return;
        }
        {
            f32 accel;
            f32 maxSpeed;

            if (((CAM->zone == 0xC) || (CAM->zone == 9) || (CAM->zone == 0x38) || (CAM->zone == 0x39) ||
                 (CAM->zone == 0x37) || (CAM->zone == 0x3B) || (CAM->zone == 0x12) || (CAM->zone == 0x15) ||
                 (CAM->zone == 0x26) || (CAM->zone == 0x3A)) &&
                (CAM->mode & 0x100)) {
                height += 30.0f;
            }
            if ((CAM->mode & 0x80) || (func_15125490(camera) != 0)) {
                goalY = CAM->player->y_position + height * 0.25f;
            } else if (CAM->unk84 & 0x20000000) {
                goalY = CAM->player->unk180 + height;
            } else if ((D_800C3671 != 0) || (PLAYER_UNK137(CAM->player) != 0) ||
                       (CAM->player->unk84.uh == 0x194) || (CAM->player->in_water == 1)) {
                goalY = CAM->player->y_position + height;
            } else if ((-10000.0f == CAM->unk360) && !(CAM->unk84 & 0x20000000) && !(CAM->unk84 & 0x200000) &&
                       (CAM->player->unk28 < 170.0f) && (300.0f < CAM->distance)) {
                goalY = CAM->player->unk180 + height;
            } else {
                factor = CAM->player->unk28 * (1.0f / 300.0f);
                factor = (factor < 0.0f) ? 0.0f : ((1.0f < factor) ? 1.0f : factor);
                goalY = CAM->player->unk180 + CAM->player->unk28 * factor + height;
            }
            if (CAM->player->unk102 != 0) {
                if (CAM->distance < 100.0f) {
                    if (CAM->eye.y < CAM->player->y_position) {
                        goalY = CAM->player->y_position - 10.0f;
                    } else {
                        goalY = CAM->player->y_position + 10.0f;
                    }
                } else {
                    goalY = CAM->player->y_position;
                }
            }
            if (outY != 0) {
                *outY = goalY;
                return;
            }
            if (CAM->snap != 0) {
                CAM->target.y = goalY;
                CAM->targetYVel = 0.0f;
                return;
            }
            if (CAM->status & 0x80) {
                accel = 5.0f;
                maxSpeed = 8.0f;
            } else if (CAM->mode & 0x100) {
                accel = 5.0f;
                maxSpeed = 9.0f;
            } else if (CAM->player->unk102 != 0) {
                accel = 7.0f;
                maxSpeed = 9.0f;
            } else {
                accel = 4.0f;
                maxSpeed = 8.0f;
            }
            if ((CAM->unk84 & 0x200000) && (goalY < CAM->prevTarget.y)) {
                accel += accel;
                maxSpeed += maxSpeed;
            }
            func_150495B0(&CAM->target.y, goalY, &CAM->targetYVel, accel, maxSpeed, CAM->dt);
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

/* Mode 0x40000 (fixed camera): keeps eye and target, refreshes the angles. Returns 1 in that mode. */
s32 func_151253CC(struct108 *camera) {
    if (CAM->mode & 0x40000) {
        CAM->unk84 &= ~0x4F;
        CAM->unk84 |= 0x2680;
        CAM->unk3A8 = 0.0f;
        CAM->unk5E8 = 0.0f;
        CAM->unk38C = 0.0f;
        func_15125330(camera);
        func_1512523C(camera);
        CAM->unk380 = CAM->yaw;
        CAM->unk3A0 = CAM->yaw * DEG2RAD;
        CAM->pitchRad = CAM->pitch * DEG2RAD;
        func_15124AB4(camera);
        if (CAM->playerExt->aimState != 0) {
            func_15127EB8(camera);
        }
        func_1512C490(camera);
        return 1;
    }
    return 0;
}

/* Whether the player is in water far (over 300 units) from unk118; 0 when closer than 100. */
s32 func_15125490(struct108 *camera) {
    struct127 *player = CAM->player;
    s32 depth;

    if (player->in_water == 1) {
        depth = fabsf(player->y_position - player->unk118);
        if (depth < 100) {
            return 0;
        }
        if (depth > 300) {
            return 1;
        }
    } else {
        return 0;
    }
    // matching: no return for 100..300 (callers see the leftover v0), as in the original
}

/* Attaches the camera to object `index` and leaves the aim state. */
void func_151254F4(struct108 *camera, s32 index) {
    CAM->unk3A0 = CAM->unk380 * DEG2RAD;
    CAM->pitchRad = CAM->pitch * DEG2RAD;
    func_15124AB4(camera);
    func_151239CC(camera, 1);
    CAM->playerExt->unk198 = 0;
    CAM->unk73C = 0;
    CAM->player = &D_800CC2D0[index];
    CAM->followRate = 0.0f;
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

/* Recomputes pitch, yaw, unk380 (yaw minus unk5E8, wrapped) and the view direction. */
void func_15125594(struct108 *camera) {
    func_1512523C(camera);
    func_15125330(camera);

    CAM->unk380 = CAM->yaw - CAM->unk5E8;
    if (CAM->yaw < CAM->unk5E8) {
        CAM->unk380 += 360.0f;
    }
    CAM->unk3A0 = CAM->unk380 * DEG2RAD;
    func_15124AB4(camera);
}


void func_15125608(struct108 *camera) {
    CAM->unk24C = 3.0f;
    CAM->unk250 = 2.5f;
}

/* Counts down the four per-player camera hold timers (D_800DBFF4[0..3]). */
void func_15125628(void) {
    // matching: a pointer per timer keeps IDO from sharing one base address (target re-lui's each access)
    u8 *timer0 = &D_800DBFF4[0];
    u8 *timer1 = &D_800DBFF4[1];
    u8 *timer2 = &D_800DBFF4[2];
    u8 *timer3 = &D_800DBFF4[3];
    u8 value;

    value = *timer0;
    if (value != 0) {
        *timer0 = value - 1;
    }
    value = *timer1;
    if (value != 0) {
        *timer1 = value - 1;
    }
    value = *timer2;
    if (value != 0) {
        *timer2 = value - 1;
    }
    value = *timer3;
    if (value != 0) {
        *timer3 = value - 1;
    }
}

/* Raises this player's hold timer to at least `frames`. */
void func_15125690(struct108 *camera, s32 frames) {
    u8 *timer = &D_800DBFF4[CAM->playerIndex];

    if (*timer < frames) {
        *timer = frames;
    }
}

/* Camera "sway"/bob: +0x29C is the sway phase angle and +0x5EC the current sway
 * amount (structs.h mis-labels both offsets as padding). While swaying is armed
 * the phase advances by a random 0/2/4 degrees per frame and wraps at 2*PI;
 * otherwise the phase decays away. The resulting amount is fed to the camera
 * eye/target orbit routine. */
void func_151256BC(struct108 *camera) {
    f32 twoPi;
    f32 amount;
    f32 eyeScale;
    f32 targetScale;

    if ((CAM->mode & 0x80000) || ((CAM->status & 8) && !(CAM->mode & 0x40000))) {
        amount = (f32)((u32)func_150ADA20() % 3);
        twoPi = 6.2831854820251465f;
        CAM->swayPhase += ((amount + amount) * DEG2RAD) * D_800BE9A4;
        while (twoPi < CAM->swayPhase) {
            CAM->swayPhase -= twoPi;
        }
        CAM->swayAmount += ((sinf(CAM->swayPhase) * 4.0f) - CAM->swayAmount) * 0.04f;
        amount = CAM->swayAmount * 0.05f;
        if (CAM->mode & 0x80000) {
            eyeScale = 4.0f;
            targetScale = 4.0f;
        } else {
            eyeScale = 1.0f;
            targetScale = 20.0f;
        }
        func_1508EF80((struct17 *)&CAM->eye, (struct17 *)&CAM->target, amount * eyeScale, (struct17 *)&CAM->eye);
        func_1508EF80((struct17 *)&CAM->target, (struct17 *)&CAM->eye, amount * targetScale,
                      (struct17 *)&CAM->target);
    } else if (CAM->swayPhase != 0.0f) {
        CAM->swayPhase -= CAM->swayPhase * 0.08f;
        CAM->swayAmount += ((sinf(CAM->swayPhase) * 4.0f) - CAM->swayAmount) * 0.04f;
        amount = CAM->swayAmount * 0.05f;
        func_1508EF80((struct17 *)&CAM->eye, (struct17 *)&CAM->target, amount, (struct17 *)&CAM->eye);
        func_1508EF80((struct17 *)&CAM->target, (struct17 *)&CAM->eye, amount * 20.0f, (struct17 *)&CAM->target);
    }
}

/* Pushes the deep-water mode 0x80 (slot 13) while func_15125490 says so, and pops it otherwise. */
void func_15125924(struct108 *camera) {
    s32 extUnk4E;
    s32 extUnk1C;

    if (CAM->playerExt != NULL) {
        extUnk1C = CAM->playerExt->unk1C;
        extUnk4E = CAM->playerExt->unk4E;
    } else {
        extUnk1C = 0;
        extUnk4E = 0;
    }

    if (!(CAM->mode & 0x40)) {
        if ((CAM->unk84 & 0x4000) || (CAM->player->unk102 != 0)) {
            if (func_15125490(camera) != 0) {
                if ((extUnk4E == 0) && (extUnk1C == 0)) {
                    if (func_15123934(camera, 0x80, 1, 1, 13) != 0) {
                        if (CAM->unk6C8 == 0) {
                            func_15124B18(camera);
                            CAM->status |= 0x1000;
                        }
                    }
                    return;
                }
            }
            if (CAM->mode & 0x80) {
                func_151239CC(camera, 13);
                CAM->unk190 = 0.0f;
            }
            if (CAM->status & 0x1000) {
                CAM->status &= ~0x1000;
                CAM->height = CAM->unk344 = CAM->eye.y - CAM->unk354;
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

/* Zone 0x2A: hands the camera to a scripted shot (D_800C3600) behind and beside the player. */
void func_15125A6C(struct108 *camera) {
    f32 sideCos;
    f32 sideSin;
    f32 radius;
    f32 yaw;
    f32 sideRadius;
    f32 height;
    f32 trig;
    f32 angle;
    f32 sideAngle;
    u8 zone;

    zone = CAM->zone;
    if (zone == 0x2A) {
        yaw = CAM->player->unk40;
        if (CAM->mode != 0x40000) {
            func_1512D560(camera, 5, 0);
            CAM->unk7E4 = 1;
        }
        D_800DBFF4[0] = 3;
        SCRIPT_CAM.unk0 = 1;
        SCRIPT_CAM.unk4 = 0;
        radius = 138.0f;
        sideRadius = 46.0f;
        height = 96.0f;
        // matching: DEG2RAD via the rodata externs; literals change the frame size
        angle = yaw * D_800A3550;
        trig = sinf(angle);
        sideAngle = (yaw - 90.0f) * D_800A3554;
        sideSin = sinf(sideAngle);
        D_800C3614 = (sideSin * sideRadius) + (CAM->player->x_position + (radius * trig));
        D_800C3618 = CAM->player->y_position + height;
        trig = cosf(angle);
        sideCos = cosf(sideAngle);
        D_800C361C = (sideCos * sideRadius) + (CAM->player->z_position + (radius * trig));
        SCRIPT_CAM.unk20 = 0.0f;
        D_800C3624 = -180.0f;
        SCRIPT_CAM.unk8 = 0.0f;
        SCRIPT_CAM.unk28 = yaw - 180.0f;
        D_800C365C = 0;
        func_1512D560(camera, 7, (s32)&SCRIPT_CAM);
        return;
    }
    if (CAM->unk7E4 != 0) {
        func_1512D560(camera, 6, 0);
        CAM->unk7E4 = 0;
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
void func_15125C40(struct108 *camera) {
    s32 level42;
    s32 active;
    s32 zone;

    zone = CAM->zone;
    level42 = (D_800D1940 == 0x42) && (zone == 0x1A);
    active = (zone == 3) || (zone == 0x1A) || level42;
    CAM->unk7CC--;
    if (active) {
        if (CAM->unk7CC == 0) {
            if (level42) {
                func_1509BFB0(3, 0x9000, 0x18, (s32)CAM->player->unk40, 0, 250);
            } else if (zone == 0x1A) {
                func_1509BFB0(3, 0x9000, 0x18, 0, 0, 250);
            }
            CAM->status |= 2;
            CAM->contPad->button |= R_TRIG;
            CAM->buttonsPressed |= R_TRIG;
            CAM->unk7CC = 1;
        }
    } else {
        if ((CAM->status & 2) && !active) {
            func_1509BFB0(1, 0x9000, 0x10, 0);
            func_1509BFB0(1, 0x9000, 0xF, 0);
            CAM->status &= ~2;
        }
        CAM->unk7CC = 2;
    }
}


/* Filters this frame's camera buttons by zone and player state (C buttons, R trigger). */
void func_15125DB4(struct108 *camera) {
    s32 zone;

    zone = CAM->zone;
    if ((CAM->playerExt->unk120 != 0) || (D_80084480 != 0)) {
        CAM->buttonsPressed = 0;
        CAM->contPad->button &= ~(R_TRIG | CAM_C_BUTTONS);
        return;
    }

    if (zone != 0) {
        if ((zone == 2) || (zone == 0x12) || (zone == 0x13) || (zone == 0xA) ||
            (zone == 0x34) || (zone == 0x10) || (zone == 0xF)) {
            if (zone == 0xF) {
                func_1509BFB0(3, 0x9000, 0x18, 0x25, 0x13, 480);
            }
            CAM->contPad->button |= R_TRIG;
            CAM->buttonsPressed |= R_TRIG;
            CAM->contPad->button &= ~CAM_C_BUTTONS;
        } else {
            CAM->buttonsPressed = D_800BE710[CAM->controller];
        }
        if ((zone == 3) || (zone == 0xD) || (zone == 0x1A) || (zone == 0x2A)) {
            CAM->buttonsPressed &= ~CAM_C_BUTTONS;
            CAM->contPad->button &= ~CAM_C_BUTTONS;
        }
        if (zone == 0x29) {
            CAM->contPad->button |= R_TRIG;
            CAM->buttonsPressed |= R_TRIG;
        }
    } else {
        if (CAM->mode & 0x100) {
            CAM->buttonsPressed &= ~CAM_C_BUTTONS;
            CAM->contPad->button &= ~CAM_C_BUTTONS;
        }
    }

    if ((CAM->player->in_water == 1) && (CAM->mode & 0x80)) {
        CAM->buttonsPressed &= ~(R_TRIG | U_CBUTTONS | D_CBUTTONS);
        CAM->contPad->button &= ~(U_CBUTTONS | D_CBUTTONS);
    }

    if (CAM->unk84 & 0x200000) {
        CAM->buttonsPressed &= ~CAM_C_BUTTONS;
    }

    if ((D_800C3671 != 0) || (CAM->status & 0x80)) {
        CAM->buttonsPressed &= ~(R_TRIG | CAM_C_BUTTONS);
        CAM->contPad->button &= ~(R_TRIG | CAM_C_BUTTONS);
    }

    if ((CAM->playerExt->unk7D != 0) || (CAM->playerExt->unk1AC != 0) || (CAM->playerExt->unk27 != 0)) {
        CAM->buttonsPressed &= ~R_TRIG;
        CAM->contPad->button &= ~R_TRIG;
    }

    if ((CAM->playerExt->unk95 != 0) || ((CAM->unk5FC != 0) && (CAM->playerExt->unk1B3 != 0))) {
        CAM->buttonsPressed &= ~(R_TRIG | CAM_C_BUTTONS);
        CAM->contPad->button &= ~(R_TRIG | CAM_C_BUTTONS);
    }

    if (((CAM->mode & 0x100) && (CAM->unk73C == 0)) || (CAM->unk73C == 3)) {
        CAM->buttonsPressed &= ~R_TRIG;
        CAM->contPad->button &= ~R_TRIG;
    }

    if ((CAM->mode & 0x40) && (CAM->unk84A != 0)) {
        CAM->unk84A -= D_800BE9E4;
        CAM->buttonsPressed &= ~R_TRIG;
        CAM->contPad->button &= ~R_TRIG;
        if (CAM->unk84A <= 0) {
            CAM->unk84A = 0;
        }
    }
}

/* Moves focus and target height, and flags whether the player moved since the last frame. */
void func_15126138(struct108 *camera) {
    struct127 *player;

    func_151247C0(camera);

    player = CAM->player;

    if (((player->x_position != CAM->lastPlayerPos.x) ||
         (player->y_position != CAM->lastPlayerPos.y) ||
         (player->z_position != CAM->lastPlayerPos.z)) && ((CAM->mode & ~0x100) != 0)) {
        CAM->playerMoved = 1;
        CAM->unk7E6 = 60;
        CAM->unk8ED = 0;
    } else {
        CAM->playerMoved = 0;
    }

    if (CAM->unk84 & 0x200000) {
        player = CAM->player;
        if (player->interaction_state != 0x1E) {
            if (player->unk102 == 0) {
                if (CAM->contPad->button & CAM_C_BUTTONS) {
                    if (!(CAM->mode & 0x40)) {
                        func_151220D0(camera);
                    }
                }
            }
        }
    }
    func_15124C38(camera, NULL);
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
/* Sound emitter handed to the aim sound callbacks by func_1000FA64. */
typedef struct CamSound {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ struct108 *camera;
    /* 0x1C */ u32 data;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u16 voice;
} CamSound;

extern u16 D_800894E0[2][3];

s32 func_15127520(CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId) {
    struct108 *camera;
    u32 index;
    u32 oldZoom;
    u32 newZoom;
    f32 zoom;
    s32 changed;

    camera = sound->camera;
    index = sound->data >> 31;
    oldZoom = sound->data & 0xFFFFFF;
    zoom = CAM->unk1A4 * -10000.0f;
    changed = 0;

    if (*soundId == D_800894E0[index][2]) {
        *soundId = 0;
    }

    sound->x = CAM->player->x_position;
    sound->y = CAM->player->y_position;
    sound->z = CAM->player->z_position;

    if (CAM->playerExt->aimState == 2) {
        newZoom = (u32)zoom;
        if (oldZoom != newZoom) {
            changed = 1;
            sound->data = (index << 31) | newZoom;
        }

        if (changed) {
            if (*soundId != D_800894E0[index][0]) {
                if (sound->voice != 0) {
                    func_100111C8(sound->voice);
                }
                sound->voice = 0;
                sound->flags &= ~0x80;
                *soundId = D_800894E0[index][0];
            }
        } else if (*soundId == D_800894E0[index][0]) {
            if (sound->voice != 0) {
                func_100111C8(sound->voice);
            }
            sound->voice = 0;
            *soundId = D_800894E0[index][1];
            func_10010F30(D_800894E0[index][1], 20000, 0x40, 0, 0);
            sound->flags &= ~0x80;
        } else if (sound->flags & 0x80) {
            func_151CC290((s32)camera);
            func_10010F30(D_800894E0[index][2], 20000, 0x40, 0, 0);
        }
    } else {
        return 1;
    }

    sound->flags &= ~0x80;
    return 0;
}
s32 func_151277B0(CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId) {
    struct108 *camera;
    u32 count;
    u32 oldKey;
    u32 newKey;
    s32 changed;
    f32 key;

    camera = sound->camera;
    oldKey = sound->data & 0xFFFFFF;
    count = sound->data >> 24;
    key = CAM->playerExt->aimYawTarget * CAM->playerExt->aimPitchTarget;
    newKey = *(u32 *)&key & 0xFFFFFF;
    changed = 0;

    if (*soundId == 0x622) {
        *soundId = 0;
    }

    sound->x = CAM->player->x_position;
    sound->y = CAM->player->y_position;
    sound->z = CAM->player->z_position;

    if (CAM->playerExt->aimState == 2) {
        if (oldKey != newKey) {
            if (count < 10) {
                if ((((oldKey - newKey) < 0) ? -(oldKey - newKey) : (oldKey - newKey)) > 100) {
                    count += D_800BE9E4;
                }
            } else {
                changed = 1;
            }
        } else {
            count = 0;
        }
        sound->data = (count << 24) | newKey;

        if (changed) {
            if (*soundId != 0x61C) {
                if (sound->voice != 0) {
                    func_100111C8(sound->voice);
                }
                sound->voice = 0;
                sound->flags &= ~0x80;
                *soundId = 0x61C;
            }
        } else if (*soundId == 0x61C) {
            if (sound->voice != 0) {
                func_100111C8(sound->voice);
            }
            sound->voice = 0;
            *soundId = 0x61D;
            sound->flags &= ~0x80;
        }
    } else {
        return 1;
    }

    if (sound->voice == 0 && *soundId == 0) {
        sound->flags &= ~0x80;
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

/* Leaves the aiming camera: restores slot 1, resets the zoom and clears this player's bit on the target. */
void func_15127EB8(struct108 *camera) {
    struct127 *player;
    struct127 *target;
    u8 linked;

    func_151239CC(camera, 1);
    CAM->playerExt->aimState = 0;
    func_151C9ED4(camera);
    CAM->unk19C = 0.0f;
    CAM->unk1A0 = 0.0f;
    CAM->unk1A4 = 0.0f;
    CAM->unk1A8 = 0.0f;
    func_1510B32C(CAM->playerIndex, 0.0f, 0.0f, 1.0f);
    CAM_VIEWS[CAM->playerIndex].unk84 = 1.0f;
    func_150627D4(CAM->player);
    D_800DBFF4[CAM->playerIndex] = 2;

    player = CAM->player;
    linked = player->unk65;
    // matching: [linked] - 1 rather than [linked - 1] (IDO folds the -0x32C differently)
    target = (linked != 0) ? &D_800CC2D0[linked] - 1 : player;

    target->pad2FC[0] &= ~(1 << CAM->playerIndex);
    target->unk74 &= ~(1 << CAM->playerIndex);
    CAM->snap = 1;
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

/* Starts a transition (func_15128030) from the current focus point. */
void func_15127FEC(struct108 *camera, s32 arg1, s32 arg2) {
    CAM->transition.state = 1;
    CAM->transition.startTarget.x = CAM->focus.x;
    CAM->transition.startTarget.y = CAM->focus.y;
    CAM->transition.startTarget.z = CAM->focus.z;
    func_1512A390();
}

extern f32 D_800DC004;


s32 func_15128030(struct108 *camera) {
    CamTransition *tr;

    tr = &CAM->transition;
    if (CAM->snap != 0) {
        CAM->transition.endYaw = 999.0f;
        CAM->transition.duration = 0;
        CAM->transition.state = 0;
        return 0;
    }
    switch (tr->state) {
    case 0:
        return 0;
    case 1:
        tr->startDistance = sqrtf((CAM->eye.x - CAM->player->x_position) * (CAM->eye.x - CAM->player->x_position) +
                                  (CAM->eye.z - CAM->player->z_position) * (CAM->eye.z - CAM->player->z_position));
        CAM->unk5E8 = 0.0f;
        CAM->unk660 = 0.0f;
        CAM->unk3A8 = 0.0f;
        CAM->unk65C = 0.0f;
        CAM->transition.startHeight = CAM->eye.y - CAM->unk35C;
        func_15125594(camera);
        tr->startYaw = CAM->yaw;
        func_151287E0(camera, &tr->endUnk5E8, &tr->endUnk3A8);
        CAM->transition.timer = tr->duration;
        tr->progress = 0.0f;
        tr->state = 2;
        // fallthrough
    case 2: {
        s32 done;
        f32 angle;

        done = CAM->transition.timer == 0;
        if (CAM->snap != 0) {
            done = 1;
            tr->progress = 1.0f;
            CAM->transition.timer = 0;
        } else {
            tr->progress = 1.0f - (f32)CAM->transition.timer / (f32)tr->duration;
        }
        func_15123568(camera);
        tr->endDistance = CAM->targetDistance;
        CAM->transition.endHeight = CAM->height;
        tr->endTarget.x = CAM->player->x_position;
        func_15124C38(camera, &tr->endTarget.y);
        angle = tr->endYaw;
        tr->endTarget.z = CAM->player->z_position;
        while (angle < 0.0f) {
            angle += 360.0f;
        }
        CAM->focus.x = CAM->player->x_position;
        CAM->focus.y = CAM->player->y_position;
        CAM->focus.z = CAM->player->z_position;
        CAM->unk390 = angle;
        while (CAM->unk390 < -360.0f) {
            CAM->unk390 += 360.0f;
        }
        CAM->target.x = tr->startTarget.x + (tr->endTarget.x - tr->startTarget.x) * tr->progress;
        CAM->target.y = tr->startTarget.y + (tr->endTarget.y - tr->startTarget.y) * tr->progress;
        CAM->target.z = tr->startTarget.z + (tr->endTarget.z - tr->startTarget.z) * tr->progress;
        if (CAM->mode != 0x40) {
            CAM->yaw = func_15048720(tr->progress, tr->startYaw, tr->endYaw);
            CAM->yawRad = CAM->yaw * DEG2RAD;
        }
        D_800DC000 = tr->startDistance + (tr->endDistance - tr->startDistance) * tr->progress;
        D_800DC004 = CAM->transition.startHeight +
                     (CAM->transition.endHeight - CAM->transition.startHeight) * tr->progress;
        CAM->unk5E8 = func_15048720(tr->progress, 0.0f, tr->endUnk5E8);
        CAM->unk3A8 = func_15048720(tr->progress, 0.0f, tr->endUnk3A8);
        if (done) {
            CAM->transition.endYaw = 999.0f;
            CAM->transition.duration = 0;
            CAM->transition.state = 0;
            CAM->unk84 |= 8;
            break;
        }
        CAM->transition.timer -= D_800BE9E4;
        if (CAM->transition.timer < 0) {
            CAM->transition.timer = 0;
        }
        return 1;
    }
    }
    // matching: no return after the transition ends (callers see the leftover v0), as in the original
}
/* Runs the handler of the current mode. */
void func_151283B8(struct108 *camera) {
    switch (CAM->mode) {
    case 0x80:
        func_15122440(camera);
        break;
    case 0x40:
        func_15129934(camera);
        break;
    case 1:
        func_15122980(camera);
        break;
    case 8:
        func_15121C80(camera, 0.0f);
        break;
    case 2:
        func_151220D0(camera);
        break;
    case 0x400:
        func_15122170(camera);
        break;
    case 0x800:
        func_15122980(camera);
        break;
    case 0x100:
        func_15120158(camera);
        break;
    case 0x800000:
        func_1512DEA4(camera);
        break;
    case 0x2000:
        break;
    }
}

/* End of the camera update: remembers the player position and counts down the snap frames. */
void func_151284C4(struct108 *camera) {
    struct127 *player;

    func_1512C490(camera);
    player = CAM->player;
    CAM->lastPlayerPos.x = player->x_position;
    CAM->lastPlayerPos.y = player->y_position;
    CAM->lastPlayerPos.z = player->z_position;
    if (CAM->snap != 0) {
        CAM->snap--;
    }
    if (CAM->unk84 & 8) {
        if (CAM->unk5FE <= 0) {
            CAM->unk5FE = 60;
            func_15128774(camera, CAM->player);
        }
    }
}
/* Modes that replace the normal follow logic. Returns 1 if one of them ran the whole update. */
s32 func_15128540(struct108 *camera) {
    s32 mode;

    if (func_151253CC(camera) != 0) {
        return 1;
    }

    if (func_15128030(camera) != 0) {
        return 0;
    }

    mode = CAM->mode;
    if (mode & 0x80000) {
        func_1512A360(camera);
        CAM->target = CAM->focus;
        func_151256BC(camera);
        func_151236D0(camera);
        func_151284C4(camera);
        return 1;
    }

    if (mode & 0x100000) {
        func_1512E4B0(camera);
        func_151236D0(camera);
        func_151284C4(camera);
        return 1;
    }

    if (mode & 0x200000) {
        func_151219D0(camera);
        func_151236D0(camera);
        func_151284C4(camera);
        return 1;
    }

    if (mode & 0x400000) {
        func_1512D380((s32)camera);
        func_151236D0(camera);
        func_151284C4(camera);
        return 1;
    }

    if (func_15126378(camera) != 0) {
        func_151284C4(camera);
        return 1;
    }
    return 0;
}

void func_15128680(struct108 *camera) {
}

/* Resets the camera to its defaults and attaches it to player playerIndex. */
void func_1512868C(struct108 *camera) {
    struct127 *player;
    u8 index;
    s32 i;

    for (i = 0; i < 21; i++) {
        CAM->slotUsed[i] = 0;
    }

    CAM->mode = 1;
    CAM->unkDC = 0;
    CAM->unk134 = 1;
    CAM->unk84 = 14;
    CAM->preset = 3;
    func_15124B18(camera);
    index = CAM->playerIndex;
    CAM->player = player = &D_800CC2D0[index];
    CAM->controller = CAM->unk3CC = index;
    CAM->playerExt = (CamPlayerExt *)player->unk31C;
    CAM->unk190 = 0.0f;
    CAM->unk198 = 0.0f;
    CAM->unk18C = 0.0f;
    CAM->unk194 = 0.0f;
    CAM->followRateTarget = 1.0f;
    CAM->playerExt->unk198 = 0;
    CAM->unk73C = 0;
}

/* Places the eye at obj (current and previous position) and looks the way the player faces. */
void func_15128774(struct108 *camera, struct127 *obj) {
    CAM->unk35C = obj->unk180;
    CAM->prevEye.x = CAM->eye.x = obj->x_position;
    CAM->prevEye.y = CAM->eye.y = obj->y_position;
    CAM->prevEye.z = CAM->eye.z = obj->z_position;
    CAM->yaw = CAM->player->unk40 - 180.0f;
    CAM->playerExt->unk18C = 999.0f;
    CAM->snap = 1;
}
