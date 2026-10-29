#include <ultra64.h>
#define func_1513D524 func_1513D524_void
#include "functions.h"
#undef func_1513D524
#include "variables.h"

s32 func_1513D524(s32 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);

extern f32 D_800A0710;
extern f32 D_800A0714;
extern f32 D_800A0718;
extern f32 D_800A071C;
extern f32 D_800A0720;
extern f32 D_800A0724;
extern f32 D_800A0728;
extern f32 D_800A072C;
extern f32 D_800A0730;
extern f32 D_800A0734;
extern f32 D_800A0738;
extern f32 D_800A073C;
extern f32 D_800A0740;
extern f32 D_800A0744;
extern f32 D_800A0748;

struct Vec3I150CCEB0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Pair150CCEB0 {
    s32 unk0;
    s32 unk4;
};

struct Arg150CCEB0 {
    struct Vec3I150CCEB0 unk00;
    struct Vec3I150CCEB0 unk0C;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    s32 unk24;
    struct Pair150CCEB0 unk28;
    f32 unk30;
};

struct Main150CCEB0 {
    u8 unk00;
    u8 pad01;
    s16 unk02;
    s16 unk04;
    u8 pad06[2];
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    struct Pair150CCEB0 unk14;
    struct Vec3I150CCEB0 unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 pad48[0x10];
};

struct Extra150CCEB0 {
    struct Vec3I150CCEB0 unk00;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[4];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
};

s32 func_150CCEB0(struct Arg150CCEB0 *arg0, u8 arg1, u8 arg2) {
    s32 ret;
    struct Main150CCEB0 main;
    struct Extra150CCEB0 extra;

    main.unk00 = 0x57;
    main.unk02 = 0x2D03;
    main.unk04 = arg0->unk24;
    main.unk08 = 0;
    main.unk0C = 0;
    main.unk10 = arg0->unk20;
    main.unk11 = arg0->unk21;
    main.unk12 = arg0->unk22;
    main.unk13 = 0xFF;
    main.unk14 = arg0->unk28;
    main.unk1C = arg0->unk00;
    main.unk28 = func_150ADA68() * 360.0f;
    main.unk2C = func_150ADA68() * 360.0f;
    main.unk30 = func_150ADA68() * 360.0f;
    main.unk40 = 0xC03E1;
    main.unk34 = 0.0f;
    main.unk38 = 0.0f;
    main.unk3C = 0.0f;
    main.unk44 = arg0->unk23;
    main.unk45 = 0xFF;
    main.unk46 = 0;
    main.unk47 = 6;

    extra.unk00 = arg0->unk0C;
    extra.unk3C = arg0->unk30;
    extra.unk10 = arg0->unk18;
    extra.unk0C = arg0->unk1C;
    extra.unk14 = ((func_150ADA68() * D_800A0710) + D_800A0714) * D_800A0718;
    extra.unk18 = ((func_150ADA68() * D_800A071C) + D_800A0720) * D_800A0724;
    extra.unk1C = ((func_150ADA68() * D_800A0728) + D_800A072C) * D_800A0730;
    extra.unk24 = 0.0f;
    extra.unk28 = ((func_150ADA68() * 206.0f) + 58.0f) * D_800A0734;
    extra.unk2C = func_150ADA68() * 360.0f;
    extra.unk30 = 0.0f;
    extra.unk34 = (func_150ADA68() * D_800A0738) * D_800A073C;
    extra.unk38 = ((func_150ADA68() * D_800A0740) + D_800A0744) * D_800A0748;

    ret = func_1513D524((s32)&main, 0x13, 0x1E, 0, 0x11, 0, 0x40, arg1, arg2);

    if (ret != 0) {
        memcpy((void *)(ret + 0x110), &extra, 0x40);
    }

    return ret;
}

typedef struct Drift150CD17C {
    f32 vx;
    f32 vy;
    f32 vz;
    f32 gravity;
    f32 drag;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    u8 pad20[4];
    f32 swayAngle;
    f32 swaySpeed;
    f32 swayRadius;
    f32 phase;
    f32 phaseSpeed;
    f32 phaseScale;
    f32 minVy;
} Drift150CD17C;

typedef struct Drifter150CD17C {
    u8 pad0[0x34];
    f32 x;
    f32 y;
    f32 z;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    f32 sway[3];
    u8 pad58[0xB8];
    Drift150CD17C drift;
} Drifter150CD17C;

void func_151436B4(f32, f32, f32, f32 *);

s32 func_150CD17C(Drifter150CD17C *arg0) {
    Drift150CD17C *drift;
    s32 i;
    f32 wave;
    f32 angle;
    f32 period;

    drift = &arg0->drift;
    arg0->x += drift->vx * D_800BE9A4;
    arg0->y += drift->vy * D_800BE9A4 + 0.5f * drift->gravity * D_800BE9A4 * D_800BE9A4;
    arg0->z += drift->vz * D_800BE9A4;
    drift->vy += drift->gravity * D_800BE9A4;
    for (i = D_800BE9E4; i != 0; i--) {
        drift->vx *= drift->drag;
        drift->vz *= drift->drag;
    }
    if (drift->vy > 0.0f) {
        for (i = D_800BE9E4; i != 0; i--) {
            drift->vy *= drift->drag;
        }
    }
    if (drift->vy < drift->minVy) {
        drift->vy = drift->minVy;
    }
    if (drift->vy < 0.0f) {
        wave = sinf(drift->phase);
        angle = drift->phaseScale * wave + 1.5707964f;
        func_151436B4(drift->swayRadius, angle, drift->swayAngle, arg0->sway);
        period = 6.2831855f;
        drift->swayAngle += drift->swaySpeed * D_800BE9A4;
        drift->phase += drift->phaseSpeed * D_800BE9A4;
        while (drift->phase > period) {
            drift->phase -= period;
        }
        while (drift->phase < 0.0f) {
            drift->phase += period;
        }
    }
    arg0->offsetX += drift->offsetX;
    arg0->offsetY += drift->offsetY;
    arg0->offsetZ += drift->offsetZ;
    return 1;
}

typedef struct Quad150CD59C {
    u8 pad0[0x2C];
    f32 halfW;
    f32 halfH;
    f32 x;
    f32 y;
    f32 z;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    f32 offX;
    f32 offY;
    f32 offZ;
    u8 pad58[0x68];
    u8 template[0x40];
    u8 *buffers[4];
    u8 pad110[4];
    f32 unk114;
} Quad150CD59C;

void func_151D5D60(void *, s16, s32, void *, u8 *);
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

void *func_150CD59C(Quad150CD59C *arg0, s16 arg1) {
    f32 *v;
    Vtx *vtx;
    void *ret;
    f32 mtx[4][4];
    f32 verts[4][3];
    u8 i;
    u8 fresh;

    func_151D5D60(arg0->buffers, arg1, 0x40, &vtx, &fresh);
    ret = vtx;
    if (vtx != 0) {
        if (fresh != 0) {
            memcpy(arg0->buffers[arg1], arg0->template, 0x40);
            memcpy(arg0->buffers[arg1] + 0x40, arg0->template, 0x40);
        }
    } else {
        return 0;
    }
    {
        u8 i;

        verts[0][0] = arg0->halfW;
        verts[0][1] = arg0->halfH;
        verts[0][2] = 0.0f;
        verts[1][0] = -arg0->halfW;
        verts[1][1] = arg0->halfH;
        verts[1][2] = 0.0f;
        verts[2][0] = -arg0->halfW;
        verts[2][1] = -arg0->halfH;
        verts[2][2] = 0.0f;
        verts[3][0] = arg0->halfW;
        verts[3][1] = -arg0->halfH;
        verts[3][2] = 0.0f;
        func_150A8050(mtx, arg0->rotX, arg0->rotY, arg0->rotZ);
        mtx[3][0] = arg0->x;
        mtx[3][1] = arg0->y;
        mtx[3][2] = arg0->z;
        if (arg0->unk114 < 0.0f) {
            mtx[3][0] += arg0->offX;
            mtx[3][1] += arg0->offY;
            mtx[3][2] += arg0->offZ;
        }
        for (i = 0; i < 4; i++) {
            Vtx *p;
            v = verts[i];
            func_150A7960((f32 *)mtx, v[0], v[1], 0.0f, &v[0], &v[1], &v[2]);
            vtx->v.ob[0] = v[0];
            vtx->v.ob[1] = v[1];
            vtx->v.ob[2] = v[2];
            vtx->v.flag = 0;
            p = vtx + 1;
            vtx = p;
        }
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_FA360/func_150CD7F8.s")

struct Inner150CDB6C {
    s32 unk0;
    f32 unk4;
};
extern u8* D_80088870;
extern f32 D_800A07A4;

void func_150CDB6C(s32 arg0) {
    struct Inner150CDB6C* q;
    if (arg0 >= 0 && arg0 < 0x100) {
        if (D_80088870 != NULL) {
            q = (struct Inner150CDB6C*)(D_80088870 + 0x28);
            q->unk4 = (f32)arg0 * D_800A07A4;
        }
    }
}
