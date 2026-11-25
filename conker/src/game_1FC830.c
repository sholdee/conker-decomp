#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct { f32 x, y, z; } TrailVec3;
typedef struct { f32 x, z; } TrailVec2;

typedef struct Vec3i151CD674 {
    s32 x;
    s32 y;
    s32 z;
} Vec3i151CD674;

extern f32 D_800D9860;
extern f32 func_15144AA8(s32);
extern struct17 *func_15144B34(s32);
extern s32 func_15046C80(struct17 *, s32, f32, f32 *);
extern struct260 *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
f32 func_15143E64(struct17 *);
struct260 *func_151CEAAC(void *arg0, void *arg1, u8 arg2, u8 arg3, s32 arg4);

/* One point of a ribbon trail. */
typedef struct {
    /* 0x00 */ TrailVec3 pos;
    /* 0x0C */ u8 pad0C[8];
    /* 0x14 */ f32 width;
    /* 0x18 */ TrailVec2 offset; /* half-width vector across the ribbon */
    /* 0x20 */ f32 dirX;
    /* 0x24 */ f32 dirZ;
} RibbonPoint;

typedef struct {
    struct127 *owner;
} RibbonInfo;

/* Actor that draws a ribbon through a ring buffer of points. */
typedef struct {
    /* 0x00 */ u8 pad0[0x25];
    /* 0x25 */ u8 capacity;
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 numPoints;
    /* 0x2D */ s8 head; /* oldest point */
    /* 0x2E */ s8 tail; /* one past the newest point */
    /* 0x2F */ u8 pad2F[0x55];
    /* 0x84 */ u8 vtxBuffer[0x10];
    /* 0x94 */ RibbonPoint *points;
    /* 0x98 */ RibbonInfo *info;
} RibbonActor;

/* The two cycle words of a render mode. */
typedef struct {
    s32 cycle1;
    s32 cycle2;
} RenderModePair;

extern RenderModePair D_800A4AC8[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

/* Draws the ribbon from its newest point back to its oldest, one quad per segment. */
Gfx *func_151CF380(RibbonActor *ribbon, Gfx *gfx, s16 arg2) {
    RibbonInfo *info;
    TrailVec3 nextPos;
    TrailVec3 curPos;
    s32 cur;
    s32 next;
    RibbonPoint *points;
    u8 update;
    f32 curWidth;
    f32 curX;
    f32 curZ;
    f32 nextWidth;
    f32 nextX;
    f32 nextZ;
    TrailVec2 nextOffset;
    TrailVec2 curOffset;
    Vtx *vtx;

    if (ribbon->numPoints < 2) {
        return gfx;
    }
    info = ribbon->info;
    points = ribbon->points;
    if (info->owner != NULL && info->owner->unk1D4 == NULL) {
        return gfx;
    }
    func_151D5D60(ribbon->vtxBuffer, arg2, (ribbon->capacity * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
    update = 1;
    gfx = func_15142B7C(func_15142C10(gfx, 0xED, 0xD2, 0x85, 0xC8, &update), 0x200005, 0x1F0600);
    gfx = func_15142FBC(func_1513F4E4(gfx, 0x54, &update), D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[1].cycle1 | D_800A4AC8[1].cycle2, &update);
    cur = ribbon->tail - 1;
    if (cur < 0) {
        cur = ribbon->capacity - 1;
    }
    next = cur - 1;
    if (next < 0) {
        next = ribbon->capacity - 1;
    }
    curPos = points[cur].pos;
    curWidth = points[cur].width;
    curX = points[cur].dirX * curWidth;
    curZ = points[cur].dirZ * curWidth;
    curOffset = points[cur].offset;
    nextPos = points[next].pos;
    nextWidth = points[next].width;
    nextX = points[next].dirX * nextWidth;
    nextZ = points[next].dirZ * nextWidth;
    nextOffset = points[next].offset;
    vtx->v.ob[0] = (s32)(curPos.x + curOffset.x + curX);
    vtx->v.ob[1] = (s32)curPos.y;
    vtx->v.ob[2] = (s32)(curPos.z + curOffset.z + curZ);
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = (s32)(curPos.x - curOffset.x + curX);
    vtx->v.ob[1] = (s32)curPos.y;
    vtx->v.ob[2] = (s32)(curPos.z - curOffset.z + curZ);
    vtx->v.flag = 0;
    vtx++;
    do {
        vtx->v.ob[0] = (s32)(nextPos.x + nextOffset.x + nextX);
        vtx->v.ob[1] = (s32)nextPos.y;
        vtx->v.ob[2] = (s32)(nextPos.z + nextOffset.z + nextZ);
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = (s32)(nextPos.x - nextOffset.x + nextX);
        vtx->v.ob[1] = (s32)nextPos.y;
        vtx->v.ob[2] = (s32)(nextPos.z - nextOffset.z + nextZ);
        vtx->v.flag = 0;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP2Triangles(gfx++, 0, 1, 2, 0, 1, 3, 2, 0);
        cur = next;
        next--;
        if (next < 0) {
            next = ribbon->capacity - 1;
        }
        if (cur != ribbon->head) {
            curPos = points[cur].pos;
            // matching: dead recompute of curWidth/curX/curZ (DCE'd) steers the address base IDO keeps
            curWidth = points[cur].width;
            curX = points[cur].dirX * curWidth;
            curZ = points[cur].dirZ * curWidth;
            nextPos = points[next].pos;
            nextWidth = points[next].width;
            nextX = points[next].dirX * nextWidth;
            nextZ = points[next].dirZ * nextWidth;
            nextOffset = points[next].offset;
            curOffset = points[cur].offset;
        }
    } while (cur != ribbon->head);
    return gfx;
}

/* Data of an actor that follows an owner: the owner, then the owner's unique id. */
typedef struct {
    /* 0x0 */ s32 owner;
    /* 0x4 */ s32 ownerId;
} FollowerData;

typedef struct {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ FollowerData *data;
} FollowerActor;

/* Message handler of a following actor: lets func_15169850 update or end the link to its owner. */
void func_151CF844(FollowerActor *actor, s32 arg1, u8 arg2) {
    FollowerData *data = actor->data;

    if (data->owner != 0) {
        func_15169850(arg1, arg2, (s32)&data->owner, (s32)&data->ownerId, (s32)actor);
    }
}

/* Common head of an effect object (struct260 is a shorter view); each effect keeps its own data
 * from +0x28. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state; /* set to -1 to free the effect */
    /* 0x10 */ u8 pad10[0x28 - 0x10];
} EffectHead;

/* Data of the ground effect spawned by func_151CF898. */
typedef struct {
    struct17 pos;
    f32 unkC;
} GroundEffectData;

typedef struct {
    EffectHead head;
    GroundEffectData data;
} GroundEffect;

/* With probability 0.222, spawns an effect on the ground at a random point up to 2000 units from
 * the camera of the object's player, within 40 degrees either side of the camera's yaw. */
void func_151CF898(struct127 *arg0, f32 probeY, f32 arg2) {
    struct108 *player;
    struct17 spawnPos;
    struct17 *camPos;
    f32 yaw;
    f32 angle;
    f32 distance;
    GroundEffectData data;
    GroundEffect *effect;

    player = arg0->camera;
    if (player != NULL) {
        if (func_150ADA68() < 0.2220000178f) {
            camPos = func_15144B34(player->unk23D);
            yaw = func_15144AA8(player->unk23D);
            angle = ((func_150ADA68() * 80.0f) + (yaw - 40.0f)) * 0.01745329238f;
            distance = func_150ADA68() * 2000.0f;

            spawnPos.unk0 = camPos->unk0 - (sinf(angle) * distance);
            spawnPos.unk4 = probeY;
            spawnPos.unk8 = camPos->unk8 - (cosf(angle) * distance);

            if (func_15046C80(&spawnPos, 0, arg2, &D_800D9860) != 0) {
                spawnPos.unk4 = D_800D9860;
                data.pos = spawnPos;
                data.unkC = 0.0f;

                effect = (GroundEffect *)func_15149130((s16)((func_150ADA20() % 131U) + 51), -1, 0x20, -1, 1, 0,
                                                       (struct37 *)sizeof(data), 0xFF, 1);
                if (effect != NULL) {
                    memcpy(&effect->data, &data, sizeof(data));
                }
            }
        }
    }
}

/* Spawn request for func_15151A38. */
typedef struct {
    /* 0x00 */ struct17 pos;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ s8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s8 unk7C;
    /* 0x7D */ s8 unk7D;
    /* 0x7E */ s16 pad7E;
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
} Spawn15151A38;

/* Particle spawn template for func_15132A4C: the layout of Particle 0x10..0x8B (see game_15F680.c),
 * copied into the new particle. */
typedef struct {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ struct17 rot;
    /* 0x1C */ struct17 scale;
    /* 0x28 */ struct17 pos;
    /* 0x34 */ struct17 vel;
    /* 0x40 */ f32 rotVelX;
    /* 0x44 */ f32 rotVelY;
    /* 0x48 */ f32 rotVelZ;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ s32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ s16 model;
    /* 0x58 */ u8 variant;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
} ParticleSpawn;

/* Spawn request for func_15150D1C. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct17 pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s8 unk2C;
} Spawn15150D1C;

extern struct17 D_800AAEEC[];
// matching: the model goes in as s32; a struct255 * argument changes the temp registers
void func_1503F404(s32, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
void func_15143134(struct17 *, struct17 *, Mtx *);
void func_15143794(s16, s16, f32, struct17 *);
s32 func_15132A4C(ParticleSpawn *, s32, s32, s32, u8, s32);
void func_15150D1C(Spawn15150D1C *, u8, s32);
void func_151A9834(struct17 *, f32, f32, void *, s32, s32, s8 *, u8, s32);
void func_15151A38(Spawn15151A38 *, u8, s32);
void func_1504715C(void *, struct127 *);

/* Breaks the object apart: seven model pieces fly off its model's points with random velocity and
 * spin, plus three more effects at 200 units above it. */
void func_151CFA4C(struct127 *arg0, u8 arg1, s32 arg2) {
    struct17 vecB;
    struct17 pos;
    struct17 vecA;
    struct17 vecC;
    u8 i;

    pos.unk0 = arg0->x_position;
    pos.unk4 = arg0->y_position + 200.0f;
    pos.unk8 = arg0->z_position;
    if (arg0->unk1D4) {
        func_1503F404((s32)arg0->unk1D4, &vecA.unk0, &vecA.unk4, &vecA.unk8, &vecB.unk0, &vecB.unk4, &vecB.unk8,
                      &vecC.unk0, &vecC.unk4, &vecC.unk8);
        {
            ParticleSpawn piece;
            const s32 models[7] = { 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75 };

            piece.radius = 1.0f;
            piece.elasticity = 0.6490000486f;
            piece.sizeXZ = piece.sizeY = (arg0->xz_scale + arg0->y_scale) * 0.5f;
            piece.rot = vecB;
            piece.scale.unk0 = 1.0f;
            piece.scale.unk4 = 0.0f;
            piece.scale.unk8 = 1.0f;
            piece.rotVelY = 0.0f;
            piece.variant = 0;
            piece.unk5C = 0;
            piece.alpha = 0xFF;
            piece.update2Fn = 0;
            piece.floorType3Fn = 0;
            piece.wallHitFn = 0;
            piece.destroyFn = 0;
            piece.squashFn = 0;
            piece.unk6A = 1;
            piece.owner = arg0;
            piece.ownerId = arg0->unique_id;
            piece.fadeStart = 12;
            piece.fadeRate = 21;
            piece.flags = 0x39E9;
            piece.updateFn = 13;
            piece.matrixFn = 2;
            piece.floorHitFn = 7;
            for (i = 0; i < 7; i++) {
                u32 yaw;
                u32 pitch;

                piece.model = models[i];
                func_15143134(&D_800AAEEC[i], &piece.pos, (Mtx *)arg0->unk1D4);
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794(yaw & 0xFF, (pitch % 25U) - 64, (func_150ADA68() * 11.44999981f) + 16.93000031f, &piece.vel);
                piece.rotVelX = (func_150ADA68() * 19.75200081f) + -9.876000404f;
                piece.rotVelZ = (func_150ADA68() * 19.75200081f) + -9.876000404f;
                piece.gravity = (func_150ADA68() * 0.5180000067f) + -1.539000034f;
                piece.lifetime = (func_150ADA20() % 101U) + 100;
                func_15132A4C(&piece, 3, 0xFF, 0, arg1, arg2);
            }
        }
        {
            Spawn15151A38 cfg;

            cfg.pos = pos;
            cfg.unk0C = 8.0f;
            cfg.unk10 = 13.0f;
            cfg.unk14 = 0.8000000119f;
            cfg.unk18 = 0.400000006f;
            cfg.unk1C = 23.0f;
            cfg.unk20 = 7.0f;
            cfg.unk24 = 13;
            cfg.unk26 = 6;
            cfg.unk28 = 0;
            cfg.unk2A = 0xFF;
            cfg.unk2C = -64;
            cfg.unk2E = 50;
            cfg.unk30 = 3;
            cfg.unk32 = 2;
            cfg.unk34 = 40;
            cfg.unk36 = 25;
            cfg.unk38 = 9;
            cfg.unk39 = 1;
            cfg.unk3A = 0x48;
            cfg.unk3C = 1;
            cfg.unk40 = 0;
            cfg.unk44 = 0;
            cfg.unk48 = 0;
            cfg.unk4C = 0;
            cfg.unk50 = 0;
            cfg.unk54 = 0;
            cfg.unk55 = 150;
            cfg.unk56 = 105;
            cfg.unk58 = 3;
            cfg.unk5C = 0xFF;
            cfg.unk60 = 0;
            cfg.unk64 = 0x220005;
            cfg.unk68 = 0x1D0600;
            cfg.unk6C = 3;
            cfg.unk70 = 0x3B;
            cfg.unk74 = 0x80;
            cfg.unk78 = 0x20;
            cfg.unk7C = 0;
            cfg.unk7D = 7;
            cfg.unk80 = 10;
            cfg.unk82 = 25;
            func_15151A38(&cfg, arg1, arg2);
        }
        {
            Spawn15150D1C cfg2;

            cfg2.unk0 = 15;
            cfg2.unk4 = 6;
            cfg2.pos = pos;
            cfg2.unk14 = 0;
            cfg2.unk16 = 0xFF;
            cfg2.unk18 = -29;
            cfg2.unk1A = 40;
            cfg2.unk1C = 375.0f;
            cfg2.unk20 = 20.0f;
            cfg2.unk24 = 40.0f;
            cfg2.unk28 = 120;
            cfg2.unk2A = 120;
            cfg2.unk2C = 2;
            func_15150D1C(&cfg2, arg1, arg2);
        }
        {
            u8 dst[0x24];
            s8 flag;
            s32 pad; // matching: unused stack slot in the original frame

            func_1504715C(dst, arg0);
            flag = 2;
            func_151A9834(&pos, pos.unk4 - 2000.0f, 200.0f, dst, (func_150ADA20() & 3) + 7, 0, &flag, arg1, arg2);
        }
    }
}

/* Message 0x18 (and 0): obj no longer exists. */
typedef struct {
    struct127 *obj;
    u8 objId;
} OwnerGoneMsg;

/* Message 0x2D: objects a and b swapped places. */
typedef struct {
    struct127 *a;
    struct127 *b;
    u8 aId;
    u8 bId;
} OwnerSwapMsg;

/* Tells every effect that follows the object that it is gone (message 0x18). */
void func_151D0024(struct127 *obj) {
    OwnerGoneMsg msg;

    msg.obj = obj;
    msg.objId = obj->unique_id;
    func_151494E0((s32)&msg, 0x18);
}

/* Data of the camera-facing strip effect spawned by func_151D0058 and drawn by func_151D014C. */
typedef struct {
    /* 0x0 */ struct127 *owner;
    /* 0x4 */ u8 ownerId;   /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 ownerUnk84; /* owner->unk84 at spawn; the effect ends when it changes */
    /* 0x8 */ f32 scale;
    /* 0xC */ u8 alpha;
    /* 0xD */ u8 variant;   /* which pair of model points the strip joins */
    /* 0xE */ u8 padE[2];
} StripEffectData;

typedef struct {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ s16 state; /* set to -1 to free the effect */
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ u8 vtxBuffer[0x14];
    /* 0x28 */ StripEffectData data;
} StripEffect;

/* Spawns a strip effect of the given variant on the object. */
void func_151D0058(struct127 *obj, u8 variant, u8 arg2, s32 arg3) {
    StripEffect *effect;
    StripEffectData data;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.scale = (obj->xz_scale + obj->y_scale) * 0.5f;
    data.variant = variant;
    data.alpha = (func_150ADA20() % 56U) + 200;
    data.ownerUnk84 = obj->unk84.uh;

    effect = (StripEffect *)func_15149130(300, -1, 0x61, 4, 0, 0x31, (struct37 *)sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Ends the strip effect once its owner's unk84 has changed. */
void func_151D0128(StripEffect *effect) {
    struct127 *owner = effect->data.owner;

    if (effect->data.ownerUnk84 != owner->unk84.uh) {
        effect->state = -1;
    }
}

extern struct17 D_800AAF5C[];
extern struct17 D_800AAF74;
extern u8 D_800AAF80[];
extern u8 D_80091154[];
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);

/* Draws the strip effect: six camera-facing quads joining two of the owner's model points, pinched
 * in the middle when the points are far apart. */
Gfx *func_151D014C(Gfx *gfx, StripEffect *effect, s16 arg2) {
    StripEffectData *data;
    Vtx *vtx;
    struct127 *owner;
    u8 update;
    struct17 *camPos;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 side[3];
    f32 taper;
    struct17 from;
    struct17 to;
    struct17 diff;
    f32 stepX;
    f32 stepY;
    f32 stepZ;
    struct17 pos;
    f32 ang;
    f32 progress;
    f32 dist;
    f32 amp;
    f32 tex;
    f32 unused;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    s32 i;

    owner = effect->data.owner;
    data = &effect->data;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->state = -1;
        return gfx;
    }
    if (owner->unk1D4 == NULL) {
        return gfx;
    }
    func_151D5D60(effect->vtxBuffer, arg2, 0x1C0, (void **)&vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    update = 1;
    func_15143134(&D_800AAF5C[data->variant], &from, (Mtx *)owner->unk1D4 + 2);
    func_15143134(&D_800AAF74, &to, (Mtx *)owner->unk1D4 + D_800AAF80[data->variant]);
    camPos = func_15144B34(arg2);
    gfx = func_15142B7C(gfx, 0x200005, 0x60600);
    gfx = func_15142E24(gfx, (s32)D_80091154, 0, 0, 0, 0, 0x7F, 0, 0, &update, 3);
    gfx = func_1513F4E4(gfx, 0x3B, &update);
    gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, data->alpha, &update);
    gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].cycle1 | D_800A4AC8[3].cycle2, &update);
    diff.unk0 = to.unk0 - from.unk0;
    diff.unk4 = to.unk4 - from.unk4;
    diff.unk8 = to.unk8 - from.unk8;
    dist = func_15143E64(&diff);
    midX = diff.unk0 * 0.5f + from.unk0 - camPos->unk0;
    midY = diff.unk4 * 0.5f + from.unk4 - camPos->unk4;
    midZ = diff.unk8 * 0.5f + from.unk8 - camPos->unk8;
    offsetX = diff.unk4 * midZ - midY * diff.unk8;
    offsetY = diff.unk8 * midX - midZ * diff.unk0;
    offsetZ = diff.unk0 * midY - midX * diff.unk4;
    if (offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ == 0.0f) {
        side[0] = 0.0f;
        side[1] = 0.0f;
        side[2] = 0.0f;
    } else {
        amp = (35.0f * data->scale) / sqrtf(offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ);
        side[0] = offsetX * amp;
        side[1] = offsetY * amp;
        side[2] = offsetZ * amp;
    }
    stepX = diff.unk0 * 0.1666666716f;
    stepY = diff.unk4 * 0.1666666716f;
    stepZ = diff.unk8 * 0.1666666716f;
    if (dist < 20.0f) {
        amp = 0.0f;
    } else if (dist < 70.0f) {
        amp = (dist - 20.0f) * 0.01999999955f * 0.6000000238f;
    } else {
        amp = 0.6000000238f;
    }
    // matching: one source line; as1 orders the hoisted loop constants against the copy by line number
    pos = from; progress = 0.0f; ang = 0.0f; tex = 0.0f; i = 0; do {
        taper = 1.0f - sinf(ang) * amp;
        offsetX = side[0] * taper;
        vtx->v.ob[0] = (s32)(offsetX + pos.unk0);
        vtx->v.ob[1] = (s32)(side[1] * taper + pos.unk4);
        vtx->v.ob[2] = (s32)(side[2] * taper + pos.unk8);
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = (s32)tex;
        vtx++;
        vtx->v.ob[0] = (s32)(pos.unk0 - offsetX);
        vtx->v.ob[1] = (s32)(pos.unk4 - side[1] * taper);
        vtx->v.ob[2] = (s32)(pos.unk8 - side[2] * taper);
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0x7C0;
        vtx->v.tc[1] = (s32)tex;
        vtx++;
        taper = 1.0f - sinf(ang + 0.5235987902f) * amp;
        offsetX = side[0] * taper;
        vtx->v.ob[0] = (s32)(pos.unk0 + stepX - offsetX);
        offsetY = side[1] * taper;
        vtx->v.ob[1] = (s32)(pos.unk4 + stepY - offsetY);
        offsetZ = side[2] * taper;
        vtx->v.ob[2] = (s32)(pos.unk8 + stepZ - offsetZ);
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0x7C0;
        vtx->v.tc[1] = (s32)(tex + 341.3333435f);
        vtx++;
        // matching: dead store; its two variable reads shift where uopt splits the basic block
        unused = offsetX + offsetY;
        vtx->v.ob[0] = (s32)(pos.unk0 + stepX + offsetX);
        vtx->v.ob[1] = (s32)(pos.unk4 + stepY + offsetY);
        vtx->v.ob[2] = (s32)(pos.unk8 + stepZ + offsetZ);
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = (s32)(tex + 341.3333435f);
        vtx++;
        i++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
        pos.unk0 += stepX;
        pos.unk4 += stepY;
        pos.unk8 += stepZ;
        progress += 0.1666666716f;
        ang += 0.5235987902f;
        tex += 341.3333435f;
    } while (i != 6);
    return gfx;
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA151D08F0;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB151D08F0;

void func_151D08F0(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA151D08F0 *temp_v0 = (SubA151D08F0 *)((u8 *)arg0 + 0x28);
    ArgB151D08F0 *b = (ArgB151D08F0 *)arg1;
    s32 b0;

    if (arg2 == 0 || arg2 == 0x18) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

typedef struct {
    u8 unk0;
    struct17 unk4;
    struct17 unk10;
    struct17 unk1C;
    f32 unk28;
} ShapeA151D09A8; /* 0x2C */

typedef struct {
    u8 unk0[0x24];
} Dst151D09A8;

typedef struct {
    struct17 unk0;
    s16 unkC;
    s16 unkE;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[2];
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    u8 unk31;
    u8 pad32[2];
    f32 unk34;
    u8 unk38;
    u8 pad39[3];
    f32 unk3C;
} EmitA151D09A8; /* 0x40 */

typedef struct {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
    s32 unk40;
} EmitB151D09A8; /* 0x44 */

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
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Particle151D09A8; /* 0x70 */

extern f32 D_800AAF84[3];
extern f32 D_800AAF90[3];
extern void func_15145EA4(s32 *, s32 *, s32, s32);
extern void func_1504715C(void *, struct127 *);
extern void func_1514FF44(ShapeA151D09A8 *, EmitA151D09A8 *, Dst151D09A8 *, u8, s32);
extern void func_15153CCC(ShapeA151D09A8 *, EmitB151D09A8 *, Dst151D09A8 *, u8, s32);
extern s32 func_1514F6E8(ShapeA151D09A8 *);
extern void func_1514F808(ShapeA151D09A8 *, f32, struct17 *);
extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151D09A8(struct127 *arg0, u8 arg1, s32 arg2) {
    struct17 pos;
    struct17 dir;
    Dst151D09A8 dst;
    ShapeA151D09A8 a;

    if (arg0->unk1D4 != NULL) {
        {
            s32 srcs[2];
            s32 dsts[2];

            srcs[0] = (s32)D_800AAF84;
            srcs[1] = (s32)D_800AAF90;
            dsts[0] = (s32)&pos;
            dsts[1] = (s32)&dir;
            func_15145EA4(srcs, dsts, (s32)arg0->unk1D4 + 0x80, 2);
            dir.unk0 -= pos.unk0;
            dir.unk4 -= pos.unk4;
            dir.unk8 -= pos.unk8;
        }
        func_1504715C(&dst, arg0);
        a.unk0 = 0;
        a.unk4 = dir;
        a.unk10 = a.unk1C = *(struct17 *)&D_800A5480;
        {
            EmitA151D09A8 emitA;

            emitA.unk0 = pos;
            emitA.unkC = 6;
            emitA.unkE = 8;
            emitA.unk10 = 0.03999999911f;
            emitA.unk14 = 0.02999999933f;
            emitA.unk18 = 0x1E;
            emitA.unk1A = 0x1E;
            emitA.unk1C = -1.467000127f;
            emitA.unk20 = 0.756000042f;
            emitA.unk24 = 0x9B;
            emitA.unk25 = 0x64;
            emitA.unk28 = 403.0f;
            emitA.unk2C = 455.0f;
            emitA.unk30 = 1;
            emitA.unk31 = 2;
            emitA.unk34 = 0.0f;
            emitA.unk38 = 1;
            emitA.unk3C = 0.0f;
            a.unk28 = 750.0f;
            func_1514FF44(&a, &emitA, &dst, arg1, arg2);
        }
        {
            EmitB151D09A8 emitB;

            emitB.unk0 = pos;
            emitB.unkC = 10.0f;
            emitB.unk10 = 25.0f;
            emitB.unk14 = 1.253000021f;
            emitB.unk18 = 0.8230000138f;
            emitB.unk1C = 0.03500000015f;
            emitB.unk20 = 0.03199999779f;
            emitB.unk24 = 0xF;
            emitB.unk26 = 8;
            emitB.unk28 = 3;
            emitB.unk2A = 2;
            emitB.unk2C = 0x1E;
            emitB.unk2E = 0x19;
            emitB.unk30 = 0x9B;
            emitB.unk32 = 0x64;
            emitB.unk38 = 0.0f;
            emitB.unk3C = 0x10;
            emitB.unk3E = 0xF;
            emitB.unk40 = 0;
            emitB.unk34 = 2;
            a.unk28 = 750.0f;
            func_15153CCC(&a, &emitB, &dst, arg1, arg2);
        }
        if (func_1514F6E8(&a) != 0) {
            s32 count;
            Particle151D09A8 spawn;
            void *ret;
            s32 pad;

            a.unk28 = 1000.0f;
            count = (func_150ADA20() % 6U) + 5;
            spawn.unk1D = 0x3E;
            spawn.unk8 = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk14 = 0;
            spawn.unk15 = 0;
            spawn.unk16 = 0;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0;
            spawn.unk19 = 0;
            spawn.unk1A = 0;
            spawn.unk1B = 0;
            spawn.unk1C = 0xFF;
            spawn.pos = pos;
            spawn.unk3C = *(struct17 *)&D_800A5480;
            spawn.unk22 = 1;
            spawn.unk58 = 0xC007;
            spawn.unk24 = 1.0f;
            spawn.unkA = (func_150ADA20() % 11U) + 0x28;
            spawn.unk1E = 1;
            spawn.unk20 = 0xFF;
            spawn.unk60 = 6;
            spawn.unk61 = 8;
            spawn.unk64 = -1;
            spawn.unk62 = -1;
            spawn.unk63 = -1;
            spawn.unk65 = 0xB;
            spawn.unk5C = 0;
            spawn.unk66 = 0xFF;
            spawn.unk68 = 0;
            spawn.unk2C = 0.0f;
            spawn.unk28 = 0.0f;
            spawn.unk6C = 10000.0f;
            while (count != 0) {
                func_1514F808(&a, (func_150ADA68() * 0.03999999911f) + 0.04999999702f, &spawn.vel);
                spawn.unk54 = (func_150ADA68() * 1.340000033f) + -2.328999996f;
                ret = func_15130280(&spawn, 1, 0, 4, arg1, arg2);
                if (ret != NULL) {
                    *(struct260 **)((u8 *)ret + 0xA8) = func_151CEAAC(NULL, ret, 0, arg1, arg2);
                }
                count--;
            }
        }
    }
}

void func_151D0ED8(void *arg0) {
    if (*(struct102 **)((u8 *)arg0 + 0xA8) != 0) {
        func_1516972C(*(struct102 *volatile *)((u8 *)arg0 + 0xA8));
    }
}

void func_151D0ED8(void *arg0);

void func_1513173C(void *arg0);

void func_151D0F08(void *arg0) {
    func_151D0ED8(arg0);
    func_1513173C(arg0);
}

void func_1513175C(void *arg0);

void func_151D0F34(void *arg0) {
    func_151D0ED8(arg0);
    func_1513175C(arg0);
}

extern s32 (*D_8008FC30[])(struct127 *, void *);

struct vec3_151D0F60 { f32 x; f32 y; f32 z; };

void func_151D0F60(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct260 *temp_v0;
    struct {
        struct127 *unk0;
        u8 unk4;
        u8 pad5[3];
        s32 unk8;
        struct vec3_151D0F60 unkC;
        struct vec3_151D0F60 unk18;
        struct vec3_151D0F60 unk24;
        f32 unk30;
        u8 unk34;
    } sp34;

    sp34.unk0 = arg0;
    sp34.unk4 = arg0->unique_id;
    sp34.unk8 = 0;

    if (D_8008FC30[arg1](arg0, &sp34.unk18) == 0) {
        sp34.unk18 = *(struct vec3_151D0F60 *)&D_800A5480;
    }

    sp34.unkC = sp34.unk18;
    sp34.unk24 = *(struct vec3_151D0F60 *)&D_800A5480;
    sp34.unk30 = 0.0f;
    sp34.unk34 = arg1;

    temp_v0 = func_15149130(0x12C, -1, 0x5F, -1, 0, 0x48, (struct37 *)0x38, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0x38);
    }
}

s32 func_151D1074(f32 *arg0, f32 *arg1) {
    arg1[0] = arg0[5];
    arg1[1] = arg0[96] + 8.0f;
    arg1[2] = arg0[7];
    return 1;
}

s32 func_151D10E4(void *arg0, s32 arg1, u8 arg2);

s32 func_151D10A4(void *arg0, s32 arg1) {
    return func_151D10E4(arg0, arg1, 0);
}

s32 func_151D10C4(void *arg0, s32 arg1) {
    return func_151D10E4(arg0, arg1, 1);
}

extern struct17 D_800AAF9C[];

s32 func_151D10E4(void *arg0, s32 arg1, u8 arg2) {
    s32 temp = (s32)((struct127 *)arg0)->unk1D4;

    if (temp == 0) {
        return 0;
    }
    func_15143134(&D_800AAF9C[arg2], (struct17 *)arg1, (struct255 *)(temp | 0));
    return 1;
}

typedef struct {
    struct127 *unk0;
    u8 unk4;
    u8 pad5[3];
    s32 unk8;
    struct17 unkC;
    struct17 unk18;
    struct17 unk24;
    f32 unk30;
    u8 unk34;
} Sub151D1138;

extern f32 func_15143E64(struct17 *);
extern s32 func_1510F8CC(s32);
void func_151D13E0(struct260 *arg0);
void func_151D1448();

void func_151D1138(struct260 *arg0) {
    Sub151D1138 *sub;
    struct127 *obj;
    u8 finished = 0;
    s32 state;

    sub = (Sub151D1138 *)((u8 *)arg0 + 0x28);
    obj = sub->unk0;

    if (obj->interaction_state == 0 || obj->unique_id != sub->unk4) {
        arg0->unkE = -1;
        return;
    }

    sub->unkC = sub->unk18;
    if (D_8008FC30[sub->unk34](obj, &sub->unk18) == 0) {
        sub->unk18 = sub->unkC;
        finished = 1;
    }

    sub->unk24.unk0 = sub->unk18.unk0 - sub->unkC.unk0;
    sub->unk24.unk4 = sub->unk18.unk4 - sub->unkC.unk4;
    sub->unk24.unk8 = sub->unk18.unk8 - sub->unkC.unk8;
    sub->unk30 = func_15143E64(&sub->unk24);

    state = func_1510F8CC(obj->unk184);
    if (sub->unk30 > 300.0f || sub->unk30 <= 0.0f || obj->unk28 != 0.0f || obj->in_water != 0 ||
        state == 5 || state == 6 || state == 9 || state == 0xD || state == 0xE) {
        finished = 1;
    }

    if (finished != 0) {
        func_151D13E0(arg0);
    } else if (sub->unk8 == 0) {
        func_151D1448(arg0);
    }
}

void func_151D1328(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

void func_151D13E0(struct260 *arg0);

void func_151D1368(struct260 *arg0) {
    func_151D13E0(arg0);
}

void func_151D1388(struct260 *arg0) {
    func_151D1368(arg0);
    func_1514933C(arg0);
}

void func_15149368(struct260 *arg0);

void func_151D13B4(struct260 *arg0) {
    func_151D1368(arg0);
    func_15149368(arg0);
}

struct Obj151D13E0 {
    char pad0[0x1C];
    s16 unk1C;
    u16 unk1E;
    char pad20[0x10];
    u8 unk30;
    char pad31[0x67];
    s32 *unk98;
};

struct Slot151D13E0 {
    char pad0[0x8];
    struct Obj151D13E0 *unk8;
};

void func_151D13E0(struct260 *arg0) {
    struct Slot151D13E0 *slot = (struct Slot151D13E0 *)((char *)arg0 + 0x28);
    struct Obj151D13E0 *obj;
    s32 *p;

    if (slot->unk8 != 0) {
        p = slot->unk8->unk98;
        slot->unk8->unk30 = 0;
        slot->unk8->unk1E &= ~0x2;
        slot->unk8->unk1E |= 0x8;
        slot->unk8->unk1E |= 0x1;
        slot->unk8->unk1C = 0x28;
        *p = 0;
        slot->unk8 = 0;
    }
}

void func_151D1448(u8 *arg0) {
    u8 *link;
    struct {
        Vec3i151CD674 position;
        s16 type;
        s16 subtype;
        s32 mode;
        u8 flag;
        u8 value;
        u8 pad_16[2];
        s32 reserved;
        u8 *object;
        f32 zero;
        u8 color;
        u8 pad_25[3];
        f32 range;
        f32 width;
        f32 height;
        f32 extent;
        f32 source_value;
    } packet;
    u8 *state;
    u8 *result;

    link = arg0 + 0x28;
    state = *(u8 **)link;
    packet.object = arg0;
    packet.color = 0xFF;
    packet.zero = 0.0f;
    if (state[4] == 0x25) {
        packet.range = 800.0f;
        packet.width = 36.0f;
        packet.height = 50.0f;
        packet.extent = 200.0f;
    } else {
        if (D_80082FA0 > 0) {
            packet.range = 260.0f;
        } else {
            packet.range = 500.0f;
        }
        packet.width = 40.0f;
        packet.height = 40.0f;
        packet.extent = 180.0f;
    }
    packet.source_value = *(f32 *)(*(u8 **)link + 0x14C);
    packet.value = 0x19;
    packet.position = *(Vec3i151CD674 *)(link + 0x18);
    packet.type = 0x12C;
    packet.subtype = 0x36;
    packet.mode = 0x13;
    packet.flag = 6;
    packet.reserved = 0;
    result = (u8 *)func_15147A80(&packet, 0x20, 0x28, 0, 0x11, 0x12, 0, 0, 0, arg0[0xC], arg0[1]);
    if (result != 0) {
        memcpy(*(void **)(result + 0x98), &packet.object, 0x20);
        *(u8 **)(link + 8) = result;
    }
}
