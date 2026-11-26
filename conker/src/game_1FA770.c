#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *memcpy(void *dst, const void *src, u32 size);

/* Events posted through func_151494E0 with a stream id as the payload. */
#define STREAM_EVENT_START 0x17 /* create the stream's emitter */
#define STREAM_EVENT_STOP  0x18 /* stop emitting; the particles still fall */
#define STREAM_EVENT_ENDED 0x23 /* the emitter was destroyed */

#define STREAM_KIND 0xB /* head.tmpl.kind of a LevelStream */

/* LevelStream flags (head.tmpl.flags). */
#define EMITTER_FLAG_EMITTING 2
#define EMITTER_FLAG_STOPPED  8


/* Creates the spawner task for a stream source. */
StreamTask *func_151CD2C0(TriggerZone *source, u8 arg1, s32 arg2) {
    StreamTask *task;
    StreamSpawner spawner;

    spawner.source = source;
    spawner.id = source->data1;
    spawner.emitter = NULL;
    spawner.textureIndex = source->data2;

    task = func_15149130(0x12C, -1, -1, -1, 0, 0x1E, 0x10, arg1, arg2);
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
                spawner->emitter = func_151CD4C0(spawner->source, spawner->textureIndex, spawner->id, task->base.header.owner, task->base.header.kind);
                spawner->source->inactive = 0;
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
            spawner->source->inactive = 1;
        }
    }
}

/* Creates the emitter for a stream source. */
LevelStream *func_151CD4C0(source, textureIndex, id, arg3, arg4)
    TriggerZone *source;
    u8 textureIndex;
    s32 id;
    u8 arg3;
    s32 arg4;
{
    LevelStream *emitter;
    EffectTemplate template;
    StreamData data;
    f32 size;

    size = source->xSize;
    template.itemCount = 0x23;
    template.pos.x = source->x;
    template.pos.y = source->y + source->ySize;
    template.pos.z = source->z;
    template.life = 0x12C;
    template.flags = 0x34;
    template.kind = STREAM_KIND;
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
    data.textureIndex = textureIndex;
    template.material = 0;
    template.unk14 = 7;
    emitter = func_15147A80(&template, sizeof(StreamData), sizeof(StreamParticle), 9, 9, 9, 0, 0, 0, arg3, arg4);
    if (emitter != NULL) {
        memcpy(emitter->data, &data, sizeof(data));
    }
    return emitter;
}

/* Moves a stream's particles under gravity; those that reach the floor are retired.
 * Returns 0 once a stopped stream has run dry. */
s32 func_151CD674(LevelStream *emitter) {
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
    if (emitter->head.count < 2 && (emitter->head.tmpl.flags & EMITTER_FLAG_STOPPED)) {
        return 0;
    }
    gravity = -0.25500003f;
    halfGravity = -0.12750001f;
    i = emitter->head.end;
    while (i != emitter->head.start) {
        i--;
        if (i < 0) {
            i = emitter->head.tmpl.itemCount - 1;
        }
        particle = &particles[i];
        velocity = particle->velocity;
        dt = D_800BE9A4;
        travel = velocity * dt;
        dtSquared = dt * dt;
        particle->pos.y += travel + halfGravity * dtSquared;
        particle->velocity = (f32)velocity + gravity * D_800BE9A4; // matching: the no-op cast keeps velocity as the first add operand
        if (particle->pos.y < data->floorY) {
            while (i != emitter->head.start) {
                emitter->head.start++;
                if (emitter->head.start == emitter->head.tmpl.itemCount) {
                    emitter->head.start = 0;
                }
                emitter->head.count--;
            }
            particles[emitter->head.start].pos.y = data->floorY;
        }
    }
    emitter->head.modelPos = emitter->head.tmpl.pos;
    return 1;
}

/* Spawns a stream's new particles at the emitter, swaying in x/z and wobbling in width. */
s32 func_151CD7BC(LevelStream *emitter) {
    StreamData *data;
    StreamParticle *particles;
    StreamParticle *particle;
    f32 unk20;
    f32 unk20Step;
    f32 dz;
    f32 dx;
    Vec2fXZ pos;
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
        dx = (emitter->head.tmpl.pos.x + sinf(data->swayPhaseX) * data->swayAmpX) - data->spawnPos.x;
        dz = (emitter->head.tmpl.pos.z + sinf(data->swayPhaseZ) * data->swayAmpZ) - data->spawnPos.z;
        dWidth = (data->baseWidth + sinf(data->widthPhase) * data->widthAmp) - data->width;
        dTc = data->tc - data->unk38;
        width = data->width;
        tc = data->tc;
        gravity = -0.255000025f;
        halfGravity = -0.1275000125f;
        velocity = -3.772f;
        while (data->spawnCount > 1.0f) {
            particle = &particles[emitter->head.end];
            particle->pos.x = pos.x;
            particle->pos.y = emitter->head.tmpl.pos.y;
            particle->pos.z = pos.z;
            particle->velocity = velocity;
            particle->width = width;
            particle->alpha = 0xFF;
            particle->tc = tc;
            particle->pos.y += velocity * D_800BE9A4 + halfGravity * (D_800BE9A4 * D_800BE9A4);
            particle->velocity = (f32)velocity + gravity * D_800BE9A4; // matching: see func_151CD674
            emitter->head.end++;
            if (emitter->head.end == emitter->head.tmpl.itemCount) {
                emitter->head.end = 0;
            }
            /* The ring is full: drop the oldest particle. */
            tail = emitter->head.start;
            emitter->head.count++;
            if (tail == emitter->head.end) {
                emitter->head.start = tail + 1;
                if (emitter->head.start == emitter->head.tmpl.itemCount) {
                    emitter->head.start = 0;
                }
                emitter->head.count--;
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

/* Fades a stream's ends: alpha ramps up over the lowest 19.7% of its height and down over the top 10%. */
s32 func_151CDB94(LevelStream *emitter) {
    StreamParticle *particles;
    StreamParticle *oldest;
    StreamParticle *particle;
    s32 i;
    f32 height;
    f32 invHeight;
    f32 rise;
    f32 fadeOutScale;
    f32 fadeInScale;

    particles = emitter->particles;
    if (emitter->head.count <= 0) {
        return 1;
    }
    i = emitter->head.end - 1;
    if (i < 0) {
        i = emitter->head.tmpl.itemCount - 1;
    }
    oldest = &particles[emitter->head.start];
    particle = &particles[i];
    height = fabsf(particle->pos.y - oldest->pos.y);
    if (height == 0.0f) {
        return 1;
    }
    invHeight = 1.0f / height;
    fadeOutScale = invHeight * 10.0f;
    i = emitter->head.start;
    do {
        particle = &particles[i];
        rise = particle->pos.y - oldest->pos.y;
        particle->alpha = 0xFF;
        if (rise < height * 0.1970000118f) {
            fadeInScale = invHeight * 5.076141834f; /* 1 / 0.197 */
            particle->alpha = (u8)(rise * fadeInScale * 255.0f);
        } else if (height - height * 0.1f < rise) {
            particle->alpha = (u8)((height - rise) * fadeOutScale * 255.0f);
        }
        i++;
        if (i >= emitter->head.tmpl.itemCount) {
            i = 0;
        }
    } while (i != emitter->head.end);
    return 1;
}

extern s32 D_800AAEE0[];
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/* Draws a stream as a camera-facing ribbon: one quad per pair of neighbouring particles, newest to oldest. */
Gfx *func_151CDE20(LevelStream *emitter, Gfx *gfx, s16 view) {
    Vtx *vtx;
    StreamParticle *particles;
    StreamData *data;
    u8 update;
    s32 next;
    s32 cur;
    Vec3f nextPos;
    Vec3f curPos;
    f32 offsetZ;
    f32 offsetX;

    if (emitter->head.count >= 2) {
        func_151D5D60(emitter->buffers, view, (emitter->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            data = emitter->data;
            particles = emitter->particles;
            update = 1;
            gfx = func_15142E24(gfx, &D_80090B60[D_800AAEE0[data->textureIndex]], 0, 0, 0, 0, D_800AAEE0[data->textureIndex], 0,
                                0, &update, 3);
            gfx = func_1513F4E4(gfx, 0x4E, &update);
            gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
            gfx = func_15142FBC(gfx,
                                D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                                D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &update);
            /* While emitting, the ribbon starts at the emitter itself. */
            if (emitter->head.tmpl.flags & EMITTER_FLAG_EMITTING) {
                next = emitter->head.end - 1;
                if (next < 0) next = emitter->head.tmpl.itemCount - 1; // matching: one line, so -g3 line numbers keep as1's order
                curPos = emitter->head.tmpl.pos;
                cur = next;
            } else {
                cur = emitter->head.end - 1;
                if (cur < 0) {
                    cur = emitter->head.tmpl.itemCount - 1;
                }
                next = cur - 1;
                if (next < 0) {
                    next = emitter->head.tmpl.itemCount - 1;
                }
                curPos = particles[cur].pos;
            }
            nextPos = particles[next].pos;
            offsetZ = D_800DD1D8[view] * particles[cur].width;
            offsetX = D_800DD1E8[view] * particles[cur].width;
            vtx->v.ob[0] = curPos.x + offsetX;
            vtx->v.ob[1] = curPos.y;
            vtx->v.ob[2] = curPos.z - offsetZ;
            vtx->v.tc[0] = 0;
            vtx->v.tc[1] = particles[cur].tc;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = particles[cur].alpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = curPos.x - offsetX;
            vtx->v.ob[1] = curPos.y;
            vtx->v.ob[2] = curPos.z + offsetZ;
            vtx->v.tc[0] = 0x7FF;
            vtx->v.tc[1] = particles[cur].tc;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = particles[cur].alpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                offsetZ = D_800DD1D8[view] * particles[next].width;
                offsetX = D_800DD1E8[view] * particles[next].width;
                vtx->v.ob[0] = nextPos.x + offsetX;
                vtx->v.ob[1] = nextPos.y;
                vtx->v.ob[2] = nextPos.z - offsetZ;
                vtx->v.tc[0] = 0;
                vtx->v.tc[1] = particles[next].tc;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = particles[next].alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = nextPos.x - offsetX;
                vtx->v.ob[1] = nextPos.y;
                vtx->v.ob[2] = nextPos.z + offsetZ;
                vtx->v.tc[0] = 0x7FF;
                vtx->v.tc[1] = particles[next].tc;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = particles[next].alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                /* The texture coordinate wrapped: restart the strip from a copy shifted back by one period. */
                if (particles[cur].tc < particles[next].tc) {
                    memcpy(vtx, vtx - 2, 2 * sizeof(Vtx));
                    vtx[-2].v.tc[1] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[1] -= 0x4000;
                    vtx++;
                }
                cur = next;
                next--;
                if (next < 0) {
                    next = emitter->head.tmpl.itemCount - 1;
                }
                curPos = particles[cur].pos;
                nextPos = particles[next].pos;
            } while (cur != emitter->head.start);
        }
    }
    return gfx;
}

/* Stops a stream emitter: no new particles, the ones in flight keep falling. */
void func_151CE47C(LevelStream *emitter) {
    emitter->head.updateKind2 = 0;
    emitter->head.tmpl.flags &= ~EMITTER_FLAG_EMITTING;
    emitter->head.tmpl.flags |= EMITTER_FLAG_STOPPED;
}

/* Destroy callbacks: tell the spawner its stream ended, then free the emitter. */
void func_151CE49C(LevelStream *emitter) {
    s32 payload;
    StreamData *data;

    data = emitter->data;
    payload = data->id;
    func_151494E0((s32)&payload, STREAM_EVENT_ENDED);
    func_151478F4(emitter);
}

void func_151CE4DC(LevelStream *emitter) {
    s32 payload;
    StreamData *data;

    data = emitter->data;
    payload = data->id;
    func_151494E0((s32)&payload, STREAM_EVENT_ENDED);
    func_15147928(emitter);
}


/* Sends `actor` event 0x60019 when its collision cylinder reaches the falling column of `source`'s stream. */
void func_151CE51C(Actor *actor, TriggerZone *source) {
    Vec3f pos;
    f32 radius;
    f32 height;
    LevelStream *emitter;

    emitter = func_151CE634(source->data1);
    if (emitter != NULL) {
        func_1515C1A0(actor, &pos, &radius, &height);
        if (emitter->head.tmpl.flags & EMITTER_FLAG_STOPPED) {
            /* The column has left the emitter: its top is the newest particle. */
            f32 bottom;
            f32 columnTop;

            bottom = pos.y - height;
            if (emitter->head.count <= 0) {
                return;
            }
            {
                s32 newest;
                StreamParticle *particles;

                newest = emitter->head.end - 1;
                if (newest < 0) {
                    newest = emitter->head.tmpl.itemCount - 1;
                }
                particles = emitter->particles;
                columnTop = particles[newest].pos.y;
            }
            if (bottom <= columnTop) {
                func_1505D024(actor, 0x60019, 0, -1);
            }
        } else {
            /* Still attached to the emitter: its bottom is the oldest particle. */
            f32 top;
            f32 columnBottom;

            top = pos.y + height;
            if (emitter->head.count <= 0) {
                return;
            }
            {
                StreamParticle *particles;

                particles = emitter->particles;
                columnBottom = particles[emitter->head.start].pos.y;
            }
            if (columnBottom <= top) {
                func_1505D024(actor, 0x60019, 0, -1);
            }
        }
    }
}


/* Finds the stream emitter whose data has the given id, or NULL. */
LevelStream *func_151CE634(s32 id) {
    u8 i;
    s32 j;
    LevelStream *emitter;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            emitter = (LevelStream *)D_800DCE50[j][D_800A5760[i]];
            while (emitter != NULL) {
                if (emitter->head.tmpl.kind == STREAM_KIND) {
                    StreamData *data = emitter->data;

                    if (data->id == id) {
                        return emitter;
                    }
                }
                emitter = (LevelStream *)emitter->head.header.next;
            }
        }
    }
    return NULL;
}
