#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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
extern void func_15182670(u8, u8, u8, u8, s32, u8, u8, s32);
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

/* FollowEffect callback; forwards to func_1514933C. */
void func_150D5A2C(struct260 *effect) {
    func_1514933C(effect);
}

/* FollowEffect callback; forwards to func_15149368. */
void func_150D5A4C(struct260 *effect) {
    func_15149368(effect);
}

/* Particle spawn template passed to func_15132A4C. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 scaleXZ;
    /* 0x0C */ f32 scaleY;
    /* 0x10 */ f32 rot[3];
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
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
} PieceSpawn;

extern s32 *D_80089A20[];
extern s32 D_800A3F14[];

s32 func_15134070(struct127 *);
s32 func_15145128(vertex *, vertex *, s32 *, s32 *);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, f32 *, s32 *, s32 *, s32 *, f32 *, s32, s32 *, f32);
s32 func_15144E80(f32 *, vertex *, vertex *, vertex *);
f32 func_15144A74(vertex *, vertex *);
void func_15146078(vertex *, vertex *, vertex *);
void func_15143874(s16, f32, f32 *, f32 *);
s32 func_15132A4C(PieceSpawn *, s32, s32, s32, u8, s32);
void func_151C05A4(vertex *, u8, s32);
void func_151C05F0(vertex *, u8, s32);
void func_151C0360(f32 *, s32 *, u8, s32);
void func_150D6388(vertex *pos, u8 arg1, s32 arg2);
void func_150D6434(vertex *pos, u8 arg1, s32 arg2);
void func_150D65F0(f32 *pos, s32 *arg1, u8 arg2, s32 arg3);
void func_150D66A4(u8 arg0, u8 arg1, s32 arg2);

/*
 * Breakup effect for obj: plays sounds, then spawns pieces (three fixed models, then
 * D_800A3F14[type] - 2 type-specific ones) flying off the surface obj's last move hit,
 * plus impact and dust effects.
 */
void func_150D5A6C(struct127 *obj, u8 arg1, s32 arg2) {
    vertex pos;
    vertex vel;
    s32 hitOut[3];
    u8 hit;
    s32 useDefaultBasis;
    f32 hitPos[3];
    s32 ignored0;
    s32 ignored1;
    s32 hitSurface;
    s32 type;
    f32 dist;
    vertex normal;
    vertex tangentU;
    vertex tangentV;
    s32 moving;
    PieceSpawn piece;

    hit = 0;
    pos.x = obj->x_position;
    pos.y = obj->y_position + 50.0f;
    pos.z = obj->z_position;
    vel.x = obj->x_position - obj->old_x_position;
    vel.y = obj->y_position - obj->old_y_position;
    vel.z = obj->z_position - obj->old_z_position;
    type = func_15134070(obj);
    if (type == 99) {
        return;
    }
    if ((obj->unk184 & 0x1F) == 0xE) {
        func_10010630(0x434, obj, 32000, 700, 2000);
    } else {
        func_10010630(0x4C8, obj, -32000, 700, 2000);
    }
    func_10010630((func_150ADA20() & 1) + 0x2B6, obj, 32000, 1000, 3000);
    func_10010630((func_150ADA20() & 3) + 0x2B1, obj, 32000, 700, 2000);
    func_151C05A4(&pos, arg1, arg2);
    func_151C05F0(&pos, arg1, arg2);
    func_15145A50(obj);
    if (obj->unk1D4 == NULL) {
        return;
    }
    if ((obj->unk74 & 0xF) == 0xF) {
        return;
    }
    moving = func_15145128(&vel, &vel, &ignored0, &ignored1) ? 1 : 0; // matching: != 0 and a plain store differ
    if (moving) {
        typedef s32 ScopeBarrier1; // matching: -g3 scope pins the 0.0f register setup at block entry

        if (func_150AC9C0(obj->old_x_position, obj->old_y_position, obj->old_z_position, vel.x, vel.y, vel.z, 0,
                          hitPos, &hitOut[0], &hitOut[1], &hitOut[2], &dist, 0, &hitSurface, 0.0f) &&
            dist < 397.0f) {
            hit = 1;
        }
    }
    {
        typedef s32 ScopeBarrier2; // matching: -g3 scope pins the 0.0f register setup at block entry

        useDefaultBasis = 1;
        if (hit) {
            useDefaultBasis = 0;
            if (func_15144E80(hitPos, &tangentU, &tangentV, &normal)) {
                s32 ignored2;
                s32 ignored3;
                s32 ignored4;
                s32 ignored5;
                s32 ignored6;
                s32 ignored7;

                func_15145128(&tangentU, &tangentU, &ignored2, &ignored3);
                func_15145128(&tangentV, &tangentV, &ignored4, &ignored5);
                func_15145128(&normal, &normal, &ignored6, &ignored7);
                if (func_15144A74(&normal, &vel) > 0.0f) {
                    normal.x = -normal.x;
                    normal.y = -normal.y;
                    normal.z = -normal.z;
                }
            } else {
                useDefaultBasis = 1;
            }
        }
        if (useDefaultBasis) {
            if (moving) {
                normal.x = -vel.x;
                normal.y = -vel.y;
                normal.z = -vel.z;
                func_15146078(&normal, &tangentU, &tangentV);
            } else {
                normal.x = 0.0f;
                normal.y = 1.0f;
                normal.z = 0.0f;
                tangentU.x = 1.0f;
                tangentU.y = 0.0f;
                tangentU.z = 0.0f;
                tangentV.x = 0.0f;
                tangentV.y = 0.0f;
                tangentV.z = 1.0f;
            }
        }
        piece.unk0 = 1.0f;
        piece.unk4 = 1.0f;
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        piece.scaleXZ = obj->xz_scale;
        piece.scaleY = obj->y_scale;
        piece.unk1C[0] = 1.0f;
        piece.unk1C[1] = 1.0f;
        piece.unk1C[2] = 1.0f;
        piece.pos = pos;
        piece.unk50 = 0x21E8;
        piece.unk58 = 0;
        piece.unk5C = 0;
        piece.unk60 = 0xFF;
        piece.unk61 = 8;
        piece.unk62 = 0;
        piece.unk63 = 0;
        piece.unk64 = 0;
        piece.unk65 = 0;
        piece.unk66 = 0;
        piece.unk67 = 0;
        piece.unk68 = 2;
        piece.unk6A = 1;
        piece.unk6C = obj;
        piece.unk70 = obj->unique_id;
        piece.unk78 = 0;
        piece.unk44 = 0.0f;
        {
            const s32 firstModels[3] = { 0x6C, 0x6D, 0x6E };
            s32 i;
            f32 offsetV;
            f32 offsetU;
            f32 scale;
            s32 randAngle;
            f32 offsetV2;
            f32 offsetU2;
            s32 j;

            piece.unk72 = 12;
            piece.unk74 = 21;
            for (i = 0; i < 3; i++) {
                scale = func_150ADA68() * 0.01319999993f + 0.02979999967f;
                randAngle = func_150ADA20();
                func_15143874((u8)randAngle, func_150ADA68() * 759.0f, &offsetU, &offsetV);
                piece.vel.x = (tangentU.x * offsetU + tangentV.x * offsetV + normal.x) * scale;
                piece.vel.y = (tangentU.y * offsetU + tangentV.y * offsetV + normal.y) * scale;
                piece.vel.z = (tangentU.z * offsetU + tangentV.z * offsetV + normal.z) * scale;
                piece.rot[0] = func_150ADA68() * 360.0f;
                piece.rot[1] = func_150ADA68() * 360.0f;
                piece.rot[2] = func_150ADA68() * 360.0f;
                piece.unk40 = func_150ADA68() * 38.44400024f + -19.22200012f;
                piece.unk48 = func_150ADA68() * 38.44400024f + -19.22200012f;
                piece.unk4C = func_150ADA68() * 0.6480000019f + -1.397000074f;
                piece.unk54 = func_150ADA20() % 21U + 30;
                piece.unk56 = firstModels[i];
                func_15132A4C(&piece, 0, 0, 0, arg1, arg2);
            }
            piece.unk72 = 12;
            piece.unk74 = 21;
            for (j = 2; j < D_800A3F14[type]; j++) {
                scale = func_150ADA68() * 0.01999999955f + 0.005200000014f;
                randAngle = func_150ADA20();
                func_15143874((u8)randAngle, func_150ADA68() * 723.0f, &offsetU2, &offsetV2);
                piece.vel.x = (tangentU.x * offsetU2 + tangentV.x * offsetV2 + normal.x) * scale;
                piece.vel.y = (tangentU.y * offsetU2 + tangentV.y * offsetV2 + normal.y) * scale;
                piece.vel.z = (tangentU.z * offsetU2 + tangentV.z * offsetV2 + normal.z) * scale;
                piece.rot[0] = func_150ADA68() * 360.0f;
                piece.rot[1] = func_150ADA68() * 360.0f;
                piece.rot[2] = func_150ADA68() * 360.0f;
                piece.unk40 = func_150ADA68() * 40.45000076f + -20.22500038f;
                piece.unk48 = func_150ADA68() * 40.45000076f + -20.22500038f;
                piece.unk4C = func_150ADA68() * 0.6260000467f + -1.089000106f;
                piece.unk54 = func_150ADA20() % 41U + 40;
                piece.unk56 = D_80089A20[type][j];
                func_15132A4C(&piece, 0, 0, 0, arg1, arg2);
            }
        }
        if (obj->camera != NULL && D_80082FA0 <= 0) {
            func_150D66A4(obj->camera->unk23D, arg1, arg2);
        }
        if (hit && obj->camera != NULL) {
            if ((hitSurface & 0x1F) == 0xE) {
                func_150D65F0(hitPos, hitOut, arg1, arg2);
            } else {
                func_151C0360(hitPos, hitOut, arg1, arg2);
            }
        }
        func_150D6434(&pos, arg1, arg2);
        func_150D6388(&pos, arg1, arg2);
    }
}

void func_151541B8(vertex *, f32, f32, f32, f32, u8, s32);

/* Spawns a func_151541B8 effect at pos with a random size and a 200-255 fourth argument. */
void func_150D6388(vertex *pos, u8 arg1, s32 arg2) {
    u32 randInt;
    f32 randFrac;

    randFrac = func_150ADA68();
    randInt = func_150ADA20();
    func_151541B8(pos, randFrac * 4.0f + 15.0f, 1.374570012f, randInt % 56 + 200, 0.0f, arg1, arg2);
}

/* Spawns a func_15154884 effect at pos with randomized parameters. */
void func_150D6434(vertex *pos, u8 arg1, s32 arg2) {
    f32 rand3;
    f32 rand2;
    f32 rand1;

    rand1 = func_150ADA68();
    rand2 = func_150ADA68();
    rand3 = func_150ADA68();
    func_15154884(pos, rand1 * 3.0f + 8.0f, rand2 * 0.4030000269f + 1.305000067f, rand3 * 50.0f + 100.0f, arg1, arg2);
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

/* Fills in fixed settings; unk50 is -1 when D_80082FA0 >= 2 or D_8008FD8C >= 8, else 0. */
void func_150D64E8(Struct150D64E8 *info, f32 *outValue) {
    info->unk00 = 6;
    info->unk04 = 3;
    *outValue = 1263.0f;
    info->unk14 = 0.08999999613f;
    info->unk18 = 0.03029999882f;
    info->unk1C = 150.0f;
    info->unk20 = 105.0f;
    info->unk24 = 396.0f;
    info->unk28 = 612.0f;
    info->unk2C = 4;
    info->unk30 = 3;
    info->unk34 = 25.0f;
    info->unk38 = 0.8120000362f;
    info->unk3C = -2.0f;
    info->unk40 = 1.618000031f;
    info->unk44 = 20;
    info->unk46 = 30;
    info->unk48 = 100;
    info->unk4A = 100;
    info->unk4C = 12;
    info->unk4E = 20;

    if (D_80082FA0 >= 2 || D_8008FD8C >= 8) {
        info->unk50 = -1;
    } else {
        info->unk50 = 0;
    }
}

void func_151A5D58(f32, u8, f32 *, s32 *, s32, s32, s32, u8, s32);

/* Impact effect on a type-0xE surface hit at pos (func_151A5D58 with randomized parameters). */
void func_150D65F0(f32 *pos, s32 *arg1, u8 arg2, s32 arg3) {
    u32 rand3;
    u32 rand2;
    f32 rand1;

    rand1 = func_150ADA68();
    rand2 = func_150ADA20();
    rand3 = func_150ADA20();
    func_151A5D58(rand1 * 100.0f + 150.0f, rand2 % 71 + 130, pos, arg1, rand3 % 31 + 50, 0, 1, arg2, arg3);
}

/* Calls func_15182670 for camera arg0 with 0xFF, 0xFF, 0xFF, a random 200-255 and a random 25-30. */
void func_150D66A4(u8 arg0, u8 arg1, s32 arg2) {
    u32 rand2;
    u32 rand1;

    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    func_15182670(0xFF, 0xFF, 0xFF, rand1 % 56 + 200, rand2 % 6 + 25, arg0, arg1, arg2);
}

void func_150D6C98(struct127 *obj, vertex *out);
extern void func_15143794(s16, s16, f32, vertex *);

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
    spawn.unk1E = 15;
    spawn.unk20 = 17;
    spawn.flags = 0x84DE01;
    spawn.unk60 = 8;
    spawn.unk61 = 6;
    spawn.unk62 = 30;
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
        func_15143794(r1 & 0xFF, r2 % 36U - 25, func_150ADA68() * 30.0f + 25.0f, &spawn.vel);
        spawn.unk54 = func_150ADA68() * (f32)0; // matching: keeps the multiply that a 0.0f literal folds away
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
        s32 pad; // matching: an unused slot in a separate scope keeps the frame layout

        grow.owner = obj;
        grow.ownerId = obj->unique_id;
        grow.time = 0.0f;
        grow.growth = 2392.159912f;
        grow.lifetime = func_150ADA68() * 3.0f + 9.0f;
        grow.fadeStart = func_150ADA68() * 80.0f + 100.0f;
        grow.fadeRate = grow.fadeStart / (grow.lifetime * grow.lifetime);
        growSpawn.unk1B = grow.fadeStart;
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
        growSpawn.unk62 = 31;
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

    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        return 0;
    }

    func_150D6C98(owner, &particle->anchor);

    size = sqrtf(data->time) * data->growth;
    particle->unk3C = size;
    particle->unk38 = size;

    particle->unk2B = data->fadeStart - data->fadeRate * data->time * data->time;
    data->time += D_800BE9A4;

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
