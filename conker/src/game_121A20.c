#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"


/* Flags of SpiralState. */
#define SPIRAL_FADE_NEAR 1 /* fade out as the camera gets close */
#define SPIRAL_SOUND 2     /* plays a positional sound */
#define SPIRAL_MUTED 4     /* sound no longer updated or restarted */

/* Per-object state of the effect spawned by func_150F4570: it circles a centre that moves
 * from one point towards another (updated by func_150F48D0). */
typedef struct {
    /* 0x00 */ f32 timeLeft;
    /* 0x04 */ Vec3f center;
    /* 0x10 */ f32 phase; /* radians */
    /* 0x14 */ f32 phaseRate;
    /* 0x18 */ f32 cosDir;
    /* 0x1C */ f32 sinDir;
    /* 0x20 */ f32 radius;
    /* 0x24 */ u8 flags; /* SPIRAL_* */
    /* 0x25 */ u8 pad25[3];
} SpiralState;

/* An object spawned by func_1513264C with a SpiralState. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x10];
    /* 0x020 */ Vec3f rot; /* degrees */
    /* 0x02C */ u8 pad2C[0xC];
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ Vec3f spin;
    /* 0x05C */ u8 pad5C[0x14];
    /* 0x070 */ u8 alpha;
    /* 0x071 */ u8 unk71;
    /* 0x072 */ u8 pad72[0x16];
    /* 0x088 */ u32 sound; /* sound id in the low half, playing handle in the high half */
    /* 0x08C */ u8 pad8C[0xE4];
    /* 0x170 */ SpiralState spiral;
} EffectObj;

extern s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);
extern EffectObj *func_1513264C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 arg3, s32 extraSize, u8 owner, s32 kind);

/* Spawns an effect that spirals from `from` to `to` at `speed` (invSpeed = 1 / speed), playing
 * `sound` near the camera when it is non-zero. */
void func_150F4570(Vec3f *from, Vec3f *to, u8 id, f32 speed, f32 invSpeed, f32 scale, u8 fadeNear, s32 sound,
                   u8 owner, s32 kind) {
    ParticleSpawn spawn;
    Vec3f dir;
    Vec3f delta;
    f32 length;
    f32 invLength;
    SpiralState spiral;
    EffectObj *obj;

    delta.x = to->x - from->x;
    delta.y = to->y - from->y;
    delta.z = to->z - from->z;
    if (func_15145128(&delta, &dir, &length, &invLength) != 0) {
        spiral.flags = (fadeNear ? SPIRAL_FADE_NEAR : 0) | (sound ? SPIRAL_SOUND : 0);
        spiral.timeLeft = length * invSpeed;
        spiral.center = *from;
        spiral.phase = func_150ADA68() * 6.2831855f;
        spiral.phaseRate = func_150ADA68() * 0.2f + 0.08000000566f;
        if (func_150ADA20() & 1) {
            spiral.phaseRate = -spiral.phaseRate;
        }
        spiral.radius = func_150ADA68() * 21.0f + 13.0f;
        {
            f32 angle = func_150ADA68() * 6.2831855f;
            spiral.cosDir = cosf(angle);
            spiral.sinDir = sinf(angle);
        }
        spawn.radius = 1.0f;
        spawn.elasticity = 1.0f;
        spawn.sizeXZ = spawn.sizeY = scale;
        spawn.rot.x = func_150ADA68() * 360.0f;
        spawn.rot.y = func_150ADA68() * 360.0f;
        spawn.rot.z = func_150ADA68() * 360.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.pos = *from;
        spawn.vel.x = dir.x * speed;
        spawn.vel.y = dir.y * speed;
        spawn.vel.z = dir.z * speed;
        spawn.rotVel.x = func_150ADA68() * 21.90300179f + -10.95150089f;
        spawn.rotVel.y = 0.0f;
        spawn.rotVel.z = func_150ADA68() * 21.90300179f + -10.95150089f;
        spawn.gravity = 0.0f;
        spawn.flags = 0x14900;
        spawn.lifetime = 300;
        spawn.model = id;
        spawn.variant = 8;
        spawn.unk5C = 0;
        spawn.alpha = 255;
        spawn.updateFn = 22;
        spawn.update2Fn = 7;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 2;
        spawn.drawFn = -1;
        spawn.unk6A = 2;
        spawn.owner = 0;
        spawn.ownerId = 0;
        spawn.fadeStart = 1;
        spawn.fadeRate = 255;
        spawn.sound = sound;
        obj = func_1513264C(&spawn, 3, 0xFF, 0, sizeof(spiral), owner, kind);
        if (obj != NULL) {
            memcpy(&obj->spiral, &spiral, sizeof(spiral));
        }
    }
}

extern void func_1514373C(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3);

/* Update: moves the spiral centre along the object's velocity and puts the object on the
 * circle around it; returns 0 once the travel time is up. */
s32 func_150F48D0(EffectObj *obj) {
    SpiralState *spiral;

    spiral = &obj->spiral;
    spiral->timeLeft -= D_800BE9A4;
    if (spiral->timeLeft <= 0.0f) {
        return 0;
    }
    {
        f32 up;
        f32 side;
        func_1514373C(spiral->phase, spiral->radius, &side, &up);
        spiral->phase += spiral->phaseRate * D_800BE9A4;
        spiral->center.x += obj->vel.x * D_800BE9A4;
        spiral->center.y += obj->vel.y * D_800BE9A4;
        spiral->center.z += obj->vel.z * D_800BE9A4;
        obj->pos.x = spiral->center.x + side * spiral->cosDir;
        obj->pos.y = spiral->center.y + up;
        obj->pos.z = spiral->center.z - side * spiral->sinDir;
    }
    obj->rot.x += obj->spin.x * D_800BE9A4;
    obj->rot.y += obj->spin.y * D_800BE9A4;
    obj->rot.z += obj->spin.z * D_800BE9A4;
    // matching: the original falls off the end here without setting a return value
}

extern Vec3f *func_15144B34(s32);
extern f32 func_15143E64(Vec3f *);

/* Fades the object out near the camera and keeps its positional sound playing (started
 * within 200 units of the camera), as its SPIRAL_* flags ask; always returns 1. */
s32 func_150F4A38(EffectObj *obj) {
    SpiralState *spiral;
    f32 dist;

    spiral = &obj->spiral;
    if ((spiral->flags & SPIRAL_FADE_NEAR) || (spiral->flags & SPIRAL_SOUND)) {
        Vec3f *camera;
        Vec3f delta;

        camera = func_15144B34(0);
        delta.x = obj->pos.x - camera->x;
        delta.y = obj->pos.y - camera->y;
        delta.z = obj->pos.z - camera->z;
        dist = func_15143E64(&delta);
    }
    if (spiral->flags & SPIRAL_FADE_NEAR) {
        if (dist < 500.0f) {
            obj->alpha = 0;
        } else if (dist < 1200.0f) {
            obj->alpha = (dist - 500.0f) * (1.0 / 700.0) * 255.0;
        } else {
            obj->alpha = 255;
        }
    }
    if ((spiral->flags & SPIRAL_SOUND) && !(spiral->flags & SPIRAL_MUTED)) {
        if (obj->sound >> 16) {
            func_1000F9D4(obj->sound >> 16, obj->pos.x, obj->pos.y, obj->pos.z);
        } else if (dist < 200.0f) {
            obj->sound |= func_10010F88(obj->sound & 0xFFFF, (func_150ADA20() & 0x3FFF) + 0x4000, 0, 0, -1,
                                        obj->pos.x, obj->pos.y, obj->pos.z, 10000, 20000) << 16;
        }
    }
    return 1;
}

/* Event handler: event 0x4E clears unk71 and sets SPIRAL_FADE_NEAR | SPIRAL_MUTED, 0x4F calls
 * func_1516972C. */
void func_150F4CFC(EffectObj *obj, s32 arg1, u8 event) {
    SpiralState *spiral;
    spiral = &obj->spiral;
    if (event == 0x4E) {
        obj->unk71 = 0;
        spiral->flags |= SPIRAL_FADE_NEAR | SPIRAL_MUTED;
    } else if (event == 0x4F) {
        func_1516972C(&obj->header);
    }
}

/* Per-object state of the emitter started by func_150F4D5C (updated by func_150F4DEC). */
typedef struct {
    /* 0x00 */ Camera *camera;
    /* 0x04 */ f32 pending; /* spirals owed; one is spawned per whole unit */
    /* 0x08 */ s8 condition; /* index into D_80088B38, or -1 for none */
    /* 0x09 */ u8 variants; /* index into D_80088B20 */
    /* 0x0A */ u8 padA[2];
} CameraEmitterData;

/* The emitter effect object created by func_15149130. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ CameraEmitterData data;
} CameraEmitter;

/* Starts an emitter that spawns spirals around `camera` (see func_150F4DEC). */
void func_150F4D5C(Camera *camera, s8 condition, u8 variants, u8 owner, s32 kind) {
    CameraEmitterData data;
    CameraEmitter *emitter;

    data.camera = camera;
    data.pending = 0.0f;
    data.condition = condition;
    data.variants = variants;

    emitter = (CameraEmitter *)func_15149130(300, -1, 0x56, -1, 0, 0, sizeof(data), owner, kind);
    if (emitter != NULL) {
        memcpy(&emitter->data, &data, sizeof(data));
    }
}

/* A list of spawn variants: an id and a scale for each. */
typedef struct {
    /* 0x00 */ u8 count;
    /* 0x04 */ s16 *ids;
    /* 0x08 */ f32 *scales;
} VariantList;

extern VariantList D_80088B20[];
extern s32 (*D_80088B38[])();
extern void func_15143874(s16 angle, f32 dist, f32 *x, f32 *z);

/* Emitter update: while its condition callback (if any) holds, spawns 0.05-0.20 spirals per frame
 * (times D_800BE9A4) around the camera, each between two random points on either side of
 * its view direction. */
void func_150F4DEC(CameraEmitter *emitter) {
    CameraEmitterData *data;
    Vec3f *eye;
    s32 yaw;
    Vec3f from;
    Vec3f to;
    VariantList *variants;
    u8 i;
    f32 speed;

    data = &emitter->data;
    if (data->condition != -1 && !D_80088B38[data->condition]()) {
        return;
    }
    data->pending += (0.05100000277f + func_150ADA68() * 0.153000012f) * D_800BE9A4;
    if (data->pending > 1.0f) {
        eye = func_15144B34(data->camera->playerIndex);
        /* degrees to 256-step binary angle */
        yaw = (data->camera->unk380 + 180.0f) * 0.7111111283f;
        do {
            func_15143874(yaw + 21, func_150ADA68() * 381.0f + 254.0f, &from.x, &from.z);
            from.y = func_150ADA68() * 212.0f + -133.0f;
            from.x += eye->x;
            from.y += eye->y;
            from.z += eye->z;
            func_15143874(yaw - 21, func_150ADA68() * 381.0f + 254.0f, &to.x, &to.z);
            to.y = func_150ADA68() * 212.0f + -133.0f;
            to.x += eye->x;
            to.y += eye->y;
            to.z += eye->z;
            if (func_150ADA20() & 1) {
                Vec3f tmp;

                tmp = from;
                from = to;
                to = tmp;
            }
            variants = &D_80088B20[data->variants];
            i = func_150ADA20() % variants->count;
            speed = func_150ADA68() * 10.0f + 5.0f;
            func_150F4570(&from, &to, variants->ids[i], speed, 1.0f / speed,
                          variants->scales[i] * (func_150ADA68() * 0.07500000298f + 0.07500000298f), FALSE, 0,
                          emitter->base.header.owner, emitter->base.header.kind);
            data->pending -= 1.0f;
        } while (data->pending > 1.0f);
    }
}
