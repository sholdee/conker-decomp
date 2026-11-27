#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/music.h"
#include "conker/sound.h"
#include "conker/weapon.h"

/*
 * Object type 0x27: moves from pos toward target (a fixed point, a Actor or a followed f32[3]) at
 * speed units per frame, spawning particles that run from origin to the moving point. Live effects are
 * registered in the 8 handle slots gHomingEffects (handle = slot + 1).
 */
struct HomingEffect {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ Vec3f target;
    /* 0x28 */ Vec3f origin;
    /* 0x34 */ f32 speed;
    /* 0x38 */ f32 startY;
    /* 0x3C */ Actor *targetObj;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 lingerTimer;
    /* 0x48 */ u8 slot;
    /* 0x49 */ u8 pad49;
    /* 0x4A */ u16 sound;
    /* 0x4C */ f32 particleAccum;
    /* 0x50 */ f32 sparkAccum;
    /* 0x54 */ CollisionHit ground;   /* the floor below the last step */
    /* 0x78 */ f32 *follow;
    /* 0x7C */ s16 lifetime; /* frames left, or -99 for unlimited */
};

/* The live homing effects by handle - 1. */
HomingEffect *gHomingEffects[8];

/* Creates an effect from (x, y, z) toward (targetX, targetY, targetZ) and registers it in a free slot. */
HomingEffect *effect_newHomingEffect(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ, s32 arg6, f32 speed,
                      Actor *targetObj, f32 originX, f32 originY, f32 originZ, f32 *follow, s16 lifetime) {
    HomingEffect *effect;
    s32 i;

    effect = effect_newObject(0x27, 0, 0x80, 1, 0xFF, 1);
    if (effect == NULL) {
        return NULL;
    }
    effect->particleAccum = 0.0f;
    effect->sparkAccum = 0.0f;
    effect->ground.height = -10000.0f;
    effect->ground.material = 0;
    effect->ground.flags = 0;
    effect->ground.source = 0;
    effect->ground.surface = NULL;
    effect->lifetime = lifetime;
    effect->follow = follow;
    effect->pos.x = x;
    effect->pos.y = y;
    effect->pos.z = z;
    effect->target.x = targetX;
    effect->target.y = targetY;
    effect->target.z = targetZ;
    effect->origin.x = originX;
    effect->origin.y = originY;
    effect->origin.z = originZ;
    effect->speed = speed;
    effect->startY = y;
    effect->targetObj = targetObj;
    effect->unk40 = arg6;
    effect->lingerTimer = 0;
    effect->sound = 0;
    for (i = 0; i < 8; i++) {
        if (gHomingEffects[i] == NULL) {
            gHomingEffects[i] = effect;
            effect->slot = i;
            return effect;
        }
    }
    effect->slot = 0;
    return effect;
}

/* Starts an effect that reaches the target in `frames` frames; returns its handle, or 0. */
s32 effect_startHomingToPoint(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ, s32 arg6, s32 frames) {
    HomingEffect *effect;

    effect = effect_newHomingEffect(x, y, z, targetX, targetY, targetZ, arg6,
                           (f32)math_distance3D(x - targetX, y - targetY, z - targetZ) / (f32)frames,
                           NULL, 0.0f, 0.0f, 0.0f, NULL, -99);
    if (effect != NULL) {
        return effect->slot + 1;
    }
    return 0;
}

/* Starts an effect that homes in on targetObj; returns its handle, or 0. */
s32 effect_startHomingOnActor(s32 x, s32 y, s32 z, Actor *targetObj, s32 arg4, f32 speed) {
    HomingEffect *effect;

    effect = effect_newHomingEffect(x, y, z, 0, 0, 0, arg4, speed, targetObj, 0.0f, 0.0f, 0.0f, NULL, -99);
    if (effect != NULL) {
        return effect->slot + 1;
    }
    return 0;
}

/* Starts an effect at `to` that follows the f32[3] at `follow` for `lifetime` frames. */
void effect_startFollowingHoming(f32 *origin, s32 *to, f32 *follow, s16 lifetime) {
    s32 x;
    s32 y;
    s32 z;

    x = to[0];
    y = to[1];
    z = to[2];
    effect_newHomingEffect(x, y, z, x, y, z, 0x1A, 10.0f, NULL, origin[0], origin[1], origin[2], follow, lifetime);
}

/* Sound callback of the effect started by effect_startHomingWithSound. */
s32 effect_homingSoundCallback(s32 arg0, s32 arg1, void **arg2, s32 arg3) {
    if (*arg2 == NULL) {
        return 1;
    }
    music_postEvent(2, (s32)*arg2);
    return 0;
}

/* Starts an effect with a looping sound (0x2D0) at its origin; returns its handle, or 0. */
s32 effect_startHomingWithSound(s32 originX, s32 originY, s32 originZ, s32 x, s32 y, s32 z, Actor *targetObj, s32 arg7,
                  f32 speed) {
    HomingEffect *effect;

    effect = effect_newHomingEffect(x, y, z, 0, 0, 0, arg7, speed, targetObj, originX, originY, originZ, NULL, -99);
    if (effect != NULL) {
        effect->sound = sound_addDynamicSound(0x2D0, originX, originY, originZ, 0x5DC0, 0x1770, 0x3E8, (SoundRecordCallback)effect_homingSoundCallback,
                                      0, 0, 0, 0);
        return effect->slot + 1;
    }
    return 0;
}

/* Destroys an effect: frees its slot, stops its sound (playing 0x2D7 at the origin) and frees the object. */
void effect_destroyHomingEffect(HomingEffect *effect) {
    s32 slot;
    HomingEffect **entry;
    u16 sound;

    slot = effect->slot;
    if (slot >= 0) {
        entry = &gHomingEffects[slot];
        if (*entry == effect) {
            *entry = NULL;
        }
    }

    sound = effect->sound;
    if (sound != 0) {
        sound_stop(sound);
        effect->sound = 0;
        sound_playAt(0x2D7, 0x5DC0, 0, 0, 0, effect->origin.x, effect->origin.y, effect->origin.z, 0x3E8, 0x1770);
    }

    effect_destroyObject(&effect->header);
}

/* Destroys the effect with the given handle, or every effect for handle 0. */
void effect_stopHomingEffect(s32 handle) {
    s32 list;

    handle--;
    for (list = 0; list != 2; list++) {
        HomingEffect *effect;

        gObjectWalkDepth++;
        effect = (HomingEffect *)gObjectLists[list][0x27];
        while (effect != NULL) {
            gObjectWalkNext[gObjectWalkDepth] = effect->header.next;
            if (handle == -1 || handle == effect->slot) {
                effect_destroyHomingEffect(effect);
            }
            effect = gObjectWalkNext[gObjectWalkDepth];
        }
        gObjectWalkDepth--;
    }
}

/* Reads the position of the effect with the given handle, truncated to integers. */
void effect_getHomingEffectPos(s32 handle, s32 *x, s32 *y, s32 *z) {
    HomingEffect *effect;

    handle--;
    if (handle >= 0 && handle < 8) {
        effect = gHomingEffects[handle];
        if (effect != NULL && effect->header.type == 0x27) {
            *x = effect->pos.x;
            *y = effect->pos.y;
            *z = effect->pos.z;
        }
    }
}

/*
 * Per-frame update of an effect: counts down its lifetime, steps pos toward the target and spawns particles
 * that run from origin to random points along that step, plus sparks at origin.
 */
void effect_updateHomingEffect(HomingEffect *arg0) {
    HomingEffect *effect; // matching: the copy of arg0 sets the frame layout
    Vec3f prevPos;
    Vec3f nextPos;
    Vec3f delta;
    Vec3f step;
    f32 dist;
    f32 move;
    f32 scale;
    s32 spawned;
    Vec3f point;
    Vec3f end;
    s32 randAngle;
    Vec3f from;
    Vec3f dir;
    f32 length;
    s32 sparkAlpha;
    f32 offsetZ;
    f32 offsetX;
    f32 unusedOut;
    f32 randWidth;
    s32 randByte;
    RayHit particle;

    effect = arg0;
    spawned = 0;
    if (effect->lifetime != -99) {
        effect->lifetime -= gFrameTicks;
        if (effect->lifetime < 0) {
            effect_destroyHomingEffect(effect);
        }
    }
    prevPos = effect->pos;
    if (effect->targetObj != NULL) {
        if (effect->targetObj->type == 0) {
            effect_destroyHomingEffect(effect);
            return;
        }
        effect->target.x = effect->targetObj->xpos;
        effect->target.y = effect->targetObj->ypos;
        effect->target.z = effect->targetObj->zpos;
    } else if (effect->follow != NULL) {
        effect->target.x = effect->follow[0];
        effect->target.y = effect->follow[1];
        effect->target.z = effect->follow[2];
    }
    delta.x = effect->target.x - effect->pos.x;
    delta.y = effect->target.y - effect->pos.y;
    delta.z = effect->target.z - effect->pos.z;
    dist = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    move = effect->speed * gFrameTicks;
    if (effect->follow == NULL) {
        if (effect->lingerTimer == 0) {
            if (dist <= move) {
                if (effect->targetObj != NULL) {
                    effect->lingerTimer = 30;
                    effect->targetObj = NULL;
                    effect->target.x += delta.x * 500.0f;
                    effect->target.y += delta.y * 500.0f;
                    effect->target.z += delta.z * 500.0f;
                } else {
                    effect_destroyHomingEffect(effect);
                    return;
                }
            }
        } else {
            effect->lingerTimer -= gFrameTicks;
            if (effect->lingerTimer <= 0) {
                effect_destroyHomingEffect(effect);
                return;
            }
        }
    }
    if (dist != 0.0f) {
        f32 ratio;

        ratio = move / dist;
        step.x = ratio * delta.x;
        step.y = ratio * delta.y;
        step.z = ratio * delta.z;
        nextPos.x = prevPos.x + step.x;
        nextPos.y = prevPos.y + step.y;
        nextPos.z = prevPos.z + step.z;
    } else {
        nextPos = effect->target;
        step = delta;
    }
    effect->pos = nextPos;
    effect->particleAccum += (526.0f + math_randomFloat() * 1228.0f) * 0.001f * gFrameDelta;
    while (effect->particleAccum > 1.0f) {
        from = effect->origin;
        {
            f32 rnd;

            rnd = math_randomFloat();
            randAngle = math_random();
            math_dirFromBinAngleXZ(randAngle & 0xFF, math_randomFloat() * 129.0f, &offsetX, &offsetZ);
            point.x = rnd * step.x + prevPos.x + offsetX;
            point.y = rnd * step.y + prevPos.y + 200.0f;
            point.z = rnd * step.z + prevPos.z + offsetZ;
        }
        end = point;
        if (collision_probeFloorSurfABSet0Cached(&point, 0, point.y - 1000.0f, &effect->ground) != 0) {
            end.y = effect->ground.height;
        }
        dir.x = end.x - from.x;
        dir.y = end.y - from.y;
        dir.z = end.z - from.z;
        {
            f32 t;

            if (math_vec3Normalize(&dir, &dir, &length, &unusedOut) != 0) {
                t = math_randomFloat() * gFrameDelta;
                scale = 190.0f * t;
                from.x += dir.x * scale;
                from.y += dir.y * scale;
                from.z += dir.z * scale;
                length -= scale;
            }
        }
        particle.obj = NULL;
        particle.dist = length;
        particle.pos = end;
        particle.centre = end;
        particle.far = end;
        particle.origin = from;
        particle.dir = dir;
        particle.tri = effect->ground.tri;
        particle.bone = 0;
        particle.part = 0;
        particle.hitType = 1;
        particle.index = 0;
        if (effect->ground.source == 2 || effect->ground.source == 3) {
            particle.surface = effect->ground.surface;
        } else {
            particle.surface = NULL;
        }
        particle.material = effect->ground.material;
        randWidth = math_randomFloat();
        scale = math_randomFloat(); // matching: scale is reused so it keeps its register
        randByte = math_random();
        if (spawned != 0 || effect->follow != NULL) {
            randAngle = 2;
        } else {
            randAngle = 1;
        }
        if (weapon_spawnProjectile(&from, NULL, 0, 0, 0, &particle, 190.0f, 0.00526315812f /* 1 / 190 */, randWidth * 50.0f + 10.0f,
                          scale * 191.0f + 160.0f, 80.0f, (randByte % 176U) + 80, 0, 0, 0, 0, 0, 0, 1, randAngle,
                          effect->unk40, 0.0f, 0xFF, -1, 0, effect->header.owner, effect->header.kind) != 0) {
            spawned = 1;
        }
        effect->particleAccum -= 1.0f;
    }
    if (effect->follow == NULL) {
        effect->sparkAccum += (56.0f + math_randomFloat() * 235.0f) * 0.001f * gFrameDelta;
        while (effect->sparkAccum > 1.0f) {
            randByte = math_random();
            sparkAlpha = math_random();
            effect_spawnFlashParticle((randByte & 3) + 6, (sparkAlpha % 156U) + 100,
                          math_randomFloat() * 1548.0f + 1093.0f, &effect->origin, 0xFF, effect->header.owner, effect->header.kind);
            effect->sparkAccum -= 1.0f;
        }
    }
}

/* The part of an object that func_150E4010 reads, at +0x110 of it. */
typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ f32 x;
    /* 0x34 */ f32 y;
    /* 0x38 */ f32 z;
    /* 0x3C */ u8 pad3C[0x24];
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ u8 pad68[0x3C];
    /* 0xA4 */ s32 unkA4;
} Obj4010State;

struct Obj4010 {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ Obj4010State state;
};

/* If unkA4 is set, calls actor_runAttackAt at the state position with the heading of (unk60, unk64) as a binary angle. */
void func_150E4010(Obj4010 *obj) {
    Obj4010State *state;

    state = &obj->state;
    if (state->unkA4 != 0) {
        actor_runAttackAt(state->x, state->y, state->z, state->unkA4 | 0x60000, -1,
                      ((u16)(math_atan2(state->unk60, state->unk64) * 10430.378f /* 0x10000 / 2pi */) - 0x4000) | 1, 0, 0);
    }
}

void func_150E411C(Game1EF500Trail *trail) {
    weapon_spawnHitEffects(trail, 0.352000028f, 0.701000035f, 0.566000044f, 0.228000015f, 0xFF, 0xFF, 0xFF);
}

struct Obj4174 {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ u8 pad34[0x18];
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 pad50[4];
    /* 0x54 */ f32 unk54;
};

/* Advances unk2C and unk30 by their per-frame rates unk4C and unk54. */
s32 func_150E4174(Obj4174 *obj) {
    obj->unk2C += obj->unk4C * gFrameDelta;
    obj->unk30 += obj->unk54 * gFrameDelta;
    return 1;
}
