#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *memcpy(void *dst, const void *src, u32 size);

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 x, z; } Vec2f;

/* Events posted through func_151494E0 with a stream id as the payload. */
#define STREAM_EVENT_START 0x17 /* create the stream's emitter */
#define STREAM_EVENT_STOP  0x18 /* stop emitting; the particles still fall */
#define STREAM_EVENT_ENDED 0x23 /* the emitter was destroyed */

/* Emitter flags (Emitter.flags). */
#define EMITTER_FLAG_EMITTING 2
#define EMITTER_FLAG_STOPPED  8

/* A level-placed stream: a column of particles falling from y + height down to y. */
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 size;
    s16 height;
    u8 padA[0xA];
    u8 unk14; /* 0 while the stream has an emitter, 1 once it ended */
    u8 pad15[3];
    s32 id;
    s32 unk1C; /* copied to StreamData.unk4C */
} StreamSource;

/* One particle of a stream (0x1C bytes); the emitter keeps them in a ring. */
typedef struct {
    Vec3f pos;
    f32 velocity;
    f32 width;
    s16 alpha;
    f32 tc; /* texture t coordinate */
} StreamParticle;

/* Per-stream state, the 0x50-byte data block of a stream emitter. */
typedef struct {
    f32 swayAmpX;
    f32 swayAmpZ;
    f32 swayPhaseX;
    f32 swayPhaseZ;
    f32 swayRateX;
    f32 swayRateZ;
    Vec2f spawnPos; /* where the last particle was spawned */
    f32 unk20;
    f32 baseWidth;
    f32 widthAmp;
    f32 widthPhase;
    f32 widthRate;
    f32 width;
    f32 unk38;
    f32 tc;
    f32 spawnCount; /* particles owed; one is spawned per whole unit */
    f32 floorY;
    s32 id;
    u8 unk4C;
    u8 pad4D[3];
} StreamData;

typedef struct Emitter {
    u8 pad0[8];
    struct Emitter *next;
    u8 padC[4];
    Vec3f pos;
    u8 pad1C[2];
    u16 flags;
    s32 kind; /* 0xB for a stream */
    u8 pad24;
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 tail; /* oldest particle */
    s8 head; /* next free slot */
    u8 pad2F;
    u8 unk30;
    u8 pad31[0x23];
    Vec3f prevPos;
    u8 pad60[0x24];
    u8 buffers[0x10];
    StreamParticle *particles;
    StreamData *data;
} Emitter;

/* Template for a new emitter, passed to func_15147A80. */
typedef struct {
    Vec3f pos;
    s16 unkC;
    s16 unkE;
    s32 kind;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
} EmitterTemplate;

/* The spawner task's state: owns at most one emitter for its source. */
typedef struct {
    StreamSource *source;
    s32 id;
    Emitter *emitter;
    u8 unkC;
    u8 padD[3];
} StreamSpawner;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    StreamSpawner spawner;
} StreamTask;

Emitter *func_151CD4C0();
void func_151CE47C(Emitter *emitter);
Emitter *func_15147A80(EmitterTemplate *template, s32 dataSize, s32 particleSize, s32 arg3, s32 arg4, s32 arg5,
                       s32 arg6, s32 arg7, s32 arg8, u8 arg9, s32 argA);

/* Creates the spawner task for a stream source. */
StreamTask *func_151CD2C0(StreamSource *source, u8 arg1, s32 arg2) {
    StreamTask *task;
    StreamSpawner spawner;

    spawner.source = source;
    spawner.id = source->id;
    spawner.emitter = NULL;
    spawner.unkC = source->unk1C;

    task = (StreamTask *)func_15149130(0x12C, -1, -1, -1, 0, 0x1E, (struct37 *)0x10, arg1, arg2);
    if (task != NULL) {
        memcpy(&task->spawner, &spawner, sizeof(spawner));
    }
    return task;
}

/* Starts stream `id` (0-3). */
void func_151CD35C(s32 id) {
    s32 payload;

    if (id >= 0 && id < 4) {
        payload = id;
        func_151494E0((s32)&payload, STREAM_EVENT_START);
    }
}

/* Stops stream `id` (0-3). */
void func_151CD394(s32 id) {
    s32 payload;

    if (id >= 0 && id < 4) {
        payload = id;
        func_151494E0((s32)&payload, STREAM_EVENT_STOP);
    }
}

/* Event handler of a stream spawner task. */
void func_151CD3CC(StreamTask *task, s32 *id, u8 event) {
    StreamSpawner *spawner;
    s32 unused[3]; // matching: the shipped -g3 frame reserves 12 unreferenced bytes here

    spawner = &task->spawner;
    if (event == STREAM_EVENT_START) {
        if (spawner->id == *id) {
            if (spawner->emitter == NULL) {
                spawner->emitter = func_151CD4C0(spawner->source, spawner->unkC, spawner->id, task->unkC, task->unk1);
                spawner->source->unk14 = 0;
            label: ; // matching: the empty block ends the if, so as1 fills the exit branch's delay slot with the ra reload
            }
        }
    } else if (event == STREAM_EVENT_STOP) {
        if (spawner->id == *id) {
            if (spawner->emitter != NULL) {
                func_151CE47C(spawner->emitter);
            }
        }
    } else if (event == STREAM_EVENT_ENDED) {
        if (spawner->id == *id) {
            spawner->emitter = NULL;
            spawner->source->unk14 = 1;
        }
    }
}

/* Creates the emitter for a stream source. */
Emitter *func_151CD4C0(source, unkC, id, arg3, arg4)
    StreamSource *source;
    u8 unkC;
    s32 id;
    u8 arg3;
    s32 arg4;
{
    Emitter *emitter;
    EmitterTemplate template;
    StreamData data;
    f32 size;

    size = source->size;
    template.unk15 = 0x23;
    template.pos.x = source->x;
    template.pos.y = source->y + source->height;
    template.pos.z = source->z;
    template.unkC = 0x12C;
    template.unkE = 0x34;
    template.kind = 0xB;
    data.floorY = source->y;
    data.swayAmpX = size * 0.125f;
    data.swayRateX = 0.03000000119f; // matching: these three ROM constants sit one ulp above 0.03f, 0.04f and 0.055f
    data.swayPhaseX = 0.0f;
    data.swayPhaseZ = 0.0f;
    data.swayAmpZ = size * 0.125f;
    data.swayRateZ = 0.04000000283f;
    data.spawnPos.x = template.pos.x;
    data.spawnPos.z = template.pos.z;
    data.baseWidth = size * 0.875f;
    data.width = data.baseWidth;
    data.widthAmp = size * 0.0625f;
    data.widthPhase = 0.0f;
    data.widthRate = 0.05500000343f;
    data.spawnCount = 0.0f;
    data.id = id;
    data.unk38 = -16384.0f;
    data.tc = -16384.0f;
    data.unk20 = 0.0f;
    data.unk4C = unkC;
    template.unk18 = 0;
    template.unk14 = 7;
    emitter = func_15147A80(&template, sizeof(StreamData), sizeof(StreamParticle), 9, 9, 9, 0, 0, 0, arg3, arg4);
    if (emitter != NULL) {
        memcpy(emitter->data, &data, sizeof(data));
    }
    return emitter;
}

/* Moves a stream's particles under gravity; those that reach the floor are retired.
 * Returns 0 once a stopped stream has run dry. */
s32 func_151CD674(Emitter *emitter) {
    StreamData *data;
    StreamParticle *particles;
    StreamParticle *particle;
    s32 i;
    f32 velocity;
    f32 gravity;
    f32 halfGravity;
    f32 dt;
    f32 travel;
    f32 dtSquared;

    data = emitter->data;
    particles = emitter->particles;
    if (emitter->count < 2 && (emitter->flags & EMITTER_FLAG_STOPPED)) {
        return 0;
    }
    gravity = -0.25500003f;
    halfGravity = -0.12750001f;
    i = emitter->head;
    while (i != emitter->tail) {
        i--;
        if (i < 0) {
            i = emitter->capacity - 1;
        }
        particle = &particles[i];
        velocity = particle->velocity;
        dt = D_800BE9A4;
        travel = velocity * dt;
        dtSquared = dt * dt;
        particle->pos.y += travel + halfGravity * dtSquared;
        particle->velocity = (f32)velocity + gravity * D_800BE9A4; // matching: the no-op cast keeps velocity as the first add operand
        if (particle->pos.y < data->floorY) {
            while (i != emitter->tail) {
                emitter->tail++;
                if (emitter->tail == emitter->capacity) {
                    emitter->tail = 0;
                }
                emitter->count--;
            }
            particles[emitter->tail].pos.y = data->floorY;
        }
    }
    emitter->prevPos = emitter->pos;
    return 1;
}

/* Spawns a stream's new particles at the emitter, swaying in x/z and wobbling in width. */
s32 func_151CD7BC(Emitter *emitter) {
    StreamData *data;
    StreamParticle *particles;
    StreamParticle *particle;
    f32 unk20;
    f32 unk20Step;
    f32 dz;
    f32 dx;
    Vec2f pos;
    f32 step;
    f32 width;
    f32 dWidth;
    f32 tc;
    f32 velocity;
    f32 dTc;
    f32 gravity;
    f32 halfGravity;
    s32 tail;

    data = emitter->data;
    particles = emitter->particles;
    data->swayPhaseX += data->swayRateX * D_800BE9A4;
    data->swayPhaseZ += data->swayRateZ * D_800BE9A4;
    data->swayPhaseX = func_15144B68(data->swayPhaseX);
    data->swayPhaseZ = func_15144B68(data->swayPhaseZ);
    data->widthPhase += data->widthRate * D_800BE9A4;
    data->widthPhase = func_15144B68(data->widthPhase);
    data->spawnCount += 0.2730129957f * D_800BE9A4;
    data->tc += (10220.0f + func_150ADA68() * 9434.0f) * 0.01f * D_800BE9A4;
    while (data->tc >= 16384.0f) {
        data->tc -= 16384.0f;
    }
    if (data->spawnCount > 1.0f) {
        step = 1.0f / data->spawnCount;
        unk20 = data->unk20 + D_800BE9A4;
        unk20Step = -(unk20 * step);
        pos = data->spawnPos;
        dx = (emitter->pos.x + sinf(data->swayPhaseX) * data->swayAmpX) - data->spawnPos.x;
        dz = (emitter->pos.z + sinf(data->swayPhaseZ) * data->swayAmpZ) - data->spawnPos.z;
        dWidth = (data->baseWidth + sinf(data->widthPhase) * data->widthAmp) - data->width;
        dTc = data->tc - data->unk38;
        width = data->width;
        tc = data->tc;
        gravity = -0.255000025f;
        halfGravity = -0.1275000125f;
        velocity = -3.772f;
        while (data->spawnCount > 1.0f) {
            particle = &particles[emitter->head];
            particle->pos.x = pos.x;
            particle->pos.y = emitter->pos.y;
            particle->pos.z = pos.z;
            particle->velocity = velocity;
            particle->width = width;
            particle->alpha = 0xFF;
            particle->tc = tc;
            particle->pos.y += velocity * D_800BE9A4 + halfGravity * (D_800BE9A4 * D_800BE9A4);
            particle->velocity = (f32)velocity + gravity * D_800BE9A4; // matching: see func_151CD674
            emitter->head++;
            if (emitter->head == emitter->capacity) {
                emitter->head = 0;
            }
            /* The ring is full: drop the oldest particle. */
            tail = emitter->tail;
            emitter->count++;
            if (tail == emitter->head) {
                emitter->tail = tail + 1;
                if (emitter->tail == emitter->capacity) {
                    emitter->tail = 0;
                }
                emitter->count--;
            }
            pos.x += dx * step;
            pos.z += dz * step;
            width += dWidth * step;
            tc += dTc * step;
            unk20 += unk20Step;
            data->spawnCount -= 1.0f;
        }
        data->spawnPos = pos;
        data->width = width;
        data->unk38 = tc;
        data->unk20 = unk20;
    }
    return 1;
}

typedef struct Game1FA770Trail {
    f32 x;
    f32 y;
    u8 pad08[0xC];
    s16 alpha;
    u8 pad16[0x6];
} Game1FA770Trail;

s32 func_151CDB94(u8 *actor) {
    Game1FA770Trail *trail;
    Game1FA770Trail *start;
    Game1FA770Trail *cur;
    s32 i;
    f32 range;
    f32 inv;
    f32 d;
    f32 fade;
    f32 k;

    trail = *(Game1FA770Trail **)(actor + 0x94);
    if (*(s8 *)(actor + 0x2C) <= 0) {
        return 1;
    }
    i = *(s8 *)(actor + 0x2E) - 1;
    if (i < 0) {
        i = actor[0x25] - 1;
    }
    start = &trail[*(s8 *)(actor + 0x2D)];
    cur = &trail[i];
    range = fabsf(cur->y - start->y);
    if (range == 0.0f) {
        return 1;
    }
    inv = 1.0f / range;
    fade = inv * 10.0f;
    i = *(s8 *)(actor + 0x2D);
    do {
        cur = &trail[i];
        d = cur->y - start->y;
        cur->alpha = 0xFF;
        if (d < range * 0.1970000118f) {
            k = inv * 5.076141834f;
            cur->alpha = (u8)(d * k * 255.0f);
        } else if (range - range * 0.1000000015f < d) {
            cur->alpha = (u8)((range - d) * fade * 255.0f);
        }
        i++;
        if (i >= actor[0x25]) {
            i = 0;
        }
    } while (i != *(s8 *)(actor + 0x2E));
    return 1;
}

typedef struct {
    f32 x, y, z;
} Vec3_151CDE20;

typedef struct {
    Vec3_151CDE20 pos;
    f32 unkC;
    f32 width;
    s16 alpha;
    s16 pad16;
    f32 tc;
} Seg151CDE20;

typedef struct {
    char pad0[0x4C];
    u8 unk4C;
} Sub151CDE20;

typedef struct {
    char pad0[0x10];
    Vec3_151CDE20 pos;
    char pad1C[2];
    u16 flags;
    char pad20[5];
    u8 count;
    char pad26[6];
    s8 unk2C;
    s8 last;
    s8 first;
    char pad2F[0x84 - 0x2F];
    char buffers[0x10];
    Seg151CDE20 *segs;
    Sub151CDE20 *sub;
} Obj151CDE20;

extern s32 D_800AAEE0[];
extern u8 D_80090B60[][12];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
void func_151D5D60(void *, s16, s32, void **, u8 *);

Gfx *func_151CDE20(Obj151CDE20 *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    Seg151CDE20 *segs;
    Sub151CDE20 *sub;
    u8 update;
    s32 i;
    s32 k;
    Vec3_151CDE20 b;
    Vec3_151CDE20 a;
    f32 dx;
    f32 dz;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->buffers, arg2, (arg0->count * 2 + 10) * sizeof(Vtx), (void **)&vtx, 0);
        if (vtx != NULL) {
            sub = arg0->sub;
            segs = arg0->segs;
            update = 1;
            gfx = func_15142E24(gfx, (s32)D_80090B60[D_800AAEE0[sub->unk4C]], 0, 0, 0, 0, D_800AAEE0[sub->unk4C], 0, 0, &update, 3);
            gfx = func_1513F4E4(gfx, 0x4E, &update);
            gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
            gfx = func_15142FBC(gfx, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &update);
            if (arg0->flags & 2) {
                i = arg0->first - 1;
                if (i < 0) i = arg0->count - 1;
                a = arg0->pos;
                k = i;
            } else {
                k = arg0->first - 1;
                if (k < 0) {
                    k = arg0->count - 1;
                }
                i = k - 1;
                if (i < 0) {
                    i = arg0->count - 1;
                }
                a = segs[k].pos;
            }
            b = segs[i].pos;
            dx = D_800DD1D8[arg2] * segs[k].width;
            dz = D_800DD1E8[arg2] * segs[k].width;
            vtx->v.ob[0] = a.x + dz;
            vtx->v.ob[1] = a.y;
            vtx->v.ob[2] = a.z - dx;
            vtx->v.tc[0] = 0;
            vtx->v.tc[1] = segs[k].tc;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = segs[k].alpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = a.x - dz;
            vtx->v.ob[1] = a.y;
            vtx->v.ob[2] = a.z + dx;
            vtx->v.tc[0] = 0x7FF;
            vtx->v.tc[1] = segs[k].tc;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = segs[k].alpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                dx = D_800DD1D8[arg2] * segs[i].width;
                dz = D_800DD1E8[arg2] * segs[i].width;
                vtx->v.ob[0] = b.x + dz;
                vtx->v.ob[1] = b.y;
                vtx->v.ob[2] = b.z - dx;
                vtx->v.tc[0] = 0;
                vtx->v.tc[1] = segs[i].tc;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = segs[i].alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = b.x - dz;
                vtx->v.ob[1] = b.y;
                vtx->v.ob[2] = b.z + dx;
                vtx->v.tc[0] = 0x7FF;
                vtx->v.tc[1] = segs[i].tc;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = segs[i].alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (segs[k].tc < segs[i].tc) {
                    memcpy(vtx, vtx - 2, 0x20);
                    vtx[-2].v.tc[1] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[1] -= 0x4000;
                    vtx++;
                }
                k = i;
                i--;
                if (i < 0) {
                    i = arg0->count - 1;
                }
                a = segs[k].pos;
                b = segs[i].pos;
            } while (k != arg0->last);
        }
    }
    return gfx;
}

/* Stops a stream emitter: no new particles, the ones in flight keep falling. */
void func_151CE47C(Emitter *emitter) {
    emitter->unk30 = 0;
    emitter->flags &= ~EMITTER_FLAG_EMITTING;
    emitter->flags |= EMITTER_FLAG_STOPPED;
}

extern void func_151478F4(void *);
extern void func_15147928(s32);

/* Destroy callbacks: tell the spawner its stream ended, then free the emitter. */
void func_151CE49C(Emitter *emitter) {
    s32 payload;
    StreamData *data;

    data = emitter->data;
    payload = data->id;
    func_151494E0((s32)&payload, STREAM_EVENT_ENDED);
    func_151478F4(emitter);
}

void func_151CE4DC(Emitter *emitter) {
    s32 payload;
    StreamData *data;

    data = emitter->data;
    payload = data->id;
    func_151494E0((s32)&payload, STREAM_EVENT_ENDED);
    func_15147928((s32)emitter);
}

struct Obj151CE634 *func_151CE634(s32);
void func_1515C1A0(void *, void *, f32 *, f32 *);
s32 func_1505D024(void *, s32, u16, s32);

typedef struct {
    f32 position[3];
    u8 unkC[0x10];
} Game1FA770TrailPoint;

void func_151CE51C(void *arg0, void *arg1) {
    f32 position[3];
    f32 radius;
    f32 height;
    u8 *obj;

    obj = (u8 *)func_151CE634(*(s32 *)((u8 *)arg1 + 0x18));
    if (obj != 0) {
        func_1515C1A0(arg0, position, &radius, &height);
        if (*(u16 *)(obj + 0x1E) & 8) {
            f32 bottom;
            f32 level;

            bottom = position[1] - height;
            if (*(s8 *)(obj + 0x2C) > 0) {
                s32 index;
                Game1FA770TrailPoint *points;

                index = *(s8 *)(obj + 0x2E) - 1;
                if (index < 0) {
                    index = obj[0x25] - 1;
                }
                points = *(Game1FA770TrailPoint **)(obj + 0x94);
                level = points[index].position[1];
            } else {
                return;
            }
            if (bottom <= level) {
                func_1505D024(arg0, 0x60019, 0, -1);
            }
        } else {
            f32 top;
            f32 level;

            top = position[1] + height;
            if (*(s8 *)(obj + 0x2C) > 0) {
                Game1FA770TrailPoint *points;

                points = *(Game1FA770TrailPoint **)(obj + 0x94);
                level = points[*(s8 *)(obj + 0x2D)].position[1];
            } else {
                return;
            }
            if (level <= top) {
                func_1505D024(arg0, 0x60019, 0, -1);
            }
        }
    }
}

typedef struct Obj151CE634Sub {
    u8 pad0[0x48];
    s32 unk48;
} Obj151CE634Sub;

typedef struct Obj151CE634 {
    u8 pad0[8];
    struct Obj151CE634 *next;
    u8 padC[0x20 - 0xC];
    s32 unk20;
    u8 pad24[0x98 - 0x24];
    Obj151CE634Sub *unk98;
} Obj151CE634;

extern s32 D_800A5760[];
extern Obj151CE634 *D_800DCE50[][0x68];

Obj151CE634 *func_151CE634(s32 arg0) {
    u8 i;
    s32 j;
    Obj151CE634 *p;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            p = D_800DCE50[j][D_800A5760[i]];
            while (p != NULL) {
                if (p->unk20 == 0xB) {
                    Obj151CE634Sub *sub = p->unk98;

                    if (sub->unk48 == arg0) {
                        return p;
                    }
                }
                p = p->next;
            }
        }
    }
    return NULL;
}
