#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Local point of the gun's muzzle in its attachment's space. */
const Vec3f D_800A2050 = { 9.574001f, -124.071396f, 91.0238f };

/* Muzzle-relative points in the gun's attachment space. */
const Vec3f D_800A205C = { 9.679999f, -7.0199966f, 24.842f };
const Vec3f D_800A2068 = { 1.4242005f, -6.4357996f, 24.477999f };
const Vec3f D_800A2074 = { -14.875598f, -6.4357996f, 24.477999f };
const Vec3f D_800A2080[2][6] = { /* two random sets of flash points */
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
void func_150FED30(Actor *obj, s32 arg1, s32 arg2) {
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

    gun = func_150FF288(obj);
    if (gun != NULL) {
        func_151D5148(obj);
        func_150FF2AC(obj, &direction, &axis0, &axis1);

        if ((obj->ob_matrix_ptr != NULL) && ((obj->unk74 & 0xF) != 0xF)) {
            valid = TRUE;
        } else {
            valid = FALSE;
        }

        func_150FF2D4(&valid, points, &position, &hit, &normal, &direction, &axis0, &axis1, &output0, &output1,
                      &value0, &value1, &endpoint, obj, gun);

        random0 = func_150ADA68();
        random1 = func_150ADA68();
        randomWord = func_150ADA20();

        if (D_800BE616 != 0) {
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
        func_151C229C(&endpoint, &direction, value0, value1, 1, 0, 300.0f, 0.003333333414f,
                      (random0 * 10.0f) + 25.0f, (random1 * 200.0f) + 600.0f,
                      50.0f, (randomWord % 156U) + 100, obj, 99, 1, 1, 1,
                      0, 1, 0, kind, 0.0f, 0xFF, directionSign, 0,
                      ((u8 *)&arg1)[3], arg2);

        if (valid) {
            func_151D3F14(&position, ((u8 *)&arg1)[3], arg2);
            /* byte 2 of an attachment is the owner bone it follows */
            func_151D4408(&hit, &normal, obj->ob_matrix_ptr[gun->bone], obj, 1.0f,
                          ((u8 *)&arg1)[3], arg2);
            func_150FF474(&position, points, ((u8 *)&arg1)[3], arg2);
        }
    }
}

/* Writes the world position of attachment set `setId` of `obj` to `out` (`obj`'s own position for -1). */
void func_150FEFD0(Actor *obj, s16 setId, Vec3f *out) {
    const Vec3f *src;
    Vec3f *dst;
    ModelAttachment *attachment;

    if (setId == -1) {
        out->x = obj->xpos;
        out->y = obj->ypos;
        out->z = obj->zpos;
    } else {
        attachment = func_1503195C(obj, setId, 0);
        if (attachment == NULL) {
            out->x = obj->xpos;
            out->y = obj->ypos;
            out->z = obj->zpos;
        } else {
            src = &D_800A2050;
            dst = out;
            func_1514654C(obj, attachment, 0, &src, &dst, 1);
        }
    }
}

/* Like func_150FED30, but spawns its shot through func_150F7470. */
void func_150FF084(Actor *obj, u8 arg1, s32 arg2) {
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

    gun = func_150FF288(obj);
    if (gun != NULL) {
        func_151D5148(obj);
        func_150FF2AC(obj, &direction, &axis0, &axis1);

        if (obj->ob_matrix_ptr != NULL) {
            valid = TRUE;
        } else {
            valid = FALSE;
        }

        func_150FF2D4(&valid, points, &position, &hit, &normal, &direction, &axis0, &axis1, &output0, &output1,
                      &value0, &value1, &endpoint, obj, gun);

        if (value1 == NULL) {
            hasValue1 = FALSE;
        } else {
            hasValue1 = TRUE;
        }

        randomWord = func_150ADA20();
        func_150F7470(&position, &direction, value0, value1, hasValue1, NULL, 54.00000381f, 0.01851851679f, 300.0f,
                      obj, 1, 1, 1, 0, 0x1A, 1, (randomWord % 5U) + 807, 0, arg1, arg2);

        if (valid) {
            func_151D3F14(&position, arg1, arg2);
            func_151D4408(&hit, &normal, obj->ob_matrix_ptr[gun->bone], obj, 1.0f,
                          arg1, arg2);
            func_150FF474(&position, points, arg1, arg2);
        }
    }
}

/* Returns `obj`'s gun attachment (set 0x82), or NULL. */
ModelAttachment *func_150FF288(Actor *obj) {
    return func_1503195C(obj, 0x82, 0);
}

/* Fills `direction`, `axis0` and `axis1` for `obj` through func_15145740. */
void func_150FF2AC(Actor *obj, Vec3f *direction, Vec3f *axis0, Vec3f *axis1) {
    func_15145740(obj, direction, axis0, axis1, 0.8110000491f);
}

/*
 * Finds where a shot starts. With `*valid` set, the points come from the gun's bone (func_150FF6E0),
 * which can fail and clear `*valid`; otherwise the start is beside `obj`, to the right of `direction`.
 */
void func_150FF2D4(u8 *valid, Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal,
                   Vec3f *direction, Vec3f *axis0, Vec3f *axis1, Vec3f *output0, Vec3f *output1,
                   Vec3f **value0, Vec3f **value1, Vec3f *endpoint, Actor *obj, ModelAttachment *gun) {
    if (*valid) {
        *valid = func_150FF6E0(points, position, hit, normal, endpoint, obj, gun);
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
    func_151D5174(obj, position, direction, axis0, axis1, output0, output1, value0, value1, endpoint);
}

/* Spawns six muzzle-flash particles at `position`, one per entry of `points`. */
void func_150FF474(Vec3f *position, Vec3f *points, u8 arg2, s32 arg3) {
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
    spawn.unk4 = (func_150ADA20() & 3) + 3;
    for (i = 0; i < 6; i++) {
        spawn.unk14 = (func_150ADA68() * 11.10000038f) + 5.0f;
        spawn.unk18 = (func_150ADA68() * 0.9990000725f) + 2.48300004f;
        spawn.unk28 = points[i];
        spawn.unk44 = (func_150ADA20() % 156U) + 100;
        variant = (func_150ADA20() & 1) ? 2 : 0;
        func_1513D2F0(&spawn, &D_800A4AA0, 0, 0, 0, 0x1B, variant + 1, 0, 0, 0, arg2, arg3);
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
u8 func_150FF6E0(Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal, Vec3f *endpoint,
                 Actor *obj, ModelAttachment *gun) {
    u8 i;
    const Vec3f *src[10];
    Vec3f *dst[10];
    u8 set;

    set = func_150ADA20() & 1;
    src[0] = &D_800A2050;
    src[1] = &D_800A2068;
    src[2] = &D_800A2074;
    src[3] = &D_800A205C;
    for (i = 0; i < 6; i++) {
        src[i + 4] = &D_800A2080[set][i];
        dst[i + 4] = &points[i];
    }
    dst[0] = position;
    dst[1] = hit;
    dst[2] = normal;
    dst[3] = endpoint;
    if (func_1514654C(obj, gun, 0, src, dst, 10)) {
        normal->x -= hit->x;
        normal->y -= hit->y;
        normal->z -= hit->z;
    } else {
        return FALSE;
    }
    return TRUE;
}
