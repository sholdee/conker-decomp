#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/blood.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/flamethrower.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/level.h"
#include "conker/particle.h"
#include "conker/sound.h"

static const Vec3f gBoneBurstStartOffset = { 0.0f, 24.0f, 11.0f };
static const Vec3f gBoneBurstDirOffset = { 0.0f, 71.0f, 9.0f };

ZoneNode *D_800D9A40;


/* While movement_obj[0] has mv: if entry 0xFA or 0xF9 of db_Object_array has its gWorldObjectContacts bit 0 set, runs
 * movement_obj[0] through blood_spawnHitEffects .. actor_kill(6) once; then offsets the four gW3HauntedHouseTexScrolls entries
 * by object 0xE's taskint[0]. */
void particle_levelUpdateScrollingHazardProps(s32 unused) {
    s32 i;
    s32 scroll;
    Actor *obj;
    WorldObject *entry;
    s32 entryIndex;
    Actor *player;

    player = movement_obj;
    // matching: the ROM loads player->mv twice, with no store between (the Oct 2000 debug build too). IDO
    // merges two reads of one field with the same type into one load, so the test reads the pointer as a word.
    if (*(s32 *)&player->mv != 0) {
        if (player->mv->death_State == 0) {
            // matching: `| 0` hides the induction variable, so IDO keeps `slti` instead of testing i != 2
            for (i = 0; i < 2; i = (i + 1) | 0) {
                entry = gfx_findObjectById(0xFA - i);
                entryIndex = entry - db_Object_array;
                if (gWorldObjectContacts[entryIndex] & 1) {
                    if (player->lifting) {
                        actor_remove(&movement_obj[player->lifting - 100], 0);
                    }
                    player->energy = 0;
                    blood_spawnHitEffects(movement_obj, 0, 0, 1, 1, 0, 0xFF, 1);
                    sound_playActorUntracked(0x627, movement_obj, 0x6D60, 500, 1000);
                    func_15145A50(movement_obj);
                    actor_kill(movement_obj, 6);
                    break;
                }
            }
            obj = collision_findObjectById(0xE);
            if (obj != NULL) {
                scroll = obj->taskint[0];
            } else {
                scroll = 0;
            }
            flamethrower_setTexScrollSpeed(gW3HauntedHouseTexScrolls[0], scroll, 0);
            flamethrower_setTexScrollSpeed(gW3HauntedHouseTexScrolls[1], 0, -scroll);
            flamethrower_setTexScrollSpeed(gW3HauntedHouseTexScrolls[2], scroll, 0);
            flamethrower_setTexScrollSpeed(gW3HauntedHouseTexScrolls[3], -scroll, 0);
        }
    }
}

/* Spawns an emitter object that follows `target` (see particle_updateBoneBurstEmitter). */
void particle_spawnBoneBurstEmitter(Actor *target) {
    BoneBurstState state;
    BoneBurstEmitter *emitter;

    state.target = target;
    state.targetId = target->unique_id;
    state.prevTargetIndex = 0;
    state.timer = 0.0f;
    state.count = 0;

    emitter = effect_newTimedEffect(300, -1, 0x5C, -1, 0, 0x44, 0x10, 0xFF, 1);
    if (emitter != NULL) {
        memcpy(&emitter->state, &state, sizeof(state));
    }
}

/* Emitter update: while the target lives, emits a burst of 3 particles from its bone 15 each time its targetIndex
 * changes to a nonzero value. */
void particle_updateBoneBurstEmitter(BoneBurstEmitter *emitter) {
    BoneBurstState *state;
    Actor *target;
    ParticleDef def;
    ParticleState particle;
    BoneBurstParticle *spawned;

    state = &emitter->state;
    if (gCutsceneStates[0] == 1) {
        return;
    }
    target = state->target;
    if (target->type == 0 || target->unique_id != state->targetId) {
        emitter->base.header.state = -1;
        return;
    }
    if (state->prevTargetIndex != target->targetIndex && target->targetIndex != 0) {
        state->count = 3;
        state->timer = 0.0f;
    }
    state->prevTargetIndex = target->targetIndex;
    if (target->targetIndex != 0) {
        actor_flashFog(&movement_obj[target->targetIndex], 0xFF, 0, 0, 0x50, 0, 0xF);
    }
    if (target->ob_matrix_ptr != NULL && state->count > 0) {
        state->timer += 0.148f * gFrameDelta;
        if (state->timer > 1.0f) {
            do {
                particle.target = target;
                particle.targetId = target->unique_id;
                particle.age = 0.0f;
                particle.lifetime = math_randomFloat() * 6.0f + 13.0f;
                particle.growth = 58.14f;
                particle.fadeFrom = math_randomFloat() * 40.0f + 60.0f;
                particle.fadeRate = particle.fadeFrom / (particle.lifetime * particle.lifetime);
                {
                    const Vec3f *src[2];
                    Vec3f *dst[2];
                    Vec3f dir;

                    src[0] = &gBoneBurstStartOffset;
                    src[1] = &gBoneBurstDirOffset;
                    dst[0] = &def.pos;
                    dst[1] = &dir;
                    math_transformOffsets(src, dst, target->ob_matrix_ptr + 15, 2);
                    dir.x -= def.pos.x;
                    dir.y -= def.pos.y;
                    dir.z -= def.pos.z;
                    if (!math_buildPerpendicularBasis(&dir, &particle.axisU, &particle.axisV)) {
                        return;
                    }
                    def.alpha = particle.fadeFrom;
                    def.texture = 0x79;
                    def.type = 0xC;
                    def.renderModes = 0x4405;
                    def.lifetime = 300;
                    def.frame = 0;
                    def.frameSpeed = 0;
                    def.rgba[0] = 0xD5;
                    def.rgba[1] = 0xDB;
                    def.rgba[2] = 0xFF;
                    def.rgba[3] = 0xFF;
                    def.height = 0.0f;
                    def.width = 0.0f;
                    def.rot = gZeroVec3f;
                    def.scale.x = 1.0f;
                    def.scale.y = 1.0f;
                    def.scale.z = 1.0f;
                    def.flags = 0x04EC0000;
                    def.colorLevel = 0xFF;
                    def.primMode = 0;
                    def.envMode = 6;
                    def.zone = NULL;
                    def.viewportMask = 0xFF;
                    def.material = 0;
                    def.fadeStart = 1;
                    def.fadeRate = 0xFF;
                    spawned = particle_newQuadParticle(&def, &gWhiteVtxColors, 0x26, 0, 0, 0x25, 0, 0, 0, 0x34, emitter->base.header.owner, emitter->base.header.kind);
                    if (spawned != NULL) {
                        memcpy(&spawned->state, &particle, sizeof(particle));
                    }
                    state->count--;
                    state->timer -= 1.0f;
                }
            } while (state->timer > 1.0f && state->count > 0);
        }
    }
}

/* Hands the emitter's target pointer and id to effect_handleOwnerActorEvent. */
void particle_onBoneBurstEmitterEvent(BoneBurstEmitter *emitter, s32 event, u8 eventType) {
    effect_handleOwnerActorEvent(event, eventType, &emitter->state.target, &emitter->state.targetId, emitter);
}

/* Hands the particle's target pointer and id to effect_handleOwnerActorEvent. */
void particle_onBoneBurstParticleEvent(BoneBurstParticle *particle, s32 event, u8 eventType) {
    effect_handleOwnerActorEvent(event, eventType, &particle->state.target, &particle->state.targetId, particle);
}

/* Particle update: follows the target's bone 15, grows and fades with age. Returns 0 when it should die. */
s32 particle_updateBoneBurstParticle(BoneBurstParticle *particle) {
    ParticleState *state;
    Actor *target;
    f32 size;

    state = &particle->state;
    target = state->target;

    if (target->type == 0 || target->unique_id != state->targetId || target->ob_matrix_ptr == NULL) {
        return 0;
    }

    math_transformOffset(&gBoneBurstStartOffset, &particle->head.def.pos, target->ob_matrix_ptr + 15);

    size = sqrtf(state->age) * state->growth;
    particle->head.def.height = size;
    particle->head.def.width = size;

    particle->head.def.alpha = (u32)(state->fadeFrom - ((state->fadeRate * state->age) * state->age));
    state->age = state->age + gFrameDelta;

    if (state->lifetime < state->age) {
        return 0;
    }
    return 1;
}

/* Draws the particle as a quad spanned by its axes. */
void particle_buildBoneBurstQuad(BoneBurstParticle *particle, s16 viewport) {
    particle_buildQuadFromAxes(&particle->head, &particle->state.axisU, &particle->state.axisV, viewport);
}

/* Clears bit 0 of obj->object_status (the object is not drawn) while camera_global_Cameras->eye.z is below -2000,
 * sets it otherwise. */
void particle_cullObjectByCameraZ(WorldObject *obj) {
    if (camera_global_Cameras->eye.z < -2000.0f) {
        obj->object_status &= ~1;
        return;
    }
    obj->object_status = (obj->object_status & ~1) | 1; // matching: a bitfield-style insert; a plain |= 1 allocates other temps
}

/* While camera_global_Cameras[player].eye.z is below -2100, mirrors (unk2FC > 840) into bit `player` of D_800D9A40->unk0. */
void func_150F33F8(s32 player) {
    if (camera_global_Cameras[player].eye.z < -2100.0f) {
        if (camera_global_Cameras[player].eye.y > 840.0f) {
            D_800D9A40->unk0 |= 1 << player;
            return;
        }
        D_800D9A40->unk0 &= ~(1 << player);
    }
}
