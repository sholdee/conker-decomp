#include <ultra64.h>
/* functions.h has placeholder prototypes for these (void returns, no args); hide them while it is
 * included and declare the real ones below. */
#define func_15048A40 func_15048A40_hdr
#define func_150DE32C func_150DE32C_hdr
#include "functions.h"
#undef func_15048A40
#undef func_150DE32C
/* variables.h declares D_800D3098 as an array, but the code loads it as a pointer to the table. */
#define D_800D3098 D_800D3098_array_decl_in_variables_h
#include "variables.h"
#undef D_800D3098
extern struct178 *D_800D3098;

typedef struct {
    f32 x, y, z;
} Vec3f;

/* An object with a rotation and a position (func_150DE458's argument; func_151149AC returns one). */
typedef struct {
    /* 0x00 */ f32 rotation[3]; // degrees, wrapped into [0, 360)
    /* 0x0C */ u8  padC[0x4];
    /* 0x10 */ s16 position[3];
    /* 0x16 */ u8  pad16[0x26];
    /* 0x3C */ s32 gravity;
    /* 0x40 */ u8  pad40[0x14];
    /* 0x54 */ u16 kind; // bit 15 is a flag
    /* 0x56 */ u8  pad56[0x4];
    /* 0x5A */ s16 velocity[3];
    /* 0x60 */ f32 spin[3]; // degrees per frame added to rotation
    /* 0x6C */ u8  pad6C[0x2];
    /* 0x6E */ u8  dead;
    /* 0x6F */ u8  pad6F[0x4];
    /* 0x73 */ u8  state; // low two bits == 3 while moving
    /* 0x74 */ u8  pad74[0x8];
    /* 0x7C */ s32 assetSet; // row of D_80090204
    /* 0x80 */ u8  pad80[0xA];
    /* 0x8A */ u8  timer; // frames left before dead is set
} Prop;

extern f32 func_15048A40(u8 angle);
extern s32 func_150A32B4(struct178 *, s32, s32, s32);
extern Prop *func_151149AC(u8);
extern void func_150A2864(s32, s32);
extern s32 D_80090204[][2];
extern s32 func_1510D0EC(s32, s32 *, s32, s32);
extern void func_1510D874(Prop *, s32, s32, s32, s32);

void func_150DE320(s32 arg0) {
}

/* For each of eight D_800D3098 records that func_150A32B4 accepts, sets the moving state of the
 * matching func_151149AC object, or (for records without one) calls func_150A2864(id, 1). */
void func_150DE32C(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    Prop *prop;
    const u8 ids[8] = { 0x18, 0x19, 0x1A, 0x1B, 0x0D, 0x30, 0x31, 0x32 };
    const u8 propIds[8] = { 0xF6, 0xF3, 0xF0, 0xED, 0, 0, 0, 0 };

    for (i = 0; i < 8; i++) {
        if (func_150A32B4(&D_800D3098[ids[i]], arg0, arg1, arg2) != 0) {
            if (propIds[i] != 0) {
                prop = func_151149AC(propIds[i]);
                prop->state = (prop->state & ~3) | 3;
            } else {
                func_150A2864(ids[i], 1);
            }
        }
    }
}

/* Per-frame update: while moving, spins the rotation (wrapping each angle), applies gravity and
 * velocity to the position and counts the timer down to dead; kind 0xC props also get two
 * D_80090204 assets passed to func_1510D874. */
void func_150DE458(Prop *prop) {
    s32 pad[4]; // matching: unused frame space above size
    s32 size;
    f32 frames;
    f32 angle;
    s32 timer;
    s32 addr;
    s32 *asset;
    s32 slot;
    s32 nextSlot;

    if ((prop->state & 3) == 3) {
        frames = D_800BE9E4;

        prop->rotation[0] += prop->spin[0] * frames;
        angle = prop->rotation[0];
        if (angle < 0.0f) {
            prop->rotation[0] = angle + 360.0f;
        } else if (angle >= 360.0f) {
            prop->rotation[0] = angle - 360.0f;
        }

        prop->rotation[1] += prop->spin[1] * frames;
        angle = prop->rotation[1];
        if (angle < 0.0f) {
            prop->rotation[1] = angle + 360.0f;
        } else if (angle >= 360.0f) {
            prop->rotation[1] = angle - 360.0f;
        }

        prop->rotation[2] += prop->spin[2] * frames;
        angle = prop->rotation[2];
        if (angle < 0.0f) {
            prop->rotation[2] = angle + 360.0f;
        } else if (angle >= 360.0f) {
            prop->rotation[2] = angle - 360.0f;
        }

        timer = prop->timer;
        prop->velocity[1] -= prop->gravity * D_800BE9E4;
        prop->position[0] += prop->velocity[0] * D_800BE9E4;
        prop->position[1] += prop->velocity[1] * D_800BE9E4;
        prop->position[2] += prop->velocity[2] * D_800BE9E4;

        timer -= D_800BE9E4;
        if (timer > 0) {
            prop->timer = timer;
        } else {
            prop->dead = 1;
        }
    }

    if ((prop->kind & ~0x8000) == 0xC) {
        // matching: a for (i = 0; i < 2; i++) form (slots i * 2 + 4, i * 2 + 5) grows the frame by 8;
        // the original steps the asset pointer and both slot numbers separately
        asset = D_80090204[prop->assetSet];
        slot = 4;
        nextSlot = 5;
        do {
            addr = func_1510D0EC(*asset, &size, 3, 0);
            func_1510D874(prop, addr, size + addr - 0x200, slot, nextSlot);
            nextSlot += 2;
            asset++;
            slot += 2;
        } while (nextSlot != 9);
    }
}

extern f32 D_800A0D48;
extern f32 D_800A0D4C;
extern f32 D_800A0D50;
extern f32 D_800A0D54;

struct Struct150DE6D8 {
    f32 unk0;
    char pad4[4];
    f32 unk8;
    char padC[6];
    s16 unk12;
    char pad14[0x68];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
};

void func_150DE6D8(struct Struct150DE6D8 *arg0) {
    arg0->unk12 = (s16)(s32)(func_15048A40((u8)(arg0->unk7C >> 3)) * D_800A0D48 + D_800A0D4C);
    arg0->unk0 = func_15048A40((u8)(arg0->unk80 >> 3)) * D_800A0D50;
    arg0->unk8 = func_15048A40((u8)(arg0->unk84 >> 3)) * D_800A0D54;
    arg0->unk7C += D_800BE9E4 * 12;
    arg0->unk80 += D_800BE9E4 * 16;
    arg0->unk84 += D_800BE9E4 * 24;
}

typedef struct { f32 x, y, z; } Vec3f_150DE7C0;

typedef union Game10BC70Pair {
    s32 value[2];
    f32 f[2];
} Game10BC70Pair;

typedef struct Game10BC70SpawnPacket {
    u8 field0;
    s8 field1;
    s16 field2;
    s16 field4;
    u8 pad6[2];
    s32 field8;
    s32 fieldC;
    u8 colour[4];
    f32 scale[2];
    Vec3f_150DE7C0 position;
    Vec3f_150DE7C0 rotation;
    f32 size[3];
    s32 field40;
    u8 field44;
    u8 field45;
    u8 field46;
    u8 field47;
    s32 field48;
    u8 field4C;
    u8 pad4D[3];
    s32 field50;
    s16 field54;
    s16 field56;
} Game10BC70SpawnPacket;

typedef struct Game10BC70Motion {
    Vec3f_150DE7C0 velocity;
    f32 duration;
    Vec3f_150DE7C0 side;
    Vec3f_150DE7C0 forward;
} Game10BC70Motion;

typedef struct Game10BC70Link {
    s16 timer;
    u8 pad2[2];
    s32 source;
    s32 target;
} Game10BC70Link;

typedef struct Game10BC70Emitter {
    u8 pad0;
    u8 flags01;
    u8 pad2[0xA];
    u8 flags0C;
    u8 padD[0x1B];
    Game10BC70Link link;
} Game10BC70Emitter;

void *memcpy(void *, const void *, u32);
void func_1514470C(s32, void *);
s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
extern Game10BC70Pair D_800A0D00;
extern f32 D_800A0D58;

void func_150DE7C0(Game10BC70Emitter *arg0) {
    Game10BC70Link *link;
    Vec3f_150DE7C0 target;
    Vec3f_150DE7C0 delta;
    f32 length;
    f32 unused;
    f32 base;
    Game10BC70Motion motion;
    Game10BC70SpawnPacket packet;
    Game10BC70Pair kinds;
    void *result;
    f32 speed;

    link = &arg0->link;
    link->timer -= D_800BE9E4;
    if (link->timer < 0) {
        kinds = D_800A0D00;
        packet.field0 = *(kinds.value + (func_150ADA20() & 1));
        func_1514470C(link->source, &packet.position);
        func_1514470C(link->target, &target);
        delta.x = target.x - packet.position.x;
        delta.y = target.y - packet.position.y;
        delta.z = target.z - packet.position.z;
        func_15145128(&delta.x, &motion.velocity.x, &length, &unused);
        speed = (func_150ADA68() * 30.0f) + 25.0f;
        base = speed;
        if (packet.field0 == 0xBF) {
            speed = base * D_800A0D58;
        }
        motion.velocity.x *= speed;
        motion.velocity.y *= speed;
        motion.velocity.z *= speed;
        motion.duration = length / speed;
        motion.forward.x = delta.x;
        motion.forward.y = 0.0f;
        motion.forward.z = delta.z;
        func_15145128(&motion.forward.x, &motion.forward.x, 0, 0);
        motion.side.x = -motion.forward.z;
        motion.side.y = 0.0f;
        motion.side.z = motion.forward.x;
        packet.rotation = *(Vec3f_150DE7C0 *)&D_800A5480;
        packet.field1 = 0;
        packet.field2 = 0x3B03;
        packet.field4 = 0x12C;
        packet.field8 = 0;
        packet.fieldC = 0;
        packet.colour[0] = 0xFF;
        packet.colour[1] = 0xFF;
        packet.colour[2] = 0xFF;
        packet.colour[3] = 0xFF;
        func_150ADA68();
        packet.scale[1] = 706.0f;
        packet.scale[0] = 706.0f;
        packet.rotation = *(Vec3f_150DE7C0 *)&D_800A5480;
        packet.field40 = 0x065C0000;
        packet.field44 = 0xFF;
        packet.field45 = 0xFF;
        packet.field46 = 0;
        packet.field47 = 7;
        packet.field48 = 0;
        packet.field4C = 0xFF;
        packet.field50 = 0;
        packet.field54 = 1;
        packet.field56 = 0xFF;
        packet.size[0] = 1.0f;
        packet.size[1] = 1.0f;
        packet.size[2] = 1.0f;
        result = func_1513D2F0((void *)&packet, (s32)&D_800A4AA0, 0x28, 0, 0,
                               0x26, 0, 3, 0xFF, 0x28, arg0->flags0C, arg0->flags01);
        if (result != 0) {
            memcpy((u8 *)result + 0x110, &motion, sizeof(motion));
        }
        link->timer = (func_150ADA20() % 201U) + 0xC8;
    }
}

s32 func_150DEACC(f32 *arg0) {
    arg0[13] += arg0[68] * D_800BE9A4;
    arg0[14] += arg0[69] * D_800BE9A4;
    arg0[15] += arg0[70] * D_800BE9A4;
    arg0[71] -= D_800BE9A4;
    if (arg0[71] < 0.0f) {
        return 0;
    }
    return 1;
}

s32 func_150DEB58(s32 arg0, s16 arg1) {
    if (D_800DBFF0[D_80082FA0].unk388 < 5.0f) {
        return 0;
    }
    return func_15140410(arg0, arg0 + 0x120, arg0 + 0x12C, arg1);
}

extern void func_150DEC28(u8, s32);
extern u8 D_800A0D0B[];
extern u8 D_800A0D2B[];
extern void func_151616D0(u8, u8, s32);
extern void func_151417C4(u8, s32);

void func_150DEBE0(s32 arg0) {
    u8 i;

    for (i = 0; i < 4; i++) {
        func_150DEC28(i, 1);
    }
}

void func_150DEC28(u8 arg0, s32 arg1) {
    s32 idx = arg0 * 4;

    func_151616D0(D_800A0D0B[idx], 0x22, 0);
    func_151417C4(D_800A0D2B[idx], 0x22);
}
