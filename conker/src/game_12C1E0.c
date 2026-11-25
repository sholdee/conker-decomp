#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Local point of the gun's muzzle in its attachment's space. */
extern Vec3f D_800A2050;

ActorCtl *func_150FF288(Actor *obj);
/* Fills `direction`, `axis0` and `axis1` for `obj` through func_15145740. */
void func_150FF2AC(Actor *obj, Vec3f *direction, Vec3f *axis0, Vec3f *axis1);
void func_150FF2D4(u8 *valid, Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal,
                   Vec3f *direction, Vec3f *axis0, Vec3f *axis1, Vec3f *output0, Vec3f *output1,
                   s32 *value0, s32 *value1, Vec3f *endpoint, Actor *obj, ActorCtl *gun);
void func_150FF474(Vec3f *position, Vec3f *points, u8 arg2, s32 arg3);
void func_151D5148(Actor *obj);
s32 func_151C229C(Vec3f *arg0, Vec3f *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, Actor *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);
void func_150F7470(Vec3f *, Vec3f *, s32, s32, s32, s32, f32, f32, f32, Actor *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
void func_151D3F14(Vec3f *position, u8 arg1, s32 arg2);
void func_151D4408(Vec3f *, Vec3f *, void *, Actor *, f32, s32, s32);
void func_1505D1C4(f32 x, f32 y, f32 z, s32 flags, s32 objIndex, s32 angle, s32 arg6, Vec3f *arg7);
s32 func_1514654C(Actor *obj, ActorCtl *attachment, s32 arg2, Vec3f **src, Vec3f **dst, s32 count);
void func_15145740(Actor *obj, Vec3f *direction, Vec3f *axis0, Vec3f *axis1, f32 arg4);

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
    s32 value0;
    s32 value1;
    ActorCtl *gun;
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

        if ((obj->unk1D4 != NULL) && ((obj->unk74 & 0xF) != 0xF)) {
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

        if (obj->id == 0x98) {
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
            func_151D4408(&hit, &normal, obj->unk1D4[((u8 *)gun)[2]], obj, 1.0f,
                          ((u8 *)&arg1)[3], arg2);
            func_150FF474(&position, points, ((u8 *)&arg1)[3], arg2);
        }
    }
}

/* Writes the world position of attachment set `setId` of `obj` to `out` (`obj`'s own position for -1). */
void func_150FEFD0(Actor *obj, s16 setId, Vec3f *out) {
    Vec3f *src;
    Vec3f *dst;
    ActorCtl *attachment;

    if (setId == -1) {
        out->x = obj->x_position;
        out->y = obj->y_position;
        out->z = obj->z_position;
    } else {
        attachment = func_1503195C(obj, setId, 0);
        if (attachment == NULL) {
            out->x = obj->x_position;
            out->y = obj->y_position;
            out->z = obj->z_position;
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
    s32 value0;
    s32 value1;
    ActorCtl *gun;
    u8 valid;
    s32 randomWord;
    s32 hasValue1;

    gun = func_150FF288(obj);
    if (gun != NULL) {
        func_151D5148(obj);
        func_150FF2AC(obj, &direction, &axis0, &axis1);

        if (obj->unk1D4 != NULL) {
            valid = TRUE;
        } else {
            valid = FALSE;
        }

        func_150FF2D4(&valid, points, &position, &hit, &normal, &direction, &axis0, &axis1, &output0, &output1,
                      &value0, &value1, &endpoint, obj, gun);

        if (value1 == 0) {
            hasValue1 = FALSE;
        } else {
            hasValue1 = TRUE;
        }

        randomWord = func_150ADA20();
        func_150F7470(&position, &direction, value0, value1, hasValue1, 0, 54.00000381f, 0.01851851679f, 300.0f,
                      obj, 1, 1, 1, 0, 0x1A, 1, (randomWord % 5U) + 807, 0, arg1, arg2);

        if (valid) {
            func_151D3F14(&position, arg1, arg2);
            func_151D4408(&hit, &normal, obj->unk1D4[((u8 *)gun)[2]], obj, 1.0f,
                          arg1, arg2);
            func_150FF474(&position, points, arg1, arg2);
        }
    }
}

/* Returns `obj`'s gun attachment (set 0x82), or NULL. */
ActorCtl *func_150FF288(Actor *obj) {
    return func_1503195C(obj, 0x82, 0);
}

/* Fills `direction`, `axis0` and `axis1` for `obj` through func_15145740. */
void func_150FF2AC(Actor *obj, Vec3f *direction, Vec3f *axis0, Vec3f *axis1) {
    func_15145740(obj, direction, axis0, axis1, 0.8110000491f);
}

u8 func_150FF6E0(Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal, Vec3f *endpoint,
                 Actor *obj, ActorCtl *gun);
void func_151D5174(Actor *obj, Vec3f *position, Vec3f *direction, Vec3f *axis0, Vec3f *axis1,
                   Vec3f *output0, Vec3f *output1, s32 *value0, s32 *value1, Vec3f *endpoint);

/*
 * Finds where a shot starts. With `*valid` set, the points come from the gun's bone (func_150FF6E0),
 * which can fail and clear `*valid`; otherwise the start is beside `obj`, to the right of `direction`.
 */
void func_150FF2D4(u8 *valid, Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal,
                   Vec3f *direction, Vec3f *axis0, Vec3f *axis1, Vec3f *output0, Vec3f *output1,
                   s32 *value0, s32 *value1, Vec3f *endpoint, Actor *obj, ActorCtl *gun) {
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
        position->x = obj->x_position + (34.0f * sideX);
        position->y = obj->y_position + 49.0f;
        position->z = obj->z_position + (34.0f * sideZ);
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
        func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0, 0, 0, 0x1B, variant + 1, 0, 0, 0, arg2, arg3); // matching: the int cast orders the arg setup
    }
}

/* Returns FALSE for object id 0x98, TRUE otherwise. */
s32 func_150FF6B4(Actor *obj, s32 arg1, s32 arg2) {
    if (obj->id == 0x98) {
        return FALSE;
    }
    return TRUE;
}

/* Muzzle-relative points in the gun's attachment space. */
extern Vec3f D_800A205C;
extern Vec3f D_800A2068;
extern Vec3f D_800A2074;
extern Vec3f D_800A2080[2][6]; /* two random sets of flash points */

/*
 * Transforms the gun's muzzle points by its bone into world space. `normal` ends up relative to `hit`.
 * Returns FALSE if the transform fails.
 */
u8 func_150FF6E0(Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal, Vec3f *endpoint,
                 Actor *obj, ActorCtl *gun) {
    u8 i;
    Vec3f *src[10];
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

void func_150FFCC8(Actor *obj, Vec3f *position, Vec3f *direction, Vec3f *axis0, Vec3f *axis1);
void func_150FFD84(Vec3f *position, Vec3f *direction, u8 arg2, s32 arg3);
void func_150FFB6C(Vec3f *position, Vec3f *origin, Actor *obj, s32 type);
void func_150FFC3C(Actor *obj);
void func_15102B38(Actor *obj, u8 bone, Vec3f *arg2, Vec3f *arg3, f32 *arg4, s32 arg5, s32 arg6, f32 arg7,
                   Vec3f *position, s32 arg9, s32 arg10, s32 arg11, u8 arg12, s32 arg13);
void func_15081690(Actor *obj, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, void *arg7, f32 arg8, s32 arg9,
                   s32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14);
s32 func_15081E78(Actor *obj, void *arg1, s32 arg2);
extern Vec3f D_800A2130;
extern Vec3f D_800A213C;

/*
 * Shot effects for `obj`: a muzzle effect and particles when it has bones, three func_1505D1C4
 * spawns along `direction` if `arg1`, a func_15081690 call if `arg2`, then a camera event.
 */
void func_150FF840(Actor *obj, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {
    Vec3f position;
    Vec3f direction;
    Vec3f axis0;
    Vec3f axis1;
    f32 motion[2];
    Vec3f trail[3];
    u8 scratch[0x64];
    void *buffer;
    ActorCtl *state;
    s32 random1;
    s32 random0;

    func_150FFCC8(obj, &position, &direction, &axis0, &axis1);
    func_151D5148(obj);
    if ((obj->unk1D4 != NULL) && ((obj->unk74 & 0xF) != 0xF)) {
        func_151D3F14(&position, arg3, arg4);
        motion[1] = ((func_150ADA68() * 3400.0f) + 4800.0f) * 0.001000000047f;
        motion[0] = ((func_150ADA68() * 220.0f) + 320.0f) * 0.1000000015f;
        random0 = func_150ADA20();
        random1 = func_150ADA20();
        func_15102B38(obj, D_80088BB0, &D_800A2130, &D_800A213C, motion,
                      (random0 % 3U) + 4, (random1 & 0x7F) + 0x80,
                      (func_150ADA68() * 450.0f) + 780.0f, &position, 0xFF,
                      0, -1, arg3, arg4);
        func_150FFD84(&position, &direction, arg3, arg4);
    }
    if (arg1) {
        s32 pad; // matching: unused stack slot
        trail[0].x = (direction.x * 0.08800000697f) + position.x;
        trail[0].y = (direction.y * 0.08800000697f) + position.y;
        trail[0].z = (direction.z * 0.08800000697f) + position.z;
        trail[1].x = (direction.x * 0.3830000162f) + position.x;
        trail[1].y = (direction.y * 0.3830000162f) + position.y;
        trail[1].z = (direction.z * 0.3830000162f) + position.z;
        trail[2].x = (direction.x * 0.810000062f) + position.x;
        trail[2].y = (direction.y * 0.810000062f) + position.y;
        trail[2].z = (direction.z * 0.810000062f) + position.z;
        func_150FFB6C(&trail[0], &position, obj, 0x32);
        func_150FFB6C(&trail[1], &position, obj, 0x30);
        func_150FFB6C(&trail[2], &position, obj, 0x31);
    }
    if (arg2) {
        state = obj->unk31C;
        if (state != NULL) {
            buffer = (u8 *)state + 0xB0;
        } else {
            buffer = scratch;
        }
        func_15081690(obj, position.x, position.y, position.z,
                      direction.x * 0.001000000047f, direction.y * 0.001000000047f,
                      direction.z * 0.001000000047f, buffer, 2000.0f, 0, 0, 0, -1, 0, 0);
        func_15081E78(obj, buffer, 0x10);
    }
    func_150FFC3C(obj);
}

/* Spawns func_1505D1C4 effect `type` at `position`, owned by `obj`. */
void func_150FFB6C(Vec3f *position, Vec3f *origin, Actor *obj, s32 type) {
    s32 flags;
    s32 index;
    u16 angle;

    flags = type | 0x60000;
    index = obj - D_800CC2D0;
    angle = obj->unk7A;
    func_1505D1C4(position->x, position->y, position->z, flags, index, angle, 0, origin);
}

s32 func_15145EA4(Vec3f **src, Vec3f **dst, void *mtx, s32 count);

/* Writes the world position of the muzzle point D_800A2130 on bone D_80088BB0 of `obj` to `out`. */
void func_150FFBDC(Actor *obj, s32 arg1, Vec3f *out) {
    Vec3f *src;
    Vec3f *dst;

    src = &D_800A2130;
    dst = out;
    func_15145EA4(&src, &dst, obj->unk1D4[D_80088BB0], 1);
}

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 unk7;
} Event151D8868;

void func_151D8868(Event151D8868 *event, s32 arg1, s32 arg2, s32 arg3);

/* Sends a func_151D8868 event (kind 1, 20-30 long) to `obj`'s camera, if it has one. */
void func_150FFC3C(Actor *obj) {
    Event151D8868 event;

    if (obj->camera != NULL) {
        event.unk0 = 1;
        event.unk2 = (func_150ADA20() % 11U) + 20;
        event.unk5 = 1 << obj->camera->playerIndex;
        event.unk4 = (func_150ADA20() & 1) + 7;
        event.unk6 = -1;
        func_151D8868(&event, 0, 0xFF, 1);
    }
}

void func_151D5A18(Actor *obj, Vec3f *position, Vec3f *direction, Vec3f *axis0, Vec3f *axis1,
                   Vec3f *arg5, u8 arg6);
void func_151D3E6C(Actor *obj, Vec3f *arg1, Vec3f *arg2, s32 arg3);

/* Gets the shot `position` and `direction` (and two axes) of `obj`. */
void func_150FFCC8(Actor *obj, Vec3f *position, Vec3f *direction, Vec3f *axis0, Vec3f *axis1) {
    func_151D5A18(obj, position, direction, axis0, axis1, D_8008FC8C[0], *D_8008FC94[0]);
    func_151D3E6C(obj, position, position, 0x8003A);
}

void func_15081E0C(Actor *obj, s32 arg1, s32 arg2);

/* Calls func_15081E0C(other, 4, 0) when `other` is a 0x9F/0xA0 object without flag 0x80 in unk94. */
void func_150FFD2C(Actor *obj, Actor *other, s32 arg2) {
    if (other->id == 0x9F || other->id == 0xA0) {
        if (!(other->unk94 & 0x80)) {
            func_15081E0C(other, 4, 0);
        }
    }
}

typedef struct {
    s32 id[4];
} Unk1DChoices;

void *func_15130280(ParticleDesc *spawn, u8 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5);
void func_15143794(s16 yaw, s16 pitch, f32 length, Vec3f *out);
void func_15145974(Vec3f *direction, f32 *yaw, f32 *pitch);
extern Unk1DChoices D_800A2160;

/* Spawns 10-18 particles at `position`, fanned out around the yaw of `direction`. */
void func_150FFD84(Vec3f *position, Vec3f *direction, u8 arg2, s32 arg3) {
    ParticleDesc spawn;
    s32 count;
    s16 baseYaw;
    s16 basePitch;
    s32 random0;
    f32 yaw;
    f32 pitch;

    count = func_150ADA20() % 9U + 10;
    spawn.renderMode = 0x2203;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r = 0xFF;
    spawn.g = 0xFF;
    spawn.b = 0xFF;
    spawn.a = 0xFF;
    spawn.r2 = 0xFF;
    spawn.g2 = 0xFF;
    spawn.b2 = 0xFF;
    spawn.brightness = 0xFF;
    spawn.pos = *position;
    spawn.offset = D_800A5480;
    spawn.fadeStart = 7;
    spawn.fadeRate = 36;
    spawn.scaleStart = 1;
    spawn.flags = 0x4C207;
    spawn.primColorMode = 0;
    spawn.envColorMode = 7;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 0;
    spawn.viewMask = 0;
    spawn.views = 0xFF;
    spawn.scaleRate = 1.0f;
    func_15145974(direction, &yaw, &pitch);
    pitch = 0.0f;
    /* degrees to a 16-bit binary angle, then to 1/256 turns */
    baseYaw = yaw * 182.0444489f;
    baseYaw >>= 8;
    baseYaw -= 138;
    basePitch = pitch * 182.0444489f;
    basePitch >>= 8;
    basePitch -= 138;
    do {
        Unk1DChoices ids;
        s32 random1;

        ids = D_800A2160;
        spawn.texture = ids.id[func_150ADA20() & 3];
        spawn.timer = func_150ADA20() % 13U + 10;
        spawn.alpha = func_150ADA20() % 101U + 155;
        spawn.width = spawn.height = func_150ADA68() * 155.0f + 75.0f;
        random0 = func_150ADA20();
        random1 = func_150ADA20();
        func_15143794(random0 % 21U + baseYaw, random1 % 21U + basePitch, func_150ADA68() * 20.0f + 20.0f,
                      &spawn.vel);
        spawn.gravity = func_150ADA68() * 0.6210000515f + -0.6570000052f;
        spawn.flags &= ~0xC0;
        random0 = (func_150ADA20() & 1) ? 0x80 : 0;
        random1 = (func_150ADA20() & 1) ? 0x40 : 0;
        spawn.flags |= random1 | random0;
        func_15130280(&spawn, 1, 0, 0, arg2, arg3);
        count--;
    } while (count > 0);
}
