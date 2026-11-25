#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_151539B4(void *, u8);
extern void func_15130374(s32, u8, s32, u8, s32);

/*
 * A streak particle: a camera-facing triangle from its tail point to a
 * width-wide edge at its head. Head and tail move with separate velocities.
 */
typedef struct {
    /* 0x00 */ u8 header[0x10];
    /* 0x10 */ vertex headPos;
    /* 0x1C */ vertex headVel;
    /* 0x28 */ vertex tailPos;
    /* 0x34 */ vertex tailVel;
    /* 0x40 */ u8 flags;            // 1: lifetime counts down, 2/4: render flags, 8: fades out
    /* 0x42 */ s16 lifetime;
    /* 0x44 */ u16 renderMode;      // low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg
    /* 0x48 */ f32 width;
    /* 0x4C */ u8 alpha;
    /* 0x4E */ s16 fadeStart;       // fade begins when lifetime drops below this
    /* 0x50 */ s16 fadeRate;        // alpha = lifetime * fadeRate while fading
    /* 0x52 */ u8 pad52[0x6];
    /* 0x58 */ Vtx vtxTemplate[3];  // tail, head + side, head - side
    /* 0x88 */ Vtx *vtxBuffers[4];  // per-view vertex buffers (two frames each)
} StreakParticle;

/* Spawn parameters for a streak particle. */
typedef struct {
    /* 0x00 */ vertex pos;
    /* 0x0C */ vertex dir;
    /* 0x18 */ f32 headSpeed;
    /* 0x1C */ f32 tailSpeed;
    /* 0x20 */ u8 headColor[4];
    /* 0x24 */ u8 tailColor[4];
    /* 0x28 */ u8 flags;
    /* 0x2A */ s16 lifetime;
    /* 0x2C */ u16 renderMode;
    /* 0x30 */ f32 width;
    /* 0x34 */ u8 alpha;
    /* 0x36 */ s16 fadeStart;
    /* 0x38 */ s16 fadeRate;
} StreakParticleDesc;

/* Allocates a streak particle (kind 0 or 1) with extraSize trailing bytes and fills it from desc. */
StreakParticle *func_15156190(StreakParticleDesc *desc, u8 kind, s32 extraSize, s32 arg3, s32 arg4) {
    StreakParticle *newParticle;
    StreakParticle *particle;
    s32 id;
    vertex pos;

    switch (kind) {
        case 0:
            id = 0x2C;
            break;
        case 1:
            id = 0x53;
            break;
        default:
            id = 0x2C;
            break;
    }

    newParticle = func_15167A68(id, arg4, extraSize + sizeof(StreakParticle), 1, (u8)arg3, 1);
    if (newParticle == NULL) {
        return NULL;
    }
    particle = newParticle; // matching: the second pointer local survives the call as v1
    particle->vtxTemplate[0].v.flag = 0;
    particle->vtxTemplate[0].v.cn[0] = desc->tailColor[0];
    particle->vtxTemplate[0].v.cn[1] = desc->tailColor[1];
    particle->vtxTemplate[0].v.cn[2] = desc->tailColor[2];
    particle->vtxTemplate[0].v.cn[3] = desc->tailColor[3];
    particle->vtxTemplate[0].v.tc[0] = 0;
    particle->vtxTemplate[0].v.tc[1] = 0;
    particle->vtxTemplate[1].v.flag = 0;
    particle->vtxTemplate[1].v.cn[0] = desc->headColor[0];
    particle->vtxTemplate[1].v.cn[1] = desc->headColor[1];
    do {} while (0); // matching: block boundary makes as1 keep the v1 copy of the pointer for the last stores
    particle->vtxTemplate[1].v.cn[2] = desc->headColor[2];
    particle->vtxTemplate[1].v.cn[3] = desc->headColor[3];
    particle->vtxTemplate[2].v.flag = 0;
    particle->vtxTemplate[2].v.cn[0] = desc->headColor[0];
    particle->vtxTemplate[2].v.cn[1] = desc->headColor[1];
    particle->vtxTemplate[2].v.cn[2] = desc->headColor[2];
    particle->vtxTemplate[2].v.cn[3] = desc->headColor[3];
    pos = desc->pos;
    particle->tailPos = pos;
    particle->headPos = pos;
    particle->headVel.x = desc->dir.x * desc->headSpeed;
    particle->headVel.y = desc->dir.y * desc->headSpeed;
    particle->headVel.z = desc->dir.z * desc->headSpeed;
    particle->tailVel.x = desc->dir.x * desc->tailSpeed;
    particle->tailVel.y = desc->dir.y * desc->tailSpeed;
    particle->tailVel.z = desc->dir.z * desc->tailSpeed;
    particle->flags = desc->flags;
    particle->lifetime = desc->lifetime;
    particle->renderMode = desc->renderMode;
    particle->width = desc->width;
    particle->alpha = desc->alpha;
    particle->fadeStart = desc->fadeStart;
    particle->fadeRate = desc->fadeRate;
    bzero(particle->vtxBuffers, sizeof(particle->vtxBuffers));
    return particle;
}

void func_15156388(StreakParticleDesc *desc, u8 kind, s32 extraSize) {
    func_15156190(desc, kind, extraSize, 0xFF, 0);
}

/* Per-frame update: count down the lifetime, move head and tail, fade out; frees the particle when expired. */
void func_151563B8(StreakParticle *arg0) {
    StreakParticle *particle;
    u8 expired;
    s16 fadedAlpha;

    // matching: copying the parameter to a local steers IDO's register choice
    particle = arg0;
    expired = FALSE;
    if (particle->flags & 1) {
        particle->lifetime -= D_800BE9E4;
        if (particle->lifetime < 0) {
            expired = TRUE;
        }
    }
    if (!expired) {
        particle->headPos.x += particle->headVel.x * D_800BE9A4;
        particle->headPos.y += particle->headVel.y * D_800BE9A4;
        particle->headPos.z += particle->headVel.z * D_800BE9A4;
        particle->tailPos.x += particle->tailVel.x * D_800BE9A4;
        particle->tailPos.y += particle->tailVel.y * D_800BE9A4;
        particle->tailPos.z += particle->tailVel.z * D_800BE9A4;
        if (particle->flags & 8) {
            if (particle->lifetime < particle->fadeStart) {
                fadedAlpha = particle->lifetime * particle->fadeRate;
                if (fadedAlpha < particle->alpha) {
                    particle->alpha = fadedAlpha;
                }
            }
        }
    }
    if (expired) {
        func_1516972C((struct102 *)particle);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
} RenderModeEntry;

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern RenderModeEntry D_800A4AC8[];
void func_151D5D60(void *, s16, s32, void **, u8 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

/* Draws a streak particle for one view as a single triangle facing that view's eye. */
Gfx *func_151564F8(Gfx *gfx, StreakParticle *particle, s16 viewIndex) {
    StreakParticle *p;
    vertex axis;
    vertex toEye;
    vertex side;
    vertex *eye;
    f32 lengthSquared;
    u8 update;
    s16 renderFlags;
    s32 geometryFlags;
    Vtx *vtx;
    u8 mode;
    u8 cycle;
    u8 fresh;
    f32 scale;

    // matching: the p alias orders IDO's spill slots (expression ids follow first use)
    p = particle;
    func_151D5D60(particle->vtxBuffers, viewIndex, sizeof(particle->vtxTemplate), (void **)&vtx, &fresh);
    if (fresh) {
        memcpy(particle->vtxBuffers[viewIndex], particle->vtxTemplate, sizeof(particle->vtxTemplate));
        memcpy(particle->vtxBuffers[viewIndex] + 3, particle->vtxTemplate, sizeof(particle->vtxTemplate));
    }
    eye = (vertex *)&D_800DBFF0[viewIndex].unk2F8;
    update = TRUE;
    renderFlags = particle->flags & 6;
    mode = particle->renderMode & 0xFF;
    cycle = (particle->renderMode & 0xFF00) >> 8;
    geometryFlags = 0;
    if (renderFlags & 2) {
        geometryFlags = 0x200;
    }
    if (renderFlags & 4) {
        geometryFlags |= 0x400;
    }
    gfx = func_15142B7C(gfx, geometryFlags | 1 | 4 | 0x200000, 0x1F0600);
    gfx = func_15142C10(gfx, 0, 0, 0, particle->alpha, &update);
    gfx = func_15142CF0(gfx, 0, 0, 0, 0, 0, 0, &update);
    gfx = func_1513F4E4(gfx, cycle, &update);
    gfx = func_15142FBC(gfx, 0x82CA0, D_800A4AC8[mode].unk0 | D_800A4AC8[mode].unk4, &update);
    axis.x = p->headPos.x - p->tailPos.x;
    axis.y = p->headPos.y - p->tailPos.y;
    axis.z = p->headPos.z - p->tailPos.z;
    toEye.x = p->headPos.x - eye->x;
    toEye.y = p->headPos.y - eye->y;
    toEye.z = p->headPos.z - eye->z;
    side.x = axis.y * toEye.z - toEye.y * axis.z;
    side.y = axis.z * toEye.x - toEye.z * axis.x;
    side.z = axis.x * toEye.y - toEye.x * axis.y;
    lengthSquared = side.x * side.x + side.y * side.y + side.z * side.z;
    if (lengthSquared == 0.0f) {
        side.x = 0.0f;
        side.y = 0.0f;
        side.z = 0.0f;
    } else {
        scale = p->width / sqrtf(lengthSquared);
        side.x *= scale;
        side.y *= scale;
        side.z *= scale;
    }
    vtx[0].v.ob[0] = p->tailPos.x;
    vtx[0].v.ob[1] = p->tailPos.y;
    vtx[0].v.ob[2] = p->tailPos.z;
    vtx[1].v.ob[0] = p->headPos.x + side.x;
    vtx[1].v.ob[1] = p->headPos.y + side.y;
    vtx[1].v.ob[2] = p->headPos.z + side.z;
    vtx[2].v.ob[0] = p->headPos.x - side.x;
    vtx[2].v.ob[1] = p->headPos.y - side.y;
    vtx[2].v.ob[2] = p->headPos.z - side.z;
    gSPVertex(gfx++, vtx, 3, 0);
    gSP1Triangle(gfx++, 0, 1, 2, 0);
    return gfx;
}

#define LOW_U8_151568F8(arg) (*(u8 *)((u8 *)&(arg) + 3))

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} struct151568F8_0C;

typedef struct {
    s32 unk0;
    s32 unk4;
} struct151568F8_08;

typedef struct {
    struct151568F8_0C unk0;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u16 unk36;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    s16 unk42;
    s16 unk44;
    u8 unk46;
    u8 unk47;
    u16 unk48;
    u8 pad4A[2];
    s32 unk4C;
    s32 unk50;
    s16 unk54;
    u8 pad56[2];
    s32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69[3];
    struct151568F8_08 unk6C;
    s16 unk74;
    s16 unk76;
    s16 unk78;
    u8 pad7A[2];
    f32 unk7C;
    s32 unk80;
    u8 unk84;
    u8 unk85;
    s8 unk86;
    s8 unk87;
    u8 unk88;
} struct151568F8_src;

typedef struct {
    s16 unk0;
    s16 unk2;
    struct151568F8_0C unk4;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u16 unk36;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    s16 unk42;
    s16 unk44;
    u8 unk46;
} struct151568F8_first;

typedef struct {
    s32 unk0;
    s32 unk4;
    u16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    struct151568F8_08 unk28;
    struct151568F8_0C unk30;
    u8 pad3C[0xC];
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    u8 pad64[0xC];
} struct151568F8_second;


void func_151568F8(struct151568F8_src *arg0, s32 arg1) {
    struct151568F8_first first;
    struct151568F8_second second;

    first.unk0 = arg0->unkC;
    first.unk2 = arg0->unkE;
    first.unk4 = arg0->unk0;
    first.unk10 = arg0->unk10;
    first.unk14 = arg0->unk14;
    first.unk12 = arg0->unk12;
    first.unk16 = arg0->unk16;
    first.unk18 = arg0->unk18;
    first.unk1C = arg0->unk1C;
    first.unk20 = arg0->unk20;
    first.unk24 = arg0->unk24;
    first.unk28 = arg0->unk28;
    first.unk29 = arg0->unk29;
    first.unk2A = arg0->unk2A;
    first.unk2B = arg0->unk2B;
    first.unk2C = arg0->unk2C;
    first.unk2D = arg0->unk2D;
    first.unk2E = arg0->unk2E;
    first.unk2F = arg0->unk2F;
    first.unk30 = arg0->unk30;
    first.unk32 = arg0->unk32;
    first.unk34 = arg0->unk34;
    first.unk36 = arg0->unk36;
    first.unk38 = arg0->unk38;
    first.unk3C = arg0->unk3C;
    first.unk40 = arg0->unk40;
    first.unk41 = arg0->unk41;
    first.unk42 = arg0->unk42;
    first.unk44 = arg0->unk44;
    first.unk46 = arg0->unk46;
    func_151539B4(&first, LOW_U8_151568F8(arg1));

    second.unk1D = arg0->unk47;
    second.unk8 = arg0->unk48;
    second.unk4 = arg0->unk50;
    second.unk0 = arg0->unk4C;
    second.unkC = arg0->unk58;
    second.unk10 = arg0->unk5C;
    second.unk14 = arg0->unk60;
    second.unk15 = arg0->unk61;
    second.unk16 = arg0->unk62;
    second.unk17 = arg0->unk63;
    second.unk18 = arg0->unk64;
    second.unk19 = arg0->unk65;
    second.unk1A = arg0->unk66;
    second.unk30 = arg0->unk0;
    second.unk58 = arg0->unk80;
    second.unk1B = arg0->unk67;
    second.unk1C = arg0->unk68;
    second.unk62 = arg0->unk86;
    second.unk63 = arg0->unk87;
    second.unk1E = arg0->unk74;
    second.unk20 = arg0->unk76;
    second.unk60 = arg0->unk84;
    second.unk61 = arg0->unk85;
    second.unk22 = arg0->unk78;
    second.unk24 = arg0->unk7C;
    second.unkA = arg0->unk54;
    second.unk28 = arg0->unk6C;
    second.unk48 = 0.0f;
    second.unk4C = 0.0f;
    second.unk50 = 0.0f;
    second.unk54 = 0.0f;
    func_15130374((s32)&second, arg0->unk88, 0, LOW_U8_151568F8(arg1), 0);
}

typedef struct {
    /* 0x00 */ s16 base;
    /* 0x02 */ s16 range;
    /* 0x04 */ s16 timer;
} struct15156B54_sub;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ struct15156B54_sub sub;
} struct15156B54;

extern f32 D_800DCA30[3][10][4];
void func_15156D24(void *arg0, u8 arg1);
extern struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);

void func_15156B54(struct15156B54 *arg0) {
    struct15156B54_sub *sub;
    u32 row;
    u32 col;
    Header header;
    Header2 header2;

    arg0->sub.timer -= D_800BE9E4;
    if (arg0->sub.timer < 0) {
        row = func_150ADA20() % 3U;
        col = func_150ADA20() % 10U;
        if ((D_800DCA30[row][col][0] != 0.0f) && (D_800DCA30[row][col][1] != 0.0f) && (D_800DCA30[row][col][2] != 0.0f)) {
            func_15156D24(D_800DCA30[row][col], arg0->unkC);
            header.unk0 = 3;
            header.unk1 = -1;
            header.unk2 = (func_150ADA20() % 11U) + 5;
            header.unk4 = 0;
            header2.unk0 = D_800DCA30[row][col][0];
            header2.unk4 = D_800DCA30[row][col][1];
            header2.unk8 = D_800DCA30[row][col][2];
            func_151602C0(&header, &header2, (func_150ADA20() % 156U) + 100, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg0->unkC, arg0->unk1);
        }
        sub = &arg0->sub;
        sub->timer = (func_150ADA20() % (u32)(sub->range + 1)) + sub->base;
    }
}

typedef struct {
    struct151568F8_0C unk0;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u16 unk36;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    s16 unk42;
    s16 unk44;
    u8 unk46;
    u8 unk47;
    u16 unk48;
    u8 pad4A[2];
    s32 unk4C;
    s32 unk50;
    s16 unk54;
    u8 pad56[2];
    s32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69[3];
    f32 unk6C;
    f32 unk70;
    s16 unk74;
    s16 unk76;
    s16 unk78;
    u8 pad7A[2];
    f32 unk7C;
    s32 unk80;
    u8 unk84;
    u8 unk85;
    s8 unk86;
    s8 unk87;
    u8 unk88;
    u8 unk89;
    u8 unk8A;
    u8 unk8B;
    u8 unk8C;
} struct15156D24;

extern f32 D_800A6040;
extern f32 D_800A6044;
extern f32 D_800A6048;
extern f32 D_800A604C;
extern f32 D_800A6050;

void func_15156D24(void *arg0, u8 arg1) {
    struct15156D24 s;

    memcpy(&s.unk0, arg0, 0xC);
    s.unkC = 0xA;
    s.unkE = 0x1E;
    s.unk12 = -0x3F;
    s.unk14 = 0xFF;
    s.unk16 = 0x80;
    s.unk8A = s.unk28 = s.unk64 = 0xFF;
    s.unk8B = s.unk29 = s.unk65 = 0xFF;
    s.unk2A = s.unk66 = 0xFF;
    s.unk8C = 0xFF;
    s.unk2B = 0xFF;
    s.unk2C = 0xFF;
    s.unk2D = 0xFF;
    s.unk2E = 0xFF;
    s.unk2F = 0xFF;
    s.unk30 = 9;
    s.unk10 = 0;
    s.unk32 = 5;
    s.unk34 = 0xA;
    s.unk36 = 0x1601;
    s.unk40 = 0x64;
    s.unk41 = 0x9B;
    s.unk42 = 4;
    s.unk44 = 0x3F;
    s.unk46 = 0;
    s.unk47 = 0x2B;
    s.unk48 = 0x1A01;
    s.unk4C = 0x200005;
    s.unk50 = 0;
    s.unk20 = s.unk24 = D_800A6040;
    s.unk18 = D_800A6044;
    s.unk1C = D_800A6048;
    s.unk38 = D_800A604C;
    s.unk3C = D_800A6050;
    s.unk54 = (func_150ADA20() % 3U) + 7;
    s.unk58 = 0;
    s.unk5C = 0;
    s.unk60 = 0xFF;
    s.unk61 = 0xFF;
    s.unk62 = 0xFF;
    s.unk63 = 0xFF;
    func_150ADA20();
    s.unk67 = 0xFF;
    s.unk68 = 0xFF;
    s.unk6C = s.unk70 = (func_150ADA68() * 150.0f) + 196.0f;
    s.unk74 = 4;
    s.unk76 = 0x3F;
    s.unk78 = 0x16;
    s.unk7C = 1.25f;
    s.unk80 = 0xE01;
    if (func_150ADA20() & 1) {
        s.unk80 |= 0x40;
    }
    if (func_150ADA20() & 1) {
        s.unk80 |= 0x80;
    }
    s.unk84 = 4;
    s.unk85 = 4;
    s.unk86 = -1;
    s.unk87 = -1;
    s.unk88 = 1;
    s.unk89 = 1;
    func_151568F8((struct151568F8_src *)&s, arg1);
}

extern void func_151D5E30(void *);

void func_15156F94(void *arg0) {
    func_151D5E30((void *)((u8 *)arg0 + 0x88));
}

void func_15156FB8(struct102 *arg0) {
    func_15156F94(arg0);
    func_15169804(arg0);
}

void func_15156FE4(struct102 *arg0) {
    func_15156F94(arg0);
    func_15169824(arg0);
}

