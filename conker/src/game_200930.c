#include <ultra64.h>
/* functions.h declares arg3/arg5 as s32; golden reads them as u8 from their home slots. */
#define func_151D5714 func_151D5714_hdr
#include "functions.h"
#undef func_151D5714
#include "variables.h"

extern void (*D_8008FC70[])(struct127 *, s16, struct17 *);
extern void *func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 allocate_memory(s32, s32, s32, s32);

void func_1503F404(void *, void *, void *, void *, void *, void *, void *, void *, void *, void *);
void *func_15132A4C(void *, s32, s32, s32, u8, s32);
void func_15131828(void *, void *, void *, void *);
void func_15131958(void *, f32, void *);
void func_151541B8(void *, f32, s32, f32, f32, u8, s32);
struct conker151D5514;
void func_1514FCE8(struct conker151D5514 *a0, u8 a1, s32 a2);

typedef struct {
    s16 v[9];
} Surf151D3480;

/* Spawn description passed to func_15132A4C. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 scaleXZ;
    /* 0x0C */ f32 scaleY;
    /* 0x10 */ struct17 rot;
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ struct17 pos;
    /* 0x34 */ struct17 vel;
    /* 0x40 */ struct17 unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerUniqueId;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 unk78;
} ParticleDesc;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ struct17 unk08;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} Spawn151D3480;

extern u8 D_800DCA20;
extern f32 D_800DCA24;
extern s32 func_1000EC24();
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, f32 *, f32 *, f32 *, void *, s32 *, s32, f32);
u8 func_15145C90(s32);
s32 func_10010E78(u16 arg0, s32 arg1, u16 arg2, s16 arg3, u8 arg4, s32 arg5,
                  s16 arg6, s16 arg7, s16 arg8, s16 arg9, s16 argA);
void func_15152B38(void *, u8, s32);
void func_15145974(void *, f32 *, f32 *);
s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
void func_15102920(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32);
void *memcpy(void *, const void *, u32);

/* Bobbing motion stored in a debris particle's user data (object + 0x170). */
typedef struct {
    /* 0x0 */ s16 timer;
    /* 0x4 */ f32 phase;
    /* 0x8 */ f32 phaseSpeed;
    /* 0xC */ f32 amplitude;
} ParticleWobble;

/* Spawns the impact effects (sound, sparks, debris, decal) of a hit at pos travelling along dir. */
void func_151D3480(struct17 *pos, struct17 *dir, Surf151D3480 *surface, u8 kind, u8 arg4, s32 arg5) {
    struct17 hitPos;
    Surf151D3480 hitSurface;
    u8 hit;
    s32 surfaceId;
    u8 solid;
    s32 particleType;
    s32 big;
    f32 hitDist;
    s32 hitUnk;
    s32 sfx;

    if (kind >= 6) {
        return;
    }
    switch (kind) {
        case 4:
            particleType = 12;
            break;
        case 2:
            particleType = 87;
            break;
        case 3:
            particleType = 88;
            break;
        case 0:
            particleType = 52;
            break;
        case 1:
            particleType = 77;
            break;
        case 5:
        default:
            particleType = 0;
            break;
    }
    if (surface == NULL) {
        if (func_150AC9C0(pos->unk0 - dir->unk0, pos->unk4 - dir->unk4, pos->unk8 - dir->unk8, dir->unk0, dir->unk4, dir->unk8,
                          &hitDist, &hitSurface, &hitPos.unk0, &hitPos.unk4, &hitPos.unk8, &hitUnk, &surfaceId, 0, 0.0f)) {
            hit = 1;
        } else {
            typedef s32 BlockScope; // matching: a declaration in this block changes IDO's branch layout
            hit = 0;
            surfaceId = -1;
        }
    } else {
        surfaceId = -1;
        hitPos = *pos;
        hitSurface = *surface;
        hit = 1;
    }
    solid = func_15145C90(surfaceId);
    if ((func_150ADA20() & 1) || !solid) {
        big = 1;
    } else {
        big = 0;
    }
    if (kind == 1) {
        if (big) {
            sfx = (func_150ADA20() % 3U) + 0x459;
            if (D_80082FA0 == 0) {
                func_1000FA64(sfx, pos->unk0, pos->unk4, pos->unk8, 0x4000, 1200, 800,
                              (s32)func_1000EC24, (void *)40, 0, 0, 0);
                sfx = 0x106;
            }
        } else {
            sfx = 0xB0;
        }
        func_10010E78(0, sfx, 0x6590, (func_150ADA20() % 500U) - 250, 0, 0, pos->unk0, pos->unk4, pos->unk8, 500, 800);
    }
    if (func_150ADA68() < D_800DCA24) {
        Spawn151D3480 sparks;

        sparks.unk00 = 5 >> D_800DCA20;
        sparks.unk04 = 4 >> D_800DCA20;
        sparks.unk08 = *pos;
        sparks.unk14 = 8.25f;
        sparks.unk18 = 5.859999657f;
        sparks.unk1C = 0.0f;
        sparks.unk20 = 0.0f;
        sparks.unk24 = 7.0f;
        sparks.unk28 = 16.0f;
        sparks.unk2C = 0;
        sparks.unk2E = 255;
        sparks.unk30 = -61;
        sparks.unk32 = 80;
        sparks.unk34 = 3;
        sparks.unk38 = 1;
        sparks.unk3C = 12;
        sparks.unk3E = 10;
        sparks.unk40 = 1;
        sparks.unk42 = 4;
        sparks.unk43 = 2;
        sparks.unk44 = 3;
        sparks.unk45 = 255;
        sparks.unk46 = 255;
        sparks.unk47 = 255;
        sparks.unk48 = 255;
        sparks.unk49 = 0;
        sparks.unk4A = 0;
        sparks.unk4B = 0;
        sparks.unk4C = 0;
        sparks.unk4D = 255;
        sparks.unk4E = 255;
        sparks.unk4F = 255;
        sparks.unk50 = 255;
        sparks.unk51 = 0;
        sparks.unk52 = 0;
        sparks.unk53 = 0;
        sparks.unk54 = 0;
        sparks.unk55 = 255;
        sparks.unk56 = 0;
        sparks.unk57 = 1;
        sparks.unk58 = 36;
        sparks.unk5C = 0x200005;
        sparks.unk60 = 0x60600;
        sparks.unk64 = 10;
        sparks.unk66 = 25;
        sparks.unk68 = 1;
        sparks.unk6A = 0;
        sparks.unk6C = 1.0f;
        sparks.unk70 = -1;
        sparks.unk71 = 0;
        sparks.unk72 = -1;
        sparks.unk73 = -1;
        func_15152B38(&sparks, arg4, arg5);
    }
    if (particleType != 0 && D_80082FA0 < 2) {
        ParticleDesc p;

        p.pos = *pos;
        func_15145974(dir, &p.rot.unk4, &p.rot.unk0);
        p.unk0 = 1.0f;
        p.unk4 = 1.0f;
        p.scaleY = 0.6499999762f;
        p.scaleXZ = 0.6499999762f;
        p.rot.unk8 = 0.0f;
        p.unk1C[0] = 1.0f;
        p.unk1C[1] = 1.0f;
        p.unk1C[2] = 1.0f;
        p.unk56 = particleType;
        p.unk58 = 0;
        p.unk5C = 0;
        p.unk60 = 255;
        p.unk62 = 0;
        p.unk63 = 0;
        p.unk64 = 0;
        p.unk65 = 0;
        p.unk66 = 0;
        p.unk67 = 0;
        p.unk68 = 2;
        p.unk6A = 0;
        p.owner = NULL;
        p.ownerUniqueId = 0;
        p.unk72 = 12;
        p.unk74 = 21;
        if (big) {
            struct17 unitDir;
            f32 unkOut1;
            f32 unkOut2;
            f32 speed;

            if (func_15145128(dir, &unitDir, &unkOut1, &unkOut2)) {
                speed = func_150ADA68() * 12.0f + 3.0f;
                p.unk54 = (func_150ADA20() % 11U) + 30;
                p.vel.unk0 = -unitDir.unk0 * speed;
                p.vel.unk4 = -unitDir.unk4 * speed;
                p.vel.unk8 = -unitDir.unk8 * speed;
                p.unk40.unk0 = (func_150ADA68() * 203280.0f + -101640.0f) * 9.999999747e-05f;
                p.unk40.unk4 = (func_150ADA68() * 203280.0f + -101640.0f) * 9.999999747e-05f;
                p.unk40.unk8 = (func_150ADA68() * 203280.0f + -101640.0f) * 9.999999747e-05f;
                p.unk4C = (func_150ADA68() * 449.0f + -744.0f) * 0.001000000047f;
                p.unk50 = 0x39E8;
                p.unk61 = 8;
                func_15132A4C(&p, 3, 255, 0, arg4, arg5);
            }
        } else {
            ParticleWobble wobble;
            void *obj;

            wobble.timer = (func_150ADA20() & 0xF) + 20;
            wobble.phase = 0.0f;
            wobble.phaseSpeed = (func_150ADA68() * 10000.0f + 10000.0f) * 9.999999747e-05f;
            wobble.amplitude = (func_150ADA68() * 21000.0f + 4000.0f) * 9.999999747e-05f;
            p.unk54 = 300;
            // matching: integer 0 (not 0.0f) makes IDO store with sw zero instead of an FPU move
            p.vel.unk0 = 0;
            p.vel.unk4 = 0;
            p.vel.unk8 = 0;
            p.unk40 = p.rot;
            p.unk50 = 0x3980;
            p.unk61 = 12;
            p.unk4C = 0;
            obj = func_15132A4C(&p, 3, 255, sizeof(ParticleWobble), arg4, arg5);
            if (obj != NULL) {
                memcpy((u8 *)obj + 0x170, &wobble, sizeof(ParticleWobble));
            }
        }
    }
    if (hit && solid) {
        func_15102920(func_150ADA68() * 20.0f + 15.0f, 255, &hitSurface, &hitPos, 300, 1, 1, 0, arg4, arg5);
    }
}

/* A debris particle as seen by its update callback. */
typedef struct {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ struct17 unk20;
    /* 0x02C */ u8 pad2C[0x50 - 0x2C];
    /* 0x050 */ struct17 unk50;
    /* 0x05C */ u8 pad5C[0x170 - 0x5C];
    /* 0x170 */ ParticleWobble wobble;
} WobbleParticle;

/* Particle update: bob unk20.y around unk50.y while the wobble timer runs, then settle on unk50. */
s32 func_151D3D50(WobbleParticle *particle) {
    ParticleWobble *wobble = &particle->wobble;

    if (wobble->timer > 0) {
        wobble->phase = wobble->phase + wobble->phaseSpeed * D_800BE9A4;
        wobble->phase = func_15144B68(wobble->phase);
        particle->unk20.unk4 = particle->unk50.unk4 + sinf(wobble->phase) * wobble->amplitude;
        wobble->timer = wobble->timer - D_800BE9E4;
    } else {
        particle->unk20 = particle->unk50;
    }
    return 1;
}

void func_15143134(void *, s32, s32);

/* Writes the world position of a point attached to bone `bone` of obj to out; objects without bone
   matrices use their origin raised by yOffset. */
void func_151D3E04(struct127 *obj, f32 *out, void *localPos, u8 bone, f32 yOffset) {
    if (obj->unk1D4 != NULL) {
        func_15143134(localPos, (s32)out, (s32)obj->unk1D4 + (bone << 6));
    } else {
        out[0] = obj->x_position;
        out[1] = obj->y_position + yOffset;
        out[2] = obj->z_position;
    }
}

extern void func_1505D1C4(f32, f32, f32, s32, s32, u16, s32, s32);

/* Calls func_1505D1C4 at the midpoint of a and b, with obj's movement angle relative to its unk31C. */
void func_151D3E6C(struct127 *obj, f32 *a, f32 *b, s32 arg3) {
    f32 x = (b[0] + a[0]) * 0.5f;
    f32 y = (b[1] + a[1]) * 0.5f;
    f32 z = (b[2] + a[2]) * 0.5f;

    {
        u16 angle;

        if (obj->unk31C != NULL) {
            angle = obj->unk76 - obj->unk31C->unk12;
        } else {
            angle = obj->unk76;
        }
        func_1505D1C4(x, y, z, arg3, obj - D_800CC2D0, angle, 0, 0);
    }
}

void func_151D3F14(struct17 *pos, u8 arg1, s32 arg2) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 3U) + 4;
    header.unk4 = 0;

    header2.unk0 = pos->unk0;
    header2.unk4 = pos->unk4;
    header2.unk8 = pos->unk8;

    func_151602C0(&header, &header2, (func_150ADA20() % 13U) + 20, 255, 232, 171, 255, 0, 0, arg1, arg2);
}

void func_151D3FF4(struct17 *pos, u8 arg1, s32 arg2) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 11U) + 20;
    header.unk4 = 0;

    header2.unk0 = pos->unk0;
    header2.unk4 = pos->unk4;
    header2.unk8 = pos->unk8;

    func_151602C0(&header, &header2, (func_150ADA20() % 24U) + 34, 255, 161, 162, 255, 0, 0, arg1, arg2);
}

void func_1507C3E0(struct127 *arg0, s16 *arg1, s16 *arg2, s16 *arg3);

/* Calls func_1505D1C4 twice (with arg5 and arg6) at pos; the second point is 90 units back along dir,
   or pos raised by half of target's height when there is no dir. */
void func_151D40D4(struct17 *pos, struct17 *dir, struct127 *obj, struct127 *target, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    u16 angle;
    struct17 point;
    s16 height;

    if (dir != NULL) {
        angle = (u16)(func_150484A0(dir->unk0, dir->unk8) * 10430.37793f);
        angle = (angle - 0x4000) | 1;
        point.unk0 = pos->unk0 + (dir->unk0 * -90.0f);
        point.unk4 = pos->unk4 + (dir->unk4 * -90.0f);
        point.unk8 = pos->unk8 + (dir->unk8 * -90.0f);
    } else {
        point = *pos;
        if (target != NULL) {
            func_1507C3E0(target, &height, NULL, NULL);
            point.unk4 += height >> 1;
        }
        angle = 0;
    }
    func_1505D1C4(pos->unk0, pos->unk4, pos->unk8, arg5, obj - D_800CC2D0, angle, arg7, (s32)&point);
    func_1505D1C4(pos->unk0, pos->unk4, pos->unk8, arg6, obj - D_800CC2D0, angle, arg7, (s32)&point);
}

void func_151D42E8(struct17 *pos, struct17 *dir, struct127 *obj, s32 arg3, s32 arg4) {
    f32 yaw;
    u16 angle;

    if (dir != NULL) {
        yaw = func_150484A0(dir->unk0, dir->unk8);
        angle = (u16)(yaw * 10430.37793f) - 0x4000 | 1;
    } else {
        angle = 0;
    }
    func_1505D1C4(pos->unk0, pos->unk4, pos->unk8, arg4, obj - D_800CC2D0, angle, 0, 0);
}

/* Unused outputs of func_1503F404's matrix decomposition. */
struct S151D4408Temps {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    s32 pad18; // matching: keeps the stack layout of the original temporaries
};

/* Spawns one particle at pos flying along dir, oriented by matrix mtx and attached to owner. */
void func_151D4408(struct17 *pos, struct17 *dir, void *mtx, struct127 *owner, f32 scale, u8 arg5, s32 arg6) {
    ParticleDesc p;
    struct S151D4408Temps unused;
    f32 speed;

    func_1503F404(mtx, &unused.unk0C, &unused.unk10, &unused.unk14, &p.rot.unk0, &p.rot.unk4, &p.rot.unk8,
                  &unused.unk00, &unused.unk04, &unused.unk08);
    p.rot.unk0 += 180.0f;
block_151D4408: // matching: an unused label ends the basic block, keeping the store above out of the jal delay slot
    speed = func_150ADA68();

    p.unk0 = 1.0f;
    p.unk4 = 1.0f;
    p.scaleY = 0.04200000316f * scale;
    p.scaleXZ = p.scaleY;
    p.unk1C[0] = 1.0f;
    p.unk1C[1] = 1.0f;
    p.unk1C[2] = 1.0f;
    p.pos = *pos;

    speed = (speed * 105.0f + 199.0f) * 0.001000000047f;
    p.vel.unk0 = dir->unk0 * speed;
    p.vel.unk4 = dir->unk4 * speed;
    p.vel.unk8 = dir->unk8 * speed;

    p.unk40.unk0 = (func_150ADA68() * 6378.0f + -3189.0f) * 0.001000000047f;
    p.unk40.unk4 = 0.0f;
    p.unk40.unk8 = (func_150ADA68() * 6378.0f + -3189.0f) * 0.001000000047f;
    p.unk4C = (func_150ADA68() * 320.0f + -707.0f) * 0.001000000047f;
    p.unk50 = 0x29E8;
    p.unk54 = (func_150ADA20() & 7) + 28;
    p.unk56 = 37;
    p.unk58 = 0;
    p.unk5C = 0;
    p.unk60 = 255;
    p.unk61 = 8;
    p.unk62 = 0;
    p.unk63 = 0;
    p.unk64 = 0;
    p.unk65 = 0;
    p.unk66 = 0;
    p.unk67 = 0;
    p.unk68 = 2;
    p.unk6A = 1;
    p.owner = owner;
    p.ownerUniqueId = owner->unique_id;
    p.unk72 = 6;
    p.unk74 = 42;
    func_15132A4C(&p, 3, 255, 0, arg5, arg6);
}

/* Identifies an object; uniqueId tells a reused slot apart. */
typedef struct {
    struct127 *obj;
    u8 uniqueId;
} ObjHandle;

/* Effect user data that follows a point on an object, computed by D_8008FC70[posFunc]. */
typedef struct {
    /* 0x00 */ struct127 *obj;
    /* 0x04 */ u8 uniqueId;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 posFunc;
    /* 0x0C */ f32 spawnTimer;
    /* 0x10 */ u8 flags; /* bit 0: pos is valid */
    /* 0x14 */ struct17 pos;
} ObjEmitter;

/* Sends message 0x3C (handled by func_151D4C38) for obj's attached effects. */
void func_151D4668(struct127 *obj) {
    ObjHandle handle;

    handle.obj = obj;
    handle.uniqueId = obj->unique_id;
    func_151494E0((s32)&handle, 0x3C);
}

/* Creates an effect that follows the point D_8008FC70[posFunc] computes on obj. */
void func_151D469C(obj, posFunc, arg2, arg3, arg4)
    struct127 *obj;
    u8 posFunc;
    s16 arg2;
    u8 arg3;
    s32 arg4;
{
    ObjEmitter emitter;
    struct260 *effect;

    emitter.obj = obj;
    emitter.uniqueId = obj->unique_id;
    emitter.posFunc = posFunc;
    emitter.spawnTimer = 0.0f;
    emitter.unk6 = -1;
    emitter.flags = 0;

    if (emitter.posFunc == 6) {
        emitter.unk6 = 0x82;
    }

    if (obj->unk1D4 != NULL) {
        if ((obj->unk74 & 0xF) != 0xF) {
            D_8008FC70[posFunc](obj, emitter.unk6, &emitter.pos);
            emitter.flags |= 1;
        }
    }

    effect = func_15149130(arg2, -1, 0x41, -1, 1, 0x35, (struct37 *)0x20, arg3, arg4);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &emitter, sizeof(ObjEmitter));
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
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
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[9];
} Spawn151D4794;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Extra151D4794;

/* An effect created by func_151D469C; its user data is an ObjEmitter. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ ObjEmitter emitter;
} EmitterEffect;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern f32 func_15143E64(struct17 *);

/* Effect update: tracks the emitter's point on its object and spawns smoke puffs along the path it moved. */
void func_151D4794(EmitterEffect *effect) {
    ObjEmitter *emitter;
    u8 wasValid;
    struct17 prev;
    struct127 *obj;
    struct17 diff;

    obj = effect->emitter.obj;
    prev = effect->emitter.pos;
    emitter = &effect->emitter;
    wasValid = effect->emitter.flags & 1;
    if (obj->interaction_state == 0 || obj->unique_id != emitter->uniqueId) {
        effect->unkE = -1;
        return;
    }
    if (obj->unk1D4 != NULL && (obj->unk74 & 0xF) != 0xF) {
        D_8008FC70[emitter->posFunc](obj, emitter->unk6, &emitter->pos);
        emitter->flags |= 1;
    }
    if (wasValid) {
        diff.unk0 = emitter->pos.unk0 - prev.unk0;
        diff.unk4 = emitter->pos.unk4 - prev.unk4;
        diff.unk8 = emitter->pos.unk8 - prev.unk8;
        if (func_15143E64(&diff) > 5.0f) {
            return;
        }
        emitter->spawnTimer += (0.200000003f + func_150ADA68() * 0.2090000063f) * D_800BE9A4;
        if (emitter->spawnTimer > 1.0f) {
            Spawn151D4794 spawn;
            Extra151D4794 extra;
            void *ret;

            extra.unkC = 0.9156860113f;
            spawn.unk1D = 0x6C;
            spawn.unk8 = 0x5103;
            spawn.unk0 = 0x200005;
            spawn.unk1E = 0x46;
            spawn.unk20 = 3;
            spawn.unk58 = 0x90DE07;
            spawn.unk60 = 8;
            spawn.unk61 = 6;
            spawn.unk62 = 0x20;
            spawn.unk63 = -1;
            spawn.unk4 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            spawn.unk22 = 0x46;
            spawn.unk14 = 0xDD;
            spawn.unk15 = 0xD3;
            spawn.unk16 = 0xCD;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0x57;
            spawn.unk19 = 0x55;
            spawn.unk1A = 0x5A;
            spawn.unk1C = 0xFF;
            spawn.unk3C.unk0 = 0.0f;
            spawn.unk3C.unk4 = 0.0f;
            spawn.unk3C.unk8 = 0.0f;
            spawn.unk24 = 1.025550008f;
            do {
                {
                f32 r = func_150ADA68();
                spawn.unk30.unk0 = diff.unk0 * r + prev.unk0;
                spawn.unk30.unk4 = diff.unk4 * r + prev.unk4;
                spawn.unk30.unk8 = diff.unk8 * r + prev.unk8;
                }
                spawn.unk48.unk0 = diff.unk0 * D_800BE9A8 * 0.2980000079f;
                spawn.unk48.unk4 = diff.unk4 * D_800BE9A8 * 0.2980000079f;
                spawn.unk48.unk8 = diff.unk8 * D_800BE9A8 * 0.2980000079f;
                extra.unk0 = func_150ADA20() & 0xFF;
                extra.unk1 = func_150ADA20() & 0xFF;
                extra.unk2 = func_150ADA20() % 5U + 4;
                extra.unk3 = func_150ADA20() % 5U + 4;
                extra.unk4 = func_150ADA68() * 6.0f;
                extra.unk8 = func_150ADA68() * 6.0f;
                spawn.unk54 = func_150ADA68() * 0.1800000072f + 0.1300000101f;
                spawn.unk58 &= ~0xC0;
                spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
                spawn.unk1B = func_150ADA20() % 61U + 60;
                spawn.unkA = func_150ADA20() % 11U + 70;
                spawn.unk28 = spawn.unk2C = func_150ADA68() * 40.0f + 40.0f;
                ret = func_15130280(&spawn, 1, 0, sizeof(extra), effect->unkC, effect->unk1);
                if (ret != NULL) {
                    memcpy((u8 *)ret + 0xA8, &extra, sizeof(extra));
                }
                emitter->spawnTimer -= 1.0f;
            } while (emitter->spawnTimer > 1.0f);
        }
    }
}

/* Effect message handler: 0x3C (see func_151D4668) destroys the effect when it belongs to the given
   object; 4 marks its tracked point as invalid. */
void func_151D4C38(EmitterEffect *effect, ObjHandle *handle, u8 msg) {
    ObjEmitter *emitter;

    emitter = &effect->emitter;
    if (msg == 0x3C) {
        if ((handle->obj == emitter->obj) || (handle->uniqueId == emitter->uniqueId)) {
            func_1516972C((struct102 *)effect);
        }
    } else if (msg == 4) {
        if ((handle->obj == emitter->obj) || (handle->uniqueId == emitter->uniqueId)) {
            emitter->flags = emitter->flags & ~1;
        }
    block_151D4C38: // matching: an empty labelled statement keeps the flag store out of the branch delay slot
        ;
    } else {
        func_15149514((s32)handle, msg, (s32)emitter, (s32)&emitter->uniqueId, (s32)effect);
    }
}

/* A smoke puff spawned by func_151D4794, with its Extra151D4794 user data. */
typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ u8 unk58[0x50];
    /* 0xA8 */ Extra151D4794 extra;
} Puff151D4794;

/* Puff update callback. */
s32 func_151D4D04(Puff151D4794 *puff, s32 arg1) {
    Extra151D4794 *extra;

    extra = &puff->extra;
    func_15131828(puff, &extra->unk4, extra, &extra->unk2);
    func_15131958(puff->unk58, extra->unkC, extra);
    return 1;
}

void func_151D469C(struct127 *obj, s32 posFunc, s32 arg2, s32 arg3, s32 arg4);

/* Attaches two emitters (point functions 0 and 1) to obj. */
void func_151D4D58(struct127 *obj) {
    func_151D469C(obj, 0, 80, 255, 1);
    func_151D469C(obj, 1, 80, 255, 1);
}


typedef struct {
    u8 pad0[4];
    f32 unk4;
    u8 pad8[0x50];
    u8 unk58;
    u8 unk59;
} S151D4DAC;

void func_151C2050(struct127 *, f32 *, f32 *, s32, f32);
void func_1507DE4C(struct127 *);
void func_15138C80(struct127 *, s32, s32);
void func_15137610(struct127 *, f32 *, f32 *, f32 *, u8, s32);
s32 func_1505D024(struct127 *, s32, u16, s32);

void func_151D4DAC(struct127 *arg0, struct127 *arg1, f32 *arg2, f32 *arg3, S151D4DAC *arg4, s32 arg5, f32 *arg6, u8 arg7, s32 arg8) {
    s32 alreadyDead;
    s32 health;
    s32 hp;
    u16 angle;

    if ((arg0->id == 0x9) || (arg0->id == 0xF) || (arg0->id == 0x10) || (arg0->id == 0x12) ||
        (arg0->id == 0x17) || (arg0->id == 0x1B) || (arg0->id == 0x1E) || (arg0->id == 0x28) ||
        (arg0->id == 0x29) || (arg0->id == 0x2A) || (arg0->id == 0x2B) || (arg0->id == 0x2C) ||
        (arg0->id == 0x2E) || (arg0->id == 0x38) || (arg0->id == 0x41) || (arg0->id == 0x42) ||
        (arg0->id == 0x4B) || (arg0->id == 0x46) || (arg0->id == 0x47) || (arg0->id == 0x49) ||
        (arg0->id == 0x4A) || (arg0->id == 0x4C) || (arg0->id == 0x4D) || (arg0->id == 0x4E) ||
        (arg0->id == 0x4F) || (arg0->id == 0x52) || (arg0->id == 0x5D) || (arg0->id == 0x60) ||
        (arg0->id == 0x61) || (arg0->id == 0x66) || (arg0->id == 0x67) || (arg0->id == 0x70) ||
        (arg0->id == 0x73) || (arg0->id == 0x77) || (arg0->id == 0x7B) || (arg0->id == 0x89) ||
        (arg0->id == 0x8C) || (arg0->id == 0x8E) || (arg0->id == 0x8F) || (arg0->id == 0x91) ||
        (arg0->id == 0x9E) || (arg0->id == 0xA6) || (arg0->id == 0xAB) || (arg0->id == 0xAC) ||
        (arg0->id == 0xB2) || (arg0->id == 0xB4) || (arg0->id == 0x5B)) {
        func_151C2050(arg1, arg2, arg6, 9, (arg4 != NULL) ? arg4->unk4 : 0.0f);
    }

    health = arg0->health;
    if (health == 0) {
        alreadyDead = 1;
    } else {
        alreadyDead = 0;
    }

    arg5 |= 0x60000;
    hp = arg0->health;
    if ((arg4 != NULL) && (arg4->unk59 == 3) && (arg4->unk58 == 0xA)) {
        if (health >= 3) {
            arg0->health = hp - 1;
        } else {
            arg0->health = 0;
            func_1507DE4C(arg0);
            func_15138C80(arg0, 0xFF, 1);
            arg5 = 0x100020;
        }
    }

    func_15137610(arg0, arg2, arg3, arg6, arg7, arg8);

    if (alreadyDead == 0) {
        angle = func_150484A0(arg6[0], arg6[2]) * 10430.37793f;
        func_1505D024(arg0, arg5 | 0x80000, (angle - 0x4000) | 1,
                      (arg1 != NULL) ? (arg1 - D_800CC2D0) : -1);
    }
}

void func_151D5148(void *arg0) {
    s16 *p = *(s16 **)((u8 *)arg0 + 0x31C);
    if (p[0x24 / 2] > 0) {
        p[0x24 / 2]--;
        p = *(s16 **)((u8 *)arg0 + 0x31C);
    }
    p[0x1AA / 2]++;
}

struct Obj151D57F8;

struct Sub151D5174 {
    u8 pad0[0x84];
    u8 unk84;
    u8 pad85[0x130 - 0x85];
    struct17 unk130;
    struct17 unk13C;
    u8 pad148[0x198 - 0x148];
    u8 unk198;
};

struct Obj151D5174 {
    u8 pad0[0x1D4];
    s32 unk1D4;
    u8 pad1D8[0x31C - 0x1D8];
    struct Sub151D5174 *unk31C;
};

#define OBJ ((struct Obj151D5174 *)arg0)

void func_151D5174(struct Obj151D57F8 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, struct17 *arg4, struct17 *arg5, struct17 *arg6, struct17 **arg7, struct17 **arg8, struct17 *arg9) {
    struct17 *pos;
    struct17 sp48;
    struct17 sp3C;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 t;

    if (OBJ->unk1D4 != 0) {
        if (OBJ->unk31C != NULL && (OBJ->unk31C->unk198 == 2 || OBJ->unk31C->unk84 != 0)) {
            pos = &OBJ->unk31C->unk13C;
            func_151450B4(arg2, arg3, &sp48);
            func_151450B4(&sp48, arg4, &sp3C);
            if (arg9 != NULL) {
                dx = arg9->unk0 - pos->unk0;
                dy = arg9->unk4 - pos->unk4;
                dz = arg9->unk8 - pos->unk8;
            } else {
                dx = arg1->unk0 - pos->unk0;
                dy = arg1->unk4 - pos->unk4;
                dz = arg1->unk8 - pos->unk8;
            }
            t = sp3C.unk0 * dx + sp3C.unk4 * dy + sp3C.unk8 * dz;
            t /= sp3C.unk0 * OBJ->unk31C->unk130.unk0 + sp3C.unk4 * OBJ->unk31C->unk130.unk4 +
                 sp3C.unk8 * OBJ->unk31C->unk130.unk8;
            arg5->unk0 = pos->unk0 + t * OBJ->unk31C->unk130.unk0;
            arg5->unk4 = pos->unk4 + t * OBJ->unk31C->unk130.unk4;
            arg5->unk8 = pos->unk8 + t * OBJ->unk31C->unk130.unk8;
            arg6->unk0 = OBJ->unk31C->unk130.unk0;
            arg6->unk4 = OBJ->unk31C->unk130.unk4;
            arg6->unk8 = OBJ->unk31C->unk130.unk8;
            *arg7 = arg5;
            *arg8 = arg6;
        end:
            return;
        }
    }
    *arg7 = NULL;
    *arg8 = NULL;
}

#undef OBJ

struct S151D5334_3C {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
};

extern void func_15164F0C(u8, u8, s32, u8, s32);

void func_151D5334(struct17 *arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5, s32 arg6) {
    s32 i;
    struct S151D5334_3C sp3C;

    sp3C.unk0 = *arg0;
    sp3C.unkC = arg1;
    sp3C.unk10 = arg2;
    sp3C.unk14 = arg3;

    for (i = 0; i < D_80082FA0 + 1; i++) {
        func_15164F0C(arg4, i, (s32)&sp3C, arg5, arg6);
    }
}

struct S151D5404_30 {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 pad7;
};

struct S151D5404_38 {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 pad19[7];
};

void func_151D5404(struct17 *arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    struct S151D5404_38 sp38;
    struct S151D5404_30 sp30;
    void *temp_v0;
    s32 i;

    sp38.unk0 = *arg0;
    sp38.unkC = arg1;
    sp38.unk10 = arg2;
    sp38.unk14 = arg3;
    sp30.unk0 = 1;
    sp30.unk2 = (func_150ADA20() % (u32)((s16)arg5 + 1)) + (s16)arg4;
    sp30.unk4 = 0;
    sp30.unk6 = 0;

    for (i = 0; i < D_80082FA0 + 1; i++) {
        sp30.unk5 = 1 << i;
        sp38.unk18 = i;
        temp_v0 = func_151D8868(&sp30, 0x1C, 0xFF, 0);
        if (temp_v0 != NULL) {
            memcpy((u8 *)temp_v0 + 0x18, &sp38, 0x1C);
        }
    }
}

struct vec151D5514 {
    s32 unk00;
    s32 unk04;
    s32 unk08;
};

struct conker151D5514 {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    s32 unk08;
    s32 unk0C;
    struct vec151D5514 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    s32 unk34;
    s32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s8 unk58;
};

void func_151D5514(struct vec151D5514 *arg0, u8 arg1, s32 arg2) {
    struct conker151D5514 sp1C;

    sp1C.unk00 = 0;
    sp1C.unk02 = 0xFF;
    sp1C.unk04 = -0x40;
    sp1C.unk06 = 0x4A;
    sp1C.unk08 = 9;
    sp1C.unk0C = 3;
    sp1C.unk10 = *arg0;
    sp1C.unk1C = 323.0f;
    sp1C.unk20 = 214.0f;
    sp1C.unk24 = 203.0f;
    sp1C.unk28 = 263.0f;
    sp1C.unk2C = 677.0f;
    sp1C.unk30 = 895.0f;
    sp1C.unk34 = 7;
    sp1C.unk38 = 3;
    sp1C.unk3C = 45.0f;
    sp1C.unk40 = 0.6020000577f;
    sp1C.unk44 = -1.732000113f;
    sp1C.unk48 = 1.512000084f;
    sp1C.unk4C = 0xF;
    sp1C.unk4E = 0xF;
    sp1C.unk50 = 0x64;
    sp1C.unk52 = 0x64;
    sp1C.unk54 = 0xC;
    sp1C.unk56 = 0x14;
    sp1C.unk58 = 0;
    func_1514FCE8(&sp1C, arg1, arg2);
}

void func_151D5648(void *arg0, u8 arg1, s32 arg2) {
    union { f32 f; f64 _a; } temp_f0;
    u32 temp_a;

    func_151D5514(arg0, arg1, arg2);
    func_151D3FF4(arg0, arg1, arg2);
    temp_f0.f = func_150ADA68();
    temp_a = func_150ADA20();
    func_151541B8(arg0, temp_f0.f * 4.0f + 12.0f, 0x3FD20C49,
                  (f32)(u32)((temp_a % 0x38U) + 0xC8), 0.0f, arg1, arg2);
}

extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);

void func_151D5714(struct127 *arg0, f32 *arg1, f32 *arg2, u8 arg3, f32 arg4, u8 arg5, s32 arg6) {
    struct17 a;
    struct17 b;
    s32 mtx;
    s32 srcs[2];
    s32 dsts[2];

    if (arg0->unk1D4 != NULL) {
        if ((arg0->unk74 & 0xF) != 0xF) {
            mtx = (s32)arg0->unk1D4 + (arg3 << 6);
            srcs[0] = (s32)arg1;
            srcs[1] = (s32)arg2;
            dsts[0] = (s32)&a;
            dsts[1] = (s32)&b;
            func_15145EA4(srcs, dsts, mtx, 2);
            b.unk0 = b.unk0 - a.unk0;
            b.unk4 = b.unk4 - a.unk4;
            b.unk8 = b.unk8 - a.unk8;
            {
                void func_151D4408(struct17 *, struct17 *, void *, struct127 *, f32, u8, s32);
                func_151D4408(&a, &b, (void *)mtx, arg0, arg4, arg5, arg6);
            }
        }
    }
}

struct Sub151D57F8 {
    u8 pad0[0xB0];
    u8 unkB0[0x8];
    f32 unkB8;
    f32 unkBC;
    f32 unkC0;
    u8 padC4[0x109 - 0xC4];
    u8 unk109;
};

struct Obj151D57F8 {
    u8 pad0[0x31C];
    struct Sub151D57F8 *unk31C;
};

extern s32 D_8008FC8C[];
extern u8 *D_8008FC94[];
void func_151D5A18(struct Obj151D57F8 *, struct17 *, struct17 *, struct17 *, struct17 *, s32, u8);
void func_151D5174(struct Obj151D57F8 *, struct17 *, struct17 *, struct17 *, struct17 *, struct17 *, struct17 *, struct17 **, struct17 **, struct17 *);
s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
void func_15081690(struct Obj151D57F8 *, f32, f32, f32, f32, f32, f32, void *, f32, s32, s32, s32, s32, s32, s32);
void func_150636A4(struct Obj151D57F8 *);

void func_151D57F8(struct Obj151D57F8 *arg0, u8 arg1) {
    struct17 pos;
    struct17 vA0;
    struct17 v94;
    struct17 v88;
    struct17 v7C;
    struct17 v70;
    struct17 *base;
    struct17 *ofs;

    func_151D5A18(arg0, &pos, &vA0, &v94, &v88, D_8008FC8C[arg1], *D_8008FC94[arg1]);
    func_151D5174(arg0, &pos, &vA0, &v94, &v88, &v7C, &v70, &base, &ofs, NULL);
    arg0->unk31C->unk109 = 1;
    if (base == NULL) {
        base = &pos;
    }
    if (ofs == NULL) {
        ofs = &vA0;
    }
    arg0->unk31C->unkB8 = base->unk0 + ofs->unk0 * 9999.0f;
    arg0->unk31C->unkBC = base->unk4 + ofs->unk4 * 9999.0f;
    arg0->unk31C->unkC0 = base->unk8 + ofs->unk8 * 9999.0f;
    if (arg0->unk31C->unk109) {
        struct17 dir;
        f32 len;

        dir.unk0 = arg0->unk31C->unkB8 - pos.unk0;
        dir.unk4 = arg0->unk31C->unkBC - pos.unk4;
        dir.unk8 = arg0->unk31C->unkC0 - pos.unk8;
        if (func_15145128(&dir, &dir, &len, NULL) != 0) {
            func_15081690(arg0, pos.unk0, pos.unk4, pos.unk8, dir.unk0, dir.unk4, dir.unk8,
                          arg0->unk31C->unkB0, len * 1.00999999f, 0, 0, 0, -1, 0, 0);
        }
    }
    func_150636A4(arg0);
}

struct Obj151D5A18 {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1D4 - 0x20];
    s32 unk1D4;
};

void func_15145740(struct Obj151D57F8 *, struct17 *, struct17 *, struct17 *, f32);

void func_151D5A18(struct Obj151D57F8 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, struct17 *arg4, s32 arg5, u8 arg6) {
    s32 src;
    s32 dst;

    func_15145740(arg0, arg2, arg3, arg4, 0.8110000491f);
    if (((struct Obj151D5A18 *)arg0)->unk1D4 != 0) {
        dst = (s32)arg1;
        src = arg5;
        func_15145EA4(&src, &dst, ((struct Obj151D5A18 *)arg0)->unk1D4 + (arg6 << 6), 1);
    } else {
        f32 dz;
        f32 dx;

        if ((9.999999747e-05f < fabsf(arg2->unk0)) || (9.999999747e-05f < fabsf(arg2->unk8))) {
            f32 inv;
            f32 z;

            z = arg2->unk8;
            inv = 1.0f / sqrtf((arg2->unk0 * arg2->unk0) + (z * z));
            dz = z * inv;
            dx = -arg2->unk0 * inv;
        } else {
            dz = 1.0f;
            dx = 0.0f;
        }
        arg1->unk0 = ((struct Obj151D5A18 *)arg0)->unk14 + (34.0f * dx);
        arg1->unk4 = ((struct Obj151D5A18 *)arg0)->unk18 + 49.0f;
        arg1->unk8 = ((struct Obj151D5A18 *)arg0)->unk1C + (34.0f * dz);
    }
}

s32 func_151452C4(s32 arg0, s32 arg1, struct17 *arg2, f32 arg3, struct17 *arg4, struct17 *arg5, f32 *arg6, f32 *arg7);
extern s32 (*D_80086C90[])(struct127 *, struct127 *, s32);

s32 func_151D5B6C(s32 arg0, s32 arg1, struct127 *arg2, s8 arg3, s32 arg4) {
    f32 radius;
    struct127 *obj;
    s32 mask;
    struct17 pos;
    struct17 sp74;
    struct17 sp68;
    f32 sp64;
    f32 sp60;
    u8 i;

    mask = -1;
    for (i = 0; i < 25; i++) {
        obj = &D_800CC2D0[i];
        if (obj->interaction_state == 0) continue;
        if (obj->unk5 == 3) continue;
        if (D_800BE9F0 != 0x23 && obj->id == 0xFF) continue;
        if (obj == arg2) continue;
        if (!(obj->unkF8 & 0x40)) continue;
        if (arg3 != -1 && D_80086C90[arg3](obj, arg2, arg4) == 0) continue;
        if (obj->unkD2 < obj->unkD4) {
            radius = obj->unkD4;
        } else {
            radius = obj->unkD2;
        }
        pos.unk0 = obj->x_position;
        pos.unk4 = obj->y_position + obj->unkD6;
        pos.unk8 = obj->z_position;
        if (func_151452C4(arg0, arg1, &pos, radius + radius, &sp74, &sp68, &sp64, &sp60)) {
            mask &= ~(1 << i);
        }
    }
    return mask;
}

void func_151D5D60(s32 arg0, s16 arg1, s32 arg2, s32 *arg3, s8 *arg4) {
    s8 sp27;
    s32 base;
    s32 *slot;

    if (arg4 == NULL) {
        arg4 = &sp27;
    }
    *arg4 = 0;

    slot = (s32 *)(*(s32 *)&arg0 + (arg1 * 4));
    base = *slot;
    if (base != 0) {
        goto block_6;
    }
    base = allocate_memory(arg2 * 2, 1, 2, 1);
    *slot = base;
    if (base == 0) {
        *arg3 = 0;
        return;
    }
    *arg4 = 1;
    base = *slot;

block_6:
    *arg3 = ((D_800BE9C0 != 0) ? 0 : arg2) + base;
}

void func_151D5E30(s32 *arg0) {
    s32 i;

    i = 0;
    do {
        if (arg0[i] != 0) {
            func_100043B4((s32 *)arg0[i], 3);
        }
        i = (u8)(i + 1);
    } while (i < 4);
}
