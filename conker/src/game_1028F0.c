#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A0AB8;
extern f32 D_800A0ABC;
extern f32 D_800A0AC0;
extern f32 D_800A0AC4;
extern f32 D_800A0AC8;
extern f32 D_800A0AB0;
extern f32 D_800A0AB4;

/* Particle spawn template passed to func_15130280. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex unk3C;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ f32 unk6C;
} ParticleSpawn; /* size 0x70 */

/* Effect (a struct260 with per-effect data at 0x28) that follows an object. */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 spawnAccum;
} FollowEffectData;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state;
    /* 0x10 */ u8 pad10[0x28 - 0x10];
    /* 0x28 */ FollowEffectData data;
} FollowEffect;

/* Per-particle data of the drifting particles that follow an object (func_150D6CC4). */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 drag; /* passed to func_15131958 with the velocity */
} DriftParticleData;

/* Per-particle data of the growing, fading particle that follows an object (func_150D6F0C). */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 time;
    /* 0x0C */ f32 lifetime;
    /* 0x10 */ f32 growth;    /* size = sqrt(time) * growth */
    /* 0x14 */ f32 fadeStart; /* unk2B = fadeStart - fadeRate * time^2 */
    /* 0x18 */ f32 fadeRate;
} GrowParticleData;

/* Live particle, as created by func_15130280 from a ParticleSpawn. */
typedef struct {
    /* 0x00 */ u8 pad0[0x2B];
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 pad2C[0x38 - 0x2C];
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ vertex anchor;
    /* 0x4C */ vertex offset;
    /* 0x58 */ vertex vel;
    /* 0x64 */ f32 accelY;
    /* 0x68 */ u8 pad68[0xA8 - 0x68];
    /* 0xA8 */ union {
        DriftParticleData drift;
        GrowParticleData grow;
    } data;
} Particle;

/* Payload of the messages the effect and particle handlers receive. */
typedef union {
    struct {
        /* 0x00 */ struct127 *object;
        /* 0x04 */ u8 id;
    } one;          /* messages 0, 3, 0x2F */
    struct {
        /* 0x00 */ struct127 *from;
        /* 0x04 */ struct127 *to;
        /* 0x08 */ u8 fromId;
        /* 0x09 */ u8 toId;
    } pair;         /* message 0x2D */
} ObjectMessage;

extern Particle *func_15130280(ParticleSpawn *, u8, s32, s32, u8, s32);
extern void func_1514373C(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3);
extern s32 func_1515C0F8(struct127 *arg0, f32 **arg1);
extern void func_15182670(s32, s32, s32, s32, s32, s32, s32, s32);
extern void *func_15154884(vertex *, f32, f32, f32, u8, s32);

/* Attaches a FollowEffect (func_150D54C8) to obj. */
void func_150D5440(struct127 *obj, u8 arg1, s32 arg2) {
    FollowEffectData data;
    FollowEffect *effect;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.spawnAccum = 0.0f;

    effect = (FollowEffect *)func_15149130(300, -1, 0x38, -1, 0, 0x28, (struct37 *)sizeof(data), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/*
 * FollowEffect update: while the owner is low (unk28 < 8) on a type-0xE surface, emits
 * particles on both sides of it at a rate of 3-4 per second (scaled by D_800BE9A4).
 */
void func_150D54C8(FollowEffect *effect) {
    struct127 *owner;
    FollowEffectData *data;
    ParticleSpawn spawn;
    f32 cosYaw;
    f32 sinYaw;
    f32 rightX;
    f32 rightZ;
    f32 *refVel;
    u8 right;
    f32 along;
    f32 speed;
    f32 sideOffset;

    owner = effect->data.owner;
    data = &effect->data;
    if (owner) {} // matching: raises owner's register priority over data
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->state = -1;
        return;
    }
    if (owner->unk28 < 8.0f && (owner->unk184 & 0x1F) == 0xE) {
        // matching: (f32)1 keeps the multiply by 1.0 that a 1.0f literal would fold away
        data->spawnAccum += (3.000000238f + func_150ADA68() * (f32)1) * D_800BE9A4;
        if (data->spawnAccum > 1.0f) {
            if (func_1515C0F8(owner, &refVel) == 0) {
                refVel = &D_800A5480;
            }
            func_1514373C(owner->unk40 * 0.01745329238f, 1.0f, &sinYaw, &cosYaw);
            rightX = cosYaw;
            rightZ = -sinYaw;
            spawn.unk1D = 40;
            spawn.unk8 = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk14 = 0xFF;
            spawn.unk15 = 0xFF;
            spawn.unk16 = 0xFF;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0xFF;
            spawn.unk19 = 0xFF;
            spawn.unk1A = 0xFF;
            spawn.unk4 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk1C = 0xFF;
            spawn.unk1E = 5;
            spawn.unk20 = 51;
            spawn.unk22 = 1;
            spawn.flags = 0x84C207;
            spawn.unk60 = 6;
            spawn.unk61 = 8;
            spawn.unk62 = -1;
            spawn.unk63 = -1;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            spawn.unk5C = 0;
            spawn.unk66 = 0xFF;
            spawn.unk24 = 1.0f;
            spawn.unk3C.x = 0.0f;
            spawn.unk3C.y = 0.0f;
            spawn.unk3C.z = 0.0f;
            do {
                right = func_150ADA20() & 1;
                along = func_150ADA68();
                speed = func_150ADA68();
                if (right) {
                    sideOffset = 35.0f;
                } else {
                    sideOffset = -35.0f;
                }
                spawn.pos.x = owner->x_position + rightX * sideOffset - sinYaw * 180.0f * 0.5f + sinYaw * 180.0f * along;
                spawn.pos.y = owner->unk180;
                spawn.pos.z = owner->z_position + rightZ * sideOffset - cosYaw * 180.0f * 0.5f + cosYaw * 180.0f * along;
                // matching: the two spellings of 6.0f / 0.564f keep IDO from sharing one rodata load
                spawn.vel.x = (right ? (speed = speed * 2.907000065f + 6.000000477f)
                                     : -(speed = speed * 2.907000065f + 6.0000005f)) * rightX - refVel[0] * 0.5640000105f;
                spawn.vel.y = 3.156000137f * speed;
                spawn.vel.z = (right ? speed : -speed) * rightZ - refVel[2] * 0.564f;
                spawn.unkA = func_150ADA20() % 10U + 31;
                spawn.unk1B = func_150ADA20() % 76U + 180;
                spawn.unk28 = spawn.unk2C = func_150ADA68() * 364.0f + 111.0f;
                spawn.unk54 = func_150ADA68() * 0.1570000052f + -2.111000061f;
                spawn.flags &= ~0xC0;
                right = (func_150ADA20() & 1) ? 0x80 : 0; // matching: reuses the u8 local for the 0x80 flag
                spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | right;
                func_15130280(&spawn, 1, 0, 0, effect->unkC, effect->unk1);
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

/* FollowEffect message handler: dies with its owner and follows it through message 0x2D. */
void func_150D596C(FollowEffect *effect, ObjectMessage *msg, u8 type) {
    FollowEffectData *data = &effect->data;
    struct127 *object;

    if (type == 0 || type == 0x2F || type == 3) {
        object = msg->one.object;
        if (object == data->owner || msg->one.id == data->ownerId) {
            func_1516972C((struct102 *)effect);
        }
    } else if (type == 0x2D) {
        if (data->owner == msg->pair.from) {
            data->owner = msg->pair.to;
            data->ownerId = msg->pair.toId;
        } else if (data->owner == msg->pair.to) {
            data->owner = msg->pair.from;
            data->ownerId = msg->pair.fromId;
        }
    }
}

void func_150D5A2C(struct260 *arg0) {
    func_1514933C(arg0);
}

void func_150D5A4C(struct260 *arg0) {
    func_15149368(arg0);
}

struct Vec3F;

typedef struct {
    f32 x, y, z;
} Vec3f_150D5A6C;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 scaleXZ;
    /* 0x0C */ f32 scaleY;
    /* 0x10 */ f32 rot[3];
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ Vec3f_150D5A6C pos;
    /* 0x34 */ f32 vel[3];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ struct127 *unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 unk78;
} Particle150D5A6C;

typedef struct {
    s32 v[3];
} Arr3_150D5A6C;

extern Arr3_150D5A6C D_800A0A30;
extern s32 *D_80089A20[];
extern s32 D_800A3F14[];

s32 func_15134070(struct127 *);
s32 func_15145128(void *, void *, void *, void *);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, void *, void *, void *, void *, f32 *, s32, s32 *, f32);
s32 func_15144E80(void *, Vec3f_150D5A6C *, Vec3f_150D5A6C *, Vec3f_150D5A6C *);
f32 func_15144A74(Vec3f_150D5A6C *, Vec3f_150D5A6C *);
void func_15146078(Vec3f_150D5A6C *, Vec3f_150D5A6C *, Vec3f_150D5A6C *);
void func_15143874(s16, f32, f32 *, f32 *);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
void func_151C05A4(void *, u8, s32);
void func_151C05F0(void *, u8, s32);
void func_151C0360(void *, void *, u8, s32);
void func_150D6388(void *arg0, u8 arg1, s32 arg2);
void func_150D6434(struct Vec3F *arg0, u8 arg1, s32 arg2);
void func_150D65F0(s32 arg0, s32 arg1, u8 arg2, s32 arg3);
void func_150D66A4(u8 arg0, u8 arg1, s32 arg2);

void func_150D5A6C(struct127 *arg0, u8 arg1, s32 arg2) {
    Vec3f_150D5A6C pos;
    Vec3f_150D5A6C vel;
    s32 sp1A4;
    s32 sp1A0;
    s32 sp19C;
    u8 hit;
    s32 needDefault;
    f32 sp188[3];
    s32 sp184;
    s32 sp180;
    s32 sp17C;
    s32 type;
    f32 dist;
    Vec3f_150D5A6C n;
    Vec3f_150D5A6C b;
    Vec3f_150D5A6C c;
    s32 found;
    Particle150D5A6C p;

    hit = 0;
    pos.x = arg0->x_position;
    pos.y = arg0->y_position + 50.0f;
    pos.z = arg0->z_position;
    vel.x = arg0->x_position - arg0->old_x_position;
    vel.y = arg0->y_position - arg0->old_y_position;
    vel.z = arg0->z_position - arg0->old_z_position;
    type = func_15134070(arg0);
    if (type == 0x63) {
        return;
    }
    if ((arg0->unk184 & 0x1F) == 0xE) {
        func_10010630(0x434, arg0, 0x7D00, 0x2BC, 0x7D0);
    } else {
        func_10010630(0x4C8, arg0, -0x7D00, 0x2BC, 0x7D0);
    }
    func_10010630((func_150ADA20() & 1) + 0x2B6, arg0, 0x7D00, 0x3E8, 0xBB8);
    func_10010630((func_150ADA20() & 3) + 0x2B1, arg0, 0x7D00, 0x2BC, 0x7D0);
    func_151C05A4(&pos, arg1, arg2);
    func_151C05F0(&pos, arg1, arg2);
    func_15145A50(arg0);
    if (arg0->unk1D4 == NULL) {
        return;
    }
    if ((arg0->unk74 & 0xF) == 0xF) {
        return;
    }
    found = func_15145128(&vel, &vel, &sp184, &sp180) ? 1 : 0;
    if (found) {
        typedef s32 ZQ;
        if (func_150AC9C0(arg0->old_x_position, arg0->old_y_position, arg0->old_z_position,
                               vel.x, vel.y, vel.z, 0, sp188, &sp19C, &sp1A0, &sp1A4, &dist, 0, &sp17C, 0.0f) && dist < 397.0f) {
            hit = 1;
        }
    }
    { typedef s32 ZQ2;
    needDefault = 1;
    if (hit) {
        needDefault = 0;
        if (func_15144E80(sp188, &b, &c, &n)) {
            s32 spCC;
            s32 spC8;
            s32 spC4;
            s32 spC0;
            s32 spBC;
            s32 spB8;

            func_15145128(&b, &b, &spCC, &spC8);
            func_15145128(&c, &c, &spC4, &spC0);
            func_15145128(&n, &n, &spBC, &spB8);
            if (func_15144A74(&n, &vel) > 0.0f) {
                n.x = -n.x;
                n.y = -n.y;
                n.z = -n.z;
            }
        } else {
            needDefault = 1;
        }
    }
    if (needDefault) {
        if (found) {
            n.x = -vel.x;
            n.y = -vel.y;
            n.z = -vel.z;
            func_15146078(&n, &b, &c);
        } else {
            n.x = 0.0f;
            n.y = 1.0f;
            n.z = 0.0f;
            b.x = 1.0f;
            b.y = 0.0f;
            b.z = 0.0f;
            c.x = 0.0f;
            c.y = 0.0f;
            c.z = 1.0f;
        }
    }
    p.unk0 = 1.0f;
    p.unk4 = 1.0f;
    n.x *= 1000.0f;
    n.y *= 1000.0f;
    n.z *= 1000.0f;
    p.scaleXZ = arg0->xz_scale;
    p.scaleY = arg0->y_scale;
    p.unk1C[0] = 1.0f;
    p.unk1C[1] = 1.0f;
    p.unk1C[2] = 1.0f;
    p.pos = pos;
    p.unk50 = 0x21E8;
    p.unk58 = 0;
    p.unk5C = 0;
    p.unk60 = 0xFF;
    p.unk61 = 8;
    p.unk62 = 0;
    p.unk63 = 0;
    p.unk64 = 0;
    p.unk65 = 0;
    p.unk66 = 0;
    p.unk67 = 0;
    p.unk68 = 2;
    p.unk6A = 1;
    p.unk6C = arg0;
    p.unk70 = arg0->unique_id;
    p.unk78 = 0;
    p.unk44 = 0.0f;
    {
        const s32 arr[3] = { 0x6C, 0x6D, 0x6E };
        s32 i;
        f32 spA4;
        f32 spA0;
        f32 scale;
        s32 r;
        f32 sp94;
        f32 sp90;
        s32 j;

    p.unk72 = 0xC;
    p.unk74 = 0x15;
    for (i = 0; i != 3; i++) {
        scale = func_150ADA68() * 0.01319999993f + 0.02979999967f;
        r = func_150ADA20();
        func_15143874((u8)r, func_150ADA68() * 759.0f, &spA0, &spA4);
        p.vel[0] = (b.x * spA0 + c.x * spA4 + n.x) * scale;
        p.vel[1] = (b.y * spA0 + c.y * spA4 + n.y) * scale;
        p.vel[2] = (b.z * spA0 + c.z * spA4 + n.z) * scale;
        p.rot[0] = func_150ADA68() * 360.0f;
        p.rot[1] = func_150ADA68() * 360.0f;
        p.rot[2] = func_150ADA68() * 360.0f;
        p.unk40 = func_150ADA68() * 38.44400024f + -19.22200012f;
        p.unk48 = func_150ADA68() * 38.44400024f + -19.22200012f;
        p.unk4C = func_150ADA68() * 0.6480000019f + -1.397000074f;
        p.unk54 = (func_150ADA20() % 21U) + 30;
        p.unk56 = arr[i];
        func_15132A4C(&p, 0, 0, 0, arg1, arg2);
    }
    p.unk72 = 0xC;
    p.unk74 = 0x15;
    for (j = 2; j < D_800A3F14[type]; j++) {
        scale = func_150ADA68() * 0.01999999955f + 0.005200000014f;
        r = func_150ADA20();
        func_15143874((u8)r, func_150ADA68() * 723.0f, &sp90, &sp94);
        p.vel[0] = (b.x * sp90 + c.x * sp94 + n.x) * scale;
        p.vel[1] = (b.y * sp90 + c.y * sp94 + n.y) * scale;
        p.vel[2] = (b.z * sp90 + c.z * sp94 + n.z) * scale;
        p.rot[0] = func_150ADA68() * 360.0f;
        p.rot[1] = func_150ADA68() * 360.0f;
        p.rot[2] = func_150ADA68() * 360.0f;
        p.unk40 = func_150ADA68() * 40.45000076f + -20.22500038f;
        p.unk48 = func_150ADA68() * 40.45000076f + -20.22500038f;
        p.unk4C = func_150ADA68() * 0.6260000467f + -1.089000106f;
        p.unk54 = (func_150ADA20() % 41U) + 40;
        p.unk56 = D_80089A20[type][j];
        func_15132A4C(&p, 0, 0, 0, arg1, arg2);
    }
    }
    if (arg0->camera != NULL && D_80082FA0 <= 0) {
        func_150D66A4(arg0->camera->unk23D, arg1, arg2);
    }
    if (hit && arg0->camera != NULL) {
        if ((sp17C & 0x1F) == 0xE) {
            func_150D65F0((s32)sp188, (s32)&sp19C, arg1, arg2);
        } else {
            func_151C0360(sp188, &sp19C, arg1, arg2);
        }
    }
    func_150D6434((struct Vec3F *)&pos, arg1, arg2);
    func_150D6388(&pos, arg1, arg2);
    }
}

void func_151541B8(void *, f32, s32, f32, f32, u8, s32);

void func_150D6388(void *arg0, u8 arg1, s32 arg2) {
    union { f32 f; f64 _a; } temp_f0;
    u32 temp_a;

    temp_f0.f = func_150ADA68();
    temp_a = func_150ADA20();
    func_151541B8(arg0, temp_f0.f * 4.0f + 15.0f, 0x3FAFF1E9,
                  (f32)(u32)((temp_a % 0x38U) + 0xC8), 0.0f, arg1, arg2);
}

void func_150D6434(struct Vec3F *arg0, u8 arg1, s32 arg2) {
    union { f32 f; f64 _a; } temp1;
    f32 temp0;

    temp0 = func_150ADA68();
    temp1.f = func_150ADA68();
    func_15154884(arg0, temp0 * 3.0f + 8.0f, temp1.f * D_800A0AB0 + D_800A0AB4,
                  func_150ADA68() * 50.0f + 100.0f, arg1, arg2);
}

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ u8  pad08[0xC];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s8  unk50;
} Struct150D64E8;

void func_150D64E8(Struct150D64E8 *arg0, f32 *arg1) {
    arg0->unk00 = 6;
    arg0->unk04 = 3;
    arg1[0] = D_800A0AB8;
    arg0->unk14 = D_800A0ABC;
    arg0->unk18 = D_800A0AC0;
    arg0->unk1C = 150.0f;
    arg0->unk20 = 105.0f;
    arg0->unk24 = 396.0f;
    arg0->unk28 = 612.0f;
    arg0->unk2C = 4;
    arg0->unk30 = 3;
    arg0->unk34 = 25.0f;
    arg0->unk38 = D_800A0AC4;
    arg0->unk3C = -2.0f;
    arg0->unk40 = D_800A0AC8;
    arg0->unk44 = 20;
    arg0->unk46 = 30;
    arg0->unk48 = 100;
    arg0->unk4A = 100;
    arg0->unk4C = 12;
    arg0->unk4E = 20;

    if (D_80082FA0 < 2) {
        if (D_8008FD8C < 8) {
            goto zero;
        }
    }

    arg0->unk50 = -1;
    return;

zero:
    arg0->unk50 = 0;
}

void func_151A5D58(f32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_150D65F0(s32 arg0, s32 arg1, u8 arg2, s32 arg3) {
    union { u32 w; f64 _a; } temp_a;
    f32 temp_f0;
    u32 temp_b;

    temp_f0 = func_150ADA68();
    temp_a.w = func_150ADA20();
    temp_b = func_150ADA20();
    func_151A5D58(temp_f0 * 100.0f + 150.0f, ((temp_a.w % 71) + 0x82) & 0xFF, arg0, arg1,
                  (temp_b % 31) + 0x32, 0, 1, arg2, arg3);
}

void func_150D66A4(u8 arg0, u8 arg1, s32 arg2) {
    u32 temp1;
    u32 temp0;

    temp0 = func_150ADA20();
    temp1 = func_150ADA20();
    func_15182670(0xFF, 0xFF, 0xFF, (u8)(temp0 % 0x38 + 0xC8), temp1 % 6 + 0x19, arg0, arg1, arg2);
}

void func_150D6C98(struct127 *obj, vertex *out);
extern void func_15143794(s16, s16, f32, f32 *);

/* Spawns 15-25 drift particles and one grow particle that follow obj. */
void func_150D6730(struct127 *obj, u8 arg1, s32 arg2) {
    s32 count;
    ParticleSpawn spawn;
    DriftParticleData drift;
    Particle *particle;
    ParticleSpawn growSpawn;
    GrowParticleData grow;
    s32 r1;
    s32 r2;

    count = func_150ADA20() % 11U + 15;
    drift.owner = obj;
    drift.ownerId = obj->unique_id;
    drift.drag = 0.8711180091f;
    spawn.unk1D = 0x6C;
    spawn.unk8 = 0x5103;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk18 = 0x14;
    spawn.unk19 = 0x14;
    spawn.unk1A = 0x14;
    spawn.unk17 = 0xFF;
    spawn.unk14 = 0x4B;
    spawn.unk15 = 0x4B;
    spawn.unk16 = 0x4B;
    spawn.unk1C = 0xFF;
    func_150D6C98(obj, &spawn.pos);
    spawn.unk1E = 0xF;
    spawn.unk20 = 0x11;
    spawn.flags = 0x84DE01;
    spawn.unk60 = 8;
    spawn.unk61 = 6;
    spawn.unk62 = 0x1E;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 8;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    spawn.unk3C.x = 0.0f;
    spawn.unk3C.y = 0.0f;
    spawn.unk3C.z = 0.0f;
    spawn.unk24 = 1.062733054f;
    do {
        spawn.unk1B = func_150ADA20() % 156U + 100;
        spawn.unk22 = spawn.unkA = func_150ADA20() % 14U + 8;
        spawn.unk28 = spawn.unk2C = func_150ADA68() * 207.0f + 238.0f;
        r1 = func_150ADA20();
        r2 = func_150ADA20();
        func_15143794(r1 & 0xFF, r2 % 36U - 25, func_150ADA68() * 30.0f + 25.0f, &spawn.vel.x);
        spawn.unk54 = func_150ADA68() * (f32)0;
        spawn.flags &= ~0xC0;
        r1 = (func_150ADA20() & 1) ? 0x80 : 0;
        r2 = (func_150ADA20() & 1) ? 0x40 : 0;
        spawn.flags |= r2 | r1;
        particle = func_15130280(&spawn, 1, 0, 0xC, arg1, arg2);
        if (particle != NULL) {
            memcpy(&particle->data.drift, &drift, sizeof(drift));
        }
        count--;
    } while (count > 0);

    {
    s32 pad;
    grow.owner = obj;
    grow.ownerId = obj->unique_id;
    grow.time = 0.0f;
    grow.growth = 2392.159912f;
    grow.lifetime = func_150ADA68() * 3.0f + 9.0f;
    grow.fadeStart = func_150ADA68() * 80.0f + 100.0f;
    grow.fadeRate = grow.fadeStart / (grow.lifetime * grow.lifetime);
    growSpawn.unk1B = (u32)grow.fadeStart;
    growSpawn.unk1D = 0x79;
    growSpawn.unk8 = 0x4403;
    growSpawn.unk0 = 0x200005;
    growSpawn.unk4 = 0;
    growSpawn.unkC = 0;
    growSpawn.unk10 = 0;
    growSpawn.unk18 = 0xFF;
    growSpawn.unk19 = 0xFF;
    growSpawn.unk1A = 0xDF;
    growSpawn.unk17 = 0xFF;
    growSpawn.unk14 = 0xFF;
    growSpawn.unk15 = 0xFF;
    growSpawn.unk16 = 0xFF;
    growSpawn.unk1C = 0xFF;
    func_150D6C98(obj, &growSpawn.pos);
    growSpawn.unk3C = *(vertex *)&D_800A5480;
    growSpawn.unk1E = 1;
    growSpawn.unk20 = 0xFF;
    growSpawn.unk22 = 1;
    growSpawn.unk24 = 1.0f;
    r1 = (func_150ADA20() & 1) ? 0x40 : 0;
    r2 = (func_150ADA20() & 1) ? 0x80 : 0;
    growSpawn.flags = r2 | 0x4C000 | r1;
    growSpawn.unk60 = 6;
    growSpawn.unk61 = 6;
    growSpawn.unk62 = 0x1F;
    growSpawn.unk63 = -1;
    growSpawn.unk64 = -1;
    growSpawn.unk65 = 9;
    growSpawn.unk5C = 0;
    growSpawn.unk66 = 0xFF;
    growSpawn.vel = *(vertex *)&D_800A5480;
    growSpawn.unkA = 300;
    growSpawn.unk2C = 0.0f;
    growSpawn.unk28 = 0.0f;
    growSpawn.unk54 = 0.0f;
    particle = func_15130280(&growSpawn, 1, 0, 0x1C, arg1, arg2);
    if (particle != NULL) {
        memcpy(&particle->data.grow, &grow, sizeof(grow));
    }
    }
}

/* Writes the point 60 units above obj's position to out. */
void func_150D6C98(struct127 *obj, vertex *out) {
    out->x = obj->x_position;
    out->y = obj->y_position + 60.0f;
    out->z = obj->z_position;
}

void func_15131958(vertex *, f32);

/* Drift particle update: follows the owner and integrates offset/velocity; returns 0 to kill. */
s32 func_150D6CC4(Particle *particle, s32 arg1) {
    DriftParticleData *data;
    struct127 *owner;

    data = &particle->data.drift;
    owner = data->owner;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        return 0;
    }
    func_150D6C98(owner, &particle->anchor);
    {
        vertex prevVel = particle->vel;
        f32 accelX;
        f32 accelY;
        f32 accelZ;

        particle->vel.y += particle->accelY * D_800BE9A4;
        func_15131958(&particle->vel, data->drag);
        accelX = (particle->vel.x - prevVel.x) * D_800BE9A8;
        accelY = (particle->vel.y - prevVel.y) * D_800BE9A8;
        accelZ = (particle->vel.z - prevVel.z) * D_800BE9A8;
        particle->offset.x += prevVel.x * D_800BE9A4 +
            accelX * D_800BE9A4 * D_800BE9A4 * 0.5f;
        particle->offset.y += prevVel.y * D_800BE9A4 +
            accelY * D_800BE9A4 * D_800BE9A4 * 0.5f;
        particle->offset.z += prevVel.z * D_800BE9A4 +
            accelZ * D_800BE9A4 * D_800BE9A4 * 0.5f;
    }
    return 1;
}

/* Drift particle message handler: dies with its owner and follows it through message 0x2D. */
void func_150D6E60(Particle *particle, ObjectMessage *msg, u8 type) {
    DriftParticleData *data = &particle->data.drift;
    struct127 *object;

    if (type == 0x2D) {
        if (data->owner == msg->pair.from) {
            data->owner = msg->pair.to;
            data->ownerId = msg->pair.toId;
        } else {
            if (data->owner == msg->pair.to) {
                data->owner = msg->pair.from;
                data->ownerId = msg->pair.fromId;
            }
        trailing_150D6E60: // matching: a label here keeps IDO from merging this path's exit branch
            ;
        }
    } else if (type == 0) {
        object = msg->one.object;
        if (object == data->owner || msg->one.id == data->ownerId) {
            func_1516972C((struct102 *)particle);
        }
    }
}

/* Grow particle update: follows the owner, grows and fades; returns 0 once its lifetime is over. */
s32 func_150D6F0C(Particle *particle, s32 arg1) {
    GrowParticleData *data;
    struct127 *owner;
    f32 size;

    data = &particle->data.grow;
    owner = data->owner;

    if (owner->interaction_state == 0) {
        goto return_zero;
    }

    do {
        if (owner->unique_id == data->ownerId) {
            break;
        }
return_zero:
        return 0;
    } while (0);

    func_150D6C98(owner, &particle->anchor);

    size = sqrtf(data->time) * data->growth;
    particle->unk3C = size;
    particle->unk38 = size;

    particle->unk2B = (u32)(data->fadeStart - ((data->fadeRate * data->time) * data->time));
    data->time = data->time + D_800BE9A4;

    if (data->lifetime < data->time) {
        return 0;
    }
    return 1;
}

/* Grow particle message handler: dies with its owner and follows it through message 0x2D. */
void func_150D7068(Particle *particle, ObjectMessage *msg, u8 type) {
    GrowParticleData *data = &particle->data.grow;
    struct127 *object;

    if (type == 0x2D) {
        if (data->owner == msg->pair.from) {
            data->owner = msg->pair.to;
            data->ownerId = msg->pair.toId;
        } else {
            if (data->owner == msg->pair.to) {
                data->owner = msg->pair.from;
                data->ownerId = msg->pair.fromId;
            }
        trailing_150D7068: // matching: a label here keeps IDO from merging this path's exit branch
            ;
        }
    } else if (type == 0) {
        object = msg->one.object;
        if (object == data->owner || msg->one.id == data->ownerId) {
            func_1516972C((struct102 *)particle);
        }
    }
}
