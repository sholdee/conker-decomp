#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/*
 * The types below are local copies of layouts first worked out elsewhere (fields this file does not
 * use are padding):
 *   FxParticle, QuadSprite                         - the particle/sprite system of game_169510.c
 *   PhysicsParticle, CollisionHit               - the pooled physics particles of game_15F680.c
 */

/* A live particle of game_169510.c; the update callbacks of D_80089CB8 / D_80089D60 get one. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ f32 fallSpeed;     /* first word of the per-behaviour motion block after the particle */
} FxParticle;

/* A pooled physics particle of game_15F680.c (the D_80089914 / D_8008998C callbacks get one). */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[3];
    /* 0x010 */ f32 radius;
    /* 0x014 */ f32 elasticity;
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ Vec3f rot;        /* the look-at point of func_150B9D14's matrix */
    /* 0x02C */ Vec3f scale;
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ Vec3f rotVel;
    /* 0x05C */ f32 gravity;
    /* 0x060 */ u32 flags;
    /* 0x064 */ s16 lifetime;
    /* 0x066 */ u8 pad66[0xB];
    /* 0x071 */ u8 updateFn;
    /* 0x072 */ u8 update2Fn;
    /* 0x073 */ u8 floorHitFn;
    /* 0x074 */ u8 floorType3Fn;
    /* 0x075 */ u8 wallHitFn;
    /* 0x076 */ u8 destroyFn;
    /* 0x077 */ u8 squashFn;
    /* 0x078 */ u8 matrixFn;
    /* 0x079 */ u8 pad79[0x97];
    /* 0x110 */ CollisionHit floor;
    /* 0x134 */ u8 pad134[0x18];
    /* 0x14C */ s32 unk14C;
    /* 0x150 */ u8 pad150[0x18];
    /* 0x168 */ s32 unk168;
} PhysicsParticle;

void *memcpy(void *, const void *, u32);
void func_15151A38(PuffSpray *config, u8 arg1, s32 arg2);
s32 func_15046C80(Vec3f *pos, s32 arg1, f32 bottom, CollisionHit *floor);
extern s32 D_80088740;
void func_15142600(Mtx *out, f32 scaleXZ, f32 scaleY, f32 sx, f32 sy, f32 sz, f32 x, f32 y, f32 z,
                   f32 atX, f32 atY, f32 atZ);

/* Falls (speed growing by 0.3 per frame-time) until lifetime 80; from 115 on grows by 1.35x a frame,
 * below 65 the height grows instead. */
s32 func_150B9560(FxParticle *particle) {
    f32 *speed = &particle->fallSpeed;
    f32 scale;

    if (particle->head.def.lifetime < 80) {
        *speed += 0.3f * D_800BE9A4;
        particle->head.def.pos.y -= *speed;
    }

    if (particle->head.def.lifetime >= 115) {
        scale = 1.35f;
        particle->head.def.width *= scale;
        particle->head.def.height *= scale;
    } else if (particle->head.def.lifetime < 65) {
        particle->head.def.height += 1.2f;
    }

    return 1;
}

/* Sets def.unk44 to 8x the lifetime once it drops below 32. */
s32 func_150B95FC(FxParticle *particle) {
    s16 lifetime = particle->head.def.lifetime;

    if (lifetime < 32) {
        particle->head.def.unk44 = lifetime * 8;
    }
    return 1;
}

/* Sets def.unk10 to 4x the lifetime once it drops below 64. */
s32 func_150B961C(QuadSprite *sprite) {
    s16 lifetime = sprite->def.lifetime;

    if (lifetime < 64) {
        sprite->def.unk10 = lifetime * 4;
    }
    return 1;
}

/*
 * Update of a physics particle. Within reach of object 1 (D_80088730): when that object moves faster
 * than 15 it may kick the particle away along its heading; otherwise the particle spawns a sprite on
 * the floor below and a burst, plays a sound and ends (returns 0). Else its rot (look-at) point
 * eases toward 50 above object 1 (D_80088720 / D_80088724).
 */
s32 func_150B963C(PhysicsParticle *particle) {
    f32 distSq;
    Vec3f playerPos;
    f32 dirX;
    f32 dirZ;
    Vec3f pos;

    if ((D_80088730 == NULL) || (D_80088730->interaction_state == 0) || (D_80088730->unique_id != 1)) {
        D_80088730 = func_15083E90(1);
    }
    if (D_80088730 != NULL) {
        playerPos.x = D_80088730->x_position;
        playerPos.y = D_80088730->y_position;
        playerPos.z = D_80088730->z_position;
        distSq = (particle->pos.x - playerPos.x) * (particle->pos.x - playerPos.x) +
                 (particle->pos.y - playerPos.y) * (particle->pos.y - playerPos.y) +
                 (particle->pos.z - playerPos.z) * (particle->pos.z - playerPos.z);
        if (distSq < ((particle->radius * particle->radius) + 2500.0f)) {
            if ((D_80088730->xz_velocity > 15.0f) && (func_150ADA20() & 1)) {
                dirX = func_151423D8(D_80088730->unk76 >> 8);
                dirZ = func_151423D8((D_80088730->unk76 >> 8) + 0x40);
                particle->updateFn = 1;
                particle->flags |= 0x7D;
                particle->floorHitFn = 1;
                particle->wallHitFn = 1;
                particle->matrixFn = 0;
                particle->vel.x = D_80088730->xz_velocity * dirX * 1.3f;
                particle->vel.y = (func_150ADA68() * 15.0f) + 10.0f;
                particle->vel.z = D_80088730->xz_velocity * dirZ * 1.3f;
                particle->lifetime = 300;
                particle->flags |= 0x80;
                particle->rot.x = 0.0f;
                particle->rot.y = 0.0f;
                particle->rot.z = 0.0f;
                return 1;
            }
            pos.x = particle->pos.x;
            pos.y = particle->pos.y + 100.0f;
            pos.z = particle->pos.z;
            if (func_15046C80(&pos, 0, particle->pos.y - 2000.0f, &particle->floor) != 0) {
            // matching: an unused label here makes as1 emit the following branch as beqzl
            lbl_spawn:
                pos.y = particle->floor.height;
                {
                    ParticleDef def;
                    s32 spawned;
                    f32 extra;
                    def.fadeStart = 25;
                    def.fadeRate = 10;
                    def.texture = 4;
                    def.type = 0;
                    def.renderModes = 0x3B02;
                    def.lifetime = (func_150ADA20() % 31U) + 40;
                    def.frame = 0;
                    def.frameSpeed = 0;
                    def.rgba[0] = 0xFF;
                    def.rgba[1] = 0xFF;
                    def.rgba[2] = 0xFF;
                    def.rgba[3] = 0xFF;
                    def.width = def.height = ((func_150ADA68() * 25.0f) + 25.0f) * 0.001f;
                    def.pos = pos;
                    def.flags = 0x0C5C0001;
                    def.unk44 = 0xFF;
                    def.unk45 = 0xFF;
                    def.unk46 = 0;
                    def.unk47 = 7;
                    def.rot.x = 0.0f;
                    def.rot.y = 0.0f;
                    def.rot.z = 0.0f;
                    def.scale.x = 0.0f;
                    def.scale.y = 0.0f;
                    def.scale.z = 0.0f;
                    extra = ((func_150ADA68() * 80.0f) + 50.0f) * 1e-05f;
                    def.unk50 = particle->floor.material;
                    spawned = func_1513D594(&def, &D_800A4AA0, 0x18, 0, 0,
                                            (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1), func_150ADA20() & 0xFF,
                                            500.0f, 500.0f, 0, (s32)&particle->floor.tri, particle->unk14C,
                                            particle->unk168, 0, 8, particle->unkC, particle->unk1);
                    if (spawned != 0) {
                        /* the 8-byte extra block after the particle and its motion axes (0x110 + 0x18); 4 bytes past extra are copied too */
                        memcpy((u8 *)spawned + 0x128, &extra, 8);
                    }
                }
            }
            {
                PuffSpray burst;
                burst.pos = particle->pos;
                burst.sizeBase = 3.0f;
                burst.sizeRange = 3.100000143f; // not 3.1f: the original constant is one ulp above it
                burst.countBase = 6;
                burst.countRange = 6;
                burst.angles.yawRange = 0xFF;
                burst.angles.pitchBase = -0x40;
                burst.angles.pitchRange = 30;
                burst.angles.yawBase = 0;
                burst.itemCountBase = 3;
                burst.itemCountRange = 3;
                burst.lifeBase = 30;
                burst.lifeRange = 20;
                burst.green = 1;
                burst.headerFlags = 0x21;
                burst.red = 0x48;
                burst.handlers[0] = 1;
                burst.handlers[1] = 0;
                burst.handlers[2] = 0;
                burst.handlers[3] = 0;
                burst.handlers[4] = 0;
                burst.handlers[5] = 0;
                burst.blue = 0;
                burst.alphaBase = 0x9A;
                burst.alphaRange = 0x65;
                burst.growthBase = 0.7050000429f; // not 0.705f: one ulp above it
                burst.growthRange = 0.31f;
                burst.speedBase = 5.0f;
                burst.speedRange = 7.0f;
                burst.unk58 = particle->unk14C;
                burst.unk5C = particle->unk168;
                burst.render.unk0 = 0;
                burst.render.setGeometryMode = 0x220005;
                burst.render.clearGeometryMode = 0x1D0600;
                burst.render.renderMode = 1;
                burst.render.unk10 = 0x3B;
                burst.render.unk14 = 0x80;
                burst.render.unk18 = 0x20;
                burst.render.primMode = 0;
                burst.render.envMode = 7;
                burst.unk80 = 10;
                burst.unk82 = 25;
                burst.unk84 = particle->floor.material;
                func_15151A38(&burst, particle->unkC, particle->unk1);
            }
            func_10010F88(D_80088740, 25000, 0, 0, 0, particle->pos.x, particle->pos.y, particle->pos.z, 1000, 4000);
            return 0;
        }
    }
    if ((D_80088720 == NULL) || (D_80088720->interaction_state == 0) || (D_80088720->unique_id != 1)) {
        D_80088720 = func_15083E90(1);
    }
    if (D_80088720 != NULL) {
        D_80088724.x = D_80088720->x_position;
        D_80088724.y = D_80088720->y_position + 50.0f;
        D_80088724.z = D_80088720->z_position;
    }
    particle->rot.x += 0.2f * (D_80088724.x - particle->rot.x);
    particle->rot.y += 0.2f * (D_80088724.y - particle->rot.y);
    particle->rot.z += 0.2f * (D_80088724.z - particle->rot.z);
    return 1;
}

/* Matrix callback: scale by size and scale, place at pos, and face the particle's rot point. */
s32 func_150B9D14(Mtx *mtx, PhysicsParticle *particle) {
    func_15142600(mtx, particle->sizeXZ, particle->sizeY, particle->scale.x,
                  particle->scale.y, particle->scale.z, particle->pos.x, particle->pos.y,
                  particle->pos.z, particle->rot.x, particle->rot.y, particle->rot.z);
    return 1;
}

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x83];
    /* 0x90 */ s32 unk90;
    /* 0x94 */ u8 pad94[4];
    /* 0x98 */ s16 unk98;
    /* 0x9A */ u8 pad9A[2];
    /* 0x9C */ s16 unk9C;
    /* 0x9E */ u8 pad9E[2];
    /* 0xA0 */ s16 unkA0;
    /* 0xA2 */ s16 unkA2;
    /* 0xA4 */ u8 padA4[2];
    /* 0xA6 */ s16 unkA6;
} Obj150B9D8C;

/* When unk98 is -1, spawns a flat sprite (updated by func_150B961C) at (unk9C, unkA6 + 2, unkA0),
 * unkA2 wide and long, with a random angle and flip. */
void func_150B9D8C(Obj150B9D8C *obj) {
    SpriteDef2C def;
    s32 randFlipHi;
    s32 randFlipLo;
    s32 randAngle;

    if (obj->unk98 == -1) {
        def.texture = 4;
        def.frame = 0;
        def.frameSpeed = 0;
        def.flags = 0x11;
        def.lifetime = 100;
        def.unk10 = 0xFF;
        def.unk11 = obj->unk90;
        def.rgba[0] = 0;
        def.rgba[1] = 0;
        def.rgba[2] = 0;
        def.rgba[3] = 0xFF;
        def.renderModes = 0x30001;

        randAngle = func_150ADA20();
        randFlipLo = func_150ADA20();
        randFlipHi = func_150ADA20();

        func_1513C4EC(&def, 0, 2, 0, obj->unk9C, obj->unkA6 + 2, obj->unkA0, obj->unkA2, obj->unkA2,
                      randAngle & 0xFF, ((randFlipHi & 1) << 1) + (randFlipLo & 1), 3, 0xFF, 0, obj->unkC,
                      obj->unk1);
    }
}
