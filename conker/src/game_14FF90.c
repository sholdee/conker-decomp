#include <ultra64.h>

#include "functions.h"
#include "variables.h"


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

#define CAM_C_BUTTONS (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)

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

extern f32 D_800895B0;
extern u8 D_800BEA0C;
extern f32 D_800DC000;
extern f32 D_800C3614;
extern f32 D_800C3618;
extern f32 D_800C361C;
extern f32 D_800C3624;
extern s8 D_800C365C;
extern u8 D_800D1940;
extern f32 D_800A34B0[4][2];
extern f32 D_800A3550;
extern f32 D_800A3554;
extern u16 D_800894E0[2][3];
extern f32 D_800DC004;

void func_15120158(struct108 *camera);
void func_15122C5C(struct108 *camera);
void func_15121C80(struct108 *camera, f32 arg1);
void func_151220D0(struct108 *camera);
void func_15122170(struct108 *camera);
void func_15122440(struct108 *camera);
void func_15122980(struct108 *camera);
void func_151219D0(struct108 *camera);
void func_151236D0(struct108 *camera);
void func_151256BC(struct108 *camera);
s32 func_15126378(struct108 *camera);
s32 func_15128030(struct108 *camera);
void func_15129934(struct108 *camera);
void func_1512A360(struct108 *camera);
void func_1512E4B0(struct108 *camera);
void func_1512DEA4(struct108 *camera);
void func_15143134(f32 *in, f32 *out, Mtx *mtx);
void func_1508EF80(struct17 *arg0, struct17 *arg1, f32 arg2, struct17 *arg3);
void func_151CC290(s32 arg0);
void func_1515BA10(s32 arg0);
void func_1515BA1C(s16 arg0);
void func_1515BA48(s32 arg0);
void func_1515BA54(s16 arg0);
void func_1515BA80(s16 arg0);
void func_1515BAAC(s16 arg0);
void func_1510B128(s32, f32, f32, f32, f32);
void func_15097798(u8);
void func_15130230(struct108 *camera, s32 arg1);
void func_15125A6C(struct108 *camera);
void func_15128CB0(struct108 *camera);
void func_15125DB4(struct108 *camera);
void func_15127EB8(struct108 *camera);
void func_1512E8E0(struct108 *camera);
void func_15126138(struct108 *camera);
void func_15125C40(struct108 *camera);
void func_15128680(struct108 *camera);
void func_1512317C(struct108 *camera);
void func_15125394(struct108 *camera);
void func_15123568(struct108 *camera);
void func_1512C200(struct108 *camera);
void func_15125924(struct108 *camera);
void func_151283B8(struct108 *camera);
void func_15125594(struct108 *camera);
void func_15123070(struct108 *camera);
void func_15123508(struct108 *camera);
void func_1512D390(struct108 *camera);
void func_1512D980(struct108 *camera);
void func_1512C068(struct108 *camera);
void func_1512B100(struct108 *camera);
void func_1512C20C(struct108 *camera);
void func_1512BB10(struct108 *camera);
void func_1512C150(struct108 *camera);
void func_151284C4(struct108 *camera);
s32 func_15128540(struct108 *camera);
void func_151287E0(struct108 *camera, f32 *outUnk5E8, f32 *outUnk3A8);
void func_1511FC60(struct108 *camera);
void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
s32 func_15125490(struct108 *camera);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, s16 *, f32 *, f32 *, f32 *, f32 *, s32, s32, f32);
void func_150AD8B0(struct17 *, struct17 *, struct17 *);
void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
void func_1507E1D0(struct127 *, f32 *, f32 *, f32 *);
void func_1512D560(void *, s32, s32);
void func_1509BFB0(s32 num, s32, s32, ...);
void func_151C9BA0(struct108 *camera, s32 arg1);
s32 func_15127520(CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId);
s32 func_151277B0(CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId);

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

/* Per-frame update of one player camera: input, zone state, then either a special mode or the
   follow camera (focus, distance, angles, collision). */
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
    func_1510B128(index = CAM->playerIndex, CAM->unk19C, CAM->unk1A0, D_800BE628[index].perspScale, fov);
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

/* Updates the status bits taken from the player state, and bit 1 (eye at or below unk360), starting
   or stopping the matching per-player effects (func_1515BAxx) when bit 1 changes. */
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

/* Follow-camera distance and yaw: look-behind (C-down), wall collision between focus and eye, and
   easing of the current distance; then places the eye behind the target at that distance. */
void func_15123A54(struct108 *camera) {
    f32 angle;
    f32 savedDistance;

    savedDistance = CAM->targetDistance;
    CAM->blocked = 0;
    CAM->blockedDistance = CAM->targetDistance;
    if (D_800C3671 == 0) {
        if (CAM->maxDistance != 0.0f) {
            CAM->targetDistance = (CAM->maxDistance < CAM->targetDistance) ? CAM->maxDistance : CAM->targetDistance;
        }
        if (CAM->snap == 0) {
            if ((CAM->status & 0x10) && (CAM->contPad->button & R_TRIG)) {
                CAM->targetDistance = 130.0f;
            }
            if ((CAM->unk73C == 0) && (CAM->status & 0x40)) {
                CAM->targetDistance = 420.0f;
            }
            if (CAM->mode & 0x403) {
                if (CAM->buttonsPressed & D_CBUTTONS) {
                    CAM->lookBehindTimer = 1;
                    CAM->lookBehindHold = 180;
                    CAM->lookBehindYawVel = 0.0f;
                } else if ((CAM->contPad->button & D_CBUTTONS) || CAM->lookBehindTimer) {
                    if (CAM->lookBehindTimer && (CAM->contPad->button & D_CBUTTONS)) {
                        CAM->lookBehindTimer += D_800BEA08;
                        CAM->lookBehindTimer = (CAM->lookBehindTimer >= 16) ? 15 : CAM->lookBehindTimer;
                    }
                    if (CAM->lookBehindTimer == 15) {
                        CAM->lookingBehind = 1;
                        CAM->presetCooldown = 20;
                        CAM->targetDistance = 300.0f;
                        CAM->height = 150.0f;
                        func_15049688(&CAM->yaw, CAM->player->unk40 - 180.0f, &CAM->lookBehindYawVel, 1.5f, 2.0f,
                                      CAM->dt);
                        if (!(CAM->contPad->button & D_CBUTTONS)) {
                            CAM->lookBehindTimer = 0;
                        }
                    } else {
                        func_15049688(&CAM->yaw, CAM->player->unk40 - 180.0f, &CAM->lookBehindYawVel, 2.0f, 4.0f,
                                      CAM->dt);
                    }
                    CAM->yawRad = CAM->yaw * DEG2RAD;
                    if ((CAM->lookBehindTimer < 15) && !(CAM->contPad->button & D_CBUTTONS)) {
                        if (CAM->playerMoved == 0) {
                            CAM->lookBehindHold -= D_800BEA08;
                            if (CAM->lookBehindHold <= 0) {
                                CAM->lookBehindTimer = 0;
                            } else if (fabsf(CAM->yaw - (CAM->player->unk40 - 180.0f)) <= 2.0f) {
                                CAM->lookBehindTimer = 0;
                            }
                        } else {
                            CAM->lookBehindTimer = 0;
                        }
                    }
                } else if (CAM->lookingBehind != 0) {
                    func_15124B18(camera);
                    CAM->lookingBehind = 0;
                    CAM->lookBehindTimer = 0;
                }
            }
        } else {
            CAM->yawRad = CAM->yaw * DEG2RAD;
        }
        angle = CAM->yawRad;
        if (D_800BEA0C != 0) {
            // matching: only the (f32)1 factor matches (dropping it or writing 1.0f changes the code)
            CAM->yaw += (f32)1 * CAM->spinDir * D_800BEA08;
            CAM->yawRad = CAM->yaw * DEG2RAD;
            angle = CAM->yawRad;
        } else if (func_150ADA20() & 1) {
            CAM->spinDir = -1.0f;
        } else {
            CAM->spinDir = 1.0f;
        }
        if ((CAM->playerExt->unk95 != 0) && (CAM->status & 0x40)) {
            angle = CAM->player->unk40 + 180.0f;
            func_15049688(&CAM->yaw, angle, &CAM->yawVel, 2.0f, 4.0f, CAM->dt);
            CAM->yawRad = CAM->yaw * DEG2RAD;
            angle = CAM->yawRad;
            CAM->targetDistance = 530.0f;
        }
        if (CAM->mode != 0x2000) {
            CAM->distance = sqrtf((CAM->eye.x - CAM->target.x) * (CAM->eye.x - CAM->target.x) +
                                  (CAM->eye.z - CAM->target.z) * (CAM->eye.z - CAM->target.z));
            if ((CAM->unk73C != 0) || (CAM->snap != 0) || (CAM->player->unk102 != 0) ||
                (!(CAM->unk84 & 0x1000) && (CAM->unk92C == 0) && !(CAM->unk84 & 0x40000) && !(CAM->status & 0x80))) {
                struct17 dir;
                s16 tri[10];
                f32 hit[3];
                f32 dist;
                f32 pad1; // matching: unused stack slots (pad1, pad2) in the frame
                s16 tri2[10];
                f32 backZ;
                f32 pad2;
                f32 backX;
                struct17 edge1;
                struct17 edge2;
                struct17 normal;
                f32 hit2[3];
                f32 dist2;
                f32 prevDistance;
                struct17 v0;
                struct17 v1;
                struct17 v2;

                dir.unk0 = sinf(CAM->yawRad);
                dir.unk4 = 0.0f;
                dir.unk8 = cosf(CAM->yawRad);
                if ((func_150AC9C0(CAM->focus.x, CAM->focus.y + 140.0f, CAM->focus.z, dir.unk0, dir.unk4, dir.unk8, 0,
                                   tri, &hit[0], &hit[1], &hit[2], &dist, 0, 0, CAM->targetDistance + 30.0f) != 0) &&
                    (dist < CAM->targetDistance)) {
                    backX = sinf(CAM->yawRad + 3.1415927410125732f) * CAM->targetDistance;
                    backZ = cosf(CAM->yawRad + 3.1415927410125732f) * CAM->targetDistance;
                    v0.unk0 = tri[0];
                    v0.unk4 = tri[1];
                    v0.unk8 = tri[2];
                    v1.unk0 = tri[3];
                    v1.unk4 = tri[4];
                    v1.unk8 = tri[5];
                    v2.unk0 = tri[6];
                    v2.unk4 = tri[7];
                    v2.unk8 = tri[8];
                    func_15048F90(&v0, &v1, &edge1);
                    func_15048F90(&v0, &v2, &edge2);
                    func_150AD8B0(&edge2, &edge1, &normal);
                    func_1504917C(&normal, &normal);
                    if (0.3253220021724701f < func_150AD900(&dir.unk0, &normal.unk0)) {
                        if (func_150AC9C0(CAM->focus.x - backX, CAM->focus.y + 140.0f, CAM->focus.z - backZ, backX,
                                          0.0f, backZ, 0, tri2, &hit2[0], &hit2[1], &hit2[2], &dist2, 0, 0,
                                          CAM->targetDistance + 30.0f) != 0) {
                            f32 dx;
                            f32 dz;

                            dx = hit2[0] - hit[0];
                            dz = hit2[2] - hit[2];
                            if ((sqrtf(dx * dx + dz * dz) < 1.0f) || (CAM->status & 0x10) || (CAM->zone == 0xC) ||
                                (CAM->zone == 9) || (CAM->zone == 0x38) || (CAM->zone == 0x39) ||
                                (CAM->zone == 0x37) || (CAM->zone == 0x3B) || (CAM->zone == 0x12) ||
                                (CAM->zone == 0x15) || (CAM->zone == 0x26) || (CAM->zone == 0x3A) ||
                                (CAM->snap != 0)) {
                                prevDistance = CAM->targetDistance;
                                if (CAM->unk73C != 0) {
                                    dist -= 16.0f;
                                } else {
                                    dist -= 12.0f;
                                }
                                dist = (dist < 50.0f) ? 50.0f : dist;
                                CAM->targetDistance = (dist < CAM->targetDistance) ? dist : CAM->targetDistance;
                                if (CAM->zone == 3) {
                                    CAM->height *= CAM->targetDistance / prevDistance;
                                    if (CAM->status & 0x10) {
                                        CAM->height = (CAM->height < CAM->target.y - CAM->unk354)
                                                          ? CAM->target.y - CAM->unk354
                                                          : CAM->height;
                                    }
                                }
                                CAM->blockedDistance = dist;
                                CAM->blocked = 1;
                            }
                        }
                    }
                }
            }
            if (CAM->transition.state != 0) {
                CAM->targetDistance = D_800DC000;
            }
            if (CAM->snap != 0) {
                CAM->distance = CAM->targetDistance;
                CAM->distanceVel = 0.0f;
                if (CAM->unk84 & 0x200000) {
                    if (CAM->mode & 0x40) {
                        CAM->unk974 = 1.0f;
                        CAM->unk964 = 1.0f;
                        CAM->unk978 = 2.0f;
                        CAM->unk968 = 2.0f;
                    } else {
                        CAM->unk978 = 8.0f;
                        CAM->unk968 = 8.0f;
                        CAM->unk974 = 6.0f;
                        CAM->unk964 = 6.0f;
                    }
                    CAM->unk984 = 0.0f;
                    CAM->unk988 = 0.0f;
                }
            } else {
                if ((CAM->presetCooldown != 0) || (CAM->mode & 0x100) ||
                    ((CAM->mode & 0x80) && (CAM->contPad->button & R_TRIG))) {
                    CAM->distanceAccel = 6.0f;
                    CAM->distanceMaxSpeed = 10.0f;
                } else if ((D_800BE616 != 0) || (CAM->unk84 & 0x200000)) {
                    if (CAM->mode & 0x40) {
                        CAM->unk974 = 1.0f;
                        CAM->unk978 = 2.0f;
                    } else {
                        CAM->unk974 = 6.0f;
                        CAM->unk978 = 8.0f;
                    }
                    func_150495B0(&CAM->unk964, CAM->unk974, &CAM->unk984, 1.0f, 2.0f, CAM->dt);
                    func_150495B0(&CAM->unk968, CAM->unk978, &CAM->unk988, 1.0f, 2.0f, CAM->dt);
                    CAM->distanceAccel = CAM->unk964;
                    CAM->distanceMaxSpeed = CAM->unk968;
                } else if ((CAM->player->unk102 != 0) || (CAM->lookingBehind != 0) ||
                           ((CAM->unk92C != 0) && (CAM->unk920 & 0x40))) {
                    CAM->distanceAccel = 6.0f;
                    CAM->distanceMaxSpeed = 8.0f;
                } else if ((CAM->buttonsPressed & (L_CBUTTONS | R_CBUTTONS)) || (CAM->mode & 0x40)) {
                    CAM->distanceAccel = 1.0f;
                    CAM->distanceMaxSpeed = 2.0f;
                } else {
                    CAM->distanceAccel = 2.0f;
                    CAM->distanceMaxSpeed = 3.0f;
                }
                if (D_800BEA0C == 0) {
                    if (CAM->blocked != 0) {
                        if ((CAM->unk73C != 0) || (D_800BE616 != 0)) {
                            func_150495B0(&CAM->distance, CAM->targetDistance, &CAM->distanceVel, 8.0f, 10.0f, CAM->dt);
                        } else {
                            func_150495B0(&CAM->distance, CAM->targetDistance, &CAM->distanceVel, 6.0f, 8.0f, CAM->dt);
                        }
                    } else {
                        func_150495B0(&CAM->distance, CAM->targetDistance, &CAM->distanceVel, CAM->distanceAccel,
                                      CAM->distanceMaxSpeed, CAM->dt);
                    }
                } else if ((D_800895B0 == 0.0f) && ((CAM->unk3E8 != 0) || (CAM->player->unk102 != 0))) {
                    func_150495B0(&CAM->distance, 200.0f, &CAM->distanceVel, 4.0f, 8.0f, CAM->dt);
                } else {
                    func_150495B0(&CAM->distance, 200.0f, &CAM->distanceVel, 0.5f, 1.0f, CAM->dt);
                }
            }
            CAM->eye.x = CAM->distance * sinf(angle) + CAM->target.x;
            CAM->eye.z = CAM->distance * cosf(angle) + CAM->target.z;
            CAM->distance = sqrtf((CAM->eye.x - CAM->target.x) * (CAM->eye.x - CAM->target.x) +
                                  (CAM->eye.z - CAM->target.z) * (CAM->eye.z - CAM->target.z));
            if (CAM->targetDistance + CAM->targetDistance < CAM->distance) {
                if ((CAM->contPad->button & (L_CBUTTONS | R_CBUTTONS)) && (D_800BE616 == 0)) {
                    CAM->contPad->button &= ~(L_CBUTTONS | R_CBUTTONS);
                    CAM->buttonsPressed &= ~(L_CBUTTONS | R_CBUTTONS);
                }
            }
            CAM->targetDistance = savedDistance;
        }
    }
}

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

/* Moves the focus point toward the player (or toward the midpoint with pairObj) at followRate, then
   adds the sideways offset. */
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

/* Height the camera target follows (player height scaled per state, plus status offsets). Stores it
   in *outY when outY is given; otherwise eases target.y toward it. */
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
            s16 discard1;
            s16 discard2;

            func_1507C3E0(CAM->player, &top, &discard1, &discard2);
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

/* Sets unk24C / unk250 (called after a preset change). */
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

/* Camera sway: while armed (mode 0x80000, or status bit 8 outside mode 0x40000) swayPhase advances by
   a random 0/2/4 degrees per frame, wrapping at 2 pi; otherwise it decays. The resulting swayAmount
   orbits eye and target around each other (func_1508EF80). */
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
        // matching: the position fields go through their own symbols (SCRIPT_CAM.unk14.. moves code)
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

/* Zones 3 and 0x1A (and 0x1A in level 0x42): plays the zone effect when unk7CC runs out and forces
   the R-trigger camera on; leaving the zone stops the effect. */
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

/* Aim eye position for `obj`: a point fixed to one of its bone matrices for the zones that have one,
   else the object position raised to 3/4 of its height. */
void func_1512623C(struct127 *obj, CamPlayerExt *ext, s32 zone, f32 *outX, f32 *outY, f32 *outZ, s32 lowView) {
    s32 useObjectPos;
    f32 localPos[3];
    f32 worldPos[3];
    s32 bone;
    Mtx *boneMtx;
    f32 offsetY;

    useObjectPos = 1;
    if (obj->unk1D4 != NULL) {
        useObjectPos = 0;
        if (lowView != 0) {
            bone = 3;
            offsetY = 20.0f;
            localPos[0] = 0.0f;
            localPos[1] = offsetY;
            localPos[2] = 0.0f;
        } else if (zone == 0x1B) {
            bone = 4;
            offsetY = 116.0f;
            localPos[1] = offsetY;
            localPos[0] = 0.0f;
            localPos[2] = 130.0f;
        } else {
            useObjectPos = 1;
        }

        if (useObjectPos == 0) {
            boneMtx = (Mtx *)obj->unk1D4;
            boneMtx += bone;
            func_15143134(localPos, worldPos, boneMtx);
            *outX = worldPos[0];
            *outY = worldPos[1];
            *outZ = worldPos[2];
        }
    }

    if (useObjectPos != 0) {
        *outX = obj->x_position;
        *outY = obj->y_position + (ext->unk114 * 0.75f);
        *outZ = obj->z_position;
    }
}

/* First-person aiming camera (R trigger in the aim zones): sets it up on the first frame, then steers
   yaw/pitch with the stick and zoom with C-up/C-down. Returns 1 while aiming. */
s32 func_15126378(struct108 *camera) {
    CamPlayerExt *ext;
    struct127 *viewer;
    f32 zoomRatio;
    f32 aspect;
    f32 maxZoom;
    f32 zoomStep;
    s32 zone;
    f32 scale;
    f32 angle;
    f32 t;
    f32 yawRad;
    f32 minRange;
    f32 aimKey;
    s32 buttons;
    CamVec3f dir;
    f32 rotated;
    f32 maxRange;
    f32 yawDelta;
    f32 pitchDelta;
    s32 valid;
    s32 state;

    scale = 0.02f;
    ext = CAM->playerExt;
    aspect = D_800BE628[CAM->playerIndex].fovY / D_800BE628[CAM->playerIndex].fovX;
    maxZoom = -(D_800BE628[CAM->playerIndex].fovX - 5.0f);
    zoomStep = D_800BE628[CAM->playerIndex].fovX * scale;
    zone = CAM->zone;
    if (((zone == 0xC) && (CAM->player->unk84.uh != 0x76)) || (zone == 0x1B)) {
        CAM->buttonsPressed |= R_TRIG;
        CAM->contPad->button |= R_TRIG;
    }
    if ((ext->aimState == 0) && ((CAM->contPad->stick_x != 0) || (CAM->contPad->stick_y != 0))) {
        ext->aimReset = 1;
    }
    // matching: [linked] - 1 rather than [linked - 1]
    viewer = (CAM->player->unk65 != 0) ? &D_800CC2D0[CAM->player->unk65] - 1 : CAM->player;
    valid = (ext->unk11A == 2) || (zone == 0x1B) || (zone == 0x1F) || (zone == 0x29);
    if (((zone == 0x23) || (zone == 0x24) || (zone == 0x16) || (zone == 0x18) || (zone == 0x41) ||
         (zone == 0xC) || (zone == 0x14) || (zone == 0x3F) || (zone == 0x1B) || (zone == 0x1F) ||
         (zone == 0x29)) &&
        (CAM->contPad->button & R_TRIG) && valid &&
        (((s32)CAM->player->unk28 == 0) || ((s32)viewer->unk28 == 0)) &&
        (CAM->player->stunned == 0) && (ext->aimState == 0)) {
        CAM->playerExt->unk198 = 0;
        CAM->unk73C = 0;
        if (((zone == 0x1F) || (zone == 0x29)) && !(CAM->status & 4)) {
            t = *(s16 *)&viewer->unk2E4 * scale;
            angle = (viewer->unk40 - 90.0f) - t;
            ext->aimReset = 1;
        } else {
            angle = CAM->player->unk40 - 90.0f;
        }
        func_15048758(&angle);
        yawRad = angle * DEG2RAD;
        func_151239CC(camera, 1);
        ext->aimState = 2;
        func_151C9BA0(camera, zone);
        func_1512623C(viewer, CAM->playerExt, zone, &CAM->eye.x, &CAM->eye.y, &CAM->eye.z, CAM->status & 0x10);
        CAM->target.x = (sinf(yawRad) * 100.0f) + CAM->eye.x;
        CAM->target.y = CAM->player->y_position + (CAM->playerExt->unk114 * 0.75f);
        CAM->target.z = (cosf(yawRad) * 100.0f) + CAM->eye.z;
        ext->aimEye = CAM->eye;
        ext->unk18C = CAM->yaw;
        ext->unk160 = CAM->eye;
        if (zone != ext->aimZone) {
            if (zone == 0x1B) {
                t = 1.0f;
            } else if (zone == 0x16) {
                t = 0.75f;
            } else {
                t = 0.5f;
            }
            if ((zone == 0x18) || (zone == 0x41) || (zone == 0x3F) || (zone == 0x14) || (zone == 0x12)) {
                t = (t < 0.0f) ? 0.0f : ((t > 0.6f) ? 0.6f : t);
            }
            ext->aimZoom = ((0.0f - maxZoom) * t) + maxZoom;
            ext->aimReset = 1;
            ext->aimZone = zone;
        }
        if (ext->aimReset != 0) {
            ext->aimYawTarget = angle;
            ext->aimPitchTarget = 0.0f;
            ext->aimPitch = 0.0f;
            ext->aimYawVel = 0.0f;
            ext->aimPitchVel = 0.0f;
            ext->aimReset = 0;
            ext->aimYaw = ext->aimYawTarget;
        }
        ext->aimZoomY = ext->aimZoom * aspect;
        CAM->unk19C = ext->aimZoom;
        CAM->unk1A0 = ext->aimZoomY;
        CAM->unk1A4 = ext->aimZoom;
        CAM->unk1A8 = ext->aimZoomY;
        func_1510B32C(CAM->playerIndex, CAM->unk19C, CAM->unk1A0, 1.0f);
        D_800BE628[CAM->playerIndex].perspScale = 1.0f;
        func_150627D4(CAM->player);
        D_800DBFF4[CAM->playerIndex] = 2;
        if ((zone == 0x24) || (zone == 0x23)) {
            func_1000FA64(0x605, 0, 0, 0, 0x6590, 300, 299, (s32)func_15127520, camera,
                          (u32)(-CAM->unk1A4 * 10000.0f), 0, 0);
        } else if ((zone == 0x1B) || (zone == 0x1F) || (zone == 0x29)) {
            aimKey = ext->aimYawTarget * ext->aimPitchTarget;
            func_1000FA64(0x622, 0, 0, 0, 0x6590, 300, 299, (s32)func_15127520, camera,
                          (u32)(-CAM->unk1A4 * 10000.0f) | 0x80000000, 0x100, 0);
            func_1000FA64(0x622, 0, 0, 0, 0x3E80, 300, 299, (s32)func_151277B0, camera, *(s32 *)&aimKey, 0x100, 0);
        }
        viewer->pad2FC[0] |= 1 << CAM->playerIndex;
        viewer->unk74 |= 1 << CAM->playerIndex;
        { typedef s32 LineMark; } // matching: -g3 line marker; as1 then schedules the aimState load after the a2 reload
    }
    state = ext->aimState;
    if (state == 0) {
        return 0;
    }
    if (state == 2) {
        buttons = CAM->contPad->button;
        func_1512623C(viewer, CAM->playerExt, zone, &CAM->eye.x, &CAM->eye.y, &CAM->eye.z, CAM->status & 0x10);
        zoomRatio = CAM->unk19C / maxZoom;
        zoomRatio = (zoomRatio < 0.0f) ? 0.0f : ((zoomRatio > 1.0f) ? 1.0f : zoomRatio);
        if (!(buttons & R_TRIG) || (zone == 0) || (ext->unk1AC != 0) ||
            ((CAM->player->health == 0) && (zone != 0x29)) ||
            ((CAM->player->stunned != 0) && (CAM->player->unk65 == 0) && ((s32)CAM->player->unk28 != 0) &&
             (zone != 0x1F) && (zone != 0x29))) {
            func_15127EB8(camera);
            return 0;
        }
        if (zone == 0x1B) {
            minRange = 19.0f;
            maxRange = 31.0f;
        } else {
            minRange = 25.0f;
            maxRange = 90.0f;
        }
        if (CAM->contPad->stick_x < 0) {
            valid = -1;
        } else {
            valid = 1;
        }
        t = 1.0f / (((maxRange - minRange) * zoomRatio) + minRange);
        yawDelta = valid * ((f32)CAM->contPad->stick_x * (f32)CAM->contPad->stick_x * (t * t));
        if (CAM->contPad->stick_y < 0) {
            valid = -1;
        } else {
            valid = 1;
        }
        pitchDelta = valid * ((f32)CAM->contPad->stick_y * (f32)CAM->contPad->stick_y * (t * t));
        if (zone == 0x1B) {
            yawDelta *= 0.25f;
            pitchDelta *= 0.5f;
            yawDelta = (yawDelta < -3.0f) ? -3.0f : ((yawDelta > 3.0f) ? 3.0f : yawDelta);
            pitchDelta = (pitchDelta < -3.0f) ? -3.0f : ((pitchDelta > 3.0f) ? 3.0f : pitchDelta);
        } else if (zone == 0xC) {
            yawDelta *= 1.5f;
            pitchDelta *= 1.5f;
        }
        ext->aimYawTarget -= yawDelta;
        ext->aimPitchTarget += pitchDelta;
        func_15048758(&ext->aimYawTarget);
        if ((zone == 0x1F) || (zone == 0x29)) {
            ext->aimPitchTarget = (ext->aimPitchTarget < -80.0f) ? -80.0f
                                : ((ext->aimPitchTarget > 30.0f) ? 30.0f : ext->aimPitchTarget);
        } else if (zone == 0x1B) {
            ext->aimPitchTarget = (ext->aimPitchTarget < -60.0f) ? -60.0f
                                : ((ext->aimPitchTarget > 50.0f) ? 50.0f : ext->aimPitchTarget);
        } else {
            ext->aimPitchTarget = (ext->aimPitchTarget < -80.0f) ? -80.0f
                                : ((ext->aimPitchTarget > 80.0f) ? 80.0f : ext->aimPitchTarget);
        }
        func_15049688(&ext->aimYaw, ext->aimYawTarget, &ext->aimYawVel, 6.0f, 8.0f, CAM->dt);
        func_15049688(&ext->aimPitch, ext->aimPitchTarget, &ext->aimPitchVel, 6.0f, 8.0f, CAM->dt);
        if ((zone != 0x16) && (zone != 0xC) && ((zone != 0x29) || (zone != 0x1F) || (D_800BE616 != 0)) &&
            ((buttons & (U_CBUTTONS | D_CBUTTONS)) != (U_CBUTTONS | D_CBUTTONS))) {
            if ((buttons & U_CBUTTONS) && (maxZoom < CAM->unk1A4) && (zone != 0xC)) {
                CAM->unk1A4 -= zoomStep;
                CAM->unk1A8 -= zoomStep * aspect;
            } else if ((buttons & D_CBUTTONS) && (CAM->unk1A4 < 0.0f)) {
                CAM->unk1A4 += zoomStep;
                CAM->unk1A8 += zoomStep * aspect;
            }
        }
        if (zone == 0x24) {
            CAM->unk1A4 = (CAM->unk1A4 < maxZoom) ? maxZoom : ((CAM->unk1A4 > 0.0f) ? 0.0f : CAM->unk1A4);
            CAM->unk1A8 = (CAM->unk1A8 < maxZoom * aspect) ? maxZoom * aspect
                        : ((CAM->unk1A8 > 0.0f) ? 0.0f : CAM->unk1A8);
        } else if (zone == 0x1B) {
            t = maxZoom * 0.5f;
            CAM->unk1A4 = (CAM->unk1A4 < t) ? t : ((CAM->unk1A4 > 0.0f) ? 0.0f : CAM->unk1A4);
            CAM->unk1A8 = (CAM->unk1A8 < t * aspect) ? t * aspect : ((CAM->unk1A8 > 0.0f) ? 0.0f : CAM->unk1A8);
        } else {
            t = maxZoom * 0.75f;
            CAM->unk1A4 = (CAM->unk1A4 < t) ? t : ((CAM->unk1A4 > 0.0f) ? 0.0f : CAM->unk1A4);
            CAM->unk1A8 = (CAM->unk1A8 < t * aspect) ? t * aspect : ((CAM->unk1A8 > 0.0f) ? 0.0f : CAM->unk1A8);
        }
        CAM->unk19C = CAM->unk1A4;
        CAM->unk1A0 = CAM->unk1A8;
        ext->aimZoom = CAM->unk1A4;
        ext->aimZoomY = CAM->unk1A8;
        dir.x = 1.0f;
        rotated = cosf(ext->aimPitch * -DEG2RAD) * dir.x;
        dir.y = sinf(ext->aimPitch * -DEG2RAD) * dir.x;
        dir.x = rotated;
        rotated = cosf(ext->aimYaw * -DEG2RAD) * dir.x;
        dir.z = sinf(ext->aimYaw * -DEG2RAD) * dir.x;
        dir.x = rotated;
        CAM->target.x = CAM->eye.x + dir.x;
        CAM->target.y = CAM->eye.y + dir.y;
        CAM->target.z = CAM->eye.z + dir.z;
        CAM->focus.x = CAM->player->x_position;
        CAM->focus.y = CAM->player->y_position;
        CAM->focus.z = CAM->player->z_position;
        ext->aimTarget = CAM->target;
        ext->aimEye = CAM->eye;
        ext->aimDir = dir;
        func_15048F90((struct17 *)&ext->aimEye, (struct17 *)&ext->aimTarget, (struct17 *)&ext->aimDir);
        D_800BE628[CAM->playerIndex].perspScale = (-0.94f * zoomRatio) + 1.0f;
        func_1510B128(CAM->playerIndex, CAM->unk19C, CAM->unk1A0, D_800BE628[CAM->playerIndex].perspScale,
                      (71.0f * zoomRatio) + 54.0f);
        return 1;
    }
    // matching: no return for other aim states, as in the original
}

/* Sound callback while aiming (zoom): follows the player and switches between the D_800894E0[index]
   sounds when the zoom changes or stops. Returns 1 when the aim ended. */
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
/* Sound callback while aiming (look): plays sound 0x61C while the aim angles keep changing, 0x61D
   once they settle. Returns 1 when the aim ended. */
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

/* Aiming view for an object without a player camera (stick-steered yaw/pitch around its head).
   Returns 1 while aiming. */
s32 func_151279A0(struct127 *obj, OSContPad *contPad) {
    CamPlayerExt *ext;
    s32 zone;
    CamVec3f pos;
    CamVec3f base;
    f32 dt;
    CamVec3f dir;
    f32 rotated;
    f32 pad; // matching: unused stack slot in the frame
    s32 buttons;
    f32 angle;

    ext = (CamPlayerExt *)obj->unk31C;
    zone = ext->cameraZone;
    dt = D_800BE9E4 * (1.0f / 60.0f);
    if ((zone == 0xC) || (zone == 0x1B)) {
        contPad->button |= R_TRIG;
    }
    if (((zone == 0x23) || (zone == 0x24) || (zone == 0x16) || (zone == 0x12) || (zone == 0x18) ||
         (zone == 0x41) || (zone == 0xC) || (zone == 0x14) || (zone == 0x3F) || (zone == 0x1B) ||
         (zone == 0x1F) || (zone == 0x29)) &&
        (contPad->button & R_TRIG) && (ext->aimState == 0)) {
        ext->aimState = 2;
        ext->aimEye = base;
        ext->aimYawTarget = obj->unk40 - 90.0f;
        ext->aimPitchTarget = 0.0f;
        ext->aimPitch = 0.0f;
        ext->aimYawVel = 0.0f;
        ext->aimPitchVel = 0.0f;
        ext->aimYaw = ext->aimYawTarget;
        func_150627D4(obj);
    }
    if (ext->aimState == 0) {
        return 0;
    }
    if (ext->aimState == 2) {
        buttons = contPad->button;
        angle = (obj->unk40 - 90.0f) * DEG2RAD;
        pos.x = obj->x_position;
        pos.y = obj->y_position + (ext->unk114 * 0.75f);
        pos.z = obj->z_position;
        base = pos;
        pos.x = (sinf(angle) * 100.0f) + base.x;
        pos.z = (cosf(angle) * 100.0f) + base.z;
        base.y += obj->y_position - obj->old_y_position;
        if (!(buttons & R_TRIG) || (zone == 0)) {
            ext->aimState = 0;
            func_150627D4(obj);
            return 0;
        }
        ext->aimYawTarget -= contPad->stick_x * 0.025f;
        ext->aimPitchTarget += contPad->stick_y * 0.025f;
        func_15048758(&ext->aimYawTarget);
        if ((zone == 0x1F) || (zone == 0x29)) {
            ext->aimPitchTarget = (ext->aimPitchTarget < -20.0f) ? -20.0f
                                : ((ext->aimPitchTarget > 80.0f) ? 80.0f : ext->aimPitchTarget);
        } else {
            ext->aimPitchTarget = (ext->aimPitchTarget < -80.0f) ? -80.0f
                                : ((ext->aimPitchTarget > 80.0f) ? 80.0f : ext->aimPitchTarget);
        }
        func_15049688(&ext->aimYaw, ext->aimYawTarget, &ext->aimYawVel, 6.0f, 8.0f, dt);
        func_15049688(&ext->aimPitch, ext->aimPitchTarget, &ext->aimPitchVel, 6.0f, 8.0f, dt);
        dir.x = 1.0f;
        rotated = cosf(ext->aimPitch * -DEG2RAD) * dir.x;
        dir.y = sinf(ext->aimPitch * -DEG2RAD) * dir.x;
        dir.x = rotated;
        rotated = cosf(ext->aimYaw * -DEG2RAD) * dir.x;
        dir.z = sinf(ext->aimYaw * -DEG2RAD) * dir.x;
        dir.x = rotated;
        pos.x = base.x + dir.x;
        pos.y = base.y + dir.y;
        pos.z = base.z + dir.z;
        ext->aimTarget = pos;
        ext->aimEye = base;
        ext->aimDir = dir;
        return 1;
    }
    // matching: no return for other aim states, as in the original
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
    D_800BE628[CAM->playerIndex].perspScale = 1.0f;
    func_150627D4(CAM->player);
    D_800DBFF4[CAM->playerIndex] = 2;

    player = CAM->player;
    linked = player->unk65;
    // matching: [linked] - 1 rather than [linked - 1] (the latter changes the code)
    target = (linked != 0) ? &D_800CC2D0[linked] - 1 : player;

    target->pad2FC[0] &= ~(1 << CAM->playerIndex);
    target->unk74 &= ~(1 << CAM->playerIndex);
    CAM->snap = 1;
}

/* Starts a transition (func_15128030) from the current focus point. */
void func_15127FEC(struct108 *camera, s32 arg1, s32 arg2) {
    CAM->transition.state = 1;
    CAM->transition.startTarget.x = CAM->focus.x;
    CAM->transition.startTarget.y = CAM->focus.y;
    CAM->transition.startTarget.z = CAM->focus.z;
    func_1512A390();
}

/* Runs the camera transition started by func_15127FEC: eases target, yaw, distance (D_800DC000) and
   height (D_800DC004) from their start values to the follow camera's over `duration` frames.
   Returns 1 while it runs. */
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
