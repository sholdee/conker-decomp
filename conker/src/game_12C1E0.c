#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Local point of the gun's muzzle in its attachment's space. */
extern struct17 D_800A2050;

struct126 *func_150FF288(struct127 *obj);
void func_150FF2AC(struct127 *obj, struct17 *direction, struct17 *axis0, struct17 *axis1);
void func_150FF2D4(u8 *valid, struct17 *points, struct17 *position, struct17 *hit, struct17 *normal,
                   struct17 *direction, struct17 *axis0, struct17 *axis1, struct17 *output0, struct17 *output1,
                   s32 *value0, s32 *value1, struct17 *endpoint, struct127 *obj, struct126 *gun);
void func_150FF474(struct17 *position, struct17 *points, u8 arg2, s32 arg3);
void func_151D5148(struct127 *obj);
s32 func_151C229C(struct17 *arg0, struct17 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, struct127 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);
void func_150F7470(struct17 *, struct17 *, s32, s32, s32, s32, f32, f32, f32, struct127 *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
void func_151D3F14(struct17 *position, u8 arg1, s32 arg2);
void func_151D4408(struct17 *, struct17 *, void *, struct127 *, f32, s32, s32);
void func_1505D1C4(f32, f32, f32, s32, s32, s32, s32, s32);
s32 func_1514654C(struct127 *obj, struct126 *attachment, s32 arg2, struct17 **src, struct17 **dst, s32 count);
void func_15145740(struct127 *obj, struct17 *direction, struct17 *axis0, struct17 *axis1, f32 arg4);

/* Fires a shot from the gun attachment (set 0x82) of `obj`, with muzzle effects when it has bones. */
void func_150FED30(struct127 *obj, s32 arg1, s32 arg2) {
    struct17 position;
    struct17 endpoint;
    struct17 direction;
    struct17 axis0;
    struct17 axis1;
    struct17 hit;
    struct17 normal;
    struct17 output0;
    struct17 output1;
    struct17 points[6];
    s32 value0;
    s32 value1;
    struct126 *gun;
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

        // matching: arg1 is read with lbu from its arg slot (a u8 param or cast caches it)
        func_151C229C(&endpoint, &direction, value0, value1, 1, 0, 300.0f, 0.003333333414f,
                      (random0 * 10.0f) + 25.0f, (random1 * 200.0f) + 600.0f,
                      50.0f, (randomWord % 156U) + 100, obj, 99, 1, 1, 1,
                      0, 1, 0, kind, 0.0f, 0xFF, directionSign, 0,
                      ((u8 *)&arg1)[3], arg2);

        if (valid) {
            func_151D3F14(&position, ((u8 *)&arg1)[3], arg2);
            /* byte 2 of an attachment is the owner bone it follows */
            func_151D4408(&hit, &normal, ((f32 (*)[4][4])obj->unk1D4)[((u8 *)gun)[2]], obj, 1.0f,
                          ((u8 *)&arg1)[3], arg2);
            func_150FF474(&position, points, ((u8 *)&arg1)[3], arg2);
        }
    }
}

/* Writes the world position of attachment set `setId` of `obj` to `out` (`obj`'s own position for -1). */
void func_150FEFD0(struct127 *obj, s16 setId, struct17 *out) {
    struct17 *src;
    struct17 *dst;
    struct126 *attachment;

    if (setId == -1) {
        out->unk0 = obj->x_position;
        out->unk4 = obj->y_position;
        out->unk8 = obj->z_position;
    } else {
        attachment = func_1503195C(obj, setId, 0);
        if (attachment == NULL) {
            out->unk0 = obj->x_position;
            out->unk4 = obj->y_position;
            out->unk8 = obj->z_position;
        } else {
            src = &D_800A2050;
            dst = out;
            func_1514654C(obj, attachment, 0, &src, &dst, 1);
        }
    }
}

/* Like func_150FED30, but spawns its shot through func_150F7470. */
void func_150FF084(struct127 *obj, s32 arg1, s32 arg2) {
    struct17 position;
    struct17 endpoint;
    struct17 direction;
    struct17 axis0;
    struct17 axis1;
    struct17 hit;
    struct17 normal;
    struct17 output0;
    struct17 output1;
    struct17 points[6];
    s32 value0;
    s32 value1;
    struct126 *gun;
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
        // matching: arg1 is read with lbu from its arg slot (a u8 param or cast caches it)
        func_150F7470(&position, &direction, value0, value1, hasValue1, 0, 54.00000381f, 0.01851851679f, 300.0f,
                      obj, 1, 1, 1, 0, 0x1A, 1, (randomWord % 5U) + 807, 0, ((u8 *)&arg1)[3], arg2);

        if (valid) {
            func_151D3F14(&position, ((u8 *)&arg1)[3], arg2);
            func_151D4408(&hit, &normal, ((f32 (*)[4][4])obj->unk1D4)[((u8 *)gun)[2]], obj, 1.0f,
                          ((u8 *)&arg1)[3], arg2);
            func_150FF474(&position, points, ((u8 *)&arg1)[3], arg2);
        }
    }
}

/* Returns `obj`'s gun attachment (set 0x82), or NULL. */
struct126 *func_150FF288(struct127 *obj) {
    return func_1503195C(obj, 0x82, 0);
}

void func_150FF2AC(struct127 *obj, struct17 *direction, struct17 *axis0, struct17 *axis1) {
    func_15145740(obj, direction, axis0, axis1, 0.8110000491f);
}

u8 func_150FF6E0(struct17 *points, struct17 *position, struct17 *hit, struct17 *normal, struct17 *endpoint,
                 struct127 *obj, struct126 *gun);
void func_151D5174(struct127 *obj, struct17 *position, struct17 *direction, struct17 *axis0, struct17 *axis1,
                   struct17 *output0, struct17 *output1, s32 *value0, s32 *value1, struct17 *endpoint);

/*
 * Finds where a shot starts. With `*valid` set, the points come from the gun's bone (func_150FF6E0),
 * which can fail and clear `*valid`; otherwise the start is beside `obj`, to the right of `direction`.
 */
void func_150FF2D4(u8 *valid, struct17 *points, struct17 *position, struct17 *hit, struct17 *normal,
                   struct17 *direction, struct17 *axis0, struct17 *axis1, struct17 *output0, struct17 *output1,
                   s32 *value0, s32 *value1, struct17 *endpoint, struct127 *obj, struct126 *gun) {
    if (*valid) {
        *valid = func_150FF6E0(points, position, hit, normal, endpoint, obj, gun);
    }
    if (!*valid) {
        f32 sideZ;
        f32 sideX;
        f32 dirX;

        dirX = direction->unk0;
        if ((0.0000999999974738f < fabsf(dirX)) || (0.0000999999974738f < fabsf(direction->unk8))) {
            f32 dirZ;
            f32 invLength;

            dirZ = direction->unk8;
            invLength = 1.0f / sqrtf((dirX * dirX) + (dirZ * dirZ));
            sideZ = dirZ * invLength;
            sideX = -dirX * invLength;
        } else {
            sideZ = 1.0f;
            sideX = 0.0f;
        }
        position->unk0 = obj->x_position + (34.0f * sideX);
        position->unk4 = obj->y_position + 49.0f;
        position->unk8 = obj->z_position + (34.0f * sideZ);
        *endpoint = *position;
    }
    func_151D5174(obj, position, direction, axis0, axis1, output0, output1, value0, value1, endpoint);
}

/* Spawns six muzzle-flash particles at `position`, one per entry of `points`. */
void func_150FF474(struct17 *position, struct17 *points, u8 arg2, s32 arg3) {
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
        struct17 position;
        struct17 unk28;
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
        func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0, 0, 0, 0x1B, variant + 1, 0, 0, 0, arg2, arg3);
    }
}

s32 func_150FF6B4(struct127 *obj, s32 arg1, s32 arg2) {
    if (obj->id == 0x98) {
        return FALSE;
    }
    return TRUE;
}

/* Muzzle-relative points in the gun's attachment space. */
extern struct17 D_800A205C;
extern struct17 D_800A2068;
extern struct17 D_800A2074;
extern struct17 D_800A2080[2][6]; /* two random sets of flash points */

/*
 * Transforms the gun's muzzle points by its bone into world space. `normal` ends up relative to `hit`.
 * Returns FALSE if the transform fails.
 */
u8 func_150FF6E0(struct17 *points, struct17 *position, struct17 *hit, struct17 *normal, struct17 *endpoint,
                 struct127 *obj, struct126 *gun) {
    u8 i;
    struct17 *src[10];
    struct17 *dst[10];
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
        normal->unk0 -= hit->unk0;
        normal->unk4 -= hit->unk4;
        normal->unk8 -= hit->unk8;
    } else {
        return FALSE;
    }
    return TRUE;
}

void func_150FFCC8(struct127 *obj, struct17 *position, struct17 *direction, struct17 *axis0, struct17 *axis1);
void func_150FFD84(struct17 *position, struct17 *direction, u8 arg2, s32 arg3);
void func_150FFB6C(struct17 *position, struct17 *origin, struct127 *obj, s32 type);
void func_150FFC3C(struct127 *obj);
void func_15102B38(struct127 *obj, u8 bone, struct17 *arg2, struct17 *arg3, f32 *arg4, s32 arg5, s32 arg6, f32 arg7,
                   struct17 *position, s32 arg9, s32 arg10, s32 arg11, u8 arg12, s32 arg13);
void func_15081690(struct127 *obj, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, void *arg7, f32 arg8, s32 arg9,
                   s32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14);
s32 func_15081E78(struct127 *obj, void *arg1, s32 arg2);
extern struct17 D_800A2130;
extern struct17 D_800A213C;

void func_150FF840(struct127 *obj, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {
    struct17 position;
    struct17 direction;
    struct17 axis0;
    struct17 axis1;
    f32 motion[2];
    struct17 trail[3];
    u8 scratch[0x64];
    void *buffer;
    struct126 *state;
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
        trail[0].unk0 = (direction.unk0 * 0.08800000697f) + position.unk0;
        trail[0].unk4 = (direction.unk4 * 0.08800000697f) + position.unk4;
        trail[0].unk8 = (direction.unk8 * 0.08800000697f) + position.unk8;
        trail[1].unk0 = (direction.unk0 * 0.3830000162f) + position.unk0;
        trail[1].unk4 = (direction.unk4 * 0.3830000162f) + position.unk4;
        trail[1].unk8 = (direction.unk8 * 0.3830000162f) + position.unk8;
        trail[2].unk0 = (direction.unk0 * 0.810000062f) + position.unk0;
        trail[2].unk4 = (direction.unk4 * 0.810000062f) + position.unk4;
        trail[2].unk8 = (direction.unk8 * 0.810000062f) + position.unk8;
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
        func_15081690(obj, position.unk0, position.unk4, position.unk8,
                      direction.unk0 * 0.001000000047f, direction.unk4 * 0.001000000047f,
                      direction.unk8 * 0.001000000047f, buffer, 2000.0f, 0, 0, 0, -1, 0, 0);
        func_15081E78(obj, buffer, 0x10);
    }
    func_150FFC3C(obj);
}

void func_150FFB6C(struct17 *position, struct17 *origin, struct127 *obj, s32 type) {
    s32 flags;
    s32 index;
    u16 angle;

    flags = type | 0x60000;
    index = obj - D_800CC2D0;
    angle = obj->unk7A;
    func_1505D1C4(position->unk0, position->unk4, position->unk8, flags, index, angle, 0, (s32)origin);
}

s32 func_15145EA4(struct17 **src, struct17 **dst, void *mtx, s32 count);

/* Writes the world position of the muzzle point D_800A2130 on bone D_80088BB0 of `obj` to `out`. */
void func_150FFBDC(struct127 *obj, s32 arg1, struct17 *out) {
    struct17 *src;
    struct17 *dst;

    src = &D_800A2130;
    dst = out;
    func_15145EA4(&src, &dst, ((f32 (*)[4][4])obj->unk1D4)[D_80088BB0], 1);
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

void func_150FFC3C(struct127 *obj) {
    Event151D8868 event;

    if (obj->camera != NULL) {
        event.unk0 = 1;
        event.unk2 = (func_150ADA20() % 11U) + 20;
        event.unk5 = 1 << obj->camera->unk23D;
        event.unk4 = (func_150ADA20() & 1) + 7;
        event.unk6 = -1;
        func_151D8868(&event, 0, 0xFF, 1);
    }
}

extern struct17 *D_8008FC8C[];
extern u8 *D_8008FC94[];
void func_151D5A18(struct127 *obj, struct17 *position, struct17 *direction, struct17 *axis0, struct17 *axis1,
                   struct17 *arg5, u8 arg6);
void func_151D3E6C(struct127 *obj, struct17 *arg1, struct17 *arg2, s32 arg3);

void func_150FFCC8(struct127 *obj, struct17 *position, struct17 *direction, struct17 *axis0, struct17 *axis1) {
    func_151D5A18(obj, position, direction, axis0, axis1, D_8008FC8C[0], *D_8008FC94[0]);
    func_151D3E6C(obj, position, position, 0x8003A);
}

void func_15081E0C(struct127 *obj, s32 arg1, s32 arg2);

void func_150FFD2C(struct127 *obj, struct127 *other, s32 arg2) {
    if (other->id == 0x9F || other->id == 0xA0) {
        if (!(other->unk94 & 0x80)) {
            func_15081E0C(other, 4, 0);
        }
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Func150FFD84Spawn;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);
extern void func_15145974(struct17 *, f32 *, f32 *);
typedef struct { s32 v[4]; } Func150FFD84Colors;
extern Func150FFD84Colors D_800A2160;

void func_150FFD84(struct17 *arg0, struct17 *arg1, u8 arg2, s32 arg3) {
    Func150FFD84Spawn spawn;
    s32 count;
    s16 angle1;
    s16 angle2;
    s32 r1;
    f32 yaw;
    f32 pitch;

    count = func_150ADA20() % 9U + 10;
    spawn.unk8 = 0x2203;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk14 = 0xFF;
    spawn.unk15 = 0xFF;
    spawn.unk16 = 0xFF;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0xFF;
    spawn.unk1A = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk30 = *arg0;
    spawn.unk3C = *(struct17 *)&D_800A5480;
    spawn.unk1E = 7;
    spawn.unk20 = 0x24;
    spawn.unk22 = 1;
    spawn.unk58 = 0x4C207;
    spawn.unk60 = 0;
    spawn.unk61 = 7;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    spawn.unk24 = 1.0f;
    func_15145974(arg1, &yaw, &pitch);
    pitch = 0.0f;
    angle1 = (s16)(yaw * 182.0444489f);
    angle1 >>= 8;
    angle1 -= 0x8A;
    angle2 = (s16)(pitch * 182.0444489f);
    angle2 >>= 8;
    angle2 -= 0x8A;
    do {
        Func150FFD84Colors colors;
        s32 r2;

        colors = D_800A2160;
        spawn.unk1D = colors.v[func_150ADA20() & 3];
        spawn.unkA = func_150ADA20() % 13U + 10;
        spawn.unk1B = func_150ADA20() % 101U + 155;
        spawn.unk28 = spawn.unk2C = func_150ADA68() * 155.0f + 75.0f;
        r1 = func_150ADA20();
        r2 = func_150ADA20();
        func_15143794(r1 % 21U + angle1, r2 % 21U + angle2, func_150ADA68() * 20.0f + 20.0f, &spawn.unk48.unk0);
        spawn.unk54 = func_150ADA68() * 0.6210000515f + -0.6570000052f;
        spawn.unk58 &= ~0xC0;
        r1 = (func_150ADA20() & 1) ? 0x80 : 0;
        r2 = (func_150ADA20() & 1) ? 0x40 : 0;
        spawn.unk58 |= r2 | r1;
        func_15130280(&spawn, 1, 0, 0, arg2, arg3);
        count--;
    } while (count > 0);
}
