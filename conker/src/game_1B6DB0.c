#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Link fields shared by every object (see game_1944C0.c). */
typedef struct ObjLink {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 list;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ struct ObjLink *prev;
    /* 0x8 */ struct ObjLink *next;
    /* 0xC */ u8 owner;
} ObjLink;

/* Trail types (see game_175250.c). */

/* Spawn description passed to func_15147DA0 (first argument). */
typedef struct {
    Vec3f pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 pad14;
    u8 pointCount;
    u8 pad16[0xA];
} TrailSpawn;

/* One point of a trail; the points form a ring buffer. */
typedef struct {
    Vec3f pos;
    f32 velocityY;
    u16 phase; /* texture coordinate along the trail */
    u8 pad12[2];
} TrailPoint;

/* Motion and appearance block that func_15147DA0 copies from its second argument. */
typedef struct {
    f32 halfWidth;
    Vec3f velocity;
    f32 gravity;
    f32 unk14;
    u8 flags;
    u8 texture;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    s16 unk1E;
} TrailMotion; /* size 0x20 */

/* Render settings that func_15147DA0 copies from its 12th argument. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 renderMode;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
} TrailRender; /* size 0x20 */

typedef struct {
    TrailMotion motion;
    u8 state; /* 0x20: handler index */
    u8 unk21;
    u8 finishFn;
    u8 hitFn;
    u8 hitKind3Fn;
    u8 unk25;
    u8 pad26[2];
    TrailRender render; /* 0x28 */
} TrailParams; /* size 0x48 */

/* Params of a trail spawned by a SprayEmitter: the TrailParams, then the floor height. */
typedef struct {
    /* 0x00 */ TrailParams base;
    /* 0x48 */ f32 floorY;
} SprayTrailParams;

typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ u8 pad10[0x1D];
    /* 0x2D */ s8 start; /* oldest point */
    /* 0x2E */ u8 pad2E[0x66];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ SprayTrailParams *params;
} Trail;

/* Spray description that func_15189900 copies into a new SprayEmitter (0x50 bytes).
   Angles are in 256ths of a turn; every "...Range" is the random spread added to the value before it. */
typedef struct {
    /* 0x00 */ s16 yaw;
    /* 0x02 */ s16 yawSpread; /* yaw varies by +-yawSpread */
    /* 0x04 */ s16 pitch;
    /* 0x06 */ s16 pitchRange;
    /* 0x08 */ Vec3f origin;
    /* 0x14 */ Vec3f extent; /* particles start on the segment origin + t * extent */
    /* 0x20 */ f32 speed;
    /* 0x24 */ f32 speedRange;
    /* 0x28 */ f32 rate; /* particles per frame */
    /* 0x2C */ f32 rateRange;
    /* 0x30 */ f32 halfWidth;
    /* 0x34 */ f32 halfWidthRange;
    /* 0x38 */ s16 unk1B; /* TrailMotion.unk1B */
    /* 0x3A */ s16 unk1BRange;
    /* 0x3C */ f32 gravity;
    /* 0x40 */ f32 gravityRange;
    /* 0x44 */ s16 unkC; /* TrailSpawn.unkC */
    /* 0x46 */ s16 unkCRange;
    /* 0x48 */ s32 pointCount;
    /* 0x4C */ s32 pointCountRange;
} SprayDesc;

typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ SprayDesc desc;
    /* 0x60 */ s16 idleTimer; /* frames until the next check for a nearby player */
    /* 0x62 */ u8 pad62[2];
    /* 0x64 */ f32 pending; /* particles owed, accumulated by rate */
    /* 0x68 */ f32 floorY;  /* lowest point of the spawn segment */
    /* 0x6C */ s32 unk6C;   /* from func_1510FD20 at the segment's midpoint */
    /* 0x70 */ u8 flags;
    /* 0x71 */ u8 pad71[7];
} SprayEmitter; /* size 0x78 */

extern SprayEmitter *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_1510F800(s32);
extern s32 func_151464B8(s32);
extern Trail *func_15147DA0(TrailSpawn *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4,
                            s32 finishFn, s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10,
                            TrailRender *render, s32 arg12, s32 arg13, s32 arg14);
extern u8 D_800BE630[];
extern s32 func_151D8E20(void);

/* Creates a spray emitter from desc; bit 0 of its flags records whether arg1 was set. */
void func_15189900(SprayDesc *desc, u8 arg1) {
    SprayEmitter *emitter;
    s32 flag;

    emitter = func_15167A68(0x1A, 1, sizeof(SprayEmitter), 1, 0xFF, 1);
    if (emitter != NULL) {
        memcpy(&emitter->desc, desc, sizeof(SprayDesc));
        func_1510F800(0);
        emitter->unk6C = func_1510FD20((s32)(emitter->desc.origin.x + (emitter->desc.extent.x * 0.5f)),
                                       (s32)(emitter->desc.origin.z + (emitter->desc.extent.z * 0.5f)));
        if (arg1 != 0) {
            flag = 1;
        } else {
            flag = 0;
        }
        emitter->flags = flag;
        emitter->idleTimer = 0;
        emitter->pending = 0.0f;
        if (emitter->desc.extent.y < 0.0f) {
            emitter->floorY = emitter->desc.origin.y + emitter->desc.extent.y;
        } else {
            emitter->floorY = emitter->desc.origin.y;
        }
    }
}

/* Spray emitter update: while a player is within 1581 units, spawns desc.rate particles per frame. */
void func_15189A00(SprayEmitter *emitter) {
    s32 i;
    s32 found;
    f32 *playerPos;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 player;
    TrailSpawn spawn;
    TrailMotion motion;
    f32 floorY;
    TrailRender render;
    Trail *trail;
    u8 *dst;

    if ((emitter->unk6C != 0) && (func_151464B8(emitter->unk6C) != 0)) {
        return;
    }
    i = D_80082FA0; // matching: dead store makes uopt color the loop bound before the timer reload
    emitter->idleTimer -= D_800BE9E4;
    if (emitter->idleTimer < 0) {
        emitter->idleTimer += 50;
        found = FALSE;
        for (i = 0; (i <= D_80082FA0) && !found; i++) {
            player = D_800BE630[i];
            playerPos = &D_800DBFF0[player].unk2F8;
            dx = playerPos[0] - emitter->desc.origin.x;
            dy = playerPos[1] - emitter->desc.origin.y;
            dz = playerPos[2] - emitter->desc.origin.z;
            if (((dx * dx) + (dy * dy) + (dz * dz)) < 2500000.0f) {
                found = TRUE;
            }
        }
        if (!found) {
            return;
        }
    }
    emitter->pending += (emitter->desc.rate + (func_150ADA68() * emitter->desc.rateRange)) * D_800BE9A4;
    if (emitter->pending > 1.0f) {
        floorY = emitter->floorY;
        spawn.unk10 = 1;
        spawn.unkE = 5;
        motion.unk1C = 16;
        motion.unk1E = 15;
        motion.unk14 = -10000.0f;
        motion.flags = 0x48;
        motion.texture = 3;
        motion.unk1A = 0xFF;
        render.unk0 = 0;
        render.unk4 = 1;
        render.unk8 = 0x160600;
        render.renderMode = 3;
        render.unk10 = 0x22;
        render.unk14 = 0x80;
        render.unk18 = 0x20;
        render.unk1C = 0;
        render.unk1D = 7;
        do {
            spawn.pointCount = (func_150ADA20() % (u32)(emitter->desc.pointCountRange + 1)) + emitter->desc.pointCount;
            {
                f32 t;
                t = func_150ADA68();
                spawn.pos.x = emitter->desc.origin.x + (t * emitter->desc.extent.x);
                spawn.pos.y = emitter->desc.origin.y + (t * emitter->desc.extent.y);
                spawn.pos.z = emitter->desc.origin.z + (t * emitter->desc.extent.z);
            }
            motion.halfWidth = (func_150ADA68() * emitter->desc.halfWidthRange) + emitter->desc.halfWidth;
            {
                f32 speed;
                s16 yaw;
                s16 pitch;
                f32 sinYaw;
                f32 cosYaw;
                f32 sinPitch;
                f32 cosPitch;
                yaw = emitter->desc.yaw - (func_150ADA20() % (u32)((emitter->desc.yawSpread * 2) + 1)) - -emitter->desc.yawSpread;
                pitch = (func_150ADA20() % (u32)(emitter->desc.pitchRange + 1)) + emitter->desc.pitch;
                speed = (func_150ADA68() * emitter->desc.speedRange) + emitter->desc.speed;
                sinYaw = func_151423D8((u8)yaw);
                cosYaw = func_151423D8((u8)(yaw - 0x40));
                sinPitch = func_151423D8((u8)pitch);
                cosPitch = func_151423D8((u8)(pitch - 0x40));
                motion.velocity.x = (speed * sinPitch) * cosYaw;
                motion.velocity.y = -speed * cosPitch;
                motion.velocity.z = (speed * sinPitch) * sinYaw;
            }
            motion.gravity = (func_150ADA68() * emitter->desc.gravityRange) + emitter->desc.gravity;
            motion.unk1B = (func_150ADA20() % (u32)(emitter->desc.unk1BRange + 1)) + emitter->desc.unk1B;
            spawn.unkC = (func_150ADA20() % (u32)(emitter->desc.unkCRange + 1)) + emitter->desc.unkC;
            trail = func_15147DA0(&spawn, &motion, 8, 1, (emitter->flags & 1) ? 10 : 0, 0, 0, 0, 0, 0, 0, &render, 0,
                                  emitter->link.owner, emitter->link.list);
            if (trail != NULL) {
                dst = (u8 *)trail->params;
                memcpy(dst + 0x48, &floorY, 4);
            }
            emitter->pending -= 1.0f;
        } while (emitter->pending > 1.0f);
    }
}

/* Spray trail handler: once the oldest point falls below the floor, maybe splashes there, then ends the trail. */
s32 func_15189EBC(Trail *trail) {
    f64 chance;
    SprayTrailParams *params = trail->params;
    TrailPoint *points = trail->points;
    Vec3f splashPos;

    if (points[trail->start].pos.y < params->floorY) {
        chance = func_150ADA68();
        if (chance < 0.8) {
            splashPos.x = points[trail->start].pos.x;
            splashPos.y = params->floorY;
            splashPos.z = points[trail->start].pos.z;
            func_151DBCBC((u8)func_151D8E20(), params->base.motion.halfWidth * 6.0f, params->base.motion.unk1B, 0,
                          &splashPos, trail->link.owner, trail->link.list);
        }
        params->base.state = 4;
        params->floorY = -10000.0f;
    }
    return 1;
}

s32 func_15189FD0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *p = &arg0;
    return 1;
}
