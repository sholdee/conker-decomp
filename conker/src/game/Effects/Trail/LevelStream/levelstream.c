#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/beam.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/model.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "conker/texture.h"
#include "conker/trail.h"
#include "conker/view.h"
#include "libc/string.h"

/* Events posted through effect_sendEventToTimedEffects with a stream id as the payload. */
#define STREAM_EVENT_START 0x17 /* create the stream's emitter */
#define STREAM_EVENT_STOP  0x18 /* stop emitting; the particles still fall */
#define STREAM_EVENT_ENDED 0x23 /* the emitter was destroyed */

#define STREAM_KIND 0xB /* head.tmpl.kind of a LevelStream */

/* LevelStream flags (head.tmpl.flags). */
#define EMITTER_FLAG_EMITTING 2
#define EMITTER_FLAG_STOPPED  8


/* Creates the spawner task for a stream source. */
StreamTask *trail_createStreamTask(TriggerZone *source, u8 owner, s32 list) {
    StreamTask *task;
    StreamSpawner spawner;

    spawner.source = source;
    spawner.id = source->data1;
    spawner.emitter = NULL;
    spawner.textureIndex = source->data2;

    task = effect_newTimedEffect(0x12C, -1, -1, -1, 0, 0x1E, 0x10, owner, list);
    if (task != NULL) {
        memcpy(&task->spawner, &spawner, sizeof(spawner));
    }
    return task;
}

/* Starts stream `id` (0-3). */
void trail_startStream(s32 id) {
    s32 payload;

    if (id >= 0 && id < 4) {
        payload = id;
        effect_sendEventToTimedEffects((s32)&payload, STREAM_EVENT_START);
    }
}

/* Stops stream `id` (0-3). */
void trail_stopStream(s32 id) {
    s32 payload;

    if (id >= 0 && id < 4) {
        payload = id;
        effect_sendEventToTimedEffects((s32)&payload, STREAM_EVENT_STOP);
    }
}

/* Event handler of a stream spawner task. */
void trail_onStreamTaskEvent(StreamTask *task, s32 *id, u8 event) {
    StreamSpawner *spawner;
    s32 unused[3]; // matching: the shipped -g3 frame reserves 12 unreferenced bytes here

    spawner = &task->spawner;
    if (event == STREAM_EVENT_START) {
        if (spawner->id == *id) {
            if (spawner->emitter == NULL) {
                spawner->emitter = trail_createStreamEmitter(spawner->source, spawner->textureIndex, spawner->id, task->base.header.owner, task->base.header.kind);
                spawner->source->inactive = 0;
            label: ; // matching: the empty block ends the if, so as1 fills the exit branch's delay slot with the ra reload
            }
        }
    } else if (event == STREAM_EVENT_STOP) {
        if (spawner->id == *id) {
            if (spawner->emitter != NULL) {
                trail_stopStreamEmitter(spawner->emitter);
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
LevelStream *trail_createStreamEmitter(source, textureIndex, id, owner, list)
    TriggerZone *source;
    u8 textureIndex;
    s32 id;
    u8 owner;
    s32 list;
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
    data.spawnTc = -16384.0f;
    data.tc = -16384.0f;
    data.unk20 = 0.0f;
    data.textureIndex = textureIndex;
    template.material = 0;
    template.shapeKind = 7;
    emitter = effect_newEffectObj(&template, sizeof(StreamData), sizeof(StreamParticle), 9, 9, 9, 0, 0, 0, owner, list);
    if (emitter != NULL) {
        memcpy(emitter->data, &data, sizeof(data));
    }
    return emitter;
}

/* Moves a stream's particles under gravity; those that reach the floor are retired.
 * Returns 0 once a stopped stream has run dry. */
s32 trail_moveStreamParticles(LevelStream *emitter) {
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
        dt = gFrameDelta;
        travel = velocity * dt;
        dtSquared = dt * dt;
        particle->pos.y += travel + halfGravity * dtSquared;
        particle->velocity = (f32)velocity + gravity * gFrameDelta; // matching: the no-op cast keeps velocity as the first add operand
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
s32 trail_spawnStreamParticles(LevelStream *emitter) {
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
    data->swayPhaseX += data->swayRateX * gFrameDelta;
    data->swayPhaseZ += data->swayRateZ * gFrameDelta;
    data->swayPhaseX = math_wrapRadians(data->swayPhaseX);
    data->swayPhaseZ = math_wrapRadians(data->swayPhaseZ);
    data->widthPhase += data->widthRate * gFrameDelta;
    data->widthPhase = math_wrapRadians(data->widthPhase);
    data->spawnCount += 0.2730129957f * gFrameDelta;
    data->tc += (10220.0f + math_randomFloat() * 9434.0f) * 0.01f * gFrameDelta;
    while (data->tc >= 16384.0f) {
        data->tc -= 16384.0f;
    }
    if (data->spawnCount > 1.0f) {
        step = 1.0f / data->spawnCount;
        unk20 = data->unk20 + gFrameDelta;
        unk20Step = -(unk20 * step);
        pos = data->spawnPos;
        dx = (emitter->head.tmpl.pos.x + sinf(data->swayPhaseX) * data->swayAmpX) - data->spawnPos.x;
        dz = (emitter->head.tmpl.pos.z + sinf(data->swayPhaseZ) * data->swayAmpZ) - data->spawnPos.z;
        dWidth = (data->baseWidth + sinf(data->widthPhase) * data->widthAmp) - data->width;
        dTc = data->tc - data->spawnTc;
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
            particle->pos.y += velocity * gFrameDelta + halfGravity * (gFrameDelta * gFrameDelta);
            particle->velocity = (f32)velocity + gravity * gFrameDelta; // matching: see trail_moveStreamParticles
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
        data->spawnTc = tc;
        data->unk20 = unk20;
    }
    return 1;
}

/* Fades a stream's ends: alpha ramps up over the lowest 19.7% of its height and down over the top 10%. */
s32 trail_fadeStreamEnds(LevelStream *emitter) {
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

/* The stream textures (gEffectTextures indices) by textureIndex. */
// matching: defined after trail_drawStream; with the values in view IDO keeps fewer values in registers there
static const s32 gStreamTextures[3];

/* Draws a stream as a camera-facing ribbon: one quad per pair of neighbouring particles, newest to oldest. */
Gfx *trail_drawStream(LevelStream *emitter, Gfx *gfx, s16 view) {
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
        particle_getViewBuffer(emitter->buffers, view, (emitter->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            data = emitter->data;
            particles = emitter->particles;
            update = 1;
            gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[gStreamTextures[data->textureIndex]], 0, 0, 0, 0, gStreamTextures[data->textureIndex], 0,
                                0, &update, 3);
            gfx = particle_setCombineMode(gfx, 0x4E, &update);
            gfx = math_gfxUpdateGeometryMode(gfx, 0x200005, 0x1F0600);
            gfx = math_gfxSetOtherMode(gfx,
                                gSpriteTlutMode | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                                gEffectRenderModes[3].cycle1 | gEffectRenderModes[3].cycle2, &update);
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
            offsetZ = gCameraAngleSines[view] * particles[cur].width;
            offsetX = gCameraAngleCosines[view] * particles[cur].width;
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
                offsetZ = gCameraAngleSines[view] * particles[next].width;
                offsetX = gCameraAngleCosines[view] * particles[next].width;
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

static const s32 gStreamTextures[3] = { 25, 26, 192 };

/* Stops a stream emitter: no new particles, the ones in flight keep falling. */
void trail_stopStreamEmitter(LevelStream *emitter) {
    emitter->head.updateKind2 = 0;
    emitter->head.tmpl.flags &= ~EMITTER_FLAG_EMITTING;
    emitter->head.tmpl.flags |= EMITTER_FLAG_STOPPED;
}

/* Destroy callbacks: tell the spawner its stream ended, then free the emitter. */
void trail_destroyStreamEmitter(LevelStream *emitter) {
    s32 payload;
    StreamData *data;

    data = emitter->data;
    payload = data->id;
    effect_sendEventToTimedEffects((s32)&payload, STREAM_EVENT_ENDED);
    effect_defaultDestroyEffectObj(emitter);
}

void trail_freeStreamEmitter(LevelStream *emitter) {
    s32 payload;
    StreamData *data;

    data = emitter->data;
    payload = data->id;
    effect_sendEventToTimedEffects((s32)&payload, STREAM_EVENT_ENDED);
    effect_defaultFreeEffectObj(emitter);
}


/* Sends `actor` event 0x60019 when its collision cylinder reaches the falling column of `source`'s stream. */
void trail_testStreamContact(Actor *actor, TriggerZone *source) {
    Vec3f pos;
    f32 radius;
    f32 height;
    LevelStream *emitter;

    emitter = trail_findStream(source->data1);
    if (emitter != NULL) {
        sprite_getActorBounds(actor, &pos, &radius, &height);
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
                actor_hitScripted(actor, 0x60019, 0, -1);
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
                actor_hitScripted(actor, 0x60019, 0, -1);
            }
        }
    }
}


/* Finds the stream emitter whose data has the given id, or NULL. */
LevelStream *trail_findStream(s32 id) {
    u8 i;
    s32 j;
    LevelStream *emitter;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            emitter = (LevelStream *)gObjectLists[j][gEffectObjTypes[i]];
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

/* Emitter update: accumulates a randomized spawn rate and, for each whole unit, spawns one
 * 0x6A effect packet at a random point of the shape (only when its zone's view mask has one of its
 * low gScreenLayout + 1 bits set, i.e. a view can see it; otherwise the accumulated units are discarded). */
void trail_updateRandomPointEmitter(Game1FBB80Object *object) {
    Game1FBB80Effect *effect;
    s32 index;
    s16 mask;

    effect = &object->effect;
    effect->accumulator += (5624.0f + math_randomFloat() * 3973.0f) * 1.0e-10f * gFrameDelta * effect->rate;
    if (effect->accumulator > 1.0f) {
        for (index = 0, mask = 0; index <= gScreenLayout; index++) {
            mask |= 1 << index;
        }
        if (!(effect->zone->views & mask)) {
            do {
                effect->accumulator -= 1.0f;
            } while (effect->accumulator > 1.0f);
            return;
        }
        {
            ParticleDef packet;
            f32 upperHeight;

            packet.texture = 0x6A;
            packet.type = 0;
            packet.renderModes = 0x2203;
            packet.lifetime = 100;
            packet.frame = 0;
            packet.frameSpeed = 0;
            packet.rgba[0] = 0xFF;
            packet.rgba[1] = 0xFF;
            packet.rgba[2] = 0xFF;
            packet.rgba[3] = 0xFF;
            packet.rot.y = 0.0f;
            packet.rot.z = 0.0f;
            packet.scale.y = 0.0f;
            packet.scale.z = 1.0f;
            packet.flags = 0x01CC0061;
            packet.colorLevel = 0xFF;
            packet.primMode = 0;
            packet.envMode = 7;
            packet.zone = effect->zone;
            do {
                // matching: `+ -1308.0f` (add.s); `- 1308.0f` emits sub.s
                packet.rot.x = (math_randomFloat() * 409.0f + -1308.0f) * 0.001f;
                packet.scale.x = (math_randomFloat() * 121.0f + 23.0f) * 0.001f;
                packet.width = math_randomFloat() * 25.0f + 10.0f;
                packet.height = math_randomFloat() * 60.0f + 60.0f;
                math_randomZoneFootprintPoint(effect->shape, &packet.pos.x, &packet.pos.z, &upperHeight, &packet.pos.y);
                if (collision_probeCeilingSet0Cached(&packet.pos, upperHeight, &effect->floor) != 0) {
                    packet.pos.y = effect->floor.height;
                    packet.alpha = math_random() % 101U + 155;
                    particle_newQuadParticle(&packet, &gWhiteVtxColors, 0x1C, 0, 0, 0x22, 0, 0, 0, 0, object->base.header.owner, object->base.header.kind);
                }
                effect->accumulator -= 1.0f;
            } while (effect->accumulator > 1.0f);
        }
    }
}

/* Integrates position/velocity under constant acceleration over one frame and ramps fade up to 1. */
s32 trail_integrateScalarMotion(TrailScalarMotion *motion) {
    {
        f32 prevVelocity = motion->velocity;

        motion->velocity += motion->acceleration * gFrameDelta;
        motion->position += (prevVelocity + 0.5f * motion->acceleration * gFrameDelta) * gFrameDelta;
    }
    motion->fade += motion->fadeRate * gFrameDelta;
    if (motion->fade > 1.0f) {
        motion->fade = 1.0f;
    }
    return 1;
}

/* Spawns a trail effect that follows owner (or anchor, when given). */
WaveTrail *trail_spawnWaveTrail(Actor *owner, DescParticleHead *anchor, u8 smooth, u8 fxOwner, s32 fxList) {
    EffectTemplate spawn;
    WaveTrailData info;
    WaveTrail *trail;
    s32 flags;
    f32 random;

    spawn.itemCount = 25;
    spawn.life = 300;
    spawn.flags = 16;
    spawn.kind = 17;
    spawn.shapeKind = 3;

    info.owner = owner;
    if (owner != NULL) {
        info.ownerId = owner->unique_id;
    } else {
        info.ownerId = 0;
    }

    info.scaled.z = 1.0f;
    info.normal.z = 1.0f;
    info.anchor = anchor;
    info.scaled.x = 0.0f;
    info.normal.x = 0.0f;
    if (smooth) {
        flags = WAVE_TRAIL_SMOOTH;
    } else {
        flags = 0;
    }
    info.flags = flags;

    info.maxLength = math_randomFloat() * 400.0f + 400.0f;
    random = math_randomFloat();
    info.phase = (random + random) * 3.141592741f; /* random angle in [0, 2pi) */

    if (!trail_getWaveTrailAnchor(&spawn.pos, owner, anchor)) {
        spawn.pos.x = 0.0f;
        spawn.pos.y = 0.0f;
        spawn.pos.z = 0.0f;
    }

    trail = effect_newEffectObj(&spawn, 40, 40, 0, 15, 15, 0, 0, 0, fxOwner, fxList);
    if (trail != NULL) {
        memcpy(trail->data, &info, sizeof(info));
    }
    return trail;
}

#define TRAIL_HAS_POS 4 /* WaveTrail.head.tmpl.flags: pos holds a valid previous position */

/* Writes the point a trail is attached to: the anchor's position if there is one, else the owner's. */
s32 trail_getWaveTrailAnchor(Vec3f *out, Actor *owner, DescParticleHead *anchor) {
    if (anchor != NULL) {
        *out = anchor->desc.pos;
    } else {
        out->x = owner->xpos;
        out->y = owner->ypos;
        out->z = owner->zpos;
    }
    return TRUE;
}

/*
 * Follows the trail's attachment point and, once it has moved far enough, pushes it as the newest point
 * of the ring buffer `points`. Points carry an XZ normal (and that normal scaled by 51) perpendicular
 * to the trail; with WAVE_TRAIL_SMOOTH the previous point's normal is re-averaged from its neighbours.
 * Returns 0 when the owner is gone or no longer matches.
 */
s32 trail_followWaveTrail(WaveTrail *trail) {
    WaveTrailData *info;
    WaveTrailPoint *entries;
    Actor *owner;
    DescParticleHead *anchor;
    Vec3f prev;
    Vec3f delta;
    f32 dist;
    f32 len;
    f32 inv;
    f32 dx;
    f32 dz;
    f32 nx;
    f32 nz;

    info = trail->data;
    entries = trail->points;
    owner = info->owner;
    anchor = info->anchor;
    if (owner != NULL && (owner->type == 0 || owner->unique_id != info->ownerId)) {
        return 0;
    }
    if (gFrameTicks > 0) {
        prev = trail->head.tmpl.pos;
        if (trail_getWaveTrailAnchor(&trail->head.tmpl.pos, owner, anchor)) {
            if (!(trail->head.tmpl.flags & TRAIL_HAS_POS)) {
                prev = trail->head.tmpl.pos;
            } else if (!(info->flags & WAVE_TRAIL_HAS_NORMAL)) {
                dx = trail->head.tmpl.pos.x - prev.x;
                dz = trail->head.tmpl.pos.z - prev.z;
                len = sqrtf(dx * dx + dz * dz);
                if (len != 0.0f) {
                    inv = 1.0f / len;
                    info->flags |= WAVE_TRAIL_HAS_NORMAL;
                    nz = dx * inv;
                    nx = -(dz * inv);
                    info->normal.x = nx;
                    info->scaled.x = nx * 51.0f;
                    info->normal.z = nz;
                    info->scaled.z = nz * 51.0f;
                }
            }
            trail->head.tmpl.flags |= TRAIL_HAS_POS;
        } else {
            trail->head.tmpl.flags &= ~TRAIL_HAS_POS;
            return 1;
        }
        delta.x = trail->head.tmpl.pos.x - prev.x;
        delta.y = trail->head.tmpl.pos.y - prev.y;
        delta.z = trail->head.tmpl.pos.z - prev.z;
        dist = math_vec3Length(&delta);
        if (dist > 0.0001f) {
            WaveTrailPoint *newest;

            newest = &entries[trail->head.end];
            newest->pos = trail->head.tmpl.pos;
            newest->dist = dist;
            len = sqrtf(delta.x * delta.x + delta.z * delta.z);
            if (len == 0.0f) {
                newest->invLen = 1.0f;
            } else {
                newest->invLen = 1.0f / len;
            }
            newest->wave = 0.0f;
            newest->scaled = info->scaled;
            newest->normal = info->normal;
            trail->head.end++;
            if (trail->head.end == trail->head.tmpl.itemCount) {
                trail->head.end = 0;
            }
            trail->head.count++;
            if (trail->head.end == trail->head.start) {
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
            }
            if ((info->flags & WAVE_TRAIL_SMOOTH) && trail->head.count >= 3) {
                WaveTrailPoint *mid;
                WaveTrailPoint *oldest;
                f32 headInv;
                s16 midIdx;
                s16 oldIdx;
                f32 d1z;
                f32 d1x;
                f32 d2z;
                f32 d2x;
                f32 midInv;
                f32 scale; // matching: a named local for 51.0f steers FP register allocation
                TrailVec2 n;
                f32 lenSq;

                midIdx = trail->head.end - 1;
                if (midIdx < 0) {
                    midIdx = trail->head.tmpl.itemCount - 1;
                }
                midIdx--;
                if (midIdx < 0) {
                    midIdx = trail->head.tmpl.itemCount - 1;
                }
                oldIdx = midIdx - 1;
                if (oldIdx < 0) {
                    oldIdx = trail->head.tmpl.itemCount - 1;
                }
                mid = &entries[midIdx];
                oldest = &entries[oldIdx];
                headInv = newest->invLen;
                midInv = mid->invLen;
                d1x = (newest->pos.x - mid->pos.x) * headInv;
                d1z = (newest->pos.z - mid->pos.z) * headInv;
                d2x = (oldest->pos.x - mid->pos.x) * midInv;
                d2z = (oldest->pos.z - mid->pos.z) * midInv;
                n.x = -(d1z - d2z);
                n.z = d1x - d2x;
                lenSq = n.x * n.x + n.z * n.z;
                if (lenSq > 0.0001f) {
                    f32 normInv;

                    scale = 51.0f;
                    normInv = 1.0f / sqrtf(lenSq);
                    n.x *= normInv;
                    n.z *= normInv;
                    mid->normal = n;
                    mid->scaled.x = n.x * scale;
                    mid->scaled.z = n.z * scale;
                } else {
                    mid->scaled.x = 0.0f;
                    mid->scaled.z = 0.0f;
                    mid->normal.x = 0.0f;
                    mid->normal.z = 0.0f;
                }
            }
        }
    }
    return 1;
}

/*
 * Updates a ring buffer of trail points: trims the oldest points so the summed segment
 * distance stays within data->maxLength, then gives each point a sine wave offset that
 * grows in amplitude along the trail.
 */
s32 trail_shapeWaveTrail(WaveTrail *actor) {
    WaveTrailData *config;
    WaveTrailPoint *points;
    s32 count;

    count = actor->head.count;
    config = actor->data;
    points = actor->points;
    if (count >= 2) {
        f32 total;
        f32 limit;
        s32 index;
        s32 current;
        WaveTrailPoint *point;

        total = 0.0f;
        index = actor->head.end - 1;
        if (index < 0) index = actor->head.tmpl.itemCount - 1;
        if (index != actor->head.start) {
            limit = config->maxLength;
            do {
                current = index;
                index--;
                if (index < 0) index = actor->head.tmpl.itemCount - 1;
                point = &points[current];
                total += point->dist;
                if (limit < total) {
                    // matching: comparing with 0.0f instead of int 0 changes the emitted code
                    if (point->dist != 0) {
                        WaveTrailPoint *previous;
                        f32 ratio;
                        f32 px, py, pz;
                        f32 dx, dy, dz;

                        ratio = (total - limit) / point->dist;
                        previous = &points[index];
                        px = previous->pos.x;
                        py = previous->pos.y;
                        pz = previous->pos.z;
                        dx = px - point->pos.x;
                        dy = py - point->pos.y;
                        dz = pz - point->pos.z;
                        previous->pos.x = px - dx * ratio;
                        previous->pos.y = py - dy * ratio;
                        previous->pos.z = pz - dz * ratio;
                        point->dist *= 1.0f - ratio;
                    }
                    while (index != actor->head.start) {
                        actor->head.start++;
                        if (actor->head.start == actor->head.tmpl.itemCount) {
                            actor->head.start = 0;
                        }
                        actor->head.count--;
                    }
                    limit = config->maxLength;
                    total = limit;
                }
            } while (index != actor->head.start);
            count = actor->head.count;
        }
    }
    if (count >= 2) {
        f32 amplitude;
        f32 phase;
        s32 index;

        amplitude = 0.0f;
        index = actor->head.end;
        phase = config->phase;
        do {
            WaveTrailPoint *point;
            f32 distance;

            index--;
            if (index < 0) index = actor->head.tmpl.itemCount - 1;
            point = &points[index];
            point->wave = sinf(phase) * amplitude;
            distance = point->dist;
            amplitude += distance * 0.037499998f;
            phase += distance * 0.0122f;
        } while (index != actor->head.start);
    }
    config->phase += -0.70900005f * gFrameDelta;
    return 1;
}

/* Draws the ribbon from its newest point back to its oldest, one quad per segment. */
Gfx *trail_drawWaveRibbon(RibbonActor *ribbon, Gfx *gfx, s16 cameraIdx) {
    RibbonInfo *info;
    Vec3f nextPos;
    Vec3f curPos;
    s32 cur;
    s32 next;
    RibbonActorPoint *points;
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

    if (ribbon->head.count < 2) {
        return gfx;
    }
    info = ribbon->info;
    points = ribbon->points;
    if (info->owner != NULL && info->owner->ob_matrix_ptr == NULL) {
        return gfx;
    }
    particle_getViewBuffer(ribbon->vtxBuffer, cameraIdx, (ribbon->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
    update = 1;
    gfx = math_gfxUpdateGeometryMode(math_gfxSetEnvColor(gfx, 0xED, 0xD2, 0x85, 0xC8, &update), 0x200005, 0x1F0600);
    gfx = math_gfxSetOtherMode(particle_setCombineMode(gfx, 0x54, &update), gSpriteTlutMode | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, gEffectRenderModes[1].cycle1 | gEffectRenderModes[1].cycle2, &update);
    cur = ribbon->head.end - 1;
    if (cur < 0) {
        cur = ribbon->head.tmpl.itemCount - 1;
    }
    next = cur - 1;
    if (next < 0) {
        next = ribbon->head.tmpl.itemCount - 1;
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
    vtx->v.ob[0] = curPos.x + curOffset.x + curX;
    vtx->v.ob[1] = curPos.y;
    vtx->v.ob[2] = curPos.z + curOffset.z + curZ;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = curPos.x - curOffset.x + curX;
    vtx->v.ob[1] = curPos.y;
    vtx->v.ob[2] = curPos.z - curOffset.z + curZ;
    vtx->v.flag = 0;
    vtx++;
    do {
        vtx->v.ob[0] = nextPos.x + nextOffset.x + nextX;
        vtx->v.ob[1] = nextPos.y;
        vtx->v.ob[2] = nextPos.z + nextOffset.z + nextZ;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = nextPos.x - nextOffset.x + nextX;
        vtx->v.ob[1] = nextPos.y;
        vtx->v.ob[2] = nextPos.z - nextOffset.z + nextZ;
        vtx->v.flag = 0;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP2Triangles(gfx++, 0, 1, 2, 0, 1, 3, 2, 0);
        cur = next;
        next--;
        if (next < 0) {
            next = ribbon->head.tmpl.itemCount - 1;
        }
        if (cur != ribbon->head.start) {
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
    } while (cur != ribbon->head.start);
    return gfx;
}

/* Message handler of a following actor: lets effect_handleOwnerActorEvent update or end the link to its owner. */
void trail_onFollowerMessage(TrackerTrail *actor, s32 event, u8 eventType) {
    TrackerTrailData *data = actor->state;

    if (data->owner != NULL) {
        // the owner pointer and the byte after it are handled like an actor reference
        effect_handleOwnerActorEvent(event, eventType, (Actor **)&data->owner, (u8 *)&data->texture, actor);
    }
}

/* Data of the ground effect spawned by trail_spawnNearCamera. */
typedef struct {
    Vec3f pos;
    f32 unkC;
} GroundEffectData;

typedef struct {
    TimedEffect base;
    GroundEffectData data;
} GroundEffect;

/* With probability 0.222, spawns an effect on the ground at a random point up to 2000 units from
 * the camera of the object's player, within 40 degrees either side of the camera's yaw. */
void trail_spawnNearCamera(Actor *obj, f32 probeY, f32 bottom) {
    Camera *player;
    Vec3f spawnPos;
    Vec3f *camPos;
    f32 yaw;
    f32 angle;
    f32 distance;
    GroundEffectData data;
    GroundEffect *effect;

    player = obj->CameraPtr;
    if (player != NULL) {
        if (math_randomFloat() < 0.2220000178f) {
            camPos = math_getCameraPos(player->CNumber);
            yaw = math_getCameraYaw(player->CNumber);
            angle = ((math_randomFloat() * 80.0f) + (yaw - 40.0f)) * 0.01745329238f;
            distance = math_randomFloat() * 2000.0f;

            spawnPos.x = camPos->x - (sinf(angle) * distance);
            spawnPos.y = probeY;
            spawnPos.z = camPos->z - (cosf(angle) * distance);

            if (collision_probeFloorSurfABSet0Cached(&spawnPos, 0, bottom, &D_800D9860) != 0) {
                spawnPos.y = D_800D9860.height;
                data.pos = spawnPos;
                data.unkC = 0.0f;

                effect = effect_newTimedEffect((math_random() % 131U) + 51, -1, 0x20, -1, 1, 0,
                                                       sizeof(data), 0xFF, 1);
                if (effect != NULL) {
                    memcpy(&effect->data, &data, sizeof(data));
                }
            }
        }
    }
}

/* The model points the seven pieces of trail_breakApart fly off from. */
static const Vec3f gBreakPiecePoints[7] = {
    { 12.21999931f, 63.10099792f, -34.88000107f },  { 59.45899963f, 27.04800034f, 23.3730011f },
    { -30.83699989f, 62.33999634f, 27.96899986f },  { 36.03700256f, -40.0929985f, -43.31699753f },
    { -41.90200043f, -24.30399895f, -39.91400146f }, { -51.70800018f, -18.10499954f, 40.625f },
    { 14.18299961f, -58.02799988f, 35.02000046f },
};

/* Breaks the object apart: seven model pieces fly off its model's points with random velocity and
 * spin, plus three more effects at 200 units above it. */
void trail_breakApart(Actor *obj, u8 owner, s32 list) {
    Vec3f vecB;
    Vec3f pos;
    Vec3f vecA;
    Vec3f vecC;
    u8 i;

    pos.x = obj->xpos;
    pos.y = obj->ypos + 200.0f;
    pos.z = obj->zpos;
    if (obj->ob_matrix_ptr) {
        // matching: the matrix goes through s32; passing the pointer directly changes the temp registers
        model_decomposeBoneMatrix((Mtx *)(s32)obj->ob_matrix_ptr, &vecA.x, &vecA.y, &vecA.z, &vecB.x, &vecB.y, &vecB.z,
                      &vecC.x, &vecC.y, &vecC.z);
        {
            ParticleSpawn78 piece;
            const s32 models[7] = { 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75 };

            piece.radius = 1.0f;
            piece.elasticity = 0.6490000486f;
            piece.sizeXZ = piece.sizeY = (obj->scale_xz + obj->scale_y) * 0.5f;
            piece.rot = vecB;
            piece.scale.x = 1.0f;
            piece.scale.y = 0.0f;
            piece.scale.z = 1.0f;
            piece.rotVel.y = 0.0f;
            piece.variant = 0;
            piece.zone = 0;
            piece.alpha = 0xFF;
            piece.update2Fn = 0;
            piece.floorType3Fn = 0;
            piece.wallHitFn = 0;
            piece.destroyFn = 0;
            piece.squashFn = 0;
            piece.lightMode = 1;
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
                math_transformOffset(&gBreakPiecePoints[i], &piece.pos, obj->ob_matrix_ptr);
                yaw = math_random();
                pitch = math_random();
                math_dirFromBinAngles(yaw & 0xFF, (pitch % 25U) - 64, (math_randomFloat() * 11.44999981f) + 16.93000031f, &piece.vel);
                piece.rotVel.x = (math_randomFloat() * 19.75200081f) + -9.876000404f;
                piece.rotVel.z = (math_randomFloat() * 19.75200081f) + -9.876000404f;
                piece.gravity = (math_randomFloat() * 0.5180000067f) + -1.539000034f;
                piece.lifetime = (math_random() % 101U) + 100;
                particle_newModelParticle(&piece, 3, 0xFF, 0, owner, list);
            }
        }
        {
            PuffSpray84 cfg;

            cfg.pos = pos;
            cfg.sizeBase = 8.0f;
            cfg.sizeRange = 13.0f;
            cfg.growthBase = 0.8000000119f;
            cfg.growthRange = 0.400000006f;
            cfg.speedBase = 23.0f;
            cfg.speedRange = 7.0f;
            cfg.countBase = 13;
            cfg.countRange = 6;
            cfg.angles.yawBase = 0;
            cfg.angles.yawRange = 0xFF;
            cfg.angles.pitchBase = -64;
            cfg.angles.pitchRange = 50;
            cfg.itemCountBase = 3;
            cfg.itemCountRange = 2;
            cfg.lifeBase = 40;
            cfg.lifeRange = 25;
            cfg.green = 9;
            cfg.headerFlags = 1;
            cfg.red = 0x48;
            cfg.handlers[0] = 1;
            cfg.handlers[1] = 0;
            cfg.handlers[2] = 0;
            cfg.handlers[3] = 0;
            cfg.handlers[4] = 0;
            cfg.handlers[5] = 0;
            cfg.blue = 0;
            cfg.alphaBase = 150;
            cfg.alphaRange = 105;
            cfg.maxLights = 3;
            cfg.lightMask = 0xFF;
            cfg.render.otherModeH = 0;
            cfg.render.setGeometryMode = 0x220005;
            cfg.render.clearGeometryMode = 0x1D0600;
            cfg.render.renderMode = 3;
            cfg.render.combiner = 0x3B;
            cfg.render.colorDither = 0x80;
            cfg.render.alphaDither = 0x20;
            cfg.render.primMode = 0;
            cfg.render.envMode = 7;
            cfg.fadeStart = 10;
            cfg.fadeRate = 25;
            particle_spawnPuffSpray(&cfg, owner, list);
        }
        {
            DecalSpray cfg2;

            cfg2.countBase = 15;
            cfg2.countRange = 6;
            cfg2.pos = pos;
            cfg2.angles.yawBase = 0;
            cfg2.angles.yawRange = 0xFF;
            cfg2.angles.pitchBase = -29;
            cfg2.angles.pitchRange = 40;
            cfg2.maxDistance = 375.0f;
            cfg2.sizeBase = 20.0f;
            cfg2.sizeRange = 40.0f;
            cfg2.alphaBase = 120;
            cfg2.alphaRange = 120;
            cfg2.kind = 2;
            particle_spawnDecalSpray(&cfg2, owner, list);
        }
        {
            CollisionHit floor;
            s8 flag;
            s32 pad; // matching: unused stack slot in the original frame

            collision_initHitFromObject(&floor, obj);
            flag = 2;
            beam_spawnAtGroundPoints(&pos, pos.y - 2000.0f, 200.0f, &floor, (math_random() & 3) + 7, 0, &flag, owner, list);
        }
    }
}

/* Tells every effect that follows the object that it is gone (message 0x18). */
void trail_notifyFollowersGone(Actor *obj) {
    ActorRef msg;

    msg.actor = obj;
    msg.uniqueId = obj->unique_id;
    effect_sendEventToTimedEffects((s32)&msg, 0x18);
}

/* Spawns a strip effect of the given variant on the object. */
void trail_spawnStripEffect(Actor *obj, u8 variant, u8 owner, s32 list) {
    StripEffect *effect;
    StripEffectData data;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.scale = (obj->scale_xz + obj->scale_y) * 0.5f;
    data.variant = variant;
    data.alpha = (math_random() % 56U) + 200;
    data.ownerUnk84 = obj->animseq.uh;

    effect = effect_newTimedEffect(300, -1, 0x61, 4, 0, 0x31, sizeof(data), owner, list);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Ends the strip effect once its owner's unk84 has changed. */
void trail_updateStripEffect(StripEffect *effect) {
    Actor *owner = effect->data.owner;

    if (effect->data.ownerUnk84 != owner->animseq.uh) {
        effect->base.header.state = -1;
    }
}

/* Per strip variant: the start point (in joint 2's space) and the joint of the end point gStripEndPoint. */
// matching: defined after trail_drawStripEffect (see gStreamTextures)
static const Vec3f gStripStartPoints[2];
static const Vec3f gStripEndPoint;
static const u8 gStripEndJoints[2];

/* Draws the strip effect: six camera-facing quads joining two of the owner's model points, pinched
 * in the middle when the points are far apart. */
Gfx *trail_drawStripEffect(Gfx *gfx, StripEffect *effect, s16 cameraIdx) {
    StripEffectData *data;
    Vtx *vtx;
    Actor *owner;
    u8 update;
    Vec3f *camPos;
    f32 midX;
    f32 midY;
    f32 midZ;
    /* Scalars, not an array: as uopt live ranges they interfere with the 1/6 constant, which stops
     * its register piece before the loop header (an array left a dead reload and an extra literal). */
    f32 sideZ;
    f32 sideY;
    f32 sideX;
    f32 taper;
    Vec3f from;
    Vec3f to;
    Vec3f diff;
    f32 stepX;
    f32 stepY;
    f32 stepZ;
    Vec3f pos;
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
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return gfx;
    }
    if (owner->ob_matrix_ptr == NULL) {
        return gfx;
    }
    particle_getViewBuffer(effect->base.buffers, cameraIdx, 0x1C0, &vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    update = 1;
    math_transformOffset(&gStripStartPoints[data->variant], &from, &owner->ob_matrix_ptr[2]);
    math_transformOffset(&gStripEndPoint, &to, &owner->ob_matrix_ptr[gStripEndJoints[data->variant]]);
    camPos = math_getCameraPos(cameraIdx);
    gfx = math_gfxUpdateGeometryMode(gfx, 0x200005, 0x60600);
    gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[127], 0, 0, 0, 0, 0x7F, 0, 0, &update, 3);
    gfx = particle_setCombineMode(gfx, 0x3B, &update);
    gfx = math_gfxSetEnvColor(gfx, 0xFF, 0xFF, 0xFF, data->alpha, &update);
    gfx = math_gfxSetOtherMode(gfx, gSpriteTlutMode | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, gEffectRenderModes[3].cycle1 | gEffectRenderModes[3].cycle2, &update);
    diff.x = to.x - from.x;
    diff.y = to.y - from.y;
    diff.z = to.z - from.z;
    dist = math_vec3Length(&diff);
    midX = diff.x * 0.5f + from.x - camPos->x;
    midY = diff.y * 0.5f + from.y - camPos->y;
    midZ = diff.z * 0.5f + from.z - camPos->z;
    offsetX = diff.y * midZ - midY * diff.z;
    offsetY = diff.z * midX - midZ * diff.x;
    offsetZ = diff.x * midY - midX * diff.y;
    if (offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ == 0.0f) {
        sideZ = sideY = sideX = 0.0f;
    } else {
        amp = (35.0f * data->scale) / sqrtf(offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ);
        sideX = offsetX * amp;
        sideY = offsetY * amp;
        sideZ = offsetZ * amp;
    }
    stepX = diff.x * 0.1666666716f;
    stepY = diff.y * 0.1666666716f;
    stepZ = diff.z * 0.1666666716f;
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
        offsetX = sideX * taper;
        vtx->v.ob[0] = offsetX + pos.x;
        vtx->v.ob[1] = sideY * taper + pos.y;
        vtx->v.ob[2] = sideZ * taper + pos.z;
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = tex;
        vtx++;
        vtx->v.ob[0] = pos.x - offsetX;
        vtx->v.ob[1] = pos.y - sideY * taper;
        vtx->v.ob[2] = pos.z - sideZ * taper;
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0x7C0;
        vtx->v.tc[1] = tex;
        vtx++;
        taper = 1.0f - sinf(ang + 0.5235987902f) * amp;
        offsetX = sideX * taper;
        vtx->v.ob[0] = pos.x + stepX - offsetX;
        offsetY = sideY * taper;
        vtx->v.ob[1] = pos.y + stepY - offsetY;
        offsetZ = sideZ * taper;
        vtx->v.ob[2] = pos.z + stepZ - offsetZ;
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0x7C0;
        vtx->v.tc[1] = tex + 341.3333435f;
        vtx++;
        // matching: dead store; its two variable reads shift where uopt splits the basic block
        unused = offsetX + offsetY;
        vtx->v.ob[0] = pos.x + stepX + offsetX;
        vtx->v.ob[1] = pos.y + stepY + offsetY;
        vtx->v.ob[2] = pos.z + stepZ + offsetZ;
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = tex + 341.3333435f;
        vtx++;
        i++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
        pos.x += stepX;
        pos.y += stepY;
        pos.z += stepZ;
        progress += 0.1666666716f;
        ang += 0.5235987902f;
        tex += 341.3333435f;
    } while (i != 6);
    return gfx;
}

static const Vec3f gStripStartPoints[2] = { { -109.0f, -87.0f, 31.0f }, { 109.0f, -87.0f, 31.0f } };
static const Vec3f gStripEndPoint = { -1.0f, -27.0f, 1.0f };
static const u8 gStripEndJoints[2] = { 23, 34 };

/* Message handler of the effects that follow an owner: frees the effect when its owner is gone
 * (messages 0 and 0x18) and follows the owner through a swap (0x2D). */
void trail_onOwnedEffectMessage(OwnedEffect *effect, void *msg, u8 type) {
    ActorRef *ref = &effect->ref;

    if (type == 0 || type == 0x18) {
        ActorRef *gone = msg;
        Actor *obj = gone->actor;

        if (obj == ref->actor || gone->uniqueId == ref->uniqueId) {
            effect_destroyObject(&effect->base.header);
        }
    } else if (type == 0x2D) {
        ActorSwapMsg *swap = msg;

        if (ref->actor == swap->actorA) {
            ref->actor = swap->actorB;
            ref->uniqueId = swap->idB;
        } else if (ref->actor == swap->actorB) {
            ref->actor = swap->actorA;
            ref->uniqueId = swap->idA;
        }
    }
}

/* The two model points (in joint 2's space) that trail_emitLineBursts's bursts run between. */
static const Vec3f gLineBurstStart = { 0.0f, -26.0f, -20.0f };
static const Vec3f gLineBurstEnd = { 0.0f, 29.0f, 131.0f };

/* Emits two particle bursts along the line between two of the object's model points, and sometimes
 * 5-10 more particles that each carry a trail_spawnWaveTrail effect. */
void trail_emitLineBursts(Actor *obj, u8 owner, s32 list) {
    Vec3f pos;
    Vec3f dir;
    CollisionHit floor;
    ConeBasis shape;

    if (obj->ob_matrix_ptr != NULL) {
        {
            const Vec3f *srcs[2];
            Vec3f *dsts[2];

            srcs[0] = &gLineBurstStart;
            srcs[1] = &gLineBurstEnd;
            dsts[0] = &pos;
            dsts[1] = &dir;
            math_transformOffsets(srcs, dsts, &obj->ob_matrix_ptr[2], 2);
            dir.x -= pos.x;
            dir.y -= pos.y;
            dir.z -= pos.z;
        }
        collision_initHitFromObject(&floor, obj);
        shape.flags = 0;
        shape.direction = dir;
        /* Start from the zero vector gZeroVec3f. */
        shape.axis0 = shape.axis1 = gZeroVec3f;
        {
            DropSpray emitA;

            emitA.pos = pos;
            emitA.countBase = 6;
            emitA.countRange = 8;
            emitA.speedBase = 0.03999999911f;
            emitA.speedRange = 0.02999999933f;
            emitA.lifeBase = 30;
            emitA.lifeRange = 30;
            emitA.gravityBase = -1.467000127f;
            emitA.gravityRange = 0.756000042f;
            emitA.alphaBase = 155;
            emitA.alphaRange = 100;
            emitA.sizeBase = 403.0f;
            emitA.sizeRange = 455.0f;
            emitA.unk30 = 1;
            emitA.kind = 2;
            emitA.unk34 = 0.0f;
            emitA.wobbles = 1;
            emitA.bounceChance = 0.0f;
            shape.spread = 750.0f;
            particle_spawnDropSpray(&shape, &emitA, &floor, owner, list);
        }
        {
            SplashSpray emitB;

            emitB.pos = pos;
            emitB.scaleBase = 10.0f;
            emitB.scaleRange = 25.0f;
            emitB.sizeBase = 1.253000021f;
            emitB.sizeRange = 0.8230000138f;
            emitB.speedBase = 0.03500000015f;
            emitB.speedRange = 0.03199999779f;
            emitB.countBase = 15;
            emitB.countRange = 8;
            emitB.itemCountBase = 3;
            emitB.itemCountRange = 2;
            emitB.lifeBase = 30;
            emitB.lifeRange = 25;
            emitB.alphaBase = 155;
            emitB.alphaRange = 100;
            emitB.burstChance = 0.0f;
            emitB.fadeStart = 16;
            emitB.fadeRate = 15;
            emitB.material = 0;
            emitB.kind = 2;
            shape.spread = 750.0f;
            particle_spawnSplashSpray(&shape, &emitB, &floor, owner, list);
        }
        if (particle_completeConeBasis(&shape) != 0) {
            s32 count;
            ParticleDesc spawn;
            CarrierParticle *particle;
            s32 pad; // matching: unused stack slot in the original frame

            shape.spread = 1000.0f;
            count = (math_random() % 6U) + 5;
            spawn.texture = 0x3E;
            spawn.renderMode = 0x2203;
            spawn.setGeometryMode = 0x200005;
            spawn.clearGeometryMode = 0;
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
            spawn.offset = gZeroVec3f;
            spawn.scaleStart = 1;
            spawn.flags = 0xC007;
            spawn.scaleRate = 1.0f;
            spawn.timer = (math_random() % 11U) + 40;
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
                particle_getRandomConeDir(&shape, (math_randomFloat() * 0.03999999911f) + 0.04999999702f, &spawn.vel);
                spawn.gravity = (math_randomFloat() * 1.340000033f) + -2.328999996f;
                particle = particle_newSpriteParticleWithFloor(&spawn, 1, 0, 4, owner, list);
                if (particle != NULL) {
                    particle->effect = trail_spawnWaveTrail(NULL, &particle->head, 0, owner, list);
                }
                count--;
            }
        }
    }
}

/* Frees the effect that travels with the particle. */
void trail_freeCarriedEffect(CarrierParticle *particle) {
    if (particle->effect != NULL) {
        // matching: a volatile read makes IDO reload the pointer for the call, as the original does
        effect_destroyObject(*(EffectHeader *volatile *)&particle->effect);
    }
}

/* Particle handler: frees the attached effect, then runs particle_destroySpriteDefault. */
void trail_destroyCarrierParticle(CarrierParticle *particle) {
    trail_freeCarriedEffect(particle);
    particle_destroySpriteDefault(&particle->head);
}

/* Particle handler: frees the attached effect, then runs particle_freeSpriteDefault. */
void trail_freeCarrierParticle(CarrierParticle *particle) {
    trail_freeCarriedEffect(particle);
    particle_freeSpriteDefault(&particle->head);
}

/* Point functions: each writes a point of the object to out and returns whether it has one. */
static s32 (*gTrackerPointProcs[])(Actor *obj, Vec3f *out) = { trail_getPointAboveFloor, trail_getModelPoint0, trail_getModelPoint1 };

/* Spawns a tracker effect that follows the object's point gTrackerPointProcs[pointFn]. */
void trail_spawnTrackerEffect(Actor *obj, u8 pointFn, u8 owner, s32 list) {
    TrackerEffect *effect;
    TrackerData data;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.trail = NULL;

    if (gTrackerPointProcs[pointFn](obj, &data.pos) == 0) {
        data.pos = gZeroVec3f;
    }

    data.prevPos = data.pos;
    data.delta = gZeroVec3f;
    data.dist = 0.0f;
    data.pointFn = pointFn;

    effect = effect_newTimedEffect(300, -1, 0x5F, -1, 0, 0x48, sizeof(data), owner, list);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Point function: 8 units above the object's unk180 height, at its x/z. */
s32 trail_getPointAboveFloor(Actor *obj, Vec3f *out) {
    out->x = obj->xpos;
    out->y = obj->floorY + 8.0f;
    out->z = obj->zpos;
    return TRUE;
}

/* Point function: the object's model point 0. */
s32 trail_getModelPoint0(Actor *obj, Vec3f *out) {
    return trail_getModelPoint(obj, out, 0);
}

/* Point function: the object's model point 1. */
s32 trail_getModelPoint1(Actor *obj, Vec3f *out) {
    return trail_getModelPoint(obj, out, 1);
}

static const Vec3f gTrackerModelPoints[2] = { { 100.0f, 8.0f, -20.0f }, { -100.0f, 8.0f, -20.0f } };

/* Writes model point gTrackerModelPoints[which] of the object, in world space, to out. */
s32 trail_getModelPoint(Actor *obj, Vec3f *out, u8 which) {
    s32 joints = (s32)obj->ob_matrix_ptr;

    if (joints == 0) {
        return FALSE;
    }
    // matching: "| 0" keeps joints from being a plain copy, so IDO gives it v0 and which a3
    math_transformOffset(&gTrackerModelPoints[which], out, (Mtx *)(joints | 0));
    return TRUE;
}

/* Tracker update: follows the owner's point, and drops the trail when the point is lost, jumps more
 * than 300 units, stops, or the owner is airborne, in water or in state 5, 6, 9, 13 or 14. */
void trail_updateTrackerEffect(TrackerEffect *effect) {
    TrackerData *data;
    Actor *owner;
    u8 finished = FALSE;
    s32 state;

    data = &effect->data;
    owner = data->owner;

    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }

    data->prevPos = data->pos;
    if (gTrackerPointProcs[data->pointFn](owner, &data->pos) == 0) {
        data->pos = data->prevPos;
        finished = TRUE;
    }

    data->delta.x = data->pos.x - data->prevPos.x;
    data->delta.y = data->pos.y - data->prevPos.y;
    data->delta.z = data->pos.z - data->prevPos.z;
    data->dist = math_vec3Length(&data->delta);

    state = collision_getSurfaceType(owner->floorMaterial);
    if (data->dist > 300.0f || data->dist <= 0.0f || owner->shad != 0.0f || owner->inwater != 0 ||
        state == 5 || state == 6 || state == 9 || state == 13 || state == 14) {
        finished = TRUE;
    }

    if (finished) {
        trail_dropTrackerTrail(effect);
    } else if (data->trail == NULL) {
        trail_spawnTrackerTrail(effect);
    }
}

/* Message handler of the tracker effect: lets effect_handleOwnerActorEvent update or end the link to its owner. */
void trail_onTrackerMessage(TrackerEffect *effect, s32 event, u8 eventType) {
    effect_handleOwnerActorEvent(event, eventType, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Tracker teardown: lets go of the trail. */
void trail_releaseTracker(TrackerEffect *effect) {
    trail_dropTrackerTrail(effect);
}

void trail_destroyTracker(TrackerEffect *effect) {
    trail_releaseTracker(effect);
    effect_defaultDestroyTimedEffect(&effect->base);
}

void trail_freeTracker(TrackerEffect *effect) {
    trail_releaseTracker(effect);
    effect_defaultFreeTimedEffect(&effect->base);
}

/* Lets go of the tracker's trail, which then fades out on its own. */
void trail_dropTrackerTrail(TrackerEffect *effect) {
    TrackerData *data = &effect->data;
    TrackerTrailData *trailData;

    if (data->trail != NULL) {
        trailData = data->trail->state;
        data->trail->head.updateKind2 = 0;
        data->trail->head.tmpl.flags &= ~0x2;
        data->trail->head.tmpl.flags |= 0x8;
        data->trail->head.tmpl.flags |= 0x1;
        data->trail->head.tmpl.life = 40;
        trailData->owner = NULL;
        data->trail = NULL;
    }
}

/* Spawns the trail that follows the tracker effect. */
void trail_spawnTrackerTrail(TrackerEffect *effect) {
    TrackerData *data;
    struct {
        EffectTemplate spawn;
        TrackerTrailData data;
    } req;
    Actor *owner;
    TrackerTrail *trail;

    data = &effect->data;
    owner = data->owner;
    req.data.owner = effect;
    req.data.alpha = 0xFF;
    req.data.texture = 0.0f;
    if (owner->character_type == 0x25) {
        req.data.maxLength = 800.0f;
        req.data.widthBase = 36.0f;
        req.data.widthScale = 50.0f;
        req.data.alphaScale = 200.0f;
    } else {
        if (gScreenLayout > 0) {
            req.data.maxLength = 260.0f;
        } else {
            req.data.maxLength = 500.0f;
        }
        req.data.widthBase = 40.0f;
        req.data.widthScale = 40.0f;
        req.data.alphaScale = 180.0f;
    }
    req.data.ownerScale = data->owner->scale_xz;
    req.spawn.itemCount = 25;
    req.spawn.pos = data->pos;
    req.spawn.life = 300;
    req.spawn.flags = 54;
    req.spawn.kind = 19;
    req.spawn.shapeKind = 6;
    req.spawn.material = 0;
    trail = effect_newEffectObj(&req.spawn, sizeof(TrackerTrailData), 0x28, 0, 0x11, 0x12, 0, 0, 0, effect->base.header.owner, effect->base.header.kind);
    if (trail != NULL) {
        memcpy(trail->state, &req.data, sizeof(TrackerTrailData));
        data->trail = trail;
    }
}
