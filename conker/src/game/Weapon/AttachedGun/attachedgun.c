#include <ultra64.h>
#include <math.h>
#include "conker/attach.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/weapon.h"

/* Local point of the gun's muzzle in its attachment's space. */
const Vec3f gAttachedGunMuzzle = { 9.574001f, -124.071396f, 91.0238f };

/* Muzzle-relative points in the gun's attachment space. */
const Vec3f D_800A205C = { 9.679999f, -7.0199966f, 24.842f };
const Vec3f D_800A2068 = { 1.4242005f, -6.4357996f, 24.477999f };
const Vec3f D_800A2074 = { -14.875598f, -6.4357996f, 24.477999f };
const Vec3f gAttachedGunFlashPoints[2][6] = { /* two random sets of flash points */
    {
        { 18.3144f, -117.26579f, 118.493004f },
        { 37.2402f, -128.4044f, 100.6656f },
        { 28.5f, -142.65921f, 77.855f },
        { 0.83379984f, -145.7742f, 72.8662f },
        { -18.092197f, -134.63681f, 90.691605f },
        { -9.351802f, -120.382f, 113.504f },
    },
    {
        { 26.1992f, -119.3948f, 115.084f },
        { 37.7034f, -133.0874f, 93.1714f },
        { 21.0788f, -145.2132f, 73.766f },
        { -7.0510006f, -143.6464f, 76.2732f },
        { -18.555199f, -129.954f, 98.186005f },
        { -1.9301987f, -117.827995f, 117.5914f },
    },
};

/* Fires a shot from the gun attachment (set 0x82) of `obj`, with muzzle effects when it has bones. */
void weapon_fireAttachedGun(Actor *obj, s32 arg1, s32 arg2) {
    Vec3f position;
    Vec3f endpoint;
    Vec3f direction;
    Vec3f axis0;
    Vec3f axis1;
    Vec3f hit;
    Vec3f normal;
    Vec3f output0;
    Vec3f output1;
    Vec3f points[6];
    Vec3f *value0;
    Vec3f *value1;
    ModelAttachment *gun;
    u8 valid;
    s32 randomWord;
    s32 kind;
    s32 directionSign;
    f32 random1;
    f32 random0;

    gun = weapon_getGunAttachment(obj);
    if (gun != NULL) {
        particle_countShot(obj);
        weapon_getShotAxes(obj, &direction, &axis0, &axis1);

        if ((obj->ob_matrix_ptr != NULL) && ((obj->cullMask & 0xF) != 0xF)) {
            valid = TRUE;
        } else {
            valid = FALSE;
        }

        weapon_findShotStart(&valid, points, &position, &hit, &normal, &direction, &axis0, &axis1, &output0, &output1,
                      &value0, &value1, &endpoint, obj, gun);

        random0 = math_randomFloat();
        random1 = math_randomFloat();
        randomWord = math_random();

        if (gIsMultiplayer != 0) {
            kind = 0x35;
        } else {
            kind = 0x1A;
        }

        if (obj->character_type == 0x98) {
            directionSign = 1;
        } else {
            directionSign = -1;
        }

        // matching: lbu of arg1 from its arg slot at each use (a u8 param gets spilled to a new slot)
        weapon_spawnProjectile(&endpoint, &direction, value0, value1, 1, 0, 300.0f, 0.003333333414f,
                      (random0 * 10.0f) + 25.0f, (random1 * 200.0f) + 600.0f,
                      50.0f, (randomWord % 156U) + 100, obj, 99, 1, 1, 1,
                      0, 1, 0, kind, 0.0f, 0xFF, directionSign, 0,
                      ((u8 *)&arg1)[3], arg2);

        if (valid) {
            particle_spawnShortLightFlash(&position, ((u8 *)&arg1)[3], arg2);
            /* byte 2 of an attachment is the owner bone it follows */
            particle_spawnBoneDirParticle(&hit, &normal, obj->ob_matrix_ptr[gun->bone], obj, 1.0f,
                          ((u8 *)&arg1)[3], arg2);
            weapon_spawnMuzzleFlashes(&position, points, ((u8 *)&arg1)[3], arg2);
        }
    }
}

/* Writes the world position of attachment set `setId` of `obj` to `out` (`obj`'s own position for -1). */
void weapon_getAttachmentPos(Actor *obj, s16 setId, Vec3f *out) {
    const Vec3f *src;
    Vec3f *dst;
    ModelAttachment *attachment;

    if (setId == -1) {
        out->x = obj->xpos;
        out->y = obj->ypos;
        out->z = obj->zpos;
    } else {
        attachment = attach_find(obj, setId, 0);
        if (attachment == NULL) {
            out->x = obj->xpos;
            out->y = obj->ypos;
            out->z = obj->zpos;
        } else {
            src = &gAttachedGunMuzzle;
            dst = out;
            math_transformAttachmentOffsets(obj, attachment, 0, &src, &dst, 1);
        }
    }
}

/* Like weapon_fireAttachedGun, but spawns its shot through effect_spawnProjectile. */
void weapon_fireAttachedGunProjectile(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f position;
    Vec3f endpoint;
    Vec3f direction;
    Vec3f axis0;
    Vec3f axis1;
    Vec3f hit;
    Vec3f normal;
    Vec3f output0;
    Vec3f output1;
    Vec3f points[6];
    Vec3f *value0;
    Vec3f *value1;
    ModelAttachment *gun;
    u8 valid;
    s32 randomWord;
    s32 hasValue1;

    gun = weapon_getGunAttachment(obj);
    if (gun != NULL) {
        particle_countShot(obj);
        weapon_getShotAxes(obj, &direction, &axis0, &axis1);

        if (obj->ob_matrix_ptr != NULL) {
            valid = TRUE;
        } else {
            valid = FALSE;
        }

        weapon_findShotStart(&valid, points, &position, &hit, &normal, &direction, &axis0, &axis1, &output0, &output1,
                      &value0, &value1, &endpoint, obj, gun);

        if (value1 == NULL) {
            hasValue1 = FALSE;
        } else {
            hasValue1 = TRUE;
        }

        randomWord = math_random();
        effect_spawnProjectile(&position, &direction, value0, value1, hasValue1, NULL, 54.00000381f, 0.01851851679f, 300.0f,
                      obj, 1, 1, 1, 0, 0x1A, 1, (randomWord % 5U) + 807, 0, arg1, arg2);

        if (valid) {
            particle_spawnShortLightFlash(&position, arg1, arg2);
            particle_spawnBoneDirParticle(&hit, &normal, obj->ob_matrix_ptr[gun->bone], obj, 1.0f,
                          arg1, arg2);
            weapon_spawnMuzzleFlashes(&position, points, arg1, arg2);
        }
    }
}

/* Returns `obj`'s gun attachment (set 0x82), or NULL. */
ModelAttachment *weapon_getGunAttachment(Actor *obj) {
    return attach_find(obj, 0x82, 0);
}

/* Fills `direction`, `axis0` and `axis1` for `obj` through math_getActorAim. */
void weapon_getShotAxes(Actor *obj, Vec3f *direction, Vec3f *axis0, Vec3f *axis1) {
    math_getActorAim(obj, direction, axis0, axis1, 0.8110000491f);
}

/*
 * Finds where a shot starts. With `*valid` set, the points come from the gun's bone (weapon_getGunMuzzlePoints),
 * which can fail and clear `*valid`; otherwise the start is beside `obj`, to the right of `direction`.
 */
void weapon_findShotStart(u8 *valid, Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal,
                   Vec3f *direction, Vec3f *axis0, Vec3f *axis1, Vec3f *output0, Vec3f *output1,
                   Vec3f **value0, Vec3f **value1, Vec3f *endpoint, Actor *obj, ModelAttachment *gun) {
    if (*valid) {
        *valid = weapon_getGunMuzzlePoints(points, position, hit, normal, endpoint, obj, gun);
    }
    if (!*valid) {
        f32 sideZ;
        f32 sideX;
        f32 dirX;

        dirX = direction->x;
        if ((0.0000999999974738f < fabsf(dirX)) || (0.0000999999974738f < fabsf(direction->z))) {
            f32 dirZ;
            f32 invLength;

            dirZ = direction->z;
            invLength = 1.0f / sqrtf((dirX * dirX) + (dirZ * dirZ));
            sideZ = dirZ * invLength;
            sideX = -dirX * invLength;
        } else {
            sideZ = 1.0f;
            sideX = 0.0f;
        }
        position->x = obj->xpos + (34.0f * sideX);
        position->y = obj->ypos + 49.0f;
        position->z = obj->zpos + (34.0f * sideZ);
        *endpoint = *position;
    }
    particle_intersectAimPlane(obj, position, direction, axis0, axis1, output0, output1, value0, value1, endpoint);
}

/* Spawns six muzzle-flash particles at `position`, one per entry of `points`. */
void weapon_spawnMuzzleFlashes(Vec3f *position, Vec3f *points, u8 arg2, s32 arg3) {
    u8 i;
    s32 variant;
    s32 pad; // matching: unused stack slot
    struct {
        u8 unk0;
        u8 unk1;
        s16 unk2;
        s16 unk4;
        u8 pad6[2];
        s32 unk8;
        s32 unkC;
        u8 color[4];
        f32 unk14;
        f32 unk18;
        Vec3f position;
        Vec3f unk28;
        f32 unk34;
        f32 unk38;
        f32 unk3C;
        s32 flags;
        u8 unk44;
        u8 unk45;
        u8 unk46;
        u8 unk47;
        s32 unk48;
        u8 unk4C;
    } spawn;

    spawn.unk0 = 0x5F;
    spawn.unk1 = 5;
    spawn.unk2 = 0x2203;
    spawn.unk8 = 0;
    spawn.unkC = 0;
    spawn.color[0] = 0xFF;
    spawn.color[1] = 0xFF;
    spawn.color[2] = 0xFF;
    spawn.color[3] = 0xFF;
    spawn.position = *position;
    spawn.unk34 = 0.0f;
    spawn.unk38 = 0.0f;
    spawn.unk3C = 0.0f;
    spawn.flags = 0x40CC0009;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 7;
    spawn.unk4C = 0xFF;
    spawn.unk48 = 0;
    spawn.unk4 = (math_random() & 3) + 3;
    for (i = 0; i < 6; i++) {
        spawn.unk14 = (math_randomFloat() * 11.10000038f) + 5.0f;
        spawn.unk18 = (math_randomFloat() * 0.9990000725f) + 2.48300004f;
        spawn.unk28 = points[i];
        spawn.unk44 = (math_random() % 156U) + 100;
        variant = (math_random() & 1) ? 2 : 0;
        particle_newQuadParticle(&spawn, &gWhiteVtxColors, 0, 0, 0, 0x1B, variant + 1, 0, 0, 0, arg2, arg3);
    }
}

/* Returns FALSE for object id 0x98, TRUE otherwise. */
s32 func_150FF6B4(Actor *obj, s32 arg1, s32 arg2) {
    if (obj->character_type == 0x98) {
        return FALSE;
    }
    return TRUE;
}

/*
 * Transforms the gun's muzzle points by its bone into world space. `normal` ends up relative to `hit`.
 * Returns FALSE if the transform fails.
 */
u8 weapon_getGunMuzzlePoints(Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal, Vec3f *endpoint,
                 Actor *obj, ModelAttachment *gun) {
    u8 i;
    const Vec3f *src[10];
    Vec3f *dst[10];
    u8 set;

    set = math_random() & 1;
    src[0] = &gAttachedGunMuzzle;
    src[1] = &D_800A2068;
    src[2] = &D_800A2074;
    src[3] = &D_800A205C;
    for (i = 0; i < 6; i++) {
        src[i + 4] = &gAttachedGunFlashPoints[set][i];
        dst[i + 4] = &points[i];
    }
    dst[0] = position;
    dst[1] = hit;
    dst[2] = normal;
    dst[3] = endpoint;
    if (math_transformAttachmentOffsets(obj, gun, 0, src, dst, 10)) {
        normal->x -= hit->x;
        normal->y -= hit->y;
        normal->z -= hit->z;
    } else {
        return FALSE;
    }
    return TRUE;
}
