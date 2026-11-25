#include <ultra64.h>
/* functions.h declares these two with other return types; they are redeclared below. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_10010F88 func_10010F88_void_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
#undef func_10010F88
// matching: the full 32-bit random value is used here; the header's u8 return adds a mask
u32 func_150ADA20(void);
/* Returns the playing sound's handle. */
s32 func_10010F88(s32 sound, u16 volume, s16 cents, u8 mix, s32 arg4, s16 x, s16 y, s16 z, s16 nearDistance,
                  s16 farDistance);
#include "variables.h"


/* Flags of SpiralState. */
#define SPIRAL_FADE_NEAR 1 /* fade out as the camera gets close */
#define SPIRAL_SOUND 2     /* plays a positional sound */
#define SPIRAL_MUTED 4     /* sound no longer updated or restarted */

/* Per-object state of the effect spawned by func_150F4570: it circles a centre that moves
 * from one point towards another (updated by func_150F48D0). */
typedef struct {
    /* 0x00 */ f32 timeLeft;
    /* 0x04 */ struct17 center;
    /* 0x10 */ f32 phase; /* radians */
    /* 0x14 */ f32 phaseRate;
    /* 0x18 */ f32 cosDir;
    /* 0x1C */ f32 sinDir;
    /* 0x20 */ f32 radius;
    /* 0x24 */ u8 flags; /* SPIRAL_* */
    /* 0x25 */ u8 pad25[3];
} SpiralState;

/* Spawn descriptor for func_1513264C (0x7C bytes). */
typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ struct17 rot; /* degrees */
    /* 0x1C */ struct17 unk1C;
    /* 0x28 */ struct17 pos;
    /* 0x34 */ struct17 vel;
    /* 0x40 */ struct17 spin; /* added to rot each frame */
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 id;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
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
    /* 0x69 */ s8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ s32 sound;
} EffectSpawn;

/* An object spawned by func_1513264C with a SpiralState. */
typedef struct {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ struct17 rot; /* degrees */
    /* 0x02C */ u8 pad2C[0xC];
    /* 0x038 */ struct17 pos;
    /* 0x044 */ struct17 vel;
    /* 0x050 */ struct17 spin;
    /* 0x05C */ u8 pad5C[0x14];
    /* 0x070 */ u8 alpha;
    /* 0x071 */ u8 unk71;
    /* 0x072 */ u8 pad72[0x16];
    /* 0x088 */ u32 sound; /* sound id in the low half, playing handle in the high half */
    /* 0x08C */ u8 pad8C[0xE4];
    /* 0x170 */ SpiralState spiral;
} EffectObj;

extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
extern EffectObj *func_1513264C(EffectSpawn *spawn, s32 arg1, s32 arg2, s32 arg3, s32 extraSize, u8 owner, s32 kind);

/* Spawns an effect that spirals from `from` to `to` at `speed` (invSpeed = 1 / speed), playing
 * `sound` near the camera when it is non-zero. */
void func_150F4570(struct17 *from, struct17 *to, u8 id, f32 speed, f32 invSpeed, f32 scale, u8 fadeNear, s32 sound,
                   u8 owner, s32 kind) {
    EffectSpawn spawn;
    struct17 dir;
    struct17 delta;
    f32 length;
    f32 invLength;
    SpiralState spiral;
    EffectObj *obj;

    delta.unk0 = to->unk0 - from->unk0;
    delta.unk4 = to->unk4 - from->unk4;
    delta.unk8 = to->unk8 - from->unk8;
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
        spawn.unk00 = 1.0f;
        spawn.unk04 = 1.0f;
        spawn.unk08 = spawn.unk0C = scale;
        spawn.rot.unk0 = func_150ADA68() * 360.0f;
        spawn.rot.unk4 = func_150ADA68() * 360.0f;
        spawn.rot.unk8 = func_150ADA68() * 360.0f;
        spawn.unk1C.unk0 = 1.0f;
        spawn.unk1C.unk4 = 1.0f;
        spawn.unk1C.unk8 = 1.0f;
        spawn.pos = *from;
        spawn.vel.unk0 = dir.unk0 * speed;
        spawn.vel.unk4 = dir.unk4 * speed;
        spawn.vel.unk8 = dir.unk8 * speed;
        spawn.spin.unk0 = func_150ADA68() * 21.90300179f + -10.95150089f;
        spawn.spin.unk4 = 0.0f;
        spawn.spin.unk8 = func_150ADA68() * 21.90300179f + -10.95150089f;
        spawn.unk4C = 0.0f;
        spawn.unk50 = 0x14900;
        spawn.unk54 = 300;
        spawn.id = id;
        spawn.unk58 = 8;
        spawn.unk5C = 0;
        spawn.unk60 = 255;
        spawn.unk61 = 22;
        spawn.unk62 = 7;
        spawn.unk63 = 0;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 2;
        spawn.unk69 = -1;
        spawn.unk6A = 2;
        spawn.unk6C = 0;
        spawn.unk70 = 0;
        spawn.unk72 = 1;
        spawn.unk74 = 255;
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
        spiral->center.unk0 += obj->vel.unk0 * D_800BE9A4;
        spiral->center.unk4 += obj->vel.unk4 * D_800BE9A4;
        spiral->center.unk8 += obj->vel.unk8 * D_800BE9A4;
        obj->pos.unk0 = spiral->center.unk0 + side * spiral->cosDir;
        obj->pos.unk4 = spiral->center.unk4 + up;
        obj->pos.unk8 = spiral->center.unk8 - side * spiral->sinDir;
    }
    obj->rot.unk0 += obj->spin.unk0 * D_800BE9A4;
    obj->rot.unk4 += obj->spin.unk4 * D_800BE9A4;
    obj->rot.unk8 += obj->spin.unk8 * D_800BE9A4;
    // matching: the original falls off the end here without setting a return value
}

extern struct17 *func_15144B34(s32);
extern f32 func_15143E64(struct17 *);

/* Fades the object out near the camera and keeps its positional sound playing (started
 * within 200 units of the camera), as its SPIRAL_* flags ask; always returns 1. */
s32 func_150F4A38(EffectObj *obj) {
    SpiralState *spiral;
    f32 dist;

    spiral = &obj->spiral;
    if ((spiral->flags & SPIRAL_FADE_NEAR) || (spiral->flags & SPIRAL_SOUND)) {
        struct17 *camera;
        struct17 delta;

        camera = func_15144B34(0);
        delta.unk0 = obj->pos.unk0 - camera->unk0;
        delta.unk4 = obj->pos.unk4 - camera->unk4;
        delta.unk8 = obj->pos.unk8 - camera->unk8;
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
            func_1000F9D4(obj->sound >> 16, obj->pos.unk0, obj->pos.unk4, obj->pos.unk8);
        } else if (dist < 200.0f) {
            obj->sound |= func_10010F88(obj->sound & 0xFFFF, (func_150ADA20() & 0x3FFF) + 0x4000, 0, 0, -1,
                                        obj->pos.unk0, obj->pos.unk4, obj->pos.unk8, 10000, 20000) << 16;
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
        func_1516972C((struct102 *)obj);
    }
}

/* Per-object state of the emitter started by func_150F4D5C (updated by func_150F4DEC). */
typedef struct {
    /* 0x00 */ struct108 *camera;
    /* 0x04 */ f32 pending; /* spirals owed; one is spawned per whole unit */
    /* 0x08 */ s8 condition; /* index into D_80088B38, or -1 for none */
    /* 0x09 */ u8 variants; /* index into D_80088B20 */
    /* 0x0A */ u8 padA[2];
} CameraEmitterData;

/* The emitter effect object created by func_15149130. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x28 - 0xD];
    /* 0x28 */ CameraEmitterData data;
} CameraEmitter;

/* Starts an emitter that spawns spirals around `camera` (see func_150F4DEC). */
void func_150F4D5C(struct108 *camera, s8 condition, u8 variants, u8 owner, s32 kind) {
    CameraEmitterData data;
    CameraEmitter *emitter;

    data.camera = camera;
    data.pending = 0.0f;
    data.condition = condition;
    data.variants = variants;

    emitter = (CameraEmitter *)func_15149130(300, -1, 0x56, -1, 0, 0, (struct37 *)sizeof(data), owner, kind);
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
    struct17 *eye;
    s32 yaw;
    struct17 from;
    struct17 to;
    VariantList *variants;
    u8 i;
    f32 speed;

    data = &emitter->data;
    if (data->condition != -1 && !D_80088B38[data->condition]()) {
        return;
    }
    data->pending += (0.05100000277f + func_150ADA68() * 0.153000012f) * D_800BE9A4;
    if (data->pending > 1.0f) {
        eye = func_15144B34(data->camera->unk23D);
        /* degrees to 256-step binary angle */
        yaw = (data->camera->unk380 + 180.0f) * 0.7111111283f;
        do {
            func_15143874(yaw + 21, func_150ADA68() * 381.0f + 254.0f, &from.unk0, &from.unk8);
            from.unk4 = func_150ADA68() * 212.0f + -133.0f;
            from.unk0 += eye->unk0;
            from.unk4 += eye->unk4;
            from.unk8 += eye->unk8;
            func_15143874(yaw - 21, func_150ADA68() * 381.0f + 254.0f, &to.unk0, &to.unk8);
            to.unk4 = func_150ADA68() * 212.0f + -133.0f;
            to.unk0 += eye->unk0;
            to.unk4 += eye->unk4;
            to.unk8 += eye->unk8;
            if (func_150ADA20() & 1) {
                struct17 tmp;

                tmp = from;
                from = to;
                to = tmp;
            }
            variants = &D_80088B20[data->variants];
            i = func_150ADA20() % variants->count;
            speed = func_150ADA68() * 10.0f + 5.0f;
            func_150F4570(&from, &to, variants->ids[i], speed, 1.0f / speed,
                          variants->scales[i] * (func_150ADA68() * 0.07500000298f + 0.07500000298f), FALSE, 0,
                          emitter->unkC, emitter->unk1);
            data->pending -= 1.0f;
        } while (data->pending > 1.0f);
    }
}
