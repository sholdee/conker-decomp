#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

extern f32 D_800A1950[];

s32 func_1509BE40();
void func_15143134(f32 *, f32 *, Mtx *);

/* State the emitter object keeps at +0x28: func_150F2C8C fills it, func_150F2D14 updates it each frame. */
typedef struct {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetId;   // target->unique_id when attached; a mismatch means the slot was reused
    /* 0x05 */ u8 prevUnk222; // target->unk222 seen last frame
    /* 0x08 */ f32 timer;     // one particle is emitted per 1.0 accumulated
    /* 0x0C */ s8 count;      // particles left in the current burst
} EmitterState;

/* The emitter object spawned by func_150F2C8C. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EmitterState state;
} Emitter;

/* State each emitted particle keeps after its Particle (copied in by func_150F2D14, updated by func_150F3214). */
typedef struct {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetId;
    /* 0x08 */ f32 age;
    /* 0x0C */ f32 lifetime;
    /* 0x10 */ f32 growth;    // width = height = growth * sqrtf(age)
    /* 0x14 */ f32 fadeFrom;  // def.unk44 = fadeFrom - fadeRate * age * age
    /* 0x18 */ f32 fadeRate;  // fadeFrom / (lifetime * lifetime): def.unk44 reaches 0 at lifetime
    /* 0x1C */ Vec3f axisU; // quad axes for func_15140410
    /* 0x28 */ Vec3f axisV;
} ParticleState;

/* A particle spawned by func_1513D2F0. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ ParticleState state;
} Particle;

// matching: D_800CC5EC is movement_obj[0].unk31C; func_150F2A60 tests it through the array and reads through
// this alias so IDO reloads it (as the original does) instead of reusing the first load
extern struct {
    u8 pad0[0x120];
    u8 unk120;
} *D_800CC5EC;
extern u8 D_800CC40C;
WorldObject *func_151149AC(s32);
void func_15136C3C(Actor *, s32, s32, s32, s32, s32, s32, s32);
void func_1507CD64(Actor *, s32);
void func_15196318(s32, s32, s32);

/* While movement_obj[0] has unk31C: if entry 0xFA or 0xF9 of db_Object_array has its D_800DBF94 bit 0 set, runs
 * movement_obj[0] through func_15136C3C .. func_1507CD64(6) once; then offsets the four D_800BE4F0 entries
 * by object 0xE's unk2E4. */
void func_150F2A60(s32 arg0) {
    s32 i;
    s32 scroll;
    Actor *obj;
    WorldObject *entry;
    s32 entryIndex;

    if (movement_obj->unk31C != NULL) {
        if (D_800CC5EC->unk120 == 0) {
            // matching: `| 0` hides the induction variable, so IDO keeps `slti` instead of testing i != 2
            for (i = 0; i < 2; i = (i + 1) | 0) {
                entry = func_151149AC((0xFA - i) & 0xFF);
                entryIndex = entry - db_Object_array;
                if (D_800DBF94[entryIndex] & 1) {
                    if (D_800CC40C != 0) {
                        func_15060F28(&movement_obj[D_800CC40C - 100], 0);
                    }
                    D_800CC49A = 0;
                    func_15136C3C(movement_obj, 0, 0, 1, 1, 0, 0xFF, 1);
                    func_10010630(0x627, movement_obj, 0x6D60, 500, 1000);
                    func_15145A50(movement_obj);
                    func_1507CD64(movement_obj, 6);
                    break;
                }
            }
            obj = func_15083E90(0xE);
            if (obj != NULL) {
                scroll = obj->unk2E4;
            } else {
                scroll = 0;
            }
            func_15196318(D_800BE4F0->unk0, scroll, 0);
            func_15196318(D_800BE4F0->unk4, 0, -scroll);
            func_15196318(D_800BE4F0->unk8, scroll, 0);
            func_15196318(D_800BE4F0->unkC, -scroll, 0);
        }
    }
}

/* Spawns an emitter object that follows `target` (see func_150F2D14). */
void func_150F2C8C(Actor *target) {
    EmitterState state;
    Emitter *emitter;

    state.target = target;
    state.targetId = target->unique_id;
    state.prevUnk222 = 0;
    state.timer = 0.0f;
    state.count = 0;

    emitter = (Emitter *)func_15149130(300, -1, 0x5C, -1, 0, 0x44, 0x10, 0xFF, 1);
    if (emitter != NULL) {
        memcpy(&emitter->state, &state, sizeof(state));
    }
}

extern f32 D_800A195C[];
void func_1502EA98(Actor *, s32, s32, s32, s32, s32, s32);
void func_15145EA4(f32 **, f32 **, Mtx *, s32);
extern s32 func_15146078(Vec3f *, Vec3f *, Vec3f *);

/* Emitter update: while the target lives, emits a burst of 3 particles from its bone 15 each time its unk222
 * changes to a nonzero value. */
void func_150F2D14(Emitter *emitter) {
    EmitterState *state;
    Actor *target;
    ParticleDef def;
    ParticleState particle;
    Particle *spawned;

    state = &emitter->state;
    if (D_800C35EA[0] == 1) {
        return;
    }
    target = state->target;
    if (target->interaction_state == 0 || target->unique_id != state->targetId) {
        emitter->base.header.state = -1;
        return;
    }
    if (state->prevUnk222 != target->unk222 && target->unk222 != 0) {
        state->count = 3;
        state->timer = 0.0f;
    }
    state->prevUnk222 = target->unk222;
    if (target->unk222 != 0) {
        func_1502EA98(&movement_obj[target->unk222], 0xFF, 0, 0, 0x50, 0, 0xF);
    }
    if (target->unk1D4 != NULL && state->count > 0) {
        state->timer += 0.148f * D_800BE9A4;
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
                    f32 *src[2];
                    f32 *dst[2];
                    Vec3f dir;

                    src[0] = D_800A1950;
                    src[1] = D_800A195C;
                    dst[0] = &def.pos.x;
                    dst[1] = &dir.x;
                    func_15145EA4(src, dst, (Mtx *)target->unk1D4 + 15, 2);
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
void func_150F3194(Emitter *emitter, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &emitter->state.target, &emitter->state.targetId, emitter);
}

/* Hands the particle's target pointer and id to func_15169850. */
void func_150F31D4(Particle *particle, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &particle->state.target, &particle->state.targetId, particle);
}

/* Particle update: follows the target's bone 15, grows and fades with age. Returns 0 when it should die. */
s32 func_150F3214(Particle *particle) {
    ParticleState *state;
    Actor *target;
    f32 size;

    state = &particle->state;
    target = state->target;

    if (target->interaction_state == 0 || target->unique_id != state->targetId || target->unk1D4 == NULL) {
        return 0;
    }

    func_15143134(D_800A1950, &particle->head.def.pos.x, (Mtx *)target->unk1D4 + 15);

    size = sqrtf(state->age) * state->growth;
    particle->head.def.height = size;
    particle->head.def.width = size;

    particle->head.def.unk44 = (u32)(state->fadeFrom - ((state->fadeRate * state->age) * state->age));
    state->age = state->age + D_800BE9A4;

    if (state->lifetime < state->age) {
        return 0;
    }
    return 1;
}

/* Draws the particle as a quad spanned by its axes. */
void func_150F337C(Particle *particle, s16 viewport) {
    func_15140410(particle, &particle->state.axisU, &particle->state.axisV, viewport);
}

/* Sets bit 0 of obj->flags unless camera_global_Cameras->eye.z is below -2000. */
void func_150F33B0(WorldObject *obj) {
    if (camera_global_Cameras->eye.z < -2000.0f) {
        obj->object_status &= ~1;
        return;
    }
    obj->object_status = (obj->object_status & ~1) | 1; // matching: a bitfield-style insert; a plain |= 1 allocates other temps
}

/* While camera_global_Cameras[player].eye.z is below -2100, mirrors (unk2FC > 840) into bit `player` of *D_800D9A40. */
void func_150F33F8(s32 player) {
    if (camera_global_Cameras[player].eye.z < -2100.0f) {
        if (camera_global_Cameras[player].eye.y > 840.0f) {
            *D_800D9A40 |= 1 << player;
            return;
        }
        *D_800D9A40 &= ~(1 << player);
    }
}

/* Animation speed for `pitch`: 0.75, or faster the further it is below -5. */
f32 func_150F34A0(Actor *obj, f32 pitch) {
    f32 speed;

    if (pitch < -5.0f) {
        speed = pitch * -0.01f + 0.7f;
    } else {
        speed = 0.75f;
    }

    return speed;
}

void func_150585F0(Actor *arg0, f32 arg1);
void func_15052590(Actor *arg0);
u8 func_150599C8(Actor *arg0, u8 arg1, u16 arg2);
f32 func_1505A3A8(f32 arg0, void *arg1, f32 arg2, f32 arg3, u8 arg4);
void func_15073FA0(void);


/* Per-frame control of a flying mode: the stick steers and pitches, B thrusts, A reverses, R holds altitude,
 * Z fires (only when D_800BE9F0 is 0x3C and fewer than 5 id-0x25 objects exist). Also picks up the
 * D_800CC26D object into unk13C and drops it again when unk25C bit 1 is set. */
void func_150F34F4(Actor *actor) {
    u16 angle;
    f32 mag;
    f32 accel;
    f32 temp; // matching: one local for stick_x and the climb limit (split locals move FP registers)
    f32 pitch;
    f32 speed; // capped xz speed, then the func_1505A3A8 rate, then the old y_velocity (IDO's allocation)
    f32 animSpeed;
    s32 anim;
    s32 count;
    f32 floorLevel;
    s32 rHeld;
    s32 i;
    f32 dist;
    f32 minDist;
    Actor *obj;
    s32 idx;

    floorLevel = 0.0f;
    actor->unkAB = 1;
    actor->unk80 = 1;
    actor->unk222 = 0;
    D_800CC288 = D_800BE710[0];
    D_800CC284 = D_800BE728[0];
    if ((dc_glbl_CharNum <= D_80082FA0) && (D_800DDE3C[dc_glbl_CharNum] != 0) && (D_800DDDC8[dc_glbl_CharNum] > 0.75f) && (D_800E0B94 != 2)) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->button = 0;
        D_800CC288 = 0;
    }
    actor->unkF8 |= 0x40;
    actor->unk1CC = actor->y_position;
    if (actor->unk103 != 0) {
        actor->unk103--;
    }
    if (actor->stunned != 0) {
        func_150585F0(actor, 0.25f);
        func_15059140(actor);
        actor->unk40 = ((s16)actor->unk7A + 0x4000) * (360.0f / 65536.0f);
        if (actor->unk13C != 0) {
            Actor *rider;

            idx = actor->unk13C - 100;
            rider = &movement_obj[idx];
            rider->unk13D = 0;
            rider->xz_velocity = actor->xz_velocity;
            rider->unk232 = 0x21;
            rider->unk218 = 0;
            rider->stunned = 0;
            rider->unkF8 &= ~0x400;
            actor->unk13C = 0;
        }
        return;
    }
    if (actor->unk102 == 0) {
        actor->in_water = 0;
        angle = func_1505A630(D_800CC284->stick_x, D_800CC284->stick_y, 0);
        mag = func_1505A5CC(D_800CC284);
        if (D_800CC284->button & R_TRIG) {
            mag = 0.0f;
        }
        angle += D_800CC280;
        actor->unk78 = actor->unk7A;
        if (mag > 1.0f) {
            actor->unk78 = angle;
        }
        actor->unk44 = mag * 0.35f;
        actor->unk232 = 7;
        actor->unk218 = 0;
        if ((D_800CC288 & (A_BUTTON | B_BUTTON)) || (actor->unk28 > 100.0f)) {
            actor->unk232 = 8;
            actor->unk102 = 1;
        }
        func_15052590(actor);
        if (D_800CC26D != 0) {
            if (actor->unk28 > 30.0f) {
                D_800D1580 = 0xFF010074;
                func_1506E8D8();
                func_1505959C(&movement_obj[D_800CC26D - 100], dc_glbl_CharNum);
                actor->unk13C = D_800CC26D;
                actor->unk102 = 1;
            }
            actor->y_velocity = 23.0f;
        }
        if (actor->y_position < actor->unk118) {
            actor->unk102 = 1;
            actor->unk86 = 1;
            actor->y_velocity = -6.0f;
        }
        return;
    }
    speed = actor->xz_velocity;
    if (speed > 60.0f) {
        speed = 60.0f;
    }
    pitch = D_800CC284->stick_y * 1.25f;
    rHeld = D_800CC284->button & R_TRIG;
    if (rHeld) {
        pitch = 0.0f;
    }
    if (actor->unk13C != 0) {
        if (!rHeld) {
            actor->y_position -= 16.0f * D_800D1550;
        }
    } else {
        minDist = 1000.0f;
        for (i = 0; i < 25; i++) {
            if ((movement_obj[i].interaction_state != 0) && (movement_obj[i].health != 0) &&
                (movement_obj[i].unk28 == 0.0f) && (movement_obj[i].unk232 != 0x21) &&
                ((movement_obj[i].id == 0x9C) || (movement_obj[i].id == 0x9D))) {
                dist = func_1505A6F8(D_800D154C, &movement_obj[i]);
                if (dist < minDist) {
                    actor->unk222 = i;
                    minDist = dist;
                }
            }
        }
    }
    if (pitch < -90.0f) {
        pitch = -90.0f;
    }
    if (pitch > 90.0f) {
        pitch = 90.0f;
    }
    if (actor->y_position > 2100.0f) {
        if (actor->y_velocity > -30.0f) {
            actor->y_velocity -= 1.5f;
        }
        pitch = 40.0f;
    }
    temp = D_800CC284->stick_x;
    actor->unkC4 += (temp - actor->unkC4) * (0.2f * D_800D1550);
    actor->unk76 -= (s16)(actor->unkC4 * D_800D1550 * 0.04f * ((100.0f - speed) * 0.015f) * 220.0f);
    actor->unk7E = 0x19;
    func_150599C8(actor, 8, actor->unk76);
    actor->target_speed = 0.0f;
    speed = 0.0f;
    accel = 1.3f;
    if (actor->y_position < actor->unk118) {
        if (actor->unk86 == 0) {
            actor->unk86 = 1;
            actor->xz_velocity = 0.0f;
            actor->y_velocity = -6.0f;
        }
    } else if (actor->unk86 != 0) {
        actor->unk86 = 0;
        actor->y_velocity = 9.0f;
    }
    if (!(D_800CC284->button & R_TRIG)) {
        if (D_800CC284->button & A_BUTTON) {
            actor->target_speed = -22.0f;
            speed = 1.0f;
            accel = 1.8f;
        }
        if (D_800CC284->button & B_BUTTON) {
            actor->target_speed = 30.0f;
            if (actor->y_velocity < 0.0f) {
                actor->target_speed -= actor->y_velocity * 0.6f;
            }
            speed = 1.5f;
            accel = 0.3f;
        }
    }
    func_1505A3A8(actor->target_speed, actor, speed, accel, 1);
    actor->unk40 = ((s16)actor->unk7A + 0x4000) * (360.0f / 65536.0f);
    actor->gravity = pitch * 0.000125f * (200.0f - actor->xz_velocity);
    if (actor->unk13C == 0) {
        if (actor->y_velocity > 26.0f) {
            actor->y_velocity = 26.0f;
        }
        if (actor->y_velocity < -34.0f) {
            actor->y_velocity = -34.0f;
        }
    }
    temp = fabsf(pitch * 0.5f);
    if (actor->unk86 != 0) {
        temp = temp * 0.4f;
        actor->gravity *= 0.6f;
    }
    if (actor->gravity == 0.0f) {
        if (fabsf(actor->y_velocity) < 1.5f) {
            actor->y_velocity = 0.0f;
        } else if (actor->y_velocity > 0.0f) {
            actor->gravity = 1.6f;
        } else {
            actor->gravity = -1.6f;
        }
    } else if ((actor->y_velocity > temp * 1.2f) && (actor->gravity < 0.0f)) {
        actor->gravity = 0.0f;
    } else if ((actor->y_velocity < -temp * 1.5f) && (actor->gravity > 0.0f)) {
        actor->gravity = 0.0f;
    }
    actor->in_water = 10;
    if (D_800CC284->button & R_TRIG) {
        actor->unkB8 += (D_800CC284->stick_y - actor->unkB8) * 0.2f;
        actor->y_velocity = 0.0f;
        actor->gravity = 0.0f;
    } else {
        actor->unkB8 = (actor->y_velocity - floorLevel) * -2.0f;
        if (actor->unkB8 > 40.0f) {
            actor->unkB8 = 40.0f;
        } else if (actor->unkB8 < -55.0f) {
            actor->unkB8 = -55.0f;
        }
    }
    speed = actor->y_velocity;
    D_800CBDD3 = 1;
    func_15059140(actor);
    D_800CBDD3 = 0;
    if ((actor->unk13C != 0) && (actor->unk28 < 50.0f)) {
        floorLevel = 50.0f;
    }
    if (!(D_800CC284->button & R_TRIG)) {
        if ((D_800CC26D != 0) && (actor->unk13C == 0) && (movement_obj[D_800CC26D - 100].unk28 == 0.0f)) {
            actor->y_position = movement_obj[D_800CC26D - 100].y_position + 70.0f;
            actor->y_velocity = 20.0f;
            func_1505959C(&movement_obj[D_800CC26D - 100], dc_glbl_CharNum);
            actor->unk13C = D_800CC26D;
            D_800D1580 = 0xFF010074;
            func_1506E8D8();
        }
        if (actor->unk13C != 0) {
            if (actor->unk25C & 2) {
                Actor *rider;

                actor->unk25C &= ~2;
                idx = actor->unk13C - 100;
                rider = &movement_obj[idx];
                rider->unk13D = 0;
                rider->xz_velocity = actor->xz_velocity;
                rider->unk232 = 0x21;
                rider->unk218 = 0;
                rider->stunned = 0;
                rider->unkF8 &= ~0x400;
                actor->unk13C = 0;
                actor->y_velocity = 0.0f;
                D_800D1580 = 0x76;
                func_1506E8D8();
            }
        } else if ((D_800CC288 & Z_TRIG) && (actor->unk103 == 0)) {
            count = 0;
            if (D_800BE9F0 == 0x3C) {
                // matching: init on the do's line keeps as1's order for the two address loads
                obj = &movement_obj[1]; do {
                    if ((obj->interaction_state != 0) && (obj->id == 0x25)) {
                        count++;
                    }
                    obj++;
                } while (obj != &D_800D121C);
                if (count < 5) {
                    D_800D1580 = 6;
                    actor->unk103 = 4;
                    func_15073FA0();
                }
            }
        }
    }
    if (actor->unk28 <= floorLevel) {
        actor->xz_velocity *= 0.6f;
        if ((D_800CC284->button & B_BUTTON) || (actor->unk86 != 0) || (actor->unk13C != 0)) {
            if (actor->unk13C != 0) {
                if (actor->y_velocity < -10.0f) {
                    D_800D1580 = 0xFF0100A7;
                    func_1506E5FC();
                    D_800D1580 = 0xFF060372;
                    func_1506E8D8();
                }
            } else if (actor->y_position < 0.0f) {
                D_800D1580 = 0xFF010072;
                func_1506E8D8();
            }
            if (actor->unk13C == 0) {
                actor->y_velocity = 20.0f;
            } else if (actor->unk86 == 0) {
                actor->y_velocity = 38.0f;
            }
            pitch = -80.0f;
        } else {
            actor->y_velocity = speed * -1.0f;
            actor->unk102 = 0;
            actor->unk28 = 0.1f;
            actor->unkB8 = 0.0f;
        }
    }
    if (!(D_800CC284->button & R_TRIG)) {
        actor->unkB8 += 20.0f;
    } else {
        actor->y_velocity = actor->unkB8 * -0.35f;
    }
    if (actor->unk86 != 0) {
        actor->unk83 = 0;
        if ((fabsf(pitch) > 8.0f) || (actor->target_speed > 7.0f)) {
            animSpeed = 1.0f;
        } else {
            animSpeed = 0.5f;
        }
    } else {
        animSpeed = func_150F34A0(actor, pitch);
    }
    anim = 0xF;
    if ((actor->unkB8 > 40.0f) && (pitch > 20.0f)) {
        anim = 0x11;
    }
    if ((actor->xz_velocity < 20.0f) && (actor->target_speed <= 0.0f) && (pitch == 0.0f)) {
        anim = 0x18;
    }
    if (actor->xz_velocity < 0) {
        anim = 0x1F;
    }
    if (actor->unk13C != 0) {
        anim = 0x17;
        animSpeed += 0.2f;
    }
    func_1505E650(actor, anim, animSpeed, 9.0f, 0.0f, 0.0f, 0);
}

void func_150F43F0(Camera *actor) {
    if (actor->zone == 0x3B) {
        func_1509BFB0(0, 0x405C, 1);
        if (actor->mode != 0x100 && actor->cmdRunning == 0) {
            if (func_15123934(actor, 8, 0, 0, 0) != 0) {
                actor->flags |= 0x300000;
                actor->flags &= ~4;
                actor->preset = 1;
                actor->presetMask = 3;
                func_15124B18(actor);
            }
            actor->unk134 = 0;
            actor->height = 125.0f;
            actor->unk34C = 125.0f;
            actor->targetDistance = 220.0f;
            actor->liftTarget = 30.0f;
        } else {
            actor->liftTarget = 0.0f;
        }
    } else if (actor->mode == 8) {
        if (actor->cmdRunning == 0) {
            func_151239CC(actor, 0);
            func_1509BFB0(0, 0x405C, 0);
        }
    }

    if (func_1509BE40(1, 0x4054, 6, 0x9000) != 0 && func_1509BE40(1, 0x405E, 6, 0x9000) == 0) {
        actor->flags |= 0x80000000;
    } else {
        actor->flags &= ~0x80000000;
    }
}
