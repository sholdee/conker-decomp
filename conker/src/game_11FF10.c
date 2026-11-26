#include <ultra64.h>
#include "functions.h"
#include "variables.h"

static const Vec3f D_800A1950 = { 0.0f, 24.0f, 11.0f };
static const Vec3f D_800A195C = { 0.0f, 71.0f, 9.0f };

ZoneNode *D_800D9A40;


/* While movement_obj[0] has mv: if entry 0xFA or 0xF9 of db_Object_array has its gWorldObjectContacts bit 0 set, runs
 * movement_obj[0] through func_15136C3C .. actor_kill(6) once; then offsets the four D_800BE4F0 entries
 * by object 0xE's taskint[0]. */
void func_150F2A60(s32 arg0) {
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
                    func_15136C3C(movement_obj, 0, 0, 1, 1, 0, 0xFF, 1);
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
            flamethrower_setTexScrollSpeed(D_800BE4F0[0], scroll, 0);
            flamethrower_setTexScrollSpeed(D_800BE4F0[1], 0, -scroll);
            flamethrower_setTexScrollSpeed(D_800BE4F0[2], scroll, 0);
            flamethrower_setTexScrollSpeed(D_800BE4F0[3], -scroll, 0);
        }
    }
}

/* Spawns an emitter object that follows `target` (see func_150F2D14). */
void func_150F2C8C(Actor *target) {
    BoneBurstState state;
    BoneBurstEmitter *emitter;

    state.target = target;
    state.targetId = target->unique_id;
    state.prevUnk222 = 0;
    state.timer = 0.0f;
    state.count = 0;

    emitter = func_15149130(300, -1, 0x5C, -1, 0, 0x44, 0x10, 0xFF, 1);
    if (emitter != NULL) {
        memcpy(&emitter->state, &state, sizeof(state));
    }
}

/* Emitter update: while the target lives, emits a burst of 3 particles from its bone 15 each time its unk222
 * changes to a nonzero value. */
void func_150F2D14(BoneBurstEmitter *emitter) {
    BoneBurstState *state;
    Actor *target;
    ParticleDef def;
    ParticleState particle;
    BoneBurstParticle *spawned;

    state = &emitter->state;
    if (D_800C35EA[0] == 1) {
        return;
    }
    target = state->target;
    if (target->type == 0 || target->unique_id != state->targetId) {
        emitter->base.header.state = -1;
        return;
    }
    if (state->prevUnk222 != target->unk222 && target->unk222 != 0) {
        state->count = 3;
        state->timer = 0.0f;
    }
    state->prevUnk222 = target->unk222;
    if (target->unk222 != 0) {
        actor_flashFog(&movement_obj[target->unk222], 0xFF, 0, 0, 0x50, 0, 0xF);
    }
    if (target->ob_matrix_ptr != NULL && state->count > 0) {
        state->timer += 0.148f * gFrameDelta;
        if (state->timer > 1.0f) {
            do {
                particle.target = target;
                particle.targetId = target->unique_id;
                particle.age = 0.0f;
                particle.lifetime = func_150ADA68() * 6.0f + 13.0f;
                particle.growth = 58.14f;
                particle.fadeFrom = func_150ADA68() * 40.0f + 60.0f;
                particle.fadeRate = particle.fadeFrom / (particle.lifetime * particle.lifetime);
                {
                    const Vec3f *src[2];
                    Vec3f *dst[2];
                    Vec3f dir;

                    src[0] = &D_800A1950;
                    src[1] = &D_800A195C;
                    dst[0] = &def.pos;
                    dst[1] = &dir;
                    func_15145EA4(src, dst, target->ob_matrix_ptr + 15, 2);
                    dir.x -= def.pos.x;
                    dir.y -= def.pos.y;
                    dir.z -= def.pos.z;
                    if (!func_15146078(&dir, &particle.axisU, &particle.axisV)) {
                        return;
                    }
                    def.unk44 = particle.fadeFrom;
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
                    def.rot = D_800A5480;
                    def.scale.x = 1.0f;
                    def.scale.y = 1.0f;
                    def.scale.z = 1.0f;
                    def.flags = 0x04EC0000;
                    def.unk45 = 0xFF;
                    def.unk46 = 0;
                    def.unk47 = 6;
                    def.unk48 = NULL;
                    def.viewportMask = 0xFF;
                    def.unk50 = 0;
                    def.fadeStart = 1;
                    def.fadeRate = 0xFF;
                    spawned = func_1513D2F0(&def, &D_800A4AA0, 0x26, 0, 0, 0x25, 0, 0, 0, 0x34, emitter->base.header.owner, emitter->base.header.kind);
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

/* Hands the emitter's target pointer and id to func_15169850. */
void func_150F3194(BoneBurstEmitter *emitter, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &emitter->state.target, &emitter->state.targetId, emitter);
}

/* Hands the particle's target pointer and id to func_15169850. */
void func_150F31D4(BoneBurstParticle *particle, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &particle->state.target, &particle->state.targetId, particle);
}

/* Particle update: follows the target's bone 15, grows and fades with age. Returns 0 when it should die. */
s32 func_150F3214(BoneBurstParticle *particle) {
    ParticleState *state;
    Actor *target;
    f32 size;

    state = &particle->state;
    target = state->target;

    if (target->type == 0 || target->unique_id != state->targetId || target->ob_matrix_ptr == NULL) {
        return 0;
    }

    func_15143134(&D_800A1950, &particle->head.def.pos, target->ob_matrix_ptr + 15);

    size = sqrtf(state->age) * state->growth;
    particle->head.def.height = size;
    particle->head.def.width = size;

    particle->head.def.unk44 = (u32)(state->fadeFrom - ((state->fadeRate * state->age) * state->age));
    state->age = state->age + gFrameDelta;

    if (state->lifetime < state->age) {
        return 0;
    }
    return 1;
}

/* Draws the particle as a quad spanned by its axes. */
void func_150F337C(BoneBurstParticle *particle, s16 viewport) {
    func_15140410(&particle->head, &particle->state.axisU, &particle->state.axisV, viewport);
}

/* Sets bit 0 of obj->flags unless camera_global_Cameras->eye.z is below -2000. */
void func_150F33B0(WorldObject *obj) {
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
