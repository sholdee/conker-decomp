#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* File-local view of struct108 (the per-player camera, layout and names as in game_14FF90.c's Camera):
   only the members used here. include/structs.h has pads over several of them. */
typedef struct Camera {
    /* 0x000 */ u8 pad0[0x84];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0x54];
    /* 0x0DC */ s32 unkDC;
    /* 0x0E0 */ u8 padE0[0x15C];
    /* 0x23C */ u8 snap;                    /* nonzero: jump to targets instead of easing */
    /* 0x23D */ u8 pad23D[0x5B];
    /* 0x298 */ s16 playerMoved;
    /* 0x29A */ u8 pad29A[0x22];
    /* 0x2BC */ vertex target;              /* look-at point */
    /* 0x2C8 */ vertex prevTarget;
    /* 0x2D4 */ u8 pad2D4[0x24];
    /* 0x2F8 */ vertex eye;                 /* camera position */
    /* 0x304 */ u8 pad304[0x40];
    /* 0x344 */ f32 unk344;
    /* 0x348 */ f32 height;
    /* 0x34C */ u8 pad34C[0x8];
    /* 0x354 */ f32 unk354;
    /* 0x358 */ u8 pad358[0x4];
    /* 0x35C */ f32 unk35C;
    /* 0x360 */ u8 pad360[0xA];
    /* 0x36A */ u16 buttonsPressed;
    /* 0x36C */ OSContPad *contPad;
    /* 0x370 */ f32 distance;
    /* 0x374 */ f32 targetDistance;
    /* 0x378 */ u8 pad378[0x4];
    /* 0x37C */ f32 yaw;                    /* degrees */
    /* 0x380 */ u8 pad380[0x10];
    /* 0x390 */ f32 unk390;
    /* 0x394 */ u8 pad394[0x8];
    /* 0x39C */ f32 yawRad;
    /* 0x3A0 */ u8 pad3A0[0x20];
    /* 0x3C0 */ vertex eyeVel;              /* easing velocity of eye */
    /* 0x3CC */ u8 pad3CC[0x4];
    /* 0x3D0 */ struct127 *player;
    /* 0x3D4 */ u8 pad3D4[0x8];
    /* 0x3DC */ f32 unk3DC;                 /* orbit yaw of the eye, radians */
    /* 0x3E0 */ u8 pad3E0[0x8];
    /* 0x3E8 */ u8 unk3E8;
    /* 0x3E9 */ u8 pad3E9[0x1FF];
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ f32 swayAmount;
    /* 0x5F0 */ s32 status;
    /* 0x5F4 */ u8 pad5F4[0x1C0];
    /* 0x7B4 */ f32 dt;                     /* frame time scale */
    /* 0x7B8 */ u8 pad7B8[0xB4];
    /* 0x86C */ f32 pitchOverride;          /* degrees, 999.0f = none */
    /* 0x870 */ u8 pad870[0x64];
    /* 0x8D4 */ f32 yawFollowTarget;
    /* 0x8D8 */ f32 yawFollowVel;
    /* 0x8DC */ f32 yawFollow;              /* how fast yaw turns toward the target */
} Camera;

extern f32 D_800A34A0;
extern f32 D_800894C0;
extern struct17 D_800894C8;

void func_150495B0(f32 *value, f32 target, f32 *vel, f32 accel, f32 maxSpeed, f32 dt);
void func_15123070(struct108 *arg0);
void func_15123A54(Camera *camera);
void func_1512E140(Camera *camera);
void func_1512A390(struct108 *arg0);

#define SQ(x) ((x) * (x))
#define FLD(o) (*(f32 *)((u8 *)arg0 + (o)))

void func_15122440(struct108 *arg0) {
    struct17 v = D_800894C8;
    f32 tmp;
    f32 yaw;
    f32 pitch;
    f32 s;
    f32 x;
    f32 z;
    f32 y;

    yaw = (arg0->unk3D0->unk40 + 90.0f) * 0.01745329238f;
    func_1512A390(arg0);
    if (!(arg0->unk5F0 & 1) && (arg0->unk3D0->unk102 == 0)) {
        yaw = arg0->unk39C - 1.570796371f;
    }
    if (arg0->unk86C != 999.0f) {
        pitch = arg0->unk86C;
    } else if (arg0->unk3D0->health == 0) {
        pitch = 0.0f;
    } else {
        pitch = (arg0->unk3D0->unk102 ? 10.0f : 0.0f) + arg0->unk3D0->unkB8;
    }
    if (arg0->unk3D0->unk102) {
        pitch *= 0.5f;
    }
    pitch = (pitch < -88.0f) ? -88.0f : ((pitch > 88.0f) ? 88.0f : pitch);
    if (fabsf(D_800894C0 - pitch) < 10.0f) {
        pitch = D_800894C0;
    } else {
        D_800894C0 = pitch;
    }
    pitch *= 0.01745329238f;
    func_15123070(arg0);
    if (*arg0->unk36C & 0x10) {
        v.unk0 = 150.0f;
    } else {
        v.unk0 = arg0->unk374;
    }
    if ((arg0->unk3D0->unk102 != 0) && (arg0->unk3E8 == 0)) {
        if (FLD(0x370) < arg0->unk374) {
            v.unk0 = FLD(0x370);
        }
    }
    tmp = cosf(pitch) * v.unk0;
    v.unk4 = sinf(pitch) * v.unk0;
    v.unk0 = tmp;
lbl:
    {
    f32 c;
    s = sinf(yaw);
    c = cosf(yaw);
    FLD(0x3DC) = yaw;
    do {
    v.unk8 = -v.unk0 * s;
    v.unk0 = v.unk0 * c;
    v.unk4 = (v.unk4 < -200.0f) ? -200.0f : ((v.unk4 > 200.0f) ? 200.0f : v.unk4);
    } while (0);
    v.unk0 += arg0->unk2BC;
    v.unk4 += arg0->unk2C0;
    v.unk8 += arg0->unk2C4;
    }
    {
    f32 accel = 1.0f;
    f32 maxv = 2.200000048f;
    x = v.unk0;
    y = v.unk4;
    z = v.unk8;
    if (arg0->unk23C) {
        arg0->unk2F8 = x;
        arg0->unk2FC = y;
        arg0->unk300 = z;
        FLD(0x3C0) = 0.0f;
        FLD(0x3C4) = 0.0f;
        FLD(0x3C8) = 0.0f;
    } else {
        accel += accel;
        maxv += maxv;
        if (arg0->unk3D0->unk102) {
            accel += accel;
            maxv += maxv;
        }
        func_150495B0(&arg0->unk2F8, x, &FLD(0x3C0), accel, maxv, arg0->unk7B4);
        func_150495B0(&arg0->unk2FC, y, &FLD(0x3C4), accel, maxv, arg0->unk7B4);
        func_150495B0(&arg0->unk300, z, &FLD(0x3C8), accel, maxv, arg0->unk7B4);
    }
    }
    if ((arg0->unk3D0->unk102 == 0) && !(arg0->unk36A & 0x10)) {
        if (sqrtf(SQ(arg0->unk2F8 - arg0->unk2BC) + SQ(arg0->unk2FC - arg0->unk2C0) + SQ(arg0->unk300 - arg0->unk2C4)) < 120.0f) {
            struct17 dir;
            func_150491EC((struct17 *)&arg0->unk2BC, (struct17 *)&arg0->unk2F8, &dir);
            arg0->unk2F8 = arg0->unk2BC + 120.0f * dir.unk0;
            arg0->unk2FC = arg0->unk2C0 + 120.0f * dir.unk4;
            arg0->unk300 = arg0->unk2C4 + 120.0f * dir.unk8;
        }
    }
    if (arg0->unk2FC < arg0->unk35C + 30.0f) {
        arg0->unk2FC = arg0->unk35C + 30.0f;
    }
    arg0->unk5E8 += (FLD(0x5EC) - arg0->unk5E8) * 0.150000006f;
    FLD(0x370) = sqrtf(SQ(arg0->unk2F8 - arg0->unk2BC) + SQ(arg0->unk300 - arg0->unk2C4));
    arg0->unk348 = FLD(0x344) = arg0->unk2FC - arg0->unk354;
}

/* Eases yawFollow toward a gain picked from the camera state and turns yaw by it toward the target's
   motion, then runs the per-frame camera updates. */
void func_15122980(Camera *camera) {
    vertex toPrevTarget;
    vertex toTarget;
    f32 prevAngle;
    f32 angle;

    if (camera->unkDC != 4) {
        func_15048F90((struct17 *)&camera->eye, (struct17 *)&camera->prevTarget, (struct17 *)&toPrevTarget);
        func_15048F90((struct17 *)&camera->eye, (struct17 *)&camera->target, (struct17 *)&toTarget);
        prevAngle = func_15048FC8((struct17 *)&toPrevTarget);
        angle = func_15048FC8((struct17 *)&toTarget);
        if (camera->playerMoved == 0) {
            camera->yawFollowTarget = 0.0f;
        } else if ((camera->unk84 & 0x100000) || ((camera->unk390 > 180.0f) && (camera->unk390 < 270.0f))) {
            camera->yawFollowTarget = 0.75f;
        } else {
            camera->yawFollowTarget = 0.5f;
        }
        func_150495B0(&camera->yawFollow, camera->yawFollowTarget, &camera->yawFollowVel, 1.0f, 2.0f, camera->dt);
        camera->yaw += func_15048A70(prevAngle, angle) * camera->yawFollow;
        camera->yawRad = camera->yaw * 0.01745329238f;
    }

    func_15123A54(camera);
    func_1512E140(camera);
    func_1512A390((struct108 *)camera);
}
