#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

typedef struct { f32 x, y, z; } TrailVec3;
typedef struct { f32 x, z; } TrailVec2;

extern f32 D_800D9860;
extern f32 func_15144AA8(s32);
extern struct17 *func_15144B34(s32);
extern s32 func_15046C80(struct17 *, s32, f32, f32 *);
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

/* Trail actor (func_15147A80, see game_105FC0.c) drawn as a ribbon through its ring buffer of points. */
typedef struct {
    /* 0x00 */ u8 pad0[0x25];
    /* 0x25 */ u8 capacity;
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail; /* oldest point */
    /* 0x2E */ s8 head; /* next point to write */
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
Gfx *func_151CF380(RibbonActor *ribbon, Gfx *gfx, s16 cameraIdx) {
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

    if (ribbon->count < 2) {
        return gfx;
    }
    info = ribbon->info;
    points = ribbon->points;
    if (info->owner != NULL && info->owner->unk1D4 == NULL) {
        return gfx;
    }
    func_151D5D60(ribbon->vtxBuffer, cameraIdx, (ribbon->capacity * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
    update = 1;
    gfx = func_15142B7C(func_15142C10(gfx, 0xED, 0xD2, 0x85, 0xC8, &update), 0x200005, 0x1F0600);
    gfx = func_15142FBC(func_1513F4E4(gfx, 0x54, &update), D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[1].cycle1 | D_800A4AC8[1].cycle2, &update);
    cur = ribbon->head - 1;
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
        if (cur != ribbon->tail) {
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
    } while (cur != ribbon->tail);
    return gfx;
}

/* Data of a trail actor that follows an owner (copied in at spawn by func_151D1448). */
typedef struct {
    /* 0x00 */ void *owner; /* NULL once the owner lets go of the trail */
    /* 0x04 */ f32 unk4;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 scale;
} FollowerData;

/* Trail actor created by func_15147A80 (see game_105FC0.c's Trail). */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u16 flags;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 pad31[0x98 - 0x31];
    /* 0x98 */ FollowerData *data;
} FollowerActor;

/* Message handler of a following actor: lets func_15169850 update or end the link to its owner. */
void func_151CF844(FollowerActor *actor, s32 arg1, u8 arg2) {
    FollowerData *data = actor->data;

    if (data->owner != NULL) {
        func_15169850(arg1, arg2, (s32)&data->owner, (s32)&data->unk4, (s32)actor);
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
void func_151CF898(struct127 *obj, f32 probeY, f32 arg2) {
    Camera *player;
    struct17 spawnPos;
    struct17 *camPos;
    f32 yaw;
    f32 angle;
    f32 distance;
    GroundEffectData data;
    GroundEffect *effect;

    player = obj->camera;
    if (player != NULL) {
        if (func_150ADA68() < 0.2220000178f) {
            camPos = func_15144B34(player->playerIndex);
            yaw = func_15144AA8(player->playerIndex);
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
                                                       sizeof(data), 0xFF, 1);
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

/* matching: the first 0x78 bytes of ParticleSpawn; a full ParticleSpawn on the stack changes the frame. */
typedef struct ParticleSpawn78 {
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
    /* 0x59 */ u8 pad59[3];
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
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
} ParticleSpawn78;

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

/* What lies under an object, filled in by func_1504715C. */
typedef struct {
    u8 unk0[0x24];
} FloorInfo;

extern struct17 D_800AAEEC[];
// matching: the model goes in as s32; a struct255 * argument changes the temp registers
void func_1503F404(s32, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
void func_15143134(struct17 *, struct17 *, Mtx *);
void func_15143794(s16, s16, f32, struct17 *);
s32 func_15132A4C(ParticleSpawn78 *, s32, s32, s32, u8, s32);
void func_15150D1C(Spawn15150D1C *, u8, s32);
void func_151A9834(struct17 *, f32, f32, FloorInfo *, s32, s32, s8 *, u8, s32);
void func_15151A38(Spawn15151A38 *, u8, s32);
void func_1504715C(FloorInfo *, struct127 *);

/* Breaks the object apart: seven model pieces fly off its model's points with random velocity and
 * spin, plus three more effects at 200 units above it. */
void func_151CFA4C(struct127 *obj, u8 arg1, s32 arg2) {
    vertex vecB;
    struct17 pos;
    struct17 vecA;
    struct17 vecC;
    u8 i;

    pos.unk0 = obj->x_position;
    pos.unk4 = obj->y_position + 200.0f;
    pos.unk8 = obj->z_position;
    if (obj->unk1D4) {
        func_1503F404((s32)obj->unk1D4, &vecA.unk0, &vecA.unk4, &vecA.unk8, &vecB.x, &vecB.y, &vecB.z,
                      &vecC.unk0, &vecC.unk4, &vecC.unk8);
        {
            ParticleSpawn78 piece;
            const s32 models[7] = { 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75 };

            piece.radius = 1.0f;
            piece.elasticity = 0.6490000486f;
            piece.sizeXZ = piece.sizeY = (obj->xz_scale + obj->y_scale) * 0.5f;
            piece.rot = vecB;
            piece.scale.x = 1.0f;
            piece.scale.y = 0.0f;
            piece.scale.z = 1.0f;
            piece.rotVel.y = 0.0f;
            piece.variant = 0;
            piece.unk5C = 0;
            piece.alpha = 0xFF;
            piece.update2Fn = 0;
            piece.floorType3Fn = 0;
            piece.wallHitFn = 0;
            piece.destroyFn = 0;
            piece.squashFn = 0;
            piece.unk6A = 1;
            piece.owner = obj;
            piece.ownerId = obj->unique_id;
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
                func_15143134(&D_800AAEEC[i], &piece.pos, (Mtx *)obj->unk1D4);
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794(yaw & 0xFF, (pitch % 25U) - 64, (func_150ADA68() * 11.44999981f) + 16.93000031f, &piece.vel);
                piece.rotVel.x = (func_150ADA68() * 19.75200081f) + -9.876000404f;
                piece.rotVel.z = (func_150ADA68() * 19.75200081f) + -9.876000404f;
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
            FloorInfo floor;
            s8 flag;
            s32 pad; // matching: unused stack slot in the original frame

            func_1504715C(&floor, obj);
            flag = 2;
            func_151A9834(&pos, pos.unk4 - 2000.0f, 200.0f, &floor, (func_150ADA20() & 3) + 7, 0, &flag, arg1, arg2);
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

    effect = (StripEffect *)func_15149130(300, -1, 0x61, 4, 0, 0x31, sizeof(data), arg2, arg3);
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
Gfx *func_151D014C(Gfx *gfx, StripEffect *effect, s16 cameraIdx) {
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
    func_151D5D60(effect->vtxBuffer, cameraIdx, 0x1C0, (void **)&vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    update = 1;
    func_15143134(&D_800AAF5C[data->variant], &from, (Mtx *)owner->unk1D4 + 2);
    func_15143134(&D_800AAF74, &to, (Mtx *)owner->unk1D4 + D_800AAF80[data->variant]);
    camPos = func_15144B34(cameraIdx);
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

/* The owner an effect follows, kept at the start of its data. */
typedef struct {
    struct127 *owner;
    u8 ownerId; /* owner->unique_id at spawn; a mismatch means the owner died */
} OwnerRef;

typedef struct {
    EffectHead head;
    OwnerRef ref;
} OwnedEffect;

/* Message handler of the effects that follow an owner: frees the effect when its owner is gone
 * (messages 0 and 0x18) and follows the owner through a swap (0x2D). */
void func_151D08F0(OwnedEffect *effect, void *msg, u8 type) {
    OwnerRef *ref = &effect->ref;

    if (type == 0 || type == 0x18) {
        OwnerGoneMsg *gone = msg;
        struct127 *obj = gone->obj;

        if (obj == ref->owner || gone->objId == ref->ownerId) {
            func_1516972C((struct102 *)effect);
        }
    } else if (type == 0x2D) {
        OwnerSwapMsg *swap = msg;

        if (ref->owner == swap->a) {
            ref->owner = swap->b;
            ref->ownerId = swap->bId;
        } else if (ref->owner == swap->b) {
            ref->owner = swap->a;
            ref->ownerId = swap->aId;
        }
    }
}

/* Emission shape shared by func_1514FF44, func_15153CCC, func_1514F6E8 and func_1514F808. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x04 */ struct17 dir;
    /* 0x10 */ struct17 unk10;
    /* 0x1C */ struct17 unk1C;
    /* 0x28 */ f32 unk28;
} EmitShape; /* 0x2C */

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
} Spawn1514FF44; /* 0x40 */

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
} Spawn15153CCC; /* 0x44 */

extern f32 D_800AAF84[3];
extern f32 D_800AAF90[3];
extern void func_15145EA4(f32 **, struct17 **, Mtx *, s32);
extern void func_1514FF44(EmitShape *, Spawn1514FF44 *, FloorInfo *, u8, s32);
extern void func_15153CCC(EmitShape *, Spawn15153CCC *, FloorInfo *, u8, s32);
extern s32 func_1514F6E8(EmitShape *);
extern void func_1514F808(EmitShape *, f32, struct17 *);
/* Partial view of the particle func_15130280 creates. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ struct260 *effect; /* effect that travels with the particle */
} CarrierParticle;

extern CarrierParticle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/* Emits two particle bursts along the line between two of the object's model points, and sometimes
 * 5-10 more particles that each carry a func_151CEAAC effect. */
void func_151D09A8(struct127 *obj, u8 arg1, s32 arg2) {
    vertex pos;
    struct17 dir;
    FloorInfo floor;
    EmitShape shape;

    if (obj->unk1D4 != NULL) {
        {
            f32 *srcs[2];
            struct17 *dsts[2];

            srcs[0] = D_800AAF84;
            srcs[1] = D_800AAF90;
            dsts[0] = &pos;
            dsts[1] = &dir;
            func_15145EA4(srcs, dsts, (Mtx *)obj->unk1D4 + 2, 2);
            dir.unk0 -= pos.x;
            dir.unk4 -= pos.y;
            dir.unk8 -= pos.z;
        }
        func_1504715C(&floor, obj);
        shape.unk0 = 0;
        shape.dir = dir;
        /* D_800A5480 (an f32 in variables.h) starts a zero vector. */
        shape.unk10 = shape.unk1C = *(struct17 *)&D_800A5480;
        {
            Spawn1514FF44 emitA;

            emitA.unk0 = *(struct17 *)&pos; // TODO(vectors): struct17/vertex mix
            emitA.unkC = 6;
            emitA.unkE = 8;
            emitA.unk10 = 0.03999999911f;
            emitA.unk14 = 0.02999999933f;
            emitA.unk18 = 30;
            emitA.unk1A = 30;
            emitA.unk1C = -1.467000127f;
            emitA.unk20 = 0.756000042f;
            emitA.unk24 = 155;
            emitA.unk25 = 100;
            emitA.unk28 = 403.0f;
            emitA.unk2C = 455.0f;
            emitA.unk30 = 1;
            emitA.unk31 = 2;
            emitA.unk34 = 0.0f;
            emitA.unk38 = 1;
            emitA.unk3C = 0.0f;
            shape.unk28 = 750.0f;
            func_1514FF44(&shape, &emitA, &floor, arg1, arg2);
        }
        {
            Spawn15153CCC emitB;

            emitB.unk0 = *(struct17 *)&pos; // TODO(vectors): struct17/vertex mix
            emitB.unkC = 10.0f;
            emitB.unk10 = 25.0f;
            emitB.unk14 = 1.253000021f;
            emitB.unk18 = 0.8230000138f;
            emitB.unk1C = 0.03500000015f;
            emitB.unk20 = 0.03199999779f;
            emitB.unk24 = 15;
            emitB.unk26 = 8;
            emitB.unk28 = 3;
            emitB.unk2A = 2;
            emitB.unk2C = 30;
            emitB.unk2E = 25;
            emitB.unk30 = 155;
            emitB.unk32 = 100;
            emitB.unk38 = 0.0f;
            emitB.unk3C = 16;
            emitB.unk3E = 15;
            emitB.unk40 = 0;
            emitB.unk34 = 2;
            shape.unk28 = 750.0f;
            func_15153CCC(&shape, &emitB, &floor, arg1, arg2);
        }
        if (func_1514F6E8(&shape) != 0) {
            s32 count;
            ParticleDesc spawn;
            CarrierParticle *particle;
            s32 pad; // matching: unused stack slot in the original frame

            shape.unk28 = 1000.0f;
            count = (func_150ADA20() % 6U) + 5;
            spawn.texture = 0x3E;
            spawn.renderMode = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.r = 0;
            spawn.g = 0;
            spawn.b = 0;
            spawn.a = 0xFF;
            spawn.r2 = 0;
            spawn.g2 = 0;
            spawn.b2 = 0;
            spawn.alpha = 0;
            spawn.brightness = 0xFF;
            spawn.pos = pos;
            spawn.offset = *(vertex *)&D_800A5480;
            spawn.scaleStart = 1;
            spawn.flags = 0xC007;
            spawn.scaleRate = 1.0f;
            spawn.timer = (func_150ADA20() % 11U) + 40;
            spawn.fadeStart = 1;
            spawn.fadeRate = 0xFF;
            spawn.primColorMode = 6;
            spawn.envColorMode = 8;
            spawn.drawFunc = -1;
            spawn.updateFunc = -1;
            spawn.deathFunc = -1;
            spawn.handlerSet = 11;
            spawn.viewMask = 0;
            spawn.views = 0xFF;
            spawn.minSize = 0;
            spawn.height = 0.0f;
            spawn.width = 0.0f;
            spawn.maxSize = 10000.0f;
            while (count != 0) {
                func_1514F808(&shape, (func_150ADA68() * 0.03999999911f) + 0.04999999702f, &spawn.vel);
                spawn.gravity = (func_150ADA68() * 1.340000033f) + -2.328999996f;
                particle = func_15130280(&spawn, 1, 0, 4, arg1, arg2);
                if (particle != NULL) {
                    particle->effect = func_151CEAAC(NULL, particle, 0, arg1, arg2);
                }
                count--;
            }
        }
    }
}

/* Frees the effect that travels with the particle. */
void func_151D0ED8(CarrierParticle *particle) {
    if (particle->effect != NULL) {
        // matching: a volatile read makes IDO reload the pointer for the call, as the original does
        func_1516972C(*(struct102 *volatile *)&particle->effect);
    }
}

void func_1513173C(CarrierParticle *particle);
void func_1513175C(CarrierParticle *particle);

/* Particle handler: frees the attached effect, then runs func_1513173C. */
void func_151D0F08(CarrierParticle *particle) {
    func_151D0ED8(particle);
    func_1513173C(particle);
}

/* Particle handler: frees the attached effect, then runs func_1513175C. */
void func_151D0F34(CarrierParticle *particle) {
    func_151D0ED8(particle);
    func_1513175C(particle);
}

/* Point functions: each writes a point of the object to out and returns whether it has one. */
extern s32 (*D_8008FC30[])(struct127 *obj, struct17 *out);

/* Data of the tracker effect: follows one point of its owner and leaves a trail behind it. */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ FollowerActor *trail; /* spawned by func_151D1448 */
    /* 0x0C */ struct17 prevPos;
    /* 0x18 */ struct17 pos;
    /* 0x24 */ struct17 delta;
    /* 0x30 */ f32 dist; /* distance moved this frame */
    /* 0x34 */ u8 pointFn; /* index into D_8008FC30 */
} TrackerData;

typedef struct {
    EffectHead head;
    TrackerData data;
} TrackerEffect;

/* Spawns a tracker effect that follows the object's point D_8008FC30[pointFn]. */
void func_151D0F60(struct127 *obj, u8 pointFn, u8 arg2, s32 arg3) {
    TrackerEffect *effect;
    TrackerData data;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.trail = NULL;

    if (D_8008FC30[pointFn](obj, &data.pos) == 0) {
        data.pos = *(struct17 *)&D_800A5480;
    }

    data.prevPos = data.pos;
    data.delta = *(struct17 *)&D_800A5480;
    data.dist = 0.0f;
    data.pointFn = pointFn;

    effect = (TrackerEffect *)func_15149130(300, -1, 0x5F, -1, 0, 0x48, sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Point function: 8 units above the object's unk180 height, at its x/z. */
s32 func_151D1074(struct127 *obj, struct17 *out) {
    out->unk0 = obj->x_position;
    out->unk4 = obj->unk180 + 8.0f;
    out->unk8 = obj->z_position;
    return TRUE;
}

s32 func_151D10E4(struct127 *obj, struct17 *out, u8 which);

/* Point function: the object's model point 0. */
s32 func_151D10A4(struct127 *obj, struct17 *out) {
    return func_151D10E4(obj, out, 0);
}

/* Point function: the object's model point 1. */
s32 func_151D10C4(struct127 *obj, struct17 *out) {
    return func_151D10E4(obj, out, 1);
}

extern struct17 D_800AAF9C[];

/* Writes model point D_800AAF9C[which] of the object, in world space, to out. */
s32 func_151D10E4(struct127 *obj, struct17 *out, u8 which) {
    s32 joints = (s32)obj->unk1D4;

    if (joints == 0) {
        return FALSE;
    }
    // matching: "| 0" keeps joints from being a plain copy, so IDO gives it v0 and which a3
    func_15143134(&D_800AAF9C[which], out, (Mtx *)(joints | 0));
    return TRUE;
}

extern s32 func_1510F8CC(s32);
void func_151D13E0(TrackerEffect *effect);
void func_151D1448(TrackerEffect *effect);

/* Tracker update: follows the owner's point, and drops the trail when the point is lost, jumps more
 * than 300 units, stops, or the owner is airborne, in water or in state 5, 6, 9, 13 or 14. */
void func_151D1138(TrackerEffect *effect) {
    TrackerData *data;
    struct127 *owner;
    u8 finished = FALSE;
    s32 state;

    data = &effect->data;
    owner = data->owner;

    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->head.state = -1;
        return;
    }

    data->prevPos = data->pos;
    if (D_8008FC30[data->pointFn](owner, &data->pos) == 0) {
        data->pos = data->prevPos;
        finished = TRUE;
    }

    data->delta.unk0 = data->pos.unk0 - data->prevPos.unk0;
    data->delta.unk4 = data->pos.unk4 - data->prevPos.unk4;
    data->delta.unk8 = data->pos.unk8 - data->prevPos.unk8;
    data->dist = func_15143E64(&data->delta);

    state = func_1510F8CC(owner->unk184);
    if (data->dist > 300.0f || data->dist <= 0.0f || owner->unk28 != 0.0f || owner->in_water != 0 ||
        state == 5 || state == 6 || state == 9 || state == 13 || state == 14) {
        finished = TRUE;
    }

    if (finished) {
        func_151D13E0(effect);
    } else if (data->trail == NULL) {
        func_151D1448(effect);
    }
}

/* Message handler of the tracker effect: lets func_15169850 update or end the link to its owner. */
void func_151D1328(TrackerEffect *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, (s32)&effect->data.owner, (s32)&effect->data.ownerId, (s32)effect);
}

/* Tracker teardown: lets go of the trail. */
void func_151D1368(TrackerEffect *effect) {
    func_151D13E0(effect);
}

void func_151D1388(TrackerEffect *effect) {
    func_151D1368(effect);
    func_1514933C((struct260 *)effect);
}

void func_151D13B4(TrackerEffect *effect) {
    func_151D1368(effect);
    func_15149368((struct260 *)effect);
}

/* Lets go of the tracker's trail, which then fades out on its own. */
void func_151D13E0(TrackerEffect *effect) {
    TrackerData *data = &effect->data;
    FollowerData *trailData;

    if (data->trail != NULL) {
        trailData = data->trail->data;
        data->trail->unk30 = 0;
        data->trail->flags &= ~0x2;
        data->trail->flags |= 0x8;
        data->trail->flags |= 0x1;
        data->trail->unk1C = 40;
        trailData->owner = NULL;
        data->trail = NULL;
    }
}

/* Spawn description passed to func_15147A80 (see game_105FC0.c's TrailSpawn). */
typedef struct {
    /* 0x00 */ struct17 pos;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 unk18;
} TrailSpawn;

extern FollowerActor *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);

/* Spawns the trail that follows the tracker effect. */
void func_151D1448(TrackerEffect *effect) {
    TrackerData *data;
    struct {
        TrailSpawn spawn;
        FollowerData data;
    } req;
    struct127 *owner;
    FollowerActor *trail;

    data = &effect->data;
    owner = data->owner;
    req.data.owner = effect;
    req.data.unk8 = 0xFF;
    req.data.unk4 = 0.0f;
    if (owner->id == 0x25) {
        req.data.unkC = 800.0f;
        req.data.unk10 = 36.0f;
        req.data.unk14 = 50.0f;
        req.data.unk18 = 200.0f;
    } else {
        if (D_80082FA0 > 0) {
            req.data.unkC = 260.0f;
        } else {
            req.data.unkC = 500.0f;
        }
        req.data.unk10 = 40.0f;
        req.data.unk14 = 40.0f;
        req.data.unk18 = 180.0f;
    }
    req.data.scale = data->owner->xz_scale;
    req.spawn.unk15 = 25;
    req.spawn.pos = data->pos;
    req.spawn.unkC = 300;
    req.spawn.unkE = 54;
    req.spawn.unk10 = 19;
    req.spawn.unk14 = 6;
    req.spawn.unk18 = 0;
    trail = func_15147A80(&req.spawn, sizeof(FollowerData), 0x28, 0, 0x11, 0x12, 0, 0, 0, effect->head.unkC, effect->head.unk1);
    if (trail != NULL) {
        memcpy(trail->data, &req.data, sizeof(FollowerData));
        data->trail = trail;
    }
}
