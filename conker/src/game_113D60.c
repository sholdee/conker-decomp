#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

typedef struct {
    /* 0x0 */ s8 type;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 duration;
    /* 0x4 */ s8 unk4;
    /* 0x5 */ s8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 pad7;
} Game113D60Event; // argument of func_151D8868

/* Wrappers for small id tables in rodata that functions copy onto their stack: the original
 * code used initialized local arrays, whose initializer data the asm rodata provides here. */
typedef struct {
    u8 ids[1];
} Game113D60Ids1;

typedef struct {
    s32 ids[2];
} Game113D60Ids2;

typedef struct {
    s32 ids[3];
} Game113D60Ids3;

extern void *func_151D8868(Game113D60Event *event, s32 arg1, s32 arg2, s32 arg3);

typedef struct Game113D60Emission {
    /* 0x00 */ f32 x, y, amplitudeX, amplitudeY;
    /* 0x10 */ f32 phaseX, phaseY, speedX, speedY;
    /* 0x20 */ f32 time, rate, remaining;
    /* 0x2C */ Vec3f position;
    /* 0x38 */ f32 scale;
    /* 0x3C */ s16 duration, durationRange;
    /* 0x40 */ u8 color;
} Game113D60Emission; // size 0x44

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ u8 data[1]; // effect-specific data
} Game113D60AnyEffect;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ Game113D60Emission emission;
} Game113D60Effect;

typedef struct Game113D60Source {
    f32 chance;
    s8 start;
    s8 end;
    s8 motion;
    u8 pad7;
    s16 duration;
    s16 durationRange;
    u8 color;
    u8 padD[3];
} Game113D60Source;

typedef void (*Game113D60EndCallback)(Vec3f *, s32);
typedef void (*Game113D60StartCallback)(Vec3f *);
typedef void (*Game113D60MotionCallback)(Vec3f *, Vec3f *, Vec3f *, Game113D60Emission *);

void *memcpy(void *, const void *, u32);
extern Game113D60StartCallback D_80088A20[];
extern Game113D60EndCallback D_80088A2C[];
extern Game113D60MotionCallback D_80088A34[];
extern Game113D60Source D_800A1190[];
extern u8 D_800C35E8;

/* Randomly starts an emission of the source effect `sourceIndex` from one of the start
 * points towards `actor`. */
void func_150E68B0(s32 actor, s32 arg1, s32 arg2, u8 sourceIndex) {
    Game113D60Source *source;
    Vec3f end;
    Vec3f start;
    Vec3f delta;
    Game113D60Emission emission;
    Game113D60Effect *effect;

    if ((actor != 0) && ((D_800C35EA[0] != 1) || (D_800C35E8 != 9))) {
        source = &D_800A1190[sourceIndex];
        if ((func_150ADA68() < source->chance) && (source->start != -1)) {
            if (source->end != -1) {
                D_80088A2C[source->end](&end, actor);
            } else {
                end.x = 0.0f;
                end.y = 0.0f;
                end.z = 0.0f;
            }
            if (source->start != -1) {
                D_80088A20[source->start](&start);
            } else {
                start.x = 0.0f;
                start.y = 0.0f;
                start.z = 0.0f;
            }
            delta.x = end.x - start.x;
            delta.y = end.y - start.y;
            delta.z = end.z - start.z;
            if ((delta.x != 0) || (delta.z != 0)) {
                if (source->motion != -1) {
                    D_80088A34[source->motion](&start, &end, &delta, &emission);
                } else {
                    emission.x = 0.0f;
                    emission.amplitudeX = 0.0f;
                    emission.phaseX = 0.0f;
                    emission.y = 0.0f;
                    emission.amplitudeY = 0.0f;
                    emission.phaseY = 0.0f;
                }
                emission.rate = func_150ADA68() * 0.150000006f + 0.200000003f;
                emission.remaining = 0.0f;
                emission.position = start;
                emission.time = 0.0f;
                emission.scale = (func_150ADA68() * 100.0f + 150.0f) * 0.1000000015f;
                emission.duration = source->duration;
                emission.durationRange = source->durationRange;
                emission.color = source->color;
                effect = (Game113D60Effect *)func_151491F4(func_150ADA68() * 70.0f + 30.0f, -1, 15, 1, 11,
                                                           sizeof(Game113D60Emission), 0xFF, 1);
                if (effect != NULL) {
                    memcpy(&effect->emission, &emission, sizeof(Game113D60Emission));
                }
            }
        }
    }
}

extern f32 sinf(f32);
extern void func_151436B4(f32, f32, f32, Vec3f *);
extern s32 func_150E5FD0(Vec3f *, Vec3f *, f32, f32, f32, u8, s32, u8, u8, s16, s8);
extern f32 func_15144528(f32, f32, f32);

/* Emits this frame's particles along a sine-wobbled direction and advances the wobble phases. */
void func_150E6B84(Game113D60Effect *effect) {
    Game113D60Emission *emission;
    f32 inverse;
    f32 time;
    f32 timeStep;
    f32 phaseStepX;
    f32 phaseStepY;
    f32 speedX;
    f32 speedY;
    f32 factor;
    f32 period;
    Vec3f direction;
    f32 phaseX;
    f32 phaseY;
    f32 sineX;
    f32 sineY;
    f32 randomX;
    f32 randomY;
    u32 randomDuration;
    u32 randomColor;

    emission = &effect->emission;
    effect->emission.remaining += effect->emission.rate * D_800BE9A4;
    if (effect->emission.remaining > 1.0f) {
        inverse = 1.0f / emission->remaining;
        factor = 0.1f;
        time = emission->time + D_800BE9A4;
        timeStep = time * inverse;
        speedX = emission->speedX * D_800BE9A4;
        speedY = emission->speedY * D_800BE9A4;
        phaseX = emission->phaseX;
        phaseY = emission->phaseY;
        phaseStepX = speedX * inverse;
        phaseStepY = speedY * inverse;
        do {
            sineX = sinf(phaseX);
            sineY = sinf(phaseY);
            func_151436B4(emission->x + sineX * emission->amplitudeX, emission->y + sineY * emission->amplitudeY, 5.0f,
                          &direction);
            randomX = func_150ADA68();
            randomY = func_150ADA68();
            randomColor = func_150ADA20();
            randomDuration = func_150ADA20();
            func_150E5FD0(&emission->position, &direction, emission->scale, (randomX * 300.0f + 100.0f) * factor,
                          (randomY * 700.0f + 300.0f) * factor, effect->header.owner, effect->header.kind,
                          emission->color, randomColor % 86 + 170,
                          randomDuration % (u32)(emission->durationRange + 1) + emission->duration, -1);
            phaseX += phaseStepX;
            phaseY += phaseStepY;
            time -= timeStep;
            emission->remaining -= 1.0f;
        } while (emission->remaining > 1.0f);
        period = 6.283185482f;
        emission->phaseX = func_15144528(phaseX, period, 0.0f);
        emission->phaseY = func_15144528(phaseY, period, 0.0f);
        emission->time = time;
    }
}

typedef struct {
    /* 0x00 */ Vec3f start;
    /* 0x0C */ Vec3f end;
} Game113D60Segment;

extern Game113D60Segment *D_80088A3C;
extern Game113D60Segment *D_80088A40;

/* Start callback: a random point on one of two segments. */
void func_150E6E34(Vec3f *out) {
    Game113D60Segment *segment;
    f32 t;

    if (func_150ADA68() < 0.5632223487f) {
        segment = D_80088A3C;
    } else {
        segment = D_80088A40;
    }
    t = func_150ADA68();
    out->x = (segment->end.x - segment->start.x) * t + segment->start.x;
    out->y = (segment->end.y - segment->start.y) * t + segment->start.y;
    out->z = (segment->end.z - segment->start.z) * t + segment->start.z;
}

extern void func_1514470C(struct134 *, void *);

/* End callback: the position of one of the two entries of the D_800D9A20 table. */
void func_150E6ED8(Vec3f *out) {
    // D_800D9A20 is declared as a single pointer in variables.h but is a two-entry table here.
    func_1514470C(D_800D9A20[func_150ADA20() & 1], out);
}

extern Game113D60Segment *D_80088A44[];

/* Start callback: a random point on one of six segments. */
void func_150E6F18(Vec3f *out) {
    Game113D60Segment *segment;
    f32 t;

    segment = D_80088A44[func_150ADA20() % 6];
    t = func_150ADA68();
    out->x = (segment->end.x - segment->start.x) * t + segment->start.x;
    out->y = (segment->end.y - segment->start.y) * t + segment->start.y;
    out->z = (segment->end.z - segment->start.z) * t + segment->start.z;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ Vec3f unk38;
} Bone150E6FAC;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Bone150E6FAC *unk10;
} Node150E6FAC;

s32 func_1514ECE0(void *, s16, void *);

/* End callback: a random point on a ring around node 0x16 of the actor's model, or the actor's
 * position when the model has no such node. */
void func_150E6FAC(Vec3f *out, Actor *actor) {
    Node150E6FAC *node;
    f32 sine;
    f32 radius;
    s16 angle;
    f32 cosine;

    if (func_1514ECE0(actor->attachments, 0x16, &node) != 0) {
        Bone150E6FAC *bone;

        radius = (func_150ADA68() * 100.0f) + 80.0f;
        angle = func_150ADA20() & 0xFF;
        cosine = func_151423D8(angle - 0x40);
        sine = func_151423D8(angle);
        bone = node->unk10;
        out->x = actor->x_position + (bone->unk38.x * -80.0f) + (cosine * radius);
        out->y = actor->y_position + (bone->unk38.y * -80.0f) + 100.0f;
        out->z = actor->z_position + (bone->unk38.z * -80.0f) + (sine * radius);
    } else {
        out->x = actor->x_position;
        out->y = actor->y_position;
        out->z = actor->z_position;
    }
}

/* End callback: the actor's position. */
void func_150E70CC(Vec3f *out, Actor *actor) {
    out->x = actor->x_position;
    out->y = actor->y_position;
    out->z = actor->z_position;
}

/* Motion callback: aims the emission along `delta` with a random wobble. */
void func_150E70EC(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission) {
    f32 dx;
    f32 dz;

    emission->x = func_150484A0(delta->x, delta->z);
    emission->amplitudeX = func_150ADA68() * 0.09000000358f;
    emission->phaseX = func_150ADA68() * 6.283185482f;
    emission->speedX = func_150ADA68() * 0.5f;
    dx = delta->x;
    dz = delta->z;
    emission->y = func_150484A0(sqrtf((dx * dx) + (dz * dz)), delta->y) - 1.570796371f;
    emission->amplitudeY = func_150ADA68() * 0.0700000003f;
    emission->phaseY = func_150ADA68() * 6.283185482f;
    emission->speedY = func_150ADA68() * 0.5f;
}

/* Motion callback: aims the emission along `delta` with a fixed tilt and a random wobble. */
void func_150E71E4(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission) {
    emission->x = func_150484A0(delta->x, delta->z);
    emission->amplitudeX = 0.471238941f;
    emission->phaseX = func_150ADA68() * 6.283185482f;
    emission->speedX = func_150ADA68() * 0.200000003f;
    emission->y = -0.5654867291f;
    emission->amplitudeY = 0.3141592741f;
    emission->phaseY = func_150ADA68() * 6.283185482f;
    emission->speedY = func_150ADA68() * 0.150000006f;
}

typedef struct {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
} Vec2f;

extern Game113D60Ids2 D_80088A5C;
extern Game113D60Ids1 D_80088A64;
extern Game113D60Ids3 D_80088A68;
extern Game113D60Ids3 D_80088A74;
extern void *func_1515548C(ScreenSpriteInfo5C *, s32, s32 *, s32, s32, u8, s32);
extern void func_15164F0C(u8, u8, void *, u8, s32);
void func_150E75A0(Vec2f *position, f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids,
                   s32 idCount, u8 owner, s32 kind);
void func_150E76D0(f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids, s32 idCount,
                   u8 owner, s32 kind);

/* Occasionally spawns one of the two func_1515548C effects, plus a sound and an event. */
void func_150E7290(u8 arg0, u8 owner, s32 kind) {
    Game113D60Ids2 ids;
    Vec2f position;
    Game113D60Event event;
    s32 pad; // matching: unused stack slot
    u32 flip4;
    u32 flip2;
    u32 random;
    f32 randomSize;

    if (func_150ADA68() < 0.00419999985f) {
        ids = D_80088A5C;
        if (func_150ADA68() < 0.7500000596f) {
            position.x = (func_150ADA68() * 300.0f) + -150.0f;
            position.y = (func_150ADA68() * 200.0f) + -100.0f;
            randomSize = func_150ADA68();
            random = func_150ADA20();
            flip2 = (func_150ADA20() & 1) ? 2 : 0;
            flip4 = (func_150ADA20() & 1) ? 4 : 0;
            func_150E75A0(&position, ((randomSize * 150.0f) + 200.0f) * 0.1f, (random % 201) + 500,
                          (u8)(flip4 | flip2) | 9, (func_150ADA20() % 26) + 100, 0xFF, 0x40, 3, ids.ids, 2, owner,
                          kind);
        } else {
            randomSize = func_150ADA68();
            flip4 = func_150ADA20(); // matching: a separate variable here does not match
            func_150E76D0(((randomSize * 100.0f) + 150.0f) * 0.1f, (flip4 % 201) + 500, 9,
                          (func_150ADA20() % 26) + 100, 0xFF, 0x40, 3, ids.ids, 2, owner, kind);
        }
        func_10010F30(0x360, 0x7FFF, 0, 0, 0);
        func_15164F0C(1, arg0, NULL, owner, kind);
        event.type = 1;
        event.duration = (func_150ADA20() % 26) + 25;
        event.unk5 = 1;
        event.unk4 = (func_150ADA20() % 6) + 3;
        event.unk6 = -1;
        func_151D8868(&event, 0, 0xFF, 0);
    }
}

/* Spawns a func_1515548C effect of the given size at `position`. */
void func_150E75A0(Vec2f *position, f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids,
                   s32 idCount, u8 owner, s32 kind) {
    ScreenSpriteInfo5C arg;
    Game113D60Ids1 id;

    id = D_80088A64;
    // matching: x and y are copied as one Vec2f
    *(Vec2f *)&arg.x = *position;
    arg.halfWidth = size;
    arg.halfHeight = size;
    arg.texture = id.ids[0];
    arg.timer = unk12;
    arg.flags = flags | 0x40;
    arg.fadeStart = unk16;
    arg.fadeRate = unk18;
    arg.kind = 4;
    arg.color1.red = 0xFF;
    arg.color1.green = 0xE6;
    arg.color1.blue = 0xBE;
    arg.color1.alpha = unk1E;
    arg.color2.red = 0xFF;
    arg.color2.green = 0xFF;
    arg.color2.blue = 0xFF;
    arg.color2.alpha = 0xFF;
    arg.unk23 = unk23;
    arg.otherModeH = 1;
    arg.unk28 = 0;
    arg.unk2C = 0;
    arg.unk40 = 0;
    arg.unk41 = 10;
    arg.combineMode = 7;
    arg.unk34 = 60;
    arg.unk38 = 0x80;
    arg.unk3C = 0x20;
    func_1515548C(&arg, 0, ids, idCount, 0, owner, kind);
}

/* Like func_150E75A0, but placed at a random screen edge. */
void func_150E76D0(f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids, s32 idCount,
                   u8 owner, s32 kind) {
    ScreenSpriteInfo5C arg;
    u8 edge;

    edge = func_150ADA20() & 3;

    arg.timer = unk12;
    arg.flags = flags & 0xFFF9;
    arg.fadeStart = unk16;
    arg.fadeRate = unk18;
    arg.kind = 5;
    arg.color1.red = 0xFF;
    arg.color1.green = 0xE6;
    arg.color1.blue = 0xBE;
    arg.color1.alpha = unk1E;
    arg.color2.red = 0xFF;
    arg.color2.green = 0xFF;
    arg.color2.blue = 0xFF;
    arg.color2.alpha = 0xFF;
    arg.unk23 = unk23;
    arg.otherModeH = 1;
    arg.unk28 = 0;
    arg.unk2C = 0;
    arg.unk40 = 0;
    arg.unk41 = 10;
    arg.combineMode = 7;
    arg.unk34 = 60;
    arg.unk38 = 0x80;
    arg.unk3C = 0x20;
    arg.halfHeight = size;
    arg.halfWidth = size;

    switch (edge) {
    case 0:
    case 1:
        {
            Game113D60Ids3 sideIds;

            sideIds = D_80088A68;
            arg.texture = sideIds.ids[func_150ADA20() % 3];
            arg.y = (func_150ADA68() * 160.0f) + -80.0f;
            switch (edge) {
            case 0:
                arg.flags |= 2;
                arg.x = 145.0f - size;
                break;
            case 1:
                arg.x = size - 145.0f;
                break;
            }
        }
        break;
    case 2:
    case 3:
        {
            Game113D60Ids3 topIds;

            topIds = D_80088A74;
            arg.texture = topIds.ids[func_150ADA20() % 3];
            arg.x = (func_150ADA68() * 260.0f) + -130.0f;
            switch (edge) {
            case 2:
                arg.flags |= 4;
                arg.y = 110.0f - size;
                break;
            case 3:
                arg.y = size - 110.0f;
                break;
            }
        }
        break;
    }

    func_1515548C(&arg, 0, ids, idCount, 0, owner, kind);
}

typedef struct {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 accum;
    /* 0x08 */ s16 count;
    /* 0x0A */ s16 next;
} Game113D60BurstHead;

typedef struct {
    /* 0x00 */ Game113D60BurstHead head;
    /* 0x0C */ s32 pad0C;
    /* 0x10 */ f32 points[2]; // `head.count` interleaved x, y pairs
} Game113D60Burst;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ Game113D60Burst burst;
} Game113D60BurstEffect;

extern f32 func_15142A80(f32);
extern f32 func_15142AC0(f32);
extern f32 func_15142B04(f32);
extern f32 func_15142B44(f32);

/* Creates a burst effect (updated by func_150E7C9C) whose `count` points lie on a random
 * curve across the screen. */
struct260 *func_150E7994(s16 count, f32 rate, u8 owner, s32 kind) {
    struct260 *obj;
    Game113D60BurstHead head;
    Game113D60Event event;
    Vec2f control[4];
    f32 *points;
    f32 t;
    f32 step;
    s16 i;

    if (count < 2) {
        return NULL;
    }

    func_1512D748(&D_800DBFF0[D_800BE9E8], 0, 1);

    event.type = 1;
    event.duration = (func_150ADA20() % 11) + 30;
    event.unk4 = 8;
    event.unk6 = -1;
    event.unk5 = 1;
    func_151D8868(&event, 0, 0xFF, 0);

    head.rate = rate;
    head.accum = 0.0f;
    head.count = count;
    head.next = 0;

    obj = func_151491F4(300, -1, 16, 1, 12, (count * sizeof(Vec2f)) + 0x10, owner, kind);
    if (obj != NULL) {
        memcpy(&((Game113D60BurstEffect *)obj)->burst.head, &head, sizeof(head));

        control[0].x = -146.0f;
        control[0].y = (func_150ADA68() * 160.0f) - 80.0f;
        control[1].x = -50.0f;
        control[1].y = (func_150ADA68() * 160.0f) - 80.0f;
        control[2].x = 50.0f;
        control[2].y = (func_150ADA68() * 160.0f) - 80.0f;
        control[3].x = 146.0f;
        control[3].y = (func_150ADA68() * 160.0f) - 80.0f;

        t = -1.0f;
        points = ((Game113D60BurstEffect *)obj)->burst.points;
        step = 3.0f / (f32)(count - 1);

        for (i = 0; i < count; i++) {
            points[i * 2] = (func_15142A80(t) * control[0].x) + (func_15142AC0(t) * control[1].x) +
                          (func_15142B04(t) * control[2].x) + (func_15142B44(t) * control[3].x);
            points[(i * 2) + 1] = (func_15142A80(t) * control[0].y) + (func_15142AC0(t) * control[1].y) +
                          (func_15142B04(t) * control[2].y) + (func_15142B44(t) * control[3].y);
            t += step;
        }
    }
    return obj;
}

/* Spawns the burst's next points, `rate` per frame, and ends the effect after the last one. */
void func_150E7C9C(Game113D60BurstEffect *effect) {
    s32 pad[2]; // matching: unused stack slots
    Vec2f position;
    f32 randomSize;
    s32 flip2;
    s32 flip4;
    Game113D60Burst *burst;

    burst = &effect->burst;
    burst->head.accum += burst->head.rate * D_800BE9A4;
    while ((burst->head.accum > 1.0f) && (burst->head.next != burst->head.count)) {
        position.x = burst->points[burst->head.next * 2];
        position.y = burst->points[burst->head.next * 2 + 1];
        randomSize = func_150ADA68();
        flip4 = (func_150ADA20() & 1) ? 4 : 0;
        flip2 = (func_150ADA20() & 1) ? 2 : 0;
        func_150E75A0(&position, (randomSize * 12.0f) + 30.0f, 300, (u8)(flip2 | flip4),
                      (func_150ADA68() * 25.0f) + 100.0f, 0xFF, 1, 0xFF, NULL, 0, effect->header.owner,
                      effect->header.kind);
        func_10010F30(0x360, 0x7FFF, (position.x * 0.4315068424f) + 64.0f, 0, 0);
        burst->head.next++;
        burst->head.accum -= 1.0f;
    }
    if (burst->head.next >= burst->head.count) {
        effect->header.state = -1;
    }
}

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
} Struct1513C650Arg; // first argument of func_1513C650 / func_1513C73C (passed as an s32)

/* Spawns a func_1513C650 effect of the given size at `position`, lasting `duration` frames
 * (-1: a fixed 300 frames without the timed flag). */
void func_150E7FEC(f32 size, u8 arg1, s32 arg2, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind) {
    s32 pad; // matching: unused stack slot
    Struct1513C650Arg arg;
    s32 argB;
    s32 argC;

    arg.unk22 = 20;
    arg.unk24 = 12;
    arg.unk06 = 0x4F;
    arg.unk07 = 0;
    arg.flags = ((duration == -1) ? 0 : 1) | (arg5 ? 0x400 : 0) | 0x100 | 0x200 | 0x1000 | 0x8000 | 0x40000;
    arg.unk04 = (duration == -1) ? 300 : duration + 20;
    arg.unk08 = 0;
    arg.unk0C = 0;
    arg.unk10 = arg1;
    arg.unk11 = 0xFF;
    arg.unk12 = arg7;
    arg.unk13 = arg8;
    arg.unk14 = arg9;
    arg.unk15 = 0xFF;
    arg.unk18 = ((arg6 != 0) ? 2 : 1) + 0x480000;
    arg.unk16 = 0;
    arg.unk17 = 6;
    if (arg5 != 0) {
        argB = 3;
        argC = 0xFF;
    } else {
        argB = 0;
        argC = 0;
    }
    func_1513C650(&arg, 0, 0, arg2, position->x, position->y, position->z, size, size, func_150ADA20() & 0xFF,
                  (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2), argB, argC, 0, owner, kind);
}

typedef struct {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ Vec3f position;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s8 unk58;
} Struct1514FCE8Arg; // first argument of func_1514FCE8

extern Vec3f D_800A1290[];
extern void func_151D3FF4(Vec3f *position, u8 owner, s32 kind);
extern void func_1514FCE8(Struct1514FCE8Arg *arg0, u8 owner, s32 kind);

/* For points 4-7 of the D_800A1290 table: spawns the func_151D3FF4 and func_1514FCE8 effects
 * 200 units above the point, plus an event. */
void func_150E81A8(index, owner, kind)
    u8 index;
    u8 owner;
    s32 kind;
{
    Vec3f position;
    Struct1514FCE8Arg arg;
    Game113D60Event event;

    if (index == 4 || index == 5 || index == 6 || index == 7) {
        position = D_800A1290[index];
        position.y += 200.0f;
        func_151D3FF4(&position, owner, kind);

        arg.unk00 = 0;
        arg.unk02 = 0xFF;
        arg.unk04 = -0x40;
        arg.unk06 = 0x4D;
        arg.unk08 = 10;
        arg.unk0C = 5;
        arg.position = position;
        arg.unk1C = 252.0f;
        arg.unk20 = 117.0f;
        arg.unk24 = 308.0f;
        arg.unk28 = 256.0f;
        arg.unk2C = 699.0f;
        arg.unk30 = 511.0f;
        arg.unk34 = 4;
        arg.unk38 = 7;
        arg.unk3C = 27.0f;
        arg.unk40 = 0.7820000648f;
        arg.unk44 = -0.9300000668f;
        arg.unk48 = 0.7300000191f;
        arg.unk4C = 25;
        arg.unk4E = 15;
        arg.unk50 = 100;
        arg.unk52 = 100;
        arg.unk54 = 12;
        arg.unk56 = 20;
        arg.unk58 = 0;

        func_1514FCE8(&arg, owner, kind);

        event.type = 1;
        event.duration = (func_150ADA20() % 11) + 30;
        event.unk4 = 8;
        event.unk6 = -1;
        event.unk5 = 1;
        func_151D8868(&event, 0, 0xFF, 0);
    }
}

typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 unkC;
} Game113D60PointData;

/* Creates effect 0x28 at `position`, lasting `duration` frames (-1: 300). */
void func_150E83AC(Vec3f *position, s16 duration, u8 owner, s32 kind) {
    Game113D60PointData data;
    struct260 *effect;

    data.position = *position;
    data.unkC = 0.0f;

    effect = func_15149130((duration == -1) ? 300 : duration, -1, 0x28, -1, (duration == -1) ? 0 : 1, 0, sizeof(data),
                           owner, kind);
    if (effect != NULL) {
        memcpy(((Game113D60AnyEffect *)effect)->data, &data, sizeof(data));
    }
}

/* Extra data of the particles func_150E8470 spawns, stored at 0xA8 of each particle. */
typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ f32 unk4;
    /* 0x8 */ f32 unk8;
} Game113D60ParticleExtra;

typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ Game113D60ParticleExtra extra;
} Game113D60Particle;

typedef struct {
    /* 0x0 */ Vec3f position;
    /* 0xC */ f32 accum;
} Game113D60Emitter;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ Game113D60Emitter emitter;
} Game113D60EmitterEffect;

extern Game113D60Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/* Spawns a random number of particles per frame at the effect's position. */
void func_150E8470(Game113D60EmitterEffect *effect) {
    Game113D60Emitter *emitter;

    emitter = &effect->emitter;
    emitter->accum += (0.09999999404f + func_150ADA68() * 0.08789999783f) * D_800BE9A4;
    if (emitter->accum > 1.0f) {
        ParticleDesc arg;
        Game113D60ParticleExtra extra;
        Game113D60Particle *particle;

        arg.texture = 0x6C;
        arg.renderMode = 0x5103;
        arg.unk0 = 0x200005;
        arg.fadeStart = 0x73;
        arg.fadeRate = 2;
        arg.flags = 0x80DE07;
        arg.primColorMode = 8;
        arg.envColorMode = 6;
        arg.updateFunc = 0x19;
        arg.deathFunc = -1;
        extra.unk0 = 0;
        extra.unk1 = 0;
        arg.unk4 = 0;
        arg.frame = 0;
        arg.frameRate = 0;
        arg.drawFunc = -1;
        arg.handlerSet = 0;
        arg.scaleStart = 0x73;
        arg.r = 0x24;
        arg.g = 0x22;
        arg.b = 0x11;
        arg.a = 0xFF;
        arg.r2 = 0x7B;
        arg.g2 = 0x93;
        arg.b2 = 0xAA;
        arg.brightness = 0xFF;
        arg.scaleRate = 1.022418022f;
        arg.pos = emitter->position;
        arg.offset.x = 0.0f;
        arg.offset.y = 0.0f;
        arg.offset.z = 0.0f;
        arg.vel.x = 0.0f;
        arg.vel.y = 0.0f;
        arg.vel.z = 0.0f;
        do {
            extra.unk2 = func_150ADA20() % 5 + 4;
            extra.unk3 = func_150ADA20() % 5 + 4;
            extra.unk4 = func_150ADA68() * 13.0f;
            extra.unk8 = func_150ADA68() * 13.0f;
            arg.gravity = (func_150ADA68() * 40.0f + 68.0f) * 0.001000000047f;
            arg.flags &= ~0xC0;
            arg.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            arg.alpha = func_150ADA20() % 156 + 100;
            arg.timer = func_150ADA20() % 34 + 141;
            arg.width = arg.height = func_150ADA68() * 71.0f + 80.0f;
            particle = func_15130280(&arg, 1, 0, sizeof(extra), effect->header.owner, effect->header.kind);
            if (particle != NULL) {
                memcpy(&particle->extra, &extra, sizeof(extra));
            }
            emitter->accum -= 1.0f;
        } while (emitter->accum > 1.0f);
    }
}

extern s32 func_15131828(Game113D60Particle *, f32 *, u8 *, u8 *);

/* Update callback of the particles func_150E8470 spawns. */
s32 func_150E8824(Game113D60Particle *particle, s32 arg1) {
    func_15131828(particle, &particle->extra.unk4, &particle->extra.unk0, &particle->extra.unk2);
    return 1;
}

/* Creates the timer effect (0x35) that func_150E88C0 updates. */
void func_150E8854(void) {
    struct260 *effect;
    f32 timer;

    timer = 10.0f;

    effect = func_15149130(300, -1, 0x35, -1, 0, 0, sizeof(timer), 0xFF, 1);
    if (effect != NULL) {
        memcpy(((Game113D60AnyEffect *)effect)->data, &timer, sizeof(timer));
    }
}

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ f32 timer;
} Game113D60TimerEffect;

// matching: unprototyped, since the call below also passes the effect (in a0) to this void function
extern void func_150E8930();

/* Counts the timer down and fires func_150E8930 each time it runs out (rearmed to 201-538). */
void func_150E88C0(Game113D60TimerEffect *effect) {
    effect->timer -= D_800BE9A4;
    if (effect->timer < 0.0f) {
        effect->timer = (func_150ADA68() * 337.0f) + 201.0f;
        func_150E8930(effect);
    }
}

extern Game113D60Ids2 D_80088A80;
extern u8 D_800BE9EB;
void func_150E8A80(void);
void func_150E90DC(void);

/* Fires the D_80088A80 pair through func_15169260 plus an event, effects 0x33 and 0x36 when the
 * D_800DCDC4 region list exists, and two sounds. */
void func_150E8930(void) {
    Game113D60Ids2 ids;
    Game113D60Event event;

    ids = D_80088A80;
    func_15169260(&ids, 2, 0, 0x1B);
    func_15164F0C(0, D_800BE9EB, NULL, 0xFF, 1);
    event.type = 1;
    event.duration = (func_150ADA20() % 21) + 20;
    event.unk5 = 1;
    event.unk4 = (func_150ADA20() % 6) + 3;
    event.unk6 = -1;
    func_151D8868(&event, 0, 0xFF, 1);
    if (D_800DCDC4 != 0) {
        func_150E8A80();
    }
    if (D_800DCDC4 != 0) {
        func_150E90DC();
    }
    func_10010F30(0x4C8, 0x7FFF, 0x40, 0x200 - (func_150ADA20() & 0x400), 0);
    func_10010F30(0x4CD, 0x5DC0, 0x40, 0x200 - (func_150ADA20() & 0x400), 0);
}

typedef struct {
    /* 0x0 */ f32 unk0;
    /* 0x4 */ f32 unk4;
    /* 0x8 */ f32 unk8;
} Game113D60Data8A80;

/* Creates effect 0x33 for 30-70 frames. */
void func_150E8A80(void) {
    struct260 *effect;
    Game113D60Data8A80 data;

    data.unk0 = 1.199999957e-07f;
    data.unk4 = 3.199999981e-07f;
    data.unk8 = 0.0f;

    effect = func_15149130((func_150ADA20() % 41) + 30, -1, 0x33, -1, 1, 0, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy(((Game113D60AnyEffect *)effect)->data, &data, sizeof(data));
    }
}

/* Node of the weighted region list at D_800DCDC4, whose weights add up to D_800DCD90. */
typedef struct Game113D60WeightedNode {
    /* 0x0 */ struct134 *region;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ f32 weight;
    /* 0xC */ struct Game113D60WeightedNode *next;
} Game113D60WeightedNode;

/* Spawn rate data shared by the region spawners: `rate + random * rateRange` per frame. */
typedef struct {
    /* 0x0 */ f32 rate;
    /* 0x4 */ f32 rateRange;
    /* 0x8 */ f32 accum;
} Game113D60Spawner;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ Game113D60Spawner spawner;
} Game113D60RegionSpawnerEffect;

typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} Game113D60Data8B1C;

extern Vec3f *func_15144B34(s32);

/* Spawns effect 0x34 at random points of the weighted regions within 1800 units of the
 * position func_15144B34 returns. */
void func_150E8B1C(Game113D60RegionSpawnerEffect *effect) {
    Vec3f *center;
    Game113D60Spawner *spawner;
    Game113D60WeightedNode *node;
    f32 pick;
    Game113D60Data8B1C data;
    Game113D60Data8B1C *dataPtr;
    f32 dx;
    f32 dy;
    f32 dz;

    dataPtr = &data; // matching: using &data directly at each use does not match
    center = func_15144B34(D_800BE9E8);
    spawner = &effect->spawner;
    spawner->accum += (spawner->rate + (func_150ADA68() * spawner->rateRange)) * D_800BE9A4 * D_800DCD90;
    if (spawner->accum > 1.0f) {
        do {
            pick = func_150ADA68() * D_800DCD90;
            node = (Game113D60WeightedNode *)D_800DCDC4;
            while (node->weight < pick) {
                pick -= node->weight;
                node = node->next;
            }
            func_1514470C(node->region, dataPtr);
            dx = data.position.x - center->x;
            dy = data.position.y - center->y;
            dz = data.position.z - center->z;
            if (dx * dx + dy * dy + dz * dz < 3240000.0f) {
                struct260 *child;

                data.unk0C = 0.3171199858f;
                data.unk10 = 1.046509981f;
                data.unk14 = 0.0f;
                child = func_15149130((func_150ADA20() % 13) + 5, -1, 0x34, -1, 1, 0, sizeof(data),
                                      effect->header.owner, effect->header.kind);
                if (child != NULL) {
                    memcpy(((Game113D60AnyEffect *)child)->data, dataPtr, sizeof(data));
                }
            }
            spawner->accum -= 1.0f;
        } while (spawner->accum > 1.0f);
    }
}

/* A spawner at a fixed point: `position`, then the spawn rate as in Game113D60Spawner. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ f32 accum;
} Game113D60PointSpawner;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ Game113D60PointSpawner spawner;
} Game113D60PointSpawnerEffect;

struct260 *func_15130374(ParticleDesc64 *, u8, s32, u8, s32);

/* Spawns func_15130374 particles within 20 units of the spawner's position. */
void func_150E8D5C(Game113D60PointSpawnerEffect *effect) {
    Game113D60PointSpawner *spawner;
    struct260 *particle;
    s32 flip80;
    s32 flip40;
    ParticleDesc64 arg;
    s32 pad; // matching: unused stack slot
    f32 extra;
    f32 zero; // matching: writing 0.0f literals instead does not match

    spawner = &effect->spawner;
    spawner->accum += (spawner->rate + (func_150ADA68() * spawner->rateRange)) * D_800BE9A4;
    if (spawner->accum > 1.0f) {
        zero = 0.0f;
        {
            f32 size;

            arg.texture = 0x16;
            arg.renderMode = 0xE01;
            arg.unk0 = 0x200005;
            arg.unk4 = 0;
            arg.frame = 0;
            arg.frameRate = 0;
            arg.flags = 0x801E05;
            arg.primColorMode = 3;
            arg.envColorMode = 3;
            arg.updateFunc = 0x1A;
            arg.deathFunc = -1;
            arg.offset.x = zero;
            arg.offset.y = zero;
            arg.offset.z = zero;
            arg.gravity = zero;
            arg.a = 0xFF;
            arg.brightness = 0xFF;
            arg.fadeStart = 30;
            arg.fadeRate = 8;
            arg.vel.x = zero;
            arg.vel.z = zero;
            do {
                extra = 0.9700999856f;
                arg.scaleStart = arg.timer = (func_150ADA20() & 0xF) + 40;
                arg.alpha = (func_150ADA20() % 119) + 100;
                size = (func_150ADA68() * 59.0f) + 80.0f;
                arg.height = arg.width = size;
                arg.scaleRate = ((func_150ADA68() * 3727.0f) + 99979.0f) * 9.999999747e-06f;
                arg.flags &= ~0xC0;
                if (func_150ADA20() & 1) {
                    flip80 = 0x80;
                } else {
                    flip80 = 0;
                }
                if (func_150ADA20() & 1) {
                    flip40 = 0x40;
                } else {
                    flip40 = 0;
                }
                arg.flags |= (u32)(flip40 | flip80);
                arg.r2 = 0x44;
                arg.g2 = 0x3C;
                arg.b2 = 0x27;
                arg.r = 0xF;
                arg.g = 0x11;
                arg.b = 5;
                func_151436B4(func_150ADA68() * 6.283185482f, func_150ADA68() * 6.283185482f, func_150ADA68() * 20.0f,
                              &arg.pos);
                arg.pos.x += spawner->position.x;
                arg.pos.y += spawner->position.y;
                arg.pos.z += spawner->position.z;
                arg.vel.y = ((func_150ADA68() * 471.0f) + -1242.0f) * 0.009999999776f;
                particle = func_15130374(&arg, 0, sizeof(extra), effect->header.owner, effect->header.kind);
                if (particle != NULL) {
                    memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
                }
                spawner->accum -= 1.0f;
            } while (spawner->accum > 1.0f);
        }
    }
}

typedef struct {
    /* 0x0 */ f32 unk0;
    /* 0x4 */ f32 unk4;
    /* 0x8 */ f32 unk8;
} Game113D60Data90DC;

/* Creates effect 0x36 for 5-30 frames. */
void func_150E90DC(void) {
    struct260 *effect;
    Game113D60Data90DC data;

    data.unk0 = 2.000000023e-07f;
    data.unk4 = 1.500000053e-07f;
    data.unk8 = 0.0f;

    effect = func_15149130((func_150ADA20() % 26) + 5, -1, 0x36, -1, 1, 0, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy(((Game113D60AnyEffect *)effect)->data, &data, sizeof(data));
    }
}

typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8 pad1C[0x14];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x38 */ s32 unk38;
} Game113D60Data9178;

/* Like func_150E8B1C, but spawns effect 0x37 within 1400 units. */
void func_150E9178(Game113D60RegionSpawnerEffect *effect) {
    Vec3f *center;
    Game113D60Spawner *spawner;
    Game113D60WeightedNode *node;
    f32 pick;
    Game113D60Data9178 data;
    f32 dx;
    f32 dy;
    f32 dz;

    center = func_15144B34(D_800BE9E8);
    spawner = &effect->spawner;
    spawner->accum += (spawner->rate + (func_150ADA68() * spawner->rateRange)) * D_800BE9A4 * D_800DCD90;
    if (spawner->accum > 1.0f) {
        do {
            pick = func_150ADA68() * D_800DCD90;
            node = (Game113D60WeightedNode *)D_800DCDC4;
            while (node->weight < pick) {
                pick -= node->weight;
                node = node->next;
            }
            func_1514470C(node->region, &data);
            dx = data.position.x - center->x;
            dy = data.position.y - center->y;
            dz = data.position.z - center->z;
            if (dx * dx + dy * dy + dz * dz < 1960000.0f) {
                struct260 *child;

                data.unk0C = 0.1932400018f;
                data.unk10 = 0.3140099943f;
                data.unk14 = 0.0f;
                data.unk18 = 99999.89844f;
                data.unk30 = 0;
                data.unk34 = 0;
                data.unk35 = 0;
                data.unk38 = 0;
                child = func_15149130((func_150ADA20() % 13) + 5, -1, 0x37, -1, 1, 0, sizeof(data),
                                      effect->header.owner, effect->header.kind);
                if (child != NULL) {
                    memcpy(((Game113D60AnyEffect *)child)->data, &data, sizeof(data));
                }
            }
            spawner->accum -= 1.0f;
        } while (spawner->accum > 1.0f);
    }
}

/* Extra data of the particles func_150E93DC spawns, stored at 0x170 of each particle. */
typedef struct {
    /* 0x00 */ f32 floorY;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x14];
} Game113D60DebrisExtra;

typedef struct {
    /* 0x000 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x028 */ u8 pad28[0x10];
    /* 0x038 */ Vec3f position;
    /* 0x044 */ u8 pad44[0x12C];
    /* 0x170 */ Game113D60DebrisExtra extra;
} Game113D60Debris;

extern u8 D_800A12F0[];
extern f32 D_800A12F4[];
extern Game113D60Debris *func_15132A4C(ParticleSpawn78 *, s32, s32, s32, u8, s32);

/* Spawns func_15132A4C debris particles (updated by func_150E971C) within 50 units of the
 * spawner's position. */
void func_150E93DC(Game113D60PointSpawnerEffect *effect) {
    Game113D60PointSpawner *spawner;
    Game113D60Debris *debris;
    ParticleSpawn78 arg;
    s32 i;
    Game113D60DebrisExtra extra; // copied into the particle uninitialized

    spawner = &effect->spawner;
    spawner->accum += (spawner->rate + (func_150ADA68() * spawner->rateRange)) * D_800BE9A4;
    if (spawner->accum > 1.0f) {
        {
            f32 pad; // matching: unused stack slot

            arg.radius = 0.0f;
            arg.elasticity = 1.0f;
            arg.scale.x = 1.0f;
            arg.scale.y = 1.0f;
            arg.scale.z = 1.0f;
            arg.flags = 0x49E8;
            arg.vel.x = 0.0f;
            arg.vel.y = 0.0f;
            arg.vel.z = 0.0f;
            arg.rotVel.y = 0.0f;
            arg.variant = 0;
            arg.unk5C = 0;
            arg.alpha = 0xFF;
            arg.update2Fn = 0;
            arg.floorHitFn = 0;
            arg.floorType3Fn = 0;
            arg.wallHitFn = 0;
            arg.destroyFn = 0;
            arg.squashFn = 0;
            arg.matrixFn = 2;
            arg.unk6A = 2;
            arg.owner = 0;
            arg.ownerId = 0;
            arg.fadeStart = 1;
            arg.fadeRate = 0xFF;
            do {
                i = func_150ADA20() % 1;
                arg.model = D_800A12F0[i];
                arg.sizeXZ = arg.sizeY = D_800A12F4[i] * ((func_150ADA68() * 351.0f) + 400.0f) * 0.001000000047f;
                arg.rot.x = func_150ADA68() * 360.0f;
                arg.rot.y = func_150ADA68() * 360.0f;
                arg.rot.z = func_150ADA68() * 360.0f;
                arg.gravity = ((func_150ADA68() * 423.0f) + -1401.0f) * 0.001000000047f;
                arg.lifetime = (func_150ADA20() % 1) + 100;
                arg.rotVel.x = ((func_150ADA68() * 27374.0f) + -13687.0f) * 0.001000000047f;
                arg.rotVel.z = ((func_150ADA68() * 27374.0f) + -13687.0f) * 0.001000000047f;
                func_151436B4(func_150ADA68() * 6.283185482f, func_150ADA68() * 6.283185482f, func_150ADA68() * 50.0f,
                              &arg.pos);
                arg.pos.x += spawner->position.x;
                arg.pos.y += spawner->position.y;
                arg.pos.z += spawner->position.z;
                arg.updateFn = 8;
                debris = func_15132A4C(&arg, 3, 0xFF, sizeof(extra), effect->header.owner, effect->header.kind);
                if (debris != NULL) {
                    memcpy(&debris->extra, &extra, sizeof(extra));
                }
                spawner->accum -= 1.0f;
            } while (spawner->accum > 1.0f);
        }
    }
}

extern s32 func_151337C0(Game113D60Debris *);

/* Update callback of the debris particles: when a particle falls below its floor height it
 * ends (returns 0), spawning a func_1513C73C splash first if its type is 0xA. */
s32 func_150E971C(Game113D60Debris *debris) {
    Game113D60DebrisExtra *extra;
    Struct1513C650Arg splash;

    func_151337C0(debris);
    extra = &debris->extra;
    if (debris->position.y < extra->floorY) {
        if ((extra->unk4 & 0x1F) == 0xA) {
            f32 size;

            size = (func_150ADA68() * 20.0f) + 20.0f;

            splash.unk06 = 0xB;
            splash.unk07 = 0;
            splash.flags = 0x9701;
            splash.unk04 = 100;
            splash.unk08 = 0;
            splash.unk0C = 0;
            splash.unk10 = 0xFF;
            splash.unk11 = 0xFF;
            splash.unk12 = 0xFF;
            splash.unk13 = 0xFF;
            splash.unk14 = 0xFF;
            splash.unk15 = 0xFF;
            splash.unk18 = 0x3B0003;
            splash.unk16 = 0;
            splash.unk17 = 7;
            splash.unk22 = 20;
            splash.unk24 = 12;

            func_1513C73C(&splash, 0, 0, (s32)extra->unk8, debris->position.x, extra->floorY + 5.0f,
                          debris->position.z, size, size, func_150ADA20() & 0xFF, 0, 0, debris->header.owner,
                          debris->header.kind);
        }
        return 0;
    }
    return 1;
}
