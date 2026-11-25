#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Particle spawn template passed to func_15132A4C (game_15F680.c's ParticleSpawn, up to 0x78). */
typedef struct {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x5C */ s16 *unk5C;
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
} ModelParticleSpawn;

/* Burst config read by func_15151A38 (game_17CAF0.c's struct Local15151A38Arg). */
typedef struct {
    /* 0x00 */ vertex position;
    /* 0x0C */ f32 sizeBase;
    /* 0x10 */ f32 sizeRange;
    /* 0x14 */ f32 growthBase;
    /* 0x18 */ f32 growthRange;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ s16 countBase;
    /* 0x26 */ s16 countRange;
    /* 0x28 */ s16 yawBase;
    /* 0x2A */ s16 yawRange;
    /* 0x2C */ s16 pitchBase;
    /* 0x2E */ s16 pitchRange;
    /* 0x30 */ s16 fadeBase;
    /* 0x32 */ s16 fadeRange;
    /* 0x34 */ s16 lifeBase;
    /* 0x36 */ s16 lifeRange;
    /* 0x38 */ u8 green;
    /* 0x39 */ u8 enabled;
    /* 0x3A */ u8 red;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 blue;
    /* 0x55 */ u8 alphaBase;
    /* 0x56 */ u8 alphaRange;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60; /* 0x60..0x7D: params block for func_15147DA0 */
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ u8 unk7C;
    /* 0x7D */ u8 unk7D;
    /* 0x7E */ u8 pad7E[2];
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s32 unk84;
} BurstConfig;

/* Ground probe result filled by func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 pad4[0x24 - 0x4];
} Ground;

extern f32 D_8009FD30[3];
extern f32 D_8009FD3C[3];
extern f32 D_8009FD48[3];

void func_15142314(s32 model, s32 joint, f32 *out);
void func_15143134(f32 *in, f32 *out, void *mtx);
void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
void func_150B82D0(vertex *, f32 *, f32 *, f32 *, u8, s32);
void func_150B85C0(vertex *, f32 *, f32 *, u8, s32);
void func_1504715C(Ground *, struct127 *);
s32 func_15046C80(vertex *, s32, f32, Ground *);
void func_1514C678(f32, f32, f32, f32, s32, s32, s32, s32, s32, f32, s32, u8);
s32 func_15132A4C(ModelParticleSpawn *, s32, s32, s32, u8, s32);
void func_15151A38(BurstConfig *, u8, s32);

void func_150B7B40(struct127 *arg0, u8 arg1, s32 arg2) {
    f32 pts[2][3];
    f32 base[3];
    f32 sx;
    f32 sy;
    f32 scale;
    f32 ss;
    f32 cc;
    s32 pad5;
    ModelParticleSpawn p;
    s32 pad2[3];
    s8 i;
    f32 offX;
    f32 offZ;
    vertex pos;
    f32 h;
    f32 r;
    f32 ratio;

    if (arg0 != NULL) {
        if (arg0->unk1D4 != NULL) {
            func_15142314((s32)arg0->unk1D4, 4, pts[0]);
            func_15142314((s32)arg0->unk1D4, 5, pts[1]);
            func_15142314((s32)arg0->unk1D4, 0, base);
        } else {
            base[0] = arg0->x_position;
            base[1] = arg0->y_position;
            base[2] = arg0->z_position;
            pts[0][0] = base[0] + 20.0f;
            pts[0][1] = base[1] + 130.0f;
            pts[0][2] = base[2];
            pts[1][0] = base[0] - 20.0f;
            pts[1][1] = base[1] + 130.0f;
            pts[1][2] = base[2];
        }
        ss = func_151423D8(arg0->unk76 >> 8);
        cc = func_151423D8((arg0->unk76 >> 8) + 0x40);
        offX = arg0->xz_velocity * ss;
        offZ = arg0->xz_velocity * cc;
        {
            f32 a[3];
            f32 b[3];

            if (arg0->unk1D4 != NULL) {
                s32 m;
                f32 dx;
                f32 dz;

                m = (s32)arg0->unk1D4 + 0x80;
                func_15143134(D_8009FD30, &pos.x, m);
                func_15143134(D_8009FD3C, a, m);
                func_15143134(D_8009FD48, b, m);
                h = fabsf(pos.y - a[1]);
                dx = b[0] - a[0];
                dz = b[2] - a[2];
                r = sqrtf(dx * dx + dz * dz);
            } else {
                func_1515C244(arg0, &pos.x, &r, &h);
            }
        }
        if (r != 0.0f) {
            ratio = h / r;
        } else {
            ratio = 1.0f;
        }
        func_150B82D0(&pos, &h, &r, &ratio, arg1, arg2);
        func_150B85C0(&pos, &h, &r, arg1, arg2);
        {
            Ground floor;
            vertex ray;

            ray.x = pos.x;
            ray.y = pos.y + 100.0f;
            ray.z = pos.z;
            func_1504715C(&floor, arg0);
            if (func_15046C80(&ray, 0, pos.y - 2000.0f, &floor) != 0) {
                ray.y = floor.height;
                func_1514C678(ray.x, ray.y + 31.0f, ray.z, r, 0, 0xFF,
                              (func_150ADA20() % 11U) + 0x1E, 0x19, 0, 0, 0, arg1);
            }
        }
        p.model = 0;
        p.lifetime = 0x12C;
        p.elasticity = 0.6000000238f;
        p.rot.x = arg0->unkB8;
        p.rot.y = arg0->unk40;
        p.rot.z = arg0->unkC4;
        p.scale.x = 1.0f;
        p.scale.y = 1.0f;
        p.scale.z = 1.0f;
        p.flags = 0x197D;
        p.variant = 0;
        p.unk5C = 0;
        p.alpha = 0xFF;
        p.updateFn = 1;
        p.update2Fn = 0;
        p.floorHitFn = 1;
        p.floorType3Fn = 0;
        p.wallHitFn = 1;
        p.destroyFn = 0;
        p.squashFn = 0;
        p.matrixFn = 0;
        p.unk6A = 0;
        p.owner = 0;
        p.ownerId = 0;
        p.fadeStart = 1;
        p.fadeRate = 0xFF;
        p.rotVel.z = 0;
        p.gravity = -1.600000024f;
        sx = arg0->xz_scale;
        if (sx > 1.0f) {
            sx = 1.0f;
        }
        if (sx < 0.5f) {
            sx = 0.5f;
        }
        sy = arg0->y_scale;
        if (sy > 1.0f) {
            sy = 1.0f;
        }
        if (sy < 0.5f) {
            sy = 0.5f;
        }
        p.radius = ((28.0f * sy) + (28.0f * sx)) * 0.5f;
        p.sizeXZ = 3.0f * sx;
        p.sizeY = 3.0f * sy;
        for (i = 0; i < 2; i++) {
            f32 speed;
            s16 ang1;
            s16 ang2;
            f32 s1;
            f32 c1;
            f32 s2;
            f32 c2;
            f32 hv;
            BurstConfig burst;

            speed = (func_150ADA68() * 30.0f * arg0->y_scale) + 10.0f;
            ang1 = func_150ADA20() & 0x7F;
            ang2 = -0x20 - (func_150ADA20() & 0x1F);
            s1 = func_151423D8(ang1);
            c1 = func_151423D8(ang1 - 0x40);
            s2 = func_151423D8(ang2);
            c2 = func_151423D8(ang2 - 0x40);
            hv = speed * s2;
            p.pos = *(vertex *)pts[i];
            p.vel.x = (hv * c1) + offX;
            p.vel.y = -speed * c2;
            p.vel.z = (hv * s1) + offZ;
            p.rotVel.x = 30.0f - (func_150ADA68() * 60.0f);
            p.rotVel.y = 30.0f - (func_150ADA68() * 60.0f);
            func_15132A4C(&p, 3, 2, 0, arg1, 0);
            scale = ((arg0->xz_scale + arg0->y_scale) * 0.5f) / 0.9599999785f;
            burst.position = *(vertex *)pts[i];
            burst.countBase = 4;
            burst.countRange = 4;
            burst.yawRange = 0xFF;
            burst.pitchBase = -0x3F;
            burst.pitchRange = 0x4B;
            burst.sizeBase = 3.0f * scale;
            burst.sizeRange = 3.0f * scale;
            burst.growthBase = 0.6000000238f;
            burst.growthRange = 0.2310000062f;
            burst.speedBase = 15.0f * scale;
            burst.speedRange = 10.0f * scale;
            burst.yawBase = 0;
            burst.fadeBase = 3;
            burst.fadeRange = 3;
            burst.lifeBase = 0x32;
            burst.lifeRange = 0x14;
            burst.green = 1;
            burst.enabled = 0x21;
            burst.red = 0x48;
            burst.unk3C = 1;
            burst.unk40 = 0;
            burst.unk44 = 0;
            burst.unk48 = 0;
            burst.unk4C = 0;
            burst.unk50 = 0;
            burst.blue = 0;
            burst.alphaBase = 0x9B;
            burst.alphaRange = 0x64;
            burst.unk58 = 3;
            burst.unk5C = 0xFF;
            burst.unk60 = 0;
            burst.unk64 = 0x220005;
            burst.unk68 = 0x1D0600;
            burst.unk6C = 1;
            burst.unk70 = 0x3B;
            burst.unk74 = 0x80;
            burst.unk78 = 0x20;
            burst.unk7C = 0;
            burst.unk7D = 7;
            burst.unk80 = 0xA;
            burst.unk82 = 0x19;
            burst.unk84 = arg0->unk184;
            func_15151A38(&burst, arg1, arg2);
        }
    }
}

void func_15143794(s16, s16, f32, f32 *);
void *func_1518A3C0(f32 *, f32 *, f32, f32 *, f32 *, f32, f32, u8, s16, u8, s32, u8, s32);

void func_150B82D0(vertex *arg0, f32 *arg1, f32 *arg2, f32 *arg3, u8 arg4, s32 arg5) {
    s32 n;
    f32 pos[3];
    f32 rot[3];
    f32 vel[3];
    f32 sc[3];
    s16 ang1;
    s16 ang2;

    n = (func_150ADA20() % 26U) + 0x23;
    while (n > 0) {
        ang1 = func_150ADA20() & 0xFF;
        ang2 = (func_150ADA20() % 61U) - 0x3F;
        func_15143794(ang1, ang2, *arg2, pos);
        pos[1] *= *arg3;
        pos[0] += arg0->x;
        pos[1] += arg0->y;
        pos[2] += arg0->z;
        rot[0] = func_150ADA68() * 360.0f;
        rot[1] = func_150ADA68() * 360.0f;
        rot[2] = func_150ADA68() * 360.0f;
        func_15143794(ang1, ang2, ((func_150ADA68() * 1221.0f) + 1806.0f) * 0.009999999776f, vel);
        sc[0] = ((func_150ADA68() * 3140.0f) + -1570.0f) * 0.009999999776f;
        sc[1] = 0.0f;
        sc[2] = ((func_150ADA68() * 3140.0f) + -1570.0f) * 0.009999999776f;
        func_1518A3C0(pos, rot, ((func_150ADA68() * 104.0f) + 78.0f) * 0.001000000047f, vel, sc,
                      ((func_150ADA68() * 633.0f) + -1064.0f) * 0.001000000047f, 0.9425339699f, 1,
                      (func_150ADA20() % 51U) + 0x4B, 0, 0, 0xFF, 1);
        n--;
    }
}


/* Particle spawn template passed to func_15130374 (same layout as ParticleSpawn in game_1368C0.c). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 lifetime;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 color0[4];
    /* 0x18 */ u8 color1[4];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex unk3C;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 flags; /* 0x40, 0x80: random flips */
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 pad66[0x70 - 0x66];
} ParticleSpawn; /* size 0x70 */

/* Live particle returned by func_15130374; its extra data (dataSize bytes) starts at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ f32 extra;
} SpawnedParticle;

extern SpawnedParticle *func_15130374(ParticleSpawn *spawn, u8 arg1, s32 dataSize, u8 arg3, s32 arg4);

/* Spawns one dust-coloured particle at `center`, sized from `radius` and `height`. */
void func_150B85C0(vertex *center, f32 *height, f32 *radius, u8 arg3, s32 arg4) {
    ParticleSpawn spawn;

    spawn.unk1D = 0x29;
    spawn.unk8 = 0xE03;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.lifetime = (func_150ADA20() % 11U) + 35;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.color1[0] = 0xB0;
    spawn.color1[1] = 0xA0;
    spawn.color1[2] = 0x2A;
    spawn.color0[0] = 0x40;
    spawn.color0[1] = 0x0B;
    spawn.color0[2] = 0x6A;
    spawn.color0[3] = 0xFF;
    spawn.color1[3] = (func_150ADA20() % 51U) + 100;
    spawn.unk1C = 0xFF;
    spawn.unk60 = 3;
    spawn.unk61 = 3;
    spawn.width = *radius * 20.0f;
    spawn.height = *height * 20.0f;
    spawn.pos = *center;
    spawn.unk3C.x = 0.0f;
    spawn.unk3C.y = 0.0f;
    spawn.unk3C.z = 0.0f;
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    spawn.unk54 = 0.0f;
    spawn.flags = (func_150ADA20() & 1 ? 0x40 : 0) | ((func_150ADA20() & 1 ? 0x80 : 0) | 0xE01) | 0x4000 | 0x8000;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.fadeStart = 25;
    spawn.fadeRate = 10;
    spawn.unk22 = 40;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk24 = 1.009518027f;
    func_15130374(&spawn, 1, 0, arg3, arg4);
}

extern void func_15143874(s16 angle, f32 speed, f32 *outX, f32 *outZ);
void *memcpy(void *, const void *, u32);

/* Spawns one particle at (x, y, z) moving along `angle`; its extra data is a float. */
s32 func_150B879C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle, s32 arg9,
                  s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 extra;
    SpawnedParticle *particle;
    ParticleSpawn spawn;

    spawn.unk1D = 0x29;
    spawn.unk8 = 0xE03;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    extra = 0.9172409773f;
    spawn.lifetime = (func_150ADA20() & 0xF) + 25;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.color1[0] = 0xB0;
    spawn.color1[1] = 0xA0;
    spawn.color1[2] = 0x2A;
    spawn.color0[0] = 0x40;
    spawn.color0[1] = 0x0B;
    spawn.color0[2] = 0x6A;
    spawn.color0[3] = 0xFF;
    spawn.color1[3] = (func_150ADA20() % 71U) + 60;
    spawn.unk1C = 0xFF;
    spawn.unk60 = 3;
    spawn.unk61 = 3;
    spawn.width = spawn.height = ((func_150ADA68() * 15738.0f) + 24699.0f) * 0.01f;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    spawn.unk3C.x = 0.0f;
    spawn.unk3C.y = 0.0f;
    spawn.unk3C.z = 0.0f;
    func_15143874(angle, ((func_150ADA68() * 154.0f) + 205.0f) * 0.1f, &spawn.vel.x, &spawn.vel.z);
    spawn.vel.y = 0.0f;
    spawn.unk54 = 0.0f;
    spawn.flags = (func_150ADA20() & 1 ? 0x40 : 0) | ((func_150ADA20() & 1 ? 0x80 : 0) | 0xE05) | 0x4000 | 0x8000;
    spawn.unk62 = 10;
    spawn.unk63 = -1;
    spawn.fadeStart = 28;
    spawn.fadeRate = 9;
    spawn.unk22 = 25;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk24 = 1.033332944f;
    particle = func_15130374(&spawn, 1, sizeof(extra), argE, 1);
    if (particle != NULL) {
        memcpy(&particle->extra, &extra, sizeof(extra));
    }
    return 1;
}
