#include <ultra64.h>
#include <math.h>
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/view.h"
#include "conker/weapon.h"

/* Per-side (0, 1) tables: byte offset of the side's joint matrix in Actor.ob_matrix_ptr, and model-space points
 * that math_transformOffsets transforms by it. */
static const s32 gJointShotMatrixOffsets[2] = { 0x240, 0x100 };
static const Vec3f gJointShotMuzzles[2] = { { 3.0f, -117.0f, 27.0f }, { -4.0f, -117.0f, 27.0f } };
static const Vec3f gJointShotBeamEnds[2] = { { 3.0f, -200.0f, 27.0f }, { -4.0f, -200.0f, 27.0f } };
static const Vec3f D_800AAB38[2] = { { -6.0f, -38.0f, 32.0f }, { 6.0f, -38.0f, 32.0f } };
static const Vec3f D_800AAB50[2] = { { -31.0f, -38.0f, 56.0f }, { 31.0f, -38.0f, 57.0f } };
static const Vec3f D_800AAB68[2] = { { 3.0f, 11.0f, 27.0f }, { -4.0f, 11.0f, 27.0f } };
/* not referenced: five more point pairs */
static const Vec3f D_800AAB80[5][2] = {
    { { 37.780003f, -50.954f, 33.052002f }, { -37.780003f, -50.954f, 33.052002f } },
    { { 37.780003f, -80.0f, 33.052002f }, { -37.780003f, -80.0f, 33.052002f } },
    { { 45.1012f, 20.005f, 36.368f }, { -45.1012f, 20.005f, 36.368f } },
    { { 85.101204f, 20.005f, 36.368f }, { -85.101204f, 20.005f, 36.368f } },
    { { 37.780003f, 44.479797f, 33.052002f }, { -37.780003f, 44.479797f, 33.052002f } },
};

/* Fires this side's shot from its joint points (or, without a skeleton, from beside the object): posts
 * a cont_spawnRumble event for its camera, side effects, then weapon_spawnProjectile along `aim` (from the joints when jointAim). */
Game1EF500Trail *weapon_fireJointShot(Actor *obj, u8 side, u8 jointAim, ModelAttachment *attachment, s16 node, u8 fxOwner,
                               s32 fxList) {
    Vec3f origin;
    Vec3f aim;
    Vec3f shotPos;
    Vec3f pointBuf;
    Vec3f dirBuf;
    Vec3f axis0;
    Vec3f start;
    Vec3f dir;
    void *matrix;
    MtxF *joints;
    Vec3f *altOrigin;
    Vec3f *altDir;
    Vec3f axis1;

    if (obj == NULL) {
        return NULL;
    }
    if (side >= 2) {
        return NULL;
    }
    if (func_15142A5C(obj) != 0) {
        return NULL;
    }
    particle_countShot(obj);
    if (!jointAim) {
        math_getActorAim(obj, &aim, &axis0, &axis1, 0.8110000491f);
    }
    joints = obj->ob_matrix_ptr;
    if (joints != NULL) {
        const Vec3f *points[5];
        Vec3f *results[5];


        matrix = (u8 *)joints + gJointShotMatrixOffsets[side];
        results[0] = &origin;
        results[1] = &start;
        results[2] = &dir;
        results[3] = &shotPos;
        results[4] = &aim;
        points[0] = &gJointShotMuzzles[side];
        points[1] = &D_800AAB38[side];
        points[2] = &D_800AAB50[side];
        points[3] = &D_800AAB68[side];
        points[4] = &gJointShotBeamEnds[side];
        if (attachment != NULL) {
            if (math_transformAttachmentOffsets(obj, attachment, 0, points, results, jointAim ? 5 : 4) == 0) {
                return NULL;
            }
        } else {
            math_transformOffsets(points, results, matrix, jointAim ? 5 : 4);
        }
        dir.x -= start.x;
        dir.y -= start.y;
        dir.z -= start.z;
        if (jointAim) {
            aim.x -= origin.x;
            aim.y -= origin.y;
            aim.z -= origin.z;
        }
    } else {
        f32 dz;
        f32 dx;
        f32 offset;

        if (jointAim) {
            return NULL;
        }
        /* No skeleton: start 34 units to the side of the aim direction (in the xz plane), 49 units up. */
        if ((fabsf(aim.x) > 9.999999747e-05f) || (fabsf(aim.z) > 9.999999747e-05f)) {
            f32 invLen;

            /* matching: a block-scope declaration; its -g3 scope steers the schedule */
            f32 sqrtf(f32);

            dx = aim.x;
            dz = aim.z;
            invLen = 1.0f / sqrtf((dx * dx) + (dz * dz));
            dz = dz * invLen;
            dx = -dx * invLen;
        } else {
            dz = 1.0f;
            dx = 0.0f;
        }
        offset = side ? 34.0f : -34.0f;
        origin.x = obj->xpos + (offset * dx);
        origin.y = obj->ypos + 49.0f;
        origin.z = obj->zpos + (offset * dz);
        shotPos = origin;
    }
    particle_intersectAimPlane(obj, &origin, &aim, &axis0, &axis1, &pointBuf, &dirBuf, &altOrigin, &altDir, &shotPos);
    if (obj->CameraPtr != NULL) {
        RumbleParams event;

        event.flags = 1;
        event.duration = (math_random() % 11U) + 11;
        event.playerMask = 1 << obj->CameraPtr->CNumber;
        math_random();
        event.strength = 8;
        event.updateKind = -1;
        cont_spawnRumble(&event, 0, 0xFF, 1);
    }
    if ((obj->ob_matrix_ptr != NULL) && ((obj->cullMask & 0xF) != 0xF)) {
        s32 mask;
        u32 i;

        if (jointAim || ((gScreenLayout < 2) && (math_random() & 1))) {
            particle_spawnShortLightFlash(&origin, fxOwner, fxList);
        }
        if ((jointAim || (gScreenLayout < 2)) && (math_random() & 1)) {
            particle_spawnBoneDirParticle(&start, &dir, matrix, obj, 1.0f, fxOwner, fxList);
        }
        mask = 1;
        for (i = gLayoutDetailShift; i > 0; i--) {
            mask |= 1 << i;
        }
        if (jointAim || !(math_random() & mask)) {
            weapon_spawnJointBeam(obj, side, &origin, node, fxOwner, fxList);
        }
    }
    return weapon_spawnProjectile(&shotPos, &aim, altOrigin, altDir, 1, 0, 300.0f, 0.003333333414f,
                         math_randomFloat() * 10.0f + 25.0f, math_randomFloat() * 150.0f + 400.0f, 50.0f,
                         (math_random() % 156U) + 100, obj, jointAim ? 0 : 1, jointAim ? 0 : 1, jointAim ? 0 : 1, 8, jointAim ? 0 : 1, 1, 0, 0x1A, 0.0f, 0xFF,
                         -1, 0, fxOwner, fxList);
}

/* Handler-table entry: the side-0 point of weapon_getJointPos. */
void weapon_getSide0JointPos(Actor *obj, s16 unused, Vec3f *out) {
    weapon_getJointPos(obj, out, 0);
}

/* Handler-table entry: the side-1 point of weapon_getJointPos. */
void weapon_getSide1JointPos(Actor *obj, s16 unused, Vec3f *out) {
    weapon_getJointPos(obj, out, 1);
}

/* Transforms this side's gJointShotMuzzles point by its joint matrix (gJointShotMatrixOffsets bytes into obj->ob_matrix_ptr) into *out. */
void weapon_getJointPos(Actor *obj, Vec3f *out, u8 side) {
    void *matrix = (u8 *)obj->ob_matrix_ptr + gJointShotMatrixOffsets[side];

    math_transformOffset(&gJointShotMuzzles[side], out, matrix);
}

/* The object whose shot description weapon_fireJointShotProjectile copies. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ RayHit shot; /* the ray of the last shot */
} ShotHolder;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ShotHolder holder;
    /* 0x9C */ u8 flags;
} ShotSource;

/* Emits this side's effects (particle_spawnShortLightFlash, particle_spawnBoneDirParticle, weapon_spawnJointBeam) from its joint points, then fires a
 * copy of the shot described by the owner's 0x1B entry, its end pushed 10% beyond its start. */
void weapon_fireJointShotProjectile(Actor *obj, u8 side, u8 fxOwner, s32 fxList) {
    Vec3f origin;
    Vec3f start;
    Vec3f dir;
    void *matrix;

    if ((obj != NULL) && (side < 2) && (func_15142A5C(obj) == 0)) {
        particle_countShot(obj);
        if (obj->ob_matrix_ptr != NULL) {
            {
                const Vec3f *points[3];
                Vec3f *results[3];

                matrix = (u8 *)obj->ob_matrix_ptr + gJointShotMatrixOffsets[side];
                points[0] = &gJointShotMuzzles[side];
                points[1] = &D_800AAB38[side];
                points[2] = &D_800AAB50[side];
                results[0] = &origin;
                results[1] = &start;
                results[2] = &dir;
                math_transformOffsets(points, results, matrix, 3);
                // matching: six dead ifs (fewer fail) change uopt's register choice: obj stays on the stack
                if (0) {} if (0) {} if (0) {} if (0) {} if (0) {} if (0) {}
                dir.x -= start.x;
                dir.y -= start.y;
                dir.z -= start.z;
            }
            particle_spawnShortLightFlash(&origin, fxOwner, fxList);
            particle_spawnBoneDirParticle(&start, &dir, matrix, obj, 1.0f, fxOwner, fxList);
            weapon_spawnJointBeam(obj, side, &origin, -1, fxOwner, fxList);
            {
                ShotSource *source;
                Vec3f end;
                Attachment *entry;
                ShotHolder *holder;
                RayHit *src;

                if (effect_findAttachmentByType(obj->attachments, 0x1B, &entry) != 0) {
                    source = entry->handle;
                    holder = &source->holder;
                    if (source->flags & 1) {
                        src = &holder->shot;
                        if (src->hitType != 0) {
                            if (src->hitType >= 2) {
                                f32 dx;
                                f32 dy;
                                f32 dz;

                                dx = src->pos.x - origin.x;
                                dy = src->pos.y - origin.y;
                                dz = src->pos.z - origin.z;
                                end.x = (dx * 1.100000024f) + origin.x;
                                end.y = (dy * 1.100000024f) + origin.y;
                                end.z = (dz * 1.100000024f) + origin.z;
                            } else {
                                end = src->pos;
                            }
                            {
                                RayHit shot;

                                shot = *src;
                                shot.origin = end;
                                effect_spawnProjectile(&origin, NULL, NULL, NULL, 0, &shot, 54.00000381f, 0.01851851679f, 500.0f, obj, 0,
                                              (src->hitType >= 2) ? 0 : 1, 1, 0, 0x1A, -1, 0, 1, fxOwner, fxList);
                            }
                        }
                    }
                }
            }
        }
    }
}

/* Spawns a effect_spawnBoneBeamParticle effect at `pos` between this side's gJointShotMuzzles/gJointShotBeamEnds points, with random
 * size, count and angle. */
void weapon_spawnJointBeam(Actor *obj, u8 side, Vec3f *pos, s16 node, u8 fxOwner, s32 fxList) {
    ParticleSize values;
    f32 angle;
    u32 random1;
    u32 random0;

    values.height = (math_randomFloat() * 1.001f) + 0.6040000319f;
    values.width = (math_randomFloat() * 5.9f) + 14.1f;
    random0 = math_random();
    random1 = math_random();
    angle = math_randomFloat();
    effect_spawnBoneBeamParticle(obj, (u32)gJointShotMatrixOffsets[side] >> 6, &gJointShotMuzzles[side], &gJointShotBeamEnds[side], &values,
                  (random0 % 4U) + 6, (random1 % 1U) + 255, (angle * 270.0f) + 359.0f, pos, 255, 0, node, fxOwner,
                  fxList);
}

/* Spawns model particle 15 on `owner` when it starts the hover (anim 0xE9, actor_updatePlayerControls); its update
 * weapon_updateHoverModel keeps it while the hover lasts. */
void weapon_spawnHoverModel(Actor *owner, u8 fxOwner, s32 fxList) {
    ParticleSpawn spawn;
    struct {
        u8 bone; /* index into the owner's ob_matrix_ptr (matrix callback 5, particle_buildOwnerMatrix) */
    } state;
    const Vec3f *origin = &gZeroVec3f;
    ModelParticle *effect;

    state.bone = 0;
    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    spawn.sizeY = 1.0f;
    spawn.sizeXZ = 1.0f;
    spawn.rot = *origin;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = *origin;
    spawn.vel = *origin;
    spawn.rotVel = *origin;
    spawn.flags = 0x11900;
    spawn.lifetime = 300;
    spawn.model = 15;
    spawn.variant = 0;
    spawn.zone = 0;
    spawn.alpha = 100;
    spawn.updateFn = 23;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 5;
    spawn.drawFn = -1;
    spawn.lightMode = 1;
    spawn.gravity = 0.0f;
    spawn.owner = owner;
    spawn.ownerId = owner->unique_id;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.sound = 0;

    effect = particle_newModelParticleWithFloor(&spawn, 3, 0xFF, 0, 1, fxOwner, fxList);
    if (effect != NULL) {
        memcpy(MODEL_PARTICLE_DATA(effect), &state, sizeof(state));
    }
}

/* Model particle update 23: returns 0 (the particle dies) once the owner's anim is no longer 0xE9 (the hover); otherwise
 * mirrors func_15142A5C(owner) in flag 0x20000 and sets the alpha to the owner's animation speed scaled to 0-255. */
s32 weapon_updateHoverModel(ModelParticle *particle) {
    Actor *owner;
    f32 speed;

    owner = particle->owner;
    if (owner->animseq.uh != 0xE9) {
        return 0;
    }
    if (func_15142A5C(owner) != 0) {
        particle->flags |= 0x20000;
    } else {
        particle->flags &= ~0x20000;
    }
    speed = owner->animsp * 35.0f;
    if (speed > 255.0f) {
        speed = 255; // matching: 255.0f schedules the clamp differently
    } else if (speed < 0.0f) {
        speed = 0.0f;
    }
    {
        // matching: the -g3 block scope keeps the float->u8 conversion after the clamp
        u8 clamped = speed;

        particle->alpha = clamped;
    }
    return 1;
}
