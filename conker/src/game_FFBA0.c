#define func_1513D2F0 func_1513D2F0_hdr
#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#undef func_1513D2F0
void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, u8 argA, s32 argB);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* Object data (at +0x28) of a trigger that spawns spark emitters. */
typedef struct EmitterTrigger {
    s16 *record;
    s32 minDelay;
    s32 delayRange;
    s32 countdown;
    f32 matrix[16];
    u8 params[2]; // [0] bit 0: spawn periodically; [1]: id matched by event 0x34
                  // matching: one array; two u8 fields swap the compare's registers in func_150D32FC
} EmitterTrigger;

/* Object data (at +0x28) of a spark emitter: created by func_150D278C, updated by func_150D2924. */
typedef struct SparkEmitter {
    s16 *record;
    f32 angle;
    f32 rate;
    f32 interval;
    f32 angleStep;
    f32 spawnAccum;
    f32 elapsed;
    f32 matrix[16];
    u8 color[3];
} SparkEmitter; // size 0x60

typedef struct EffectObject {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    union {
        EmitterTrigger trigger;
        SparkEmitter emitter;
    } data;
} EffectObject;

void func_150D278C(s16 *record, f32 *matrix, u8 unkC, s32 unk1);

/* While params[0] bit 0 is set, spawns a spark emitter after each random delay. */
void func_150D26F0(EffectObject *obj) {
    EmitterTrigger *trigger = &obj->data.trigger;

    if (trigger->params[0] & 1) {
        trigger->countdown -= D_800BE9E4;
        if (trigger->countdown < 0) {
            func_150D278C(trigger->record, trigger->matrix, obj->unkC, obj->unk1);
            trigger->countdown = func_150ADA20() % (u32)(trigger->delayRange + 1) + trigger->minDelay;
        }
    }
}

/* Spawns a spark emitter object with a random spin, rate and color. */
void func_150D278C(s16 *record, f32 *matrix, u8 unkC, s32 unk1) {
    SparkEmitter emitter;
    struct260 *object;

    emitter.record = record;
    emitter.angle = func_150ADA68() * 2.0f * 3.141592741f;
    emitter.rate = func_150ADA68() * 0.4f + 0.3f;
    emitter.interval = 1.0f / emitter.rate;
    emitter.angleStep = func_150ADA68() * 0.13000001f + 0.3f;
    if (func_150ADA20() & 1) {
        emitter.angleStep = -emitter.angleStep;
    }
    emitter.spawnAccum = 0.0f;
    emitter.elapsed = 0.0f;
    memcpy(emitter.matrix, matrix, sizeof(emitter.matrix));
    {
        const u8 colors[3][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x00, 0x00 }, { 0x00, 0xFF, 0x00 } };
        u8 i = func_150ADA20() % 3U;

        emitter.color[0] = colors[i][0];
        emitter.color[1] = colors[i][1];
        emitter.color[2] = colors[i][2];
    }
    // the header types func_15149130's data-size parameter as struct37 *
    object = func_15149130(func_150ADA20() % 101U + 100, -1, 0x32, -1, 1, 0, (struct37 *)sizeof(emitter), unkC, unk1);
    if (object != NULL) {
        memcpy(&((EffectObject *)object)->data, &emitter, sizeof(emitter));
    }
}

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    struct17 unk1C;
    struct17 unk28;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    u8 unk4C;
    u8 pad4D[3];
    s32 unk50;
} Func150D2924Spawn;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
} Func150D2924Extra;

typedef struct {
    s16 *unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C[16];
    u8 unk5C;
    u8 unk5D;
    u8 unk5E;
} Func150D2924Sub;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Func150D2924Sub unk28;
} Func150D2924Obj;

extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern void func_1514373C(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3);
extern s32 D_800A4AA0;

void func_150D2924(Func150D2924Obj *arg0) {
    Func150D2924Sub *sub;
    s16 *src;

    sub = &arg0->unk28;
    sub->unk14 += sub->unk8 * D_800BE9A4;
    sub->unk18 += D_800BE9A4;
    if (sub->unk14 > 1.0f) {
        Func150D2924Spawn spawn;
        void *ret;
        Func150D2924Extra extra;

        src = sub->unk0;
        extra.unk2C = 9.0f;
        extra.unk18 = 0.0f;
        extra.unk1C = 0.0f;
        spawn.unk0 = 0x59;
        spawn.unk1 = 0;
        spawn.unk2 = 0x5A03;
        spawn.unk4 = 0x16;
        spawn.unk8 = 0;
        spawn.unkC = 0;
        spawn.unk10 = sub->unk5C;
        spawn.unk11 = sub->unk5D;
        spawn.unk12 = sub->unk5E;
        spawn.unk13 = 0xFF;
        spawn.unk18 = 1.0f;
        spawn.unk1C.unk0 = src[0];
        spawn.unk1C.unk4 = src[1];
        spawn.unk1C.unk8 = src[2];
        spawn.unk34 = 1.0f;
        spawn.unk38 = 1.0f;
        spawn.unk3C = 1.0f;
        spawn.unk40 = 0x026C0000;
        spawn.unk45 = 0xFF;
        spawn.unk46 = 0;
        spawn.unk47 = 6;
        spawn.unk48 = 0;
        spawn.unk4C = 0xFF;
        do {
            extra.unk28 = 22.0f - sub->unk18;
            if (extra.unk28 > 0.0f) {
                struct17 pos;

                func_1514373C(sub->unk4, src[3], &pos.unk0, &pos.unk8);
                pos.unk4 = src[4];
                func_150A7960(sub->unk1C, pos.unk0, pos.unk4, pos.unk8, &spawn.unk28.unk0, &spawn.unk28.unk4, &spawn.unk28.unk8);
                spawn.unk14 = func_150ADA68() * 25.0f + 50.0f;
                extra.unk0 = extra.unk8 = func_150ADA68() * 1024.0f + -2560.0f + 2048.0f;
                extra.unkC = func_150ADA68() * 1024.0f + 1536.0f + 2048.0f;
                extra.unk4 = extra.unkC;
                extra.unk10 = func_150ADA68() * 768.0f + 768.0f;
                extra.unk14 = func_150ADA68() * 768.0f + 768.0f;
                extra.unk20 = (func_150ADA68() * 80.0f + 50.0f) * 0.001000000047f;
                extra.unk24 = (func_150ADA68() * 80.0f + 50.0f) * 0.001000000047f;
                spawn.unk44 = extra.unk2C * extra.unk28;
                ret = func_1513D2F0(&spawn, &D_800A4AA0, 0, 0x26, 0, 0x1F, 0, 0, 0, 0x30, arg0->unkC, arg0->unk1);
                if (ret != NULL) {
                    memcpy((u8 *)ret + 0x110, &extra, sizeof(extra));
                }
            }
            sub->unk4 += sub->unk10;
            sub->unk18 -= sub->unkC;
            sub->unk14 -= 1.0f;
        } while (sub->unk14 > 1.0f);
    }
}

typedef struct GameFFBA0BeamVertex {
    s16 x;
    s16 y;
    s16 z;
    s16 flag;
    s16 s;
    s16 t;
    u8 color[4];
} GameFFBA0BeamVertex;

typedef struct Vec3f_150D2D6C {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_150D2D6C;

void func_150AD8B0(f32 *, f32 *, f32 *);
s32 func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);

typedef struct GameFFBA0Beam {
    u8 pad0[0x2C];
    f32 width;
    u8 pad30[4];
    Vec3f_150D2D6C start;
    Vec3f_150D2D6C end;
    u8 pad4C[0x74];
    u8 vertexTemplate[0x40];
    u8 *buffers[4];
    f32 texture[2];
} GameFFBA0Beam;

void *func_150D2D6C(GameFFBA0Beam *arg0, s16 arg1) {
    GameFFBA0BeamVertex *vertices;
    void *result;
    Vec3f_150D2D6C *camera;
    Vec3f_150D2D6C delta;
    Vec3f_150D2D6C end;
    Vec3f_150D2D6C middle;
    Vec3f_150D2D6C view;
    Vec3f_150D2D6C side;
    f32 lengthSquared;
    f32 *texture;
    u8 fresh;

    func_151D5D60(arg0->buffers, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(arg0->buffers[arg1], arg0->vertexTemplate, 0x40);
            memcpy(arg0->buffers[arg1] + 0x40, arg0->vertexTemplate, 0x40);
        }
    } else {
        return NULL;
    }
    texture = arg0->texture;
    camera = (Vec3f_150D2D6C *)func_15144B34(arg1);
    delta.x = arg0->end.x - arg0->start.x;
    delta.y = arg0->end.y - arg0->start.y;
    delta.z = arg0->end.z - arg0->start.z;
    end.x = arg0->start.x + delta.x;
    end.y = arg0->start.y + delta.y;
    end.z = arg0->start.z + delta.z;
    middle.x = arg0->start.x + delta.x * 0.5f;
    middle.y = arg0->start.y + delta.y * 0.5f;
    middle.z = arg0->start.z + delta.z * 0.5f;
    view.x = middle.x - camera->x;
    view.y = middle.y - camera->y;
    view.z = middle.z - camera->z;
    func_150AD8B0(&delta.x, &view.x, &side.x);
    lengthSquared = side.x * side.x + side.y * side.y + side.z * side.z;
    if (lengthSquared != 0.0f) {
        f32 scale = arg0->width / sqrtf(lengthSquared);

        side.x = side.x * scale;
        side.y = side.y * scale;
        side.z = side.z * scale;
    } else {
        side.x = 0;
        side.y = 0;
        side.z = 0;
    }
    vertices->x = arg0->start.x - side.x;
    vertices->y = arg0->start.y - side.y;
    vertices->z = arg0->start.z - side.z;
    vertices->s = texture[1];
    vertices->flag = 0;
    vertices++;
    vertices->x = end.x - side.x;
    vertices->y = end.y - side.y;
    vertices->z = end.z - side.z;
    vertices->s = texture[0];
    vertices->flag = 0;
    vertices++;
    vertices->x = end.x + side.x;
    vertices->y = end.y + side.y;
    vertices->z = end.z + side.z;
    vertices->s = texture[0];
    vertices->flag = 0;
    vertices++;
    vertices->x = arg0->start.x + side.x;
    vertices->y = arg0->start.y + side.y;
    vertices->z = arg0->start.z + side.z;
    vertices->s = texture[1];
    vertices->flag = 0;
    return result;
}

/* Motion data (at +0x110) of a spark particle spawned by func_150D2924. */
typedef struct SparkMotion {
    f32 posX;
    f32 posY;
    f32 baseX;
    f32 baseY;
    f32 amplitudeX;
    f32 amplitudeY;
    f32 phaseX;
    f32 phaseY;
    f32 phaseSpeedX;
    f32 phaseSpeedY;
    f32 life;
    f32 fadeScale;
} SparkMotion;

typedef struct SparkParticle {
    u8 pad0[0x5C];
    u8 unk5C; // fades out with the remaining life
    u8 pad5D[0xB3];
    SparkMotion motion;
} SparkParticle;

/* Sways a spark around its base position and fades it out; returns 0 once its life runs out. */
s32 func_150D317C(SparkParticle *particle) {
    SparkMotion *motion = &particle->motion;

    motion->phaseX += motion->phaseSpeedX * D_800BE9A4;
    motion->phaseY += motion->phaseSpeedY * D_800BE9A4;
    motion->phaseX = func_15144B68(motion->phaseX);
    motion->phaseY = func_15144B68(motion->phaseY);
    motion->posX = sinf(motion->phaseX) * motion->amplitudeX + motion->baseX;
    motion->posY = sinf(motion->phaseY) * motion->amplitudeY + motion->baseY;
    motion->life -= D_800BE9A4;
    particle->unk5C = motion->fadeScale * motion->life;
    if (motion->life > 0.0f) {
        return 1;
    }
    return 0;
}

/* On event 0x34 carrying this trigger's id, spawns a spark emitter immediately. */
void func_150D32FC(EffectObject *obj, u8 *msg, u8 event) {
    EmitterTrigger *trigger = &obj->data.trigger;

    if (event == 0x34 && msg[0] == trigger->params[1]) {
        func_150D278C(trigger->record, trigger->matrix, obj->unkC, obj->unk1);
    }
}
